/*
	$License:
	Copyright (C) 2011 InvenSense Corporation, All Rights Reserved.

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
	$
 */

/**
 * @defgroup
 * @brief
 *
 * @{
 *      @file     mpu6k_input.h
 *      @brief
 */
#ifndef __MPU6K_INPUT_H_
#define __MPU6K_INPUT_H_

#define MPU6K_INPUT_DRIVER "mpu6k_input"

struct mpu6k_input_platform_data {
	void (*power_on) (bool);
	__s8 orientation[9];
	const char *acc_cal_path;
	const char *gyro_cal_path;
};
#endif
