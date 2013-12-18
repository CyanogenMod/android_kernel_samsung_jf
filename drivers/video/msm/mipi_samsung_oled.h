/* Copyright (c) 2010, Code Aurora Forum. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of Code Aurora Forum, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef MIPI_SAMSUNG_OLED_H
#define MIPI_SAMSUNG_OLED_H

#define USE_ACL
#define USE_ELVSS
#include "smart_dimming.h"

#if defined(CONFIG_FB_MSM_MIPI_SAMSUNG_OLED_VIDEO_WVGA_PT)
#include "smart_mtp_s6e63m0.h"
#elif defined(CONFIG_FB_MSM_MIPI_SAMSUNG_OLED_VIDEO_HD_PT)
#include "smart_mtp_s6e8aa0x01.h"
#elif defined(CONFIG_FB_MSM_MIPI_SAMSUNG_OLED_CMD_QHD_PT)
#include "smart_mtp_s6e39a0x02.h"
#endif

#define SmartDimming_useSampleValue
#define SmartDimming_CANDELA_UPPER_LIMIT (300)
#define MTP_DATA_SIZE (24)
#define MTP_DATA_SIZE_S6E63M0 (21)
#define ELVSS_DATA_SIZE (24)
#define MTP_REGISTER	(0xD3)
#define ELVSS_REGISTER	 (0xD4)
#define SmartDimming_GammaUpdate_Pos (2)
#define DIMMING_BL (20)

#if defined(CONFIG_FB_MSM_MIPI_SAMSUNG_OLED_VIDEO_WVGA_PT) ||\
	defined(CONFIG_FB_MSM_MIPI_SAMSUNG_OLED_VIDEO_HD_PT) ||\
	defined(CONFIG_FB_MSM_MIPI_SAMSUNG_OLED_CMD_QHD_PT)
#define USE_READ_ID
#endif
enum mipi_samsung_cmd_list {
#ifdef CONFIG_FB_MSM_MIPI_SAMSUNG_OLED_VIDEO_HD_PT
	PANEL_READY_TO_ON_FAST,
#endif
	PANEL_READY_TO_ON,
	PANEL_READY_TO_OFF,
	PANEL_ON,
	PANEL_OFF,
	PANEL_LATE_ON,
	PANEL_EARLY_OFF,
	PANEL_GAMMA_UPDATE,
	PANEL_ELVSS_UPDATE,
	PANEL_ACL_ON,
	PANEL_ACL_OFF,
	PANEL_ACL_UPDATE,
	MTP_READ_ENABLE,
	PANEL_BRIGHT_CTRL,
};

enum gamma_mode_list {
	GAMMA_2_2 = 0,
	GAMMA_1_9 = 1,
	GAMMA_SMART = 2,
};

enum {
	MIPI_SUSPEND_STATE,
	MIPI_RESUME_STATE,
};

struct cmd_set {
	struct dsi_cmd_desc *cmd;
	int size;
};

struct gamma_table {
	char *table;
	int table_cnt;
	int data_size;
};

struct display_status {
	unsigned char acl_on;
	unsigned char gamma_mode; /* 1: 1.9 gamma, 0: 2.2 gamma */
	unsigned char is_smart_dim_loaded;
	unsigned char is_elvss_loaded;
	unsigned char auto_brightness;
};

#if defined(CONFIG_MIPI_SAMSUNG_ESD_REFRESH)
extern void set_esd_refresh(boolean stat);
#endif

struct dsi_cmd_desc_LCD {
	int lux;
	char strID[8];
	struct dsi_cmd_desc *cmd;
};


struct mipi_samsung_driver_data {
	struct dsi_buf samsung_tx_buf;
	struct dsi_buf samsung_rx_buf;
	struct msm_panel_common_pdata *mipi_samsung_disp_pdata;
	struct mipi_panel_data *mpd;
	struct display_status dstat;
#if defined(CONFIG_HAS_EARLYSUSPEND)
	struct early_suspend early_suspend;
#endif
#if defined(CONFIG_HAS_EARLYSUSPEND) || defined(CONFIG_LCD_CLASS_DEVICE)
	struct platform_device *msm_pdev;
#endif
#if defined(CONFIG_MIPI_SAMSUNG_ESD_REFRESH)
	boolean esd_refresh;
#endif
};

struct mipi_panel_data {
	const char panel_name[20];
	struct cmd_set ready_to_on;
#ifdef CONFIG_FB_MSM_MIPI_SAMSUNG_OLED_VIDEO_HD_PT
	struct cmd_set ready_to_on_4_8;
	struct cmd_set ready_to_on_4_8_fast;
	struct cmd_set ready_to_on_fast;
#endif
	struct cmd_set ready_to_off;
	struct cmd_set on;
	struct cmd_set off;
	struct cmd_set late_on;
	struct cmd_set early_off;
	struct cmd_set gamma_update;
	struct cmd_set elvss_update;
	struct cmd_set mtp_read_enable;
#ifdef CONFIG_FB_MSM_MIPI_SAMSUNG_OLED_VIDEO_HD_PT
	struct cmd_set elvss_update_4_8;
#endif
	struct cmd_set acl_update;
#ifdef USE_ACL
	struct cmd_set acl_on;
	struct cmd_set acl_off;
	boolean ldi_acl_stat;
#endif
	struct str_smart_dim smart;
	signed char lcd_current_cd_idx;
	unsigned char lcd_mtp_data[MTP_DATA_SIZE+16] ;
	unsigned char lcd_elvss_data[ELVSS_DATA_SIZE+16];
	unsigned char *gamma_smartdim;
	unsigned char *gamma_initial;
#ifdef CONFIG_FB_MSM_MIPI_SAMSUNG_OLED_VIDEO_HD_PT
	unsigned char *gamma_smartdim_4_8;
	int *lux_table;
	int lux_table_max_cnt;
	struct SMART_DIM smart_s6e8aa0x01;
	struct cmd_set combined_ctrl;
#endif
#if defined(CONFIG_FB_MSM_MIPI_SAMSUNG_OLED_VIDEO_WVGA_PT) ||\
	defined(CONFIG_FB_MSM_MIPI_SAMSUNG_OLED_VIDEO_HD_PT) ||\
	defined(CONFIG_FB_MSM_MIPI_SAMSUNG_OLED_CMD_QHD_PT)
	unsigned int manufacture_id;
#endif
#if defined(CONFIG_FB_MSM_MIPI_SAMSUNG_OLED_VIDEO_WVGA_PT)
	struct SMART_DIM smart_s6e63m0;
#endif
#if  defined(CONFIG_FB_MSM_MIPI_SAMSUNG_OLED_CMD_QHD_PT)
	struct SMART_DIM smart_s6e39a0x02;
#endif
	int (*set_gamma)(int bl_level, enum gamma_mode_list gamma_mode);
	int (*set_acl)(int bl_level);
	int (*set_elvss)(int bl_level);
#ifdef CONFIG_FB_MSM_MIPI_SAMSUNG_OLED_VIDEO_HD_PT
	int (*set_elvss_4_8)(int bl_level);
	int (*prepare_brightness_control_cmd_array)(int lcd_type, int bl_level);
	void  (*prepare_fast_cmd_array)(int lcd_type);

#endif
	struct mipi_samsung_driver_data *msd;
};

int mipi_samsung_device_register(struct msm_panel_info *pinfo,
					u32 channel, u32 panel,
					struct mipi_panel_data *mpd);

#if defined(CONFIG_FB_MSM_MIPI_SAMSUNG_OLED_VIDEO_HD_PT) ||\
	defined(CONFIG_FB_MSM_MIPI_SAMSUNG_OLED_CMD_QHD_PT) ||\
	defined(CONFIG_FB_MSM_MIPI_SAMSUNG_OLED_VIDEO_WVGA_PT)

void reset_gamma_level(void);
unsigned char bypass_LCD_Id(void);
#endif

#if defined(CONFIG_FB_MSM_MIPI_SAMSUNG_OLED_VIDEO_HD_PT)
	/* [junesok] This was originally defined in samsung_cmc624.c */
/*extern u8 mtp_read_data[];
*/
#endif

#if defined(CONFIG_SAMSUNG_CMC624)
extern int cmc_fast_reset;
#endif
#endif  /* MIPI_SAMSUNG_OLED_H */
