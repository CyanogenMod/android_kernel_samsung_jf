/*
 * driver/misc/sec_misc.c
 *
 * driver supporting miscellaneous functions for Samsung P-series device
 *
 * COPYRIGHT(C) Samsung Electronics Co., Ltd. 2006-2011 All Right Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <linux/module.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/vmalloc.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/firmware.h>
#include <linux/wakelock.h>
#include <linux/blkdev.h>
#include <mach/gpio.h>
#include <linux/sec_param.h>
#include <mach/sec_debug.h>
#define MOVINAND_CHECKSUM
#define RORY_CONTROL

static struct wake_lock sec_misc_wake_lock;

#ifdef MOVINAND_CHECKSUM
unsigned char emmc_checksum_done;
unsigned char emmc_checksum_pass;
#endif

static const struct file_operations sec_misc_fops = {
	.owner = THIS_MODULE,
	/*	.read = sec_misc_read,
		.ioctl = sec_misc_ioctl,
		.open = sec_misc_open,
		.release = sec_misc_release, */
};

static struct miscdevice sec_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "sec_misc",
	.fops = &sec_misc_fops,
};

#ifdef MOVINAND_CHECKSUM
static ssize_t emmc_checksum_done_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return snprintf(buf, sizeof(buf), "%d\n", emmc_checksum_done);
}

static ssize_t emmc_checksum_done_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	int state;

	if (sscanf(buf, "%i", &state) != 1 || (state < 0 || state > 1))
		return -EINVAL;

	emmc_checksum_done = (unsigned char)state;
	return size;
}

static DEVICE_ATTR(emmc_checksum_done, S_IRUGO | S_IWUSR ,
		emmc_checksum_done_show, emmc_checksum_done_store);

static ssize_t emmc_checksum_pass_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return snprintf(buf, sizeof(buf), "%d\n", emmc_checksum_pass);
}

static ssize_t emmc_checksum_pass_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	int state;

	if (sscanf(buf, "%i", &state) != 1 || (state < 0 || state > 1))
		return -EINVAL;

	emmc_checksum_pass = (unsigned char)state;
	return size;
}

static DEVICE_ATTR(emmc_checksum_pass, S_IRUGO | S_IWUSR ,
		emmc_checksum_pass_show, emmc_checksum_pass_store);
#endif /*MOVINAND_CHECKSUM*/


#ifdef RORY_CONTROL
static ssize_t rory_control_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	int rory_control;

	sec_get_param(param_rory_control, &rory_control);

	return snprintf(buf, sizeof(buf), "%d\n", rory_control);
}

static ssize_t rory_control_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	int rory_control;

	sscanf(buf, "%i", &rory_control);

	pr_info("rory control store ..... %d\n", rory_control);

	/* write to param */
	sec_set_param(param_rory_control, &rory_control);

	return size;
}

static DEVICE_ATTR(rory_control, S_IRUGO | S_IWUSR ,
		rory_control_show, rory_control_store);
#endif /*RORY_CONTROL*/

static unsigned int convert_debug_level_str(const char *str)
{
	if (strncasecmp(str, "0xA0A0", 6) == 0)
		return KERNEL_SEC_DEBUG_LEVEL_LOW;

	if (strncasecmp(str, "0xB0B0", 6) == 0)
		return KERNEL_SEC_DEBUG_LEVEL_MID;

	if (strncasecmp(str, "0xC0C0", 6) == 0)
		return KERNEL_SEC_DEBUG_LEVEL_HIGH;

	return 0;
}

static void convert_debug_level_int(unsigned int val, char *str)
{
	if (val == KERNEL_SEC_DEBUG_LEVEL_LOW) {
		strlcpy(str, "0xA0A0", sizeof("0xA0A0") + 1);
		return;
	}

	if (val == KERNEL_SEC_DEBUG_LEVEL_MID) {
		strlcpy(str, "0xB0B0", sizeof("0xB0B0") + 1);
		return;
	}

	if (val == KERNEL_SEC_DEBUG_LEVEL_HIGH) {
		strlcpy(str, "0xC0C0", sizeof("0xC0C0") + 1);
		return;
	}
}

static ssize_t debug_level_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	char buffer[7];
	convert_debug_level_int(kernel_sec_get_debug_level(), buffer);

	return snprintf(buf, sizeof(buffer)+1, "%s\n", buffer);

}

static ssize_t debug_level_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	int sec_debug_level = convert_debug_level_str(buf);

	if (sec_debug_level == 0)
		return -EINVAL;

	kernel_sec_set_debug_level(sec_debug_level);

	return size;

}

static DEVICE_ATTR(debug_level, S_IRUGO | S_IWUSR ,
		debug_level_show, debug_level_store);

#if defined(CONFIG_MACH_APEXQ) || defined(CONFIG_MACH_AEGIS2)
static ssize_t slideCount_show
	(struct device *dev, struct device_attribute *attr, char *buf)
{
	int slideCount;

	sec_get_param(param_slideCount, &slideCount);

	return snprintf(buf, sizeof(buf), "%d\n", slideCount);
}

static ssize_t slideCount_store
	(struct device *dev, struct device_attribute *attr,\
		const char *buf, size_t size)
{
	int slideCount;

	sscanf(buf, "%i", &slideCount);
	sec_set_param(param_slideCount, &slideCount);

	return size;
}
static DEVICE_ATTR(slideCount, S_IRUGO | S_IWUSR | S_IWGRP,\
			slideCount_show, slideCount_store);
#endif

struct device *sec_misc_dev;

static struct device_attribute *sec_misc_attrs[] = {
	&dev_attr_emmc_checksum_done,
	&dev_attr_emmc_checksum_pass,
	&dev_attr_rory_control,
	&dev_attr_debug_level,
#if defined(CONFIG_MACH_APEXQ) || defined(CONFIG_MACH_AEGIS2)
	&dev_attr_slideCount,
#endif
};

static int __init sec_misc_init(void)
{
	int ret = 0;
	int i;

	ret = misc_register(&sec_misc_device);
	if (ret < 0) {
		printk(KERN_ERR "misc_register failed!\n");
		goto failed_register_misc;
	}

	sec_misc_dev = device_create(sec_class, NULL, 0, NULL, "sec_misc");
	if (IS_ERR(sec_misc_dev)) {
		printk(KERN_ERR "failed to create device!\n");
		ret = -ENODEV;
		goto failed_create_device;
	}

	for (i = 0; i < ARRAY_SIZE(sec_misc_attrs) ; i++) {
		ret = device_create_file(sec_misc_dev, sec_misc_attrs[i]);
		if (ret < 0) {
			pr_err("failed to create device file - %s\n",
					dev_attr_emmc_checksum_done.attr.name);
			goto failed_create_device_file;
		}
	}

	wake_lock_init(&sec_misc_wake_lock, WAKE_LOCK_SUSPEND, "sec_misc");

	return 0;

failed_create_device_file:
	if (i) {
		for (--i; i >= 0; i--)
			device_remove_file(sec_misc_dev, sec_misc_attrs[i]);
	}
failed_create_device:
	misc_deregister(&sec_misc_device);
failed_register_misc:
	return ret;
}

static void __exit sec_misc_exit(void)
{
	wake_lock_destroy(&sec_misc_wake_lock);
}

module_init(sec_misc_init);
module_exit(sec_misc_exit);

/* Module information */
MODULE_AUTHOR("Samsung");
MODULE_DESCRIPTION("Samsung PX misc. driver");
MODULE_LICENSE("GPL");
