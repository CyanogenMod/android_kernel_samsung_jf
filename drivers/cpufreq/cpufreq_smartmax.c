/*
 * drivers/cpufreq/cpufreq_smartmax.c
 *
 * Copyright (C) 2013 maxwen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Author: maxwen
 *
 * Based on the ondemand and smartassV2 governor
 *
 * ondemand:
 *  Copyright (C)  2001 Russell King
 *            (C)  2003 Venkatesh Pallipadi <venkatesh.pallipadi@intel.com>.
 *                      Jun Nakajima <jun.nakajima@intel.com>
 *
 * smartassV2:
 * Author: Erasmux
 *
 * For a general overview of CPU governors see the relavent part in
 * Documentation/cpu-freq/governors.txt
 *
 */

#include <linux/module.h>
#include <linux/cpu.h>
#include <linux/cpumask.h>
#include <linux/cpufreq.h>
#include <linux/sched.h>
#include <linux/tick.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/moduleparam.h>
#include <linux/jiffies.h>
#include <linux/earlysuspend.h>
#include <linux/input.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/kernel_stat.h>

#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif

/******************** Tunable parameters: ********************/

/*
 * The "ideal" frequency to use. The governor will ramp up faster
 * towards the ideal frequency and slower after it has passed it. Similarly,
 * lowering the frequency towards the ideal frequency is faster than below it.
 */

#define GOV_IDLE_FREQ 475000

#define DEFAULT_SUSPEND_IDEAL_FREQ GOV_IDLE_FREQ
static unsigned int suspend_ideal_freq;

#define DEFAULT_AWAKE_IDEAL_FREQ GOV_IDLE_FREQ
static unsigned int awake_ideal_freq;

/*
 * Freqeuncy delta when ramping up above the ideal freqeuncy.
 * Zero disables and causes to always jump straight to max frequency.
 * When below the ideal freqeuncy we always ramp up to the ideal freq.
 */
#define DEFAULT_RAMP_UP_STEP 300000
static unsigned int ramp_up_step;

/*
 * Freqeuncy delta when ramping down below the ideal freqeuncy.
 * Zero disables and will calculate ramp down according to load heuristic.
 * When above the ideal freqeuncy we always ramp down to the ideal freq.
 */
#define DEFAULT_RAMP_DOWN_STEP 150000
static unsigned int ramp_down_step;

/*
 * CPU freq will be increased if measured load > max_cpu_load;
 */
#define DEFAULT_MAX_CPU_LOAD 80
static unsigned int max_cpu_load;

/*
 * CPU freq will be decreased if measured load < min_cpu_load;
 */
#define DEFAULT_MIN_CPU_LOAD 50
static unsigned int min_cpu_load;

/*
 * The minimum amount of time in nsecs to spend at a frequency before we can ramp up.
 * Notice we ignore this when we are below the ideal frequency.
 */
#define DEFAULT_UP_RATE 40000
static unsigned int up_rate;

/*
 * The minimum amount of time in nsecs to spend at a frequency before we can ramp down.
 * Notice we ignore this when we are above the ideal frequency.
 */
#define DEFAULT_DOWN_RATE 80000
static unsigned int down_rate;

/* in nsecs */
#define DEFAULT_SAMPLING_RATE 40000
static unsigned int sampling_rate;

/* in nsecs */
#define DEFAULT_INPUT_BOOST_DURATION 50000000
static unsigned int input_boost_duration;

static unsigned int touch_poke_freq = 760000;
static bool touch_poke = true;

/*
 * should ramp_up steps during boost be possible
 */
static bool ramp_up_during_boost = true;

/*
 * external boost interface - boost if duration is written
 * to sysfs for boost_duration
 */
static unsigned int boost_freq = 760000;
static bool boost = true;

/* in nsecs */
static unsigned int boost_duration = 0;

/* Consider IO as busy */
#define DEFAULT_IO_IS_BUSY 1
static unsigned int io_is_busy;

#define DEFAULT_IGNORE_NICE 1
static unsigned int ignore_nice;

/*************** End of tunables ***************/

static unsigned int dbs_enable; /* number of CPUs using this policy */

static void do_dbs_timer(struct work_struct *work);

struct smartmax_info_s {
	struct cpufreq_policy *cur_policy;
	struct cpufreq_frequency_table *freq_table;
	struct delayed_work work;
	cputime64_t prev_cpu_idle;
	cputime64_t prev_cpu_iowait;
	cputime64_t prev_cpu_wall;
	cputime64_t prev_cpu_nice;
	cputime64_t freq_change_time;
	unsigned int cur_cpu_load;
	unsigned int old_freq;
	int ramp_dir;
	bool enable;
	unsigned int ideal_speed;
	unsigned int cpu;
	struct mutex timer_mutex;
};
static DEFINE_PER_CPU(struct smartmax_info_s, smartmax_info);

#define SMARTMAX_DEBUG 0

#if SMARTMAX_DEBUG
#define dprintk(flag,msg...) do { \
	if (debug_mask & flag) printk(KERN_DEBUG "[smartmax]" ":" msg); \
	} while (0)
#else
#define dprintk(flag,msg...)
#endif

enum {
	SMARTMAX_DEBUG_JUMPS = 1,
	SMARTMAX_DEBUG_LOAD = 2,
	SMARTMAX_DEBUG_ALG = 4,
	SMARTMAX_DEBUG_BOOST = 8,
	SMARTMAX_DEBUG_INPUT = 16,
	SMARTMAX_DEBUG_SUSPEND = 32
};

/*
 * Combination of the above debug flags.
 */
#if SMARTMAX_DEBUG
static unsigned long debug_mask = SMARTMAX_DEBUG_LOAD|SMARTMAX_DEBUG_JUMPS|SMARTMAX_DEBUG_ALG|SMARTMAX_DEBUG_BOOST|SMARTMAX_DEBUG_INPUT|SMARTMAX_DEBUG_SUSPEND;
#else
static unsigned long debug_mask;
#endif

/*
 * dbs_mutex protects dbs_enable in governor start/stop.
 */
static DEFINE_MUTEX(dbs_mutex);

extern int tegra_input_boost(int cpu, unsigned int target_freq);

static bool boost_task_alive = false;
static struct task_struct *boost_task;
static cputime64_t boost_end_time = 0ULL;
static unsigned int cur_boost_freq = 0;
static unsigned int cur_boost_duration = 0;
static bool boost_running = false;
static unsigned int ideal_freq;
static bool is_suspended = false;

#ifdef CONFIG_HAS_EARLYSUSPEND
static struct early_suspend smartmax_early_suspend_handler;
#endif

#define LATENCY_MULTIPLIER			(1000)
#define MIN_LATENCY_MULTIPLIER			(100)

static int cpufreq_governor_smartmax(struct cpufreq_policy *policy,
		unsigned int event);

#ifndef CONFIG_CPU_FREQ_DEFAULT_GOV_SMARTMAX
static
#endif
struct cpufreq_governor cpufreq_gov_smartmax = { .name = "smartmax", .governor =
		cpufreq_governor_smartmax, .max_transition_latency = 9000000, .owner =
		THIS_MODULE , };

static inline cputime64_t get_cpu_idle_time_jiffy(unsigned int cpu,
		cputime64_t *wall) {
	u64 idle_time;
	u64 cur_wall_time;
	u64 busy_time;

	cur_wall_time = jiffies64_to_cputime64(get_jiffies_64());

	busy_time  = kcpustat_cpu(cpu).cpustat[CPUTIME_USER];
	busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_SYSTEM];
	busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_IRQ];
	busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_SOFTIRQ];
	busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_STEAL];
	busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_NICE];

	idle_time = cur_wall_time - busy_time;
	if (wall)
		*wall = jiffies_to_usecs(cur_wall_time);

	return jiffies_to_usecs(idle_time);
}

static inline cputime64_t get_cpu_idle_time(unsigned int cpu, cputime64_t *wall) {
	u64 idle_time = get_cpu_idle_time_us(cpu, wall);

	if (idle_time == -1ULL)
		return get_cpu_idle_time_jiffy(cpu, wall);

	return idle_time;
}

static inline cputime64_t get_cpu_iowait_time(unsigned int cpu,
		cputime64_t *wall) {
	u64 iowait_time = get_cpu_iowait_time_us(cpu, wall);

	if (iowait_time == -1ULL)
		return 0;

	return iowait_time;
}

inline static void smartmax_update_min_max(
		struct smartmax_info_s *this_smartmax, struct cpufreq_policy *policy) {
	this_smartmax->ideal_speed = // ideal_freq; but make sure it obeys the policy min/max
			policy->min < ideal_freq ?
					(ideal_freq < policy->max ? ideal_freq : policy->max) :
					policy->min;

}

inline static void smartmax_update_min_max_allcpus(void) {
	unsigned int i;

	for_each_online_cpu(i)
	{
		struct smartmax_info_s *this_smartmax = &per_cpu(smartmax_info, i);
		if (this_smartmax->enable)
			smartmax_update_min_max(this_smartmax, this_smartmax->cur_policy);
	}
}

inline static unsigned int validate_freq(struct cpufreq_policy *policy,
		int freq) {
	if (freq > (int) policy->max)
		return policy->max;
	if (freq < (int) policy->min)
		return policy->min;
	return freq;
}

/* We want all CPUs to do sampling nearly on same jiffy */
static inline unsigned int get_timer_delay(void) {
	unsigned int delay = usecs_to_jiffies(sampling_rate);

	if (num_online_cpus() > 1)
		delay -= jiffies % delay;
	return delay;
}

static inline void dbs_timer_init(struct smartmax_info_s *this_smartmax) {
	int delay = get_timer_delay();

	INIT_DELAYED_WORK_DEFERRABLE(&this_smartmax->work, do_dbs_timer);
	schedule_delayed_work_on(this_smartmax->cpu, &this_smartmax->work, delay);
}

static inline void dbs_timer_exit(struct smartmax_info_s *this_smartmax) {
	cancel_delayed_work_sync(&this_smartmax->work);
}

inline static void target_freq(struct cpufreq_policy *policy,
		struct smartmax_info_s *this_smartmax, int new_freq, int old_freq,
		int prefered_relation) {
	int index, target;
	struct cpufreq_frequency_table *table = this_smartmax->freq_table;
#if SMARTMAX_DEBUG
	unsigned int cpu = this_smartmax->cpu;
#endif

	dprintk(SMARTMAX_DEBUG_ALG, "%d: %s\n", old_freq, __func__);

	if (new_freq == old_freq)
		return;
	new_freq = validate_freq(policy, new_freq);
	if (new_freq == old_freq)
		return;

	if (table
			&& !cpufreq_frequency_table_target(policy, table, new_freq,
					prefered_relation, &index)) {
		target = table[index].frequency;
		if (target == old_freq) {
			// if for example we are ramping up to *at most* current + ramp_up_step
			// but there is no such frequency higher than the current, try also
			// to ramp up to *at least* current + ramp_up_step.
			if (new_freq > old_freq && prefered_relation == CPUFREQ_RELATION_H
					&& !cpufreq_frequency_table_target(policy, table, new_freq,
							CPUFREQ_RELATION_C, &index))
				target = table[index].frequency;
			// simlarly for ramping down:
			else if (new_freq < old_freq
					&& prefered_relation == CPUFREQ_RELATION_L
					&& !cpufreq_frequency_table_target(policy, table, new_freq,
							CPUFREQ_RELATION_H, &index))
				target = table[index].frequency;
		}

		if (target == old_freq) {
			// We should not get here:
			// If we got here we tried to change to a validated new_freq which is different
			// from old_freq, so there is no reason for us to remain at same frequency.
			dprintk(SMARTMAX_DEBUG_ALG, "%d: frequency change failed to %d (%d)\n",
					old_freq, new_freq, target);
			return;
		}
	} else
		target = new_freq;

	dprintk(SMARTMAX_DEBUG_JUMPS, "%d: jumping to %d (%d) cpu %d\n", old_freq, new_freq, target, cpu);

	__cpufreq_driver_target(policy, target, prefered_relation);

	// remember last time we changed frequency
	this_smartmax->freq_change_time = ktime_to_ns(ktime_get());
}

/* We use the same work function to sale up and down */
static void cpufreq_smartmax_freq_change(struct smartmax_info_s *this_smartmax) {
	unsigned int cpu;
	unsigned int new_freq = 0;
	unsigned int old_freq;
	int ramp_dir;
	struct cpufreq_policy *policy;
	unsigned int relation = CPUFREQ_RELATION_L;

	ramp_dir = this_smartmax->ramp_dir;
	old_freq = this_smartmax->old_freq;
	policy = this_smartmax->cur_policy;
	cpu = this_smartmax->cpu;

	dprintk(SMARTMAX_DEBUG_ALG, "%d: %s\n", old_freq, __func__);
	
	if (old_freq != policy->cur) {
		// frequency was changed by someone else?
		dprintk(SMARTMAX_DEBUG_ALG, "%d: frequency changed by 3rd party to %d\n",
				old_freq, policy->cur);
		new_freq = old_freq;
	} else if (ramp_dir > 0 && nr_running() > 1) {
		// ramp up logic:
		if (old_freq < this_smartmax->ideal_speed)
			new_freq = this_smartmax->ideal_speed;
		else if (ramp_up_step) {
			new_freq = old_freq + ramp_up_step;
			relation = CPUFREQ_RELATION_H;
		} else {
			new_freq = policy->max;
			relation = CPUFREQ_RELATION_H;
		}
	} else if (ramp_dir < 0) {
		// ramp down logic:
		if (old_freq > this_smartmax->ideal_speed) {
			new_freq = this_smartmax->ideal_speed;
			relation = CPUFREQ_RELATION_H;
		} else if (ramp_down_step)
			new_freq = old_freq - ramp_down_step;
		else {
			// Load heuristics: Adjust new_freq such that, assuming a linear
			// scaling of load vs. frequency, the load in the new frequency
			// will be max_cpu_load:
			new_freq = old_freq * this_smartmax->cur_cpu_load / max_cpu_load;
			if (new_freq > old_freq) // min_cpu_load > max_cpu_load ?!
				new_freq = old_freq - 1;
		}
	}

	if (new_freq!=0){
		target_freq(policy, this_smartmax, new_freq, old_freq, relation);
	}
	
	this_smartmax->ramp_dir = 0;
}

static inline void cpufreq_smartmax_get_ramp_direction(unsigned int debug_load, unsigned int cur, struct smartmax_info_s *this_smartmax, struct cpufreq_policy *policy, cputime64_t now)
{
	// Scale up if load is above max or if there where no idle cycles since coming out of idle,
	// additionally, if we are at or above the ideal_speed, verify we have been at this frequency
	// for at least up_rate:
	if (debug_load > max_cpu_load && cur < policy->max
			&& (cur < this_smartmax->ideal_speed
				|| (now - this_smartmax->freq_change_time) >= up_rate)) {
		dprintk(SMARTMAX_DEBUG_ALG,
				"%d: ramp up: load %d\n", cur, debug_load);
		this_smartmax->ramp_dir = 1;
	}
	// Similarly for scale down: load should be below min and if we are at or below ideal
	// frequency we require that we have been at this frequency for at least down_rate:
	else if (debug_load < min_cpu_load && cur > policy->min
			&& (cur > this_smartmax->ideal_speed
				|| (now - this_smartmax->freq_change_time) >= down_rate)) {
		dprintk(SMARTMAX_DEBUG_ALG,
				"%d: ramp down: load %d\n", cur, debug_load);
		this_smartmax->ramp_dir = -1;
	}
}

static void cpufreq_smartmax_timer(struct smartmax_info_s *this_smartmax) {
	unsigned int cur;
	struct cpufreq_policy *policy = this_smartmax->cur_policy;
	cputime64_t now = ktime_to_ns(ktime_get());
	unsigned int max_load_freq;
	unsigned int debug_load = 0;
	unsigned int debug_iowait = 0;
	unsigned int j = 0;
#if SMARTMAX_DEBUG
	unsigned int cpu = this_smartmax->cpu;
#endif

	cur = policy->cur;
		
	dprintk(SMARTMAX_DEBUG_ALG, "%d: %s cpu %d %lld\n", cur, __func__, cpu, now);


	/* Get Absolute Load - in terms of freq */
	max_load_freq = 0;

	for_each_cpu(j, policy->cpus)
	{
		struct smartmax_info_s *j_this_smartmax;
		cputime64_t cur_wall_time, cur_idle_time, cur_iowait_time;
		unsigned int idle_time, wall_time, iowait_time;
		unsigned int load, load_freq;
		int freq_avg;

		j_this_smartmax = &per_cpu(smartmax_info, j);

		cur_idle_time = get_cpu_idle_time(j, &cur_wall_time);
		cur_iowait_time = get_cpu_iowait_time(j, &cur_wall_time);

		wall_time = cur_wall_time - j_this_smartmax->prev_cpu_wall;
		j_this_smartmax->prev_cpu_wall = cur_wall_time;

		idle_time = cur_idle_time - j_this_smartmax->prev_cpu_idle;
		j_this_smartmax->prev_cpu_idle = cur_idle_time;

		iowait_time = cur_iowait_time - j_this_smartmax->prev_cpu_iowait;
		j_this_smartmax->prev_cpu_iowait = cur_iowait_time;

		if (ignore_nice) {
			cputime64_t cur_nice;
			unsigned long cur_nice_jiffies;

			cur_nice = kcpustat_cpu(j).cpustat[CPUTIME_NICE] -
					 j_this_smartmax->prev_cpu_nice;
			cur_nice_jiffies = (unsigned long)
					cputime64_to_jiffies64(cur_nice);

			j_this_smartmax->prev_cpu_nice = kcpustat_cpu(j).cpustat[CPUTIME_NICE];
			idle_time += jiffies_to_usecs(cur_nice_jiffies);
		}

		/*
		 * For the purpose of ondemand, waiting for disk IO is an
		 * indication that you're performance critical, and not that
		 * the system is actually idle. So subtract the iowait time
		 * from the cpu idle time.
		 */

		if (io_is_busy && idle_time >= iowait_time)
			idle_time -= iowait_time;

		if (idle_time >= iowait_time)
			idle_time -= iowait_time;

		if (unlikely(!wall_time || wall_time < idle_time))
			continue;

		load = 100 * (wall_time - idle_time) / wall_time;

		freq_avg = __cpufreq_driver_getavg(policy, j);
		if (freq_avg <= 0)
			freq_avg = policy->cur;

		load_freq = load * freq_avg;
		if (load_freq > max_load_freq) {
			max_load_freq = load_freq;
			debug_load = load;
			debug_iowait = 100 * iowait_time / wall_time;
		}
	}

	dprintk(SMARTMAX_DEBUG_LOAD, "%d: load %d\n", cur, debug_load);

	this_smartmax->cur_cpu_load = debug_load;
	this_smartmax->old_freq = cur;
	this_smartmax->ramp_dir = 0;

	cpufreq_smartmax_get_ramp_direction(debug_load, cur, this_smartmax, policy, now);

	// no changes
	if (this_smartmax->ramp_dir == 0)		
		return;

	// boost - but not block ramp up steps based on load if requested
	if (boost_running && time_before64 (now, boost_end_time)) {
		dprintk(SMARTMAX_DEBUG_BOOST, "%d: boost running %llu %llu\n", cur, now, boost_end_time);
		
		if (this_smartmax->ramp_dir == -1)
			return;
		else {
			if (ramp_up_during_boost)
				dprintk(SMARTMAX_DEBUG_BOOST, "%d: boost running but ramp_up above boost freq requested\n", cur);
			else
				return;
		}
	} else
		boost_running = false;

	cpufreq_smartmax_freq_change(this_smartmax);
}

static void do_dbs_timer(struct work_struct *work) {
	struct smartmax_info_s *this_smartmax =
			container_of(work, struct smartmax_info_s, work.work);
	unsigned int cpu = this_smartmax->cpu;
	int delay = get_timer_delay();

	mutex_lock(&this_smartmax->timer_mutex);

	cpufreq_smartmax_timer(this_smartmax);

	schedule_delayed_work_on(cpu, &this_smartmax->work, delay);
	mutex_unlock(&this_smartmax->timer_mutex);
}

static void update_idle_time(bool online) {
	int j = 0;

	for_each_possible_cpu(j)
	{
		struct smartmax_info_s *j_this_smartmax;

		if (online && !cpu_online(j)) {
			continue;
		}
		j_this_smartmax = &per_cpu(smartmax_info, j);

		j_this_smartmax->prev_cpu_idle = get_cpu_idle_time(j,
				&j_this_smartmax->prev_cpu_wall);
		if (ignore_nice)
			j_this_smartmax->prev_cpu_nice = kcpustat_cpu(j).cpustat[CPUTIME_NICE];

	}
}

static ssize_t show_debug_mask(struct kobject *kobj, struct attribute *attr,
		char *buf) {
	return sprintf(buf, "%lu\n", debug_mask);
}

static ssize_t store_debug_mask(struct kobject *kobj, struct attribute *attr,
		const char *buf, size_t count) {
	ssize_t res;
	unsigned long input;
	res = strict_strtoul(buf, 0, &input);
	if (res >= 0)
		debug_mask = input;
	else
		return -EINVAL;
	return count;
}

static ssize_t show_up_rate(struct kobject *kobj, struct attribute *attr,
		char *buf) {
	return sprintf(buf, "%u\n", up_rate);
}

static ssize_t store_up_rate(struct kobject *kobj, struct attribute *attr,
		const char *buf, size_t count) {
	ssize_t res;
	unsigned long input;
	res = strict_strtoul(buf, 0, &input);
	if (res >= 0 && input >= 0 && input <= 100000000)
		up_rate = input;
	else
		return -EINVAL;
	return count;
}

static ssize_t show_down_rate(struct kobject *kobj, struct attribute *attr,
		char *buf) {
	return sprintf(buf, "%u\n", down_rate);
}

static ssize_t store_down_rate(struct kobject *kobj, struct attribute *attr,
		const char *buf, size_t count) {
	ssize_t res;
	unsigned long input;
	res = strict_strtoul(buf, 0, &input);
	if (res >= 0 && input >= 0 && input <= 100000000)
		down_rate = input;
	else
		return -EINVAL;
	return count;
}

static ssize_t show_awake_ideal_freq(struct kobject *kobj, struct attribute *attr,
		char *buf) {
	return sprintf(buf, "%u\n", awake_ideal_freq);
}

static ssize_t store_awake_ideal_freq(struct kobject *kobj, struct attribute *attr,
		const char *buf, size_t count) {
	ssize_t res;
	unsigned long input;
	res = strict_strtoul(buf, 0, &input);
	if (res >= 0 && input >= 0) {
		awake_ideal_freq = input;
		if (!is_suspended){
			ideal_freq = awake_ideal_freq;
			smartmax_update_min_max_allcpus();
		}
	} else
		return -EINVAL;
	return count;
}

static ssize_t show_suspend_ideal_freq(struct kobject *kobj, struct attribute *attr,
		char *buf) {
	return sprintf(buf, "%u\n", suspend_ideal_freq);
}

static ssize_t store_suspend_ideal_freq(struct kobject *kobj, struct attribute *attr,
		const char *buf, size_t count) {
	ssize_t res;
	unsigned long input;
	res = strict_strtoul(buf, 0, &input);
	if (res >= 0 && input >= 0) {
		suspend_ideal_freq = input;
		if (is_suspended){
			ideal_freq = suspend_ideal_freq;
			smartmax_update_min_max_allcpus();
		}
	} else
		return -EINVAL;
	return count;
}

static ssize_t show_ramp_up_step(struct kobject *kobj, struct attribute *attr,
		char *buf) {
	return sprintf(buf, "%u\n", ramp_up_step);
}

static ssize_t store_ramp_up_step(struct kobject *kobj, struct attribute *attr,
		const char *buf, size_t count) {
	ssize_t res;
	unsigned long input;
	res = strict_strtoul(buf, 0, &input);
	if (res >= 0 && input >= 0)
		ramp_up_step = input;
	else
		return -EINVAL;
	return count;
}

static ssize_t show_ramp_down_step(struct kobject *kobj, struct attribute *attr,
		char *buf) {
	return sprintf(buf, "%u\n", ramp_down_step);
}

static ssize_t store_ramp_down_step(struct kobject *kobj,
		struct attribute *attr, const char *buf, size_t count) {
	ssize_t res;
	unsigned long input;
	res = strict_strtoul(buf, 0, &input);
	if (res >= 0 && input >= 0)
		ramp_down_step = input;
	else
		return -EINVAL;
	return count;
}

static ssize_t show_max_cpu_load(struct kobject *kobj, struct attribute *attr,
		char *buf) {
	return sprintf(buf, "%u\n", max_cpu_load);
}

static ssize_t store_max_cpu_load(struct kobject *kobj, struct attribute *attr,
		const char *buf, size_t count) {
	ssize_t res;
	unsigned long input;
	res = strict_strtoul(buf, 0, &input);
	if (res >= 0 && input > 0 && input <= 100)
		max_cpu_load = input;
	else
		return -EINVAL;
	return count;
}

static ssize_t show_min_cpu_load(struct kobject *kobj, struct attribute *attr,
		char *buf) {
	return sprintf(buf, "%u\n", min_cpu_load);
}

static ssize_t store_min_cpu_load(struct kobject *kobj, struct attribute *attr,
		const char *buf, size_t count) {
	ssize_t res;
	unsigned long input;
	res = strict_strtoul(buf, 0, &input);
	if (res >= 0 && input > 0 && input < 100)
		min_cpu_load = input;
	else
		return -EINVAL;
	return count;
}

static ssize_t show_sampling_rate(struct kobject *kobj, struct attribute *attr,
		char *buf) {
	return sprintf(buf, "%u\n", sampling_rate);
}

static ssize_t store_sampling_rate(struct kobject *kobj, struct attribute *attr,
		const char *buf, size_t count) {
	ssize_t res;
	unsigned long input;
	res = strict_strtoul(buf, 0, &input);
	if (res >= 0 && input > 10000)
		sampling_rate = input;
	else
		return -EINVAL;
	return count;
}

static ssize_t show_touch_poke_freq(struct kobject *kobj,
		struct attribute *attr, char *buf) {
	return sprintf(buf, "%u\n", touch_poke_freq);
}

static ssize_t store_touch_poke_freq(struct kobject *a, struct attribute *b,
		const char *buf, size_t count) {
	ssize_t res;
	unsigned long input;
	res = strict_strtoul(buf, 0, &input);
	if (res >= 0){
		touch_poke_freq = input;
	
		if (touch_poke_freq == 0)
			touch_poke = false;
		else
			touch_poke = true;
	} else
		return -EINVAL;	
	return count;
}

static ssize_t show_input_boost_duration(struct kobject *kobj,
		struct attribute *attr, char *buf) {
	return sprintf(buf, "%u\n", input_boost_duration);
}

static ssize_t store_input_boost_duration(struct kobject *a,
		struct attribute *b, const char *buf, size_t count) {
	ssize_t res;
	unsigned long input;
	res = strict_strtoul(buf, 0, &input);
	if (res >= 0 && input > 10000)
		input_boost_duration = input;
	else
		return -EINVAL;
	return count;
}

static ssize_t show_ramp_up_during_boost(struct kobject *kobj,
		struct attribute *attr, char *buf) {
	return sprintf(buf, "%d\n", ramp_up_during_boost);
}

static ssize_t store_ramp_up_during_boost(struct kobject *a, struct attribute *b,
		const char *buf, size_t count) {
	ssize_t res;
	unsigned long input;
	res = strict_strtoul(buf, 0, &input);
	if (res >= 0) {
		if (input == 0)
			ramp_up_during_boost = false;
		else if (input == 1)
			ramp_up_during_boost = true;
		else
			return -EINVAL;
	} else
		return -EINVAL;
	return count;
}

static ssize_t show_boost_freq(struct kobject *kobj, struct attribute *attr,
		char *buf) {
	return sprintf(buf, "%u\n", boost_freq);
}

static ssize_t store_boost_freq(struct kobject *a, struct attribute *b,
		const char *buf, size_t count) {
	ssize_t res;
	unsigned long input;
	res = strict_strtoul(buf, 0, &input);
	if (res >= 0) {
		boost_freq = input;
		if (boost_freq == 0)
			boost = false;
		else
			boost = true;
	} else
		return -EINVAL;	
	return count;
}

static ssize_t show_boost_duration(struct kobject *kobj, struct attribute *attr,
		char *buf) {
	return sprintf(buf, "%d\n", boost_running);
}

static ssize_t store_boost_duration(struct kobject *a, struct attribute *b,
		const char *buf, size_t count) {
	ssize_t res;
	unsigned long input;
	res = strict_strtoul(buf, 0, &input);
	if (res >= 0 && input > 10000){
		boost_duration = input;
		if (boost) {
			// no need to bother if currently a boost is running anyway
			if (boost_task_alive && boost_running)
				return count;

			if (boost_task_alive) {
				cur_boost_freq = boost_freq;
				cur_boost_duration = boost_duration;
				wake_up_process(boost_task);
			}
		}
	} else
		return -EINVAL;
	return count;
}

static ssize_t show_io_is_busy(struct kobject *kobj, struct attribute *attr,
		char *buf) {
	return sprintf(buf, "%d\n", io_is_busy);
}

static ssize_t store_io_is_busy(struct kobject *a, struct attribute *b,
		const char *buf, size_t count) {
	ssize_t res;
	unsigned long input;

	res = strict_strtoul(buf, 0, &input);
	if (res >= 0) {
		if (input > 1)
			input = 1;
		if (input == io_is_busy) { /* nothing to do */
			return count;
		}
		io_is_busy = input;
	} else
		return -EINVAL;	
	return count;
}

static ssize_t show_ignore_nice(struct kobject *kobj, struct attribute *attr,
		char *buf) {
	return sprintf(buf, "%d\n", ignore_nice);
}

static ssize_t store_ignore_nice(struct kobject *a, struct attribute *b,
		const char *buf, size_t count) {
	ssize_t res;
	unsigned long input;

	res = strict_strtoul(buf, 0, &input);
	if (res >= 0) {
		if (input > 1)
			input = 1;
		if (input == ignore_nice) { /* nothing to do */
			return count;
		}
		ignore_nice = input;
		/* we need to re-evaluate prev_cpu_idle */
		update_idle_time(true);
	} else
		return -EINVAL;	
	return count;
}

#define define_global_rw_attr(_name)		\
static struct global_attr _name##_attr =	\
	__ATTR(_name, 0644, show_##_name, store_##_name)

define_global_rw_attr(debug_mask);
define_global_rw_attr(up_rate);
define_global_rw_attr(down_rate);
define_global_rw_attr(ramp_up_step);
define_global_rw_attr(ramp_down_step);
define_global_rw_attr(max_cpu_load);
define_global_rw_attr(min_cpu_load);
define_global_rw_attr(sampling_rate);
define_global_rw_attr(touch_poke_freq);
define_global_rw_attr(input_boost_duration);
define_global_rw_attr(boost_freq);
define_global_rw_attr(boost_duration);
define_global_rw_attr(io_is_busy);
define_global_rw_attr(ignore_nice);
define_global_rw_attr(ramp_up_during_boost);
define_global_rw_attr(awake_ideal_freq);
define_global_rw_attr(suspend_ideal_freq);

static struct attribute * smartmax_attributes[] = { 
	&debug_mask_attr.attr,
	&up_rate_attr.attr, 
	&down_rate_attr.attr, 
	&ramp_up_step_attr.attr, 
	&ramp_down_step_attr.attr,
	&max_cpu_load_attr.attr, 
	&min_cpu_load_attr.attr,
	&sampling_rate_attr.attr, 
	&touch_poke_freq_attr.attr,
	&input_boost_duration_attr.attr, 
	&boost_freq_attr.attr, 
	&boost_duration_attr.attr, 
	&io_is_busy_attr.attr,
	&ignore_nice_attr.attr, 
	&ramp_up_during_boost_attr.attr, 
	&awake_ideal_freq_attr.attr,
	&suspend_ideal_freq_attr.attr,		
	NULL , };

static struct attribute_group smartmax_attr_group = { .attrs =
		smartmax_attributes, .name = "smartmax", };

static int cpufreq_smartmax_boost_task(void *data) {
	struct cpufreq_policy *policy;
	struct smartmax_info_s *this_smartmax;
	cputime64_t now;

	while (1) {
		set_current_state(TASK_INTERRUPTIBLE);
		schedule();

		if (kthread_should_stop())
			break;

		set_current_state(TASK_RUNNING);

		if (boost_running)
			continue;
		
		/* we always boost cpu 0 */
		this_smartmax = &per_cpu(smartmax_info, 0);
		if (!this_smartmax)
			continue;

		policy = this_smartmax->cur_policy;
		if (!policy)
			continue;

		if (lock_policy_rwsem_write(0) < 0)
			continue;

		mutex_lock(&this_smartmax->timer_mutex);

		if (policy->cur < cur_boost_freq) {
			boost_running = true;
		
			now = ktime_to_ns(ktime_get());
			boost_end_time = now + cur_boost_duration;
			dprintk(SMARTMAX_DEBUG_BOOST, "%s %llu %llu\n", __func__, now, boost_end_time);

			target_freq(policy, this_smartmax, cur_boost_freq, this_smartmax->old_freq, CPUFREQ_RELATION_H);
			this_smartmax->prev_cpu_idle = get_cpu_idle_time(0, &this_smartmax->prev_cpu_wall);
		}
		mutex_unlock(&this_smartmax->timer_mutex);
				
		unlock_policy_rwsem_write(0);
	}

	return 0;
}

static void dbs_input_event(struct input_handle *handle, unsigned int type,
		unsigned int code, int value) {
	if (touch_poke && type == EV_SYN && code == SYN_REPORT) {
		// no need to bother if currently a boost is running anyway
		if (boost_task_alive && boost_running)
			return;

		if (boost_task_alive) {
			cur_boost_freq = touch_poke_freq;
			cur_boost_duration = input_boost_duration;
			wake_up_process(boost_task);
		}
	}
}

static int input_dev_filter(const char* input_dev_name) {
	int ret = 0;
	if (strstr(input_dev_name, "touchscreen")
			|| strstr(input_dev_name, "-keypad")
			|| strstr(input_dev_name, "-nav")
			|| strstr(input_dev_name, "-oj")) {
	} else {
		ret = 1;
	}
	return ret;
}

static int dbs_input_connect(struct input_handler *handler,
		struct input_dev *dev, const struct input_device_id *id) {
	struct input_handle *handle;
	int error;

	/* filter out those input_dev that we don't care */
	if (input_dev_filter(dev->name))
		return 0;

	dprintk(SMARTMAX_DEBUG_INPUT, "%s\n", __func__);

	handle = kzalloc(sizeof(struct input_handle), GFP_KERNEL);
	if (!handle)
		return -ENOMEM;

	handle->dev = dev;
	handle->handler = handler;
	handle->name = "cpufreq";

	error = input_register_handle(handle);
	if (error)
		goto err2;

	error = input_open_device(handle);
	if (error)
		goto err1;

	return 0;
	err1: input_unregister_handle(handle);
	err2: kfree(handle);
	return error;
}

static void dbs_input_disconnect(struct input_handle *handle) {
	input_close_device(handle);
	input_unregister_handle(handle);
	kfree(handle);
}

static const struct input_device_id dbs_ids[] = { { .driver_info = 1 }, { }, };

static struct input_handler dbs_input_handler = { .event = dbs_input_event,
		.connect = dbs_input_connect, .disconnect = dbs_input_disconnect,
		.name = "cpufreq_smartmax", .id_table = dbs_ids, };

#ifdef CONFIG_HAS_EARLYSUSPEND
static void smartmax_early_suspend(struct early_suspend *h)
{
	dprintk(SMARTMAX_DEBUG_SUSPEND, "%s\n", __func__);
	ideal_freq = suspend_ideal_freq;
	is_suspended = true;
	smartmax_update_min_max_allcpus();
}

static void smartmax_late_resume(struct early_suspend *h)
{
	dprintk(SMARTMAX_DEBUG_SUSPEND, "%s\n", __func__);
	ideal_freq = awake_ideal_freq;
	is_suspended = false;
	smartmax_update_min_max_allcpus();
}
#endif

static int cpufreq_governor_smartmax(struct cpufreq_policy *new_policy,
		unsigned int event) {
	unsigned int cpu = new_policy->cpu;
	int rc;
	struct smartmax_info_s *this_smartmax = &per_cpu(smartmax_info, cpu);
	struct sched_param param = { .sched_priority = 1 };
    unsigned int latency;
    unsigned int min_sampling_rate;

	switch (event) {
	case CPUFREQ_GOV_START:
		if ((!cpu_online(cpu)) || (!new_policy->cur))return -EINVAL;

		mutex_lock(&dbs_mutex);

		this_smartmax->cur_policy = new_policy;
		this_smartmax->cpu = cpu;
		this_smartmax->enable = true;

		smartmax_update_min_max(this_smartmax,new_policy);

		this_smartmax->freq_table = cpufreq_frequency_get_table(cpu);

		update_idle_time(false);

		dbs_enable++;
		
		if (dbs_enable == 1) {
			if (!boost_task_alive) {
				boost_task = kthread_create (
						cpufreq_smartmax_boost_task,
						NULL,
						"kinputboostd"
				);

				if (IS_ERR(boost_task)) {
					dbs_enable--;
					mutex_unlock(&dbs_mutex);
					return PTR_ERR(boost_task);
				}

				sched_setscheduler_nocheck(boost_task, SCHED_RR, &param);
				get_task_struct(boost_task);
				boost_task_alive = true;
			}
			rc = input_register_handler(&dbs_input_handler);
			if (rc) {
				dbs_enable--;
				mutex_unlock(&dbs_mutex);
				return rc;
			}
			rc = sysfs_create_group(cpufreq_global_kobject,
					&smartmax_attr_group);
			if (rc) {
				dbs_enable--;
				mutex_unlock(&dbs_mutex);
				return rc;
			}
#ifdef CONFIG_HAS_EARLYSUSPEND
			register_early_suspend(&smartmax_early_suspend_handler);
#endif
			latency = new_policy->cpuinfo.transition_latency / 1000;
			if (latency == 0)
				latency = 1;
			
			min_sampling_rate = max(sampling_rate, MIN_LATENCY_MULTIPLIER * latency);
			sampling_rate = max(min_sampling_rate, latency * LATENCY_MULTIPLIER);
		}

		mutex_unlock(&dbs_mutex);
		mutex_init(&this_smartmax->timer_mutex);
		dbs_timer_init(this_smartmax);

		break;
	case CPUFREQ_GOV_LIMITS:
		mutex_lock(&this_smartmax->timer_mutex);
		smartmax_update_min_max(this_smartmax,new_policy);

		if (this_smartmax->cur_policy->cur > new_policy->max) {
			dprintk(SMARTMAX_DEBUG_JUMPS,"jumping to new max freq: %d\n",new_policy->max);
			__cpufreq_driver_target(this_smartmax->cur_policy,
					new_policy->max, CPUFREQ_RELATION_H);
		}
		else if (this_smartmax->cur_policy->cur < new_policy->min) {
			dprintk(SMARTMAX_DEBUG_JUMPS,"jumping to new min freq: %d\n",new_policy->min);
			__cpufreq_driver_target(this_smartmax->cur_policy,
					new_policy->min, CPUFREQ_RELATION_C);
		}
		mutex_unlock(&this_smartmax->timer_mutex);
		break;

	case CPUFREQ_GOV_STOP:
		dbs_timer_exit(this_smartmax);

		mutex_lock(&dbs_mutex);
		mutex_destroy(&this_smartmax->timer_mutex);
		this_smartmax->enable = false;
		dbs_enable--;

		if (!dbs_enable){
			sysfs_remove_group(cpufreq_global_kobject, &smartmax_attr_group);
			input_unregister_handler(&dbs_input_handler);
#ifdef CONFIG_HAS_EARLYSUSPEND
			unregister_early_suspend(&smartmax_early_suspend_handler);
#endif
		}
		
		mutex_unlock(&dbs_mutex);
		break;
	}

	return 0;
}

static int __init cpufreq_smartmax_init(void) {
	unsigned int i;
	struct smartmax_info_s *this_smartmax;
	up_rate = DEFAULT_UP_RATE;
	down_rate = DEFAULT_DOWN_RATE;
	suspend_ideal_freq = DEFAULT_SUSPEND_IDEAL_FREQ;
	awake_ideal_freq = DEFAULT_AWAKE_IDEAL_FREQ;
	ideal_freq = awake_ideal_freq;
	ramp_up_step = DEFAULT_RAMP_UP_STEP;
	ramp_down_step = DEFAULT_RAMP_DOWN_STEP;
	max_cpu_load = DEFAULT_MAX_CPU_LOAD;
	min_cpu_load = DEFAULT_MIN_CPU_LOAD;
	sampling_rate = DEFAULT_SAMPLING_RATE;
	input_boost_duration = DEFAULT_INPUT_BOOST_DURATION;
	io_is_busy = DEFAULT_IO_IS_BUSY;
	ignore_nice = DEFAULT_IGNORE_NICE;

	/* Initalize per-cpu data: */for_each_possible_cpu(i)
	{
		this_smartmax = &per_cpu(smartmax_info, i);
		this_smartmax->enable = false;
		this_smartmax->cur_policy = 0;
		this_smartmax->ramp_dir = 0;
		this_smartmax->freq_change_time = 0;
		this_smartmax->cur_cpu_load = 0;
	}

#ifdef CONFIG_HAS_EARLYSUSPEND
	smartmax_early_suspend_handler.suspend = smartmax_early_suspend;
	smartmax_early_suspend_handler.resume = smartmax_late_resume;
	smartmax_early_suspend_handler.level = EARLY_SUSPEND_LEVEL_DISABLE_FB + 100;
#endif
	
	return cpufreq_register_governor(&cpufreq_gov_smartmax);
}

#ifdef CONFIG_CPU_FREQ_DEFAULT_GOV_SMARTMAX
fs_initcall(cpufreq_smartmax_init);
#else
module_init(cpufreq_smartmax_init);
#endif

static void __exit cpufreq_smartmax_exit(void) {
	cpufreq_unregister_governor(&cpufreq_gov_smartmax);
}

module_exit(cpufreq_smartmax_exit);

MODULE_AUTHOR("maxwen");
MODULE_DESCRIPTION("'cpufreq_smartmax' - A smart cpufreq governor");
MODULE_LICENSE("GPL");



