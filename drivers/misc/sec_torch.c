/*  drivers/misc/sec_torch.c
 *
 *  Copyright (C) 2013 Samsung Electronics Co.Ltd
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */
 
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <mach/gpio.h>
#include <linux/delay.h>
#include <linux/wakelock.h>
#include <linux/hrtimer.h>
#include <linux/err.h>
#include <linux/sec_torch.h>

int Torch_Flash_mode;
struct hrtimer torch_timer;
struct work_struct  torch_work;
static struct workqueue_struct *torch_wq;
struct wake_lock torch_wake_lock;

void (*Torch_Flash_OnOff) (unsigned long);

/* sys fs  */
struct class *torch_class;
EXPORT_SYMBOL(torch_class);
struct device *torch_dev;
EXPORT_SYMBOL(torch_dev);

static ssize_t Torch_Flash_Control(struct device *dev, struct device_attribute *attr, const char *buf, size_t size);
static DEVICE_ATTR(torch_ctl, 0220, NULL, Torch_Flash_Control);

#if 0
void Torch_Flash_Off_by_cam()
{
	torch_debug_msg("Torch_Flash_Off_by_cam\n");
	Torch_Flash_mode = 0;
	wake_unlock(&torch_wake_lock);
	hrtimer_cancel(&torch_timer);
}
#endif

static ssize_t Torch_Flash_Control(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	long val = simple_strtol(buf, NULL, 10);
	if(Torch_Flash_OnOff == NULL)
	{
		pr_err("Torch_Flash_OnOff was not registered\n");
		return ENOSYS;
	}
	if (val == 0) {
		torch_debug_msg("Torch_Flash_Off\n");
		Torch_Flash_mode = 0;
		Torch_Flash_OnOff(0);
		wake_unlock(&torch_wake_lock);
		hrtimer_cancel(&torch_timer);
	} else if(Torch_Flash_mode == 1) {
		torch_debug_msg("Torch_Flash is already On!!\n");
		hrtimer_start(&torch_timer, ktime_set(val, 0), HRTIMER_MODE_REL);
		torch_debug_msg("timer set to %ld\n", val);
	} else {
		torch_debug_msg("Torch_Flash_On\n");
		Torch_Flash_mode = 1;
		Torch_Flash_OnOff(1);
		wake_lock(&torch_wake_lock);
		hrtimer_start(&torch_timer, ktime_set(val, 0), HRTIMER_MODE_REL);
		torch_debug_msg("timer set to %ld\n", val);
	}
	return size;
}

static enum hrtimer_restart torch_flash_timer_func(struct hrtimer *timer)
{
	queue_work(torch_wq, &torch_work);
	return HRTIMER_NORESTART;
}

static void time_expire_work_func(struct work_struct *work)
{
	torch_debug_msg("Torch_Flash_timer_Off\n");
	Torch_Flash_mode = 0;
	if(Torch_Flash_OnOff)
		Torch_Flash_OnOff(0);
	else
		pr_err("Torch_Flash_OnOff was not registered\n");
	torch_debug_msg("Torch_Flash_timer_Off\n");
	wake_unlock(&torch_wake_lock);
}

static int torch_flash_probe(struct platform_device *pdev)
{
	torch_debug_msg("torch_flash_probe\n");
	
	torch_class = class_create(THIS_MODULE, "torch");
	if (IS_ERR(torch_class))
		pr_err("Failed to create class(torch)!\n");

	torch_dev = device_create(torch_class, NULL, 0, NULL, "torch");
	if (IS_ERR(torch_dev))
		pr_err("Failed to create device(torch)!\n");

	if (device_create_file(torch_dev, &dev_attr_torch_ctl) < 0)
		pr_err("Failed to create device file(%s)!\n", dev_attr_torch_ctl.attr.name); 

	wake_lock_init(&torch_wake_lock, WAKE_LOCK_SUSPEND, "torch_wake_lock");

	torch_wq = create_singlethread_workqueue("torch_wq");
	if (!torch_wq)
		pr_err("failed to allocate workqueue\n");

	INIT_WORK(&torch_work, time_expire_work_func);
	hrtimer_init(&torch_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	torch_timer.function = torch_flash_timer_func;
	
	return 0;
}

static int torch_flash_remove(struct platform_device *pdev)
{
	wake_lock_destroy(&torch_wake_lock);
	destroy_workqueue(torch_wq);
	return 0;
}

static struct platform_driver torch_flash_driver = {
	.probe		= torch_flash_probe,
	.remove		= torch_flash_remove,
	.driver		= {
		.name	= "torch-flash",
		.owner	= THIS_MODULE,
	},
};

static int __init torch_flash_init(void)
{
	torch_debug_msg("torch_flash_init\n");

	return platform_driver_register(&torch_flash_driver);
}

static void __exit torch_flash_exit(void)
{
	platform_driver_unregister(&torch_flash_driver);
}

module_init(torch_flash_init);
module_exit(torch_flash_exit);

MODULE_DESCRIPTION("Samsung Electronics Corp Torch driver");
MODULE_LICENSE("GPL");
