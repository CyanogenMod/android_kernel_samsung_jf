/*
 * charging_temperature_data.h
 *
 * header file supporting temperature functions for Samsung device
 *
 * COPYRIGHT(C) Samsung Electronics Co., Ltd. 2012-2017 All Right Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __CHARGING_TEMPERATURE_DATA_H
#define __CHARGING_TEMPERATURE_DATA_H __FILE__

#define DEFAULT_HIGH_BLOCK_TEMP	650
#define DEFAULT_HIGH_RECOVER_TEMP	430
#define DEFAULT_LOW_BLOCK_TEMP	-50
#define DEFAULT_LOW_RECOVER_TEMP	0

static const struct pm8xxx_adc_map_pt temp_table[] = {
	{26250,	 800},
	{26583,	 750},
	{26979,	 700},
	{27429,	 650},
	{27941,	 600},
	{28523,	 550},
	{29170,	 500},
	{29875,	 450},
	{30684,	 400},
	{31705,	 350},
	{32726,	 300},
	{33747,	 250},
	{34768,	 200},
	{35789,	 150},
	{36810,	 100},
	{37837,	  50},
	{38710,	   0},
	{39539,	 -50},
	{40269,	-100},
	{41099,	-150},
	{41859,	-200},
};

#endif /* __CHARGING_TEMPERATURE_DATA_H */
