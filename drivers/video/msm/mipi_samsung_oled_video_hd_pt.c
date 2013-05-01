/* Copyright (c) 2010-2011, Code Aurora Forum. All rights reserved.
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

#include "msm_fb.h"
#include "msm_fb_panel.h"
#include "mipi_dsi.h"
#include "mipi_samsung_oled.h"
#ifdef CONFIG_SAMSUNG_CMC624
#include "samsung_cmc624.h"
#endif

static struct msm_panel_info pinfo;
static struct mipi_panel_data mipi_pd;

extern unsigned char LCD_Get_Value(void);

#define MAX_GAMMA_VALUE 25
#define FAST_INIT_MAX_CMDS ARRAY_SIZE(samsung_panel_ready_to_on_cmds_4_8_fast)
/*
 * [1] Level2 / MTP key Enable
 */
static char mtp_key_enable1[] = {
	0xF0,
	0x5A, 0x5A
};
static char mtp_key_enable2[] = {
	0xF1,
	0x5A, 0x5A
};

/*
 * [2] Panel Condition Set
 */
 /* FOR 4.65 inch */
#if defined(CONFIG_MACH_ACCELERATE)
static char panel_cond_set[] = {
	0xF8,
	0x19, 0x35, 0x00, 0x00, 0x00,
	0x93, 0x00, 0x3C, 0x7D, 0x08,
	0x27, 0x7D, 0x3F, 0x10, 0x08,
	0x00, 0x20, 0x04, 0x08, 0x6E,
	0x08, 0x00, 0x00, 0x02, 0x08,
	0x08, 0x23, 0x23, 0xC0, 0xC1,
	0x01, 0x81, 0xC1, 0x00, 0xC1,
	0xF6, 0xF6, 0xC1
};
#else
static char panel_cond_set[] = {
	0xF8,
	0x3D, 0x32, 0x00, 0x00, 0x00,
	0x8D, 0x00, 0x39, 0x78, 0x08,
	0x26, 0x78, 0x3C, 0x00, 0x00,
	0x00, 0x20, 0x04, 0x08, 0x69,
	0x00, 0x00, 0x00, 0x02, 0x07,
	0x07, 0x21, 0x21, 0xC0, 0xC8,
	0x08, 0x48, 0xC1, 0x00, 0xC1,
	0xFF, 0xFF, 0xC8
};
#endif
/* FOR 4.8 inch */
static char panel_cond_set_4_8[] = {
	0xF8,
	0x19, 0x35, 0x00, 0x00, 0x00,
	0x94, 0x00, 0x3C, 0x7D, 0x10,
	0x27, 0x08, 0x6E, 0x00, 0x00,
	0x00, 0x00, 0x04, 0x08, 0x6E,
	0x00, 0x00, 0x00, 0x00, 0x08,
	0x08, 0x23, 0x37, 0xC0, 0xC1,
	0x01, 0x81, 0xC1, 0x00, 0xC3,
	0xF6, 0xF6, 0xC1
};
/*
 * [2] Display Condition Set
 */
static char display_cond_set[] = {
	0xF2,
	0x80, 0x03, 0x0D
};

/*
 * [3] Gamma Condition Set
 */


/* 300 cd
static char gamma_cond_set[] = {
	0xFA,
	0x01, 0x7A, 0x39, 0x87, 0xBD,
	0xD7, 0xA7, 0xAE, 0xC2, 0xA1,
	0xBE, 0xCD, 0xB6, 0x8B, 0x9E,
	0x82, 0xA6, 0xB2, 0xA1, 0x00,
	0xD8, 0x00, 0xC0, 0x00, 0xF7
};
*/
/* For 4.65 inch */
static char gamma_cond_set[] = {
	0xFA,
	0x01, 0x4A, 0x01, 0x4D, 0x85, 0xAD,
	0xA4, 0xA1, 0xCD, 0xA7, 0xBD, 0xDA,
	0xBD, 0x95, 0xBD, 0x95, 0xB7, 0xCE,
	0xB5, 0x00, 0x8F, 0x00, 0x82, 0x00, 0xB8,
};
/* For 4.8 inch */
static char gamma_cond_set_4_8[] = {
	0xFA,
	0x01, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
};
static char gamma_set_update[] = {
	0xF7,
	0x03,
	/* adding 8 byte padding*/
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

/*
 * [4] ETC Condition Set
 */
static char etc_cond_set1[] = {
	0xF6,
	0x00, 0x02, 0x00
};
/* For 4.65 inch */
static char etc_cond_set2[] = {
	0xB6,
	0x0C, 0x02, 0x03, 0x32, 0xFF,
	0x44, 0x44, 0xC0, 0x00
};
/* For 4.8 inch */
static char etc_cond_set2_4_8[] = {
	0xB6,
	0x0C, 0x02, 0x03, 0x32, 0xC0,
	0x44, 0x44, 0xC0, 0x00
};
/* For 4.65 inch */
static char etc_cond_set3[] = {
	0xD9,
	0x14, 0x40, 0x0C, 0xCB, 0xCE,
	0x6E, 0xC4, 0x07, 0x40, 0x40,
	0xD0, 0x00, 0x60, 0x19
};
/* For 4.8 inch */
static char etc_cond_set3_4_8[] = {
	0xD9,
	0x14, 0x40, 0x0C, 0xCB, 0xCE,
	0x6E, 0xC4, 0x07, 0x40, 0x41,
	0xC1, 0x00, 0x60, 0x19
};
static char etc_cond_set4[] = {
	0xE1,
	0x10, 0x1C, 0x17, 0x08, 0x1D
};
static char etc_cond_set5[] = {
	0xE2,
	0xED, 0x07, 0xC3, 0x13, 0x0D,
	0x03
};
static char etc_cond_set6[] = {
	0xE3,
	0x40
};
static char etc_cond_set7[] = {
	0xE4,
	0x00, 0x0, 0x14, 0x80, 0x00,
	0x00, 0x00
};
/* For 4.65 inch */
static char etc_cond_set8[] = {
	0xF4,
	0xCF, 0x0A, 0x12, 0x10, 0x1E,
	0x33, 0x02
};
/* For 4.8 inch */
static char etc_cond_set8_4_8[] = {
	0xF4,
	0xCF, 0x0A, 0x15, 0x10, 0x19,
	0x33, 0x02
};

static char oled_gamma_7500K[] = {
	0xFA,
	0x01, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
	/* adding 8 byte padding*/
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

#ifdef USE_ELVSS
static char elvss_cond_set[] = {
	0xB1, 0x04, 0x00,
	/* adding 8 byte padding*/
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};
#endif

#ifdef USE_ACL
static char acl_on[] = {
	0xC0,
	0x01,
	/* adding 8 byte padding*/
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

static char acl_off[] = {
	0xC0,
	0x00,
	/* adding 8 byte padding*/
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};
#endif

enum {
	GAMMA_20CD,
	GAMMA_30CD,
	GAMMA_40CD,
	GAMMA_50CD,
	GAMMA_60CD,
	GAMMA_70CD,
	GAMMA_80CD,
	GAMMA_90CD,
	GAMMA_100CD,
	GAMMA_110CD,
	GAMMA_120CD,
	GAMMA_130CD,
	GAMMA_140CD,
	GAMMA_150CD,
	GAMMA_160CD,
	GAMMA_170CD,
	GAMMA_180CD,
	GAMMA_190CD,
	GAMMA_200CD,
	GAMMA_210CD,
	GAMMA_220CD,
	GAMMA_230CD,
	GAMMA_240CD,
	GAMMA_250CD,
	GAMMA_260CD,
	GAMMA_270CD,
	GAMMA_280CD,
	GAMMA_290CD,
	GAMMA_300CD,
};

static int lux_tbl_acl[] = {
	20, 30, 40, 50, 60,
	70, 80, 90, 100, 110,
	120, 130, 140, 150, 160,
	170, 180, 190, 200, 210,
	220, 230, 240, 250, 260,
	270, 280, 290, 300
};


/* 4.65" gamma table A1-line*/
static char gamma_cond_300cd[] = {
	0x5F, 0x2E, 0x67, 0xAA,
	0xC6, 0xAC, 0xB0, 0xC8, 0xBB,
	0xBE, 0xCB, 0xBD, 0x97, 0xA5,
	0x91, 0xAF, 0xB8, 0xAB, 0x00,
	0xC2, 0x00, 0xBA, 0x00, 0xE2
};

static char gamma_cond_300cd_4_8[] = {
	0x43, 0x14, 0x45, 0xAD,
	0xBE, 0xA9, 0xB0, 0xC3, 0xAF,
	0xC1, 0xCD, 0xC0, 0x95, 0xA2,
	0x91, 0xAC, 0xB5, 0xAA, 0x00,
	0xB0, 0x00, 0xA0, 0x00, 0xCC
};

static char GAMMA_SmartDimming_COND_SET[] = {
	0xFA,
	0x01, 0x4A, 0x01, 0x4D, 0x7A,
	0x5D, 0xA5, 0x9C, 0xCA, 0xA4,
	0xBD, 0xDC, 0xBE, 0x93, 0xBD,
	0x95, 0xBA, 0xD2, 0xB7, 0x00,
	0x81, 0x00, 0x75, 0x00, 0xA5,
	/* adding 8 byte padding*/
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

#ifdef AID_OPERATION_4_8_INCH
static char panel_cond_aid_ref[] = {
	0xF8,
	0x19, 0x35, 0x00, 0x00, 0x00,
	0x94, 0x00, 0x3C, 0x7D, 0x10,
	0x27, 0x08, 0x6E, 0x00, 0x00,
	0x00, 0x00, 0x04, 0x08, 0x6E,
	0x00, 0x00, 0x00, 0x00, 0x08,
	0x08, 0x23, 0x37, 0xC0, 0xC1,
	0x01, 0x81, 0xC1, 0x00, 0xC3,
	0xF6, 0xF6, 0xC1,
	/* adding 8 byte padding*/
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

static char etc_cond_set3_aid_ref[] = {
	0xD9,
	0x14, 0x40, 0x0C, 0xCB, 0xCE,
	0x6E, 0xC4, 0x07, 0xC0, 0x41,
	0xC1, 0x00, 0x60, 0x19,
	/* adding 8 byte padding*/
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};
#endif

/*
 * Display On, Stand-by Off Command
 */
static char all_pixel_off[] = { 0x22, /* no param */ };
static char normal_mode_on[] = { 0x13, /* no parm */ };
static char display_on[] = { 0x29, /* no param */ };
static char display_off[] = { 0x28, /* no param */ };
static char sleep_in[] = { 0x10, /* no param */ };
static char sleep_out[] = { 0x11, /* no param */ };

/* For 4.65 inch */
static struct dsi_cmd_desc samsung_panel_ready_to_on_cmds[] = {
/*+ condition for non CMC624 +*/
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(mtp_key_enable1), mtp_key_enable1},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(mtp_key_enable2), mtp_key_enable2},

	{DTYPE_DCS_WRITE, 1, 0, 0, 10,
		sizeof(sleep_out), sleep_out},
/*- condition for non CMC624 -*/
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(panel_cond_set), panel_cond_set},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(display_cond_set), display_cond_set},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(gamma_cond_set), gamma_cond_set},
	{DTYPE_GEN_WRITE1, 1, 0, 0, 0,
		sizeof(gamma_set_update), gamma_set_update},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(etc_cond_set1), etc_cond_set1},
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(etc_cond_set2), etc_cond_set2},
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(etc_cond_set3), etc_cond_set3},
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(etc_cond_set4), etc_cond_set4},
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(etc_cond_set5), etc_cond_set5},
	{DTYPE_GEN_WRITE1, 1, 0, 0, 0,
		sizeof(etc_cond_set6), etc_cond_set6},
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(etc_cond_set7), etc_cond_set7},
	{DTYPE_GEN_LWRITE, 1, 0, 0, 120,
		sizeof(etc_cond_set8), etc_cond_set8},
};
/* For 4.8 inch */
static struct dsi_cmd_desc samsung_panel_ready_to_on_cmds_4_8[] = {
/*+ condition for non CMC624 +*/
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(mtp_key_enable1), mtp_key_enable1},
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(mtp_key_enable2), mtp_key_enable2},

	{DTYPE_GEN_WRITE, 1, 0, 0, 10,
		sizeof(sleep_out), sleep_out},
/*- condition for non CMC624 -*/
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(panel_cond_set_4_8), panel_cond_set_4_8},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(display_cond_set), display_cond_set},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(gamma_cond_set_4_8), gamma_cond_set_4_8},
	{DTYPE_GEN_WRITE1, 1, 0, 0, 0,
		sizeof(gamma_set_update), gamma_set_update},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(etc_cond_set1), etc_cond_set1},
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(etc_cond_set2_4_8), etc_cond_set2_4_8},
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(etc_cond_set3_4_8), etc_cond_set3_4_8},
	{DTYPE_GEN_LWRITE, 1, 0, 0, 120,
		sizeof(etc_cond_set8_4_8), etc_cond_set8_4_8},
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(elvss_cond_set), elvss_cond_set},
};

static struct dsi_cmd_desc samsung_panel_ready_to_on_cmds_4_8_fast[] = {
/*+ condition for non CMC624 +*/
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(mtp_key_enable1), mtp_key_enable1},
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(mtp_key_enable2), mtp_key_enable2},

	{DTYPE_GEN_WRITE, 1, 0, 0, 10,
		sizeof(sleep_out), sleep_out},
/*- condition for non CMC624 -*/
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(panel_cond_aid_ref), panel_cond_aid_ref},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(display_cond_set), display_cond_set},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(GAMMA_SmartDimming_COND_SET),
		GAMMA_SmartDimming_COND_SET},
	{DTYPE_GEN_WRITE1, 1, 0, 0, 0,
		sizeof(gamma_set_update), gamma_set_update},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(etc_cond_set1), etc_cond_set1},
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(etc_cond_set2_4_8), etc_cond_set2_4_8},
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(etc_cond_set3_aid_ref), etc_cond_set3_aid_ref},
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(etc_cond_set8_4_8), etc_cond_set8_4_8},
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(elvss_cond_set), elvss_cond_set},
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(acl_on), acl_on},
/* Will be initialized dyanamically from prepare_fast_init_cmd_array()*/
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
	 sizeof(NULL), NULL},
};

/* For 4.65 inch */
static struct dsi_cmd_desc samsung_panel_ready_to_on_cmds_fast[] = {
/*+ condition for non CMC624 +*/
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(mtp_key_enable1), mtp_key_enable1},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(mtp_key_enable2), mtp_key_enable2},

	{DTYPE_DCS_WRITE, 1, 0, 0, 10,
		sizeof(sleep_out), sleep_out},
/*- condition for non CMC624 -*/
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(panel_cond_set), panel_cond_set},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(display_cond_set), display_cond_set},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(GAMMA_SmartDimming_COND_SET),
		GAMMA_SmartDimming_COND_SET},
	{DTYPE_GEN_WRITE1, 1, 0, 0, 0,
		sizeof(gamma_set_update), gamma_set_update},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(etc_cond_set1), etc_cond_set1},
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(etc_cond_set2), etc_cond_set2},
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(etc_cond_set3), etc_cond_set3},
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(etc_cond_set4), etc_cond_set4},
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(etc_cond_set5), etc_cond_set5},
	{DTYPE_GEN_WRITE1, 1, 0, 0, 0,
		sizeof(etc_cond_set6), etc_cond_set6},
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(etc_cond_set7), etc_cond_set7},
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(etc_cond_set8), etc_cond_set8},
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(acl_on), acl_on},
/* Will be initialized dyanamically from prepare_fast_init_cmd_array()*/
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(NULL), NULL},
};

static struct dsi_cmd_desc samsung_panel_ready_to_off_cmds[] = {
	{DTYPE_GEN_WRITE, 1, 0, 0, 0,
		sizeof(display_off), display_off},
};

static struct dsi_cmd_desc samsung_panel_on_cmds[] = {
	{DTYPE_GEN_WRITE, 1, 0, 0, 0,
		sizeof(display_on), display_on},
};

static struct dsi_cmd_desc samsung_panel_off_cmds[] = {
	{DTYPE_GEN_WRITE, 1, 0, 0, 0,
		sizeof(sleep_in), sleep_in},
};

static struct dsi_cmd_desc samsung_panel_late_on_cmds[] = {
	{DTYPE_GEN_WRITE, 1, 0, 0, 0,
		sizeof(normal_mode_on), normal_mode_on},
	{DTYPE_GEN_WRITE, 1, 0, 0, 5,
		sizeof(display_on), display_on},
};

static struct dsi_cmd_desc samsung_panel_early_off_cmds[] = {
	{DTYPE_GEN_WRITE, 1, 0, 0, 0,
		sizeof(all_pixel_off), all_pixel_off},
};

#if defined(AID_OPERATION_4_8_INCH)
static struct dsi_cmd_desc samsung_panel_gamma_update_cmds[4] = {
	{DTYPE_GEN_LWRITE, 1, 0, 0, 1,
	 sizeof(oled_gamma_7500K), oled_gamma_7500K}
	,
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
	 sizeof(panel_cond_aid_ref), panel_cond_aid_ref}
	,
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
	 sizeof(etc_cond_set3_aid_ref), etc_cond_set3_aid_ref}
	,
	{DTYPE_GEN_WRITE1, 1, 0, 0, 0,
	 sizeof(gamma_set_update), gamma_set_update}
	,
};
#else
static struct dsi_cmd_desc samsung_panel_gamma_update_cmds[2] = {
	{DTYPE_GEN_LWRITE, 1, 0, 0, 1,
	 sizeof(oled_gamma_7500K), oled_gamma_7500K}
	,
	{DTYPE_GEN_WRITE1, 1, 0, 0, 0,
	 sizeof(gamma_set_update), gamma_set_update}
	,
};
#endif

#ifdef USE_ACL
/********************* ACL *******************/
static char ACL_COND_SET_50[] = {
	0xC1,
	0x47, 0x53, 0x13, 0x53, 0x00, 0x00,
	0x02, 0xCF, 0x00, 0x00, 0x04, 0xFF,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x09, 0x10, 0x18, 0x1F, 0x27,
	0x2E, 0x36, 0x3D, 0x45, 0x4C,
	/* adding 8 byte padding*/
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};
static char ACL_COND_SET_40[] = {
	0xC1,
	0x47, 0x53, 0x13, 0x53, 0x00, 0x00,
	0x02, 0xCF, 0x00, 0x00, 0x04, 0xFF,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x07, 0x0C, 0x12, 0x17, 0x1D,
	0x23, 0x28, 0x2E, 0x33, 0x39,
	/* adding 8 byte padding*/
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};


static struct dsi_cmd_desc DSI_CMD_ACL_50 = {
DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(ACL_COND_SET_50), ACL_COND_SET_50 };
static struct dsi_cmd_desc DSI_CMD_ACL_40 = {
DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(ACL_COND_SET_40), ACL_COND_SET_40 };

static struct dsi_cmd_desc_LCD lcd_acl_table[] = {
	{0, "20", NULL},
	{0, "30", NULL},
	{0, "40", NULL},
	{40, "50", &DSI_CMD_ACL_40},
	{40, "60", &DSI_CMD_ACL_40},
	{40, "70", &DSI_CMD_ACL_40},
	{40, "80", &DSI_CMD_ACL_40},
	{40, "90", &DSI_CMD_ACL_40},
	{40, "100", &DSI_CMD_ACL_40},
	{40, "110", &DSI_CMD_ACL_40},
	{40, "120", &DSI_CMD_ACL_40},
	{40, "130", &DSI_CMD_ACL_40},
	{40, "140", &DSI_CMD_ACL_40},
	{40, "150", &DSI_CMD_ACL_40},
	{40, "160", &DSI_CMD_ACL_40},
	{40, "170", &DSI_CMD_ACL_40},
	{40, "180", &DSI_CMD_ACL_40},
	{40, "190", &DSI_CMD_ACL_40},
	{40, "200", &DSI_CMD_ACL_40},
	{40, "210", &DSI_CMD_ACL_40},
	{40, "220", &DSI_CMD_ACL_40},
	{40, "230", &DSI_CMD_ACL_40},
	{40, "240", &DSI_CMD_ACL_40},
	{40, "250", &DSI_CMD_ACL_40},
	{40, "260", &DSI_CMD_ACL_40},
	{40, "270", &DSI_CMD_ACL_40},
	{40, "280", &DSI_CMD_ACL_40},
	{40, "290", &DSI_CMD_ACL_40},
	{50, "300", &DSI_CMD_ACL_50},
};

static struct dsi_cmd_desc_LCD lcd_acl_table_4_8[] = {
	{0, "20", NULL},
	{0, "30", NULL},
	{0, "40", NULL},
	{40, "50", &DSI_CMD_ACL_40},
	{40, "60", &DSI_CMD_ACL_40},
	{40, "70", &DSI_CMD_ACL_40},
	{40, "80", &DSI_CMD_ACL_40},
	{40, "90", &DSI_CMD_ACL_40},
	{40, "100", &DSI_CMD_ACL_40},
	{40, "110", &DSI_CMD_ACL_40},
	{40, "120", &DSI_CMD_ACL_40},
	{40, "130", &DSI_CMD_ACL_40},
	{40, "140", &DSI_CMD_ACL_40},
	{40, "150", &DSI_CMD_ACL_40},
	{40, "160", &DSI_CMD_ACL_40},
	{40, "170", &DSI_CMD_ACL_40},
	{40, "180", &DSI_CMD_ACL_40},
	{40, "190", &DSI_CMD_ACL_40},
	{40, "200", &DSI_CMD_ACL_40},
	{40, "210", &DSI_CMD_ACL_40},
	{40, "220", &DSI_CMD_ACL_40},
	{40, "230", &DSI_CMD_ACL_40},
	{40, "240", &DSI_CMD_ACL_40},
	{40, "250", &DSI_CMD_ACL_40},
	{40, "260", &DSI_CMD_ACL_40},
	{40, "270", &DSI_CMD_ACL_40},
	{40, "280", &DSI_CMD_ACL_40},
	{40, "290", &DSI_CMD_ACL_40},
	{50, "300", &DSI_CMD_ACL_50},
};


static struct dsi_cmd_desc samsung_panel_acl_on_cmds[] = {
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(acl_on), acl_on},
};

static struct dsi_cmd_desc samsung_panel_acl_off_cmds[] = {
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(acl_off), acl_off},
};
static struct dsi_cmd_desc samsung_panel_acl_update_cmds[] = {
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
	 sizeof(ACL_COND_SET_40), ACL_COND_SET_40}
	,
};
#endif

static struct dsi_cmd_desc samsung_panel_elvss_update_cmds[] = {
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
	 sizeof(elvss_cond_set), elvss_cond_set},
};

static struct dsi_cmd_desc samsung_panel_elvss_update_cmds_4_8[] = {
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
	 sizeof(elvss_cond_set), elvss_cond_set},
};

/********************* ELVSS *******************/
#define LCD_ELVSS_DELTA_300CD (0)
#define LCD_ELVSS_DELTA_200CD (0x08)
#define LCD_ELVSS_DELTA_160CD (0x0D)
#define LCD_ELVSS_DELTA_100CD (0x11)
#define LCD_ELVSS_RESULT_LIMIT	(0x9F)
static int GET_ELVSS_ID[] = {
	LCD_ELVSS_DELTA_100CD,/* 0 = 20_dimming,*/
	LCD_ELVSS_DELTA_100CD,/* 1 = 30*/
	LCD_ELVSS_DELTA_100CD,/* 2 = 40*/
	LCD_ELVSS_DELTA_100CD,/* 3 = 50,*/
	LCD_ELVSS_DELTA_100CD,/* 4 = 60,*/
	LCD_ELVSS_DELTA_100CD,/* 5 = 70,*/
	LCD_ELVSS_DELTA_100CD,/* 6 = 80,*/
	LCD_ELVSS_DELTA_100CD,/* 7 = 90,*/
	LCD_ELVSS_DELTA_100CD,/* 8 = 100,*/
	LCD_ELVSS_DELTA_160CD,/* 9 = 110,*/
	LCD_ELVSS_DELTA_160CD,/* 10 = 120,*/
	LCD_ELVSS_DELTA_160CD,/* 11 = 130,*/
	LCD_ELVSS_DELTA_160CD,/* 12 = 140,*/
	LCD_ELVSS_DELTA_160CD,/* 13 = 150,*/
	LCD_ELVSS_DELTA_160CD,/* 14 = 160,*/
	LCD_ELVSS_DELTA_200CD,/* 15 = 170,*/
	LCD_ELVSS_DELTA_200CD,/* 16 = 180,*/
	LCD_ELVSS_DELTA_200CD,/* 17 = 190,*/
	LCD_ELVSS_DELTA_200CD,/* 18 = 200,*/
	LCD_ELVSS_DELTA_300CD,/* 19 = 210,*/
	LCD_ELVSS_DELTA_300CD,/* 20 = 220,*/
	LCD_ELVSS_DELTA_300CD,/* 21 = 230,*/
	LCD_ELVSS_DELTA_300CD,/* 22 = 240,*/
	LCD_ELVSS_DELTA_300CD,/* 23 = 250,*/
	LCD_ELVSS_DELTA_300CD,/* 24 = 260,*/
	LCD_ELVSS_DELTA_300CD,/* 25 = 270,*/
	LCD_ELVSS_DELTA_300CD,/* 26 = 280,*/
	LCD_ELVSS_DELTA_300CD,/* 27 = 290,*/
	LCD_ELVSS_DELTA_300CD/* 28 = 300,*/
};

#define LCD_ELVSS_DEFAULT_100CD (0x9F)
#define LCD_ELVSS_DEFAULT_160CD (0x9F)
#define LCD_ELVSS_DEFAULT_200CD (0x9F)
#define LCD_ELVSS_DEFAULT_300CD (0x95)
static int GET_DEFAULT_ELVSS_ID[] = {
	LCD_ELVSS_DELTA_100CD,/* 0 = 20_dimming,*/
	LCD_ELVSS_DELTA_100CD,/* 1 = 30*/
	LCD_ELVSS_DELTA_100CD,/* 2 = 40*/
	LCD_ELVSS_DELTA_100CD,/* 3 = 50,*/
	LCD_ELVSS_DELTA_100CD,/* 4 = 60,*/
	LCD_ELVSS_DELTA_100CD,/* 5 = 70,*/
	LCD_ELVSS_DELTA_100CD,/* 6 = 80,*/
	LCD_ELVSS_DELTA_100CD,/* 7 = 90,*/
	LCD_ELVSS_DELTA_100CD,/* 8 = 100,*/
	LCD_ELVSS_DELTA_160CD,/* 9 = 110,*/
	LCD_ELVSS_DELTA_160CD,/* 10 = 120,*/
	LCD_ELVSS_DELTA_160CD,/* 11 = 130,*/
	LCD_ELVSS_DELTA_160CD,/* 12 = 140,*/
	LCD_ELVSS_DELTA_160CD,/* 13 = 150,*/
	LCD_ELVSS_DELTA_160CD,/* 14 = 160,*/
	LCD_ELVSS_DELTA_200CD,/* 15 = 170,*/
	LCD_ELVSS_DELTA_200CD,/* 16 = 180,*/
	LCD_ELVSS_DELTA_200CD,/* 17 = 190,*/
	LCD_ELVSS_DELTA_200CD,/* 18 = 200,*/
	LCD_ELVSS_DELTA_300CD,/* 19 = 210,*/
	LCD_ELVSS_DELTA_300CD,/* 20 = 220,*/
	LCD_ELVSS_DELTA_300CD,/* 21 = 230,*/
	LCD_ELVSS_DELTA_300CD,/* 22 = 240,*/
	LCD_ELVSS_DELTA_300CD,/* 23 = 250,*/
	LCD_ELVSS_DELTA_300CD,/* 24 = 260,*/
	LCD_ELVSS_DELTA_300CD,/* 25 = 270,*/
	LCD_ELVSS_DELTA_300CD,/* 26 = 280,*/
	LCD_ELVSS_DELTA_300CD,/* 27 = 290,*/
	LCD_ELVSS_DELTA_300CD/* 28 = 300,*/
};

#define LCD_ELVSS_RESULT_LIMIT_4_8 (0x9F)
static int GET_ELVSS_ID_4_8[] = {
	0x0C,/* 0 = 20_dimming,*/
	0x0C,/* 1 = 30*/
	0x0C,/* 2 = 40*/
	0x0C,/* 3 = 50,*/
	0x0C,/* 4 = 60,*/
	0x0C,/* 5 = 70,*/
	0x0C,/* 6 = 80,*/
	0x0C,/* 7 = 90,*/
	0x0C,/* 8 = 100,*/
	0x0C,/* 9 = 110,*/
	0x0B,/* 10= 120,*/
	0x0A,/* 11= 130,*/
	0x09,/* 12= 140,*/
	0x08,/* 13= 150,*/
	0x07,/* 14= 160,*/
	0x06,/* 15= 170,*/
	0x05,/* 16= 180,*/
	0x05,/* 17= 190,*/
	0x09,/* 18= 200,*/
	0x08,/* 19= 210,*/
	0x07,/* 20= 220,*/
	0x06,/* 21= 230,*/
	0x06,/* 22= 240,*/
	0x05,/* 23= 250,*/
	0x04,/* 24= 260,*/
	0x03,/* 25= 270,*/
	0x02,/* 26= 280,*/
	0x01,/* 27= 290,*/
	0x00/* 28= 300,*/
};


static int get_candela_index(int bl_level)
{
	int backlightlevel;

	/* brightness setting from platform is from 0 to 255
	 * But in this driver, brightness is only supported from 0 to 24 */

	switch (bl_level) {
	case 0 ... 20:
		backlightlevel = GAMMA_20CD;
		break;
	case 21 ... 39:
		backlightlevel = GAMMA_30CD;
		break;
	case 40 ... 49:
		backlightlevel = GAMMA_40CD;
		break;
	case 50 ... 59:
		backlightlevel = GAMMA_50CD;
		break;
	case 60 ... 69:
		backlightlevel = GAMMA_60CD;
		break;
	case 70 ... 79:
		backlightlevel = GAMMA_70CD;
		break;
	case 80 ... 89:
		backlightlevel = GAMMA_80CD;
		break;
	case 90 ... 99:
		backlightlevel = GAMMA_90CD;
		break;
	case 100 ... 109:
		backlightlevel = GAMMA_100CD;
		break;
	case 110 ... 119:
		backlightlevel = GAMMA_110CD;
		break;
	case 120 ... 129:
		backlightlevel = GAMMA_120CD;
		break;
	case 130 ... 139:
		backlightlevel = GAMMA_130CD;
		break;
	case 140 ... 149:
		backlightlevel = GAMMA_140CD;
		break;
	case 150 ... 159:
		backlightlevel = GAMMA_150CD;
		break;
	case 160 ... 169:
		backlightlevel = GAMMA_160CD;
		break;
	case 170 ... 179:
		backlightlevel = GAMMA_170CD;
		break;
	case 180 ... 189:
		backlightlevel = GAMMA_180CD;
		break;
	case 190 ... 199:
		backlightlevel = GAMMA_190CD;
		break;
	case 200 ... 209:
		backlightlevel = GAMMA_200CD;
		break;
	case 210 ... 214:
		backlightlevel = GAMMA_210CD;
		break;

	case 215 ... 219:
		backlightlevel = GAMMA_210CD;
		break;
	case 220 ... 224:
		backlightlevel = GAMMA_220CD;
		break;
	case 225 ... 229:
		backlightlevel = GAMMA_220CD;
		break;
	case 230 ... 234:
		backlightlevel = GAMMA_230CD;
		break;
	case 235 ... 239:
		backlightlevel = GAMMA_230CD;
		break;
	case 240 ... 244:
		backlightlevel = GAMMA_240CD;
		break;
	case 245 ... 249:
		backlightlevel = GAMMA_240CD;
		break;
	case 250 ... 254:
		backlightlevel = GAMMA_250CD;
		break;
	case 255:
		if (mipi_pd.msd->dstat.auto_brightness == 1)
			backlightlevel = GAMMA_300CD;
		else
			backlightlevel = GAMMA_250CD;
		break;
	default:
		backlightlevel = GAMMA_20CD;
		break;
	}
	return backlightlevel;
}


static struct dsi_cmd_desc combined_ctrl[] = {
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
	 sizeof(oled_gamma_7500K), oled_gamma_7500K}
	,
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
	 sizeof(panel_cond_aid_ref), panel_cond_aid_ref}
	,
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
	 sizeof(etc_cond_set3_aid_ref), etc_cond_set3_aid_ref}
	,
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
	 sizeof(gamma_set_update), gamma_set_update}
	,
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
	 sizeof(elvss_cond_set), elvss_cond_set}
	,
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
	sizeof(acl_on),	acl_on}
	,
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
	sizeof(ACL_COND_SET_40), ACL_COND_SET_40}
	,
};

static int set_acl_on_level(int bl_level)
{
	int cd;
	cd = get_candela_index(bl_level);

	if ((LCD_Get_Value() == 0x20) || bypass_LCD_Id() == 0x20 ||\
		(LCD_Get_Value() == 0x40)) {
		if (!lcd_acl_table_4_8[cd].lux)
			return 1;

		samsung_panel_acl_update_cmds[0].dlen =
		    lcd_acl_table_4_8[cd].cmd->dlen;
		samsung_panel_acl_update_cmds[0].payload =
		    lcd_acl_table_4_8[cd].cmd->payload;
	} else {
		if (!lcd_acl_table[cd].lux)
			return 1;

		samsung_panel_acl_update_cmds[0].dlen =
		    lcd_acl_table[cd].cmd->dlen;
		samsung_panel_acl_update_cmds[0].payload =
		    lcd_acl_table[cd].cmd->payload;
	}
	if (!lcd_acl_table_4_8[cd].lux)
		return 1;

	samsung_panel_acl_update_cmds[0].dlen =
	    lcd_acl_table_4_8[cd].cmd->dlen;
	samsung_panel_acl_update_cmds[0].payload =
	    lcd_acl_table_4_8[cd].cmd->payload;
	return 0;
}

static int is_acl_para_change(int bl_level)
{
	int cd = get_candela_index(bl_level);
	int change = 0;

	if (!lcd_acl_table_4_8[cd].lux)
		return 0;

	change = memcmp(samsung_panel_acl_update_cmds[0].payload,
			lcd_acl_table_4_8[cd].cmd->payload,
			lcd_acl_table_4_8[cd].cmd->dlen);
	return change;
}

static int set_elvss_level(int bl_level)
{
	unsigned char calc_elvss;
	int cd;
	unsigned char elvss_pulse;
	cd = get_candela_index(bl_level);
	elvss_pulse = mipi_pd.lcd_elvss_data[0];

	if (elvss_pulse == 0)
		calc_elvss = GET_DEFAULT_ELVSS_ID[cd];
	else
		calc_elvss = elvss_pulse + GET_ELVSS_ID[cd];

	pr_debug("%s: elvss_pulse=%x,calc_elvss = %x\n", __func__, elvss_pulse,
		calc_elvss);
	if (calc_elvss > LCD_ELVSS_RESULT_LIMIT)
		calc_elvss = LCD_ELVSS_RESULT_LIMIT;

	if (elvss_cond_set[2] == calc_elvss)
		return 1;

	elvss_cond_set[2] = calc_elvss;

	return 0;
}

static int set_elvss_level_4_8(int bl_level)
{
	unsigned char calc_elvss;
	int cd;
	unsigned char elvss_pulse;
	cd = get_candela_index(bl_level);
	elvss_pulse = mipi_pd.lcd_elvss_data[0];

	calc_elvss = elvss_pulse + GET_ELVSS_ID_4_8[cd];

	pr_debug("%s: elvss_pulse=%x,calc_elvss = %x\n", __func__, elvss_pulse,
		calc_elvss);
	if (calc_elvss > LCD_ELVSS_RESULT_LIMIT_4_8)
		calc_elvss = LCD_ELVSS_RESULT_LIMIT_4_8;

	if (elvss_cond_set[2] == calc_elvss)
		return 1;

	elvss_cond_set[2] = calc_elvss;

	return 0;
}

void reset_gamma_level(void)
{
	pr_info("reset_gamma_level\n");
	mipi_pd.lcd_current_cd_idx = -1;

	mipi_pd.ldi_acl_stat = false;
	elvss_cond_set[2] = 0x00;

#ifdef AID_OPERATION_4_8_INCH
	panel_cond_aid_ref[1] = 0xff;
	panel_cond_aid_ref[18] = 0xff;
	etc_cond_set3_aid_ref[9] = 0xff;
#endif
}

#ifdef AID_OPERATION_4_8_INCH
#if defined(AID_CASE_1)
/*
*	only smart dimmg range : 300CD ~ 190CD
*	AOR fix range : 180CD ~ 110CD
*	AOR adjust range : 100CD ~ 20CD
*/
#define aid_ratio_index 18
static int aid_below_110_ratio_table[10][2] = {
/*	CD	RATIO	*/
	{20,	0x88},
	{30,	0x7A},
	{40,	0x6C},
	{50,	0x5E},
	{60,	0x50},
	{70,	0x41},
	{80,	0x32},
	{90,	0x22},
	{100,	0x12},
	/* upper 110CD */
	{110,	0x42},
};
static int  aid_operation(int lux)
{
	int index;
	int ratio;
	static int aid_status;
	char panel_cond_aid_ref_1 = panel_cond_aid_ref[1] ;
	char panel_cond_aid_ref_18 = panel_cond_aid_ref[18];
	char etc_cond_set3_aid_ref_9 = etc_cond_set3_aid_ref[9];
	int no_change = 0;

	if ((LCD_Get_Value() == 0x20) || (bypass_LCD_Id() == 0x20) ||\
		(LCD_Get_Value() == 0x40)) {
		/*4.8 LCD_ID*/
		if (lux == 0) {
			panel_cond_aid_ref[1] = 0x19;
			panel_cond_aid_ref[18] = 0x04;
			etc_cond_set3_aid_ref[9] = 0x40;
			aid_status = 0;
		} else if (lux >= 190) {
			panel_cond_aid_ref[1] = 0x19;
			panel_cond_aid_ref[18] = 0x04;
			etc_cond_set3_aid_ref[9] = 0xC0;
			aid_status = 0;
		} else if (lux >= 110) {
			ratio = aid_below_110_ratio_table[9][1];

			panel_cond_aid_ref[1] = 0x59;
			panel_cond_aid_ref[18] = ratio;
			etc_cond_set3_aid_ref[9] = 0xC0;
			aid_status = 1;
		} else {
			index = (lux / 10) - 2;
			ratio = aid_below_110_ratio_table[index][1];

			panel_cond_aid_ref[1] = 0x59;
			panel_cond_aid_ref[18] = ratio;
			etc_cond_set3_aid_ref[9] = 0xC0;
			aid_status = 1;
		}
		pr_debug("%s brightness_level : %d adi_status:%d", __func__,
			lux, aid_status);
	} else {
		/* 4.65 LCD_ID*/
		memcpy(panel_cond_aid_ref,
				panel_cond_set, sizeof(panel_cond_set));
		memcpy(etc_cond_set3_aid_ref,
				etc_cond_set3, sizeof(etc_cond_set3));
		pr_info("%s LCD is 4.65 inch", __func__);
	}
	/*4.8 LCD_ID*/
	if (lux == 0) {
		panel_cond_aid_ref[1] = 0x19;
		panel_cond_aid_ref[18] = 0x04;
		etc_cond_set3_aid_ref[9] = 0x40;
		aid_status = 0;
	} else if (lux >= 190) {
		panel_cond_aid_ref[1] = 0x19;
		panel_cond_aid_ref[18] = 0x04;
		etc_cond_set3_aid_ref[9] = 0xC0;
		aid_status = 0;
	} else if (lux >= 110) {
		ratio = aid_below_110_ratio_table[9][1];

		panel_cond_aid_ref[1] = 0x59;
		panel_cond_aid_ref[18] = ratio;
		etc_cond_set3_aid_ref[9] = 0xC0;
		aid_status = 1;
	} else {
		index = (lux / 10) - 2;
		ratio = aid_below_110_ratio_table[index][1];

		panel_cond_aid_ref[1] = 0x59;
		panel_cond_aid_ref[18] = ratio;
		etc_cond_set3_aid_ref[9] = 0xC0;
		aid_status = 1;
	}
	pr_debug("%s brightness_level : %d adi_status:%d", __func__,
		lux, aid_status);

	no_change = (panel_cond_aid_ref_1 == panel_cond_aid_ref[1])
		&& (panel_cond_aid_ref_18 == panel_cond_aid_ref[18])
		&& (etc_cond_set3_aid_ref_9 == etc_cond_set3_aid_ref[9]);

	return !no_change;
}
#endif
#endif

static int set_gamma_level(int bl_level, enum gamma_mode_list gamma_mode)
{
	int cd;
	int *lux_tbl = lux_tbl_acl;
	int aid_change = 0;

	cd = get_candela_index(bl_level);

	if (mipi_pd.lcd_current_cd_idx == cd) {
		pr_debug("mipi_pd.lcd_current_cd_idx :%d cd:%d bl_level:%d\n",
			mipi_pd.lcd_current_cd_idx, cd, bl_level);
		return -EINVAL;
	} else
	    mipi_pd.lcd_current_cd_idx = cd;

	if (gamma_mode == GAMMA_SMART) {

		/*  SMART Dimming gamma_lux;  */
		char pBuffer[256];
		int i;
		int gamma_lux;

		gamma_lux = lux_tbl[cd];

		if (gamma_lux > SmartDimming_CANDELA_UPPER_LIMIT)
			gamma_lux = SmartDimming_CANDELA_UPPER_LIMIT;

		for (i = SmartDimming_GammaUpdate_Pos;
		     i < sizeof(GAMMA_SmartDimming_COND_SET); i++)
			GAMMA_SmartDimming_COND_SET[i] = 0;

		mipi_pd.smart_s6e8aa0x01.brightness_level = gamma_lux;
		generate_gamma(&mipi_pd.smart_s6e8aa0x01,
			&(GAMMA_SmartDimming_COND_SET[2]), GAMMA_SET_MAX);

#ifdef AID_OPERATION_4_8_INCH
		aid_change = aid_operation(gamma_lux);
#endif

		samsung_panel_gamma_update_cmds[0].dlen =
		    sizeof(GAMMA_SmartDimming_COND_SET);
		samsung_panel_gamma_update_cmds[0].payload =
		    GAMMA_SmartDimming_COND_SET;
		pBuffer[0] = 0;
		for (i = 0; i < sizeof(GAMMA_SmartDimming_COND_SET); i++) {
			snprintf(pBuffer + strnlen(pBuffer, 256), 256, " %02x",
				 GAMMA_SmartDimming_COND_SET[i]);
		}
		pr_debug("SD: %03d %s\n", gamma_lux, pBuffer);
		pr_info("bl_level:%d,cd:%d:Candela:%d aid_change:%d\n",
			bl_level, cd, gamma_lux, aid_change);
		}
	return aid_change;
}

static void  prepare_fast_init_cmd_array(int lcd_type)
{

	if (!mipi_pd.ldi_acl_stat) {
		/* dont send acl commands if acl is off */
		mipi_pd.ready_to_on_4_8_fast.size = FAST_INIT_MAX_CMDS - 2;
		return;
	}

	if (lcd_type == 0x20 || lcd_type == 0x40) {
		samsung_panel_ready_to_on_cmds_4_8_fast[FAST_INIT_MAX_CMDS-1]
			.payload = samsung_panel_acl_update_cmds[0].payload;
		samsung_panel_ready_to_on_cmds_4_8_fast[FAST_INIT_MAX_CMDS-1]
			.dlen = samsung_panel_acl_update_cmds[0].dlen;
	} else {
		samsung_panel_ready_to_on_cmds_fast[FAST_INIT_MAX_CMDS-1]
			.payload = samsung_panel_acl_update_cmds[0].payload;
		samsung_panel_ready_to_on_cmds_fast[FAST_INIT_MAX_CMDS-1]
			.dlen = samsung_panel_acl_update_cmds[0].dlen;
	}
	mipi_pd.ready_to_on_4_8_fast.size = FAST_INIT_MAX_CMDS;
	return;
}

static int prepare_brightness_control_cmd_array(int lcd_type, int bl_level)
{
	int cmd_size = 0, aid_change = 0;
	unsigned char cmds_send_flag = 0;

	aid_change = set_gamma_level(bl_level,
				mipi_pd.msd->dstat.gamma_mode);
	if (aid_change < 0)
		return -EINVAL;
	/* Prepare the list */
	cmds_send_flag |= aid_change << 0;
	if (lcd_type == 0x20 || lcd_type == 0x40) {
		if (!set_elvss_level_4_8(bl_level))
			cmds_send_flag |= 1<<1;
	} else {
		if (!set_elvss_level(bl_level))
			cmds_send_flag |= 1<<1;
	}
	if (mipi_pd.msd->dstat.acl_on) {
		int acl_change = is_acl_para_change(bl_level);
		int acl_30_40_case = set_acl_on_level(bl_level);
		if (acl_30_40_case  &&
			mipi_pd.ldi_acl_stat == true) {

			cmds_send_flag |= 1<<2;
			mipi_pd.ldi_acl_stat = false;
		}
		if (!acl_30_40_case) {
				if (mipi_pd.ldi_acl_stat == false) {

					cmds_send_flag |= 0x3<<3;
					mipi_pd.ldi_acl_stat = true;

				} else if (acl_change)
					cmds_send_flag |= 1<<4;
		}
	}
	combined_ctrl[cmd_size].payload =
		samsung_panel_gamma_update_cmds[0].payload;
	combined_ctrl[cmd_size].dlen =
		samsung_panel_gamma_update_cmds[0].dlen;
	cmd_size++;

	if (cmds_send_flag & 0x1) {
		/* aid change */
		combined_ctrl[cmd_size].payload = panel_cond_aid_ref;
		combined_ctrl[cmd_size].dlen = sizeof(panel_cond_aid_ref);
		cmd_size++;

		combined_ctrl[cmd_size].payload = etc_cond_set3_aid_ref;
		combined_ctrl[cmd_size].dlen = sizeof(etc_cond_set3_aid_ref);
		cmd_size++;
	}
	combined_ctrl[cmd_size].payload = gamma_set_update;
	combined_ctrl[cmd_size].dlen = sizeof(gamma_set_update);
	cmd_size++;

	if (cmds_send_flag & 0x2) { /* elvss change */

		combined_ctrl[cmd_size].payload =
			samsung_panel_elvss_update_cmds[0].payload;
		combined_ctrl[cmd_size].dlen =
			samsung_panel_elvss_update_cmds[0].dlen;
		cmd_size++;
	}
	if (cmds_send_flag & 0x4) { /* acl off */

		combined_ctrl[cmd_size].payload = acl_off;
		combined_ctrl[cmd_size].dlen = sizeof(acl_off);
		cmd_size++;
	}
	if (cmds_send_flag & 0x8) { /* acl on */

		combined_ctrl[cmd_size].payload = acl_on;
		combined_ctrl[cmd_size].dlen = sizeof(acl_on);
		cmd_size++;
	}
	if (cmds_send_flag & 0x10) { /* acl update */

		combined_ctrl[cmd_size].payload =
			samsung_panel_acl_update_cmds[0].payload;
		combined_ctrl[cmd_size].dlen =
			samsung_panel_acl_update_cmds[0].dlen;
		cmd_size++;
	}
	mipi_pd.combined_ctrl.size = cmd_size;
	return cmds_send_flag;
}
static struct mipi_panel_data mipi_pd = {
	.panel_name	= "SMD_AMS465GS0x\n",
	.ready_to_on	= {samsung_panel_ready_to_on_cmds
				, ARRAY_SIZE(samsung_panel_ready_to_on_cmds)},
	.ready_to_on_4_8 = {samsung_panel_ready_to_on_cmds_4_8
			, ARRAY_SIZE(samsung_panel_ready_to_on_cmds_4_8)},
	.ready_to_off	= {samsung_panel_ready_to_off_cmds
				, ARRAY_SIZE(samsung_panel_ready_to_off_cmds)},
	.on		= {samsung_panel_on_cmds
				, ARRAY_SIZE(samsung_panel_on_cmds)},
	.off		= {samsung_panel_off_cmds
				, ARRAY_SIZE(samsung_panel_off_cmds)},
	.late_on	= {samsung_panel_late_on_cmds
				, ARRAY_SIZE(samsung_panel_late_on_cmds)},
	.early_off	= {samsung_panel_early_off_cmds
				, ARRAY_SIZE(samsung_panel_early_off_cmds)},
	.gamma_update = {samsung_panel_gamma_update_cmds,
			 ARRAY_SIZE(samsung_panel_gamma_update_cmds)},
 #ifdef USE_ELVSS
	.elvss_update = {
	samsung_panel_elvss_update_cmds,
	     ARRAY_SIZE(samsung_panel_elvss_update_cmds)},
	.elvss_update_4_8 = {
	samsung_panel_elvss_update_cmds_4_8,
	     ARRAY_SIZE(samsung_panel_elvss_update_cmds_4_8)},
#endif
#ifdef USE_ACL
	.acl_on = {samsung_panel_acl_on_cmds
			, ARRAY_SIZE(samsung_panel_acl_on_cmds)},
	.acl_off = {samsung_panel_acl_off_cmds
			, ARRAY_SIZE(samsung_panel_acl_off_cmds)},
	.acl_update = {
	samsung_panel_acl_update_cmds,
	     ARRAY_SIZE(samsung_panel_acl_update_cmds)},
#endif
	.set_gamma = set_gamma_level,
	.gamma_initial = gamma_cond_set_4_8,
	.gamma_smartdim = gamma_cond_300cd,
	.gamma_smartdim_4_8 = gamma_cond_300cd_4_8,
	.set_acl = set_acl_on_level,
	.set_elvss_4_8 = set_elvss_level_4_8,
	.set_elvss = set_elvss_level,
	.lcd_current_cd_idx = -1,
	.lux_table = lux_tbl_acl,
	.lux_table_max_cnt = ARRAY_SIZE(lux_tbl_acl),
	.ready_to_on_4_8_fast = {samsung_panel_ready_to_on_cmds_4_8_fast
			, ARRAY_SIZE(samsung_panel_ready_to_on_cmds_4_8_fast)},
	.ready_to_on_fast = {samsung_panel_ready_to_on_cmds_fast
			, ARRAY_SIZE(samsung_panel_ready_to_on_cmds_fast)},
	.combined_ctrl = {combined_ctrl, ARRAY_SIZE(combined_ctrl)},
	.prepare_brightness_control_cmd_array =
		prepare_brightness_control_cmd_array,
	.prepare_fast_cmd_array =
		prepare_fast_init_cmd_array,
};

static struct mipi_dsi_phy_ctrl dsi_video_mode_phy_db = {
	/* DSI_BIT_CLK at 500MHz, 4 lane, RGB888 */
	{0x0F, 0x0a, 0x04, 0x00, 0x20}, /* regulator */
	/* timing   */
	{0xB8, 0x8E, 0x1F, 0x00, 0x97, 0x99, 0x22, 0x90,
	 0x23, 0x03, 0x04, 0xa0},
	/* phy ctrl */
	{0x5f, 0x00, 0x00, 0x10},
	/* strength */
	{0xff, 0x00, 0x06, 0x00},
	/* pll control */
	{0x0, 0x7f, 0x1, 0x1a, 0x00, 0x50, 0x48, 0x63,
	 0x31, 0x0F, 0x03,/* 4 lane */
	 0x00, 0x14, 0x03, 0x00, 0x02, 0x00, 0x20, 0x00, 0x01},
};


static int __init mipi_cmd_samsung_oled_qhd_pt_init(void)
{
	int ret;

	printk(KERN_DEBUG "[lcd] mipi_cmd_samsung_oled_qhd_pt_init start\n");

#ifdef CONFIG_FB_MSM_MIPI_PANEL_DETECT
	if (msm_fb_detect_client("mipi_cmd_samsung_oled_qhd"))
		return 0;
#endif
	pinfo.xres = 720;
	pinfo.yres = 1280;
	pinfo.mode2_xres = 0;
	pinfo.mode2_yres = 0;
	pinfo.mode2_bpp = 0;
	/*
	 *
	 * Panel's Horizontal input timing requirement is to
	 * include dummy(pad) data of 200 clk in addition to
	 * width and porch/sync width values
	 */

	pinfo.type = MIPI_VIDEO_PANEL;
	pinfo.pdest = DISPLAY_1;
	pinfo.wait_cycle = 0;
	pinfo.bpp = 24;

	pinfo.lcdc.h_back_porch = 175;
	pinfo.lcdc.h_front_porch = 173;

	pinfo.lcdc.h_pulse_width = 2;
	pinfo.lcdc.v_back_porch = 2;
	pinfo.lcdc.v_front_porch = 13;
	pinfo.lcdc.v_pulse_width = 1;
	pinfo.lcdc.border_clr = 0;	/* blk */
	pinfo.lcdc.underflow_clr = 0xff;	/* blue */
	pinfo.lcdc.hsync_skew = 0;

	pinfo.lcdc.xres_pad = 0;
	pinfo.lcdc.yres_pad = 0;

	pinfo.bl_max = 255;
	pinfo.bl_min = 1;
	pinfo.fb_num = 2;

	pinfo.clk_rate = 499000000;

	pinfo.lcd.v_back_porch = pinfo.lcdc.v_back_porch;
	pinfo.lcd.v_front_porch = pinfo.lcdc.v_front_porch;
	pinfo.lcd.v_pulse_width = pinfo.lcdc.v_pulse_width;

	pinfo.mipi.mode = DSI_VIDEO_MODE;
	pinfo.mipi.pulse_mode_hsa_he = TRUE;
	pinfo.mipi.hfp_power_stop = TRUE;
	pinfo.mipi.hbp_power_stop = TRUE;
	pinfo.mipi.hsa_power_stop = TRUE;
	pinfo.mipi.eof_bllp_power_stop = TRUE;
	pinfo.mipi.bllp_power_stop = TRUE;
	pinfo.mipi.traffic_mode = DSI_BURST_MODE;
	pinfo.mipi.dst_format = DSI_VIDEO_DST_FORMAT_RGB888;
	pinfo.mipi.vc = 0;
	pinfo.mipi.rgb_swap = DSI_RGB_SWAP_RGB;
	pinfo.mipi.data_lane0 = TRUE;
	pinfo.mipi.data_lane1 = TRUE;
	pinfo.mipi.data_lane2 = TRUE;
	pinfo.mipi.data_lane3 = TRUE;
	pinfo.mipi.t_clk_post = 0x19;
	pinfo.mipi.t_clk_pre = 0x30;
	pinfo.mipi.stream = 0; /* dma_p */
	pinfo.mipi.mdp_trigger = DSI_CMD_TRIGGER_SW;
	pinfo.mipi.dma_trigger = DSI_CMD_TRIGGER_SW;
	pinfo.mipi.frame_rate = 60;
	pinfo.mipi.dsi_phy_db = &dsi_video_mode_phy_db;
	pinfo.mipi.force_clk_lane_hs = 1;

	/*
	*	To support NONE CMC & HAS CMC
	*	some kind of parameter changing
	*/
	panel_cond_set_4_8[28] = 0x37;
	panel_cond_aid_ref[28] = 0x37;

	ret = mipi_samsung_device_register(&pinfo, MIPI_DSI_PRIM,
						MIPI_DSI_PANEL_WVGA_PT,
						&mipi_pd);
	if (ret)
		pr_err("%s: failed to register device!\n", __func__);

	printk(KERN_DEBUG "[lcd] mipi_cmd_samsung_oled_qhd_pt_init end\n");

	return ret;
}
module_init(mipi_cmd_samsung_oled_qhd_pt_init);
