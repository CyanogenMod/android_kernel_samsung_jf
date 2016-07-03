/* Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/msm_tsens.h>
#include <linux/workqueue.h>
#include <linux/cpu.h>
#include <linux/cpufreq.h>
#include <linux/msm_tsens.h>
#include <linux/msm_thermal.h>
#include <linux/platform_device.h>
#include <linux/of.h>

/* Sampling rate of the thermal algorithm in ms */
unsigned int poll_ms = 1000;
module_param(poll_ms, int, 0644);

static struct thermal_info {
	unsigned int limited_max_freq;
	unsigned int thres_pos;
} info = {
	.limited_max_freq = UINT_MAX,
	.thres_pos = 0,
};

static int temp_threshold[] = {
	0,
	85,
	90,
	95,
	100,
	105,
	110,
};

static int temp_threshold_clr[] = {
	0,
	80,
	85,
	90,
	95,
	100,
	105,
};

static int temp_action[] = {
	UINT_MAX,
	1674000,
	1350000,
	1026000,
	810000,
	594000,
	384000,
};

static struct msm_thermal_data msm_thermal_info;

static struct delayed_work check_temp_work;

static int msm_thermal_cpufreq_callback(struct notifier_block *nfb,
		unsigned long event, void *data)
{
	struct cpufreq_policy *policy = data;

	switch (event) {
	case CPUFREQ_INCOMPATIBLE:
		pr_debug("Mitigating CPU%u to freq max: %u\n",
				policy->cpu, info.limited_max_freq);

		cpufreq_verify_within_limits(policy, policy->cpuinfo.min_freq,
				info.limited_max_freq);
		break;
	}
	return NOTIFY_OK;
}

static struct notifier_block msm_thermal_cpufreq_notifier = {
	.notifier_call = msm_thermal_cpufreq_callback,
};

static void update_cpu_freq(unsigned int cpu)
{
	int ret = 0;

	if (cpu_online(cpu)) {
		ret = cpufreq_update_policy(cpu);
		if (ret)
			pr_err("Unable to update policy for cpu:%u. err:%d\n",
					cpu, ret);
	}
}

static void do_freq_control(unsigned long temp)
{
	unsigned int cpu = 0;
	unsigned int max_freq = info.limited_max_freq;
	unsigned int i;

	/* Search for the next appropriate temperature threshold */
	if (temp >= temp_threshold[info.thres_pos]) {
		for (i = info.thres_pos; i < ARRAY_SIZE(temp_threshold); i++) {
			if (temp < temp_threshold[i]) {
				info.thres_pos = i - 1;
				break;
			}
		}
	} else if (temp < temp_threshold_clr[info.thres_pos]) {
		for (i = info.thres_pos; i >= 0 ; i--) {
			if (temp >= temp_threshold[i]) {
				info.thres_pos = i;
				break;
			}
		}
	}

	max_freq = temp_action[info.thres_pos];

	if (max_freq == info.limited_max_freq)
		return;

	info.limited_max_freq = max_freq;

	/* Update new limits */
	get_online_cpus();
	for_each_possible_cpu(cpu) {
		pr_info("Limiting CPU%u max frequency to %u. Temp:%lu\n",
				cpu, max_freq, temp);
		update_cpu_freq(cpu);
	}
	put_online_cpus();
}

static void do_core_control(unsigned long temp)
{
	/* TODO: Add core control */
}

static void check_temp(struct work_struct *work)
{
	struct tsens_device tsens_dev;
	unsigned long temp = 0;
	int ret = 0;

	tsens_dev.sensor_num = msm_thermal_info.sensor_id;
	ret = tsens_get_temp(&tsens_dev, &temp);
	if (ret) {
		pr_err("Unable to read TSENS sensor:%d. err:%d\n",
				tsens_dev.sensor_num, ret);
		goto reschedule;
	}

	do_core_control(temp);
	do_freq_control(temp);

reschedule:
	schedule_delayed_work(&check_temp_work,	msecs_to_jiffies(poll_ms));
}

int __devinit msm_thermal_init(struct msm_thermal_data *pdata)
{
	int ret = 0;

	BUG_ON(!pdata);
	BUG_ON(pdata->sensor_id >= TSENS_MAX_SENSORS);
	memcpy(&msm_thermal_info, pdata, sizeof(struct msm_thermal_data));

	cpufreq_register_notifier(&msm_thermal_cpufreq_notifier,
			CPUFREQ_POLICY_NOTIFIER);

	INIT_DELAYED_WORK(&check_temp_work, check_temp);
	schedule_delayed_work(&check_temp_work, 0);

	return ret;
}

static int __devinit msm_thermal_dev_probe(struct platform_device *pdev)
{
	int ret = 0;
	char *key = NULL;
	struct device_node *node = pdev->dev.of_node;
	struct msm_thermal_data data;

	memset(&data, 0, sizeof(struct msm_thermal_data));
	key = "qcom,sensor-id";
	ret = of_property_read_u32(node, key, &data.sensor_id);
	if (ret)
		goto fail;
	WARN_ON(data.sensor_id >= TSENS_MAX_SENSORS);

fail:
	if (ret)
		pr_err("%s: Failed reading node=%s, key=%s\n",
		       __func__, node->full_name, key);
	else
		ret = msm_thermal_init(&data);

	return ret;
}

static struct of_device_id msm_thermal_match_table[] = {
	{.compatible = "qcom,msm-thermal"},
	{},
};

static struct platform_driver msm_thermal_device_driver = {
	.probe = msm_thermal_dev_probe,
	.driver = {
		.name = "msm-thermal",
		.owner = THIS_MODULE,
		.of_match_table = msm_thermal_match_table,
	},
};

int __init msm_thermal_device_init(void)
{
	return platform_driver_register(&msm_thermal_device_driver);
}
