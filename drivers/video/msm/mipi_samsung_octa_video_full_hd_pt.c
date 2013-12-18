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
#include "mipi_samsung_octa.h"

static struct msm_panel_info pinfo;
static struct mipi_panel_data mipi_pd;

static int lux_tbl[] = {
	10, 11, 12, 13, 14,
	15, 16, 17, 19, 20,
	21, 22, 24, 25, 27,
	29, 30, 32, 34, 37,
	39, 41, 44, 47, 50,
	53, 56, 60, 64, 68,
	72, 77, 82, 87, 93,
	98, 105, 111, 119, 126, 
	134, 143, 152, 162, 172,
	183, 195, 207, 220, 234,
	249, 265, 282, 300,
};

static char samsung_nop[] = {
	0x0, 0x0, 0x0, 0x0,
};

static char samsung_test_key_on1[] = {
	0xF0,
	0x5A, 0x5A,
};

static char samsung_test_key_on3[] = {
	0xFC,
	0x5A, 0x5A,
};

static char samsung_test_key_off3[] = {
	0xFC,
	0xA5, 0xA5,
};

static char samsung_touchkey_off[] = {
	0xFF,
	0x1F,
};

static char samsung_touchkey_on[] = {
	0xFF,
	0x07,
};

static char samsung_touchkey_off_revH[] = {
	0xFF,
	0x1F, 0x01,
};

static char samsung_touchkey_on_revH[] = {
	0xFF,
	0x07, 0x01,
};

static char samsung_ltps_panel_control[] = {
	0xCB,
	0x1F, 0x00, 0x10, 0x10, 0xED,
	0x04, 0x00, 0x00, 0x03, 0x00,
	0x08, 0x00, 0x06, 0x00, 0x00,
	0x00, 0x04, 0x92, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x10, 0x72,
	0x10, 0x72, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x01, 0x00, 0x00,
	0x95, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x10, 0x72, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x80,
	0x88, 0x00, 0x00
};

static char samsung_scan_timming_0[] = {
	0xB0,
	0x29,
};

static char samsung_scan_timming_1[] = {
	0xFE,
	0x01, 0X12, 0X22, 0X8C,
	0XA2, 0X00, 0X80, 0X0A,
	0X01,
};

static char samsung_scan_timming_0_FE[] = {
	0xB0,
	0x2C,
};

static char samsung_scan_timming_1_FE[] = {
	0xFE,
	0x8C,
};

static char samsung_lpts_0[] = {
	0xB0,
	0x20,
};

static char samsung_lpts_1[] = {
	0xCB,
	0x04,
};

static char samsung_lpts_2[] = {
	0xB0,
	0x02,
};

static char samsung_lpts_3[] = {
	0xCB,
	0x10,
};

static char samsung_display_contol[] = {
	0xF2,
	0x00, 0x06, 0x0A,
};

static char samsung_scan_timming[] = {
	0xFD,
	0x14, 0x09,
};

static char samsung_scan_timming_after0[] = {
	0xC0,
	0x00, 0x02, 0x03, 0x32,
	0xC0, 0x44, 0x44, 0xC0,
	0x00, 0x08, 0x20,
	0xC0
};

static char samsung_scan_timming_after1[] = {
	0xF4,
	0xAB, 0x8D, 0x19, 0x02,
	0x0D, 0x8C, 0x77, 0x01,
	0x01
};

static char samsung_scan_timming_after2[] = {
	0xF6,
	0x00, 0x30, 0x04, 0x03,
	0x00, 0x00, 0x00, 0x0D
};

static char samsung_brightness_gamma[] = {
	0xCA,
	0x01, 0x00, 0x01, 0x00,
	0x01, 0x00, 0x80, 0x80,
	0x80, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x00, 0x00,
	0x00,
};

static char samsung_brightness_control_on[] = {
	0x53,
	0x20,
	0x00,
};

static char samsung_gamma_update[] = {
	0xF7,
	0x03,
	0x00,
};

static char samsung_brightness_aor_condition[] = {
	0xB2,
	0x01, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x06
};

/*
	B6 register description :
	For low temperature(REV_H) 13 ~ 16 parameter should have default value.
	All revision has same value
*/
static char samsung_elvss_condition[] = {
	0xB6,
	0x28, 0x04, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x09, 0x99, 0x9F, 0x0F,
};

static char samsung_acl_condition[] = {
	0x55,
	0x00,
};

static char samsung_brightness_aor_0[] = {
	0xB2,
	0x01, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x06,
};

static char samsung_brightness_aor_40[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x03, 0x06,
};

static char samsung_brightness_aor_10p4[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xCA,
};

static char samsung_brightness_aor_14p1[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x11,
};

static char samsung_brightness_aor_18p5[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x66,
};

static char samsung_brightness_aor_23p6[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x01, 0xC8,
};

static char samsung_brightness_aor_29p6[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x02, 0x3C,
};

static char samsung_brightness_aor_31p9[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x02, 0x68,
};

static char samsung_brightness_aor_38p7[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x02, 0xEC,
};

static char samsung_brightness_aor_41p5[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x03, 0x23,
};

static char samsung_brightness_aor_47p4[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x03, 0x95,
};

static char samsung_brightness_aor_50p7[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x03, 0xD4,
};

static char samsung_brightness_aor_57p2[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x04, 0x53,
};

static char samsung_brightness_aor_59p7[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x04, 0x82,
};

static char samsung_brightness_aor_65p6[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x04, 0xF5,
};

static char samsung_brightness_aor_68p1[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x05, 0x26,
};

static char samsung_brightness_aor_74p7[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x05, 0xA5,
};

static char samsung_brightness_aor_76p2[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x05, 0xC1,
};

static char samsung_brightness_aor_83p0[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x06, 0x46,
};

static char samsung_brightness_aor_84p6[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x06, 0x65,
};

static char samsung_brightness_aor_91p3[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x06, 0xE5,
};

static char samsung_brightness_aor_92p7[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x07, 0x01,
};

static char samsung_brightness_aor_1p65[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x20,
};

static char samsung_brightness_aor_8p53[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xA5,
};

static char samsung_brightness_aor_13p4[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x03,
};

static char samsung_brightness_aor_19p1[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x72,
};

static char samsung_brightness_aor_23p8[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x01, 0xCC,
};

static char samsung_brightness_aor_28p5[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x02, 0x27,
};

static char samsung_brightness_aor_33p2[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x02, 0x83,
};

static char samsung_brightness_aor_37p1[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x02, 0xCE,
};

static char samsung_brightness_aor_41p1[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x03, 0x1B,
};

static char samsung_brightness_aor_45p1[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x03, 0x69,
};

static char samsung_brightness_aor_48p7[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x03, 0xAE,
};

static char samsung_brightness_aor_51p4[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x03, 0xE2,
};

static char samsung_brightness_aor_54p1[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x04, 0x16,
};

static char samsung_brightness_aor_57p0[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x04, 0x4E,
};

static char samsung_brightness_aor_59p9[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x04, 0x86,
};

static char samsung_brightness_aor_62p8[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x04, 0xBF,
};

static char samsung_brightness_aor_64p7[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x04, 0xE3,
};

static char samsung_brightness_aor_66p5[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x05, 0x06,
};

static char samsung_brightness_aor_69p2[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x05, 0x3B,
};

static char samsung_brightness_aor_71p0[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x05, 0x5E,
};

static char samsung_brightness_aor_72p9[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x05, 0x82,
};
static char samsung_brightness_aor_73p7[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x05, 0x92,
};
static char samsung_brightness_aor_75p4[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x05, 0xB3,
};
static char samsung_brightness_aor_77p1[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x05, 0xD4,
};
static char samsung_brightness_aor_78p0[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x05, 0xE5,
};
static char samsung_brightness_aor_79p7[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x06, 0x06,
};
static char samsung_brightness_aor_80p6[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x06, 0x16,
};
static char samsung_brightness_aor_81p4[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x06, 0x27,
};
static char samsung_brightness_aor_82p3[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x06, 0x38,
};
static char samsung_brightness_aor_84p1[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x06, 0x5B,
};
static char samsung_brightness_aor_85p0[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x06, 0x6C,
};
static char samsung_brightness_aor_85p9[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x06, 0x7D,
};
static char samsung_brightness_aor_86p8[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x06, 0x8F,
};
static char samsung_brightness_aor_87p7[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x06, 0xA0,
};
static char samsung_brightness_aor_88p6[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x06, 0xB2,
};
static char samsung_brightness_aor_89p5[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x06, 0xC3,
};
static char samsung_brightness_aor_90p4[] = {
	0xB2,
	0x09, 0x00, 0x00, 0x00,
	0x00, 0x06, 0xD5,
};


static char samsung_brightness_aor_default[] = {
	0xB2,
	0x01, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
};

static char samsung_brightness_aor_ref[] = {
	0xB2,
	0x01, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x06,
};

static char samsung_brightness_aor_pre[] = {
	0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
};


static char samsung_brightness_elvss_default[] = {
	0xB6,
	0x28, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x09, 0x99, 0x9F, 0x0F,
};

static char samsung_brightness_elvss_ref[] = {
	0xB6,
	0x28, 0x04, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x09, 0x99, 0x9F, 0x0F,
};

static char samsung_brightness_elvss_pre[] = {
	0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,

};


static char samsung_brightness_acl_default[] = {
	0x55,
	0x00,
};

static char samsung_brightness_acl_ref[] = {
	0x55,
	0x00,
};

static char samsung_brightness_acl_pre[] = {
	0x00,
	0x00,
};

static char samsung_brightness_acl_cont_pre[] = {
	0x00,
	0x00, 0x00, 0x00,
};

/* B5h
 * 1st - 0x01 : PSRE , 0x03 : no PSRE
 * 2nd(OPR) - 0x99 : 60%
 * 3rd(ACL) - 0x23 : 27.6% , 0x27 : 30%, 0x35 : 40%
 */
static char samsung_brightness_acl_cont_ref[] = {
	0xB5,
	0x00, 0x99, 0x35,
};


static char samsung_brightness_acl_cont_default[] = {
	0xB5,
	0x03, 0x99, 0x35,
};
static char samsung_brightness_psre_cont[] = {
	0xBC,
	0x20, 0x38,
};
static char samsung_brightness_write_als[] = {
	0xE3,
	0xff, 0xff, 0xff, 0xff,
};


static char samsung_temperature_compensation[] = {
	0xB8,
	0x38, 0x07, 0x30, 0x00, 0x28,
	0x19, 0x02,
};

static char samsung_display_on[] = { 0x29, /* no param */ };
static char samsung_display_off[] = { 0x28, /* no param */ };
static char samsung_sleep_in[] = { 0x10, /* no param */ };
static char samsung_sleep_out[] = { 0x11, /* no param */ };

static struct dsi_cmd_desc samsung_on_cmds_revB[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_test_key_on1), samsung_test_key_on1},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_test_key_on3), samsung_test_key_on3},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_scan_timming_0),
				samsung_scan_timming_0},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_scan_timming_1),
				samsung_scan_timming_1},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 20,
		sizeof(samsung_sleep_out), samsung_sleep_out},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_lpts_0), samsung_lpts_0},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_lpts_1), samsung_lpts_1},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_display_contol), samsung_display_contol},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 100,
		sizeof(samsung_gamma_update), samsung_gamma_update},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_lpts_0), samsung_lpts_0},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_lpts_1), samsung_lpts_1},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_display_contol), samsung_display_contol},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_scan_timming), samsung_scan_timming},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_scan_timming_after0),
					samsung_scan_timming_after0},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_scan_timming_after1),
					samsung_scan_timming_after1},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_scan_timming_after2),
					samsung_scan_timming_after2},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_brightness_gamma),
				samsung_brightness_gamma},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
			sizeof(samsung_gamma_update), samsung_gamma_update},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_brightness_control_on),
					samsung_brightness_control_on},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_brightness_aor_condition),
				samsung_brightness_aor_condition},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_elvss_condition), samsung_elvss_condition},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 20,
		sizeof(samsung_acl_condition), samsung_acl_condition},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
			sizeof(samsung_gamma_update), samsung_gamma_update},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_test_key_on1), samsung_test_key_on1},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_test_key_on3), samsung_test_key_on3},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_touchkey_off), samsung_touchkey_off},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_display_on), samsung_display_on},
};

static struct dsi_cmd_desc samsung_on_cmds_revC[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_test_key_on1), samsung_test_key_on1},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_test_key_on3), samsung_test_key_on3},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_scan_timming_0),
				samsung_scan_timming_0},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_scan_timming_1),
				samsung_scan_timming_1},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 20,
		sizeof(samsung_sleep_out), samsung_sleep_out},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_lpts_0), samsung_lpts_0},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_lpts_1), samsung_lpts_1},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_display_contol), samsung_display_contol},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_test_key_on1), samsung_test_key_on1},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_test_key_on3), samsung_test_key_on3},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_touchkey_off), samsung_touchkey_off},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 100,
		sizeof(samsung_gamma_update), samsung_gamma_update},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_lpts_0), samsung_lpts_0},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_lpts_1), samsung_lpts_1},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_display_contol), samsung_display_contol},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_scan_timming), samsung_scan_timming},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_scan_timming_after0),
					samsung_scan_timming_after0},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_scan_timming_after1),
					samsung_scan_timming_after1},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_scan_timming_after2),
					samsung_scan_timming_after2},


	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_brightness_gamma),
				samsung_brightness_gamma},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
			sizeof(samsung_gamma_update), samsung_gamma_update},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_brightness_control_on),
					samsung_brightness_control_on},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_brightness_aor_condition),
				samsung_brightness_aor_condition},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_elvss_condition), samsung_elvss_condition},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 20,
		sizeof(samsung_acl_condition), samsung_acl_condition},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
			sizeof(samsung_gamma_update), samsung_gamma_update},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_display_on), samsung_display_on},
};



static struct dsi_cmd_desc samsung_on_cmds_revF[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_test_key_on1), samsung_test_key_on1},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_test_key_on3), samsung_test_key_on3},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_touchkey_off), samsung_touchkey_off},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 20,
		sizeof(samsung_sleep_out), samsung_sleep_out},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_lpts_0), samsung_lpts_0},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_lpts_1), samsung_lpts_1},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_display_contol), samsung_display_contol},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 100,
		sizeof(samsung_gamma_update), samsung_gamma_update},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_brightness_gamma),
				samsung_brightness_gamma},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_brightness_aor_condition),
				samsung_brightness_aor_condition},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_elvss_condition), samsung_elvss_condition},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_brightness_acl_cont_default), samsung_brightness_acl_cont_default},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_acl_condition), samsung_acl_condition},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_gamma_update), samsung_gamma_update},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_display_on), samsung_display_on},
};

static struct dsi_cmd_desc samsung_on_cmds_revG[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_test_key_on1), samsung_test_key_on1},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_test_key_on3), samsung_test_key_on3},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_touchkey_off), samsung_touchkey_off},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 20,
		sizeof(samsung_sleep_out), samsung_sleep_out},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_lpts_0), samsung_lpts_0},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_lpts_1), samsung_lpts_1},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_display_contol), samsung_display_contol},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 100,
		sizeof(samsung_gamma_update), samsung_gamma_update},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_brightness_gamma),
				samsung_brightness_gamma},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_brightness_aor_condition),
				samsung_brightness_aor_condition},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_elvss_condition), samsung_elvss_condition},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_brightness_acl_cont_default), samsung_brightness_acl_cont_default},	

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_acl_condition), samsung_acl_condition},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_temperature_compensation), samsung_temperature_compensation},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_gamma_update), samsung_gamma_update},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_display_on), samsung_display_on},
};

static struct dsi_cmd_desc samsung_on_cmds_revH[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_test_key_on1), samsung_test_key_on1},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_test_key_on3), samsung_test_key_on3},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_touchkey_off_revH), samsung_touchkey_off_revH},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 20,
		sizeof(samsung_sleep_out), samsung_sleep_out},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_lpts_0), samsung_lpts_0},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_lpts_1), samsung_lpts_1},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_display_contol), samsung_display_contol},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 100,
		sizeof(samsung_gamma_update), samsung_gamma_update},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_brightness_gamma),
				samsung_brightness_gamma},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_brightness_aor_condition),
				samsung_brightness_aor_condition},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_elvss_condition), samsung_elvss_condition},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_scan_timming_0_FE), samsung_scan_timming_0_FE},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_scan_timming_1_FE), samsung_scan_timming_1_FE},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_brightness_acl_cont_default), samsung_brightness_acl_cont_default},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_acl_condition), samsung_acl_condition},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_temperature_compensation), samsung_temperature_compensation},	

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_gamma_update), samsung_gamma_update},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
			sizeof(samsung_touchkey_on_revH), samsung_touchkey_on_revH},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_display_on), samsung_display_on},
};

static struct dsi_cmd_desc samsung_on_cmds_revI[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_test_key_on1), samsung_test_key_on1},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_test_key_on3), samsung_test_key_on3},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_touchkey_off_revH), samsung_touchkey_off_revH},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_test_key_off3), samsung_test_key_off3},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 20,
		sizeof(samsung_sleep_out), samsung_sleep_out},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_lpts_0), samsung_lpts_0},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_lpts_1), samsung_lpts_1},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_lpts_2), samsung_lpts_2},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_lpts_3), samsung_lpts_3},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_display_contol), samsung_display_contol},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 100,
		sizeof(samsung_gamma_update), samsung_gamma_update},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_brightness_gamma),
				samsung_brightness_gamma},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_brightness_aor_condition),
				samsung_brightness_aor_condition},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_elvss_condition), samsung_elvss_condition},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_brightness_acl_cont_default), samsung_brightness_acl_cont_default},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_acl_condition), samsung_acl_condition},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_temperature_compensation), samsung_temperature_compensation},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_gamma_update), samsung_gamma_update},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_test_key_on3), samsung_test_key_on3},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_touchkey_on_revH), samsung_touchkey_on_revH},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_test_key_off3), samsung_test_key_off3},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_display_on), samsung_display_on},
};

static struct dsi_cmd_desc panel_off_cmds[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 40,
		sizeof(samsung_display_off), samsung_display_off},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 120,
		sizeof(samsung_sleep_in), samsung_sleep_in},
};

static struct dsi_cmd_desc panel_late_on_cmds[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 5,
		sizeof(samsung_display_on), samsung_display_on},
};

static struct dsi_cmd_desc panel_early_off_cmds[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_sleep_in), samsung_sleep_in},
};

static struct dsi_cmd_desc panel_mtp_enable_cmds[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_test_key_on1), samsung_test_key_on1},
};

static struct dsi_cmd_desc panel_mtp_disable_cmds[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_nop),
					samsung_nop},
};

static struct dsi_cmd_desc panel_need_flip_cmds[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_ltps_panel_control), samsung_ltps_panel_control},
};

static struct dsi_cmd_desc panel_touchkey_on[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(samsung_touchkey_on), samsung_touchkey_on},
};

static struct dsi_cmd_desc brightness_packet[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(samsung_nop), samsung_nop},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(samsung_nop), samsung_nop},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(samsung_nop), samsung_nop},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(samsung_nop), samsung_nop},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(samsung_nop), samsung_nop},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(samsung_nop), samsung_nop},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(samsung_nop), samsung_nop},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(samsung_nop), samsung_nop},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(samsung_nop), samsung_nop},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0, sizeof(samsung_nop), samsung_nop},
};

static struct dsi_cmd_desc panel_acl_cmds[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
	sizeof(samsung_brightness_acl_cont_default), samsung_brightness_acl_cont_default},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
	sizeof(samsung_brightness_acl_ref), samsung_brightness_acl_ref},
};

enum {
	GAMMA_10CD,
	GAMMA_11CD,
	GAMMA_12CD,
	GAMMA_13CD,
	GAMMA_14CD,
	GAMMA_15CD,
	GAMMA_16CD,
	GAMMA_17CD,
	GAMMA_19CD,
	GAMMA_20CD,
	GAMMA_21CD,
	GAMMA_22CD,
	GAMMA_24CD,
	GAMMA_25CD,
	GAMMA_27CD,
	GAMMA_29CD,
	GAMMA_30CD,
	GAMMA_32CD,
	GAMMA_34CD,
	GAMMA_37CD,
	GAMMA_39CD,
	GAMMA_41CD,
	GAMMA_44CD,
	GAMMA_47CD,
	GAMMA_50CD,
	GAMMA_53CD,
	GAMMA_56CD,
	GAMMA_60CD,
	GAMMA_64CD,
	GAMMA_68CD,
	GAMMA_72CD,
	GAMMA_77CD,
	GAMMA_82CD,
	GAMMA_87CD,
	GAMMA_93CD,
	GAMMA_98CD,
	GAMMA_105CD,
	GAMMA_111CD,
	GAMMA_119CD,
	GAMMA_126CD,
	GAMMA_134CD,
	GAMMA_143CD,
	GAMMA_152CD,
	GAMMA_162CD,
	GAMMA_172CD,
	GAMMA_183CD,
	GAMMA_195CD,
	GAMMA_207CD,
	GAMMA_220CD,
	GAMMA_234CD,
	GAMMA_249CD,
	GAMMA_265CD,
	GAMMA_282CD,
	GAMMA_300CD,
};

static int get_candela_index(int bl_level)
{
	int backlightlevel;

	/* brightness setting from platform is from 0 to 255
	 * But in this driver, brightness is only supported from 0 to 24 */

	backlightlevel = 0;

	switch (bl_level) {
	case 0 ... 10:
		backlightlevel = GAMMA_10CD;
		break;
	case 11:
		backlightlevel = GAMMA_11CD;
		break;
	case 12:
		backlightlevel = GAMMA_12CD;
		break;
	case 13:
		backlightlevel = GAMMA_13CD;
		break;
	case 14:
		backlightlevel = GAMMA_14CD;
		break;
	case 15:
		backlightlevel = GAMMA_15CD;
		break;
	case 16:
		backlightlevel = GAMMA_16CD;
		break;
	case 17 ... 18:
		backlightlevel = GAMMA_17CD;
		break;
	case 19:
		backlightlevel = GAMMA_19CD;
		break;
	case 20:
		backlightlevel = GAMMA_20CD;
		break;
	case 21:
		backlightlevel = GAMMA_21CD;
		break;
	case 22 ... 23:
		backlightlevel = GAMMA_22CD;
		break;
	case 24:
		backlightlevel = GAMMA_24CD;
		break;
	case 25 ... 26:
		backlightlevel = GAMMA_25CD;
		break;
	case 27 ... 28:
		backlightlevel = GAMMA_27CD;
		break;
	case 29:
		backlightlevel = GAMMA_29CD;
		break;
	case 30 ... 31:
		backlightlevel = GAMMA_30CD;
		break;
	case 32:
		backlightlevel = GAMMA_32CD;
		break;
	case 33 ... 36:
		backlightlevel = GAMMA_34CD;
		break;
	case 37 ... 38:
		backlightlevel = GAMMA_37CD;
		break;
	case 39 ... 40:
		backlightlevel = GAMMA_39CD;
		break;
	case 41 ... 43:
		backlightlevel = GAMMA_41CD;
		break;
	case 44 ... 46:
		backlightlevel = GAMMA_44CD;
		break;
	case 47 ... 49:
		backlightlevel = GAMMA_47CD;
		break;
	case 50 ... 52:
		backlightlevel = GAMMA_50CD;
		break;
	case 53 ... 55:
		backlightlevel = GAMMA_53CD;
		break;
	case 56 ... 59:
		backlightlevel = GAMMA_56CD;
		break;
	case 60 ... 63:
		backlightlevel = GAMMA_60CD;
		break;
	case 64 ... 67:
		backlightlevel = GAMMA_64CD;
		break;
	case 68 ... 71:
		backlightlevel = GAMMA_68CD;
		break;
	case 72 ... 76:
		backlightlevel = GAMMA_72CD;
		break;
	case 77 ... 81:
		backlightlevel = GAMMA_77CD;
		break;
	case 82 ... 86:
		backlightlevel = GAMMA_82CD;
		break;
	case 87 ... 92:
		backlightlevel = GAMMA_87CD;
		break;
	case 93 ... 97:
		backlightlevel = GAMMA_93CD;
		break;
	case 98 ... 104:
		backlightlevel = GAMMA_98CD;
		break;
	case 105 ... 110:
		backlightlevel = GAMMA_105CD;
		break;
	case 111 ... 118:
		backlightlevel = GAMMA_111CD;
		break;
	case 119 ... 125:
		backlightlevel = GAMMA_119CD;
		break;
	case 126 ... 133:
		backlightlevel = GAMMA_126CD;
		break;
	case 134 ... 142:
		backlightlevel = GAMMA_134CD;
		break;
	case 143 ... 149:
		backlightlevel = GAMMA_143CD;
		break;
	case 150 ... 161:
		backlightlevel = GAMMA_152CD;
		break;
	case 162 ... 171:
		backlightlevel = GAMMA_162CD;
		break;
	case 172 ... 182:
		backlightlevel = GAMMA_172CD;
		break;
	case 183 ... 194:
		backlightlevel = GAMMA_183CD;
		break;
	case 195 ... 206:
		backlightlevel = GAMMA_195CD;
		break;
	case 207 ... 219:
		backlightlevel = GAMMA_207CD;
		break;
	case 220 ... 232:
		backlightlevel = GAMMA_220CD;
		break;
	case 233 ... 248:
		backlightlevel = GAMMA_234CD;
		break;
	case 249:
		backlightlevel = GAMMA_249CD;
		break;
	case 250 ... 251:
		backlightlevel = GAMMA_265CD;
		break;
	case 252 ... 253:
		backlightlevel = GAMMA_282CD;
		break;
	case 254 ... 255:
		backlightlevel = GAMMA_300CD;
		break;
	default:
		pr_info("%s lcd error bl_level : %d", __func__, bl_level);
		backlightlevel = GAMMA_152CD;
		break;
	}

	return backlightlevel;
}

#define DEFAULT_ELVSS 0x04
#define DEFAULT_ELVSS_EVT1 0x0A

static int elvss_array_default[][2] = {
	{110, 0x0B},	/* 110CD 0*/
	{120, 0x0A},	/* 120CD 1*/
	{130, 0x09},	/* 130CD 2*/
	{140, 0x08},	/* 140CD 3*/
	{150, 0x07},	/* 150CD 4*/
	{160, 0x06},	/* 160CD 5*/
	{170, 0x05},	/* 170CD 6*/
	{180, 0x05},	/* 180CD 7*/
	{190, 0x09},	/* 190CD 8*/
	{200, 0x08},	/* 200CD 9*/
	{210, 0x07},	/* 210CD 10*/
	{220, 0x06},	/* 220CD 11*/
	{230, 0x06},	/* 230CD 12*/
	{240, 0x05},	/* 240CD 13*/
	{250, 0x04},	/* 250CD 14*/
	{260, 0x03},	/* 260CD 15*/
	{270, 0x02},	/* 270CD 16*/
	{280, 0x02},	/* 280CD 17*/
	{290, 0x01},	/* 290CD 18*/
	{300, 0x00},	/* 300CD 19*/
};

static int elvss_array[][2] = {
	{9,  0x00},	/* default*/
	{105, 0x0F},	/* 105CD 0*/
	{111, 0x0B},	/* 111CD 1*/
	{119, 0x0A},	/* 119CD 2*/
	{126, 0x09},	/* 126CD 3*/
	{134, 0x08},	/* 134CD 4*/
	{143, 0x07},	/* 143CD 5*/
	{152, 0x06},	/* 152CD 6*/
	{162, 0x05},	/* 162CD 7*/
	{172, 0x05},	/* 172CD 8*/
	{183, 0x09},	/* 183CD 9*/
	{195, 0x08},	/* 195CD 10*/
	{207, 0x07},	/* 207CD 11*/
	{220, 0x06},	/* 220CD 12*/
	{234, 0x05},	/* 234CD 13*/
	{249, 0x04},	/* 249CD 14*/
	{265, 0x03},	/* 265CD 15*/
	{282, 0x01},	/* 282CD 16*/
	{300, 0x00},	/* 300CD 17*/
};
#define EVLSS_ARRAY_SIZE (sizeof(elvss_array)/sizeof(elvss_array[0]))

static int get_elvss_value(int candela, int id3)
{
	int elvss_value;
	int default_elvss;
	int i;

	elvss_value = 0;

	if ((id3 == EVT1_ID) || (id3 == EVT1_FIRST_ID) || (id3 == EVT1_SECOND_ID)
		|| (id3 == EVT1_THIRD_ID) || (id3 == EVT1_FOUTRH_ID) 
		|| (id3 == EVT1_REV_H_ID3_1) || (id3 == EVT1_REV_H_ID3_2)
		|| (id3 == EVT1_REV_I_ID3_1) || (id3 == EVT1_REV_I_ID3_2))
		default_elvss = DEFAULT_ELVSS_EVT1;
	else
		default_elvss = DEFAULT_ELVSS;

	if (candela <= 105) {
		elvss_value = default_elvss + 0x0F;
		return elvss_value;
	} 

	if ((id3 == CCG6_ID) || (id3 == EVT1_ID)
		|| (id3 == EVT1_FIRST_ID) || (id3 == EVT1_SECOND_ID) 
		|| (id3 == EVT1_THIRD_ID) || (id3 == EVT1_FOUTRH_ID)
		|| (id3 == EVT1_REV_H_ID3_1) || (id3 == EVT1_REV_H_ID3_2)
		|| (id3 == EVT1_REV_I_ID3_1) || (id3 == EVT1_REV_I_ID3_2)) {

		for (i=0; i< EVLSS_ARRAY_SIZE-1; i++) {
			if(candela > elvss_array[i][0] &&
				candela <= elvss_array[i+1][0])
			{
				pr_debug("%s : candela = %d, elvss_array[%d+1][1] = %x\n",
					__func__, candela, i, elvss_array[i+1][1]);
				elvss_value = default_elvss + elvss_array[i+1][1];
				break;
			}
		}
	} else {
		elvss_value = default_elvss +
				elvss_array_default[((candela - 100)/10)-1][1];
	}

	return elvss_value;
}

static void aor_copy_vt888(int candela)
{
	if (candela >= 190) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_0,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela >= 110) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_40,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela >= 100) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_14p1,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela >= 90) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_23p6,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela >= 80) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_31p9,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela >= 70) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_41p5,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela >= 60) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_50p7,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela >= 50) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_59p7,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela >= 40) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_68p1,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela >= 30) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_76p2,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela >= 20) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_84p6,
					sizeof(samsung_brightness_aor_ref));
	} else {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_92p7,
					sizeof(samsung_brightness_aor_ref));
	}
}

static void aor_copy_vt232(int candela)
{
	if (candela >= 190) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_0,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela >= 110) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_40,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela >= 100) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_10p4,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela >= 90) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_18p5,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela >= 80) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_29p6,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela >= 70) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_38p7,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela >= 60) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_47p4,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela >= 50) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_57p2,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela >= 40) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_65p6,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela >= 30) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_74p7,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela >= 20) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_83p0,
					sizeof(samsung_brightness_aor_ref));
	} else {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_91p3,
					sizeof(samsung_brightness_aor_ref));
	}
}

static void aor_copy_CCG6(int candela)
{
	if (candela >= 183) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_0,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela >= 111) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_40,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 105) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_1p65,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 98) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_8p53,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 93) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_13p4,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 87) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_19p1,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 82) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_23p8,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 77) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_28p5,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 72) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_33p2,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 68) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_37p1,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 64) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_41p1,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 60) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_45p1,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 56) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_48p7,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 53) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_51p4,
					sizeof(samsung_brightness_aor_ref));	
	} else if (candela == 50) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_54p1,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 47) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_57p0,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 44) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_59p9,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 41) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_62p8,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 39) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_64p7,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 37) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_66p5,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 34) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_69p2,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 32) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_71p0,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 30) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_72p9,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 29) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_73p7,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 27) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_75p4,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 25) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_77p1,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 24) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_78p0,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 22) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_79p7,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 21) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_80p6,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 20) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_81p4,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 19) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_82p3,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 17) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_84p1,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 16) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_85p0,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 15) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_85p9,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 14) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_86p8,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 13) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_87p7,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 12) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_88p6,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 11) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_89p5,
					sizeof(samsung_brightness_aor_ref));
	} else if (candela == 10) {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_90p4,
					sizeof(samsung_brightness_aor_ref));
	} else {
		memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_ref,
					sizeof(samsung_brightness_aor_ref));
	}
}

static int brightness_control(int bl_level)
{
	/*
		samsung_brightness_gamma
		samsung_brightness_aor_ref
		samsung_brightness_acl_ref
		samsung_brightness_elvss_ref
		samsung_gamma_update
	*/
	int id1, id2, id3;
	int cmd_size;
	char elvss_value;
	int candela;
	int i;
	char* c8_reg_1;
	char* c8_reg_2;
	c8_reg_1 = get_c8_reg_1();
	c8_reg_2 = get_c8_reg_2();

	id1 = (mipi_pd.manufacture_id & 0x00FF0000) >> 16;
	id2 = (mipi_pd.manufacture_id & 0x0000FF00) >> 8;
	id3 = mipi_pd.manufacture_id & 0xFF;

	if (bl_level < 10)
		bl_level = 10;

	candela = lux_tbl[get_candela_index(bl_level)];

	pr_info("%s brightness_level : %d, candela : %d, auto : %d\n", 
					__func__, mipi_pd.brightness_level, candela, get_auto_brightness());

	cmd_size = 0;

	/* aor ****************************************************************************/
	/* 0xB2 setting */
	memcpy(samsung_brightness_aor_pre, samsung_brightness_aor_ref,
					sizeof(samsung_brightness_aor_ref));
	if ((id3 == CCG6_ID) || (id3 == EVT1_ID) 
		|| (id3 == EVT1_FIRST_ID) || (id3 == EVT1_SECOND_ID)
		|| (id3 == EVT1_THIRD_ID) || (id3 == EVT1_FOUTRH_ID)
		|| (id3 == EVT1_REV_H_ID3_1) || (id3 == EVT1_REV_H_ID3_2)
		|| (id3 == EVT1_REV_I_ID3_1) || (id3 == EVT1_REV_I_ID3_2))
		aor_copy_CCG6(candela);
	else if (id3 == 0x02)
		aor_copy_vt232(candela);
	else
		aor_copy_vt888(candela);

	if (memcmp(samsung_brightness_aor_pre, samsung_brightness_aor_ref,
			sizeof(samsung_brightness_aor_ref))) {
		brightness_packet[cmd_size].payload =
					samsung_brightness_aor_ref;
		brightness_packet[cmd_size].dlen =
					sizeof(samsung_brightness_aor_ref);
		cmd_size++;
	}

	/* elvss ****************************************************************************/
	/* 0xB6 setting */
	memcpy(samsung_brightness_elvss_pre, samsung_brightness_elvss_ref,
					sizeof(samsung_brightness_elvss_ref));

	elvss_value = get_elvss_value(candela, id3);

	if (elvss_value >= 0x29)
		elvss_value = 0x29;

	samsung_brightness_elvss_ref[2] = elvss_value;

	if (mipi_pd.ldi_rev >= 'G')
		samsung_brightness_elvss_ref[1] = 0x2C;

	if (mipi_pd.ldi_rev >= 'H') {
		if (get_auto_brightness() == 6)
			samsung_brightness_elvss_ref[2] = 0x01;
	} else {
		if (get_auto_brightness() == 6)
			/*if auto bl is 6, b6's 1st para has to be c8's 40th / 01h(revH) (elvss_400cd)*/
			samsung_brightness_elvss_ref[16] = get_elvss_400cd();
		else  /*recover original's ELVSS offset b6's 17th / 0x0A(revH) */
			samsung_brightness_elvss_ref[16] = get_elvss_offset();
	}
	pr_debug("%s: 0xb6_17th(%x)!!\n", __func__, samsung_brightness_elvss_ref[16]);

	// ELVSS lOW TEMPERATURE 
	if ((mipi_pd.ldi_rev >= 'G') && mipi_pd.need_update) {
		if (get_auto_brightness() != 6)
			if (mipi_pd.temperature <= -20)
				samsung_brightness_elvss_ref[2] -= 0x04;
	}

	if (memcmp(samsung_brightness_elvss_pre, samsung_brightness_elvss_ref,
				sizeof(samsung_brightness_elvss_ref))) {
		brightness_packet[cmd_size].payload =
					samsung_brightness_elvss_ref;
		brightness_packet[cmd_size].dlen =
					sizeof(samsung_brightness_elvss_ref);
		cmd_size++;
	}

	// ELVSS lOW TEMPERATURE
	if ((mipi_pd.ldi_rev >= 'G') && mipi_pd.need_update) {
		samsung_temperature_compensation[6] = mipi_pd.temperature_value;
		brightness_packet[cmd_size].payload =
					samsung_temperature_compensation;
		brightness_packet[cmd_size].dlen =
					sizeof(samsung_temperature_compensation);
		cmd_size++;
	}

	/* acl control *************************************************************************/
	/* 0xB5 setting */
	memcpy(samsung_brightness_acl_cont_pre, samsung_brightness_acl_cont_ref,
						sizeof(samsung_brightness_acl_cont_ref));
	if(get_auto_brightness() == 6) {
		samsung_brightness_acl_cont_ref[1] = 0x01; /* PSRE set */
	} else {
		samsung_brightness_acl_cont_ref[1] = 0x03; /* No PSRE set */
	}

	if (memcmp(samsung_brightness_acl_cont_pre, samsung_brightness_acl_cont_ref,
				sizeof(samsung_brightness_acl_cont_ref))) {
		brightness_packet[cmd_size].payload =
				samsung_brightness_acl_cont_ref;
		brightness_packet[cmd_size].dlen =
				sizeof(samsung_brightness_acl_cont_ref);
		cmd_size++;
	}
	pr_debug("%s: 0xB5(%x)!!\n", __func__,samsung_brightness_acl_cont_ref[1]);

	/* write als *************************************************************************/
	/* 0xE3 setting */
	if (mipi_pd.first_bl_hbm_psre  && (get_auto_brightness() == 6)) {
		brightness_packet[cmd_size].payload =
				samsung_brightness_write_als;
		brightness_packet[cmd_size].dlen =
				sizeof(samsung_brightness_write_als);
		cmd_size++;
	}

	/* write power saving *****************************************************************/
	/* 0x55 setting */
	memcpy(samsung_brightness_acl_pre, samsung_brightness_acl_ref,
					sizeof(samsung_brightness_acl_ref));

	if (get_auto_brightness() == 6) {
		samsung_brightness_acl_ref[1] = 0x41; /*RE low, ACL on 40p*/
	} else {
		if (mipi_pd.acl_status || mipi_pd.siop_status)
			samsung_brightness_acl_ref[1] = 0x01; /*ACL on 40p*/
		else
			samsung_brightness_acl_ref[1] = 0x00; /*ACL off*/
	}

	if (memcmp(samsung_brightness_acl_pre, samsung_brightness_acl_ref,
				sizeof(samsung_brightness_acl_ref))) {
		brightness_packet[cmd_size].payload =
				samsung_brightness_acl_ref;
		brightness_packet[cmd_size].dlen =
				sizeof(samsung_brightness_acl_ref);
		cmd_size++;
	}
	pr_debug("%s: 0x55(%x)!!\n", __func__, samsung_brightness_acl_ref[1]);


	/* PSRE control 1 **********************************************************************/
	/* 0xBC setting */
	if (mipi_pd.first_bl_hbm_psre  && (get_auto_brightness() == 6)) {
		brightness_packet[cmd_size].payload =
					samsung_brightness_psre_cont;
		brightness_packet[cmd_size].dlen =
					sizeof(samsung_brightness_psre_cont);
		cmd_size++;
	}

	/* gamma ******************************************************************************/
	/* 0xCA setting */
	mipi_pd.smart_se6e8fa.brightness_level = candela;

	generate_gamma(&mipi_pd.smart_se6e8fa,
			&(samsung_brightness_gamma[1]), GAMMA_SET_MAX);


	/* samsung_brightness_gamma[1~21] have to be mtp400cd [0~5][73~87] */
	if (get_auto_brightness() == 6) {
		for (i=1; i<=6 ; i++)
			samsung_brightness_gamma[i] = *(c8_reg_1+(i-1));
		if (mipi_pd.ldi_rev == 'I')
			for (i=7; i<=21 ; i++)
				samsung_brightness_gamma[i] = *(c8_reg_2+(i-7));
	}
#ifdef CONFIG_HBM_PSRE_DEBUG
	/*for debug*/
	printk("%s CA[1~21] : ",__func__);
	for (i=1; i<=21; i++)
		printk("(%x)",samsung_brightness_gamma[i]);
	printk("\n");
#endif

	brightness_packet[cmd_size].payload = samsung_brightness_gamma;
	brightness_packet[cmd_size].dlen = sizeof(samsung_brightness_gamma);
	cmd_size++;
	
	/* gamma update ***********************************************************************/
	/* 0xF7 setting */
	brightness_packet[cmd_size].payload = samsung_gamma_update;
	brightness_packet[cmd_size].dlen = sizeof(samsung_gamma_update);
	cmd_size++;

	mipi_pd.brightness.size = cmd_size;
	mipi_pd.brightness_level = candela;

	mipi_pd.need_update = 0;

	return 1;
}

static int acl_control(int bl_level)
{
	/* acl control *************************************************************************/
	/* 0xB5 setting */
	if (get_auto_brightness() == 6)
		samsung_brightness_acl_cont_default[1] = 0x01;
	else
		samsung_brightness_acl_cont_default[1] = 0x03;

	/* write power saving *****************************************************************/
	/* 0x55 setting */
	if (get_auto_brightness() == 6) {
		samsung_brightness_acl_ref[1] = 0x41; /*ACL on 40p, re low*/
	} else {
		if (mipi_pd.acl_status || mipi_pd.siop_status)
			samsung_brightness_acl_ref[1] = 0x01; /*ACL on 40p*/
		else
			samsung_brightness_acl_ref[1] = 0x00; /*ACL off*/
	}

	return 1;
}

static int cmd_set_change(int cmd_set, int panel_id)
{
	int id1, id2, id3;
	id1 = (panel_id & 0x00FF0000) >> 16;
	id2 = (panel_id & 0x0000FF00) >> 8;
	id3 = panel_id & 0xFF;

	switch (cmd_set) {
	case PANEL_ON:
		if (id1 == 0x00) {
			mipi_pd.on.cmd = samsung_on_cmds_revB;
			mipi_pd.on.size =
				ARRAY_SIZE(samsung_on_cmds_revB);
			mipi_pd.ldi_rev = 'B';
		} else if (id1 == 0x20) {
			mipi_pd.on.cmd = samsung_on_cmds_revB;
			mipi_pd.on.size =
				ARRAY_SIZE(samsung_on_cmds_revB);
			mipi_pd.ldi_rev = 'B';
		} else if (id3 == EVT1_ID || id3 == EVT1_FIRST_ID || id3 == EVT1_SECOND_ID) {
			mipi_pd.on.cmd = samsung_on_cmds_revF;
			mipi_pd.on.size =
				ARRAY_SIZE(samsung_on_cmds_revF);
			mipi_pd.ldi_rev = 'F';
		} else if (id3 == EVT1_THIRD_ID || id3 == EVT1_FOUTRH_ID) {
			mipi_pd.on.cmd = samsung_on_cmds_revG;
			mipi_pd.on.size =
				ARRAY_SIZE(samsung_on_cmds_revG);
			mipi_pd.ldi_rev = 'G';
		} else if (id3 == EVT1_REV_H_ID3_1 || id3 == EVT1_REV_H_ID3_2) {
			samsung_elvss_condition[1] = 0x2C;
			mipi_pd.on.cmd = samsung_on_cmds_revH;
			mipi_pd.on.size =
				ARRAY_SIZE(samsung_on_cmds_revH);
			mipi_pd.ldi_rev = 'H';
		} else if (id3 == EVT1_REV_I_ID3_1 || id3 == EVT1_REV_I_ID3_2) {
			samsung_elvss_condition[1] = 0x2C;
			mipi_pd.on.cmd = samsung_on_cmds_revI;
			mipi_pd.on.size =
				ARRAY_SIZE(samsung_on_cmds_revI);
			mipi_pd.ldi_rev = 'I';
		} else { /*id3 0x02, 0x03*/
			mipi_pd.on.cmd = samsung_on_cmds_revC;
			mipi_pd.on.size =
				ARRAY_SIZE(samsung_on_cmds_revC);
			mipi_pd.ldi_rev = 'C';
		}
		break;
	default:
		;
	}

	pr_info("%s : ldi_rev : %c \n", __func__, mipi_pd.ldi_rev);

	return 0;
}

static void hover_flicker_change(char CB_3rd)
{
	if (CB_3rd == 0x10)
		samsung_scan_timming_1_FE[1] = 0x75;
}

static void reset_bl_level(void)
{
	/* copy current brightness level */
	if (memcmp(samsung_brightness_aor_ref, samsung_brightness_aor_default,
				sizeof(samsung_brightness_aor_ref))) {
		memcpy(samsung_brightness_aor_condition,
				samsung_brightness_aor_ref,
				sizeof(samsung_brightness_aor_condition));
	}

	if (memcmp(samsung_brightness_elvss_ref, samsung_brightness_elvss_default,
				sizeof(samsung_brightness_elvss_ref))) {
		memcpy(samsung_elvss_condition,
					samsung_brightness_elvss_ref,
					sizeof(samsung_elvss_condition));
	}

	/* reset brightness change value */
	memcpy(samsung_brightness_aor_ref, samsung_brightness_aor_default,
					sizeof(samsung_brightness_aor_ref));
	memcpy(samsung_brightness_acl_ref, samsung_brightness_acl_default,
					sizeof(samsung_brightness_acl_ref));
	memcpy(samsung_brightness_elvss_ref, samsung_brightness_elvss_default,
					sizeof(samsung_brightness_elvss_ref));

}

static struct mipi_panel_data mipi_pd = {
	.panel_name	= "SDC_AMS499QP01\n",
	.on		= {samsung_on_cmds_revC
				, ARRAY_SIZE(samsung_on_cmds_revC)},
	.off		= {panel_off_cmds
				, ARRAY_SIZE(panel_off_cmds)},
	.late_on		= {panel_late_on_cmds
				, ARRAY_SIZE(panel_late_on_cmds)},
	.early_off	= {panel_early_off_cmds
				, ARRAY_SIZE(panel_early_off_cmds)},

	.brightness	= {brightness_packet
				, ARRAY_SIZE(brightness_packet)},
	.backlight_control = brightness_control,

	.reset_bl_level = reset_bl_level,

	.mtp_enable	= {panel_mtp_enable_cmds
				, ARRAY_SIZE(panel_mtp_enable_cmds)},
	.mtp_disable	= {panel_mtp_disable_cmds
				, ARRAY_SIZE(panel_mtp_disable_cmds)},
	.need_flip	= {panel_need_flip_cmds
				, ARRAY_SIZE(panel_mtp_disable_cmds)},
	.touch_key	= {panel_touchkey_on
				,ARRAY_SIZE(panel_touchkey_on)},

	.cmd_set_change = cmd_set_change,

	.hover_flicker_change = hover_flicker_change,

	.lux_table = lux_tbl,
	.lux_table_max_cnt = ARRAY_SIZE(lux_tbl),

	.acl_control = acl_control,
	.acl_cmds = {panel_acl_cmds
				, ARRAY_SIZE(panel_acl_cmds)},	
};

static struct mipi_dsi_phy_ctrl dsi_video_mode_phy_db = {
	/* 1920*1200, RGB888, 4 Lane 60 fps video mode */
	/* regulator */
	.regulator = {0x03, 0x0a, 0x04, 0x00, 0x20},
	/* timing */
	.timing = {0xE0, 0x48, 0x25, 0x00, 0x66, 0x6D, 0x2A, 0x4B,
	0x3F, 0x03, 0x04, 0xa0},
	/* phy ctrl */
	.ctrl = {0x5f, 0x00, 0x00, 0x10},
	/* strength */
	.strength = {0xff, 0x00, 0x06, 0x00},
	/* pll control */
	.pll = {0x0, 0x7f, 0x31, 0xda, 0x00, 0x50, 0x48, 0x63,
	0x51, 0x0f, 0x03,
	0x00, 0x14, 0x03, 0x00, 0x02, 0x00, 0x20, 0x00, 0x01 },
};

static int __init mipi_video_samsung_octa_full_hd_pt_init(void)
{
	int ret;

	printk(KERN_DEBUG "[lcd] mipi_video_samsung_octa_full_hd_pt_init start\n");

#ifdef CONFIG_SUPPORT_DISPLAY_OCTA_TFT
	if (check_display_type() != SAMSUNG_OCTA_FULL_HD) {
		pr_err("[lcd] error : this rev(%d) is not for OCTA, return!!\n",
			system_rev);
		return 1;
	}
#endif

	/*
	* LSI 1, MAGNA 0
	*/
	pr_info("get_ldi_chip : %d %s", get_ldi_chip(),
				get_ldi_chip() > 0 ? "LSI" : "MAGNA");

	if (get_ldi_chip() != LDI_LSI)
		return 1;

#ifdef CONFIG_FB_MSM_MIPI_PANEL_DETECT
	if (msm_fb_detect_client("mipi_cmd_samsung_oled_qhd"))
		return 0;
#endif
	pinfo.xres = 1080;
	pinfo.yres = 1920;
	pinfo.height = 111;
	pinfo.width = 62;

	pinfo.type = MIPI_VIDEO_PANEL;
	pinfo.pdest = DISPLAY_1;
	pinfo.wait_cycle = 0;
	pinfo.bpp = 24;

	pinfo.lcdc.h_back_porch = 36;
	pinfo.lcdc.h_front_porch = 159;
	pinfo.lcdc.h_pulse_width = 10;

	pinfo.lcdc.v_back_porch = 4;
	pinfo.lcdc.v_front_porch = 10;
	pinfo.lcdc.v_pulse_width = 2;

	pinfo.lcdc.border_clr = 0;	/* blk */
	pinfo.lcdc.underflow_clr = 0x0;	/* black */
	pinfo.lcdc.hsync_skew = 0;

	pinfo.bl_max = 255;
	pinfo.bl_min = 1;
	pinfo.fb_num = 2;

	pinfo.clk_rate = 898000000;

	pinfo.mipi.mode = DSI_VIDEO_MODE;
	pinfo.mipi.pulse_mode_hsa_he = FALSE;
	pinfo.mipi.hfp_power_stop = TRUE;
	pinfo.mipi.hbp_power_stop = FALSE;
	pinfo.mipi.hsa_power_stop = FALSE;
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
	pinfo.mipi.t_clk_post = 0x04;
	pinfo.mipi.t_clk_pre = 0x1c;
	pinfo.mipi.stream = 0; /* dma_p */
	pinfo.mipi.mdp_trigger = DSI_CMD_TRIGGER_SW;
	pinfo.mipi.dma_trigger = DSI_CMD_TRIGGER_SW;
	pinfo.mipi.frame_rate = 60;
	pinfo.mipi.dsi_phy_db = &dsi_video_mode_phy_db;
	pinfo.mipi.force_clk_lane_hs = 1;
	pinfo.mipi.esc_byte_ratio = 2;

	ret = mipi_samsung_octa_device_register(&pinfo, MIPI_DSI_PRIM,
						MIPI_DSI_PANEL_FULL_HD_PT,
						&mipi_pd);
	if (ret)
		pr_err("%s: failed to register device!\n", __func__);

	printk(KERN_DEBUG "%s: get_lcd_attached(%d)!\n",
				__func__, get_lcd_attached());
	if (get_lcd_attached() == 0)
		return -ENODEV;

	printk(KERN_DEBUG "[lcd] mipi_video_samsung_octa_full_hd_pt_init end\n");

	return ret;
}
module_init(mipi_video_samsung_octa_full_hd_pt_init);
