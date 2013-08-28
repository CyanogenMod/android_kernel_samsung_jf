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

#ifdef CONFIG_HAS_EARLYSUSPEND
#undef CONFIG_HAS_EARLYSUSPEND
#endif

#if defined(CONFIG_MIPI_SUPPORT_TFT_IN_HW_00_01)
#include <mach/apq8064-gpio.h>
#endif
#include "smart_mtp_se6e8fa.h"

enum mipi_samsung_cmd_list {

	PANEL_READY_TO_ON,
	PANEL_READY_TO_OFF,
	PANEL_ON,
	PANEL_OFF,
	PANEL_LATE_ON,
	PANEL_EARLY_OFF,
	PANEL_BRIGHT_CTRL,
	PANEL_MTP_ENABLE,
	PANEL_MTP_DISABLE,
	PANEL_NEED_FLIP,
	PANEL_ACL_CONTROL,
	PANLE_TOUCH_KEY,
	PANEL_LPTS,
};

enum {
	MIPI_SUSPEND_STATE,
	MIPI_RESUME_STATE,
};

struct cmd_set {
	struct dsi_cmd_desc *cmd;
	int size;
};


struct display_status {
	unsigned char auto_brightness;
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
};

struct mipi_panel_data {
	const char panel_name[20];
	struct cmd_set on;
	struct cmd_set off;
	struct cmd_set late_on;
	struct cmd_set early_off;
	struct cmd_set mtp_enable;
	struct cmd_set mtp_disable;
	struct cmd_set need_flip;
	struct cmd_set brightness;
	struct cmd_set acl_cmds;
	struct cmd_set touch_key;
	struct cmd_set lpts;
#if defined(CONFIG_MIPI_SUPPORT_TFT_IN_HW_00_01)
	struct cmd_set on_tft;
	struct cmd_set off_tft;
	struct cmd_set late_on_tft;
	struct cmd_set early_off_tft;
	struct cmd_set mtp_enable_tft;
	struct cmd_set mtp_disable_tft;
	struct cmd_set need_flip_tft;
	struct cmd_set brightness_tft;
#endif

	unsigned int manufacture_id;

	struct mipi_samsung_driver_data *msd;

	int (*backlight_control)(int bl_level);
	int (*acl_control)(int bl_level);
	int (*cmd_set_change)(int cmd_set, int panel_id);
	int (*lpts_set_change)(int fps);
	void (*hover_flicker_change)(char CB_3rd);

	int *lux_table;
	int lux_table_max_cnt;
	struct SMART_DIM smart_se6e8fa;

	void(*reset_bl_level)(void);
	int brightness_level;

	int acl_status;
	int siop_status;

	int first_bl_hbm_psre;
	char ldi_rev;

	int coordinate[2];

	int temperature;
	char temperature_value;
	int need_update;

};

int get_lcd_attached(void);

unsigned char get_auto_brightness(void);
char* get_c8_reg_1(void);
char* get_c8_reg_2(void);
char get_elvss_400cd(void);
char get_elvss_offset(void);
char* get_b1_reg(void);
char* get_b6_reg_magna(void);

int mipi_samsung_octa_device_register(struct msm_panel_info *pinfo,
					u32 channel, u32 panel,
					struct mipi_panel_data *mpd);

int get_ldi_chip(void);

extern struct mutex dsi_tx_mutex;
#endif  /* MIPI_SAMSUNG_OLED_H */
