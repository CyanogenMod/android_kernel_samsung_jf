/*
 *  sec-switch.c
 *
 *  Copyright (C) 2012 Samsung Electronics
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/spi/spi_gpio.h>
#include <linux/gpio.h>
#include <linux/gpio_keys.h>
#include <linux/gpio_event.h>
#include <linux/i2c.h>
#include <linux/i2c-gpio.h>
#include <linux/input.h>
/*#include <linux/mmc/host.h>*/
#include <linux/regulator/machine.h>
#include <linux/regulator/max8649.h>
#include <linux/regulator/fixed.h>
#include <linux/mfd/wm8994/pdata.h>
#include <linux/mfd/max77693.h>
#include <linux/mfd/max77693-private.h>
#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>
#include <linux/gpio.h>

#include <linux/power_supply.h>

#include <linux/switch.h>
#include <linux/sii9234.h>

#ifdef CONFIG_MACH_STRETTO
#include <mach/stretto-gpio.h>
#endif

#include "devices.h"
#include "board-8930.h"

#ifdef CONFIG_USB_HOST_NOTIFY
#include <linux/host_notify.h>
#endif
#include <linux/pm_runtime.h>
#include <linux/usb.h>
#include <linux/usb/hcd.h>

#ifdef CONFIG_JACK_MON
#include <linux/jack.h>
#endif

#ifdef CONFIG_TOUCHSCREEN_SYNAPTICS_I2C_RMI
#include <linux/i2c/synaptics_rmi_msm8930.h>
#endif

#ifdef CONFIG_TOUCHSCREEN_ATMEL_MXT224S_KS02
#include <linux/i2c/mxt224s_ks02.h>
#endif

#include <linux/sec_class.h>

static struct switch_dev switch_dock = {
	.name = "dock",
};

struct device *switch_dev;
EXPORT_SYMBOL(switch_dev);

#ifdef CONFIG_TOUCHSCREEN_SYNAPTICS_I2C_RMI
struct synaptics_rmi_callbacks *charger_callbacks;
#endif

#ifdef CONFIG_TOUCHSCREEN_ATMEL_MXT224S_KS02
struct tsp_callbacks *charger_callbacks;
#endif


/* charger cable state */
bool is_cable_attached;
bool is_jig_attached;

static ssize_t midas_switch_show_vbus(struct device *dev,
				      struct device_attribute *attr, char *buf)
{
	int i;
	struct regulator *regulator;

	regulator = regulator_get(NULL, "safeout1");
	if (IS_ERR(regulator)) {
		pr_warn("%s: fail to get regulator\n", __func__);
		return sprintf(buf, "UNKNOWN\n");
	}
	if (regulator_is_enabled(regulator))
		i = sprintf(buf, "VBUS is enabled\n");
	else
		i = sprintf(buf, "VBUS is disabled\n");
	regulator_put(regulator);

	return i;
}

static ssize_t midas_switch_store_vbus(struct device *dev,
				       struct device_attribute *attr,
				       const char *buf, size_t count)
{
	int disable, ret, usb_mode;
	struct regulator *regulator;
	/* struct s3c_udc *udc = platform_get_drvdata(&s3c_device_usbgadget); */

	if (!strncmp(buf, "0", 1))
		disable = 0;
	else if (!strncmp(buf, "1", 1))
		disable = 1;
	else {
		pr_warn("%s: Wrong command\n", __func__);
		return count;
	}

	pr_info("%s: disable=%d\n", __func__, disable);
	usb_mode =
	    disable ? USB_CABLE_DETACHED_WITHOUT_NOTI : USB_CABLE_ATTACHED;
	/* ret = udc->change_usb_mode(usb_mode); */
	ret = -1;
	if (ret < 0)
		pr_err("%s: fail to change mode!!!\n", __func__);

	regulator = regulator_get(NULL, "safeout1");
	if (IS_ERR(regulator)) {
		pr_warn("%s: fail to get regulator\n", __func__);
		return count;
	}

	if (disable) {
		if (regulator_is_enabled(regulator))
			regulator_force_disable(regulator);
		if (!regulator_is_enabled(regulator))
			regulator_enable(regulator);
	} else {
		if (!regulator_is_enabled(regulator))
			regulator_enable(regulator);
	}
	regulator_put(regulator);

	return count;
}

DEVICE_ATTR(disable_vbus, 0664, midas_switch_show_vbus,
	    midas_switch_store_vbus);

#ifdef CONFIG_TARGET_LOCALE_KOR
#include "../../../drivers/usb/gadget/s3c_udc.h"
/* usb access control for SEC DM */
struct device *usb_lock;
static int is_usb_locked;

static ssize_t midas_switch_show_usb_lock(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	if (is_usb_locked)
		return snprintf(buf, PAGE_SIZE, "USB_LOCK");
	else
		return snprintf(buf, PAGE_SIZE, "USB_UNLOCK");
}

static ssize_t midas_switch_store_usb_lock(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	int lock;
	struct s3c_udc *udc =
		platform_get_drvdata(&msm8960_device_gadget_peripheral);

	if (!strncmp(buf, "0", 1))
		lock = 0;
	else if (!strncmp(buf, "1", 1))
		lock = 1;
	else {
		pr_warn("%s: Wrong command\n", __func__);
		return count;
	}

	if (IS_ERR_OR_NULL(udc))
		return count;

	pr_info("%s: lock=%d\n", __func__, lock);

	if (lock != is_usb_locked) {
		is_usb_locked = lock;

		if (lock) {
			if (udc->udc_enabled)
				usb_gadget_vbus_disconnect(&udc->gadget);
		}
	}

	return count;
}

static DEVICE_ATTR(enable, 0664,
		   midas_switch_show_usb_lock, midas_switch_store_usb_lock);
#endif

static int __init midas_sec_switch_init(void)
{
	int ret;
	switch_dev = device_create(sec_class, NULL, 0, NULL, "switch");

	if (IS_ERR(switch_dev))
		pr_err("Failed to create device(switch)!\n");

	ret = device_create_file(switch_dev, &dev_attr_disable_vbus);
	if (ret)
		pr_err("Failed to create device file(disable_vbus)!\n");

#ifdef CONFIG_TARGET_LOCALE_KOR
	usb_lock = device_create(sec_class, switch_dev,
				MKDEV(0, 0), NULL, ".usb_lock");

	if (IS_ERR(usb_lock))
		pr_err("Failed to create device (usb_lock)!\n");

	if (device_create_file(usb_lock, &dev_attr_enable) < 0)
		pr_err("Failed to create device file(.usblock/enable)!\n");
#endif

	return 0;
};

int current_cable_type = POWER_SUPPLY_TYPE_BATTERY;
#ifdef CONFIG_CHARGER_MAX77XXX
int max77693_muic_charger_cb(enum cable_type_muic cable_type)
{
#ifdef CONFIG_CHARGER_MAX77XXX
	struct power_supply *psy = power_supply_get_by_name("battery");
	union power_supply_propval value;
#endif
	pr_info("%s: %d\n", __func__, cable_type);

#ifdef CONFIG_TOUCHSCREEN_SYNAPTICS_I2C_RMI
	if (charger_callbacks && charger_callbacks->inform_charger)
		charger_callbacks->inform_charger(charger_callbacks,
		cable_type);
#endif
#ifdef CONFIG_TOUCHSCREEN_ATMEL_MXT224S_KS02
	if (charger_callbacks && charger_callbacks->inform_charger)
		charger_callbacks->inform_charger(charger_callbacks,
		cable_type);
#endif



#ifdef CONFIG_JACK_MON
	switch (cable_type) {
	case CABLE_TYPE_OTG_MUIC:
	case CABLE_TYPE_NONE_MUIC:
	case CABLE_TYPE_JIG_UART_OFF_MUIC:
	case CABLE_TYPE_MHL_MUIC:
		is_cable_attached = false;
		break;
	case CABLE_TYPE_USB_MUIC:
#ifdef CONFIG_CHARGER_MAX77XXX
		value.intval = POWER_SUPPLY_TYPE_USB;
#endif
	case CABLE_TYPE_JIG_USB_OFF_MUIC:
	case CABLE_TYPE_JIG_USB_ON_MUIC:
	case CABLE_TYPE_SMARTDOCK_USB_MUIC:
		is_cable_attached = true;
		break;
	case CABLE_TYPE_MHL_VB_MUIC:
		is_cable_attached = true;
		break;
	case CABLE_TYPE_AUDIODOCK_MUIC:
	case CABLE_TYPE_TA_MUIC:
	case CABLE_TYPE_CARDOCK_MUIC:
	case CABLE_TYPE_DESKDOCK_MUIC:
	case CABLE_TYPE_SMARTDOCK_MUIC:
	case CABLE_TYPE_SMARTDOCK_TA_MUIC:
	case CABLE_TYPE_JIG_UART_OFF_VB_MUIC:
	case CABLE_TYPE_INCOMPATIBLE_MUIC:
		is_cable_attached = true;
		break;
	default:
		pr_err("%s: invalid type:%d\n", __func__, cable_type);
		return -EINVAL;
	}
#endif

#ifdef CONFIG_CHARGER_MAX77XXX
	/*  charger setting */

	switch (cable_type) {
	case CABLE_TYPE_NONE_MUIC:
		current_cable_type = POWER_SUPPLY_TYPE_BATTERY;
		break;
	case CABLE_TYPE_MHL_VB_MUIC:
#ifdef CONFIG_SAMSUNG_LPM_MODE
		if(poweroff_charging)
			current_cable_type = POWER_SUPPLY_TYPE_USB;
		else
#endif
			goto skip;
		break;
	case CABLE_TYPE_MHL_MUIC:
#ifdef CONFIG_SAMSUNG_LPM_MODE
		if(poweroff_charging)
			current_cable_type = POWER_SUPPLY_TYPE_BATTERY;
		else
#endif
			goto skip;
		break;
	case CABLE_TYPE_USB_MUIC:
	case CABLE_TYPE_JIG_USB_OFF_MUIC:
	case CABLE_TYPE_JIG_USB_ON_MUIC:
	case CABLE_TYPE_SMARTDOCK_USB_MUIC:
		current_cable_type = POWER_SUPPLY_TYPE_USB;
		break;
	case CABLE_TYPE_JIG_UART_OFF_VB_MUIC:
		current_cable_type = POWER_SUPPLY_TYPE_UARTOFF;
		break;
	case CABLE_TYPE_TA_MUIC:
		current_cable_type = POWER_SUPPLY_TYPE_MAINS;
		break;
	case CABLE_TYPE_AUDIODOCK_MUIC:
	case CABLE_TYPE_CARDOCK_MUIC:
	case CABLE_TYPE_DESKDOCK_MUIC:
	case CABLE_TYPE_SMARTDOCK_MUIC:
	case CABLE_TYPE_SMARTDOCK_TA_MUIC:
		current_cable_type = POWER_SUPPLY_TYPE_MISC;
		break;
	case CABLE_TYPE_OTG_MUIC:
		goto skip;
	case CABLE_TYPE_JIG_UART_OFF_MUIC:
		current_cable_type = POWER_SUPPLY_TYPE_BATTERY;
		break;
	case CABLE_TYPE_INCOMPATIBLE_MUIC:
		current_cable_type = POWER_SUPPLY_TYPE_UNKNOWN;
		break;
	default:
		pr_err("%s: invalid type for charger:%d\n",
				__func__, cable_type);
		goto skip;
	}

	if (!psy || !psy->set_property)
		pr_err("%s: fail to get battery psy\n", __func__);
	else {
		value.intval = current_cable_type<<ONLINE_TYPE_MAIN_SHIFT;
		psy->set_property(psy, POWER_SUPPLY_PROP_ONLINE, &value);
	}
#endif
skip:
#ifdef CONFIG_JACK_MON
	jack_event_handler("charger", is_cable_attached);
#endif

	return 0;
}
#endif
int max77693_get_jig_state(void)
{
	pr_info("%s: %d\n", __func__, is_jig_attached);
	return is_jig_attached;
}
EXPORT_SYMBOL(max77693_get_jig_state);

void max77693_set_jig_state(int jig_state)
{
	pr_info("%s: %d\n", __func__, jig_state);
	is_jig_attached = jig_state;
}

/* usb cable call back function */
void max77693_muic_usb_cb(u8 usb_mode)
{
#ifdef CONFIG_TARGET_LOCALE_KOR
	if (is_usb_locked) {
		pr_info("%s: usb locked by mdm\n", __func__);
		return;
	}
#endif

	pr_info("MUIC usb_cb:%d\n", usb_mode);
	if (usb_mode == USB_CABLE_DETACHED
		|| usb_mode == USB_CABLE_ATTACHED) {
		sec_otg_set_vbus_state(usb_mode);
#if defined(CONFIG_USB_HOST_NOTIFY)
	} else if (usb_mode == USB_OTGHOST_DETACHED
		|| usb_mode == USB_OTGHOST_ATTACHED) {
		if (usb_mode == USB_OTGHOST_DETACHED)
			msm_otg_set_id_state(1);
		else
			msm_otg_set_id_state(0);
	} else if (usb_mode == USB_POWERED_HOST_DETACHED
		|| usb_mode == USB_POWERED_HOST_ATTACHED) {
		if (usb_mode == USB_POWERED_HOST_DETACHED)
			msm_otg_set_smartdock_state(1);
		else
			msm_otg_set_smartdock_state(0);
	}
#else
	}
#endif

#ifdef CONFIG_JACK_MON
	if (usb_mode == USB_OTGHOST_ATTACHED
	|| usb_mode == USB_POWERED_HOST_ATTACHED)
		jack_event_handler("host", USB_CABLE_ATTACHED);
	else if (usb_mode == USB_OTGHOST_DETACHED
	|| usb_mode == USB_POWERED_HOST_DETACHED)
		jack_event_handler("host", USB_CABLE_DETACHED);
	else if ((usb_mode == USB_CABLE_ATTACHED)
		|| (usb_mode == USB_CABLE_DETACHED))
		jack_event_handler("usb", usb_mode);
#endif

}

void max77693_muic_mhl_cb(int attached)
{
	pr_info("MUIC attached:%d\n", attached);
	if (attached == MAX77693_MUIC_ATTACHED) {
		pr_info("MHL Attached !!\n");
#ifdef CONFIG_MHL_NEW_CBUS_MSC_CMD
	//	sii9234_wake_lock();
#endif
#ifdef CONFIG_VIDEO_MHL_V2
		mhl_onoff_ex(1);
#endif
	} else {
		pr_info("MHL Detached !!\n");
#ifdef CONFIG_VIDEO_MHL_V2
		mhl_onoff_ex(0);
#endif
#ifdef CONFIG_MHL_NEW_CBUS_MSC_CMD
	//	sii9234_wake_unlock();
#endif
	}
}

bool max77693_muic_is_mhl_attached(void)
{
	return 0;
}

void max77693_muic_deskdock_cb(bool attached)
{
	pr_info("MUIC deskdock attached=%d\n", attached);
	if (attached) {
#ifdef CONFIG_JACK_MON
		jack_event_handler("cradle", 1);
#endif
		switch_set_state(&switch_dock, 1);
	} else {
#ifdef CONFIG_JACK_MON
		jack_event_handler("cradle", 0);
#endif
		switch_set_state(&switch_dock, 0);
	}
}

void max77693_muic_cardock_cb(bool attached)
{
	pr_info("MUIC cardock attached=%d\n", attached);
	pr_info("##MUIC [ %s ]- func : %s !!\n", __FILE__, __func__);
	if (attached) {
#ifdef CONFIG_JACK_MON
		jack_event_handler("cradle", 2);
#endif
		switch_set_state(&switch_dock, 2);
	} else {
#ifdef CONFIG_JACK_MON
		jack_event_handler("cradle", 0);
#endif
		switch_set_state(&switch_dock, 0);
	}
}

void max77693_muic_audiodock_cb(bool attached)
{
	pr_info("MUIC audiodock attached=%d\n", attached);
	pr_info("##MUIC [ %s ]- func : %s !!\n", __FILE__, __func__);
	if (attached) {
#ifdef CONFIG_JACK_MON
		jack_event_handler("cradle", 7);
#endif
		switch_set_state(&switch_dock, 7);
	} else {
#ifdef CONFIG_JACK_MON
		jack_event_handler("cradle", 0);
#endif
		switch_set_state(&switch_dock, 0);
	}
}

void max77693_muic_smartdock_cb(bool attached, u8 cable_type)
{
	pr_info("MUIC smartdock attached=%d\n", attached);
	pr_info("##MUIC [ %s ]- func : %s !!\n", __FILE__, __func__);
	if (attached) {
#ifdef CONFIG_JACK_MON
		jack_event_handler("cradle", 8);
#endif
		switch_set_state(&switch_dock, 8);
#if defined(CONFIG_USB_HOST_NOTIFY)
	if (cable_type != CABLE_TYPE_SMARTDOCK_USB_MUIC)
		msm_otg_set_smartdock_state(0);
#endif
	} else {
#ifdef CONFIG_JACK_MON
		jack_event_handler("cradle", 0);
#endif
		switch_set_state(&switch_dock, 0);
#if defined(CONFIG_USB_HOST_NOTIFY)
	if (cable_type != CABLE_TYPE_SMARTDOCK_USB_MUIC)
		msm_otg_set_smartdock_state(1);
#endif
	}
}

void max77693_muic_init_cb(void)
{
	int ret;

	/* for CarDock, DeskDock */
	ret = switch_dev_register(&switch_dock);

	pr_info("MUIC ret=%d\n", ret);

	if (ret < 0)
		pr_err("Failed to register dock switch. %d\n", ret);
}

#if defined(CONFIG_MUIC_DET_JACK)
extern void jack_status_change(int attached);
extern void earkey_status_change(int pressed, int code);

void max77693_muic_earjack_cb(int attached)
{
	pr_info("%s: \n", __func__);
	jack_status_change(attached);
}
void max77693_muic_earjackkey_cb(int pressed, unsigned int code)
{
	pr_info("%s: \n", __func__);
	earkey_status_change(pressed, code);
}
#endif

#ifdef CONFIG_USB_HOST_NOTIFY
int max77693_muic_host_notify_cb(int enable)
{
	return msm_otg_power_cb(enable);
}
#endif

int max77693_muic_set_safeout(int path)
{
	struct regulator *regulator;

	pr_info("MUIC safeout path=%d\n", path);

	if (path == CP_USB_MODE) {
		regulator = regulator_get(NULL, "safeout1");
		if (IS_ERR(regulator))
			return -ENODEV;
		if (regulator_is_enabled(regulator))
			regulator_force_disable(regulator);
		regulator_put(regulator);

		regulator = regulator_get(NULL, "safeout2");
		if (IS_ERR(regulator))
			return -ENODEV;
		if (!regulator_is_enabled(regulator))
			regulator_enable(regulator);
		regulator_put(regulator);
	} else {
		/* AP_USB_MODE || AUDIO_MODE */
		regulator = regulator_get(NULL, "safeout2");
		if (IS_ERR(regulator))
			return -ENODEV;
		if (regulator_is_enabled(regulator))
			regulator_force_disable(regulator);
		regulator_put(regulator);

		regulator = regulator_get(NULL, "safeout1");
		if (IS_ERR(regulator))
			return -ENODEV;
		if (!regulator_is_enabled(regulator))
			regulator_enable(regulator);
		regulator_put(regulator);
	}

	return 0;
}

struct max77693_muic_data max77693_muic = {
	.usb_cb = max77693_muic_usb_cb,
#ifdef CONFIG_CHARGER_MAX77XXX
	.charger_cb = max77693_muic_charger_cb,
#endif
	.mhl_cb = max77693_muic_mhl_cb,
	.is_mhl_attached = max77693_muic_is_mhl_attached,
	.set_safeout = max77693_muic_set_safeout,
	.init_cb = max77693_muic_init_cb,
	.deskdock_cb = max77693_muic_deskdock_cb,
#if defined(CONFIG_MACH_MELIUS_SKT) || defined(CONFIG_MACH_MELIUS_KTT) || \
	defined(CONFIG_MACH_MELIUS_LGT) || defined(CONFIG_MACH_MELIUS_CHN_CTC) || defined(CONFIG_MACH_MELIUS_CHN_CMCC)
	.cardock_cb = NULL,
#else
	.cardock_cb = max77693_muic_cardock_cb,
#endif
	.smartdock_cb = max77693_muic_smartdock_cb,
	.audiodock_cb = max77693_muic_audiodock_cb,
#ifdef CONFIG_USB_HOST_NOTIFY
	.host_notify_cb = max77693_muic_host_notify_cb,
#else
	.host_notify_cb = NULL,
#endif
#if defined(CONFIG_MUIC_DET_JACK)
	.earjack_cb = max77693_muic_earjack_cb,
	.earjackkey_cb = max77693_muic_earjackkey_cb,
#endif
	.gpio_usb_sel = -1,
	.jig_state = max77693_set_jig_state,
};

device_initcall(midas_sec_switch_init);
