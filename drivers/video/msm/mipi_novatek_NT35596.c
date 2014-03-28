/* Copyright (c) 2010-2012, Code Aurora Forum. All rights reserved.
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
#define DEBUG	1
#include <linux/lcd.h>
#include <linux/leds.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include "msm_fb.h"
#include "mipi_dsi.h"
#include "mipi_novatek_NT35596.h"
// #include "mdp4_video_enhance.h" //Dibya

#ifdef CONFIG_SAMSUNG_CMC624
#include <linux/i2c/samsung_cmc624.h>
#endif

#include <linux/delay.h>
#include <linux/pwm.h>
#include <linux/gpio.h>
#include <mach/msm8930-gpio.h>


static struct mipi_novatek_driver_data msd;
static unsigned int recovery_boot_mode;
extern unsigned int system_rev;

enum {
    LCD_STATUS_OFF = 0,
    LCD_STATUS_ON,
};
#if defined(CONFIG_MACH_MELIUS)
int is_lcd_on;
#else
static int is_lcd_on;
#endif
#ifdef CONFIG_MACH_JF
#include <linux/i2c/synaptics_rmi.h>
#endif

#ifdef CONFIG_MACH_MELIUS
#include <linux/i2c/synaptics_rmi_msm8930.h>
#endif


#define WA_FOR_FACTORY_MODE
#define READ_MTP_ONCE


#ifndef CONFIG_SAMSUNG_CMC624
static char WRDISBV[] = {
	0x51,
	0xFF,
	0x00, 0x00,
};

static struct dsi_cmd_desc novatek_video_backlight_cmds[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(WRDISBV), WRDISBV},
};
#endif

static int mipi_novatek_disp_send_cmd(struct msm_fb_data_type *mfd,
				       enum mipi_novatek_cmd_list cmd,
				       unsigned char lock)
{
	struct dsi_cmd_desc *cmd_desc;
	int cmd_size = 0;

	if (lock)
		mutex_lock(&mfd->dma->ov_mutex);

	pr_info("%s : %s\n", __func__, msd.mpd->panel_name);
	pr_info("%s cmd = 0x%x\n", __func__, cmd);

	switch (cmd) {
	case PANEL_READY_TO_ON:
		cmd_desc = msd.mpd->ready_to_on.cmd;
		cmd_size = msd.mpd->ready_to_on.size;
		break;
	case PANEL_READY_TO_OFF:
		cmd_desc = msd.mpd->ready_to_off.cmd;
		cmd_size = msd.mpd->ready_to_off.size;
		break;
	case PANEL_ON:
		cmd_desc = msd.mpd->on.cmd;
		cmd_size = msd.mpd->on.size;
		break;
	case PANEL_OFF:
		cmd_desc = msd.mpd->off.cmd;
		cmd_size = msd.mpd->off.size;
		break;
	case PANEL_LATE_ON:
		cmd_desc = msd.mpd->late_on.cmd;
		cmd_size = msd.mpd->late_on.size;
		break;
	case PANEL_EARLY_OFF:
		cmd_desc = msd.mpd->early_off.cmd;
		cmd_size = msd.mpd->early_off.size;
		break;
	case MTP_READ_ENABLE:
		cmd_desc = msd.mpd->mtp_read_enable.cmd;
		cmd_size = msd.mpd->mtp_read_enable.size;
		break;

	default:
		goto unknown_command;
		;
	}
	
	pr_info("%s cmd size = %d\n", __func__, cmd_size);

	if (!cmd_size)
		goto unknown_command;

	if (lock) {
		mipi_dsi_mdp_busy_wait();
		mipi_dsi_cmds_tx(&msd.novatek_tx_buf, cmd_desc, cmd_size);

		mutex_unlock(&mfd->dma->ov_mutex);
	} else {
		mipi_dsi_mdp_busy_wait();
		mipi_dsi_cmds_tx(&msd.novatek_tx_buf, cmd_desc, cmd_size);
	}

/*	wake_unlock(&idle_wake_lock);*//*temp*/

	pr_info("%s end\n", __func__);

	return 0;

unknown_command:
	if (lock)
		mutex_unlock(&mfd->dma->ov_mutex);

/*	wake_unlock(&idle_wake_lock);*//*temp*/

	return 0;
}

/**
 * LCD ON.
 *
 * Set LCD On via MIPI interface .
 * Set Backlight on.
*/
static int mipi_novatek_disp_on(struct platform_device *pdev)
{
	struct msm_fb_data_type *mfd;
	struct mipi_panel_info *mipi;

	int disp_bl_cont_gpio;
	disp_bl_cont_gpio = (system_rev>1)?GPIO_LCD_BOOSTER_EN:GPIO_LCD_BOOSTER_EN_EMUL;

	pr_info("****** %s *******\n", __func__);

	mfd = platform_get_drvdata(pdev);
	if (unlikely(!mfd))
		return -ENODEV;
	if (unlikely(mfd->key != MFD_KEY))
		return -EINVAL;

	mipi = &mfd->panel_info.mipi;

	if (mipi->mode == DSI_VIDEO_MODE)
		mipi_novatek_disp_send_cmd(mfd, PANEL_ON, false);

#if defined(CONFIG_FB_MDP4_ENHANCE)
	is_negativeMode_on();
#endif
	mipi_novatek_disp_send_cmd(mfd, PANEL_READY_TO_ON, false);
	pr_debug("%s: DISP_BL_CONT_GPIO High\n", __func__);
	gpio_set_value(disp_bl_cont_gpio , 1);

	is_lcd_on = LCD_STATUS_ON;

#if !defined(CONFIG_HAS_EARLYSUSPEND)
	mipi_novatek_disp_send_cmd(mfd, PANEL_LATE_ON, false);
#endif

#if defined(CONFIG_MIPI_SAMSUNG_ESD_REFRESH)
	set_esd_enable();
#endif

	pr_info("%s:Display on completed\n", __func__);
return 0;
}
static int mipi_novatek_disp_off(struct platform_device *pdev)
{
	int disp_bl_cont_gpio;
	struct msm_fb_data_type *mfd;

#if defined(CONFIG_MIPI_SAMSUNG_ESD_REFRESH)
	set_esd_disable();
#endif	
	disp_bl_cont_gpio = (system_rev>1)?GPIO_LCD_BOOSTER_EN:GPIO_LCD_BOOSTER_EN_EMUL;
	mfd = platform_get_drvdata(pdev);
	
	if (unlikely(!mfd))
		return -ENODEV;
	if (unlikely(mfd->key != MFD_KEY))
		return -EINVAL;
	pr_debug("%s: DISP_BL_CONT_GPIO low\n", __func__);
	gpio_set_value(disp_bl_cont_gpio, 0);

	mipi_novatek_disp_send_cmd(mfd, PANEL_READY_TO_OFF, false);
	mipi_novatek_disp_send_cmd(mfd, PANEL_OFF, false);


	pr_info("%s:Display off completed\n", __func__);
	is_lcd_on = LCD_STATUS_OFF;

	return 0;
}

static void __devinit mipi_novatek_disp_shutdown(struct platform_device *pdev)
{
	static struct mipi_dsi_platform_data *mipi_dsi_pdata;
	struct msm_fb_data_type *mfd;

	if (pdev->id != 0)
		return;

	mfd = platform_get_drvdata(pdev);

	mipi_dsi_pdata = pdev->dev.platform_data;

	if (mipi_dsi_pdata == NULL) {
		pr_err("LCD Power off failure: No Platform Data\n");
		return;
	}

	if (mfd) {
		mipi_novatek_disp_send_cmd(mfd, PANEL_READY_TO_OFF, false);
		mipi_novatek_disp_send_cmd(mfd, PANEL_OFF, false);
	}
}

static void mipi_novatek_disp_set_backlight(struct msm_fb_data_type *mfd)
{
	struct mipi_panel_info *mipi;
	static int bl_level_old;

	pr_info("%s : level (%d)\n", __func__, mfd->bl_level);

#if defined(CONFIG_MIPI_SAMSUNG_ESD_REFRESH)
	if (msd.esd_refresh == true) {
		pr_debug("ESD Refresh on going cannot set backlight\n");
		goto end;
	}
#endif
	mipi  = &mfd->panel_info.mipi;
	if (bl_level_old == mfd->bl_level)
		goto end;
	if (!mfd->panel_power_on)
		goto end;

#ifdef CONFIG_SAMSUNG_CMC624
	cmc624_pwm_control_cabc(mfd->bl_level);
#else
	mutex_lock(&mfd->dma->ov_mutex);
	/* mdp4_dsi_cmd_busy_wait: will turn on dsi clock also */
	mipi_dsi_mdp_busy_wait();

	WRDISBV[1] = (unsigned char)mfd->bl_level;

	mipi_dsi_cmds_tx(&msd.novatek_tx_buf, novatek_video_backlight_cmds,
		ARRAY_SIZE(novatek_video_backlight_cmds));
	mutex_unlock(&mfd->dma->ov_mutex);
#endif
	bl_level_old = mfd->bl_level;
#if defined(CONFIG_TOUCHSCREEN_SYNAPTICS_I2C_RMI)
	synaptics_inform_callbacks(RMI4_CALLBACK_LCD, is_lcd_on);
#endif
end:
	return;
}

#if defined(CONFIG_HAS_EARLYSUSPEND)
static void mipi_novatek_disp_early_suspend(struct early_suspend *h)
{
	struct msm_fb_data_type *mfd;
	pr_info("%s", __func__);

	mfd = platform_get_drvdata(msd.msm_pdev);
	if (unlikely(!mfd)) {
		pr_info("%s NO PDEV.\n", __func__);
		return;
	}
	if (unlikely(mfd->key != MFD_KEY)) {
		pr_info("%s MFD_KEY is not matched.\n", __func__);
		return;
	}

	mipi_novatek_disp_send_cmd(mfd, PANEL_EARLY_OFF, true);

	mfd->resume_state = MIPI_SUSPEND_STATE;
	
#if defined(CONFIG_MIPI_SAMSUNG_ESD_REFRESH)
	set_esd_disable();
#endif	
}

static void mipi_novatek_disp_late_resume(struct early_suspend *h)
{
	struct msm_fb_data_type *mfd;

	mfd = platform_get_drvdata(msd.msm_pdev);
	if (unlikely(!mfd)) {
		pr_info("%s NO PDEV.\n", __func__);
		return;
	}
	if (unlikely(mfd->key != MFD_KEY)) {
		pr_info("%s MFD_KEY is not matched.\n", __func__);
		return;
	}

	mfd->resume_state = MIPI_RESUME_STATE;
	pr_info("%s", __func__);
}
#endif
#if defined(CONFIG_MIPI_SAMSUNG_ESD_REFRESH)
void set_esd_refresh(boolean stat)
{
	msd.esd_refresh = stat;
}
boolean get_esd_refresh_stat(void)
{
	return msd.esd_refresh;
}
#endif

#if defined(CONFIG_LCD_CLASS_DEVICE)
#ifdef WA_FOR_FACTORY_MODE
static ssize_t mipi_novatek_disp_get_power(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct msm_fb_data_type *mfd;
	int rc;

	mfd = platform_get_drvdata(msd.msm_pdev);
	if (unlikely(!mfd))
		return -ENODEV;
	if (unlikely(mfd->key != MFD_KEY))
		return -EINVAL;

	rc = sprintf(buf, "%d\n", mfd->panel_power_on);
	pr_info("mipi_novatek_disp_get_power(%d)\n", mfd->panel_power_on);

	return rc;
}

static ssize_t mipi_novatek_disp_set_power(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct msm_fb_data_type *mfd;
	unsigned int power;

	mfd = platform_get_drvdata(msd.msm_pdev);

	if (sscanf(buf, "%u", &power) != 1)
		return -EINVAL;

	if (power == mfd->panel_power_on)
		return 0;

	if (power) {
		mfd->fbi->fbops->fb_blank(FB_BLANK_UNBLANK, mfd->fbi);
		mfd->fbi->fbops->fb_pan_display(&mfd->fbi->var, mfd->fbi);
		mipi_novatek_disp_send_cmd(mfd, PANEL_LATE_ON, true);
		mipi_novatek_disp_set_backlight(mfd);
	} else {
		mfd->fbi->fbops->fb_blank(FB_BLANK_POWERDOWN, mfd->fbi);
	}

	pr_info("mipi_novatek_disp_set_power\n");

	return size;
}
#else
static int mipi_novatek_disp_get_power(struct lcd_device *dev)
{
	struct msm_fb_data_type *mfd;

	mfd = platform_get_drvdata(msd.msm_pdev);
	if (unlikely(!mfd))
		return -ENODEV;
	if (unlikely(mfd->key != MFD_KEY))
		return -EINVAL;

	pr_info("mipi_novatek_disp_get_power(%d)\n", mfd->panel_power_on);

	return mfd->panel_power_on;
}

static int mipi_novatek_disp_set_power(struct lcd_device *dev, int power)
{
	struct msm_fb_data_type *mfd;

	mfd = platform_get_drvdata(msd.msm_pdev);

	if (power == mfd->panel_power_on)
		return 0;

	if (power) {
		mfd->fbi->fbops->fb_blank(FB_BLANK_UNBLANK, mfd->fbi);
		mfd->fbi->fbops->fb_pan_display(&mfd->fbi->var, mfd->fbi);
		mipi_novatek_disp_send_cmd(mfd, PANEL_LATE_ON, true);
		mipi_novatek_disp_backlight(mfd);
	} else {
		mfd->fbi->fbops->fb_blank(FB_BLANK_POWERDOWN, mfd->fbi);
	}

	pr_info("mipi_novatek_disp_set_power\n");
	return 0;
}
#endif

static ssize_t mipi_novatek_lcdtype_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	char temp[20];

	snprintf(temp, strnlen(msd.mpd->panel_name, 20) + 1,
						msd.mpd->panel_name);
	strlcat(buf, temp, 20);
	return strnlen(buf, 20);
}

static ssize_t mipi_novatek_disp_gamma_mode_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	int rc;

	rc = sprintf(buf, "%d\n", msd.dstat.gamma_mode);
	pr_info("gamma_mode: %d\n", *buf);

	return rc;
}

static ssize_t mipi_novatek_disp_gamma_mode_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	if (sysfs_streq(buf, "1") && !msd.dstat.gamma_mode) {
		/* 1.9 gamma */
		msd.dstat.gamma_mode = GAMMA_1_9;
	} else if (sysfs_streq(buf, "0") && msd.dstat.gamma_mode) {
		/* 2.2 gamma */
		msd.dstat.gamma_mode = GAMMA_2_2;
	} else {
		pr_info("%s: Invalid argument!!", __func__);
	}

	return size;
}

#if defined(CONFIG_MACH_MELIUS)
static ssize_t siop_enable_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	pr_debug("Current CABC value : %s\n", 
		(cmc624_state.cabc_mode == 0) ? "Disabled" : "Enabled");
	return sprintf(buf, "%d\n", cmc624_state.cabc_mode);
}

static ssize_t siop_enable_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
/* Disable siop for eur model */
#if 1/*common for melius*/
	return size;
#else
	int value;
	sscanf(buf, "%2d", &value);

	if (value < CABC_OFF_MODE || value >= MAX_CABC_MODE) {
		pr_debug("[CMC624:ERROR] : wrong cabc mode value : %d\n",
			value);
		return size;
	}

	if (cmc624_state.suspended == TRUE) {
		cmc624_state.cabc_mode = value;
		return size;
	}

	if (msd.dstat.auto_brightness == 0) {
		cabc_onoff_ctrl(value);
		pr_debug("[CMC624:INFO] set cabc by siop : %d\n", value);
	} else {
		pr_debug("[CMC624:INFO] cabc already set by settings : %d\n", msd.dstat.auto_brightness);
	}

	return size;
#endif
}
#endif


static ssize_t mipi_novatek_disp_acl_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	int rc;

	rc = sprintf(buf, "%d\n", msd.dstat.acl_on);
	pr_info("acl status: %d\n", *buf);

	return rc;
}

static ssize_t mipi_novatek_disp_acl_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct msm_fb_data_type *mfd;

	mfd = platform_get_drvdata(msd.msm_pdev);

	if (!mfd->panel_power_on) {
		pr_info("%s: panel is off state. updating state value.\n",
			__func__);
		if (sysfs_streq(buf, "1") && !msd.dstat.acl_on)
			msd.dstat.acl_on = true;
		else if (sysfs_streq(buf, "0") && msd.dstat.acl_on)
			msd.dstat.acl_on = false;
		else
			pr_info("%s: Invalid argument!!", __func__);
	} else {

#ifdef CONFIG_ACL_FUNCTION /*temp*/
		if (sysfs_streq(buf, "1") && !msd.dstat.acl_on) {
			if (msd.mpd->set_acl(mfd->bl_level))
				mipi_novatek_disp_send_cmd(
					mfd, PANEL_ACL_OFF, true);
			else {
				mipi_novatek_disp_send_cmd(
					mfd, PANEL_ACL_ON, true);
				mipi_novatek_disp_send_cmd(
					mfd, PANEL_ACL_UPDATE, true);
			}
			msd.dstat.acl_on = true;
		} else if (sysfs_streq(buf, "0") && msd.dstat.acl_on) {
			mipi_novatek_disp_send_cmd(mfd, PANEL_ACL_OFF, true);
			msd.dstat.acl_on = false;
		} else {
			pr_info("%s: Invalid argument!!", __func__);
		}
#endif
	}

	return size;
}

static ssize_t mipi_novatek_auto_brightness_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	int rc;

	rc = sprintf(buf, "%d\n",
					msd.dstat.auto_brightness);
	pr_info("auot_brightness: %d\n", *buf);

	return rc;
}

static ssize_t mipi_novatek_auto_brightness_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct msm_fb_data_type *mfd;
	mfd = platform_get_drvdata(msd.msm_pdev);

	if (sysfs_streq(buf, "0"))
		msd.dstat.auto_brightness = 0;
	else if (sysfs_streq(buf, "1"))
		msd.dstat.auto_brightness = 1;
	else if (sysfs_streq(buf, "2"))
		msd.dstat.auto_brightness = 2;
	else if (sysfs_streq(buf, "3"))
		msd.dstat.auto_brightness = 3;
	else if (sysfs_streq(buf, "4"))
		msd.dstat.auto_brightness = 4;
	else if (sysfs_streq(buf, "5"))
		msd.dstat.auto_brightness = 5;
	else if (sysfs_streq(buf, "6"))
		msd.dstat.auto_brightness = 6;
	else
		pr_info("%s: Invalid argument!!", __func__);

	pr_info("%s : level (%d), bl_level (%d) \n", 
		__func__, msd.dstat.auto_brightness, mfd->bl_level);

#ifdef CONFIG_SAMSUNG_CMC624
	if (samsung_has_cmc624()) {
		cabc_onoff_ctrl(msd.dstat.auto_brightness);
	}
#endif
	return size;
}


static struct lcd_ops mipi_novatek_disp_props = {
#ifdef WA_FOR_FACTORY_MODE
	.get_power = NULL,
	.set_power = NULL,
#else
	.get_power = mipi_novatek_disp_get_power,
	.set_power = mipi_novatek_disp_set_power,
#endif
};

#ifdef WA_FOR_FACTORY_MODE
static DEVICE_ATTR(lcd_power, S_IRUGO | S_IWUSR,
			mipi_novatek_disp_get_power,
			mipi_novatek_disp_set_power);
#endif
static DEVICE_ATTR(lcd_type, S_IRUGO, mipi_novatek_lcdtype_show, NULL);
static DEVICE_ATTR(gamma_mode, S_IRUGO | S_IWUSR | S_IWGRP,
			mipi_novatek_disp_gamma_mode_show,
			mipi_novatek_disp_gamma_mode_store);
static DEVICE_ATTR(power_reduce, S_IRUGO | S_IWUSR | S_IWGRP,
			mipi_novatek_disp_acl_show,
			mipi_novatek_disp_acl_store);
static DEVICE_ATTR(auto_brightness, S_IRUGO | S_IWUSR | S_IWGRP,
			mipi_novatek_auto_brightness_show,
			mipi_novatek_auto_brightness_store);
#if defined(CONFIG_MACH_MELIUS)
static DEVICE_ATTR(siop_enable, S_IRUGO | S_IWUSR | S_IWGRP,
			siop_enable_show,
			siop_enable_store);
#endif
#endif


static int __devinit mipi_novatek_disp_probe(struct platform_device *pdev)
{
	struct platform_device *msm_fb_added_dev;
#if defined(CONFIG_LCD_CLASS_DEVICE)
	struct lcd_device *lcd_device;
	int ret;
#endif
#if defined(CONFIG_BACKLIGHT_CLASS_DEVICE)
	struct backlight_device *bd;
#endif
	msd.dstat.acl_on = false;
    is_lcd_on = LCD_STATUS_ON;

	if (pdev->id == 0) {
		msd.mipi_novatek_disp_pdata = pdev->dev.platform_data;

#ifdef CONFIG_SAMSUNG_CMC624
	if (samsung_has_cmc624()) {
		printk(KERN_DEBUG "Is_There_cmc624 : CMC624 is there!!!!");
		samsung_cmc624_init();
	} else {
		printk(KERN_DEBUG "Is_There_cmc624 : CMC624 is not there!!!!");
	}
#endif

		return 0;
	}

	msm_fb_added_dev = msm_fb_add_device(pdev);

#if defined(CONFIG_HAS_EARLYSUSPEND) || defined(CONFIG_LCD_CLASS_DEVICE)
	msd.msm_pdev = msm_fb_added_dev;
#endif

#if defined(CONFIG_HAS_EARLYSUSPEND)
	msd.early_suspend.suspend = mipi_novatek_disp_early_suspend;
	msd.early_suspend.resume = mipi_novatek_disp_late_resume;
	msd.early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN;
	register_early_suspend(&msd.early_suspend);

#endif

#if defined(CONFIG_LCD_CLASS_DEVICE)
	lcd_device = lcd_device_register("panel", &pdev->dev, NULL,
					&mipi_novatek_disp_props);

	if (IS_ERR(lcd_device)) {
		ret = PTR_ERR(lcd_device);
		printk(KERN_ERR "lcd : failed to register device\n");
		return ret;
	}

#ifdef WA_FOR_FACTORY_MODE
	sysfs_remove_file(&lcd_device->dev.kobj,
					&dev_attr_lcd_power.attr);

	ret = sysfs_create_file(&lcd_device->dev.kobj,
					&dev_attr_lcd_power.attr);
	if (ret) {
		pr_info("sysfs create fail-%s\n",
				dev_attr_lcd_power.attr.name);
	}
#endif

	ret = sysfs_create_file(&lcd_device->dev.kobj,
					&dev_attr_lcd_type.attr);
	if (ret) {
		pr_info("sysfs create fail-%s\n",
				dev_attr_lcd_type.attr.name);
	}

	ret = sysfs_create_file(&lcd_device->dev.kobj,
					&dev_attr_gamma_mode.attr);
	if (ret) {
		pr_info("sysfs create fail-%s\n",
				dev_attr_gamma_mode.attr.name);
	}

	ret = sysfs_create_file(&lcd_device->dev.kobj,
					&dev_attr_power_reduce.attr);
	if (ret) {
		pr_info("sysfs create fail-%s\n",
				dev_attr_power_reduce.attr.name);
	}

#if defined(CONFIG_MACH_MELIUS)
	ret = sysfs_create_file(&lcd_device->dev.kobj,
					&dev_attr_siop_enable.attr);
	if (ret) {
		pr_info("sysfs create fail-%s\n",
				dev_attr_siop_enable.attr.name);
	}
#endif

#if defined(CONFIG_BACKLIGHT_CLASS_DEVICE)
	bd = backlight_device_register("panel", &lcd_device->dev,
						NULL, NULL, NULL);
	if (IS_ERR(bd)) {
		ret = PTR_ERR(bd);
		pr_info("backlight : failed to register device\n");
		return ret;
	}

	ret = sysfs_create_file(&bd->dev.kobj,
					&dev_attr_auto_brightness.attr);
	if (ret) {
		pr_info("sysfs create fail-%s\n",
				dev_attr_auto_brightness.attr.name);
	}
#endif
#endif

#ifdef CONFIG_SAMSUNG_CMC624
	if (samsung_has_cmc624()) {
			ret = cmc624_sysfs_init();
			if (ret < 0)
				pr_debug("CMC624 sysfs initialize FAILED\n");
		}
	else
#if defined(CONFIG_FB_MDP4_ENHANCE)
		init_mdnie_class();
#endif
#endif

	pr_debug("%s:Display probe completed\n", __func__);
	return 0;
}

static struct platform_driver this_driver = {
	.probe  = mipi_novatek_disp_probe,
	.driver = {
		.name   = "mipi_novatek_nt35596",
	},
	.shutdown = mipi_novatek_disp_shutdown
};

static struct msm_fb_panel_data novatek_panel_data = {
	.on		= mipi_novatek_disp_on,
	.off		= mipi_novatek_disp_off,
	.set_backlight	= mipi_novatek_disp_set_backlight,
};

static int ch_used[3];

int mipi_novatek_disp_device_register(struct msm_panel_info *pinfo,
					u32 channel, u32 panel,
					struct mipi_panel_data *mpd)
{
	struct platform_device *pdev = NULL;
	int ret = 0;

	if ((channel >= 3) || ch_used[channel])
		return -ENODEV;

	ch_used[channel] = TRUE;

	pdev = platform_device_alloc("mipi_novatek_nt35596",
					   (panel << 8)|channel);
	if (!pdev)
		return -ENOMEM;

	novatek_panel_data.panel_info = *pinfo;
	msd.mpd = mpd;
	if (!msd.mpd) {
		printk(KERN_ERR
		  "%s: get mipi_panel_data failed!\n", __func__);
		goto err_device_put;
	}
	mpd->msd = &msd;
	ret = platform_device_add_data(pdev, &novatek_panel_data,
		sizeof(novatek_panel_data));
	if (ret) {
		printk(KERN_ERR
		  "%s: platform_device_add_data failed!\n", __func__);
		goto err_device_put;
	}

	ret = platform_device_add(pdev);
	if (ret) {
		printk(KERN_ERR
		  "%s: platform_device_register failed!\n", __func__);
		goto err_device_put;
	}

	return ret;

err_device_put:
	platform_device_put(pdev);
	return ret;
}


static int __init current_boot_mode(char *mode)
{
	/*
	*	1 is recovery booting
	*	0 is normal booting
	*/

	if (strncmp(mode, "1", 1) == 0)
		recovery_boot_mode = 1;
	else
		recovery_boot_mode = 0;

	pr_debug("%s %s", __func__, recovery_boot_mode == 1 ?
						"recovery" : "normal");
	return 1;
}
__setup("androidboot.boot_recovery=", current_boot_mode);

static int __init mipi_novatek_disp_init(void)
{
	mipi_dsi_buf_alloc(&msd.novatek_tx_buf, DSI_BUF_SIZE);
	mipi_dsi_buf_alloc(&msd.novatek_rx_buf, DSI_BUF_SIZE);

	return platform_driver_register(&this_driver);
}
module_init(mipi_novatek_disp_init);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Novatek NT35596 LCD driver");
