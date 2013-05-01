/*
 * =================================================================
 *
 *       Filename:  smart_mtp_s6e8aa0x01.h
 *
 *    Description:  Smart dimming algorithm implementation
 *
 *        Author: jb09.kim
 *        Company:  Samsung Electronics
 *
 * ================================================================
 */
/*
<one line to give the program's name and a brief idea of what it does.>
Copyright (C) 2012, Samsung Electronics. All rights reserved.

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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
*/
#ifndef _SMART_MTP_S6E8AA0X01_H_
#define _SMART_MTP_S6E8AA0X01_H_

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/wait.h>
#include <linux/ctype.h>
#include <asm/div64.h>


/*
*	4.8 inch model use AID function
*	CASE#1 is used for now.
*/
#define AID_OPERATION_4_8_INCH
#define AID_CASE_1


/*
*	To set default AID algorithm
*/
#if !defined(AID_CASE_1) && !defined(AID_CASE_2) && !defined(AID_CASE_3)
#define AID_CASE_1
#endif

#define LUMINANCE_MAX 35
#define GAMMA_SET_MAX 24
#define BIT_SHIFT 14
/*
	it means BIT_SHIFT is 14.  pow(2,BIT_SHIFT) is 16384.
	BIT_SHIFT is used for right bit shfit
*/
#define BIT_SHFIT_MUL 16384

#define S6E8AA0X01_GRAY_SCALE_MAX 256

/*4.713*16384 */
#define S6E8AA0X01_VREG0_REF 77218

/*V0,V1,V15,V35,V59,V87,V177,V255*/
#define S6E8AA0X01_MAX 8

/* PANEL DEPENDENT THINGS */
#define MAX_CANDELA 300
#define MIN_CANDELA	20

/*
*	ID 0x20
*/
#define V1_300CD_R_20 0x43
#define V1_300CD_G_20 0x14
#define V1_300CD_B_20 0x45

#define V15_300CD_R_20 0xAD
#define V15_300CD_G_20 0xBE
#define V15_300CD_B_20 0xA9

#define V35_300CD_R_20 0xB0
#define V35_300CD_G_20 0xC3
#define V35_300CD_B_20 0xAF

#define V59_300CD_R_20 0xC1
#define V59_300CD_G_20 0xCD
#define V59_300CD_B_20 0xC0

#define V87_300CD_R_20 0x95
#define V87_300CD_G_20 0xA2
#define V87_300CD_B_20 0x91

#define V171_300CD_R_20 0xAC
#define V171_300CD_G_20 0xB5
#define V171_300CD_B_20 0xAA

#define V255_300CD_R_MSB_20 0x00
#define V255_300CD_R_LSB_20 0xB0

#define V255_300CD_G_MSB_20 0x00
#define V255_300CD_G_LSB_20 0xA0

#define V255_300CD_B_MSB_20 0x00
#define V255_300CD_B_LSB_20 0xCC

/*
*	ID 0x40
*/
#define V1_300CD_R_40 0x44
#define V1_300CD_G_40 0x0F
#define V1_300CD_B_40 0x42

#define V15_300CD_R_40 0xAB
#define V15_300CD_G_40 0xC1
#define V15_300CD_B_40 0xAC

#define V35_300CD_R_40 0xAF
#define V35_300CD_G_40 0xC8
#define V35_300CD_B_40 0xB3

#define V59_300CD_R_40 0xC2
#define V59_300CD_G_40 0xD0
#define V59_300CD_B_40 0xC1

#define V87_300CD_R_40 0x97
#define V87_300CD_G_40 0xA9
#define V87_300CD_B_40 0x95

#define V171_300CD_R_40 0xB3
#define V171_300CD_G_40 0xBE
#define V171_300CD_B_40 0xB3

#define V255_300CD_R_MSB_40 0x00
#define V255_300CD_R_LSB_40 0x9C

#define V255_300CD_G_MSB_40 0x00
#define V255_300CD_G_LSB_40 0x85

#define V255_300CD_B_MSB_40 0x00
#define V255_300CD_B_LSB_40 0xB2
/* PANEL DEPENDENT THINGS END*/


enum {
	V1_INDEX = 0,
	V15_INDEX = 1,
	V35_INDEX = 2,
	V59_INDEX = 3,
	V87_INDEX = 4,
	V171_INDEX = 5,
	V255_INDEX = 6,
};

struct GAMMA_LEVEL {
	int level_0;
	int level_1;
	int level_15;
	int level_35;
	int level_59;
	int level_87;
	int level_171;
	int level_255;
} __packed;

struct RGB_OUTPUT_VOLTARE {
	struct GAMMA_LEVEL R_VOLTAGE;
	struct GAMMA_LEVEL G_VOLTAGE;
	struct GAMMA_LEVEL B_VOLTAGE;
} __packed;

struct GRAY_VOLTAGE {
	/*
		This voltage value use 14bit right shit
		it means voltage is divied by 16384.
	*/
	int R_Gray;
	int G_Gray;
	int B_Gray;
} __packed;

struct GRAY_SCALE {
	struct GRAY_VOLTAGE TABLE[S6E8AA0X01_GRAY_SCALE_MAX];
} __packed;

struct MTP_SET {
	char OFFSET_1;
	char OFFSET_15;
	char OFFSET_35;
	char OFFSET_59;
	char OFFSET_87;
	char OFFSET_171;
	char OFFSET_255_MSB;
	char OFFSET_255_LSB;
} __packed;

struct MTP_OFFSET {
	/*
		MTP_OFFSET is consist of 22 byte.
		First byte is dummy and 21 byte is useful.
	*/
	struct MTP_SET R_OFFSET;
	struct MTP_SET G_OFFSET;
	struct MTP_SET B_OFFSET;
} __packed;

struct illuminance_table {
	int lux;
	char gamma_setting[GAMMA_SET_MAX];
} __packed;

struct SMART_DIM {
	struct MTP_OFFSET MTP;
	struct RGB_OUTPUT_VOLTARE RGB_OUTPUT;
	struct GRAY_SCALE GRAY;

	/* Because of AID funtion, below members are added*/
	int lux_table_max;
	int *plux_table;
	struct illuminance_table gen_table[LUMINANCE_MAX];

	int brightness_level;
	int ldi_revision;
} __packed;

void generate_gamma(struct SMART_DIM *smart_dim, char *str, int size);
int smart_dimming_init(struct SMART_DIM *psmart);
void get_min_lux_table(char *str, int size);

#endif
