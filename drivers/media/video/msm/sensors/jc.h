/* Copyright (c) 2011, Code Aurora Forum. All rights reserved.
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

#ifndef __JC_H__
#define __JC_H__

#include <linux/types.h>
#include <mach/board.h>

#define	JC_DEBUG
#ifdef JC_DEBUG
#define CAM_DEBUG(fmt, arg...)	\
do {\
	printk(KERN_DEBUG "[JC] %s:" fmt "\n", \
	__func__, ##arg); } \
	while (0)

#define cam_info(fmt, arg...)	\
do {\
	printk(KERN_INFO "[JC] %s:" fmt "\n", __func__, ##arg); } \
	while (0)

#define cam_err(fmt, arg...)	\
do {\
	printk(KERN_ERR "[JC] %s:" fmt "\n", __func__, ##arg); } \
	while (0)

#define cam_i2c_dbg(fmt, arg...)	\
do { \
	printk(KERN_ERR "[JC] %s:" fmt "\n", __func__, ##arg); } \
	while (0)
#else
#define CAM_DEBUG(fmt, arg...)
#define cam_info(fmt, arg...)
#define cam_err(fmt, arg...)
#define cam_i2c_dbg(fmt, arg...)
#endif

#define FLASH_OFF		0
#define CAPTURE_FLASH	1
#define MOVIE_FLASH		2

struct jc_isp {
	wait_queue_head_t wait;
	unsigned int irq;	/* irq issued by ISP */
	unsigned int issued;
	unsigned int int_factor;
	unsigned int bad_fw:1;
};

u8 buf_port_seting0[] = {
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
		  0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,
		 };
u8 buf_port_seting1[] = {
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		 };
u8 buf_port_seting2[] = {
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C,
		  0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x10,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		 };
u8 buf_port_seting0_m10mo[] = {
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C,
		  0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x05,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
		 };
u8 buf_port_seting1_m10mo[] = {
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		 };
u8 buf_port_seting2_m10mo[] = {
		  0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x14,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		 };

#define JC_FW_VER_LEN		2
#define JC_FW_VER_STR_LEN  11
#define JC_SEN_FW_VER_LEN	30
#define JC_FW_VER_FILE_CUR	0x1FF080
#define JC_FW_VER_NUM		0x000018
#define JC_FW_VER_STR	       0x001EF080

#define JC_M9MO_FW_PATH		"/system/cameradata/RS_M9MO.bin"
#define JC_M10MO_FW_PATH		"/system/cameradata/RS_M10MO.bin"
#define JC_M9MO_FW_PATH_SD		"/mnt/shell/emulated/0/RS_M9MO.bin"
#define JC_M10MO_FW_PATH_SD		"/mnt/shell/emulated/0/RS_M10MO.bin"

#define JC_FW_REQ_PATH	"RS_M9MO.bin"
#define JC_EVT31_FW_REQ_PATH	"RS_M9MO_EVT3.1.bin"
#define FW_INFO_PATH		"/mnt/shell/emulated/0/FW_INFO.bin"
#define JC_FW_DUMP_PATH	"/data/M9MO_dump.bin"
#define JC_FACTORY_CSV_PATH	"/data/FACTORY_CSV_RAW.bin"
#define JC_SIO_LOADER_PATH_M9MO       "/system/cameradata/M9MO_SFW.bin"
#define JC_SIO_LOADER_PATH_M10MO       "/system/cameradata/M10MO_SFW.bin"

#define JC_FLASH_BASE_ADDR	0x00000000
#define JC_FLASH_READ_BASE_ADDR	0x000000
#define JC_FLASH_BASE_ADDR_1	0x001FF000
#define JC_INT_RAM_BASE_ADDR	0x01100000
#define JC_SIO_LOADER_BASE 0x01000100

#define JC_I2C_RETRY		5
#define JC_I2C_VERIFY		50
#define JC_I2C_SPI_VERIFY		500

/* Category */
#define JC_CATEGORY_SYS	0x00
#define JC_CATEGORY_PARM	0x01
#define JC_CATEGORY_MON	0x02
#define JC_CATEGORY_AE	0x03
#define JC_CATEGORY_NEW	0x04
#define JC_CATEGORY_WB	0x06
#define JC_CATEGORY_EXIF	0x07
#define JC_CATEGORY_OT    0x08
#define JC_CATEGORY_FD	0x09
#define JC_CATEGORY_LENS	0x0A
#define JC_CATEGORY_CAPPARM	0x0B
#define JC_CATEGORY_CAPCTRL	0x0C
#define JC_CATEGORY_TEST	0x0D
#define JC_CATEGORY_ADJST	0x0E
#define JC_CATEGORY_FLASH	0x0F    /* F/W update */

/* JC_CATEGORY_SYS: 0x00 */
#define JC_SYS_PJT_CODE	0x01
#define JC_SYS_VER_FW		0x02
#define JC_SYS_VER_HW		0x04
#define JC_SYS_VER_PARAM	0x06
#define JC_SYS_VER_AWB	0x08
#define JC_SYS_USER_VER	0x0A
#define JC_SYS_MODE		0x0B
#define JC_SYS_ESD_INT	0x0E

#define JC_SYS_INT_EN		0x10
#define JC_SYS_INT_FACTOR	0x1C
#define JC_SYS_FRAMESYNC_CNT	0x14
#define JC_SYS_LENS_TIMER	0x28


/* JC_CATEGORY_PARAM: 0x01 */
#define JC_PARM_OUT_SEL	0x00
#define JC_PARM_MON_SIZE	0x01
#define JC_PARM_EFFECT	0x0B
#define JC_PARM_FLEX_FPS	0x67
#define JC_PARM_HDMOVIE	0x32
#define JC_PARM_VIDEO_SNAP_IMG_TRANSFER_START 0x3A
#define JC_PARM_SEL_FRAME_VIDEO_SNAP 0x3B
#define JC_PARM_MON_MOVIE_SELECT	0x3C
#define JC_PARM_VSS_MODE 0x6E

/* JC_CATEGORY_MON: 0x02 */
#define JC_MON_ZOOM		0x01
#define JC_MON_HR_ZOOM    0x04
#define JC_MON_MON_REVERSE	0x05
#define JC_MON_MON_MIRROR	0x06
#define JC_MON_SHOT_REVERSE	0x07
#define JC_MON_SHOT_MIRROR	0x08
#define JC_MON_CFIXB		0x09
#define JC_MON_CFIXR		0x0A
#define JC_MON_COLOR_EFFECT	0x0B
#define JC_MON_CHROMA_LVL	0x0F
#define JC_MON_EDGE_LVL	0x11
#define JC_MON_EDGE_CTRL	0x20
#define JC_MON_POINT_COLOR	0x22
#define JC_MON_TONE_CTRL	0x25
#define JC_MON_START_VIDEO_SNAP_SHOT 0x56
#define JC_MON_VIDEO_SNAP_SHOT_FRAME_COUNT 0x57

/* JC_CATEGORY_AE: 0x03 */
#define JC_AE_LOCK		0x00
#define JC_AE_MODE		0x01
#define JC_AE_ISOSEL		0x05
#define JC_AE_FLICKER		0x06
#define JC_AE_INDEX		0x09
#define JC_AE_EP_MODE_MON	0x0A
#define JC_AE_EP_MODE_CAP	0x0B
#define JC_AF_AE_LOCK		0x0D
#define JC_AE_AUTO_BRACKET_EV	0x20
#define JC_AE_STABILITY	0x21
#define JC_AE_EV_PRG_MODE_CAP	0x34
#define JC_AE_EV_PRG_MODE_MON	0x35
#define JC_AE_EV_PRG_F_NUMBER	0x36
#define JC_AE_EV_PRG_SS_NUMERATOR		0x37
#define JC_AE_EV_PRG_SS_DENOMINATOR	0x39
#define JC_AE_EV_PRG_ISO_VALUE	0x3B
#define JC_AE_EV_PRG_F_NUMBER_MON				0x3D
#define JC_AE_EV_PRG_SS_NUMERATOR_MON		0x3E
#define JC_AE_EV_PRG_SS_DENOMINATOR_MON		0x40
#define JC_AE_EV_PRG_ISO_VALUE_MON			0x42
#define JC_AE_NOW_AV	0x54
#define JC_AE_NOW_TV	0x58
#define JC_AE_NOW_SV	0x5C
#define JC_AE_NOW_LV	0x52

/* JC_CATEGORY_NEW: 0x04 */
#define JC_NEW_TIME_INFO		0x02
#define JC_NEW_DETECT_SCENE	0x0B
#define JC_NEW_OIS_VERSION	0x1B

/* JC_CATEGORY_WB: 0x06 */
#define JC_AWB_LOCK		0x00
#define JC_WB_AWB_MODE	0x02
#define JC_WB_AWB_MANUAL	0x03
#define JC_WB_GBAM_MODE	0x8D
#define JC_WB_G_VALUE		0x8E
#define JC_WB_B_VALUE		0x8F
#define JC_WB_A_VALUE		0x90
#define JC_WB_M_VALUE		0x91
#define JC_WB_K_VALUE		0x92
#define JC_WB_CWB_MODE		0x93
#define JC_WB_SET_CUSTOM_RG	0x94
#define JC_WB_SET_CUSTOM_BG	0x96
#define JC_WB_GET_CUSTOM_RG	0x98
#define JC_WB_GET_CUSTOM_BG	0x9A
#define JC_WB_WBB_MODE		0x9C
#define JC_WB_WBB_AB		0x9D
#define JC_WB_WBB_GM		0x9E

/* JC_CATEGORY_EXIF: 0x07 */
#define JC_EXIF_EXPTIME_NUM	0x00
#define JC_EXIF_EXPTIME_DEN	0x04
#define JC_EXIF_TV_NUM	0x08
#define JC_EXIF_TV_DEN	0x0C
#define JC_EXIF_BV_NUM	0x18
#define JC_EXIF_BV_DEN	0x1C
#define JC_EXIF_EBV_NUM	0x20
#define JC_EXIF_EBV_DEN	0x24
#define JC_EXIF_ISO		0x28
#define JC_EXIF_FLASH		0x2A
#define JC_EXIF_AV_NUM	0x10
#define JC_EXIF_AV_DEN	0x14
#define JC_EXIF_FL	0x11
#define JC_EXIF_FL_35	0x13

/* JC_CATEGORY_OT: 0x08 */
#define JC_OT_TRACKING_CTL		0x00
#define JC_OT_INFO_READY			0x01
#define JC_OT_X_START_LOCATION	0x05
#define JC_OT_Y_START_LOCATION	0x07
#define JC_OT_X_END_LOCATION		0x09
#define JC_OT_Y_END_LOCATION		0x0B
#define JC_OT_TRACKING_X_LOCATION	0x10
#define JC_OT_TRACKING_Y_LOCATION	0x12
#define JC_OT_TRACKING_FRAME_WIDTH	0x14
#define JC_OT_TRACKING_FRAME_HEIGHT	0x16
#define JC_OT_TRACKING_STATUS		0x18
#define JC_OT_FRAME_WIDTH			0x30

/* JC_CATEGORY_FD: 0x09 */
#define JC_FD_CTL					0x00
#define JC_FD_SIZE				0x01
#define JC_FD_MAX					0x02
#define JC_FD_RED_EYE				0x55
#define JC_FD_RED_DET_STATUS		0x56
#define JC_FD_BLINK_FRAMENO		0x59
#define JC_FD_BLINK_LEVEL_1		0x5A
#define JC_FD_BLINK_LEVEL_2		0x5B
#define JC_FD_BLINK_LEVEL_3		0x5C

/* JC_CATEGORY_LENS: 0x0A */
#define JC_LENS_AF_INITIAL		0x00
#define JC_LENS_AF_LENS_CLOSE		0x01
#define JC_LENS_AF_ZOOM_CTRL		0x02
#define JC_LENS_AF_START_STOP		0x03
#define JC_LENS_AF_IRIS_STEP		0x05
#define JC_LENS_AF_ZOOM_LEVEL		0x06
#define JC_LENS_AF_SCAN_RANGE		0x07
#define JC_LENS_AF_MODE			0x08
#define JC_LENS_AF_WINDOW_MODE	0x09
#define JC_LENS_AF_BACKLASH_ADJ	0x0A
#define JC_LENS_AF_FOCUS_ADJ		0x0B
#define JC_LENS_AF_TILT_ADJ		0x0C
#define JC_LENS_AF_AF_ADJ			0x0D
#define JC_LENS_AF_PUNT_ADJ		0x0E
#define JC_LENS_AF_ZOOM_ADJ		0x0F
#define JC_LENS_AF_ADJ_TEMP_VALUE	0x0C
#define JC_LENS_AF_ALGORITHM		0x0D
#define JC_LENS_ZOOM_LEVEL_INFO	0x10
#define JC_LENS_AF_LED			0x1C
#define JC_LENS_AF_CAL			0x1D
#define JC_LENS_AF_RESULT			0x20
#define JC_LENS_ZOOM_SET_INFO		0x22
#define JC_LENS_ZOOM_SPEED		0x25
#define JC_LENS_ZOOM_STATUS		0x26
#define JC_LENS_AF_TOUCH_POSX		0x30
#define JC_LENS_AF_TOUCH_POSY		0x32
#define JC_LENS_AF_VERSION		0x60

/* JC_CATEGORY_CAPPARM: 0x0B */
#define JC_CAPPARM_YUVOUT_MAIN	0x00
#define JC_CAPPARM_MAIN_IMG_SIZE	0x01
#define JC_CAPPARM_YUVOUT_PREVIEW	0x05
#define JC_CAPPARM_PREVIEW_IMG_SIZE	0x06
#define JC_CAPPARM_YUVOUT_THUMB	0x0A
#define JC_CAPPARM_THUMB_IMG_SIZE	0x0B
#define JC_CAPPARM_JPEG_SIZE_MAX	0x0F
#define JC_CAPPARM_JPEG_SIZE_MIN	0x13
#define JC_CAPPARM_JPEG_RATIO		0x17
#define JC_CAPPARM_MCC_MODE		0x1D
#define JC_CAPPARM_STROBE_EN		0x22
#define JC_CAPPARM_STROBE_CHARGE	0x27
#define JC_CAPPARM_STROBE_EVC		0x28
#define JC_CAPPARM_STROBE_UP_DOWN	0x29
#define JC_CAPPARM_WDR_EN			0x2C
#define JC_CAPPARM_JPEG_RATIO_OFS	0x1B
#define JC_CAPPARM_THUMB_JPEG_MAX	0x3C
#define JC_CAPPARM_STROBE_BATT_INFO	0x3F
#define JC_CAPPARM_AFB_CAP_EN		0x53

/* JC_CATEGORY_CAPCTRL: 0x0C */
#define JC_CAPCTRL_CAP_MODE	0x00
#define JC_CAPCTRL_CAP_FRM_INTERVAL 0x01
#define JC_CAPCTRL_CAP_FRM_COUNT 0x02
#define JC_CAPCTRL_START_DUALCAP 0x05
#define JC_CAPCTRL_FRM_SEL	0x06
#define JC_CAPCTRL_FRM_PRV_SEL	0x07
#define JC_CAPCTRL_FRM_THUMB_SEL	0x08
#define JC_CAPCTRL_TRANSFER	0x09
#define JC_CAPCTRL_IMG_SIZE	0x0D
#define JC_CAPCTRL_THUMB_SIZE	0x11

/* JC_CATEGORY_CAPCTRL: 0x0C  JC_CAPCTRL_CAP_MODE: 0x00 */
#define JC_CAP_MODE_SINGLE_CAPTURE		(0x00)
#define JC_CAP_MODE_MULTI_CAPTURE			(0x01)
#define JC_CAP_MODE_DUAL_CAPTURE			(0x05)
#define JC_CAP_MODE_BRACKET_CAPTURE		(0x06)
#define JC_CAP_MODE_ADDPIXEL_CAPTURE		(0x08)
#define JC_CAP_MODE_PANORAMA_CAPTURE		(0x0B)
#define JC_CAP_MODE_BLINK_CAPTURE			(0x0C)
#define JC_CAP_MODE_RAW			(0x0D)

/* JC_CATEGORY_ADJST: 0x0E */
#define JC_ADJST_SHUTTER_MODE	0x33
#define JC_ADJST_AWB_RG_H	0x3C
#define JC_ADJST_AWB_RG_L	0x3D
#define JC_ADJST_AWB_BG_H	0x3E
#define JC_ADJST_AWB_BG_L	0x3F

/* JC_CATEGORY_FLASH: 0x0F */
#define JC_FLASH_ADDR		0x00
#define JC_FLASH_BYTE		0x04
#define JC_FLASH_ERASE	0x06
#define JC_FLASH_WR		0x07
#define JC_FLASH_RAM_CLEAR	0x08
#define JC_FLASH_CAM_START	0x12
#define JC_FLASH_SEL		0x13

/* JC_CATEGORY_TEST:	0x0D */
#define JC_TEST_ADD_SHOW		0x06
#define JC_TEST_LOG_STR_ADD	0x08
#define JC_TEST_LOG_SEL			0x0C
#define JC_TEST_LOG_ACT		0x0E
#define JC_TEST_LOG_MODE		0x0F
#define JC_TEST_LOG_STR_LEN	0x14
#define JC_TEST_OUTPUT_YCO_TEST_DATA	0x1B
#define JC_TEST_ISP_PROCESS		0x59

/* JC Sensor Mode */
#define JC_SYSINIT_MODE	0x0
#define JC_PARMSET_MODE	0x1
#define JC_MONITOR_MODE	0x2
#define JC_STILLCAP_MODE	0x3

/* Interrupt Factor */
#define JC_INT_SOUND		(1 << 15)
#define JC_INT_LENS_INIT	(1 << 14)
#define JC_INT_FD		(1 << 13)
#define JC_INT_FRAME_SYNC	(1 << 12)
#define JC_INT_CAPTURE	(1 << 11)
#define JC_INT_ZOOM		(1 << 10)
#define JC_INT_AF		(1 << 9)
#define JC_INT_MODE		(1 << 8)
#define JC_INT_ATSCENE	(1 << 7)
#define JC_INT_ATSCENE_UPDATE	(1 << 6)
#define JC_INT_AF_STATUS		(1 << 5)
#define JC_INT_OIS_SET	(1 << 4)
#define JC_INT_OIS_INIT	(1 << 3)
#define JC_INT_STNW_DETECT	(1 << 2)
#define JC_INT_SCENARIO_FIN	(1 << 1)
#define JC_INT_PRINT	(1 << 0)

/* ESD Interrupt */
#define JC_INT_ESD		(1 << 0)


/* level at or below which we need to enable flash when in auto mode */
#define LOW_LIGHT_LEVEL		0x20

#define LOWLIGHT_DEFAULT	0x002B	/*for tuning*/
#define LOWLIGHT_SCENE_NIGHT	0x003D	/*for night mode*/
#define LOWLIGHT_ISO50		0xB52A	/*for tuning*/
#define LOWLIGHT_ISO100		0x9DBA	/*for tuning*/
#define LOWLIGHT_ISO200		0x864A	/*for tuning*/
#define LOWLIGHT_ISO400		0x738A	/*for tuning*/

#define LOWLIGHT_EV_P4		0x003B
#define LOWLIGHT_EV_P3		0x0037
#define LOWLIGHT_EV_P2		0x0033
#define LOWLIGHT_EV_P1		0x002F
#define LOWLIGHT_EV_M1		0x0026
#define LOWLIGHT_EV_M2		0x0021
#define LOWLIGHT_EV_M3		0x001C
#define LOWLIGHT_EV_M4		0x0014

/* for flash */
#define ERRSCL_AUTO			0x01CA	/*for tuning*/
#define USER_AESCL_AUTO		0x01CE	/*for tuning*/
#define ERRSCL_NOW			0x01CC	/*for tuning*/
#define USER_AESCL_NOW		0x01D0	/*for tuning*/

#define CAP_GAINOFFSET		0x0186	/*for tuning*/

/* Other models use these defined in jc_regs_v2.h */
#ifdef CONFIG_MACH_JAGUAR
#define AE_OFSETVAL			3450	/*for tuning max 5.1times*/
#define AE_MAXDIFF			5000	/*for tuning max =< 5000*/
#endif

#define JC_DELAY_RETRIES 100/*for modesel_fix, awbsts, half_move_sts*/

/* preview size idx*/
#define PREVIEW_SIZE_VGA    0    /* 640x480 */
#define PREVIEW_SIZE_D1     1    /* 720x480 */
#define PREVIEW_SIZE_WVGA   2    /* 800x480 */
#define PREVIEW_SIZE_XGA    3    /* 1024x768*/
#define PREVIEW_SIZE_HD     4    /* 1280x720*/
#define PREVIEW_SIZE_FHD    5    /* 1920x1080*/
#define PREVIEW_SIZE_MMS    6    /* 528x432 */
#define PREVIEW_SIZE_QCIF   8    /* 176x144 */


/* DTP */
#define DTP_OFF				0
#define DTP_ON				1
#define DTP_OFF_ACK			2
#define DTP_ON_ACK			3

#define PREVIEW_MODE		0
#define MOVIE_MODE			1

#define SHUTTER_AF_MODE		0
#define TOUCH_AF_MODE		1

struct jc_userset {
	unsigned int	focus_mode;
	unsigned int	focus_status;
	unsigned int	continuous_af;

	unsigned int	metering;
	unsigned int	exposure;
	unsigned int	wb;
	unsigned int	iso;
	int				contrast;
	int				saturation;
	int				sharpness;
	int				brightness;
	int				ev;
	int				scene;
	unsigned int	zoom;
	unsigned int	effect;		/* Color FX (AKA Color tone) */
	unsigned int	scenemode;
	unsigned int	detectmode;
	unsigned int	antishake;
	unsigned int	fps;
	unsigned int	flash_mode;
	unsigned int	flash_state;
	unsigned int	stabilize;	/* IS */
	unsigned int	strobe;
	unsigned int	jpeg_quality;
	/*unsigned int preview_size;*/
	unsigned int	preview_size_idx;
	unsigned int	capture_size;
	unsigned int	thumbnail_size;
};


/*extern struct jc_reg jc_regs;*/
struct reg_struct_init {
    /* PLL setting */
	uint8_t pre_pll_clk_div;	/* 0x0305 */
	uint8_t plstatim;			/* 0x302b */
	uint8_t reg_3024;			/* 0x3024 */
	uint8_t image_orientation;	/* 0x0101 */
	uint8_t vndmy_ablmgshlmt;	/* 0x300a */
	uint8_t y_opbaddr_start_di; /* 0x3014 */
	uint8_t reg_0x3015;			/* 0x3015 */
	uint8_t reg_0x301c;			/* 0x301c */
	uint8_t reg_0x302c;			/* 0x302c */
	uint8_t reg_0x3031;			/* 0x3031 */
	uint8_t reg_0x3041;			/* 0x3041 */
	uint8_t reg_0x3051;			/* 0x3051 */
	uint8_t reg_0x3053;			/* 0x3053 */
	uint8_t reg_0x3057;			/* 0x3057 */
	uint8_t reg_0x305c;			/* 0x305c */
	uint8_t reg_0x305d;			/* 0x305d */
	uint8_t reg_0x3060;			/* 0x3060 */
	uint8_t reg_0x3065;			/* 0x3065 */
	uint8_t reg_0x30aa;			/* 0x30aa */
	uint8_t reg_0x30ab;
	uint8_t reg_0x30b0;
	uint8_t reg_0x30b2;
	uint8_t reg_0x30d3;
	uint8_t reg_0x3106;
	uint8_t reg_0x310c;
	uint8_t reg_0x3304;
	uint8_t reg_0x3305;
	uint8_t reg_0x3306;
	uint8_t reg_0x3307;
	uint8_t reg_0x3308;
	uint8_t reg_0x3309;
	uint8_t reg_0x330a;
	uint8_t reg_0x330b;
	uint8_t reg_0x330c;
	uint8_t reg_0x330d;
	uint8_t reg_0x330f;
	uint8_t reg_0x3381;
};

struct reg_struct {
	uint8_t pll_multiplier;			/* 0x0307 */
	uint8_t frame_length_lines_hi;	/* 0x0340 */
	uint8_t frame_length_lines_lo;	/* 0x0341 */
	uint8_t y_addr_start;			/* 0x0347 */
	uint8_t y_add_end;				/* 0x034b */
	uint8_t x_output_size_msb;		/* 0x034c */
	uint8_t x_output_size_lsb;		/* 0x034d */
	uint8_t y_output_size_msb;		/* 0x034e */
	uint8_t y_output_size_lsb;		/* 0x034f */
	uint8_t x_even_inc;				/* 0x0381 */
	uint8_t x_odd_inc;				/* 0x0383 */
	uint8_t y_even_inc;				/* 0x0385 */
	uint8_t y_odd_inc;				/* 0x0387 */
	uint8_t hmodeadd;				/* 0x3001 */
	uint8_t vmodeadd;				/* 0x3016 */
	uint8_t vapplinepos_start;		/* 0x3069 */
	uint8_t vapplinepos_end;		/* 0x306b */
	uint8_t shutter;				/* 0x3086 */
	uint8_t haddave;				/* 0x30e8 */
	uint8_t lanesel;				/* 0x3301 */
};

struct jc_i2c_reg_conf {
	unsigned short waddr;
	unsigned short wdata;
};

enum jc_test_mode_t {
	TEST_OFF,
	TEST_1,
	TEST_2,
	TEST_3
};

enum jc_resolution_t {
	QTR_SIZE,
	FULL_SIZE,
	INVALID_SIZE
};
enum jc_setting {
	RES_PREVIEW,
	RES_CAPTURE
};
enum mt9p012_reg_update {
	/* Sensor egisters that need to be updated during initialization */
	REG_INIT,
	/* Sensor egisters that needs periodic I2C writes */
	UPDATE_PERIODIC,
	/* All the sensor Registers will be updated */
	UPDATE_ALL,
	/* Not valid update */
	UPDATE_INVALID
};
/*
struct jc_reg {
	const struct reg_struct_init  *reg_pat_init;
	const struct reg_struct  *reg_pat;
};
*/

extern u8 torchonoff;

extern int jc_spi_read(u8 *buf, size_t len, const int rxSize);
extern int jc_spi_write(const u8 *addr, const int len, const int txSize);
extern int jc_spi_init(void);

#endif /* __JC_H__ */
