/*
 * Copyright (C) 2012 Samsung Electronics, Inc.
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

#ifndef __ASM_ARCH_SEC_HEADSET_H
#define __ASM_ARCH_SEC_HEADSET_H

#ifdef __KERNEL__

enum {
	SEC_JACK_NO_DEVICE				= 0x0,
	SEC_HEADSET_4POLE				= 0x01 << 0,
	SEC_HEADSET_3POLE				= 0x01 << 1,
};

struct sec_jack_zone {
	unsigned int adc_high;
	unsigned int delay_us;
	unsigned int check_count;
	unsigned int jack_type;
};

struct sec_jack_buttons_zone {
	unsigned int code;
	unsigned int adc_low;
	unsigned int adc_high;
};

struct sec_jack_platform_data {
#if defined(CONFIG_SAMSUNG_JACK_GNDLDET)
	int	(*get_l_jack_state) (void);
#endif
	int	(*get_send_key_state) (void);
	void	(*set_micbias_state) (bool);
	int	(*get_adc_value) (void);
	struct sec_jack_zone			*zones;
	struct sec_jack_buttons_zone	*buttons_zones;
	int	num_zones;
	int	num_buttons_zones;
	int	det_gpio;
	int	send_end_gpio;
	bool	det_active_high;
	bool	send_end_active_high;
};

#endif

#endif
