/*
 * Copyright (C) 2011 Samsung Electronics, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _SII9234_H_
#define _SII9234_H_

#ifdef __KERNEL__

struct sii9234_platform_data {
	int (*get_irq)(void);
	void (*hw_onoff)(bool);
	void (*hw_reset)(void);
	/*TBD*/
	int gpio;
	void (*mhl_sel)(bool);
	void (*gpio_cfg)(void);
#ifdef CONFIG_VIDEO_MHL_V2
	int prio;
	void (*enable)(bool enable);
	void (*power)(int on);
	void (*enable_vbus)(bool enable);
#ifdef CONFIG_MHL_NEW_CBUS_MSC_CMD
	void (*vbus_present)(bool on , int mhl_charge);
#else
	void (*vbus_present)(bool on);
#endif
	u8 power_state;
	struct i2c_client *mhl_tx_client;
	struct i2c_client *tpi_client;
	struct i2c_client *hdmi_rx_client;
	struct i2c_client *cbus_client;
#endif
	u32 swing_level;
};

extern u8 mhl_onoff_ex(bool on);

#endif

#endif
