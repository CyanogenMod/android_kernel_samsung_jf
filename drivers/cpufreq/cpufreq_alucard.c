/*
 *  drivers/cpufreq/cpufreq_alucard.c
 *
 *  Copyright (C)  2011 Samsung Electronics co. ltd
 *    ByungChang Cha <bc.cha@samsung.com>
 *
 *  Based on ondemand governor
 *  Copyright (C)  2001 Russell King
 *            (C)  2003 Venkatesh Pallipadi <venkatesh.pallipadi@intel.com>.
 *                      Jun Nakajima <jun.nakajima@intel.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 * 
 * Created by Alucard_24@xda
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cpufreq.h>
#include <linux/cpu.h>
#include <linux/jiffies.h>
#include <linux/kernel_stat.h>
#include <linux/mutex.h>
#include <linux/hrtimer.h>
#include <linux/tick.h>
#include <linux/ktime.h>
#include <linux/sched.h>
#include <linux/slab.h>
/*
 * dbs is used in this file as a shortform for demandbased switching
 * It helps to keep variable names smaller, simpler
 */

static void do_alucard_timer(struct work_struct *work);
static int cpufreq_governor_alucard(struct cpufreq_policy *policy,
				unsigned int event);

#ifndef CONFIG_CPU_FREQ_DEFAULT_GOV_ALUCARD
static
#endif
struct cpufreq_governor cpufreq_gov_alucard = {
	.name                   = "alucard",
	.governor               = cpufreq_governor_alucard,
	.owner                  = THIS_MODULE,
};

struct cpufreq_alucard_cpuinfo {
	cputime64_t prev_cpu_wall;
	cputime64_t prev_cpu_idle;
	struct cpufreq_frequency_table *freq_table;
	struct delayed_work work;
	struct cpufreq_policy *cur_policy;
	int cpu;
	unsigned int enable:1;
	/*
	 * mutex that serializes governor limit change with
	 * do_alucard_timer invocation. We do not want do_alucard_timer to run
	 * when user is changing the governor or limits.
	 */
	struct mutex timer_mutex;
};

static DEFINE_PER_CPU(struct cpufreq_alucard_cpuinfo, od_alucard_cpuinfo);

static unsigned int alucard_enable;	/* number of CPUs using this policy */
/*
 * alucard_mutex protects alucard_enable in governor start/stop.
 */
static DEFINE_MUTEX(alucard_mutex);

/* alucard tuners */
static struct alucard_tuners {
	atomic_t sampling_rate;
	atomic_t inc_cpu_load_at_min_freq;
	atomic_t inc_cpu_load;
	atomic_t dec_cpu_load_at_min_freq;
	atomic_t dec_cpu_load;
	atomic_t freq_responsiveness;
	atomic_t pump_inc_step;
	atomic_t pump_dec_step;
} alucard_tuners_ins = {
	.sampling_rate = ATOMIC_INIT(50000),
	.inc_cpu_load_at_min_freq = ATOMIC_INIT(70),
	.inc_cpu_load = ATOMIC_INIT(70),
	.dec_cpu_load_at_min_freq = ATOMIC_INIT(70),
	.dec_cpu_load = ATOMIC_INIT(70),
	.freq_responsiveness = ATOMIC_INIT(1134000),
	.pump_inc_step = ATOMIC_INIT(1),
	.pump_dec_step = ATOMIC_INIT(2),
};

/************************** sysfs interface ************************/

/* cpufreq_alucard Governor Tunables */
#define show_one(file_name, object)					\
static ssize_t show_##file_name						\
(struct kobject *kobj, struct attribute *attr, char *buf)		\
{									\
	return sprintf(buf, "%d\n", atomic_read(&alucard_tuners_ins.object));		\
}
show_one(sampling_rate, sampling_rate);
show_one(inc_cpu_load_at_min_freq, inc_cpu_load_at_min_freq);
show_one(inc_cpu_load, inc_cpu_load);
show_one(dec_cpu_load_at_min_freq, dec_cpu_load_at_min_freq);
show_one(dec_cpu_load, dec_cpu_load);
show_one(freq_responsiveness, freq_responsiveness);
show_one(pump_inc_step, pump_inc_step);
show_one(pump_dec_step, pump_dec_step);

/**
 * update_sampling_rate - update sampling rate effective immediately if needed.
 * @new_rate: new sampling rate
 *
 * If new rate is smaller than the old, simply updaing
 * alucard_tuners_ins.sampling_rate might not be appropriate. For example,
 * if the original sampling_rate was 1 second and the requested new sampling
 * rate is 10 ms because the user needs immediate reaction from ondemand
 * governor, but not sure if higher frequency will be required or not,
 * then, the governor may change the sampling rate too late; up to 1 second
 * later. Thus, if we are reducing the sampling rate, we need to make the
 * new value effective immediately.
 */
static void update_sampling_rate(unsigned int new_rate)
{
	int cpu;

	atomic_set(&alucard_tuners_ins.sampling_rate,new_rate);

	get_online_cpus();
	for_each_online_cpu(cpu) {
		struct cpufreq_policy *policy;
		struct cpufreq_alucard_cpuinfo *alucard_cpuinfo;
		unsigned long next_sampling, appointed_at;

		policy = cpufreq_cpu_get(cpu);
		if (!policy)
			continue;
		alucard_cpuinfo = &per_cpu(od_alucard_cpuinfo, policy->cpu);
		cpufreq_cpu_put(policy);

		mutex_lock(&alucard_cpuinfo->timer_mutex);

		if (!delayed_work_pending(&alucard_cpuinfo->work)) {
			mutex_unlock(&alucard_cpuinfo->timer_mutex);
			continue;
		}

		next_sampling  = jiffies + usecs_to_jiffies(new_rate);
		appointed_at = alucard_cpuinfo->work.timer.expires;


		if (time_before(next_sampling, appointed_at)) {

			mutex_unlock(&alucard_cpuinfo->timer_mutex);
			cancel_delayed_work_sync(&alucard_cpuinfo->work);
			mutex_lock(&alucard_cpuinfo->timer_mutex);

			#ifdef CONFIG_CPU_EXYNOS4210
				mod_delayed_work_on(alucard_cpuinfo->cpu, system_wq, &alucard_cpuinfo->work, usecs_to_jiffies(new_rate));
			#else
				queue_delayed_work_on(alucard_cpuinfo->cpu, system_wq, &alucard_cpuinfo->work, usecs_to_jiffies(new_rate));
			#endif
		}
		mutex_unlock(&alucard_cpuinfo->timer_mutex);
	}
	put_online_cpus();
}

/* sampling_rate */
static ssize_t store_sampling_rate(struct kobject *a, struct attribute *b,
				   const char *buf, size_t count)
{
	int input;
	int ret;

	ret = sscanf(buf, "%d", &input);
	if (ret != 1)
		return -EINVAL;

	input = max(input,10000);
	
	if (input == atomic_read(&alucard_tuners_ins.sampling_rate))
		return count;

	update_sampling_rate(input);

	return count;
}

/* inc_cpu_load_at_min_freq */
static ssize_t store_inc_cpu_load_at_min_freq(struct kobject *a, struct attribute *b,
				   const char *buf, size_t count)
{
	int input;
	int ret;

	ret = sscanf(buf, "%d", &input);
	if (ret != 1) {
		return -EINVAL;
	}

	input = min(input,atomic_read(&alucard_tuners_ins.inc_cpu_load));

	if (input == atomic_read(&alucard_tuners_ins.inc_cpu_load_at_min_freq))
		return count;

	atomic_set(&alucard_tuners_ins.inc_cpu_load_at_min_freq,input);

	return count;
}

/* inc_cpu_load */
static ssize_t store_inc_cpu_load(struct kobject *a, struct attribute *b,
					const char *buf, size_t count)
{
	int input;
	int ret;

	ret = sscanf(buf, "%d", &input);
	if (ret != 1)
		return -EINVAL;

	input = max(min(input,100),0);

	if (input == atomic_read(&alucard_tuners_ins.inc_cpu_load))
		return count;

	atomic_set(&alucard_tuners_ins.inc_cpu_load,input);

	return count;
}

/* dec_cpu_load_at_min_freq */
static ssize_t store_dec_cpu_load_at_min_freq(struct kobject *a, struct attribute *b,
				   const char *buf, size_t count)
{
	int input;
	int ret;

	ret = sscanf(buf, "%d", &input);
	if (ret != 1) {
		return -EINVAL;
	}

	input = min(input,atomic_read(&alucard_tuners_ins.dec_cpu_load));

	if (input == atomic_read(&alucard_tuners_ins.dec_cpu_load_at_min_freq))
		return count;

	atomic_set(&alucard_tuners_ins.dec_cpu_load_at_min_freq,input);

	return count;
}

/* dec_cpu_load */
static ssize_t store_dec_cpu_load(struct kobject *a, struct attribute *b,
					const char *buf, size_t count)
{
	int input;
	int ret;

	ret = sscanf(buf, "%d", &input);
	if (ret != 1)
		return -EINVAL;

	input = max(min(input,95),5);

	if (input == atomic_read(&alucard_tuners_ins.dec_cpu_load))
		return count;

	atomic_set(&alucard_tuners_ins.dec_cpu_load,input);

	return count;
}

/* freq_responsiveness */
static ssize_t store_freq_responsiveness(struct kobject *a, struct attribute *b,
				   const char *buf, size_t count)
{
	int input;
	int ret;

	ret = sscanf(buf, "%d", &input);
	if (ret != 1)
		return -EINVAL;

	if (input == atomic_read(&alucard_tuners_ins.freq_responsiveness))
		return count;

	atomic_set(&alucard_tuners_ins.freq_responsiveness,input);

	return count;
}

/* pump_inc_step */
static ssize_t store_pump_inc_step(struct kobject *a, struct attribute *b,
			       const char *buf, size_t count)
{
	int input;
	int ret;

	ret = sscanf(buf, "%d", &input);
	if (ret != 1)
		return -EINVAL;

	input = max(min(input,3),1);

	if (input == atomic_read(&alucard_tuners_ins.pump_inc_step))
		return count;

	atomic_set(&alucard_tuners_ins.pump_inc_step,input);

	return count;
}

/* pump_dec_step */
static ssize_t store_pump_dec_step(struct kobject *a, struct attribute *b,
			       const char *buf, size_t count)
{
	int input;
	int ret;

	ret = sscanf(buf, "%d", &input);
	if (ret != 1)
		return -EINVAL;

	input = max(min(input,3),1);

	if (input == atomic_read(&alucard_tuners_ins.pump_dec_step))
		return count;

	atomic_set(&alucard_tuners_ins.pump_dec_step,input);

	return count;
}

define_one_global_rw(sampling_rate);
define_one_global_rw(inc_cpu_load_at_min_freq);
define_one_global_rw(inc_cpu_load);
define_one_global_rw(dec_cpu_load_at_min_freq);
define_one_global_rw(dec_cpu_load);
define_one_global_rw(freq_responsiveness);
define_one_global_rw(pump_inc_step);
define_one_global_rw(pump_dec_step);

static struct attribute *alucard_attributes[] = {
	&sampling_rate.attr,
	&inc_cpu_load_at_min_freq.attr,
	&inc_cpu_load.attr,
	&dec_cpu_load_at_min_freq.attr,
	&dec_cpu_load.attr,
	&freq_responsiveness.attr,
	&pump_inc_step.attr,
	&pump_dec_step.attr,
	NULL
};

static struct attribute_group alucard_attr_group = {
	.attrs = alucard_attributes,
	.name = "alucard",
};

/************************** sysfs end ************************/

static void alucard_check_cpu(struct cpufreq_alucard_cpuinfo *this_alucard_cpuinfo)
{
	struct cpufreq_policy *cpu_policy;
	unsigned int min_freq;
	unsigned int max_freq;
	unsigned int freq_responsiveness;
	int dec_cpu_load;
	int inc_cpu_load;
	int pump_inc_step;
	int pump_dec_step;
	cputime64_t cur_wall_time, cur_idle_time;
	unsigned int wall_time, idle_time;
#ifndef CONFIG_CPU_EXYNOS4210
	unsigned int index = 0;
	unsigned int tmp_freq = 0;
#endif
	unsigned int next_freq = 0;
	int cur_load = -1;
	unsigned int cpu;
	
	cpu = this_alucard_cpuinfo->cpu;
	cpu_policy = this_alucard_cpuinfo->cur_policy;

	cur_idle_time = get_cpu_idle_time_us(cpu, NULL);
	cur_idle_time += get_cpu_iowait_time_us(cpu, &cur_wall_time);

	wall_time = (unsigned int)
			(cur_wall_time - this_alucard_cpuinfo->prev_cpu_wall);
	this_alucard_cpuinfo->prev_cpu_wall = cur_wall_time;

	idle_time = (unsigned int)
			(cur_idle_time - this_alucard_cpuinfo->prev_cpu_idle);
	this_alucard_cpuinfo->prev_cpu_idle = cur_idle_time;

	if (!cpu_policy)
		return;

	/*printk(KERN_ERR "TIMER CPU[%u], wall[%u], idle[%u]\n",cpu, wall_time, idle_time);*/
	if (wall_time >= idle_time) { /*if wall_time < idle_time, evaluate cpu load next time*/
		cur_load = wall_time > idle_time ? (100 * (wall_time - idle_time)) / wall_time : 1;/*if wall_time is equal to idle_time cpu_load is equal to 1*/
		tmp_freq = cpu_policy->cur;
		/* Checking Frequency Limit */
		min_freq = cpu_policy->min;
		max_freq = cpu_policy->max;

		freq_responsiveness = atomic_read(&alucard_tuners_ins.freq_responsiveness);
	
		pump_inc_step = atomic_read(&alucard_tuners_ins.pump_inc_step);
		pump_dec_step = atomic_read(&alucard_tuners_ins.pump_dec_step);

		/* CPUs Online Scale Frequency*/
		if (cpu_policy->cur < freq_responsiveness) {
			inc_cpu_load = atomic_read(&alucard_tuners_ins.inc_cpu_load_at_min_freq);
			dec_cpu_load = atomic_read(&alucard_tuners_ins.dec_cpu_load_at_min_freq);
		} else {
			inc_cpu_load = atomic_read(&alucard_tuners_ins.inc_cpu_load);
			dec_cpu_load = atomic_read(&alucard_tuners_ins.dec_cpu_load);
		}		
		/* Check for frequency increase or for frequency decrease */
#ifdef CONFIG_CPU_EXYNOS4210
		if (cur_load >= inc_cpu_load && cpu_policy->cur < max_freq) {
			next_freq = min(cpu_policy->cur + (pump_inc_step * 100000), max_freq);
		} else if (cur_load < dec_cpu_load && cpu_policy->cur > min_freq) {
			next_freq = max(cpu_policy->cur - (pump_dec_step * 100000), min_freq);
		} else {
			next_freq = cpu_policy->cur;
		}
#else
		if (cur_load >= inc_cpu_load && cpu_policy->cur < max_freq) {
			tmp_freq = min(cpu_policy->cur + (pump_inc_step * 108000), max_freq);
		} else if (cur_load < dec_cpu_load && cpu_policy->cur > min_freq) {
			tmp_freq = max(cpu_policy->cur - (pump_dec_step * 108000), min_freq);
		} else {
			tmp_freq = cpu_policy->cur;
		}
		cpufreq_frequency_table_target(cpu_policy, this_alucard_cpuinfo->freq_table, tmp_freq,
			CPUFREQ_RELATION_H, &index);
		if (this_alucard_cpuinfo->freq_table[index].frequency != cpu_policy->cur) {
			cpufreq_frequency_table_target(cpu_policy, this_alucard_cpuinfo->freq_table, tmp_freq,
				CPUFREQ_RELATION_C, &index);
		}
	 	next_freq = this_alucard_cpuinfo->freq_table[index].frequency;
#endif
		/*printk(KERN_ERR "FREQ CALC.: CPU[%u], load[%d], target freq[%u], cur freq[%u], min freq[%u], max_freq[%u]\n",cpu, cur_load, next_freq, cpu_policy->cur, cpu_policy->min, max_freq);*/
		if (next_freq != cpu_policy->cur && cpu_online(cpu)) {
			__cpufreq_driver_target(cpu_policy, next_freq, CPUFREQ_RELATION_L);
		}
	}

}

static void do_alucard_timer(struct work_struct *work)
{
	struct cpufreq_alucard_cpuinfo *alucard_cpuinfo;
	int delay;
	unsigned int cpu;

	alucard_cpuinfo = container_of(work, struct cpufreq_alucard_cpuinfo, work.work);
	cpu = alucard_cpuinfo->cpu;

	mutex_lock(&alucard_cpuinfo->timer_mutex);
	alucard_check_cpu(alucard_cpuinfo);
	/* We want all CPUs to do sampling nearly on
	 * same jiffy
	 */
	delay = usecs_to_jiffies(atomic_read(&alucard_tuners_ins.sampling_rate));
	if (num_online_cpus() > 1) {
		delay -= jiffies % delay;
	}

	alucard_check_cpu(alucard_cpuinfo);

	queue_delayed_work_on(cpu, system_wq, &alucard_cpuinfo->work, delay);
	mutex_unlock(&alucard_cpuinfo->timer_mutex);
}

static int cpufreq_governor_alucard(struct cpufreq_policy *policy,
				unsigned int event)
{
	unsigned int cpu;
	struct cpufreq_alucard_cpuinfo *this_alucard_cpuinfo;
	int rc, delay;

	cpu = policy->cpu;
	this_alucard_cpuinfo = &per_cpu(od_alucard_cpuinfo, cpu);

	switch (event) {
	case CPUFREQ_GOV_START:
		if ((!cpu_online(cpu)) || (!policy->cur))
			return -EINVAL;

		mutex_lock(&alucard_mutex);

		this_alucard_cpuinfo->cur_policy = policy;

		this_alucard_cpuinfo->prev_cpu_idle = get_cpu_idle_time_us(cpu, NULL);
		this_alucard_cpuinfo->prev_cpu_idle += get_cpu_iowait_time_us(cpu, &this_alucard_cpuinfo->prev_cpu_wall);

		this_alucard_cpuinfo->freq_table = cpufreq_frequency_get_table(cpu);
		this_alucard_cpuinfo->cpu = cpu;

		mutex_init(&this_alucard_cpuinfo->timer_mutex);

		alucard_enable++;
		/*
		 * Start the timerschedule work, when this governor
		 * is used for first time
		 */
		if (alucard_enable == 1) {
			rc = sysfs_create_group(cpufreq_global_kobject,
						&alucard_attr_group);
			if (rc) {
				mutex_unlock(&alucard_mutex);
				return rc;
			}
		}

		mutex_unlock(&alucard_mutex);

		delay=usecs_to_jiffies(atomic_read(&alucard_tuners_ins.sampling_rate));
		if (num_online_cpus() > 1) {
			delay -= jiffies % delay;
		}

		this_alucard_cpuinfo->enable = 1;
		INIT_DELAYED_WORK_DEFERRABLE(&this_alucard_cpuinfo->work, do_alucard_timer);
		queue_delayed_work_on(this_alucard_cpuinfo->cpu, system_wq, &this_alucard_cpuinfo->work, delay);

		break;

	case CPUFREQ_GOV_STOP:
		this_alucard_cpuinfo->enable = 0;
		cancel_delayed_work_sync(&this_alucard_cpuinfo->work);

		mutex_lock(&alucard_mutex);
		alucard_enable--;
		mutex_destroy(&this_alucard_cpuinfo->timer_mutex);

		if (!alucard_enable) {
			sysfs_remove_group(cpufreq_global_kobject,
					   &alucard_attr_group);			
		}
		mutex_unlock(&alucard_mutex);
		
		break;

	case CPUFREQ_GOV_LIMITS:
		mutex_lock(&this_alucard_cpuinfo->timer_mutex);
		if (policy->max < this_alucard_cpuinfo->cur_policy->cur)
			__cpufreq_driver_target(this_alucard_cpuinfo->cur_policy,
				policy->max, CPUFREQ_RELATION_H);
		else if (policy->min > this_alucard_cpuinfo->cur_policy->cur)
			__cpufreq_driver_target(this_alucard_cpuinfo->cur_policy,
				policy->min, CPUFREQ_RELATION_C);
		mutex_unlock(&this_alucard_cpuinfo->timer_mutex);

		break;
	}
	return 0;
}

static int __init cpufreq_gov_alucard_init(void)
{
	return cpufreq_register_governor(&cpufreq_gov_alucard);
}

static void __exit cpufreq_gov_alucard_exit(void)
{
	cpufreq_unregister_governor(&cpufreq_gov_alucard);
}

MODULE_AUTHOR("Alucard24@XDA");
MODULE_DESCRIPTION("'cpufreq_alucard' - A dynamic cpufreq governor v1.0 (SnapDragon)");
MODULE_LICENSE("GPL");

#ifdef CONFIG_CPU_FREQ_DEFAULT_GOV_ALUCARD
fs_initcall(cpufreq_gov_alucard_init);
#else
module_init(cpufreq_gov_alucard_init);
#endif
module_exit(cpufreq_gov_alucard_exit);


