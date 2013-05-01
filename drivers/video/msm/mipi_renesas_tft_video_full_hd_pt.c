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
#include "mipi_samsung_tft.h"

static struct msm_panel_info pinfo;
static struct mipi_panel_data mipi_pd;

enum {
	GAMMA_30CD  =   8, // MIN 10 from platform
	GAMMA_40CD  =  16,
	GAMMA_50CD  =  22,
	GAMMA_60CD  =  28,
	GAMMA_70CD  =  34,
	GAMMA_80CD  =  40,
	GAMMA_90CD  =  46,
	GAMMA_100CD =  52,
	GAMMA_110CD =  58,
	GAMMA_120CD =  64,
	GAMMA_130CD =  70,
	GAMMA_140CD =  76,
	GAMMA_150CD =  82, // DEF 150 from platform
	GAMMA_160CD =  94,
	GAMMA_170CD = 106,
	GAMMA_180CD = 118,
	GAMMA_190CD = 130,
	GAMMA_200CD = 142,
	GAMMA_210CD = 154,
	GAMMA_220CD = 166,
	GAMMA_230CD = 178,
	GAMMA_240CD = 190,
	GAMMA_250CD = 202, // MAX 255 from platform
	GAMMA_300CD = 202,
};


static char renesas_manufacture_cmd_access_off[] = {
	0xB0,
	0x04,
};

static char renesas_nop[] = {
	0x0, 0x0, 0x0, 0x0,
};

static char renesas_manufacture_cmd_access_on[] = {
	0xB0,
	0x03,
};

static char renesas_brightness_setting[] = {
	0x51,
	0xCA, /*0xFF,*/
};

static char renesas_brightness_packet[] = {
	0x51,
	0xCA, /*0xFF, */
};

static char renesas_cabc_control[] = {
	0x55,
	0x00,
};

#if defined(AUTO_BRIGHTNESS_CABC_FUNCTION)
static char renesas_cabc_enable_control[] = {
	0x55,
	0x01,
};

static char renesas_cabc_disable_control[] = {
	0x55,
	0x00,
};
#endif

static char renesas_backlight_control[] = {
	0x53,
	0x2C,
};

static char renesas_memory_access_control[] = {
	0x36,
	0xC0,
};

static char renesas_display_on[] = { 0x29, /* no param */ };
static char renesas_display_off[] = { 0x28, /* no param */ };
static char renesas_sleep_in[] = { 0x10, /* no param */ };
static char renesas_sleep_out[] = { 0x11, /* no param */ };

static struct dsi_cmd_desc renesas_ready_to_on_cmds[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(renesas_brightness_setting), renesas_brightness_setting},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(renesas_cabc_control), renesas_cabc_control},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(renesas_backlight_control), renesas_backlight_control},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(renesas_sleep_out), renesas_sleep_out},

	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(renesas_display_on), renesas_display_on},
};

static struct dsi_cmd_desc panel_off_cmds[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(renesas_display_off), renesas_display_off},
};

static struct dsi_cmd_desc panel_late_on_cmds[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 5,
		sizeof(renesas_display_on), renesas_display_on},
};

static struct dsi_cmd_desc panel_early_off_cmds[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(renesas_sleep_in), renesas_sleep_in},
};

static struct dsi_cmd_desc panel_mtp_enable_cmds[] = {
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(renesas_manufacture_cmd_access_off),
					renesas_manufacture_cmd_access_off},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(renesas_nop), renesas_nop},
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(renesas_nop), renesas_nop},
};

static struct dsi_cmd_desc panel_mtp_disable_cmds[] = {
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(renesas_manufacture_cmd_access_on),
					renesas_manufacture_cmd_access_on},
};

static struct dsi_cmd_desc panel_need_flip_cmds[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(renesas_memory_access_control),
					renesas_memory_access_control},
};

static struct dsi_cmd_desc brightness_packet[] = {
	{DTYPE_DCS_LWRITE, 1, 0, 0, 0,
		sizeof(renesas_brightness_packet), renesas_brightness_packet},
};

#if defined(AUTO_BRIGHTNESS_CABC_FUNCTION)
static struct dsi_cmd_desc panel_cabc_enable_cmds[] = {
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(renesas_cabc_enable_control), renesas_cabc_enable_control},
};

static struct dsi_cmd_desc panel_cabc_disable_cmds[] = {
	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(renesas_cabc_disable_control), renesas_cabc_disable_control},
};
#endif

static int get_candela_index(int bl_level)
{
	int backlightlevel;

	/* brightness setting from platform is from 0 to 255
	 * But in this driver, brightness is only supported from 0 to 24 */

	switch (bl_level) {
	case 0 ... 39:
		backlightlevel = GAMMA_30CD; /* 0*/
		break;
	case 40 ... 49:
		backlightlevel = GAMMA_40CD; /* 1 */
		break;
	case 50 ... 59:
		backlightlevel = GAMMA_50CD; /* 2 */
		break;
	case 60 ... 69:
		backlightlevel = GAMMA_60CD; /* 3 */
		break;
	case 70 ... 79:
		backlightlevel = GAMMA_70CD; /* 4 */
		break;
	case 80 ... 89:
		backlightlevel = GAMMA_80CD; /* 5 */
		break;
	case 90 ... 99:
		backlightlevel = GAMMA_90CD; /* 6 */
		break;
	case 100 ... 109:
		backlightlevel = GAMMA_100CD; /* 7 */
		break;
	case 110 ... 119:
		backlightlevel = GAMMA_110CD; /* 8 */
		break;
	case 120 ... 129:
		backlightlevel = GAMMA_120CD; /* 9 */
		break;
	case 130 ... 139:
		backlightlevel = GAMMA_130CD; /* 10 */
		break;
	case 140 ... 149:
		backlightlevel = GAMMA_140CD; /* 11 */
		break;
	case 150 ... 159:
		backlightlevel = GAMMA_150CD; /* 12 */
		break;
	case 160 ... 169:
		backlightlevel = GAMMA_160CD; /* 13 */
		break;
	case 170 ... 179:
		backlightlevel = GAMMA_170CD; /* 14 */
		break;
	case 180 ... 189:
		backlightlevel = GAMMA_180CD; /* 15 */
		break;
	case 190 ... 199:
		backlightlevel = GAMMA_190CD; /* 16 */
		break;
	case 200 ... 209:
		backlightlevel = GAMMA_200CD; /* 17 */
		break;
	case 210 ... 219:
		backlightlevel = GAMMA_210CD; /* 18 */
		break;
	case 220 ... 229:
		backlightlevel = GAMMA_220CD; /* 10 */
		break;
	case 230 ... 239:
		backlightlevel = GAMMA_230CD; /* 20 */
		break;
	case 240 ... 249:
		backlightlevel = GAMMA_240CD; /* 21 */
		break;
	case 250 ... 254:
		backlightlevel = GAMMA_250CD; /* 22 */
		break;
	case 255:
		if (mipi_pd.msd->dstat.auto_brightness == 1)
			backlightlevel = GAMMA_300CD; /* 23 */
		else
			backlightlevel = GAMMA_250CD; /* 22 */
		break;
	default:
		backlightlevel = GAMMA_40CD; /* 1 */
		break;
	}
	return backlightlevel;
}

static int brightness_control(int bl_level)
{
	int level = get_candela_index(bl_level);
	int compensation = 500;

	/* 0x0 ~ 0x0FFF,  0 ~ 4095*/
	int value = 178 * level + compensation;

	if (value > 0xFFF)
		value = 0xFFF;

	if (system_rev == 0) {
		renesas_brightness_packet[1] = value & 0xFF;
		renesas_brightness_packet[1] = (value >> 8) & 0xFF;
	} else
		renesas_brightness_packet[1] =  level & 0xFF;

	return 0;
}

static struct mipi_panel_data mipi_pd = {
	.panel_name	= "JDI_ACX454AKM\n",
	.on		= {renesas_ready_to_on_cmds
				, ARRAY_SIZE(renesas_ready_to_on_cmds)},
	.off		= {panel_off_cmds
				, ARRAY_SIZE(panel_off_cmds)},
	.late_on		= {panel_late_on_cmds
				, ARRAY_SIZE(panel_late_on_cmds)},
	.early_off	= {panel_early_off_cmds
				, ARRAY_SIZE(panel_early_off_cmds)},

	.brightness	= {brightness_packet
				, ARRAY_SIZE(brightness_packet)},
	.backlight_control = brightness_control,

	.mtp_enable	= {panel_mtp_enable_cmds
				, ARRAY_SIZE(panel_mtp_enable_cmds)},
	.mtp_disable	= {panel_mtp_disable_cmds
				, ARRAY_SIZE(panel_mtp_disable_cmds)},
	.need_flip	= {panel_need_flip_cmds
				, ARRAY_SIZE(panel_mtp_disable_cmds)},
#if defined(AUTO_BRIGHTNESS_CABC_FUNCTION)
	.cabc_enable = {panel_cabc_enable_cmds
				, ARRAY_SIZE(panel_cabc_enable_cmds)},
	.cabc_disable	= {panel_cabc_disable_cmds
				, ARRAY_SIZE(panel_cabc_disable_cmds)},
#endif
};

static struct mipi_dsi_phy_ctrl dsi_video_mode_phy_db = {
	/* 1920*1200, RGB888, 4 Lane 60 fps video mode */
	/* regulator */
	.regulator = {0x03, 0x0a, 0x04, 0x00, 0x20},
	/* timing */
	.timing = {0xD9, 0x31, 0x1E, 0x00, 0x52, 0x5E, 0x24, 0x35,
	0x3C, 0x03, 0x04, 0xa0},
	/* phy ctrl */
	.ctrl = {0x5f, 0x00, 0x00, 0x10},
	/* strength */
	.strength = {0xff, 0x00, 0x06, 0x00},
	/* pll control */
	.pll = {0x0, 0x7f, 0x31, 0xda, 0x00, 0x50, 0x48, 0x63,
	0x41, 0x0f, 0x03,
	0x00, 0x14, 0x03, 0x00, 0x02, 0x00, 0x20, 0x00, 0x01 },
};

static int __init mipi_cmd_samsung_tft_full_hd_pt_init(void)
{
	int ret;

	printk(KERN_DEBUG "[lcd] mipi_cmd_samsung_tft_full_hd_pt_init start\n");

#ifdef CONFIG_SUPPORT_DISPLAY_OCTA_TFT
#if 0
	if (check_display_type() != SAMSUNG_TFT_FULL_HD) {
		pr_err("[lcd] error : this rev(%d) is not for TFT, return!!\n",
			system_rev);
		return 1;
	}
#endif
#endif

#ifdef CONFIG_FB_MSM_MIPI_PANEL_DETECT
	if (msm_fb_detect_client("mipi_cmd_samsung_oled_qhd"))
		return 0;
#endif
	pinfo.xres = 1080;
	pinfo.yres = 1920;

	pinfo.type = MIPI_VIDEO_PANEL;
	pinfo.pdest = DISPLAY_1;
	pinfo.wait_cycle = 0;
	pinfo.bpp = 24;

	pinfo.lcdc.h_back_porch = 100;
	pinfo.lcdc.h_front_porch = 97;
	pinfo.lcdc.h_pulse_width = 14;

	pinfo.lcdc.v_back_porch = 6;
	pinfo.lcdc.v_front_porch = 8;
	pinfo.lcdc.v_pulse_width = 2;

	pinfo.lcdc.border_clr = 0;	/* blk */
	pinfo.lcdc.underflow_clr = 0x000000;	/* blue */
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
	pinfo.mipi.esc_byte_ratio = 1;

	ret = mipi_samsung_tft_device_register(&pinfo, MIPI_DSI_PRIM,
						MIPI_DSI_PANEL_FULL_HD_PT,
						&mipi_pd);
	if (ret)
		pr_err("%s: failed to register device!\n", __func__);

	printk(KERN_DEBUG "[lcd] mipi_cmd_samsung_tft_full_hd_pt_init end\n");

	return ret;
}
module_init(mipi_cmd_samsung_tft_full_hd_pt_init);
