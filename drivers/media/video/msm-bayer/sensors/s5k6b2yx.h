/* Copyright (c) 2012, Code Aurora Forum. All rights reserved.
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

#include <mach/board.h>
extern struct platform_driver s5k6b2yx_driver;
extern struct class *camera_class;

#define	S5K6B2YX_DEBUG
#ifdef S5K6B2YX_DEBUG
#define CAM_DEBUG(fmt, arg...)	\
do {\
	printk(KERN_DEBUG "[S5K6B2YX] %s:" fmt "\n", \
	__func__, ##arg); } \
	while (0)

#define cam_info(fmt, arg...)	\
do {\
	printk(KERN_INFO "[S5K6B2YX] %s:" fmt "\n", __func__, ##arg); } \
	while (0)

#define cam_err(fmt, arg...)	\
do {\
	printk(KERN_ERR "[S5K6B2YX] %s:" fmt "\n", __func__, ##arg); } \
	while (0)

#define cam_i2c_dbg(fmt, arg...)	\
do { \
	printk(KERN_ERR "[S5K6B2YX] %s:" fmt "\n", __func__, ##arg); } \
	while (0)
#else
#define CAM_DEBUG(fmt, arg...)
#define cam_info(fmt, arg...)
#define cam_err(fmt, arg...)
#define cam_i2c_dbg(fmt, arg...)
#endif
