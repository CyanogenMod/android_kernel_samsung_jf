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
#include "board-8064.h"

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
#include <linux/i2c/synaptics_rmi.h>
#endif

#ifdef CONFIG_KEYBOARD_CYPRESS_TOUCH_236
#include <linux/i2c/cypress_touchkey.h>
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

static int __init midas_sec_switch_init(void)
{
	int ret;
	switch_dev = device_create(sec_class, NULL, 0, NULL, "switch");

	if (IS_ERR(switch_dev))
		pr_err("Failed to create device(switch)!\n");

	ret = device_create_file(switch_dev, &dev_attr_disable_vbus);
	if (ret)
		pr_err("Failed to create device file(disable_vbus)!\n");

	return 0;
};

int current_cable_type = POWER_SUPPLY_TYPE_BATTERY;
int max77693_muic_charger_cb(enum cable_type_muic cable_type)
{
#ifdef CONFIG_CHARGER_MAX77693
	struct power_supply *psy = power_supply_get_by_name("battery");
	union power_supply_propval value;
	static enum cable_type_muic previous_cable_type = CABLE_TYPE_NONE_MUIC;
#endif
	pr_info("%s: %d\n", __func__, cable_type);

#ifdef CONFIG_TOUCHSCREEN_SYNAPTICS_I2C_RMI

	if (charger_callbacks && charger_callbacks->inform_charger)
		charger_callbacks->inform_charger(charger_callbacks,
		cable_type);

#endif

#ifdef CONFIG_KEYBOARD_CYPRESS_TOUCH_236
	touchkey_charger_infom(cable_type);
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
#ifdef CONFIG_CHARGER_MAX77693
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

#ifdef CONFIG_CHARGER_MAX77693
	/*  charger setting */
	if (previous_cable_type == cable_type) {
		pr_info("%s: SKIP cable setting\n", __func__);
		goto skip;
	}
	previous_cable_type = cable_type;

	switch (cable_type) {
	case CABLE_TYPE_NONE_MUIC:
		current_cable_type = POWER_SUPPLY_TYPE_BATTERY;
		break;
	case CABLE_TYPE_MHL_VB_MUIC:
		if(poweroff_charging)
			current_cable_type = POWER_SUPPLY_TYPE_USB;
		else
			goto skip;
		break;
	case CABLE_TYPE_MHL_MUIC:
		if(poweroff_charging)
			current_cable_type = POWER_SUPPLY_TYPE_BATTERY;
		else
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
	case CABLE_TYPE_CDP_MUIC:
		current_cable_type = POWER_SUPPLY_TYPE_USB_CDP;
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
	pr_info("MUIC usb_cb:%d\n", usb_mode);
	if (usb_mode == USB_CABLE_DETACHED
		|| usb_mode == USB_CABLE_ATTACHED) {
		msm_otg_set_vbus_state(usb_mode);
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
#ifdef CONFIG_VIDEO_MHL_V2
static BLOCKING_NOTIFIER_HEAD(acc_notifier);
int acc_register_notifier(struct notifier_block *nb)
{
	int ret;
	ret = blocking_notifier_chain_register(&acc_notifier, nb);
	return ret;
}

int acc_unregister_notifier(struct notifier_block *nb)
{
	return blocking_notifier_chain_unregister(&acc_notifier, nb);
}

static int acc_notify(int event)
{
	int ret;
	ret = blocking_notifier_call_chain(&acc_notifier, event, NULL);
	return ret;
}
#endif
/*extern void MHL_On(bool on);*/
void max77693_muic_mhl_cb(int attached)
{
	pr_info("MUIC attached:%d\n", attached);
	if (attached == MAX77693_MUIC_ATTACHED) {
		/*MHL_On(1);*/ /* GPIO_LEVEL_HIGH */
		pr_info("MHL Attached !!\n");
#ifdef CONFIG_VIDEO_MHL_V2
		wake_lock(&max77693_muic.mhl_wake_lock);
		acc_notify(1);
#endif
	} else {
		/*MHL_On(0);*/ /* GPIO_LEVEL_LOW */
		pr_info("MHL Detached !!\n");
#ifdef CONFIG_VIDEO_MHL_V2
		wake_unlock(&max77693_muic.mhl_wake_lock);
		acc_notify(0);
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

void max77693_muic_smartdock_cb(bool attached)
{
	pr_info("MUIC smartdock attached=%d\n", attached);
	pr_info("##MUIC [ %s ]- func : %s !!\n", __FILE__, __func__);
	if (attached) {
#ifdef CONFIG_JACK_MON
		jack_event_handler("cradle", 8);
#endif
		switch_set_state(&switch_dock, 8);
	} else {
#ifdef CONFIG_JACK_MON
		jack_event_handler("cradle", 0);
#endif
		switch_set_state(&switch_dock, 0);
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
		regulator = regulator_get(NULL, "safeout1");
		if (IS_ERR(regulator))
			return -ENODEV;
		if (!regulator_is_enabled(regulator))
			regulator_enable(regulator);
		regulator_put(regulator);

		regulator = regulator_get(NULL, "safeout2");
		if (IS_ERR(regulator))
			return -ENODEV;
		if (regulator_is_enabled(regulator))
			regulator_force_disable(regulator);
		regulator_put(regulator);
	}

	return 0;
}

struct max77693_muic_data max77693_muic = {
	.usb_cb = max77693_muic_usb_cb,
	.charger_cb = max77693_muic_charger_cb,
	.mhl_cb = max77693_muic_mhl_cb,
	.is_mhl_attached = max77693_muic_is_mhl_attached,
	.set_safeout = max77693_muic_set_safeout,
	.init_cb = max77693_muic_init_cb,
	.deskdock_cb = max77693_muic_deskdock_cb,
	.cardock_cb = max77693_muic_cardock_cb,
	.smartdock_cb = max77693_muic_smartdock_cb,
	.audiodock_cb = max77693_muic_audiodock_cb,
#ifdef CONFIG_USB_HOST_NOTIFY
	.host_notify_cb = max77693_muic_host_notify_cb,
#else
	.host_notify_cb = NULL,
#endif
	.gpio_usb_sel = -1,
	.jig_state = max77693_set_jig_state,
};

device_initcall(midas_sec_switch_init);
