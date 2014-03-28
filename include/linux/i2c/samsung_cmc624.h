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

struct cmc624RegisterSet {
	unsigned int RegAddr;
	unsigned int Data;
};

enum eLcd_mDNIe_UI {
	mDNIe_UI_MODE,
	mDNIe_VIDEO_MODE,
	mDNIe_VIDEO_WARM_MODE,
	mDNIe_VIDEO_COLD_MODE,
	mDNIe_CAMERA_MODE,
	mDNIe_NAVI,
	mDNIe_GALLERY_MODE,
	mDNIe_VT_MODE,
	mDNIe_BROWSER_ON_MODE,
	mDNIe_EBOOK_MODE,
	mDNIe_EMAIL_MODE,
#if defined(CONFIG_TDMB)
	mDNIe_DMB_MODE = 20,
#endif
	MAX_mDNIe_MODE,
};

enum SCENARIO_COLOR_TONE {
	COLOR_TONE_1 = 40,
	COLOR_TONE_2,
	COLOR_TONE_3,
	COLOR_TONE_MAX,
};

enum eCurrent_Temp {
	TEMP_STANDARD = 0,
	TEMP_WARM,
	TEMP_COLD,
	MAX_TEMP_MODE,
};

enum eBackground_Mode {
	DYNAMIC_MODE = 0,
	STANDARD_MODE,
	MOVIE_MODE,
	AUTO_MODE,
	MAX_BACKGROUND_MODE,
};

enum eCabc_Mode {
	CABC_OFF_MODE = 0,
	CABC_ON_MODE,
	MAX_CABC_MODE,
};

enum eOutdoor_Mode {
	OUTDOOR_OFF_MODE = 0,
	OUTDOOR_ON_MODE,
	MAX_OUTDOOR_MODE,
};

enum POWER_LUT_LEVEL {
	LUT_LEVEL_MANUAL_AND_INDOOR,
	LUT_LEVEL_OUTDOOR_1,
	LUT_LEVEL_OUTDOOR_2,
	LUT_LEVEL_MAX,
};

enum POWER_LUT {
	LUT_DEFAULT,
	LUT_VIDEO,
	LUT_MAX,
};

enum eNegative_Mode {
	NEGATIVE_OFF_MODE = 0,
	NEGATIVE_ON_MODE,
	MAX_NEGATIVE_MODE,
};

enum mDNIe_mode_CABC_type {
	mode_type_CABC_none,
	mode_type_CABC_on,
	mode_type_CABC_off,
};

#if defined(CONFIG_FB_EBOOK_PANEL_SCENARIO)
enum eEbook_Mode {
	EBOOK_OFF = 0,
	EBOOK_ON,
	MAX_EBOOK_MODE,
};
#endif

enum ACCESSIBILITY {
		ACCESSIBILITY_OFF,
		NEGATIVE,
		COLOR_BLIND,
		ACCESSIBILITY_MAX,
};

struct str_sub_unit {
	char *name;
	const struct cmc624RegisterSet *value;
	int size;
};

/* For Accessibility */
struct str_blind_unit {
	char *name;
	struct cmc624RegisterSet *value;
	int size;
};

struct str_blind_tuning {
/* Array Index 0 : cabc off tuning value
Array Index 1 : cabc on tunning value */
	struct str_blind_unit value[MAX_CABC_MODE];
};

#define MDNIE_COLOR_BLIND_CMD	18

struct str_sub_tuning {
/* Array Index 0 : cabc off tuning value
Array Index 1 : cabc on tunning value */
	struct str_sub_unit value[MAX_CABC_MODE];
};

#define TUNE_FLAG_CABC_AUTO         0
#define TUNE_FLAG_CABC_ALWAYS_OFF   1
#define TUNE_FLAG_CABC_ALWAYS_ON    2


struct str_main_unit {
	char *name;
	int flag;
	const struct cmc624RegisterSet *tune;
	int plut;
	int size;
};

struct str_main_tuning {
/* Array Index 0 : cabc off tuning value
Array Index 1 : cabc on tunning value */
	struct str_main_unit value[MAX_CABC_MODE];
};

extern struct cmc624_state_type cmc624_state;
#define NUM_ITEM_POWER_LUT	9
#define NUM_POWER_LUT	2
#define LUT_LEVEL_MAX	3


#define CMC624_INITSEQ cmc624_init
/*#define CMC624_INITSEQ cmc624_bypass*/
#define CMC624_MAX_SETTINGS	 100
#define TUNING_FILE_PATH "/sdcard/tuning/"

#define PMIC_GPIO_LVDS_nSHDN 0	/*  LVDS_nSHDN GPIO on PM8058 is 19 */
#define PMIC_GPIO_BACKLIGHT_RST 51	/*  LVDS_nSHDN GPIO on PM8058 is 37 */
#define PMIC_GPIO_LCD_LEVEL_HIGH	1
#define PMIC_GPIO_LCD_LEVEL_LOW		0

/*
#define IMA_PWR_EN			13
#define IMA_nRST			16
#define IMA_SLEEP			17
#define IMA_CMC_EN			15
*/
#define MLCD_ON				128
#define IMA_I2C_SDA			42
#define IMA_I2C_SCL			43

#define CMC624_FAILSAFE		106
#define P8LTE_LCDON			1
#define P8LTE_LCDOFF		0

/*  BYPASS Mode OFF */
#define BYPASS_MODE_ON		0

struct cmc624_state_type {
	enum eCabc_Mode cabc_mode;
	unsigned int brightness;
	unsigned int suspended;
	unsigned int power_lut_idx;
	enum eLcd_mDNIe_UI scenario;
	enum SCENARIO_COLOR_TONE browser_scenario;
	enum eBackground_Mode background;
/*This value must reset to 0 (standard value) when change scenario*/
	enum eCurrent_Temp temperature;
	enum eOutdoor_Mode outdoor;
	enum ACCESSIBILITY negative;
#if defined(CONFIG_FB_EBOOK_PANEL_SCENARIO)
	enum eEbook_Mode ebook;
#endif
	enum ACCESSIBILITY blind;
	const struct str_sub_unit *sub_tune;
	const struct str_main_unit *main_tune;
};

/*  CMC624 function */
extern int cmc624_set_pwm_backlight(int level);
int cmc624_sysfs_init(void);
void bypass_onoff_ctrl(int value);
void cabc_onoff_ctrl(int value);
void set_backlight_pwm(int level);
int load_tuning_data(char *filename);
int apply_main_tune_value(enum eLcd_mDNIe_UI ui, enum eBackground_Mode bg,
			enum eCabc_Mode cabc, int force);
int apply_sub_tune_value(enum eCurrent_Temp temp, enum eOutdoor_Mode ove,
			enum eCabc_Mode cabc, int force);
int apply_browser_tune_value(enum SCENARIO_COLOR_TONE browser_mode, int force);
int apply_negative_tune_value(enum ACCESSIBILITY negative_mode,
			enum eCabc_Mode cabc);
#if defined(CONFIG_FB_EBOOK_PANEL_SCENARIO)
int apply_ebook_tune_value(enum eEbook_Mode ebook_mode,
			enum eCabc_Mode cabc);
#endif
int apply_blind_tune_value(
	enum ACCESSIBILITY blind_mode, enum eCabc_Mode cabc);
void dump_cmc624_register(void);
int samsung_cmc624_init(void);
/*int samsung_cmc624_setup(void);*/
int samsung_cmc624_on(int enable);
int samsung_cmc624_bypass_mode(void);
int samsung_cmc624_normal_mode(void);
int samsung_lvds_on(int enable);
int samsung_backlight_en(int enable);
void cmc624_manual_brightness(int bl_lvl);
bool samsung_has_cmc624(void);
void samsung_get_id(unsigned char *buf);
void Check_Prog(void);
#if defined(CONFIG_MIPI_SAMSUNG_ESD_REFRESH)
void Check_Cmc_Esd_Data(u16 *data);
#endif
void change_mon_clk(void);
void cmc624_Set_Region_Ext(int enable, int hStart, int hEnd, int vStart,
			   int vEnd);
int cmc624_set_pwm_backlight( int level);
void cmc624_pwm_control_cabc(int value255);
unsigned char LCD_Get_Value(void);
unsigned char LCD_ID3(void);
bool Is_4_8LCD_cmc(void);
bool Is_4_65LCD_cmc(void);
int cmc_power(int on);
extern struct cmc624_state_type cmc624_state;

