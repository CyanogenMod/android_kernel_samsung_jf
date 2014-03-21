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
#include "mipi_dsi.h"
#include "mipi_novatek_NT35596.h"


static struct msm_panel_info pinfo;

static char WRCMD2[] = {
	0xFF,
	0x05,
	0x00, 0x00,
};

static char WRMTP[] = {
	0xFB,
	0x01,
	0x00, 0x00,
};

static char ESD_X3_0[] = {
	0x92,
	0xFF,
	0x00, 0x00,
};

static char ESD_X3_1[] = {
	0xC5,
	0x70,
	0x00, 0x00,
}; 

static char ESD_X3_2[] = {
	0xC8,
	0x10,
	0x00, 0x00,
}; 

static char ESD_X3_3[] = {
	0xCF,
	0x00,
	0x00, 0x00,
}; 

static char ESD_X3_4[] = {
	0xD0,
	0x7F,
	0x00, 0x00,
};

static char ESD_X3_5[] = {
	0xD0,
 	0x2F,
 	0x00, 0x00,
};
 
static char WRCMD1[] = {
	0xFF,
	0x00,
	0x00, 0x00,
}; 

static char WRCABCMB[] = {
	0xD3,
	0x08,
	0x00, 0x00,
}; 
static char WRCABCMB_1[] = {
	0xD4,
	0x08,
	0x00, 0x00,
}; 

/* Add MIPI Commnad for flicker */
static char PAGESEL_01[] = {
	0xFF,
	0x01,
	0x00, 0x00,
};

static char RELOADCMD2_P0[] = {
	0xFB,
	0x01,
	0x00, 0x00,
};

static char REDV255_POS[] = {
	0xB1,
	0x03,
	0xB2, 0xFF,
};

static char REDV255_NEG[] = {
	0xED,
	0x03,
	0xEE, 0xFF,
};

static char PAGESEL_02[] = {
	0xFF,
	0x02,
	0x00, 0x00,
};

static char GREENV255_POS[] = {
	0x30,
	0x03,
	0x31, 0xFF,
};

static char GREENV255_NEG[] = {
	0x6F,
	0x03,
	0x70, 0xFF,
};

static char BLUEV255_POS[] = {
	0xAD,
	0x03,
	0xAE, 0xFF,
};

static char BLUEV255_NEG[] = {
	0xE9,
	0x03,
	0xEA, 0xFF,
};

static char PAGESEL_00[] = {
	0xFF,
	0x00,
	0x00, 0x00,
};
/* End*/

static char SLPIN[2] = {
	0x10,
};
static char SLPOUT[2] = {
	0x11,
};
static char DISPON[2] = {
	0x29,
};

static char DISPOFF[2] = {
	0x28,
};

static struct dsi_cmd_desc novatek_video_on_cmds[] = {

	{DTYPE_GEN_LWRITE, 1, 0, 0, 10,
		sizeof(WRCMD2), WRCMD2},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(WRMTP), WRMTP},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(ESD_X3_0), ESD_X3_0},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(ESD_X3_1), ESD_X3_1},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(ESD_X3_2), ESD_X3_2},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(ESD_X3_3), ESD_X3_3},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(ESD_X3_4), ESD_X3_4},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(PAGESEL_01), PAGESEL_01},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(RELOADCMD2_P0), RELOADCMD2_P0},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(REDV255_POS), REDV255_POS},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(REDV255_NEG), REDV255_NEG},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(PAGESEL_02), PAGESEL_02},	

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(RELOADCMD2_P0), RELOADCMD2_P0},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(GREENV255_POS), GREENV255_POS},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(GREENV255_NEG), GREENV255_NEG},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(BLUEV255_POS), BLUEV255_POS},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(BLUEV255_NEG), BLUEV255_NEG},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(PAGESEL_00), PAGESEL_00},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 10,
		sizeof(WRCMD1), WRCMD1},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(WRCABCMB), WRCABCMB},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
		sizeof(WRCABCMB_1), WRCABCMB_1},

	{DTYPE_DCS_WRITE, 1, 0, 0, 120,
		sizeof(SLPOUT), SLPOUT},

	{DTYPE_DCS_WRITE, 1, 0, 0, 10,
		sizeof(DISPON), DISPON},
};

static struct dsi_cmd_desc novatek_sharp_video_on_cmds[] = {

	{DTYPE_GEN_LWRITE, 1, 0, 0, 10,
	  sizeof(WRCMD2), WRCMD2},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
	  sizeof(WRMTP), WRMTP},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
	  sizeof(ESD_X3_1), ESD_X3_1},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
	  sizeof(ESD_X3_2), ESD_X3_2},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
	  sizeof(ESD_X3_3), ESD_X3_3},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
	  sizeof(ESD_X3_4), ESD_X3_5},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
	  sizeof(PAGESEL_00), PAGESEL_00},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 10,
	  sizeof(WRCMD1), WRCMD1},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
	  sizeof(WRCABCMB), WRCABCMB},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
	  sizeof(WRCABCMB_1), WRCABCMB_1},

	{DTYPE_GEN_LWRITE, 1, 0, 0, 0,
	  sizeof(WRCMD1), WRCMD1},

	{DTYPE_DCS_WRITE, 1, 0, 0, 120,
	  sizeof(SLPOUT), SLPOUT},

	{DTYPE_DCS_WRITE, 1, 0, 0, 10,
	  sizeof(DISPON), DISPON},
};

static struct dsi_cmd_desc novatek_video_off_cmds[] = {
	{DTYPE_DCS_WRITE, 1, 0, 0, 150,
		sizeof(SLPIN), SLPIN},
	{DTYPE_DCS_WRITE, 1, 0, 0, 150,
		sizeof(DISPOFF), DISPOFF},
};

static struct dsi_cmd_desc novatek_sharp_video_off_cmds[] = {
	{DTYPE_DCS_WRITE, 1, 0, 0, 150,
		sizeof(DISPOFF), DISPOFF},
	{DTYPE_DCS_WRITE, 1, 0, 0, 150,
		sizeof(SLPIN), SLPIN},
};

static struct mipi_panel_data mipi_pd = {
	.panel_name = "JDI_DX16D100VM0AAA\n",
	.on = {novatek_video_on_cmds
			, ARRAY_SIZE(novatek_video_on_cmds)},
	.off = {novatek_video_off_cmds
			, ARRAY_SIZE(novatek_video_off_cmds)},
	};

static struct mipi_panel_data mipi_sharp_pd = {
	.panel_name = "SHARP\n",
	.on = {novatek_sharp_video_on_cmds
			, ARRAY_SIZE(novatek_sharp_video_on_cmds)},
	.off = {novatek_sharp_video_off_cmds
			, ARRAY_SIZE(novatek_sharp_video_off_cmds)},
};

static struct mipi_dsi_phy_ctrl dsi_video_mode_phy_db = {
	/* DSI_BIT_CLK at 450MHz, 4 lane, RGB888 */
	{0x0F, 0x0a, 0x04, 0x00, 0x20}, /* regulator */
	/* timing   */
	{0x7F, 0x30, 0x13, 0x00, 0x41, 0x47, 0x17, 0x34,
	 0x20, 0x03, 0x04, 0xa0},
	/* phy ctrl */
	{0x5f, 0x00, 0x00, 0x10},
	/* strength */
	{0xff, 0x00, 0x06, 0x00},
	/* pll control */
	{0x0, 0x7f, 0x31, 0xda, 0x00, 0x50, 0x48, 0x63,
	 0x31, 0x0F, 0x03,/* 4 lane */
	 0x00, 0x14, 0x03, 0x00, 0x02, 0x00, 0x20, 0x00, 0x01},
};


static int __init mipi_novatek_tft_video_hd_pt_init(void)
{
	int ret = 0;

	printk("%s: start!\n", __func__);
	if (msm_fb_detect_client("mipi_novatek_tft_video_hd"))
		return 0;

	printk("[LCD] LCD id : %d\n", g_lcd_id);

	/* Landscape */
	pinfo.xres = 720;
	pinfo.yres = 1280;
	pinfo.mode2_xres = 0;
	pinfo.mode2_yres = 0;
#if defined(CONFIG_MACH_MELIUS)
//	pinfo.width = 78; //Dibya
//	pinfo.height = 139; //Dibya
#endif
	pinfo.mode2_bpp = 0;
	pinfo.type = MIPI_VIDEO_PANEL;
	pinfo.pdest = DISPLAY_1;
	pinfo.wait_cycle = 0;
	pinfo.bpp = 24; /* RGB888 = 24 bits-per-pixel */
	pinfo.fb_num = 2; /* using two frame buffers */

	/* bitclk */

	pinfo.clk_rate = 499000000;

	/*
	 * this panel is operated by DE,
	 * vsycn and hsync are ignored
	 */

	pinfo.lcdc.h_back_porch = 133;/* thfp */
	pinfo.lcdc.h_front_porch = 213;	/* thb */
	pinfo.lcdc.h_pulse_width = 7;	/* thpw */

	pinfo.lcdc.v_back_porch = 6;	/* tvfp */
	pinfo.lcdc.v_front_porch = 8;	/* tvb */
	pinfo.lcdc.v_pulse_width = 2;	/* tvpw */

	pinfo.lcdc.border_clr = 0;		/* black */
	pinfo.lcdc.underflow_clr = 0x0;	/* black */

	pinfo.lcdc.hsync_skew = 0;

	pinfo.lcdc.border_clr = 0;		/* black */
	pinfo.lcdc.underflow_clr = 0x0; /* black */

	/* Backlight levels - controled via PMIC pwm gpio */
	pinfo.bl_max = 255;
	pinfo.bl_min = 1;

	/* mipi - general */
	pinfo.mipi.mode = DSI_VIDEO_MODE;
	pinfo.mipi.pulse_mode_hsa_he = TRUE;
	pinfo.mipi.hfp_power_stop = FALSE;
	pinfo.mipi.hbp_power_stop = FALSE;
	pinfo.mipi.hsa_power_stop = FALSE;
	pinfo.mipi.eof_bllp_power_stop = TRUE;
	pinfo.mipi.bllp_power_stop = TRUE;
	pinfo.mipi.traffic_mode = DSI_BURST_MODE;
	pinfo.mipi.dst_format = DSI_VIDEO_DST_FORMAT_RGB888;
	pinfo.mipi.vc = 0;

	pinfo.mipi.rgb_swap = DSI_RGB_SWAP_RGB;
	pinfo.mipi.dlane_swap = 0x00;
	pinfo.mipi.data_lane0 = TRUE;
	pinfo.mipi.data_lane1 = TRUE;
	pinfo.mipi.data_lane2 = TRUE;
	pinfo.mipi.data_lane3 = TRUE;

	pinfo.mipi.force_clk_lane_hs = 1;
	pinfo.mipi.no_max_pkt_size = 0;

	pinfo.mipi.tx_eot_append = FALSE;
	pinfo.mipi.t_clk_post = 0x19;
	pinfo.mipi.t_clk_pre = 0x2D;
	pinfo.mipi.stream = 0; /* dma_p */
	pinfo.mipi.mdp_trigger = DSI_CMD_TRIGGER_SW;
	pinfo.mipi.dma_trigger = DSI_CMD_TRIGGER_SW;
	pinfo.mipi.frame_rate = 60;

	pinfo.mipi.esc_byte_ratio = 4;
	pinfo.mipi.dsi_phy_db = &dsi_video_mode_phy_db;


	switch(g_lcd_id) {
	case LCD_PANEL_SHARP:
		ret = mipi_novatek_disp_device_register(&pinfo, MIPI_DSI_PRIM,
							MIPI_DSI_PANEL_720P_PT, &mipi_sharp_pd);
		break;
	case LCD_PANEL_JDI:
	default:
		ret = mipi_novatek_disp_device_register(&pinfo, MIPI_DSI_PRIM,
							MIPI_DSI_PANEL_720P_PT, &mipi_pd);
		break;
	};

	if (ret)
		pr_err("%s: failed to register device!\n", __func__);

	printk("%s: end!\n", __func__);

	return ret;
}

module_init(mipi_novatek_tft_video_hd_pt_init);

