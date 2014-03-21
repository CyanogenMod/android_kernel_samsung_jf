/*
 *	Samsung Mipi ESD refresh driver.
 *
 *	Author: Krishna Kishor Jha <krishna.jha@samsung.com>
 *	Copyright (C) 2012, Samsung Electronics. All rights reserved.
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */
#ifndef __MIPI_SAMSUNG_ESD_REFRESH_H_
#define __MIPI_SAMSUNG_ESD_REFRESH_H_

#include <linux/module.h>
#include <linux/sysdev.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include <linux/types.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/switch.h>
#include <linux/input.h>
#include <linux/timer.h>
#include <linux/wakelock.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include "msm_fb.h"
#include "mipi_dsi.h"

#define WAKE_LOCK_TIME		(10 * HZ)	/* 1 sec */
#define ESD_EXCEPT_CNT 0
#define LP11_RECOVERY 0

extern void set_esd_refresh(boolean status);
extern struct class *sec_class;

struct esd_data_t {
	struct sec_esd_platform_data *pdata;
	struct wake_lock det_wake_lock;
	struct work_struct  det_work;
	int esd_count;
	struct delayed_work  esd_enable_delay;
	boolean esd_ignore;
	boolean refresh_ongoing;
	boolean esd_irq_enable;
	unsigned int esd_processed_count;
};
struct mipi_controls {
	struct platform_device *mipi_dev;
#if defined(CONFIG_HAS_EARLYSUSPEND)
	struct early_suspend early_suspend;
#endif
};


void register_mipi_dev(struct platform_device *mipi_dev);
extern void set_esd_enable(void);
extern void set_esd_disable(void);

#ifdef READ_REGISTER_ESD
void esd_execute(void);
#endif

#endif /* __MIPI_SAMSUNG_ESD_REFRESH_H_*/
