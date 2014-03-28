/*
 * =====================================================================
 *
 *       Filename:  cmc624_sysfs.c
 *
 *    Description:  SYSFS Node control driver
 *
 *        Version:  1.0
 *        Created:  2011 05/30 15:04:45
 *       Revision:  none
 *       Compiler:  arm-linux-gcc
 *
 *         Author:  Park Gyu Tae (),
 *        Company:  Samsung Electronics
 *
 * =====================================================================

Copyright (C) 2011, Samsung Electronics. All rights reserved.

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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *

*/
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/vmalloc.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/firmware.h>
#include <linux/wakelock.h>
#include <linux/blkdev.h>
#include <linux/i2c.h>
#include <mach/gpio.h>

#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/earlysuspend.h>
#include "msm_fb.h"
#include <linux/time.h>
#include <linux/timer.h>
#include <linux/wakelock.h>
#include <linux/i2c/samsung_cmc624.h>
#include <linux/mfd/pm8xxx/pm8038.h>

static struct class *mdnie_class;
struct device *tune_cmc624_dev;
struct device *tune_mdnie_dev_cmc;

extern struct str_blind_tuning blind_tune_value;

#define FALSE 0
#define TRUE  1
#define DUMP_CMC624_REGISTER 0
/* ##########################################################
 * #
 * # LCD POWER on / Off Sysfs node
 * #
 * ##########################################################*/

static ssize_t lcd_power_file_cmd_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	pr_debug("called %s\n", __func__);
	return sprintf(buf, "%u\n", cmc624_state.suspended);
}
static ssize_t lcd_power_file_cmd_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	int value;

	sscanf(buf, "%d", &value);

	pr_debug("[lcd_power] in lcd_power_file_cmd_store," \
		" input value = %d\n", value);

	if ((cmc624_state.suspended == TRUE) && (value == 1)) {
		samsung_cmc624_on(value);
		pr_debug("[lcd_power on] <= value : %d\n", value);
		cmc624_state.suspended = FALSE;
	} else if ((cmc624_state.suspended == FALSE) && (value == 0)) {
		samsung_cmc624_on(value);
		pr_debug("[lcd_power off] <= value : %d\n", value);
		cmc624_state.suspended = TRUE;
	} else
		pr_debug("[lcd_power] lcd is already = %d\n",
		cmc624_state.suspended);

	return size;
}

static DEVICE_ATTR(lcd_power, 0664, lcd_power_file_cmd_show,
	lcd_power_file_cmd_store);

/* ##########################################################
 * #
 * # LCD Type Sysfs node
 * #
 * ##########################################################*/
static const char lcdtype_name[][64] = {
		"SEC_LTN089AL03-802",
		};

static ssize_t lcdtype_file_cmd_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	pr_debug("type: %s\n", lcdtype_name[0]);

	return sprintf(buf, lcdtype_name[0]);
}

static DEVICE_ATTR(lcdtype, 0664, lcdtype_file_cmd_show, NULL);

/* ##########################################################
 * #
 * # Scenario change Sysfs node
 * #
 * ##########################################################*/
static ssize_t scenario_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	pr_debug("[CMC624:info] : %s called\n", __func__);
	return sprintf(buf, "Current Scenario Mode : %d\n",
		cmc624_state.scenario);
}


static ssize_t scenario_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	int ret;
	int value;

	sscanf(buf, "%d", &value);
	pr_debug("[CMC624:INFO] set scenario mode : %d\n", value);

	if (value < mDNIe_UI_MODE || ((value >= MAX_mDNIe_MODE)
		&& (value < COLOR_TONE_1)) || value >= COLOR_TONE_MAX) {
		pr_debug("[CMC624:ERROR] : wrong scenario mode value : %d\n",
			value);
		return size;
	}

#if defined(CONFIG_TDMB)
	if (value == mDNIe_DMB_MODE)
		value = 10;
#endif

	if (cmc624_state.suspended == TRUE) {
		if (value >= COLOR_TONE_1)
			cmc624_state.browser_scenario = value;
		else
			cmc624_state.scenario = value;
		return size;
	}
	if (value >= COLOR_TONE_1)
		ret = apply_browser_tune_value(value, 0);
	else
		ret = apply_main_tune_value(value, cmc624_state.background,
			cmc624_state.cabc_mode, 0);

	if (ret != 0)
		pr_debug("[CMC624:ERROR] ERROR : set main tune value faild\n");

	return size;
}
static DEVICE_ATTR(scenario, 0664, scenario_show, scenario_store);

/* ##########################################################
 * #
 * # Tuning Sysfs node
 * #
 * ##########################################################*/
#define MAX_FILE_NAME 50
static int tuning_enable;
static char tuning_filename[MAX_FILE_NAME] = {0, };
static ssize_t tuning_show(struct device *dev,
		struct device_attribute *attr, char *buf)

{
	int ret = 0;
	ret = sprintf(buf, "Tunned File Name : %s\n", tuning_filename);

	return ret;
}


static ssize_t tuning_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	memset(tuning_filename, 0, sizeof(tuning_filename));
	strcpy(tuning_filename, "/sdcard/tuning/");
	strncat(tuning_filename, buf, size-1);

	pr_debug("[CMC624:INFO]:%s:%s\n", __func__, tuning_filename);

	if (load_tuning_data(tuning_filename) <= 0) {
		pr_debug("[CMC624:ERROR]:load_tunig_data() failed\n");
		return size;
	}
	tuning_enable = 1;
	return size;
}

static DEVICE_ATTR(tuning, 0664, tuning_show, tuning_store);

/* ##########################################################
 * #
 * # MDNIE OVE Sysfs node
 * #
 * ##########################################################*/
static ssize_t outdoor_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "Current OVE Value : %s\n",
		(cmc624_state.outdoor == 0) ? "Disabled" : "Enabled");
}


static ssize_t outdoor_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	int ret;
	int value;

	sscanf(buf, "%d", &value);
	pr_debug("[CMC624:INFO] set outdoor : %d\n", value);

	if (value < OUTDOOR_OFF_MODE || value >= MAX_OUTDOOR_MODE) {
		pr_debug("[CMC624:ERROR] : wrong outdoor mode value : %d\n",
			value);
		return size;
	}

	if (cmc624_state.suspended == TRUE) {
		cmc624_state.outdoor = value;
		return size;
	}

	ret = apply_sub_tune_value(cmc624_state.temperature, value,
		cmc624_state.cabc_mode, 0);
	if (ret != 0)
		pr_debug("[CMC624:ERROR] ERROR : set sub tune value faild\n");

	return size;
}

static DEVICE_ATTR(outdoor, 0664, outdoor_show, outdoor_store);

/* ##########################################################
 * #
 * # MDNIE CABC Sysfs node
 * #
 * ##########################################################*/
static ssize_t cabc_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "Current CABC Value : %s\n",
		(cmc624_state.cabc_mode == 0) ? "Disabled" : "Enabled");
}

static ssize_t cabc_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	int value;

	sscanf(buf, "%d", &value);
	pr_debug("[CMC624:INFO] set cabc : %d\n", value);

	if (value < CABC_OFF_MODE || value >= MAX_CABC_MODE) {
		pr_debug("[CMC624:ERROR] : wrong cabc mode value : %d\n",
			value);
		return size;
	}

	if (cmc624_state.suspended == TRUE) {
		cmc624_state.cabc_mode = value;
		return size;
	}

	cabc_onoff_ctrl(value);
	
	return size;
}

static DEVICE_ATTR(cabc, 0664, cabc_show, cabc_store);

#if defined(CONFIG_FB_EBOOK_PANEL_SCENARIO)
/* ##########################################################
 * #
 * # MDNIE EBOOK Sysfs node
 * #
 * ##########################################################*/
static ssize_t ebook_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "Current ebook Value : %s\n",
		(cmc624_state.ebook == 0) ? "Disabled" : "Enabled");
}

static ssize_t ebook_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	int ret;
	int value;

	sscanf(buf, "%d", &value);
	pr_debug("[CMC624:INFO] set ebook : %d\n", value);

	if (value < EBOOK_OFF || value >= MAX_EBOOK_MODE) {
		pr_debug("[CMC624:ERROR] : wrong ebook mode value : %d\n",
			value);
		return size;
	}

	if (cmc624_state.suspended == TRUE) {
		cmc624_state.ebook = value;
		return size;
	}
	ret = apply_ebook_tune_value(value, cmc624_state.cabc_mode);
	if (ret != 0)
		pr_debug("[CMC624:ERROR] ERROR : set ebook value faild\n");

	return size;
}

static DEVICE_ATTR(ebook, 0664, ebook_show, ebook_store);
#endif

/* ##########################################################
 * #
 * # MDNIE Accessibility Sysfs node
 * #
 * ##########################################################*/

static ssize_t accessibility_show(struct device *dev,
			struct device_attribute *attr,
			char *buf)
{
	char *pos = buf;
	//unsigned short *wbuf;
	struct cmc624RegisterSet *wbuf;
	int i = 0;

	pr_debug("[CMC624:INFO] %s\n", __func__);

	pos += sprintf(pos, "%d, ", cmc624_state.blind);
	if (cmc624_state.blind == COLOR_BLIND) {
			wbuf = blind_tune_value.value[cmc624_state.cabc_mode].value;
			while ( i != MDNIE_COLOR_BLIND_CMD) {
				if (likely(wbuf[i].Data >= 0x71 && wbuf[i].Data<= 0x79))
					pos += sprintf(pos, "0x%04x, ", wbuf[i+1].Data);
				i++;
			}
	}
	pos += sprintf(pos, "\n");

	return pos - buf;
}

static ssize_t accessibility_store(struct device *dev,
			struct device_attribute *attr,
			const char *buf, size_t size)
{

	unsigned int value, s[9], cabc, i = 0;
	int ret;
	struct cmc624RegisterSet *wbuf;

	ret = sscanf(buf, "%d %x %x %x %x %x %x %x %x %x",
		&value, &s[0], &s[1], &s[2], &s[3],
		&s[4], &s[5], &s[6], &s[7], &s[8]);

	pr_debug("[CMC624:INFO] set accesibility : %d\n", value);
	pr_info("%s cmd_value : %d size : %d\n", __func__, value, size);

	if (ret < 0)
		return ret;
	else {
		if (value >= ACCESSIBILITY_MAX)
			value = ACCESSIBILITY_OFF;
		
		if (value == COLOR_BLIND) {
			if (ret != 10)
				return -EINVAL;
			for (cabc = 0; cabc < MAX_CABC_MODE; cabc++) {
					wbuf = blind_tune_value.value[cabc].value;
					while ( i != MDNIE_COLOR_BLIND_CMD) {
						if (likely(wbuf[i].RegAddr >= 0x71 && wbuf[i].RegAddr <= 0x79)) {
							wbuf[i].Data = s[wbuf[i].RegAddr - 0x71];
						}
						i++;
//						pr_info("%s : wbuf[%d](0x%04x)\n", __func__, i, wbuf[i].Data );
					}
				i = 0;
			}
			ret = apply_blind_tune_value(value, cmc624_state.cabc_mode);
			if (ret != 0) {
				pr_err("[CMC624:ERROR] ERROR : set blind value failed.\n");
			}
			
			cmc624_state.blind = value;
			cmc624_state.negative = ACCESSIBILITY_OFF;
		} else if(value == NEGATIVE) {
			ret = apply_negative_tune_value(value, cmc624_state.cabc_mode);
			if (ret != 0) {
				pr_err("[CMC624:ERROR] ERROR : set negative value failed.\n");
			}

			cmc624_state.blind = ACCESSIBILITY_OFF;
			cmc624_state.negative = value;
		} else if (value == ACCESSIBILITY_OFF) {
			ret = apply_blind_tune_value(value, cmc624_state.cabc_mode);
			if (ret != 0) {
				pr_err("[CMC624:ERROR] ERROR : set blind value failed.\n");
			}
			
			ret = apply_negative_tune_value(value, cmc624_state.cabc_mode);
			if (ret != 0) {
				pr_err("[CMC624:ERROR] ERROR : set negative value failed.\n");
			}
		} else
			pr_info("%s ACCESSIBILITY_MAX", __func__);
	}
	return size;
}
static DEVICE_ATTR(accessibility, 0664, accessibility_show, accessibility_store);


/* ##########################################################
 * #
 * # MDNIE Temprature Sysfs node
 * #
 * ##########################################################*/
static ssize_t mdnie_temp_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	const char temp_name[MAX_TEMP_MODE][16] = {
		"STANDARD",
		"WARM",
		"COLD",
	};

	if (cmc624_state.temperature >= MAX_TEMP_MODE) {
		pr_debug("[CMC624:ERROR] : wrong color temperature mode value : %d\n",
			cmc624_state.temperature);
		return 0;
	}

	return sprintf(buf, "Current Color Temperature Mode : %s\n",
		temp_name[cmc624_state.temperature]);
}


static ssize_t mdnie_temp_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	int ret;
	int value;

	sscanf(buf, "%d", &value);
	pr_debug("[CMC624:INFO] set color temperature : %d\n", value);

	if (value < TEMP_STANDARD || value >= MAX_TEMP_MODE) {
		pr_debug("[CMC624:ERROR] : wrong color temperature mode value :%d\n"
		, value);
		return size;
	}

	if (cmc624_state.suspended == TRUE) {
		cmc624_state.temperature = value;
		return size;
	}
	ret = apply_sub_tune_value(value, cmc624_state.outdoor,
		cmc624_state.cabc_mode, 0);
	if (ret != 0)
		pr_debug("[CMC624:ERROR] ERROR : set sub tune value faild\n");
	return size;
}

static DEVICE_ATTR(mdnie_temp, 0664, mdnie_temp_show, mdnie_temp_store);

/* ##########################################################
 * #
 * # MDNIE BG Sysfs node
 * #
 * ##########################################################*/

/* ##########################################################
 * #
 * #	0. Dynamic
 * #	1. Standard
 * #	2. Video
 * #	3. Natural
 * #
 * ##########################################################*/

static ssize_t mode_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	const char background_name[MAX_BACKGROUND_MODE][16] = {
		"STANDARD",
		"DYNAMIC",
		"MOVIE",
		"NATURAL",
	};

	if (cmc624_state.background >= MAX_BACKGROUND_MODE) {
		pr_debug("[CMC624:ERROR] : Undefined Background Mode : %d\n",
			cmc624_state.background);
		return 0;
	}
	pr_debug("%s, cmc624_state.backgroudn : %d\n",
		__func__, cmc624_state.background);
	return sprintf(buf, "Current Background Mode : %s\n",
		background_name[cmc624_state.background]);
}
static ssize_t mode_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	int ret;
	int value;

	sscanf(buf, "%d", &value);
	pr_debug("[CMC624:INFO] set background mode : %d\n", value);

	if (value < DYNAMIC_MODE || value >= MAX_BACKGROUND_MODE) {
		pr_debug("[CMC624:ERROR] : wrong backgound mode value : %d\n",
			value);
		return size;
	}

	if (cmc624_state.suspended == TRUE) {
		cmc624_state.background = value;
		return size;
	}
	ret = apply_main_tune_value(cmc624_state.scenario, value,
		cmc624_state.cabc_mode, 0);
	if (ret != 0)
		pr_debug("[CMC624:ERROR] ERROR : set main tune value faild\n");
	return size;
}


static DEVICE_ATTR(mode, 0664, mode_show, mode_store);

#if DUMP_CMC624_REGISTER
/* ##########################################################
 * #
 * # MDNIE DUMP Sysfs node
 * #   - dump cmc624 register
 * ##########################################################*/
static ssize_t mdnie_dump_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	pr_debug("##################################\n");
	pr_debug(" CMC624 Register DUMP\n");
	pr_debug("##################################\n");

	dump_cmc624_register();

	return 0;
}
static ssize_t mdnie_dump_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	int ret;
	int value;
	pr_debug("[CMC624:INFO] dump\n");
	return size;
}

static DEVICE_ATTR(mdnie_dump, 0664, mdnie_dump_show, mdnie_dump_store);
#endif

int cmc624_sysfs_init(void)
{

	int ret = 0;
	/*  1. CLASS Create
	 *  2. Device Create
	 *  3. node create
	 *   - bypass on/off node
	 *   - cabc on/off node
	 *   - lcd_power node
	 *   - scenario node
	 *   - tuning node
	 *   - mdnie_outdoor node
	 *   - mdnie_bg node*/
	mdnie_class = class_create(THIS_MODULE, "mdnie");
	if (IS_ERR(mdnie_class)) {
		pr_debug("Failed to create class(mdnie_class)!!\n");
		ret = -1;
	}
	tune_mdnie_dev_cmc = device_create(mdnie_class, NULL, 0, NULL, "mdnie");
		if (IS_ERR(tune_mdnie_dev_cmc)) {
			pr_debug("Failed to create device(tune_mdnie_dev_cmc)!!");
			ret = -1;
		}
		if (device_create_file(tune_mdnie_dev_cmc,
			&dev_attr_lcd_power) < 0) {
			pr_debug("Failed to create device file!(%s)!\n",\
				dev_attr_lcd_power.attr.name);
		ret = -1;
	}
	if (device_create_file(tune_mdnie_dev_cmc, &dev_attr_lcdtype) < 0) {
		pr_debug("Failed to create device file!(%s)!\n",\
			dev_attr_lcdtype.attr.name);
		ret = -1;
	}
	if (device_create_file(tune_mdnie_dev_cmc, &dev_attr_scenario) < 0) {
		pr_debug("Failed to create device file!(%s)!\n",\
			dev_attr_scenario.attr.name);
		ret = -1;
	}
	if (device_create_file(tune_mdnie_dev_cmc, &dev_attr_tuning) < 0) {
		pr_debug("Failed to create device file(%s)!\n",\
			dev_attr_tuning.attr.name);
		ret = -1;
	}
	if (device_create_file(tune_mdnie_dev_cmc, &dev_attr_outdoor) < 0) {
		pr_debug("[CMC624:ERROR] device_crate_filed(%s)\n",\
			dev_attr_outdoor.attr.name);
		ret = -1;
	}

	if (device_create_file(tune_mdnie_dev_cmc, &dev_attr_mdnie_temp) < 0) {
		pr_debug("[CMC624:ERROR] device_crate_filed(%s)\n",\
			dev_attr_mdnie_temp.attr.name);
		ret = -1;
	}
	if (device_create_file(tune_mdnie_dev_cmc, &dev_attr_mode) < 0) {
		pr_debug("[CMC624:ERROR] device_crate_filed(%s)\n",\
			dev_attr_mode.attr.name);
		ret = -1;
	}

#if defined(CONFIG_FB_EBOOK_PANEL_SCENARIO)
	if (device_create_file(tune_mdnie_dev_cmc, &dev_attr_ebook) < 0) {
		pr_debug("[CMC624:ERROR] device_create_file(%s)\n",\
			dev_attr_ebook.attr.name);
		ret = -1;
	}
#endif

	if (device_create_file(tune_mdnie_dev_cmc, &dev_attr_cabc) < 0) {
		pr_debug("[CMC624:ERROR] device_create_file(%s)\n",\
			dev_attr_cabc.attr.name);
		ret = -1;
	}

	if (device_create_file(tune_mdnie_dev_cmc, &dev_attr_accessibility) < 0) {
		pr_debug("[CMC624:ERROR] device_create_file(%s)\n",\
			dev_attr_accessibility.attr.name);
		ret = -1;
	}
#if DUMP_CMC624_REGISTER
	if (device_create_file(tune_mdnie_dev_cmc, &dev_attr_mdnie_dump) < 0) {
		pr_debug("[CMC624:ERROR] device_crate_filed(%s)\n"\
		, dev_attr_mdnie_dump.attr.name);
		ret = -1;
	}
#endif
	return 0;
}
