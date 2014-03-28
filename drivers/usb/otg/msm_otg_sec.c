/*
 * drivers/usb/otg/msm_otg_sec.c
 *
 * Copyright (c) 2012, Samsung Electronics
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

#include <linux/gpio.h>
#include <linux/host_notify.h>
#ifdef CONFIG_USB_SWITCH_FSA9485
#include <linux/i2c/fsa9485.h>
#endif

#include <linux/usb/msm_hsusb.h>
#ifdef pr_fmt
#undef pr_fmt
#endif
#define pr_fmt(fmt) "%s %d: " fmt, __func__, __LINE__

#define SM_WORK_TIMER_FREQ	(jiffies + msecs_to_jiffies(1000))

#if 1
#define MSM_OTG_LATE_INIT
//#define MSM_OTG_DIRECT_VBUS
#endif

static void msm_otg_set_vbus_state(int online);
static void msm_hsusb_vbus_power(struct msm_otg *motg, bool on);
static int ulpi_write(struct usb_phy *phy, u32 val, u32 reg);
static int ulpi_read(struct usb_phy *phy, u32 reg);

static void msm_otg_set_id_state_pbatest(int id, struct host_notify_dev *ndev)
{
	struct msm_otg *motg = container_of(ndev, struct msm_otg, ndev);

#ifdef MSM_OTG_DIRECT_VBUS
	pr_info("[OTG] %s %d, id: %d\n", __func__, __LINE__, id);
	msm_hsusb_vbus_power(motg, id);
#else
	struct usb_phy *phy = &motg->phy;

	pr_info("[OTG] %s %d, id: %d\n", __func__, __LINE__, id);
	if (atomic_read(&motg->in_lpm))
		pm_runtime_resume(phy->dev);

	if (!id)
		set_bit(ID, &motg->inputs);
	else
		clear_bit(ID, &motg->inputs);

	schedule_work(&motg->sm_work);
#endif

#ifdef CONFIG_USB_SWITCH_FSA9485
	if (!id)
		fsa9485_otg_detach();
#endif
}

static void msm_otg_host_phy_tune(struct msm_otg *otg,
		u32 paramb, u32 paramc)
{
	ulpi_write(&otg->phy, paramb, 0x81);
	ulpi_write(&otg->phy, paramc, 0x82);

	pr_info("ULPI 0x%x: 0x%x: 0x%x: 0x%x\n",
			ulpi_read(&otg->phy, 0x80),
			ulpi_read(&otg->phy, 0x81),
			ulpi_read(&otg->phy, 0x82),
			ulpi_read(&otg->phy, 0x83));
	mdelay(100);
}

static void msm_otg_host_notify_set(struct msm_otg *motg, int state)
{
	pr_info("boost : %d\n", state);

	if (state &&
		(motg->ndev.mode == NOTIFY_HOST_MODE ||
		motg->ndev.mode == NOTIFY_TEST_MODE))
		motg->ndev.booster = NOTIFY_POWER_ON;
	else
		motg->ndev.booster = NOTIFY_POWER_OFF;
}

static void msm_otg_host_notify(struct msm_otg *motg, int on)
{
	pr_info("host_notify: %d, dock %d\n", on, motg->smartdock);

	if (on)
		msm_otg_host_phy_tune(motg, 0x33, 0x14);

	if (motg->smartdock) {
		motg->ndev.state = NOTIFY_HOST_REMOVE;
		return;
	}

	if (on) {
		motg->ndev.mode = NOTIFY_HOST_MODE;
		host_state_notify(&motg->ndev, NOTIFY_HOST_ADD);
	} else {
		motg->ndev.mode = NOTIFY_NONE_MODE;
		host_state_notify(&motg->ndev, NOTIFY_HOST_REMOVE);
	}
}

#if 0
static void msm_otg_sm_work_timer_func(unsigned long data)
{
	struct msm_otg *motg = (struct msm_otg *) data;
	struct usb_phy *phy = &motg->phy;

	if (atomic_read(&motg->in_lpm)) {
		dev_info(motg->phy.dev, "sm_work_timer: in lpm\n");
		return;
	}

	if (phy->state > OTG_STATE_B_IDLE) {
		dev_info(motg->phy.dev, "sm_work_timer: on working\n");
		return;
	}

	if (!schedule_work(&motg->sm_work)) {
		dev_info(motg->phy.dev, "sm_work_timer: pending\n");
		mod_timer(&motg->sm_work_timer, SM_WORK_TIMER_FREQ);
	}
}
#endif

#ifdef CONFIG_USB_HOST_NOTIFY

enum usb_notify_state {
	ACC_POWER_ON = 0,
	ACC_POWER_OFF,
	ACC_POWER_OVER_CURRENT,
};

static void msm_otg_notify_work(struct work_struct *w)
{
	struct msm_otg *motg = container_of(w, struct msm_otg, notify_work);

	if (motg->smartdock)
		return;

	switch (motg->notify_state) {
	case ACC_POWER_ON:
		dev_info(motg->phy.dev, "Acc power on detect\n");
		break;
	case ACC_POWER_OFF:
		dev_info(motg->phy.dev, "Acc power off detect\n");
		break;
	case ACC_POWER_OVER_CURRENT:
		host_state_notify(&motg->ndev, NOTIFY_HOST_OVERCURRENT);
		dev_err(motg->phy.dev, "OTG overcurrent!!!!!!\n");
		break;
	default:
		break;
	}
}

static void msm_otg_late_power_work(struct work_struct *w)
{
	struct msm_otg *motg = container_of((struct delayed_work *)w,
				struct msm_otg, late_power_work);

	dev_info(motg->phy.dev, "%s, ID=%d, booster=%d\n",
		__func__, test_bit(ID, &motg->inputs), motg->ndev.booster);

	if (!test_bit(ID, &motg->inputs) &&
		(motg->ndev.booster == NOTIFY_POWER_OFF)) {
		msm_hsusb_vbus_power(motg, 0);
		msleep(100);
		msm_hsusb_vbus_power(motg, 1);
	}
}

int msm_otg_power_cb(int active)
{
	struct msm_otg *motg = the_msm_otg;

	int otg_power = active;

	if (!motg) {
		pr_err("%s: motg is null.\n", __func__);
		return -1;
	}

	dev_info(motg->phy.dev, "%s, ID=%d, otg_power=%d, ndev.mode=%d\n",
		__func__, test_bit(ID, &motg->inputs),
		otg_power, motg->ndev.mode);

	if (!test_bit(ID, &motg->inputs)) {
		if (otg_power) {
			motg->ndev.booster = NOTIFY_POWER_ON;
			motg->notify_state = ACC_POWER_ON;
			schedule_work(&motg->notify_work);
		} else {
			motg->ndev.booster = NOTIFY_POWER_OFF;
			motg->notify_state = ACC_POWER_OVER_CURRENT;
			schedule_work(&motg->notify_work);
		}
	} else {
		if (!otg_power) {
			if (motg->ndev.mode == NOTIFY_HOST_MODE ||
				motg->notify_state == ACC_POWER_ON) {
				motg->ndev.booster = NOTIFY_POWER_OFF;
				motg->notify_state = ACC_POWER_OFF;
				schedule_work(&motg->notify_work);
			}
		}
	}
	return motg->ndev.mode;
}
EXPORT_SYMBOL_GPL(msm_otg_power_cb);
#endif

static void msm_otg_power_work(struct work_struct *w)
{
	struct msm_otg *motg = container_of(w, struct msm_otg, otg_power_work);
	int otg_test = gpio_get_value_cansleep(motg->pdata->otg_test_gpio);

	msleep(200);

	pr_info("id: %d, otg_test: %d\n",
			test_bit(ID, &motg->inputs), otg_test);

	/* host */
	if (!test_bit(ID, &motg->inputs)) {
		if (otg_test) {
			motg->ndev.booster = NOTIFY_POWER_ON;
		} else {
			motg->ndev.booster = NOTIFY_POWER_OFF;
			host_state_notify(&motg->ndev, NOTIFY_HOST_OVERCURRENT);
			pr_err("OTG Overcurrent!\n");
		}

	} else {
		if (motg->ndev.mode == NOTIFY_HOST_MODE && otg_test == 0)
			motg->ndev.booster = NOTIFY_POWER_OFF;
	}
}

static irqreturn_t msm_otg_test_irq(int irq, void *data)
{
	struct msm_otg *motg = data;

	schedule_work(&motg->otg_power_work);

	return IRQ_HANDLED;
}

static int msm_host_notify_init(struct device *dev, struct msm_otg *motg)
{
	int ret = 0;

	INIT_WORK(&motg->otg_power_work, msm_otg_power_work);
	if (motg->pdata->otg_test_gpio) {
		int otg_test_irq = gpio_to_irq(motg->pdata->otg_test_gpio);

		ret = request_threaded_irq(otg_test_irq, NULL,
						msm_otg_test_irq,
						IRQF_TRIGGER_RISING |
						IRQF_TRIGGER_FALLING,
						"gpio_vbus", motg);
		if (ret)
			pr_err("request irq failed for otg_test\n");
		else
			pr_err("request irq succeed for otg_test\n");
	}

	motg->ndev.name = "usb_otg";
	motg->ndev.set_booster = msm_otg_set_id_state_pbatest;
	ret = host_notify_dev_register(&motg->ndev);
	if (ret)
		dev_err(dev, "Failed to host_notify_dev_register\n");
	else
		dev_info(dev, "success to host_notify_dev_register\n");

	INIT_WORK(&motg->notify_work, msm_otg_notify_work);
#ifdef MSM_OTG_LATE_INIT
	INIT_DELAYED_WORK(&motg->late_power_work,
					msm_otg_late_power_work);
	motg->notify_state = ACC_POWER_OFF;
#endif
	return 0;
}

static int msm_host_notify_exit(struct msm_otg *motg)
{
	int ret = 0;

	host_notify_dev_unregister(&motg->ndev);

#ifdef MSM_OTG_LATE_INIT
	cancel_delayed_work_sync(&motg->late_power_work);
#endif

	return ret;
}

/*
 * Exported functions
 */

void sec_otg_set_dock_state(int enable)
{
	struct msm_otg *motg = the_msm_otg;
	struct usb_phy *phy = &motg->phy;

	if (enable) {
		pr_info("DOCK : attached\n");
		motg->smartdock = true;
		clear_bit(ID, &motg->inputs);

		if (atomic_read(&motg->in_lpm)) {
			pr_info("DOCK : in LPM\n");
			pm_runtime_resume(phy->dev);
		}

		if (test_bit(B_SESS_VLD, &motg->inputs)) {
			pr_info("clear B_SESS_VLD\n");
			clear_bit(B_SESS_VLD, &motg->inputs);
		}

		schedule_work(&motg->sm_work);

	} else {
		pr_info("DOCK : detached\n");
		motg->smartdock = false;
		set_bit(ID, &motg->inputs);
	}

}
EXPORT_SYMBOL_GPL(sec_otg_set_dock_state);

void msm_otg_set_id_state(int online)
{
	struct msm_otg *motg = the_msm_otg;

	if (online) {
		dev_info(motg->phy.dev, "MUIC: ID set\n");
		set_bit(ID, &motg->inputs);
	} else {
		dev_info(motg->phy.dev, "MUIC: ID clear\n");
		clear_bit(ID, &motg->inputs);
		set_bit(A_BUS_REQ, &motg->inputs);
	}

	if (atomic_read(&motg->pm_suspended))
		motg->sm_work_pending = true;
	else
		queue_work(system_nrt_wq, &motg->sm_work);
}
EXPORT_SYMBOL_GPL(msm_otg_set_id_state);

void msm_otg_set_smartdock_state(bool online)
{
	struct msm_otg *motg = the_msm_otg;

	if (online) {
		dev_info(motg->phy.dev, "SMARTDOCK : ID set\n");
		motg->smartdock = false;
		set_bit(ID, &motg->inputs);
	} else {
		dev_info(motg->phy.dev, "SMARTDOCK : ID clear\n");
		motg->smartdock = true;
		clear_bit(ID, &motg->inputs);
	}

	if (test_bit(B_SESS_VLD, &motg->inputs))
		clear_bit(B_SESS_VLD, &motg->inputs);

	if (atomic_read(&motg->pm_suspended))
		motg->sm_work_pending = true;
	else
		queue_work(system_nrt_wq, &motg->sm_work);
}
EXPORT_SYMBOL_GPL(msm_otg_set_smartdock_state);

void sec_otg_set_id_state(bool enable)
{
	struct msm_otg *motg = the_msm_otg;
	struct usb_phy *phy = &motg->phy;

	pr_info("msm_otg_set_id_state is called\n");
	if (atomic_read(&motg->in_lpm)) {
		pr_info("msm_otg_set_id_state : in LPM\n");
		pm_runtime_resume(phy->dev);
	}

}
EXPORT_SYMBOL_GPL(sec_otg_set_id_state);
