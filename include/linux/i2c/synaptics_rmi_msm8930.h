/*
 * Copyright (c) 2011, 2012 Synaptics Incorporated
 * Copyright (c) 2011 Unixphere
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _SYNAPTICS_RMI4_GENERIC_H_
#define _SYNAPTICS_RMI4_GENERIC_H_

#define SYNAPTICS_HW_RESET_TIME	400
#define SYNAPTICS_HW_RESET_TIME_B0	100

#define OCTA_PANEL_REVISION_43	0x02
#define OCTA_PANEL_REVISION_40	0x01
#define OCTA_PANEL_REVISION_34	0x00

struct synaptics_rmi_f1a_button_map {
	unsigned char nbuttons;
	unsigned char *map;
};

enum RMI4_CALLBACKS {
    RMI4_CALLBACK_CHARGER = 1,
    RMI4_CALLBACK_HALLSENSOR,
    RMI4_CALLBACK_TSP_RESET,
    RMI4_CALLBACK_LCD,
    RMI4_CALLBACK_MAX
};

struct synaptics_rmi_callbacks {
	void (*inform_charger)(struct synaptics_rmi_callbacks *, int);
	void (*inform_hallsensor)(struct synaptics_rmi_callbacks *, int);
	void (*inform_tsp_reset)(struct synaptics_rmi_callbacks *);
	void (*inform_lcd)(struct synaptics_rmi_callbacks *, int);
};

void synaptics_inform_callbacks(int type, int value);

/**
 * struct synaptics_rmi4_platform_data - contains the rmi4 platform data
 * @irq_number: irq number
 * @irq_type: irq type
 * @x flip: x flip flag
 * @y flip: y flip flag
 * @regulator_en: regulator enable flag
 * @gpio_config: callback for gpio set up
 *
 * This structure gives platform data for rmi4.
 */
struct synaptics_rmi4_platform_data {
	bool x_flip;
	bool y_flip;
	int panel_revision;	/* to identify panel info */
	bool regulator_en;
	unsigned gpio;
	int irq_type;
	int max_x;
	int max_y;
	int max_width;
	struct synaptics_rmi_f1a_button_map *f1a_button_map;
	int (*gpio_config)(unsigned interrupt_gpio, bool configure);
	void (*power)(bool enable);
	void (*i2c_set)(bool enable);
	void (*tkey_led_vdd_on)(bool);
	void (*register_cb)(struct synaptics_rmi_callbacks *);
};
#endif
