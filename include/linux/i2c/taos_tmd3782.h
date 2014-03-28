/*
 * Copyright (C) 2010 Samsung Electronics. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

#ifndef __LINUX_TAOS_H
#define __LINUX_TAOS_H

#include <linux/types.h>

#ifdef __KERNEL__
#define TAOS_OPT "taos-opt"
#define MIN 1

#define WINTYPE_OTHERS	'0'
#define WINTYPE_WHITE		'2'
#define WHITEWINDOW_HI_THRESHOLD		720
#define WHITEWINDOW_LOW_THRESHOLD		590
#if defined(CONFIG_MACH_MELIUS_USC) || defined(CONFIG_MACH_MELIUS_SPR)
#define BLACKWINDOW_HI_THRESHOLD		750
#define BLACKWINDOW_LOW_THRESHOLD		520
#else
#define BLACKWINDOW_HI_THRESHOLD		650
#define BLACKWINDOW_LOW_THRESHOLD		520
#endif

struct taos_platform_data {
	int als_int;
	void (*power)(bool);
	int (*light_adc_value)(void);
	void (*led_on)(bool);
	int prox_thresh_hi;
	int prox_thresh_low;
	int prox_th_hi_cal;
	int prox_th_low_cal;
	int als_time;
	int intr_filter;
	int prox_pulsecnt;
	int als_gain;
	int coef_atime;
	int ga;
	int coef_a;
	int coef_b;
	int coef_c;
	int coef_d;
	int min_max;
};
#endif /*__KERNEL__*/
#endif
