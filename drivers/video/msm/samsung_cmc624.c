/*
 * Copyright (C) 2011, Samsung Electronics. All rights reserved.
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
*/

#if defined(CONFIG_MACH_MELIUS_EUR_OPEN) || defined(CONFIG_MACH_MELIUS_EUR_LTE)
#undef CMC624_DEBUG
#else
#define CMC624_DEBUG
#endif

#include <linux/delay.h>
#include <linux/gpio.h>
#include <mach/gpio.h>
#include "msm_fb.h"

#ifndef CONFIG_FB_MSM_MIPI_NOVATEK_VIDEO_HD_PT_PANEL
#include <linux/regulator/gpio-regulator.h>
#endif
#include <linux/regulator/consumer.h>
#include <linux/mfd/pm8xxx/pm8038.h>
#include <mach/msm8930-gpio.h>
#include <linux/i2c/samsung_cmc624.h>
#include "mipi_novatek_NT35596.h"

#if defined(CONFIG_FB_MSM_MIPI_NOVATEK_VIDEO_WXGA_PT_PANEL)
#include "samsung_cmc624_tune_konalte.h"
#elif defined(CONFIG_FB_MSM_MIPI_NOVATEK_VIDEO_HD_PT_PANEL)
#include "samsung_cmc624_tune_melius.h"
#else
#include "samsung_cmc624_tune.h"
#endif

/*
	 * V_IMA_1.8V		VREG_L3A
	 * DISPLAY_3.3V		VREG_L8A
	 * V_IMA_1.2V		GPIO LCD_PWR_EN
	 * LCD_VOUT			GPIO LCD_PWR_EN
*/
/*
static struct regulator *v_ima_1_8v;
static struct regulator *display_3_3v;
*/

#define LCD_PWR_EN 70
#define PWM_DUTY_SHARP 86 / 82

#if defined(CONFIG_FB_MSM_MIPI_NOVATEK_VIDEO_HD_PT_PANEL)
#define CABC_CUTOFF_BACKLIGHT_VALUE	40
#endif
struct cmc624_data {
	struct i2c_client *client;
};

static struct cmc624_data *p_cmc624_data;
static struct i2c_client *g_client;
#define I2C_M_WR 0		/* for i2c */
#define I2c_M_RD 1		/* for i2c */
unsigned long last_cmc624_Bank = 0xffff;
unsigned long last_cmc624_Algorithm = 0xffff;
static struct cmc624RegisterSet cmc624_TuneSeq[CMC624_MAX_SETTINGS];
static int cmc624_TuneSeqLen;
#define CMC624_BRIGHTNESS_MAX_LEVEL 1600

// extern bool is_hover_on; //Dibya

struct cmc624_state_type cmc624_state = {
	.cabc_mode = CABC_OFF_MODE,
	.brightness = 157,
	.suspended = 0,
	.power_lut_idx = LUT_LEVEL_MANUAL_AND_INDOOR,
	.scenario = mDNIe_UI_MODE,
	.browser_scenario = COLOR_TONE_1,
	.background = STANDARD_MODE,
	.temperature = TEMP_STANDARD,
	.outdoor = OUTDOOR_OFF_MODE,
	.sub_tune = NULL,
	.main_tune = NULL,
	.negative = NEGATIVE_OFF_MODE,
#if defined(CONFIG_FB_EBOOK_PANEL_SCENARIO)
	.ebook = EBOOK_OFF,
#endif
	.blind = ACCESSIBILITY_OFF,
};

u16 gIDs[3] = { 0, };
boolean video_mode;

static DEFINE_MUTEX(tuning_mutex);
/*
* functions for I2C transactions
*/
unsigned char cmc624_Power_LUT[NUM_POWER_LUT][NUM_ITEM_POWER_LUT] = {
	{0x42, 0x47, 0x3E, 0x52, 0x42, 0x3F, 0x3A, 0x37, 0x3F},
	{0x38, 0x3d, 0x34, 0x48, 0x38, 0x35, 0x30, 0x2d, 0x35},
};

u16 power_lut[LUT_LEVEL_MAX][NUM_POWER_LUT][NUM_ITEM_POWER_LUT] = {
	 /* Indoor power look up table */
	{
		{0x547, 0x4E1, 0x4F5, 0x5C2, 0x547, 0x50A, 0x4A3, 0x466, 0x50A},
		{0x47A, 0x4E1, 0x428, 0x5C2, 0x47A, 0x43D, 0x3D7, 0x399, 0x43D},
	},
	/* Outdoor power look up table for outdoor 1 (1k~15k) */
	{
		{0x547, 0x68F, 0x4F5, 0x68F, 0x547, 0x50A, 0x4A3, 0x466, 0x50A},
		{0x47A, 0x5C2, 0x428, 0x5C2, 0x47A, 0x43D, 0x3D7, 0x399, 0x43D},
	},
	/* Outdoor power look up table (15k ~) */
	{
		{2047, 2047, 2047, 2047, 2047, 2047, 2047, 2047, 2047},
		{2047, 2047, 2047, 2047, 2047, 2047, 2047, 2047, 2047},
	},
};


static int is_cmc624_on;
/*  ###############################################################
 *  #
 *  #       TUNE VALUE
 *  #
 *  ############################################################### */
u16 cmc624_default_plut[NUM_ITEM_POWER_LUT] = {
	0x547, 0x5AD, 0x4F5, 0x68F, 0x547, 0x50A, 0x4A3, 0x466, 0x50A
};

u16 cmc624_video_plut[NUM_ITEM_POWER_LUT] = {
	0x47A, 0x4E1, 0x428, 0x5C2, 0x47A, 0x43D, 0x3D7, 0x399, 0x43D
};

const struct str_sub_tuning sub_tune_value[MAX_TEMP_MODE][MAX_OUTDOOR_MODE] = {
#if 0
	{
	 {
		.value[CABC_OFF_MODE] = {.name =
				"STANDARD, OUTDOOR:OFF, CABC:OFF", .value =
				NULL, .size = 0},
		.value[CABC_ON_MODE] = {.name =
				"STANDARD, OUTDOOR:OFF, CABC:ON", .value =
				NULL, .size = 0}
	  },
	 {
		.value[CABC_OFF_MODE] = {.name =
				"STANDARD, OUTDOOR:ON, CABC:OFF", .value =
				ove_cabcoff, .size = ARRAY_SIZE(ove_cabcoff)},
		.value[CABC_ON_MODE] = {.name =
				"STANDARD, OUTDOOR:ON, CABC:ON", .value =
				ove_cabcoff, .size = ARRAY_SIZE(ove_cabcoff)}
	  }
	 },
	{
	 {
		.value[CABC_OFF_MODE] = {.name =
				"WARM, OUTDOOR:OFF, CABC:OFF", .value =
				warm_cabcoff, .size =
				   ARRAY_SIZE(warm_cabcoff)},
		.value[CABC_ON_MODE] = {.name =
				"WARM, OUTDOOR:OFF,	CABC:ON",
				.value = warm_cabcoff,
				.size = ARRAY_SIZE(warm_cabcoff)}
	  },
	 {
		.value[CABC_OFF_MODE] = {.name =
				"WARM, OUTDOOR:ON, CABC:OFF",
				.value = warm_ove_cabcoff, .size =
				ARRAY_SIZE(warm_ove_cabcoff)},
		.value[CABC_ON_MODE] = {.name =
				"WARM, OUTDOOR:ON, CABC:ON",
				.value = warm_ove_cabcoff, .size =
				ARRAY_SIZE(warm_ove_cabcoff)}
	  }
	 },
	{
	 {
		.value[CABC_OFF_MODE] = {.name =
				"COLD, OUTDOOR:OFF, CABC:OFF", .value =
				cold_cabcoff, .size =
				ARRAY_SIZE(cold_cabcoff)},
		.value[CABC_ON_MODE] = {.name =
				"COLD, OUTDOOR:OFF, CABC:ON", .value =
				cold_cabcoff, .size = ARRAY_SIZE(cold_cabcoff)}
	  },
	 {
		.value[CABC_OFF_MODE] = {.name =
				"COLD, OUTDOOR:ON, CABC:OFF", .value =
				cold_ove_cabcoff,  .size =
				   ARRAY_SIZE(cold_ove_cabcoff)},
		.value[CABC_ON_MODE] = {.name =
				"COLD, OUTDOOR:ON, CABC:ON", .value =
				cold_ove_cabcoff, .size =
				ARRAY_SIZE(cold_ove_cabcoff)}
	  }
	 },
#endif
};

const struct str_main_tuning tune_value[MAX_BACKGROUND_MODE][MAX_mDNIe_MODE] = {
		/* DYNAMIC */
		{{.value[CABC_OFF_MODE] = {
				.name = "DYN_UI_OFF", .flag = 0, .tune =
				dynamic_ui_cabcoff, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(dynamic_ui_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "DYN_UI_ON", .flag = 0, .tune =
				dynamic_ui_cabcon, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(dynamic_ui_cabcon)} },
		{.value[CABC_OFF_MODE] = {
				.name = "DYN_VIDEO_OFF", .flag = 0, .tune =
				dynamic_video_cabcoff, .plut =
				LUT_VIDEO, .size =
				ARRAY_SIZE(dynamic_video_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "DYN_VIDEO_ON",
				.flag = 0, .tune =
				dynamic_video_cabcon, .plut =
				LUT_VIDEO, .size =
				ARRAY_SIZE(dynamic_video_cabcon)} },
		{.value[CABC_OFF_MODE] = {
				.name = "DYN_VIDEO_W_OFF", .flag = 0, .tune =
				dynamic_video_cabcoff, .plut =
				LUT_VIDEO, .size =
				ARRAY_SIZE(dynamic_video_cabcoff)},
		.value[CABC_ON_MODE] = {
				.name = "DYN_VIDEO_W_ON", .flag = 0, .tune =
				dynamic_video_cabcon, .plut =
				LUT_VIDEO, .size =
				ARRAY_SIZE(dynamic_video_cabcon)} },
		{.value[CABC_OFF_MODE] = {
				.name = "DYN_VIDEO_C_OFF", .flag = 0, .tune =
				dynamic_video_cabcoff, .plut =
				LUT_VIDEO, .size =
				ARRAY_SIZE(dynamic_video_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "DYN_VIDEO_C_ON",
				.flag = 0, .tune =
				dynamic_video_cabcon, .plut =
				LUT_VIDEO, .size =
				ARRAY_SIZE(dynamic_video_cabcon)} },
		{.value[CABC_OFF_MODE] = {
				.name = "DYN_CAMERA_OFF", .flag =
				TUNE_FLAG_CABC_ALWAYS_OFF, .tune =
				camera_cabcoff, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(camera_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "DYN_CAMERA_ON",
				.flag =	TUNE_FLAG_CABC_ALWAYS_OFF, .tune =
				camera_cabcoff, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(camera_cabcoff)} },
		{.value[CABC_OFF_MODE] = {
				.name = "DYN_NAVI_OFF", .flag = 0, .tune =
				NULL, .plut = LUT_DEFAULT, .size = 0},
		.value[CABC_ON_MODE] = {.name = "DYN_NAVI_ON",
				.flag = 0, .tune =
				NULL, .plut = LUT_DEFAULT, .size = 0} },
		{.value[CABC_OFF_MODE] = {
				.name = "DYN_GALLERY_OFF", .flag = 0, .tune =
				dynamic_gallery_cabcoff, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(dynamic_gallery_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "DYN_GALLERY_ON",
				.flag = 0, .tune =
				dynamic_gallery_cabcon, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(dynamic_gallery_cabcon)} },
		{.value[CABC_OFF_MODE] = {
				.name = "DYN_VTCALL_OFF", .flag = 0, .tune =
				dynamic_vtcall_cabcoff, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(dynamic_vtcall_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "DYN_VTCALL_ON", .flag = 0,
				.tune =	dynamic_vtcall_cabcon, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(dynamic_vtcall_cabcon)} } ,
		{.value[CABC_OFF_MODE] = {
				.name = "DYN_BROWSER_OFF", .flag = 0, .tune =
				dynamic_browser_cabcoff, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(dynamic_browser_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "DYN_BROWSER_ON", .flag = 0,
				.tune =	dynamic_browser_cabcon, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(dynamic_browser_cabcon)} },
		{.value[CABC_OFF_MODE] = {
				.name = "EBOOK_OFF", .flag = 0, .tune =
				ebook_cabcoff, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(ebook_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "EBOOK_ON", .flag = 0,
				.tune = ebook_cabcon, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(ebook_cabcon)} },
		{.value[CABC_OFF_MODE] = {
				.name = "EMAIL_OFF", .flag = 0, .tune =
				email_cabcoff, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(email_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "EMAIL_ON", .flag = 0,
				.tune = email_cabcon, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(email_cabcon)} },
#if defined(CONFIG_TDMB)
		{.value[CABC_OFF_MODE] = {
				.name = "DYN_DMB_OFF", .flag = 0, .tune =
				dynamic_dmb_cabcoff, .plut = LUT_VIDEO, .size =
				ARRAY_SIZE(dynamic_dmb_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "DYN_DMB_ON",
				.flag = 0, .tune =
				dynamic_dmb_cabcon, .plut = LUT_VIDEO, .size =
				ARRAY_SIZE(dynamic_dmb_cabcon)} }
#endif
		},
		/* STANDARD */
		{{.value[CABC_OFF_MODE] = {
				.name = "STD_UI_OFF", .flag = 0, .tune =
				standard_ui_cabcoff, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(standard_ui_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "STD_UI_ON",
				.flag = 0, .tune =
				standard_ui_cabcon, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(standard_ui_cabcon)} },
		{.value[CABC_OFF_MODE] = {
				.name = "STD_VIDEO_OFF", .flag = 0, .tune =
				standard_video_cabcoff, .plut =
				LUT_VIDEO, .size =
				ARRAY_SIZE(standard_video_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "STD_VIDEO_ON",
				.flag = 0, .tune =
				standard_video_cabcon, .plut =
				LUT_VIDEO, .size =
				ARRAY_SIZE(standard_video_cabcon)} },
		{.value[CABC_OFF_MODE] = {
				.name = "STD_VIDEO_W_OFF", .flag = 0, .tune =
				standard_video_cabcoff, .plut =
				LUT_VIDEO, .size =
				ARRAY_SIZE(standard_video_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "STD_VIDEO_W_ON",
				.flag = 0, .tune =
				standard_video_cabcon, .plut =
				LUT_VIDEO, .size =
				ARRAY_SIZE(standard_video_cabcon)} },
		{.value[CABC_OFF_MODE] = {
				.name = "STD_VIDEO_C_OFF", .flag = 0, .tune =
				standard_video_cabcoff, .plut =
				LUT_VIDEO, .size =
				ARRAY_SIZE(standard_video_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "STD_VIDEO_C_ON",
				.flag = 0, .tune =
				standard_video_cabcon, .plut =
				LUT_VIDEO, .size =
				ARRAY_SIZE(standard_video_cabcon)} },
		{.value[CABC_OFF_MODE] = {
				.name = "STD_CAMERA_OFF", .flag =
				TUNE_FLAG_CABC_ALWAYS_OFF, .tune =
				camera_cabcoff, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(camera_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "STD_CAMERA_ON", .flag =
				TUNE_FLAG_CABC_ALWAYS_OFF, .tune =
				camera_cabcoff, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(camera_cabcoff)} },
		{.value[CABC_OFF_MODE] = {
				.name = "STD_NAVI_OFF", .flag = 0, .tune =
				NULL, .plut = LUT_DEFAULT, .size = 0},
		.value[CABC_ON_MODE] = {.name = "STD_NAVI_ON",
				.flag = 0, .tune =
				NULL, .plut = LUT_DEFAULT, .size = 0} },
		{.value[CABC_OFF_MODE] = {
				.name = "STD_GALLERY_OFF", .flag = 0, .tune =
				standard_gallery_cabcoff, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(standard_gallery_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "STD_GALLERY_ON",
				.flag = 0, .tune =
				standard_gallery_cabcon, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(standard_gallery_cabcon)} },
		{.value[CABC_OFF_MODE] = {
						.name = "STD_VTCALL_OFF", .flag = 0, .tune =
						standard_vtcall_cabcoff, .plut = LUT_DEFAULT, .size =
						ARRAY_SIZE(standard_vtcall_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "STD_VTCALL_ON", .flag = 0,
						.tune = standard_vtcall_cabcon, .plut = LUT_DEFAULT, .size =
						ARRAY_SIZE(standard_vtcall_cabcon)} },
		{.value[CABC_OFF_MODE] = {
				.name = "STD_BROWSER_OFF", .flag = 0, .tune =
				standard_browser_cabcoff, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(standard_browser_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "STD_BROWSER_ON", .flag = 0,
				.tune =	standard_browser_cabcon, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(standard_browser_cabcon)} },
		{.value[CABC_OFF_MODE] = {
				.name = "EBOOK_OFF", .flag = 0, .tune =
				ebook_cabcoff, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(ebook_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "EBOOK_ON", .flag = 0,
				.tune = ebook_cabcon, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(ebook_cabcon)} },
		{.value[CABC_OFF_MODE] = {
				.name = "EMAIL_OFF", .flag = 0, .tune =
				email_cabcoff, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(email_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "EMAIL_ON", .flag = 0,
				.tune = email_cabcon, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(email_cabcon)} },
#if defined(CONFIG_TDMB)
		{.value[CABC_OFF_MODE] = {
				.name = "STD_DMB_OFF", .flag = 0, .tune =
				standard_dmb_cabcoff, .plut = LUT_VIDEO, .size =
				ARRAY_SIZE(standard_dmb_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "STD_DMB_ON", .flag = 0, .tune =
				standard_dmb_cabcon, .plut = LUT_VIDEO, .size =
				ARRAY_SIZE(standard_dmb_cabcoff)} }
#endif
		},
		/* MOVIE */
		{{.value[CABC_OFF_MODE] = {
				.name = "MOV_UI_OFF", .flag = 0, .tune =
				movie_ui_cabcoff, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(movie_ui_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "MOV_UI_ON", .flag = 0, .tune =
				movie_ui_cabcon, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(movie_ui_cabcon)} },
		{.value[CABC_OFF_MODE] = {
				.name = "MOV_VIDEO_OFF", .flag = 0, .tune =
				movie_video_cabcoff, .plut =
				LUT_VIDEO, .size =
				ARRAY_SIZE(movie_video_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "MOV_VIDEO_ON",
				.flag = 0, .tune =
				movie_video_cabcon, .plut =
				LUT_VIDEO, .size =
				ARRAY_SIZE(movie_video_cabcon)} },
		{.value[CABC_OFF_MODE] = {
				.name = "MOV_VIDEO_W_OFF", .flag = 0, .tune =
				movie_video_cabcoff, .plut =
				LUT_VIDEO, .size =
				ARRAY_SIZE(movie_video_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "MOV_VIDEO_W_ON",
				.flag = 0, .tune =
				movie_video_cabcon, .plut =
				LUT_VIDEO, .size =
				ARRAY_SIZE(movie_video_cabcon)} },
		{.value[CABC_OFF_MODE] = {
				.name = "MOV_VIDEO_C_OFF", .flag = 0, .tune =
				movie_video_cabcoff, .plut =
				LUT_VIDEO, .size =
				ARRAY_SIZE(movie_video_cabcoff) },
		.value[CABC_ON_MODE] = {.name = "MOV_VIDEO_C_ON",
				.flag = 0, .tune =
				movie_video_cabcon, .plut =
				LUT_VIDEO, .size =
				ARRAY_SIZE(movie_video_cabcon) } },
		{.value[CABC_OFF_MODE] = {
				.name = "MOV_CAMERA_OFF", .flag =
				TUNE_FLAG_CABC_ALWAYS_OFF, .tune =
				camera_cabcoff, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(camera_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "MOV_CAMERA_ON", .flag =
				TUNE_FLAG_CABC_ALWAYS_OFF, .tune =
				camera_cabcoff, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(camera_cabcoff) } },
		{.value[CABC_OFF_MODE] = {
				.name = "MOV_NAVI_OFF", .flag = 0, .tune =
				NULL, .plut = LUT_DEFAULT, .size = 0},
		.value[CABC_ON_MODE] = {.name = "MOV_NAVI_ON",
				.flag = 0, .tune =
				NULL, .plut = LUT_DEFAULT, .size = 0} },
		{.value[CABC_OFF_MODE] = {
				.name = "MOV_GALLERY_OFF", .flag = 0, .tune =
				movie_gallery_cabcoff, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(movie_gallery_cabcoff) },
		.value[CABC_ON_MODE] = {
				.name = "MOV_GALLERY_ON", .flag = 0, .tune =
				movie_gallery_cabcon, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(movie_gallery_cabcon) } },
		{.value[CABC_OFF_MODE] = {
				.name = "MOV_VTCALL_OFF", .flag = 0, .tune =
				movie_vtcall_cabcoff, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(movie_vtcall_cabcoff) },
		.value[CABC_ON_MODE] = {
				.name = "MOV_VTCALL_ON", .flag = 0, .tune =
				movie_vtcall_cabcon, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(movie_vtcall_cabcon) } },
		{.value[CABC_OFF_MODE] = {
				.name = "MOV_BROWSER_OFF", .flag = 0, .tune =
				movie_browser_cabcoff, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(movie_browser_cabcoff) },
		.value[CABC_ON_MODE] = {
				.name = "MOV_BROWSER_ON", .flag = 0, .tune =
				movie_browser_cabcon, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(movie_browser_cabcon) } },
		{.value[CABC_OFF_MODE] = {
				.name = "EBOOK_OFF", .flag = 0, .tune =
				ebook_cabcoff, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(ebook_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "EBOOK_ON", .flag = 0,
				.tune = ebook_cabcon, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(ebook_cabcon)} },
		{.value[CABC_OFF_MODE] = {
				.name = "EMAIL_OFF", .flag = 0, .tune =
				email_cabcoff, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(email_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "EMAIL_ON", .flag = 0,
				.tune = email_cabcon, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(email_cabcon)} },
#if defined(CONFIG_TDMB)
		{.value[CABC_OFF_MODE] = {
				.name = "MOV_DMB_OFF", .flag = 0, .tune =
				movie_dmb_cabcoff, .plut =
				LUT_VIDEO, .size =
				ARRAY_SIZE(movie_dmb_cabcoff) },
		.value[CABC_ON_MODE] = {
				.name = "MOV_DMB_ON", .flag = 0, .tune =
				movie_dmb_cabcon, .plut =
				LUT_VIDEO, .size =
				ARRAY_SIZE(movie_dmb_cabcon) } }
#endif
		},
		/* AUTO */
		{{.value[CABC_OFF_MODE] = {
				.name = "AUT_UI_OFF", .flag = 0, .tune =
				auto_ui_cabcoff, .plut =
				LUT_DEFAULT , .size = ARRAY_SIZE(auto_ui_cabcoff) },
		.value[CABC_ON_MODE]  = {
				.name = "AUT_UI_ON", .flag = 0, .tune =
				auto_ui_cabcon, .plut =
				LUT_DEFAULT, .size =
				ARRAY_SIZE(auto_ui_cabcon) } },
		{.value[CABC_OFF_MODE] = {
				.name = "AUT_VIDEO_OFF", .flag = 0, .tune =
				auto_video_cabcoff, .plut =
				LUT_VIDEO, .size =
				ARRAY_SIZE(auto_video_cabcoff) },
		.value[CABC_ON_MODE]  = {
				.name = "AUT_VIDEO_ON", .flag = 0, .tune =
				auto_video_cabcon, .plut =
				LUT_VIDEO , .size =
				ARRAY_SIZE(auto_video_cabcon) } },
		{.value[CABC_OFF_MODE] = {
				.name = "AUT_VIDEO_W_OFF", .flag = 0, .tune =
				auto_video_cabcoff, .plut =
				LUT_VIDEO , .size =
				ARRAY_SIZE(auto_video_cabcoff) },
		.value[CABC_ON_MODE]  = {
				.name = "AUT_VIDEO_W_ON", .flag = 0, .tune =
				auto_video_cabcon, .plut =
				LUT_VIDEO , .size =
				ARRAY_SIZE(auto_video_cabcon) } },
		{.value[CABC_OFF_MODE] = {
				.name = "AUT_VIDEO_C_OFF", .flag = 0, .tune =
				auto_video_cabcoff, .plut =
				LUT_VIDEO , .size =
				ARRAY_SIZE(auto_video_cabcoff) },
		.value[CABC_ON_MODE]  = {
				.name = "AUT_VIDEO_C_ON", .flag = 0, .tune =
				auto_video_cabcon, .plut =
				LUT_VIDEO , .size =
				ARRAY_SIZE(auto_video_cabcon) } },
		{.value[CABC_OFF_MODE] = {
				.name = "AUT_CAMERA_OFF", .flag =
				TUNE_FLAG_CABC_ALWAYS_OFF, .tune =
				camera_cabcoff, .plut = LUT_DEFAULT , .size =
				ARRAY_SIZE(camera_cabcoff)},
		.value[CABC_ON_MODE]  = {
				.name = "AUT_CAMERA_ON", .flag =
				TUNE_FLAG_CABC_ALWAYS_OFF, .tune =
				camera_cabcoff, .plut = LUT_DEFAULT , .size =
				ARRAY_SIZE(camera_cabcoff)} },
		{.value[CABC_OFF_MODE] = {
				.name = "AUT_NAVI_OFF", .flag = 0, .tune =
				NULL, .plut = LUT_DEFAULT , .size =
				0},
		.value[CABC_ON_MODE]  = {
				.name = "AUT_NAVI_ON", .flag = 0, .tune =
				NULL, .plut = LUT_DEFAULT , .size =
				0} },
		{.value[CABC_OFF_MODE] = {
				.name = "AUT_GALLERY_OFF", .flag = 0, .tune =
				auto_gallery_cabcoff, .plut = LUT_DEFAULT , .size =
				ARRAY_SIZE(auto_gallery_cabcoff) },
		.value[CABC_ON_MODE]  = {
				.name = "AUT_GALLERY_ON", .flag = 0, .tune =
				auto_gallery_cabcon, .plut = LUT_DEFAULT , .size =
				ARRAY_SIZE(auto_gallery_cabcon) } },
		{.value[CABC_OFF_MODE] = {
				.name = "AUT_VTCALL_OFF", .flag = 0, .tune =
				auto_vtcall_cabcoff, .plut = LUT_DEFAULT , .size =
				ARRAY_SIZE(auto_vtcall_cabcoff) },
		.value[CABC_ON_MODE]  = {
				.name = "AUT_VTCALL_ON", .flag = 0, .tune =
				auto_vtcall_cabcon, .plut = LUT_DEFAULT , .size =
				ARRAY_SIZE(auto_vtcall_cabcon) } },				
		{.value[CABC_OFF_MODE] = {
				.name = "AUT_BROWSER_OFF", .flag = 0, .tune =
				auto_browser_cabcoff, .plut = LUT_DEFAULT , .size =
				ARRAY_SIZE(auto_browser_cabcoff) },
		.value[CABC_ON_MODE]  = {
				.name = "AUT_BROWSER_ON", .flag = 0, .tune =
				auto_browser_cabcon, .plut = LUT_DEFAULT , .size =
				ARRAY_SIZE(auto_browser_cabcon) } },
		{.value[CABC_OFF_MODE] = {
				.name = "EBOOK_OFF", .flag = 0, .tune =
				ebook_cabcoff, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(ebook_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "EBOOK_ON", .flag = 0,
				.tune = ebook_cabcon, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(ebook_cabcon)} },
		{.value[CABC_OFF_MODE] = {
				.name = "EMAIL_OFF", .flag = 0, .tune =
				email_cabcoff, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(email_cabcoff)},
		.value[CABC_ON_MODE] = {.name = "EMAIL_ON", .flag = 0,
				.tune = email_cabcon, .plut = LUT_DEFAULT, .size =
				ARRAY_SIZE(email_cabcon)} },
#if defined(CONFIG_TDMB)
		{.value[CABC_OFF_MODE] = {
				.name = "AUT_DMB_OFF", .flag = 0, .tune =
				auto_dmb_cabcoff, .plut = LUT_VIDEO ,
				.size = ARRAY_SIZE(auto_dmb_cabcoff) },
		.value[CABC_ON_MODE]  = {
				.name = "AUT_DMB_ON", .flag = 0, .tune =
				auto_dmb_cabcon, .plut = LUT_VIDEO ,
				.size = ARRAY_SIZE(auto_dmb_cabcon) } }
#endif
		}
};
const struct str_sub_tuning browser_tune_value[COLOR_TONE_MAX] = {
#if 0
		/* browser tone*/
		{
			.value[CABC_OFF_MODE] = {.name =
			"BROWSER_TONE1,CABC:OFF",
			.value = browser_tone1_tune, .size =
			ARRAY_SIZE(browser_tone1_tune)},
			.value[CABC_ON_MODE] = {.name =
			"BROWSER_TONE1,CABC:ON",
			.value = browser_tone1_tune, .size =
			ARRAY_SIZE(browser_tone1_tune)}
		},
		{
			.value[CABC_OFF_MODE] = {.name =
			"BROWSER_TONE2,CABC:OFF",
			.value = browser_tone2_tune, .size =
			ARRAY_SIZE(browser_tone2_tune)},
			.value[CABC_ON_MODE] = {.name =
			"BROWSER_TONE2,CABC:ON",
			.value = browser_tone2_tune, .size =
			ARRAY_SIZE(browser_tone2_tune)}
		},
		{
			.value[CABC_OFF_MODE] = {.name =
			"BROWSER_TONE3,CABC:OFF",
			.value = browser_tone3_tune, .size =
			ARRAY_SIZE(browser_tone3_tune)},
			.value[CABC_ON_MODE] = {.name =
			"BROWSER_TONE3,CABC:ON",
			.value = browser_tone3_tune, .size =
			ARRAY_SIZE(browser_tone3_tune)}
		},
#endif
};
const struct str_sub_tuning negative_tune_value = {
			.value[CABC_OFF_MODE] = {.name =
			"NEGATIVE_TONE : CABC:OFF",
			.value = negative_cabcoff, .size =
			ARRAY_SIZE(negative_cabcoff)},
			.value[CABC_ON_MODE] = {.name =
			"NEGATIVE_TONE : CABC:ON",
			.value = negative_cabcon, .size =
			ARRAY_SIZE(negative_cabcon)}
};

#if defined(CONFIG_FB_EBOOK_PANEL_SCENARIO)
const struct str_sub_tuning ebook_tune_value = {
	.value[CABC_OFF_MODE] = {.name =
			"EBOOK_OFF : CABC:OFF",
			.value = ebook_cabcoff, .size =
			ARRAY_SIZE(ebook_cabcoff)},
	.value[CABC_ON_MODE] = {.name =
			"EBOOK_ON : CABC:ON",
			.value = ebook_cabcon, .size =
			ARRAY_SIZE(ebook_cabcon)}
};
#endif

struct str_blind_tuning blind_tune_value = {
	.value[CABC_OFF_MODE] = {.name =
			"BLIND_OFF : CABC:OFF",
			.value = cmc624_tune_color_blind_cabcoff, .size =
			ARRAY_SIZE(cmc624_tune_color_blind_cabcoff)},
	.value[CABC_ON_MODE] = {.name =
			"BLIND_ON : CABC:ON",
			.value = cmc624_tune_color_blind_cabcon, .size =
			ARRAY_SIZE(cmc624_tune_color_blind_cabcon)}
};


/*  end of TUNE VALUE
 *  ########################################################## */

bool cmc624_I2cWrite16(unsigned char Addr, unsigned long Data)
{
	int err = -1000;
	struct i2c_msg msg[1];
	unsigned char data[3];

	if (!p_cmc624_data) {
		pr_info("p_cmc624_data is NULL\n");
		return -ENODEV;
	}

	if (!is_cmc624_on) {
		pr_info("cmc624 power down..\n");
		return -ENODEV;
	}
	g_client = p_cmc624_data->client;

	if ((g_client == NULL)) {
		pr_info("cmc624_I2cWrite16 g_client is NULL\n");
		return -ENODEV;
	}

	if (!g_client->adapter) {
		pr_info("cmc624_I2cWrite16 g_client->adapter is NULL\n");
		return -ENODEV;
	}
#if defined(CONFIG_TARGET_LOCALE_KOR_SKT)\
	|| defined(CONFIG_TARGET_LOCALE_KOR_LGU)
	if (Addr == 0x0000) {
		if (Data == last_cmc624_Bank)
			return 0;
		last_cmc624_Bank = Data;
	} else if (Addr == 0x0001 && last_cmc624_Bank == 0) {
		last_cmc624_Algorithm = Data;
	}
#endif

	data[0] = Addr;
	data[1] = ((Data >> 8) & 0xFF);
	data[2] = (Data) & 0xFF;
	msg->addr = g_client->addr;
	msg->flags = I2C_M_WR;
	msg->len = 3;
	msg->buf = data;

	err = i2c_transfer(g_client->adapter, msg, 1);

	if (err >= 0) {
		/*pr_info("%s %d i2c transfer OK\n", __func__, __LINE__); */
		return 0;
	}

	pr_info("%s i2c transfer error:%d(a:%d)\n", __func__, err, Addr);
	return err;
}

int cmc624_I2cRead16(u8 reg, u16 *val)
{
	int err;
	struct i2c_msg msg[2];
	u8 regaddr = reg;
	u8 data[2];

	if (!p_cmc624_data) {
		pr_err("%s p_cmc624_data is NULL\n", __func__);
		return -ENODEV;
	}
	g_client = p_cmc624_data->client;

	if ((g_client == NULL)) {
		pr_err("%s g_client is NULL\n", __func__);
		return -ENODEV;
	}

	if (!g_client->adapter) {
		pr_err("%s g_client->adapter is NULL\n", __func__);
		return -ENODEV;
	}

	if (regaddr == 0x0001) {
		*val = last_cmc624_Algorithm;
		return 0;
	}

	msg[0].addr = g_client->addr;
	msg[0].flags = I2C_M_WR;
	msg[0].len = 1;
	msg[0].buf = &regaddr;
	msg[1].addr = g_client->addr;
	msg[1].flags = I2C_M_RD;
	msg[1].len = 2;
	msg[1].buf = &data[0];
	err = i2c_transfer(g_client->adapter, &msg[0], 2);

	if (err >= 0) {
		*val = (data[0] << 8) | data[1];
		return 0;
	}
	/* add by inter.park */
	pr_err("%s %d i2c transfer error: %d\n", __func__, __LINE__, err);

	return err;
}

/*#####################################################
 *	CMC624 Backlight Control function
  #####################################################
*/

static int cmc624_set_tune_value(const struct cmc624RegisterSet *value,
				 int array_size)
{
	int ret = 0;
	unsigned int num;
	unsigned int i = 0;

	mutex_lock(&tuning_mutex);
	num = array_size;

	for (i = 0; i < num; i++) {
		ret = cmc624_I2cWrite16(value[i].RegAddr, value[i].Data);
//		pr_info("\naddr =%x , data = 0x%04x,",value[i].RegAddr,value[i].Data);
		if (ret != 0) {
			pr_debug
			    ("[CMC624:ERROR]:cmc624_I2cWrite16 failed : %d\n",
			     ret);
			goto set_error;
		}
	}

 set_error:
	mutex_unlock(&tuning_mutex);
	return ret;
}

#if defined(CONFIG_FB_MSM_MIPI_NOVATEK_VIDEO_HD_PT_PANEL)
#define DUTY_DIM 30
#define DUTY_MIN 40
#define DUTY_25  408
#define DUTY_DEFAULT 1093
#define DUTY_MAX 1632
#define PWM_DUTY_MAX 1	
static int value255_old;	
	
	/* Backlight levels */
#define BRIGHTNESS_OFF   0
#define BRIGHTNESS_DIM   20
#define BRIGHTNESS_MIN   25
#define BRIGHTNESS_25   63
#define BRIGHTNESS_DEFAULT  171
#define BRIGHTNESS_MAX   255
/*
 * CMC624 PWM control
 */
static struct cmc624RegisterSet pwm_cabcoff[] = {
	{0x00, 0x0001}, /* BANK 1 */
	{0xF8, 0x0011}, /* PWM HIGH ACTIVE, USE REGISTER VALUE */
	{0xF9, },	/* PWM Counter */

	{0x00, 0x0000}, /* BANK 0 */
	{0xFD, 0xFFFF}, /* MODULE REG MASK RELEASE */
	{0xFE, 0xFFFF}, /* MODULE REG MASK RELEASE */
	{0xFF, 0x0000}, /* MASK RELEASE */
};
static int cmc624_scale_pwm_dutycycle(int level)
{
	int scaled_level = 0;
	if (level == BRIGHTNESS_OFF)
		scaled_level = BRIGHTNESS_OFF;
	else if (level <= BRIGHTNESS_DIM)
		scaled_level = PWM_DUTY_MAX*DUTY_DIM;
	else if (level <= BRIGHTNESS_MIN)
		scaled_level = (level - BRIGHTNESS_DIM) *
			(PWM_DUTY_MAX * DUTY_MIN - PWM_DUTY_MAX * DUTY_DIM) /
			(BRIGHTNESS_MIN - BRIGHTNESS_DIM) +
			PWM_DUTY_MAX * DUTY_DIM;
	else if (level <= BRIGHTNESS_25)
		scaled_level = (level - BRIGHTNESS_MIN) *
			(PWM_DUTY_MAX * DUTY_25 - PWM_DUTY_MAX * DUTY_MIN) /
			(BRIGHTNESS_25 - BRIGHTNESS_MIN) +
			PWM_DUTY_MAX * DUTY_MIN;
	else if (level <= BRIGHTNESS_DEFAULT)
		scaled_level = (level - BRIGHTNESS_25) *
			(PWM_DUTY_MAX * DUTY_DEFAULT - PWM_DUTY_MAX * DUTY_25)
			/ (BRIGHTNESS_DEFAULT - BRIGHTNESS_25) +
			PWM_DUTY_MAX * DUTY_25;
	else if (level <= BRIGHTNESS_MAX)
		scaled_level = (level - BRIGHTNESS_DEFAULT) *
			(PWM_DUTY_MAX * DUTY_MAX - PWM_DUTY_MAX * DUTY_DEFAULT)
			/ (BRIGHTNESS_MAX - BRIGHTNESS_DEFAULT) +
			PWM_DUTY_MAX * DUTY_DEFAULT;
	 pr_debug("%s: level: %d, scaled_level: %d, proc:%s, pid: %d, tgid:%d\n",
		__func__, level, scaled_level, current->comm,
		current->pid, current->tgid);
	return scaled_level;
}

int cmc624_set_pwm_backlight( int level)
{	
	int ret;
	/* set pwm counter value for cabc-off pwm */
	pwm_cabcoff[2].Data = cmc624_scale_pwm_dutycycle(level);
	pr_debug("%s: cabc off: intensity=%d,  pwm cnt=%d\n",
			__func__, level, pwm_cabcoff[2].Data);
	
	ret = cmc624_set_tune_value(pwm_cabcoff, ARRAY_SIZE(pwm_cabcoff));
	return 0;
}
#endif

void cmc624_pwm_control(int value)
{
	printk("%s: value(%d)", __func__, value);
	
	mutex_lock(&tuning_mutex);
	cmc624_I2cWrite16(0x00, 0x0001);
	cmc624_I2cWrite16(0xF8, 0x0011);

	/* Set pwm value regarding LCD panel */
	if(g_lcd_id == LCD_PANEL_SHARP)
        	cmc624_I2cWrite16(0xF9, value * PWM_DUTY_SHARP); /* value = 0~2047 */
	else
        	cmc624_I2cWrite16(0xF9, value); /* value = 0~2047 */

	cmc624_I2cWrite16(0xFF, 0x0000);
	mutex_unlock(&tuning_mutex);
}

void cmc624_pwm_control_cabc(int value255)
{
	const u16 *p_plut;
	int value;
	int idx;

	cmc624_state.brightness = value255;

	if (!is_cmc624_on) {
		pr_debug("cmc624 power down.\n");
		return;
	}

	value = cmc624_scale_pwm_dutycycle(value255);
	value255_old=value;
	pr_err("%s : bg:%d, scen:%d, cabc:%d, bright:%d->%d\n", __func__, cmc624_state.background, cmc624_state.scenario, cmc624_state.cabc_mode,value255,value);

	idx = tune_value[cmc624_state.background][cmc624_state.scenario].value[cmc624_state.cabc_mode].plut;

	/* Change LUT table value when hover on */
//	if( is_hover_on && cmc624_state.cabc_mode == CABC_ON_MODE && cmc624_state.scenario == mDNIe_VIDEO_MODE) //Dibya
	if( cmc624_state.cabc_mode == CABC_ON_MODE && cmc624_state.scenario == mDNIe_VIDEO_MODE)
		idx = LUT_DEFAULT;
	
	p_plut = power_lut[cmc624_state.power_lut_idx][idx];

	if(p_plut == NULL)
		p_plut = cmc624_default_plut;

#if defined(CONFIG_FB_MSM_MIPI_NOVATEK_VIDEO_HD_PT_PANEL)
	if (value255 < CABC_CUTOFF_BACKLIGHT_VALUE) {
		cmc624_pwm_control(value);
		return;
	}
#endif
	/* cmc624 pwm should be controlled by cabc off mode when camera running */
	if(cmc624_state.cabc_mode == CABC_OFF_MODE || cmc624_state.scenario == mDNIe_CAMERA_MODE) {
		cmc624_pwm_control(value);
		return;
	}

	mutex_lock(&tuning_mutex);
	
	cmc624_I2cWrite16(0x00, 0x0001);

	/*PowerLUT*/
    if (g_lcd_id == LCD_PANEL_SHARP) {
        cmc624_I2cWrite16(0xBB, (p_plut[0] * value * PWM_DUTY_SHARP / 2047));
	    cmc624_I2cWrite16(0xBC, (p_plut[1] * value * PWM_DUTY_SHARP / 2047));
	    cmc624_I2cWrite16(0xBD, (p_plut[2] * value * PWM_DUTY_SHARP / 2047));
	    cmc624_I2cWrite16(0xBE, (p_plut[3] * value * PWM_DUTY_SHARP / 2047));
	    cmc624_I2cWrite16(0xBF, (p_plut[4] * value * PWM_DUTY_SHARP / 2047));
	    cmc624_I2cWrite16(0xC0, (p_plut[5] * value * PWM_DUTY_SHARP / 2047));
	    cmc624_I2cWrite16(0xC1, (p_plut[6] * value * PWM_DUTY_SHARP / 2047));
	    cmc624_I2cWrite16(0xC2, (p_plut[7] * value * PWM_DUTY_SHARP / 2047));
	    cmc624_I2cWrite16(0xC3, (p_plut[8] * value * PWM_DUTY_SHARP / 2047));
    } else {
        cmc624_I2cWrite16(0xBB, (p_plut[0] * value / 2047));
	    cmc624_I2cWrite16(0xBC, (p_plut[1] * value / 2047));
	    cmc624_I2cWrite16(0xBD, (p_plut[2] * value / 2047));
	    cmc624_I2cWrite16(0xBE, (p_plut[3] * value / 2047));
	    cmc624_I2cWrite16(0xBF, (p_plut[4] * value / 2047));
	    cmc624_I2cWrite16(0xC0, (p_plut[5] * value / 2047));
	    cmc624_I2cWrite16(0xC1, (p_plut[6] * value / 2047));
	    cmc624_I2cWrite16(0xC2, (p_plut[7] * value / 2047));
	    cmc624_I2cWrite16(0xC3, (p_plut[8] * value / 2047));
    }

	cmc624_I2cWrite16(0xF8, 0x0010);
	cmc624_I2cWrite16(0xFF, 0x0000);

	mutex_unlock(&tuning_mutex);	
}

/* value: 0 ~ 1600*/

int Islcdonoff(void)
{
	/*return 0;*/
	return cmc624_state.suspended;	/* tmps solution*/
}

/*value: 0 ~ 100*/
static void cmc624_manual_pwm_brightness(int value)
{
	pr_debug("[CMC624:INFO]: %s value : %d\n", __func__, value);
	return;
}

/* value: 0 ~ 1600*/
void cmc624_pwm_brightness(int value)
{
	pr_debug("[CMC624:Info] : %s : value : %d\n", __func__, value);
	return;
}

void cmc624_pwm_brightness_bypass(int value)
{
	int data;
	pr_debug("%s : BL brightness level = %d\n", __func__, value);

	if (value < 0)
		data = 0;
	else if (value > 1600)
		data = 1600;
	else
		data = value;

	if (data < 16)
		data = 1;
		/* Range of data 0~1600, min value 0~15 is same as 0 */
	else
		data = data >> 4;

	cmc624_state.brightness = data;
	cmc624_manual_pwm_brightness(data);
}

#define VALUE_FOR_1600	16
void set_backlight_pwm(int level)
{

	mutex_lock(&tuning_mutex);
	if (!Islcdonoff()) {
		if (cmc624_state.main_tune == NULL) {
			/*
			 * struct cmc624_state_type cmc624_state = {
			 * .cabc_mode = CABC_ON_MODE,
			 * .brightness = 42,
			 * .suspended = 0,
			 * .scenario = mDNIe_UI_MODE,
			 * .background = STANDARD_MODE,
			 * .temperature = TEMP_STANDARD,
			 * .outdoor = OUTDOOR_OFF_MODE,
			 * .sub_tune = NULL,
			 * .main_tune = NULL,
			 *};
			 */
			pr_info
			    ("===================================\n");
			pr_info
			    ("[CMC624] cmc624_state.main_tune is NULL?...\n");
			pr_info("[CMC624] DUMP :\n");
			pr_info("[CMC624] cabc_mode : %d\n",
				cmc624_state.cabc_mode);
			pr_info("[CMC624] brightness : %d\n",
				cmc624_state.brightness);
			pr_info("[CMC624] suspended : %d\n",
				cmc624_state.suspended);
			pr_info("[CMC624] scenario : %d\n",
				cmc624_state.scenario);
			pr_info("[CMC624] background : %d\n",
				cmc624_state.background);
			pr_info("[CMC624] temperature : %d\n",
				cmc624_state.temperature);
			pr_info("[CMC624] outdoor : %d\n",
				cmc624_state.outdoor);
			pr_info
			    ("===================================\n");
			mutex_unlock(&tuning_mutex);
			return;
		}
		if (cmc624_state.cabc_mode == CABC_OFF_MODE ||
		    (cmc624_state.main_tune->flag & TUNE_FLAG_CABC_ALWAYS_OFF))
			cmc624_manual_pwm_brightness(level);
		else
			cmc624_pwm_brightness(level * VALUE_FOR_1600);
	}
	mutex_unlock(&tuning_mutex);
}

/*#####################################################
 *	CMC624 common function
 *
 * void bypass_onoff_ctrl(int value);
 * void cabc_onoff_ctrl(int value);
 * int set_mdnie_scenario_mode(unsigned int mode);
 * int load_tuning_data(char *filename);
 * int apply_main_tune_value(enum eLcd_mDNIe_UI ui,
 * enum eBackground_Mode bg, enum eCabc_Mode cabc, int force);
 * int apply_sub_tune_value(enum eCurrent_Temp temp,
 * enum eOutdoor_Mode ove, enum eCabc_Mode cabc, int force);
  #####################################################
*/

void bypass_onoff_ctrl(int value)
{
	pr_debug("[CMC624:Info] : %s : value : %d\n", __func__, value);
	return;
}

void cabc_onoff_ctrl(int value)
{
	int _cabc_on = 0;
	pr_err("[CMC624]:%s (%d)\n", __func__, value);

	if (value)
		_cabc_on = 1;
		
	if (value >= 5)
		cmc624_state.power_lut_idx = LUT_LEVEL_OUTDOOR_2;
	else
		cmc624_state.power_lut_idx = LUT_LEVEL_OUTDOOR_1;


	if (cmc624_state.negative == NEGATIVE) {
		apply_negative_tune_value(cmc624_state.negative, _cabc_on);
	} else if (cmc624_state.blind == COLOR_BLIND) {
		apply_blind_tune_value(COLOR_BLIND, _cabc_on);
	} else {
		apply_main_tune_value(cmc624_state.scenario,\
			cmc624_state.background,\
			_cabc_on, 0);
	}

	return;
}

static struct cmc624RegisterSet cmc624_TuneSeq[CMC624_MAX_SETTINGS];
static int parse_text(char *src, int len)
{
	int i, count, ret;
	int index = 0;
	char *str_line[CMC624_MAX_SETTINGS];
	char *sstart;
	char *c;
	unsigned int data1, data2;

	c = src;
	count = 0;
	sstart = c;

	for (i = 0; i < len; i++, c++) {
		char a = *c;
		if (a == '\r' || a == '\n') {
			if (c > sstart) {
				str_line[count] = sstart;
				count++;
			}
			*c = '\0';
			sstart = c + 1;
		}
	}

	if (c > sstart) {
		str_line[count] = sstart;
		count++;
	}

	for (i = 0; i < count; i++) {
		ret = sscanf(str_line[i], "0x%x,0x%x\n", &data1, &data2);
		pr_debug("Result => [0x%2x 0x%4x] %s\n", data1, data2,
			 (ret == 2) ? "Ok" : "Not available");
		if (ret == 2) {
			cmc624_TuneSeq[index].RegAddr = (unsigned char)data1;
			cmc624_TuneSeq[index++].Data = (unsigned long)data2;
		}
	}
	return index;
}

bool cmc624_tune(unsigned long num)
{
	unsigned int i;

	pr_debug("[CMC624:INFO] Start tuning CMC624\n");

	for (i = 0; i < num; i++) {
		pr_debug("[CMC624:Tuning][%2d] : reg : 0x%2x, data: 0x%4x\n",
			 i + 1, cmc624_TuneSeq[i].RegAddr,
			 cmc624_TuneSeq[i].Data);

		if (cmc624_I2cWrite16
		    (cmc624_TuneSeq[i].RegAddr, cmc624_TuneSeq[i].Data) != 0) {
			pr_err("[CMC624:ERROR] : I2CWrite failed\n");
			return 0;
		}
		pr_debug("[CMC624:Tunig] : Write Done\n");

		if (cmc624_TuneSeq[i].RegAddr == CMC624_REG_SWRESET
		    && cmc624_TuneSeq[i].Data == 0xffff) {
			mdelay(3);
		}
	}
	pr_debug("[CMC624:INFO] End tuning CMC624\n");
	return 1;
}

int load_tuning_data(char *filename)
{
	struct file *filp;
	char *dp;
	long l;
	loff_t pos;
	int ret;
	mm_segment_t fs;

	pr_debug("[CMC624:INFO]:%s called loading file name : %s\n", __func__,
		 filename);
	cmc624_TuneSeqLen = 0;

	fs = get_fs();
	set_fs(get_ds());

	filp = filp_open(filename, O_RDONLY, 0);
	if (IS_ERR(filp)) {
		pr_debug("[CMC624:ERROR]:File open failed\n");
		return -1;
	}

	l = filp->f_path.dentry->d_inode->i_size;
	pr_debug("[CMC624:INFO]: Loading File Size : %ld(bytes)", l);

	dp = kmalloc(l + 10, GFP_KERNEL);
	if (dp == NULL) {
		pr_debug(
			"[CMC624:ERROR]:Can't not alloc memory for tuning file load\n");
		filp_close(filp, current->files);
		return -1;
	}
	pos = 0;
	memset(dp, 0, l);
	pr_debug("[CMC624:INFO] : before vfs_read()\n");
	ret = vfs_read(filp, (char __user *)dp, l, &pos);
	pr_debug("[CMC624:INFO] : after vfs_read()\n");

	if (ret != l) {
		pr_debug("[CMC624:ERROR] : vfs_read() filed ret : %d\n", ret);
		kfree(dp);
		filp_close(filp, current->files);
		return -1;
	}

	filp_close(filp, current->files);

	set_fs(fs);
	cmc624_TuneSeqLen = parse_text(dp, l);

	if (!cmc624_TuneSeqLen) {
		pr_debug("[CMC624:ERROR]:Nothing to parse\n");
		kfree(dp);
		return -1;
	}

	pr_debug("[CMC624:INFO] : Loading Tuning Value's Count : %d",
		cmc624_TuneSeqLen);
	cmc624_tune(cmc624_TuneSeqLen);

	kfree(dp);
	return cmc624_TuneSeqLen;
}
#define SUB_TUNE	0
#define MAIN_TUNE	1
#define BROW_TUNE	2
int init_tune_flag[3] = {0,};

int apply_sub_tune_value(enum eCurrent_Temp temp, enum eOutdoor_Mode outdoor,
			 enum eCabc_Mode cabc, int force)
{

	pr_err("[CMC624:ERROR]:%s: is not used.\n", __func__);
	return 0;	//	skip this function temporary.
	
#if 0	
	int register_size;

	if (cmc624_state.negative == 1) {
		cmc624_state.temperature = temp;
		cmc624_state.outdoor = outdoor;
		return 0;
	}

/*
    if(tuning_enable){
		pr_debug("[CMC624:INFO]:%s:Tuning mode Enabled\n", __func__);
		return 0;
    }
*/

	if (force == 0) {
		if ((cmc624_state.temperature == temp)
		    && (cmc624_state.outdoor == outdoor)) {
			pr_debug(
			"[CMC624:INFO]:%s:already setted temp : %d, over : %d\n"
			, __func__, temp, outdoor);
			return 1;
		}
	}
	pr_debug("=================================================\n");
	pr_debug(" CMC624 Mode Change.sub tune\n");
	pr_debug("==================================================\n");
	pr_debug("[CMC624:INFO]:%s:curr sub tune : %s\n", __func__,
		sub_tune_value[cmc624_state.temperature][cmc624_state.outdoor].
		value[cmc624_state.cabc_mode].name);
	pr_debug("[CMC624:INFO]:%s: sub tune : %s\n", __func__,
		sub_tune_value[temp][outdoor].value[cabc].name);

	if ((outdoor == OUTDOOR_OFF_MODE) || (temp == TEMP_STANDARD)) {
		pr_debug("[CMC624:INFO]:%s:set default main tune\n", __func__);
			register_size =
			tune_value[cmc624_state.background]
			[cmc624_state.scenario].
			value[cmc624_state.cabc_mode].size;
		if (cmc624_set_tune_value(tune_value[cmc624_state.background]
					[cmc624_state.scenario].
					value[cmc624_state.cabc_mode].tune,
					register_size) != 0) {
			pr_debug("[CMC624:ERROR]:%s: set tune value falied\n",\
				__func__);
			return -1;
		}

		if ((outdoor != OUTDOOR_OFF_MODE) || (temp != TEMP_STANDARD))
			goto set_sub_tune;
	} else {
set_sub_tune:
		register_size = sub_tune_value[temp][outdoor].value[cabc].size;
		if (cmc624_set_tune_value
			(sub_tune_value[temp][outdoor].value[cabc].value,
			register_size)) {
			pr_debug("[CMC624:ERROR]:%s: set tune value falied\n",\
				 __func__);
			return -1;
		}
	}
	cmc624_state.temperature = temp;
	cmc624_state.outdoor = outdoor;
	return 0;
#endif
}

void cmc_timing_generator_reset(void)
{
	mutex_lock(&tuning_mutex);
	/* Switch off cmc timing generator */
	cmc624_I2cWrite16(0x00, 0x0003);
	cmc624_I2cWrite16(0x80, 0x0000);
	cmc624_I2cWrite16(0x00, 0x0002);
	cmc624_I2cWrite16(0x52, 0x0000);

	/* Switch on cmc timing generator */
	cmc624_I2cWrite16(0x00, 0x0003);
	cmc624_I2cWrite16(0x80, 0x0001);
	cmc624_I2cWrite16(0x00, 0x0002);
	cmc624_I2cWrite16(0x52, 0x0001);
	mutex_unlock(&tuning_mutex);
}

int apply_main_tune_value(enum eLcd_mDNIe_UI ui, enum eBackground_Mode bg,
			  enum eCabc_Mode cabc, int force)
{
	enum eCurrent_Temp temp = 0;
	if (cmc624_state.negative == NEGATIVE || cmc624_state.blind == COLOR_BLIND) {
		cmc624_state.scenario = ui;
		cmc624_state.background = bg;
		cmc624_state.cabc_mode = cabc;
		return 0;
	}

	pr_debug("==================================================\n");
	pr_debug(" CMC624 Mode Change. Main tune\n");
	pr_debug("==================================================\n");

	if (force == 0) {
		if ((cmc624_state.scenario == ui)
			&& (cmc624_state.background == bg)
			&& (cmc624_state.cabc_mode == cabc)) {
			pr_debug("[CMC624:INFO]:%s:already setted ui : %d, bg : %d, cabc : %d\n",
							__func__, ui, bg, cabc);
			return 0;
		}
	}

	pr_debug("[CMC624:INFO]:%s:curr main tune : %s\n", __func__,
		tune_value[cmc624_state.background][cmc624_state.scenario].
		value[cmc624_state.cabc_mode].name);
	pr_debug("[CMC624:INFO]:%s: main tune : %s\n", __func__,
		tune_value[bg][ui].value[cabc].name);

	if (ui == mDNIe_VIDEO_MODE ||
		ui == mDNIe_VIDEO_WARM_MODE ||
		ui == mDNIe_VIDEO_COLD_MODE)
		video_mode = true;
	else
		video_mode = false;

	pr_debug("[CMC624:INFO]:%s set, size : %d\n",\
		tune_value[bg][ui].value[cabc].name,\
		tune_value[bg][ui].value[cabc].size);

	if (cmc624_set_tune_value
		(tune_value[bg][ui].value[cabc].tune,
		tune_value[bg][ui].value[cabc].size) != 0) {
		pr_err("[CMC624:ERROR]:%s: set tune value falied\n", __func__);
		goto rest_set;
	}

	if ((ui == mDNIe_VIDEO_WARM_MODE) || (ui == mDNIe_VIDEO_COLD_MODE)) {
		if (ui == mDNIe_VIDEO_WARM_MODE)
			temp = TEMP_WARM;
		else if (ui == mDNIe_VIDEO_COLD_MODE)
			temp = TEMP_COLD;
		if (apply_sub_tune_value(temp, cmc624_state.outdoor, cabc, 0)
			!= 0) {
			pr_err("[CMC624:ERROR]:%s:apply_sub_tune_value() faield\n", __func__);
			goto rest_set;
		}
	}

rest_set:

	cmc624_state.scenario = ui;
	cmc624_state.background = bg;
	cmc624_state.cabc_mode = cabc;
	cmc624_state.main_tune = &tune_value[bg][ui].value[cabc];

	if (ui == mDNIe_UI_MODE) {
		cmc624_state.temperature = TEMP_STANDARD;
		cmc624_state.outdoor = OUTDOOR_OFF_MODE;
	}

	if (ui == mDNIe_VIDEO_WARM_MODE)
		cmc624_state.temperature = TEMP_WARM;
	else if (ui == mDNIe_VIDEO_COLD_MODE)
		cmc624_state.temperature = TEMP_COLD;

	cmc624_pwm_control_cabc(cmc624_state.brightness);

	return 0;
}

int apply_browser_tune_value(enum SCENARIO_COLOR_TONE browser_mode, int force)
{	
	pr_err("[CMC624:ERROR]:%s: is not used.\n", __func__);
	return 0;	//	skip this function temporary.
	
#if 0	
	browser_mode -= COLOR_TONE_1;

	if (cmc624_state.negative == 1) {
		cmc624_state.browser_scenario = browser_mode;
		cmc624_state.scenario = mDNIe_BROWSER_ON_MODE;
		return 0;
	}
	pr_debug("==================================================\n");
	pr_debug(" CMC624 Mode Change. brower tune\n");
	pr_debug("==================================================\n");

	if (!init_tune_flag[BROW_TUNE])
		init_tune_flag[MAIN_TUNE] = 0; init_tune_flag[SUB_TUNE] = 0;\
		init_tune_flag[BROW_TUNE] = 1; force = 1;

	if ((force == 0) && (cmc624_state.browser_scenario == browser_mode)) {
			pr_debug(
			"[CMC624:INFO]:%s:already setted browser tone :	%d\n",
			__func__ , browser_mode);
			return 0;
	}
	pr_debug("[CMC624:INFO]:%s curr browser tune : %s\n", __func__,
		browser_tune_value[cmc624_state.browser_scenario].
		value[cmc624_state.cabc_mode].name);
	pr_debug("[CMC624:INFO]:%s browser tune : %s\n", __func__,
		browser_tune_value[browser_mode].
		value[cmc624_state.cabc_mode].name);
	pr_debug("[CMC624:INFO]:%s set, size : %d\n",\
		browser_tune_value[browser_mode].
		value[cmc624_state.cabc_mode].name,
		browser_tune_value[browser_mode].\
		value[cmc624_state.cabc_mode].size);
	if (cmc624_set_tune_value(browser_tune_value[browser_mode].\
		value[cmc624_state.cabc_mode].value,\
		browser_tune_value[browser_mode].\
		value[cmc624_state.cabc_mode].size) != 0) {
		pr_err("[CMC624:ERROR]:%s: set tune value falied\n", __func__);
		return -1;
	}
	cmc624_state.browser_scenario = browser_mode;
	cmc624_state.scenario = mDNIe_BROWSER_ON_MODE;
	return 0;
#endif
}
int apply_negative_tune_value(
	enum ACCESSIBILITY negative_mode, enum eCabc_Mode cabc) {
	int register_size;

	pr_debug("==================================================\n");
	pr_debug(" CMC624 Negative Change.\n");
	pr_debug("==================================================\n");

	if (negative_mode == 0) {
		cmc624_state.negative = negative_mode;
		apply_main_tune_value(cmc624_state.scenario,
			cmc624_state.background, cmc624_state.cabc_mode, 1);
		pr_info("[CMC624:INFO]:%s:negative setted disalbe : %d\n",
			__func__, negative_mode);
		return 0;
	}
	register_size = negative_tune_value.value[cabc].size;
	if (cmc624_set_tune_value(
		negative_tune_value.value[cabc].value, register_size)
		!= 0) {
		pr_err("[CMC624:ERROR]:%s:set tune value falied\n", __func__);
		return -1;
	}
	cmc624_state.negative = negative_mode;
	cmc624_state.cabc_mode = cabc;

	cmc624_pwm_control_cabc(cmc624_state.brightness);
	return 0;
}

#if defined(CONFIG_FB_EBOOK_PANEL_SCENARIO)
int apply_ebook_tune_value(
	enum eEbook_Mode ebook_mode, enum eCabc_Mode cabc) {
	int register_size;

	pr_debug("==================================================\n");
	pr_debug(" CMC624 Ebook Change.\n");
	pr_debug("==================================================\n");

	if (ebook_mode == 0) {
		cmc624_state.ebook = ebook_mode;
		apply_main_tune_value(cmc624_state.scenario,
			cmc624_state.background, cmc624_state.cabc_mode, 1);
		pr_info("[CMC624:INFO]:%s:ebook setted disabled: %d\n",
			__func__, ebook_mode);
		return 0;
	}
	register_size = ebook_tune_value.value[cabc].size;
	if (cmc624_set_tune_value(
		ebook_tune_value.value[cabc].value, register_size)
		!= 0) {
		pr_err("[CMC624:ERROR]:%s:set tune value falied\n", __func__);
		return -1;
	}
	cmc624_state.ebook = ebook_mode;
	cmc624_state.cabc_mode = cabc;

	cmc624_pwm_control_cabc(cmc624_state.brightness);
	return 0;
}

#endif

int apply_blind_tune_value(enum ACCESSIBILITY blind_mode, enum eCabc_Mode cabc)
{
	int register_size;

	pr_debug("==================================================\n");
	pr_debug(" CMC624 Blind Change.\n");
	pr_debug("==================================================\n");


	if (blind_mode == 0) {
		cmc624_state.blind = blind_mode;
		apply_main_tune_value(cmc624_state.scenario,
			cmc624_state.background, cmc624_state.cabc_mode, 1);
		pr_info("[CMC624:INFO]:%s:blind setted disabled: %d\n",
			__func__, blind_mode);
		return 0;
	}

	register_size = blind_tune_value.value[cabc].size;
	if (cmc624_set_tune_value(
		blind_tune_value.value[cabc].value, register_size)
		!= 0) {
		pr_err("[CMC624:ERROR]  :%s:set tune value falied\n", __func__);
		return -1;
	}
	cmc624_state.blind = blind_mode;
	cmc624_state.cabc_mode = cabc;

	cmc624_pwm_control_cabc(cmc624_state.brightness);
	return 0;
}


int samsung_test_tune_dmb(void)
{
	pr_err("[CMC624:ERROR]:%s: is not used.\n", __func__);
	return 0;	//	skip this function temporary.

#if 0
	int i = 0;

	pr_info("[%s]\n", __func__);
	for (i = 0; i < ARRAY_SIZE(cmc624_tune_dmb_test); i++) {
		if (cmc624_I2cWrite16
		    (cmc624_tune_dmb_test[i].RegAddr,
		     cmc624_tune_dmb_test[i].Data)) {
			pr_err("why return false??!!!\n");
		}
	}
#endif
}

#ifdef CMC624_DEBUG
#define LDI_MTP_ADDR 0xd3
#define LDI_ID_ADDR 0xd1
u8 mtp_read_data[24];
u8 id_buffer[8];
unsigned char LCD_ID;
unsigned char LCD_ELVSS_ID3;
static void read_ldi_reg(unsigned long src_add, unsigned char *read_data,
			 unsigned char read_length)
{
	int i;
	u16 data = 0;
	/* LP Operation2 (Read IDs) */
	/* cmc624_I2cWrite16( 0x00, 0x0003 ); */
	cmc624_I2cWrite16(0xaa, read_length);

	cmc624_I2cWrite16(0x02, 0x1100);
	/* GENERIC WRITE 1 PARA */
	cmc624_I2cWrite16(0x04, 0x00b0);
	/* WRITE B0 */
	cmc624_I2cWrite16(0x02, 0x1501);
	/* GENERIC READ 1 PARA */
	cmc624_I2cWrite16(0x04, src_add);
	/* WRITE d1/ d3 (Address of MTP) */

	if (src_add == LDI_ID_ADDR)
		pr_info("Read IDs from D1h\n");
	else
		pr_info("READ Bright Gamma Offset from MTP(D3h)\n");

	for (i = 0; i < read_length; i++) {
		cmc624_I2cRead16(0x08, &data);	/* Read Data Valid Register */

		if ((data & 0x1000) != 0x1000)
			pr_info("[%d] invalid data %x!!!\n", i, data);
			/* Data Valid Check 0x1000? */
		else
			pr_info("[%d] 0x08 data %x!!!\n", i, data);
			/* Data Valid Check 0x1000? */
		cmc624_I2cRead16(0x06, &data);
		/* Read Nst Parameter */
		read_data[i] = data;	/*Store Nst Parameter */
		pr_info("[%d] IDs = %x\n", i, read_data[i]);

	}
	if (read_length == 8) {
		LCD_ID = read_data[1];
		LCD_ELVSS_ID3 = read_data[2];
	}

	cmc624_I2cRead16(0x08, &data);
	/* Read Data Valid Register */
	if ((data & 0x1000) != 0x1000)
		pr_info("End of read IDs %x!!!\n", data);
		/* Data Valid Check 0x1000? */
	else
		pr_info("0x08 data %x!!!\n", data);
		/* Data Valid Check 0x1000? */
}

unsigned char LCD_Get_Value(void){
	return LCD_ID;
}

bool Is_4_8LCD_cmc(void){
	if ((LCD_ID == 0x20) || (LCD_ID == 0x40) || (LCD_ID == 0x60))
		return true;
	else
		return false;
}

bool Is_4_65LCD_cmc(void){
	if (LCD_ID == 0xAE)
		return true;
	else
		return false;
}

unsigned char LCD_ID3(void){
	return LCD_ELVSS_ID3;
}
#endif

/* ################# END of SYSFS Function ############################# */

static bool CMC624_SetUp(void)
{
#ifdef CMC624_DEBUG
	u16 data = 0;
#endif
	int i = 0;
	static int bInit=0, bRead;

	mutex_lock(&tuning_mutex);
	if (!bInit) {

		for (i = 0; i < ARRAY_SIZE(cmc624_init); i++) {
#if defined(CONFIG_FB_MSM_MIPI_NOVATEK_VIDEO_HD_PT_PANEL)
				if(bRead){
				if ((cmc624_init[i].RegAddr == 0xF9) 
					&& (cmc624_init[i].Data == 0x0266) ){
					cmc624_init[i].Data=value255_old;
				}
			}
#endif
			if (cmc624_I2cWrite16
			    (cmc624_init[i].RegAddr, cmc624_init[i].Data)) {
				pr_err("why return false??!!!\n");
				mutex_unlock(&tuning_mutex);
				return FALSE;
			}
			if (cmc624_init[i].RegAddr == 0x3E
			    && cmc624_init[i].Data == 0x2223) {
				mdelay(1);
			}
		}
		
#ifdef CMC624_DEBUG
		mdelay(5);
		if (!bRead) {
			bRead = 1;
			pr_info("CMC Init Sequence 2 Start....\n");
			read_ldi_reg(LDI_ID_ADDR, id_buffer, 8);
			read_ldi_reg(LDI_MTP_ADDR, mtp_read_data, 24);
		}
		cmc624_I2cRead16(0x00, &data);
		pr_info("0x00 data %x!!!\n", data);
		cmc624_I2cRead16(0x82, &data);
		pr_info("0x82 data %x!!!\n", data);
		cmc624_I2cRead16(0x83, &data);
		pr_info("0x83 data %x!!!\n", data);
		cmc624_I2cRead16(0xC2, &data);
		pr_info("0xC2 data %x!!!\n", data);
		cmc624_I2cRead16(0xC3, &data);
		pr_info("0xC3 data %x!!!\n", data);
#endif
		/*CONV */
		cmc624_I2cWrite16(0x00, 0x0003);	/* BANK 3 */
		cmc624_I2cWrite16(0x01, 0x0001);	/* MIPI TO MIPI */

		cmc624_I2cWrite16(0x00, 0x0002);	/* BANK 2 */
		cmc624_I2cWrite16(0x52, 0x0001);	/* RGB IF ENABLE */

		cmc624_I2cWrite16(0x00, 0x0003);	/* BANK 3 goooooood */

	} else {

		pr_info("cmc624_init3!!!\n");

		for (i = 0; i < ARRAY_SIZE(cmc624_wakeup); i++) {
			if (cmc624_I2cWrite16
			    (cmc624_wakeup[i].RegAddr, cmc624_wakeup[i].Data)) {
				mutex_unlock(&tuning_mutex);
				pr_err("why return false??!!!\n");
				return FALSE;
			}
		}
	}
/*	samsung_test_tune_dmb();   */
	mutex_unlock(&tuning_mutex);

	return TRUE;
}

void samsung_get_id(unsigned char *buf)
{
	buf[0] = (unsigned char)gIDs[0] & 0xff;
	buf[1] = (unsigned char)gIDs[1] & 0xff;
	buf[2] = (unsigned char)gIDs[2] & 0xff;

	pr_info("[%s] IDs = %x%x%x\n", __func__, buf[0], buf[1], buf[2]);
}

void Check_Prog(void)
{
	u16 bank = 0;
	u16 data = 0;

	pr_info("++++++%s++++\n", __func__);
	mutex_lock(&tuning_mutex);
	cmc624_I2cWrite16(0x00, 0x0003);	/* BANK 3 */

	cmc624_I2cRead16(0x00, &data);	/* Read Nst Parameter */
	pr_info("0x00 data %x!!!\n", data);
	cmc624_I2cRead16(0x82, &data);	/* Read Nst Parameter */
	pr_info("0x82 data %x!!!\n", data);
	cmc624_I2cRead16(0x83, &data);	/* Read Nst Parameter */
	pr_info("0x83 data %x!!!\n", data);
	cmc624_I2cRead16(0xC2, &data);	/* Read Nst Parameter */
	pr_info("0xC2 data %x!!!\n", data);
	cmc624_I2cRead16(0xC3, &data);	/* Read Nst Parameter */
	pr_info("0xC3 data %x!!!\n", data);

	cmc624_I2cRead16(0x01, &data);
	cmc624_I2cRead16(0x00, &bank);
	pr_info("[bank %d] 0x01 data %x!!!\n", bank, data);

	cmc624_I2cWrite16(0x00, 0x0002);	/* BANK 2 */
	cmc624_I2cRead16(0x52, &data);
	cmc624_I2cRead16(0x00, &bank);
	pr_info("[bank %d] 0x52 data %x!!!\n", bank, data);

	cmc624_I2cWrite16(0x00, 0x0005);	/* BANK 5 */
	cmc624_I2cRead16(0x0B, &data);
	cmc624_I2cRead16(0x00, &bank);
	pr_info("[bank %d] 0x0B data %x!!!\n", bank, data);

	mutex_unlock(&tuning_mutex);
	pr_info("-----%s-----\n", __func__);
}

void change_mon_clk(void)
{
	pr_info("++++++%s++++\n", __func__);
	cmc624_I2cWrite16(0x00, 0x0002);	/* BANK 2 */
	cmc624_I2cWrite16(0x3F, 0x0107);
	pr_info("-----%s-----\n", __func__);
}

int samsung_cmc624_on(int enable)
{

	pr_info("[LCD] %s:enable:%d\n", __func__, enable);

	if (enable) {

		pr_info("CMC Init Sequence!\n");
		is_cmc624_on = 1;
		
		if(!CMC624_SetUp())
			return FALSE;
		
		cmc624_state.suspended = 0;
		if (cmc624_state.negative == NEGATIVE) {
			apply_negative_tune_value(cmc624_state.negative,\
			cmc624_state.cabc_mode);
		} else if (cmc624_state.blind == COLOR_BLIND) {
			apply_blind_tune_value(COLOR_BLIND, cmc624_state.cabc_mode);
		} else {
			apply_main_tune_value(cmc624_state.scenario,\
			cmc624_state.background,\
			cmc624_state.cabc_mode, 1);
		}

		msleep(10);
#ifdef TIMER_DEBUG
		timer_init();
#endif

	} else {
		is_cmc624_on = 0;
		cmc624_state.suspended = 1;
	}

	return TRUE;

}

int samsung_cmc624_bypass_mode(void)
{
	int ret = 0;
	gpio_set_value(IMA_SLEEP, 1);
//	ret = gpio_get_value(IMA_SLEEP);
	pr_debug("%s, IMA_SLEEP : %d\n", __func__, ret);
	udelay(20);

	gpio_set_value(IMA_nRST, 0);
//	ret = gpio_get_value(IMA_nRST);
	pr_debug("%s, IMA_nRST : %d\n", __func__, ret);
	msleep(4);
	gpio_set_value(IMA_nRST, 1);
//	ret = gpio_get_value(IMA_nRST);
	pr_debug("%s, IMA_nRST : %d\n", __func__, ret);

	return ret;

}

int samsung_cmc624_normal_mode(void)
{
	int ret = 0;

	gpio_set_value(IMA_SLEEP, 1);
//	ret = gpio_get_value(IMA_SLEEP);
	pr_debug("%s, IMA_SLEEP : %d\n", __func__, ret);
	udelay(20);

	gpio_set_value(IMA_nRST, 0);
//	ret = gpio_get_value(IMA_nRST);
	pr_debug("%s, IMA_nRST : %d\n", __func__, ret);
	msleep(4);
	gpio_set_value(IMA_nRST, 1);
//	ret = gpio_get_value(IMA_nRST);
	pr_debug("%s, IMA_nRST : %d\n", __func__, ret);

	return ret;

}

/*  Platform Range : 0 ~ 255
 *  CMC624   Range : 0 ~ 100
 *  User Platform Range :
 *   - MIN : 30
 *   - MAX : 255
 *  if under 30, CMC624 val : 2%
 *  if 30, CMC624 val : 3%
 *  if default, CMC624 val : 49%
 *  if 255, CMC624 val : 100%
 */

#define DIMMING_BRIGHTNESS_VALUE    20
#define MINIMUM_BRIGHTNESS_VALUE    30
#define MAXIMUM_BRIGHTNESS_VALUE    255
#define DEFAULT_BRIGHTNESS_VALUE    144
#define QUATER_BRIGHTNESS_VALUE     87

#define DIMMING_CMC624_VALUE    1
#define MINIMUM_CMC624_VALUE    1	/*  0% progress bar */
#define MAXIMUM_CMC624_VALUE    100	/*  100% progress bar */
#define DEFAULT_CMC624_VALUE    49	/* 50% progress bar */
#define QUATER_CMC624_VALUE     9	/*  25% progress bar */

#define QUATER_DEFAUT_MAGIC_VALUE   3
#define UNDER_DEFAULT_MAGIC_VALUE   52
#define OVER_DEFAULT_MAGIC_VALUE    17

#if defined(CONFIG_MACH_P4_LTE) && defined(CONFIG_TARGET_LOCALE_JPN_NTT)
void cmc624_manual_brightness(int bl_lvl)
{
	/*  Platform Range : 0 ~ 255
	 *  CMC624   Range : 0 ~ 100
	 *  User Platform Range :
	 *   - MIN : 30
	 *   - MAX : 255
	 *
	 *  if 30, CMC624 val : 5
	 *  if 255, CMC624 val : 100
	 *  */
	int value;

	if (bl_lvl < 30) {
		value = bl_lvl / 6;
	} else if (bl_lvl <= 255) {
		value = ((95 * bl_lvl) / 225) - 7;
	} else {
		pr_debug("[CMC624] Wrong Backlight scope!!!!!! [%d]\n",
			 bl_lvl);
		value = 50;
	}

	if (value < 0 || value > 100)
		pr_debug("[CMC624] Wrong value scope [%d]\n", value);
	pr_debug("[CMC624] BL : %d Value : %d\n", bl_lvl, value);
	set_backlight_pwm(value);
	cmc624_state.brightness = value;
}

#else
void cmc624_manual_brightness(int bl_lvl)
{
	int value;

	if (bl_lvl < MINIMUM_BRIGHTNESS_VALUE) {
		if (bl_lvl == 0)
			value = 0;
		else
			value = DIMMING_CMC624_VALUE;
	} else if (bl_lvl <= QUATER_BRIGHTNESS_VALUE) {
		value =
		    ((QUATER_CMC624_VALUE -
		      MINIMUM_CMC624_VALUE) * bl_lvl /
		     (QUATER_BRIGHTNESS_VALUE - MINIMUM_BRIGHTNESS_VALUE)) -
		    QUATER_DEFAUT_MAGIC_VALUE;
	} else if (bl_lvl <= DEFAULT_BRIGHTNESS_VALUE) {
		value =
		    ((DEFAULT_CMC624_VALUE -
		      QUATER_CMC624_VALUE) * bl_lvl /
		     (DEFAULT_BRIGHTNESS_VALUE - QUATER_BRIGHTNESS_VALUE)) -
		    UNDER_DEFAULT_MAGIC_VALUE;
	} else if (bl_lvl <= MAXIMUM_BRIGHTNESS_VALUE) {
		value =
		    ((MAXIMUM_CMC624_VALUE -
		      DEFAULT_CMC624_VALUE) * bl_lvl /
		     (MAXIMUM_BRIGHTNESS_VALUE - DEFAULT_BRIGHTNESS_VALUE)) -
		    OVER_DEFAULT_MAGIC_VALUE;
	} else {
		pr_debug("[CMC624] Wrong Backlight scope!!!!!! [%d]\n",
			 bl_lvl);
		value = 50;
	}

	if (value < 0 || value > 100)
		pr_debug("[CMC624] Wrong value scope [%d]\n", value);
	value = 50;
	pr_debug("[CMC624] BL : %d Value : %d\n", bl_lvl, value);
	set_backlight_pwm(value);
	cmc624_state.brightness = value;
}
#endif

#if defined(CONFIG_TARGET_LOCALE_KOR_SKT)\
	 || defined(CONFIG_TARGET_LOCALE_KOR_LGU)
void cmc624_Set_Region_Ext(int enable, int hStart, int hEnd, int vStart,
			   int vEnd)
{
	u16 data = 0;

	mutex_lock(&tuning_mutex);

	cmc624_I2cWrite16(0x0000, 0x0000);
	cmc624_I2cRead16(0x0001, &data);

	data &= 0x00ff;

	if (enable) {
		cmc624_I2cWrite16(0x00, 0x0000);	/*BANK 0*/
		cmc624_I2cWrite16(0x0c, 0x5555);     /*ROI on */
		cmc624_I2cWrite16(0x0d, 0x1555);	/*ROI on */
		cmc624_I2cWrite16(0x0e, hStart);	/*ROI x start 0 */
		cmc624_I2cWrite16(0x0f, hEnd);	/*ROI x end 1279 */
		cmc624_I2cWrite16(0x10, vStart + 1);	/*ROI y start 1 */
		cmc624_I2cWrite16(0x11, vEnd + 1);	/*ROI y end 800 */
		cmc624_I2cWrite16(0xff, 0x0000);	/*Mask Release */
	} else {
		cmc624_I2cWrite16(0x00, 0x0000);	/*BANK 0 */
		cmc624_I2cWrite16(0x0c, 0x0000);	/*ROI off */
		cmc624_I2cWrite16(0x0d, 0x0000);	/*ROI off */
		cmc624_I2cWrite16(0xff, 0x0000);	/*Mask Release */
	}

	mutex_unlock(&tuning_mutex);

	cmc624_current_region_enable = enable;
}
EXPORT_SYMBOL(cmc624_Set_Region_Ext);

#endif

int samsung_lvds_on(int enable)
{
	int status;

	if (enable)
		status =
		    gpio_direction_output(PM8917_GPIO_PM_TO_SYS
					  (PMIC_GPIO_LVDS_nSHDN),
					  PMIC_GPIO_LCD_LEVEL_HIGH);
	else
		status =
		    gpio_direction_output(PM8917_GPIO_PM_TO_SYS
					  (PMIC_GPIO_LVDS_nSHDN),
					  PMIC_GPIO_LCD_LEVEL_LOW);

	status =
	    gpio_get_value_cansleep(PM8917_GPIO_PM_TO_SYS
				    (PMIC_GPIO_LVDS_nSHDN));

	pr_debug("gpio_get_value result. LVDS_nSHDN : %d\n", status);

	return (unsigned int)status;

}

int samsung_backlight_en(int enable)
{
	int status;

	if (enable)
		status =
		    gpio_direction_output(PMIC_GPIO_BACKLIGHT_RST,
					  PMIC_GPIO_LCD_LEVEL_HIGH);
	else
		status =
		    gpio_direction_output(PMIC_GPIO_BACKLIGHT_RST,
					  PMIC_GPIO_LCD_LEVEL_LOW);
	status =
	    gpio_get_value_cansleep(PMIC_GPIO_BACKLIGHT_RST);
	pr_debug("gpio_get_value. BACKLIGHT_EN : %d\n", status);

	return (unsigned int)status;
}

static int cmc624_i2c_probe(struct i2c_client *client,
			    const struct i2c_device_id *id)
{
	struct cmc624_data *data;

	pr_info("%s +\n", __func__);
	pr_debug("==============================\n");
	pr_debug("cmc624 attach START!!!\n");
	pr_debug("==============================\n");

	data = kzalloc(sizeof(struct cmc624_data), GFP_KERNEL);
	if (!data) {
		dev_err(&client->dev, "Failed to allocate memory\n");
		return -ENOMEM;
	}

	data->client = client;
	i2c_set_clientdata(client, data);

	dev_info(&client->dev, "cmc624 i2c probe success!!!\n");

	p_cmc624_data = data;

	pr_debug("==============================\n");
	pr_debug("CMC624 SYSFS INIT!!!\n");
	pr_debug("==============================\n");
	pr_info("%s -\n", __func__);

	return 0;
}

static int __devexit cmc624_i2c_remove(struct i2c_client *client)
{
	struct cmc624_data *data = i2c_get_clientdata(client);

	i2c_set_clientdata(client, NULL);

	kfree(data);

	dev_info(&client->dev, "cmc624 i2c remove success!!!\n");

	return 0;
}

void cmc624_shutdown(struct i2c_client *client)
{
	pr_debug("-0- %s called -0-\n", __func__);
}

static const struct i2c_device_id cmc624[] = {
	{"cmc624", 0},
};

MODULE_DEVICE_TABLE(i2c, cmc624);

struct i2c_driver cmc624_i2c_driver = {
	.driver = {
		   .name = "cmc624",
		   .owner = THIS_MODULE,
		   },
	.probe = cmc624_i2c_probe,
	.remove = __devexit_p(cmc624_i2c_remove),
	.id_table = cmc624,
	.shutdown = cmc624_shutdown,
};

int samsung_cmc624_init(void)
{
	int ret;

	/* register I2C driver for CMC624 */

	pr_debug("<cmc624_i2c_driver Add START>\n");
	ret = i2c_add_driver(&cmc624_i2c_driver);
	pr_debug("cmc624_init Return value  (%d)\n", ret);
	pr_debug("<cmc624_i2c_driver Add END>\n");

	return 0;

}
bool samsung_has_cmc624(void)
{
#ifdef CONFIG_MACH_M2_ATT
	if ((system_rev >= 11) && (system_rev != 13))
		return false;
	else
		return true;
#elif defined(CONFIG_MACH_M2_SPR)
	if (system_rev >= 11)
		return false;
	else
		return true;
#elif defined(CONFIG_MACH_M2_VZW)
	if ((system_rev == 4) || (system_rev >= 14))
		return false;
	else
		return true;
#elif defined(CONFIG_MACH_M2_SKT)
	if (system_rev >= 10)
		return false;
	else
		return true;
#elif defined(CONFIG_MACH_M2_DCM) || defined(CONFIG_MACH_K2_KDI)
	if (system_rev >= 5)
		return false;
	else
		return true;
#elif defined(CONFIG_MACH_KONA)
	if(system_rev >= 3)
		return true;
	else 
		return false;
#elif defined(CONFIG_MACH_MELIUS)
#if defined(CONFIG_MACH_MELIUS_EUR_LTE) || defined(CONFIG_MACH_MELIUS_EUR_OPEN) || \
	defined(CONFIG_MACH_MELIUS_SKT) || defined(CONFIG_MACH_MELIUS_KTT) || \
	defined(CONFIG_MACH_MELIUS_LGT)
	if (system_rev > 2)
		return true;
	else
		return false;
#else
	return true;
#endif
#else
	return false;
#endif
}
EXPORT_SYMBOL(samsung_has_cmc624);

static int  cmc_pmic_gpios_pmconfig(int state)
{
	int ret;

	struct pm_gpio param = {
		.direction = PM_GPIO_DIR_OUT,
		.output_buffer = PM_GPIO_OUT_BUF_CMOS,
		.output_value = 0,
		.pull = PM_GPIO_PULL_NO,
		.vin_sel = 2,
		.out_strength = PM_GPIO_STRENGTH_HIGH,
		.function = PM_GPIO_FUNC_NORMAL,
		.inv_int_pol = 0,
		.disable_pin = state,
	};
	ret = pm8xxx_gpio_config(IMA_PWR_EN, &param);
	if (ret) {
		pr_err("%s: Failed to configure gpio %d\n", __func__,
			IMA_PWR_EN);
		return ret;
	}
	ret = pm8xxx_gpio_config(IMA_nRST, &param);
	if (ret) {
		pr_err("%s: Failed to configure gpio %d\n", __func__,
			IMA_nRST);
		return ret;
	}
	ret = pm8xxx_gpio_config(IMA_SLEEP, &param);
	if (ret) {
		pr_err("%s: Failed to configure gpio %d\n", __func__,
			IMA_SLEEP);
		return ret;
	}
	ret = pm8xxx_gpio_config(IMA_CMC_EN, &param);
	if (ret) {
		pr_err("%s: Failed to configure gpio %d\n", __func__,
			IMA_CMC_EN);
		return ret;
	}

	return 0;
}

int cmc_power(int on)
{
	static struct regulator *reg_l29;
	int rc;

	reg_l29 = regulator_get(NULL, "8921_l29");
	if (IS_ERR(reg_l29)) {
		pr_err("could not get 8921_l29, rc = %ld\n",
			PTR_ERR(reg_l29));
		return -ENODEV;
	}
	rc = regulator_set_voltage(reg_l29, 0, 1800000);
	if (rc) {
		pr_err("set_voltage l29 failed, rc=%d\n", rc);
		return -EINVAL;
	}

	if (on) {
		cmc_pmic_gpios_pmconfig(0);
	
		/*RESETB->hi, FAILSAFE->lo, SLEEPB->hi */
		pr_info("Performing CMC624 Power On Init\n");
		gpio_set_value(IMA_nRST, 1); /*RESETB*/
		gpio_set_value(IMA_SLEEP, 1); /*SLEEPB*/
		udelay(50); /*Wait 50us*/
		/* V_IMA_1.1V on*/
		gpio_set_value(IMA_PWR_EN, 1);
		udelay(50);
		/*V_IMA_1.8V on*/
		
		rc = regulator_set_optimum_mode(reg_l29, 100000);
		if (rc < 0) {
			pr_err("set_optimum_mode l29 failed, rc=%d\n", rc);
			return -EINVAL;
		}
		rc = regulator_enable(reg_l29);
		if (rc) {
			pr_err("enable l29 failed, rc=%d\n", rc);
			return -ENODEV;
		}

		udelay(50);
		gpio_set_value(IMA_CMC_EN, 1); /*FAIL_SAFEB*/
		udelay(50);
		gpio_set_value(IMA_nRST, 0); /*RESETB*/
		mdelay(4);
		gpio_set_value(IMA_nRST, 1); /*RESETB*/
		return 0;
	} else {
		pr_info("CMC Power off ................\n");
		/*FAILSAFE->lo*/
		gpio_set_value(IMA_CMC_EN, 0);
		/*V_IMA_1.8V off*/
		rc = regulator_disable(reg_l29);
		if (rc) {
			pr_err("disable reg_l29 failed, rc=%d\n", rc);
			return -ENODEV;
		}

		rc = regulator_set_optimum_mode(reg_l29, 100);
		if (rc < 0) {
			pr_err("set_optimum_mode l29 failed, rc=%d\n", rc);
			return -EINVAL;
		}

		/* V_IMA_1.1V off*/
		gpio_set_value(IMA_PWR_EN, 0);
		/* RESETB->lo, FAILSAFE->lo, SLEEPB->lo */
		gpio_set_value(IMA_nRST, 0);
		gpio_set_value(IMA_SLEEP, 0);

		cmc_pmic_gpios_pmconfig(1);

		gpio_tlmm_config(GPIO_CFG(GPIO_IMA_I2C_SDA, 0,
			GPIO_CFG_INPUT,
			GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), GPIO_CFG_DISABLE);

		gpio_tlmm_config(GPIO_CFG(GPIO_IMA_I2C_SCL, 0,
			GPIO_CFG_INPUT,
			GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), GPIO_CFG_DISABLE);
		return 0;
	}
}
EXPORT_SYMBOL(cmc_power);

/* Module information */
MODULE_AUTHOR("Samsung Electronics");
MODULE_DESCRIPTION("samsung CMC624 image converter");
MODULE_LICENSE("GPL");
