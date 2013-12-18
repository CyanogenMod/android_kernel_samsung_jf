/*
 * bq24157_charger.h
 * Samsung BQ24157 Charger Header
 *
 * Copyright (C) 2012 Samsung Electronics, Inc.
 *
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

#ifndef __BQ24157_CHARGER_H
#define __BQ24157_CHARGER_H __FILE__

/* Slave address should be shifted to the right 1bit.
 * R/W bit should NOT be included.
 */
#define SEC_CHARGER_I2C_SLAVEADDR	0x6a

/* BQ24157 Registers. */
#define BQ24157_STATUS		0x00
#define BQ24157_CONTROL		0x01
#define BQ24157_VOLTAGE		0x02
#define BQ24157_VENDOR		0x03
#define BQ24157_CURRENT		0x04
#define BQ24157_SPECIAL		0x05
#define BQ24157_SAFETY		0x06

#endif /* __BQ24157_CHARGER_H */
