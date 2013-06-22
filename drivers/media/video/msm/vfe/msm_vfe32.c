/* Copyright (c) 2011-2012, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/atomic.h>
#include <linux/regulator/consumer.h>
#include <linux/clk.h>
#include <mach/irqs.h>
#include <mach/camera.h>
#include <media/v4l2-device.h>
#include <media/v4l2-subdev.h>
#include <media/msm_isp.h>

#include "msm.h"
#include "msm_cam_server.h"
#include "msm_vfe32.h"
#if defined(CONFIG_MACH_JACTIVE_ATT) || defined(CONFIG_MACH_JACTIVE_EUR)
#include "msm_fb.h"
#include "mdp4.h"
#endif

atomic_t irq_cnt;
extern unsigned int open_fail_flag;

//#define FIXED_WM 1

#define VFE32_AXI_OFFSET 0x0050
#define vfe32_get_ch_ping_addr(base, chn) \
	(msm_camera_io_r((base) + 0x0050 + 0x18 * (chn)))
#define vfe32_get_ch_pong_addr(base, chn) \
	(msm_camera_io_r((base) + 0x0050 + 0x18 * (chn) + 4))
#define vfe32_get_ch_addr(ping_pong, base, chn) \
	((((ping_pong) & (1 << (chn))) == 0) ? \
	(vfe32_get_ch_pong_addr((base), chn)) : \
	(vfe32_get_ch_ping_addr((base), chn)))

#define vfe32_put_ch_ping_addr(base, chn, addr) \
	(msm_camera_io_w((addr), (base) + 0x0050 + 0x18 * (chn)))
#define vfe32_put_ch_pong_addr(base, chn, addr) \
	(msm_camera_io_w((addr), (base) + 0x0050 + 0x18 * (chn) + 4))
#define vfe32_put_ch_addr(ping_pong, base, chn, addr) \
	(((ping_pong) & (1 << (chn))) == 0 ?   \
	vfe32_put_ch_pong_addr((base), (chn), (addr)) : \
	vfe32_put_ch_ping_addr((base), (chn), (addr)))

static uint32_t vfe_clk_rate;
static void vfe32_send_isp_msg(struct v4l2_subdev *sd,
	uint32_t vfeFrameId, uint32_t isp_msg_id);


struct vfe32_isr_queue_cmd {
	struct list_head list;
	uint32_t                           vfeInterruptStatus0;
	uint32_t                           vfeInterruptStatus1;
};

static struct vfe32_cmd_type vfe32_cmd[] = {
/* 0*/	{VFE_CMD_DUMMY_0},
		{VFE_CMD_SET_CLK},
		{VFE_CMD_RESET},
		{VFE_CMD_START},
		{VFE_CMD_TEST_GEN_START},
/* 5*/	{VFE_CMD_OPERATION_CFG, V32_OPERATION_CFG_LEN},
		{VFE_CMD_AXI_OUT_CFG, V32_AXI_OUT_LEN, V32_AXI_OUT_OFF, 0xFF},
		{VFE_CMD_CAMIF_CFG, V32_CAMIF_LEN, V32_CAMIF_OFF, 0xFF},
		{VFE_CMD_AXI_INPUT_CFG},
		{VFE_CMD_BLACK_LEVEL_CFG, V32_BLACK_LEVEL_LEN,
		V32_BLACK_LEVEL_OFF,
		0xFF},
/*10*/  {VFE_CMD_MESH_ROLL_OFF_CFG, V32_MESH_ROLL_OFF_CFG_LEN,
		V32_MESH_ROLL_OFF_CFG_OFF, 0xFF},
		{VFE_CMD_DEMUX_CFG, V32_DEMUX_LEN, V32_DEMUX_OFF, 0xFF},
		{VFE_CMD_FOV_CFG, V32_FOV_LEN, V32_FOV_OFF, 0xFF},
		{VFE_CMD_MAIN_SCALER_CFG, V32_MAIN_SCALER_LEN,
		V32_MAIN_SCALER_OFF, 0xFF},
		{VFE_CMD_WB_CFG, V32_WB_LEN, V32_WB_OFF, 0xFF},
/*15*/	{VFE_CMD_COLOR_COR_CFG, V32_COLOR_COR_LEN, V32_COLOR_COR_OFF, 0xFF},
		{VFE_CMD_RGB_G_CFG, V32_RGB_G_LEN, V32_RGB_G_OFF, 0xFF},
		{VFE_CMD_LA_CFG, V32_LA_LEN, V32_LA_OFF, 0xFF },
		{VFE_CMD_CHROMA_EN_CFG, V32_CHROMA_EN_LEN, V32_CHROMA_EN_OFF,
		0xFF},
		{VFE_CMD_CHROMA_SUP_CFG, V32_CHROMA_SUP_LEN, V32_CHROMA_SUP_OFF,
		0xFF},
/*20*/	{VFE_CMD_MCE_CFG, V32_MCE_LEN, V32_MCE_OFF, 0xFF},
		{VFE_CMD_SK_ENHAN_CFG, V32_SCE_LEN, V32_SCE_OFF, 0xFF},
		{VFE_CMD_ASF_CFG, V32_ASF_LEN, V32_ASF_OFF, 0xFF},
		{VFE_CMD_S2Y_CFG, V32_S2Y_LEN, V32_S2Y_OFF, 0xFF},
		{VFE_CMD_S2CbCr_CFG, V32_S2CbCr_LEN, V32_S2CbCr_OFF, 0xFF},
/*25*/	{VFE_CMD_CHROMA_SUBS_CFG, V32_CHROMA_SUBS_LEN, V32_CHROMA_SUBS_OFF,
		0xFF},
		{VFE_CMD_OUT_CLAMP_CFG, V32_OUT_CLAMP_LEN, V32_OUT_CLAMP_OFF,
		0xFF},
		{VFE_CMD_FRAME_SKIP_CFG, V32_FRAME_SKIP_LEN, V32_FRAME_SKIP_OFF,
		0xFF},
		{VFE_CMD_DUMMY_1},
		{VFE_CMD_DUMMY_2},
/*30*/	{VFE_CMD_DUMMY_3},
		{VFE_CMD_UPDATE},
		{VFE_CMD_BL_LVL_UPDATE, V32_BLACK_LEVEL_LEN,
		V32_BLACK_LEVEL_OFF, 0xFF},
		{VFE_CMD_DEMUX_UPDATE, V32_DEMUX_LEN, V32_DEMUX_OFF, 0xFF},
		{VFE_CMD_FOV_UPDATE, V32_FOV_LEN, V32_FOV_OFF, 0xFF},
/*35*/	{VFE_CMD_MAIN_SCALER_UPDATE, V32_MAIN_SCALER_LEN, V32_MAIN_SCALER_OFF,
		0xFF},
		{VFE_CMD_WB_UPDATE, V32_WB_LEN, V32_WB_OFF, 0xFF},
		{VFE_CMD_COLOR_COR_UPDATE, V32_COLOR_COR_LEN, V32_COLOR_COR_OFF,
		0xFF},
		{VFE_CMD_RGB_G_UPDATE, V32_RGB_G_LEN, V32_CHROMA_EN_OFF, 0xFF},
		{VFE_CMD_LA_UPDATE, V32_LA_LEN, V32_LA_OFF, 0xFF },
/*40*/	{VFE_CMD_CHROMA_EN_UPDATE, V32_CHROMA_EN_LEN, V32_CHROMA_EN_OFF,
		0xFF},
		{VFE_CMD_CHROMA_SUP_UPDATE, V32_CHROMA_SUP_LEN,
		V32_CHROMA_SUP_OFF, 0xFF},
		{VFE_CMD_MCE_UPDATE, V32_MCE_LEN, V32_MCE_OFF, 0xFF},
		{VFE_CMD_SK_ENHAN_UPDATE, V32_SCE_LEN, V32_SCE_OFF, 0xFF},
		{VFE_CMD_S2CbCr_UPDATE, V32_S2CbCr_LEN, V32_S2CbCr_OFF, 0xFF},
/*45*/	{VFE_CMD_S2Y_UPDATE, V32_S2Y_LEN, V32_S2Y_OFF, 0xFF},
		{VFE_CMD_ASF_UPDATE, V32_ASF_UPDATE_LEN, V32_ASF_OFF, 0xFF},
		{VFE_CMD_FRAME_SKIP_UPDATE},
		{VFE_CMD_CAMIF_FRAME_UPDATE},
		{VFE_CMD_STATS_AF_UPDATE, V32_STATS_AF_LEN, V32_STATS_AF_OFF},
/*50*/	{VFE_CMD_STATS_AE_UPDATE, V32_STATS_AE_LEN, V32_STATS_AE_OFF},
		{VFE_CMD_STATS_AWB_UPDATE, V32_STATS_AWB_LEN,
		V32_STATS_AWB_OFF},
		{VFE_CMD_STATS_RS_UPDATE, V32_STATS_RS_LEN, V32_STATS_RS_OFF},
		{VFE_CMD_STATS_CS_UPDATE, V32_STATS_CS_LEN, V32_STATS_CS_OFF},
		{VFE_CMD_STATS_SKIN_UPDATE},
/*55*/	{VFE_CMD_STATS_IHIST_UPDATE, V32_STATS_IHIST_LEN, V32_STATS_IHIST_OFF},
		{VFE_CMD_DUMMY_4},
		{VFE_CMD_EPOCH1_ACK},
		{VFE_CMD_EPOCH2_ACK},
		{VFE_CMD_START_RECORDING},
/*60*/	{VFE_CMD_STOP_RECORDING},
		{VFE_CMD_DUMMY_5},
		{VFE_CMD_DUMMY_6},
		{VFE_CMD_CAPTURE, V32_CAPTURE_LEN, 0xFF},
		{VFE_CMD_DUMMY_7},
/*65*/	{VFE_CMD_STOP},
		{VFE_CMD_GET_HW_VERSION, V32_GET_HW_VERSION_LEN,
		V32_GET_HW_VERSION_OFF},
		{VFE_CMD_GET_FRAME_SKIP_COUNTS},
		{VFE_CMD_OUTPUT1_BUFFER_ENQ},
		{VFE_CMD_OUTPUT2_BUFFER_ENQ},
/*70*/	{VFE_CMD_OUTPUT3_BUFFER_ENQ},
		{VFE_CMD_JPEG_OUT_BUF_ENQ},
		{VFE_CMD_RAW_OUT_BUF_ENQ},
		{VFE_CMD_RAW_IN_BUF_ENQ},
		{VFE_CMD_STATS_AF_ENQ},
/*75*/	{VFE_CMD_STATS_AE_ENQ},
		{VFE_CMD_STATS_AWB_ENQ},
		{VFE_CMD_STATS_RS_ENQ},
		{VFE_CMD_STATS_CS_ENQ},
		{VFE_CMD_STATS_SKIN_ENQ},
/*80*/	{VFE_CMD_STATS_IHIST_ENQ},
		{VFE_CMD_DUMMY_8},
		{VFE_CMD_JPEG_ENC_CFG},
		{VFE_CMD_DUMMY_9},
		{VFE_CMD_STATS_AF_START, V32_STATS_AF_LEN, V32_STATS_AF_OFF},
/*85*/	{VFE_CMD_STATS_AF_STOP},
		{VFE_CMD_STATS_AE_START, V32_STATS_AE_LEN, V32_STATS_AE_OFF},
		{VFE_CMD_STATS_AE_STOP},
		{VFE_CMD_STATS_AWB_START, V32_STATS_AWB_LEN, V32_STATS_AWB_OFF},
		{VFE_CMD_STATS_AWB_STOP},
/*90*/	{VFE_CMD_STATS_RS_START, V32_STATS_RS_LEN, V32_STATS_RS_OFF},
		{VFE_CMD_STATS_RS_STOP},
		{VFE_CMD_STATS_CS_START, V32_STATS_CS_LEN, V32_STATS_CS_OFF},
		{VFE_CMD_STATS_CS_STOP},
		{VFE_CMD_STATS_SKIN_START},
/*95*/	{VFE_CMD_STATS_SKIN_STOP},
		{VFE_CMD_STATS_IHIST_START,
		V32_STATS_IHIST_LEN, V32_STATS_IHIST_OFF},
		{VFE_CMD_STATS_IHIST_STOP},
		{VFE_CMD_DUMMY_10},
		{VFE_CMD_SYNC_TIMER_SETTING, V32_SYNC_TIMER_LEN,
			V32_SYNC_TIMER_OFF},
/*100*/	{VFE_CMD_ASYNC_TIMER_SETTING, V32_ASYNC_TIMER_LEN, V32_ASYNC_TIMER_OFF},
		{VFE_CMD_LIVESHOT},
		{VFE_CMD_LA_SETUP},
		{VFE_CMD_LINEARIZATION_CFG, V32_LINEARIZATION_LEN1,
			V32_LINEARIZATION_OFF1},
		{VFE_CMD_DEMOSAICV3},
/*105*/	{VFE_CMD_DEMOSAICV3_ABCC_CFG},
		{VFE_CMD_DEMOSAICV3_DBCC_CFG, V32_DEMOSAICV3_DBCC_LEN,
			V32_DEMOSAICV3_DBCC_OFF},
		{VFE_CMD_DEMOSAICV3_DBPC_CFG},
		{VFE_CMD_DEMOSAICV3_ABF_CFG, V32_DEMOSAICV3_ABF_LEN,
			V32_DEMOSAICV3_ABF_OFF},
		{VFE_CMD_DEMOSAICV3_ABCC_UPDATE},
/*110*/	{VFE_CMD_DEMOSAICV3_DBCC_UPDATE, V32_DEMOSAICV3_DBCC_LEN,
			V32_DEMOSAICV3_DBCC_OFF},
		{VFE_CMD_DEMOSAICV3_DBPC_UPDATE},
		{VFE_CMD_XBAR_CFG},
		{VFE_CMD_MODULE_CFG, V32_MODULE_CFG_LEN, V32_MODULE_CFG_OFF},
		{VFE_CMD_ZSL},
/*115*/	{VFE_CMD_LINEARIZATION_UPDATE, V32_LINEARIZATION_LEN1,
			V32_LINEARIZATION_OFF1},
		{VFE_CMD_DEMOSAICV3_ABF_UPDATE, V32_DEMOSAICV3_ABF_LEN,
			V32_DEMOSAICV3_ABF_OFF},
		{VFE_CMD_CLF_CFG, V32_CLF_CFG_LEN, V32_CLF_CFG_OFF},
		{VFE_CMD_CLF_LUMA_UPDATE, V32_CLF_LUMA_UPDATE_LEN,
			V32_CLF_LUMA_UPDATE_OFF},
		{VFE_CMD_CLF_CHROMA_UPDATE, V32_CLF_CHROMA_UPDATE_LEN,
			V32_CLF_CHROMA_UPDATE_OFF},
/*120*/ {VFE_CMD_PCA_ROLL_OFF_CFG},
		{VFE_CMD_PCA_ROLL_OFF_UPDATE},
		{VFE_CMD_GET_REG_DUMP},
		{VFE_CMD_GET_LINEARIZATON_TABLE},
		{VFE_CMD_GET_MESH_ROLLOFF_TABLE},
/*125*/ {VFE_CMD_GET_PCA_ROLLOFF_TABLE},
		{VFE_CMD_GET_RGB_G_TABLE},
		{VFE_CMD_GET_LA_TABLE},
		{VFE_CMD_DEMOSAICV3_UPDATE},
		{VFE_CMD_ACTIVE_REGION_CFG},
/*130*/ {VFE_CMD_COLOR_PROCESSING_CONFIG},
		{VFE_CMD_STATS_WB_AEC_CONFIG},
		{VFE_CMD_STATS_WB_AEC_UPDATE},
		{VFE_CMD_Y_GAMMA_CONFIG},
		{VFE_CMD_SCALE_OUTPUT1_CONFIG},
/*135*/ {VFE_CMD_SCALE_OUTPUT2_CONFIG},
		{VFE_CMD_CAPTURE_RAW},
		{VFE_CMD_STOP_LIVESHOT},
		{VFE_CMD_RECONFIG_VFE},
		{VFE_CMD_STATS_REQBUF},
/*140*/	{VFE_CMD_STATS_ENQUEUEBUF},
		{VFE_CMD_STATS_FLUSH_BUFQ},
		{VFE_CMD_STATS_UNREGBUF},
		{VFE_CMD_STATS_BG_START, V32_STATS_BG_LEN, V32_STATS_BG_OFF},
		{VFE_CMD_STATS_BG_STOP},
/*145*/	{VFE_CMD_STATS_BF_START, V32_STATS_BF_LEN, V32_STATS_BF_OFF},
		{VFE_CMD_STATS_BF_STOP},
/*147*/	{VFE_CMD_STATS_BHIST_START, V32_STATS_BHIST_LEN,
			V32_STATS_BHIST_OFF},
/*148*/	{VFE_CMD_STATS_BHIST_STOP},
		{VFE_CMD_RESET_2},
/*150*/	{VFE_CMD_FOV_ENC_CFG},
		{VFE_CMD_FOV_VIEW_CFG},
		{VFE_CMD_FOV_ENC_UPDATE},
		{VFE_CMD_FOV_VIEW_UPDATE},
		{VFE_CMD_SCALER_ENC_CFG},
/*155*/	{VFE_CMD_SCALER_VIEW_CFG},
		{VFE_CMD_SCALER_ENC_UPDATE},
		{VFE_CMD_SCALER_VIEW_UPDATE},
		{VFE_CMD_COLORXFORM_ENC_CFG},
		{VFE_CMD_COLORXFORM_VIEW_CFG},
/*160*/	{VFE_CMD_COLORXFORM_ENC_UPDATE},
		{VFE_CMD_COLORXFORM_VIEW_UPDATE},
		{VFE_CMD_TEST_GEN_CFG},
		{VFE_CMD_SELECT_RDI},
		{VFE_CMD_SET_STATS_VER},
		{VFE_CMD_STOP_RECORDING_DONE},
		{VFE_CMD_RGB_ALL_CFG},
		{VFE_CMD_RGB_ALL_UPDATE},
};

uint32_t vfe32_AXI_WM_CFG[] = {
	0x0000004C,
	0x00000064,
	0x0000007C,
	0x00000094,
	0x000000AC,
	0x000000C4,
	0x000000DC,
};

static const char * const vfe32_general_cmd[] = {
	"DUMMY_0",  /* 0 */
	"SET_CLK",
	"RESET",
	"START",
	"TEST_GEN_START",
	"OPERATION_CFG",  /* 5 */
	"AXI_OUT_CFG",
	"CAMIF_CFG",
	"AXI_INPUT_CFG",
	"BLACK_LEVEL_CFG",
	"ROLL_OFF_CFG",  /* 10 */
	"DEMUX_CFG",
	"FOV_CFG",
	"MAIN_SCALER_CFG",
	"WB_CFG",
	"COLOR_COR_CFG", /* 15 */
	"RGB_G_CFG",
	"LA_CFG",
	"CHROMA_EN_CFG",
	"CHROMA_SUP_CFG",
	"MCE_CFG", /* 20 */
	"SK_ENHAN_CFG",
	"ASF_CFG",
	"S2Y_CFG",
	"S2CbCr_CFG",
	"CHROMA_SUBS_CFG",  /* 25 */
	"OUT_CLAMP_CFG",
	"FRAME_SKIP_CFG",
	"DUMMY_1",
	"DUMMY_2",
	"DUMMY_3",  /* 30 */
	"UPDATE",
	"BL_LVL_UPDATE",
	"DEMUX_UPDATE",
	"FOV_UPDATE",
	"MAIN_SCALER_UPDATE",  /* 35 */
	"WB_UPDATE",
	"COLOR_COR_UPDATE",
	"RGB_G_UPDATE",
	"LA_UPDATE",
	"CHROMA_EN_UPDATE",  /* 40 */
	"CHROMA_SUP_UPDATE",
	"MCE_UPDATE",
	"SK_ENHAN_UPDATE",
	"S2CbCr_UPDATE",
	"S2Y_UPDATE",  /* 45 */
	"ASF_UPDATE",
	"FRAME_SKIP_UPDATE",
	"CAMIF_FRAME_UPDATE",
	"STATS_AF_UPDATE",
	"STATS_AE_UPDATE",  /* 50 */
	"STATS_AWB_UPDATE",
	"STATS_RS_UPDATE",
	"STATS_CS_UPDATE",
	"STATS_SKIN_UPDATE",
	"STATS_IHIST_UPDATE",  /* 55 */
	"DUMMY_4",
	"EPOCH1_ACK",
	"EPOCH2_ACK",
	"START_RECORDING",
	"STOP_RECORDING",  /* 60 */
	"DUMMY_5",
	"DUMMY_6",
	"CAPTURE",
	"DUMMY_7",
	"STOP",  /* 65 */
	"GET_HW_VERSION",
	"GET_FRAME_SKIP_COUNTS",
	"OUTPUT1_BUFFER_ENQ",
	"OUTPUT2_BUFFER_ENQ",
	"OUTPUT3_BUFFER_ENQ",  /* 70 */
	"JPEG_OUT_BUF_ENQ",
	"RAW_OUT_BUF_ENQ",
	"RAW_IN_BUF_ENQ",
	"STATS_AF_ENQ",
	"STATS_AE_ENQ",  /* 75 */
	"STATS_AWB_ENQ",
	"STATS_RS_ENQ",
	"STATS_CS_ENQ",
	"STATS_SKIN_ENQ",
	"STATS_IHIST_ENQ",  /* 80 */
	"DUMMY_8",
	"JPEG_ENC_CFG",
	"DUMMY_9",
	"STATS_AF_START",
	"STATS_AF_STOP",  /* 85 */
	"STATS_AE_START",
	"STATS_AE_STOP",
	"STATS_AWB_START",
	"STATS_AWB_STOP",
	"STATS_RS_START",  /* 90 */
	"STATS_RS_STOP",
	"STATS_CS_START",
	"STATS_CS_STOP",
	"STATS_SKIN_START",
	"STATS_SKIN_STOP",  /* 95 */
	"STATS_IHIST_START",
	"STATS_IHIST_STOP",
	"DUMMY_10",
	"SYNC_TIMER_SETTING",
	"ASYNC_TIMER_SETTING",  /* 100 */
	"LIVESHOT",
	"LA_SETUP",
	"LINEARIZATION_CFG",
	"DEMOSAICV3",
	"DEMOSAICV3_ABCC_CFG", /* 105 */
	"DEMOSAICV3_DBCC_CFG",
	"DEMOSAICV3_DBPC_CFG",
	"DEMOSAICV3_ABF_CFG",
	"DEMOSAICV3_ABCC_UPDATE",
	"DEMOSAICV3_DBCC_UPDATE", /* 110 */
	"DEMOSAICV3_DBPC_UPDATE",
	"XBAR_CFG",
	"MODULE_CFG",
	"V32_ZSL",
	"LINEARIZATION_UPDATE", /*115*/
	"DEMOSAICV3_ABF_UPDATE",
	"CLF_CFG",
	"CLF_LUMA_UPDATE",
	"CLF_CHROMA_UPDATE",
	"PCA_ROLL_OFF_CFG", /*120*/
	"PCA_ROLL_OFF_UPDATE",
	"GET_REG_DUMP",
	"GET_LINEARIZATON_TABLE",
	"GET_MESH_ROLLOFF_TABLE",
	"GET_PCA_ROLLOFF_TABLE", /*125*/
	"GET_RGB_G_TABLE",
	"GET_LA_TABLE",
	"DEMOSAICV3_UPDATE",
	"VFE_CMD_ACTIVE_REGION_CFG",
	"VFE_CMD_COLOR_PROCESSING_CONFIG", /*130*/
	"VFE_CMD_STATS_WB_AEC_CONFIG",
	"VFE_CMD_STATS_WB_AEC_UPDATE",
	"VFE_CMD_Y_GAMMA_CONFIG",
	"VFE_CMD_SCALE_OUTPUT1_CONFIG",
	"VFE_CMD_SCALE_OUTPUT2_CONFIG", /*135*/
	"VFE_CMD_CAPTURE_RAW",
	"VFE_CMD_STOP_LIVESHOT",
	"VFE_CMD_RECONFIG_VFE",
	"STATS_REQBUF",
	"STATS_ENQUEUEBUF", /*140*/
	"STATS_FLUSH_BUFQ",
	"STATS_UNREGBUF",
	"STATS_BG_START",
	"STATS_BG_STOP",
	"STATS_BF_START", /*145*/
	"STATS_BF_STOP",
	"STATS_BHIST_START",
	"STATS_BHIST_STOP",
	"RDI_SEL" /*150*/
	"VFE_CMD_FOV_ENC_CFG",
	"VFE_CMD_FOV_VIEW_CFG",
	"VFE_CMD_FOV_ENC_UPDATE",
	"VFE_CMD_FOV_VIEW_UPDATE",
	"VFE_CMD_SCALER_ENC_CFG",
	"VFE_CMD_SCALER_VIEW_CFG",
	"VFE_CMD_SCALER_ENC_UPDATE",
	"VFE_CMD_SCALER_VIEW_UPDATE",
	"VFE_CMD_COLORXFORM_ENC_CFG",
	"VFE_CMD_COLORXFORM_VIEW_CFG",
	"VFE_CMD_COLORXFORM_ENC_UPDATE",
	"VFE_CMD_COLORXFORM_VIEW_UPDATE",
	"VFE_CMD_TEST_GEN_CFG",
	"VFE_CMD_SELECT_RDI",
	"VFE_CMD_SET_STATS_VER",
	"VFE_CMD_STOP_RECORDING_DONE",
	"VFE_CMD_RGB_ALL_CFG",
	"VFE_CMD_RGB_ALL_UPDATE",
};

#if defined(CONFIG_MACH_JACTIVE_ATT) || defined(CONFIG_MACH_JACTIVE_EUR)
static atomic_t recovery_active;
static uint32_t recover_irq_mask0, recover_irq_mask1;
#endif

uint8_t vfe32_use_bayer_stats(struct vfe32_ctrl_type *vfe32_ctrl)
{
	if (vfe32_ctrl->ver_num.main >= 4) {
		/* VFE 4 or above uses bayer stats */
		return TRUE;
	} else {
		return FALSE;
	}
}

static void axi_enable_wm_irq(struct vfe_share_ctrl_t *share_ctrl)
{
	uint32_t irq_mask = 0, irq_comp_mask = 0;
	uint16_t vfe_output_mode1 = 0;
	uint32_t rdi_comp_select = 0;

	uint16_t vfe_output_mode =
		share_ctrl->outpath.output_mode &
			~(VFE32_OUTPUT_MODE_TERTIARY1|
			VFE32_OUTPUT_MODE_TERTIARY2|
			  VFE32_OUTPUT_MODE_TERTIARY3);

	vfe_output_mode1 =
		((share_ctrl->outpath.output_mode &
		VFE32_OUTPUT_MODE_TERTIARY1)  &&
		(share_ctrl->outpath.output_mode &
		VFE32_OUTPUT_MODE_TERTIARY2));

	if (vfe_output_mode || vfe_output_mode1)
		irq_comp_mask =
		msm_camera_io_r(share_ctrl->vfebase +
			VFE_IRQ_COMP_MASK);
	irq_mask = msm_camera_io_r(share_ctrl->vfebase +
				VFE_IRQ_MASK_0);

	if (share_ctrl->outpath.output_mode &
			VFE32_OUTPUT_MODE_PRIMARY) {
		if (share_ctrl->current_mode == VFE_OUTPUTS_RAW)
			irq_comp_mask |= (
				0x1 << share_ctrl->outpath.out0.ch0);
		else
			irq_comp_mask |= (
				0x1 << share_ctrl->outpath.out0.ch0 |
				0x1 << share_ctrl->outpath.out0.ch1);
		irq_mask |= VFE_IRQ_STATUS0_IMAGE_COMPOSIT_DONE0_MASK;
	} else if (share_ctrl->outpath.output_mode &
			   VFE32_OUTPUT_MODE_PRIMARY_ALL_CHNLS) {
		irq_comp_mask |= (
			0x1 << share_ctrl->outpath.out0.ch0 |
			0x1 << share_ctrl->outpath.out0.ch1 |
			0x1 << share_ctrl->outpath.out0.ch2);
		irq_mask |= VFE_IRQ_STATUS0_IMAGE_COMPOSIT_DONE0_MASK;
	}
	if (share_ctrl->outpath.output_mode &
			VFE32_OUTPUT_MODE_SECONDARY) {
		irq_comp_mask |= (
			0x1 << (share_ctrl->outpath.out1.ch0 + 8) |
			0x1 << (share_ctrl->outpath.out1.ch1 + 8));
		irq_mask |= VFE_IRQ_STATUS0_IMAGE_COMPOSIT_DONE1_MASK;
	} else if (share_ctrl->outpath.output_mode &
			VFE32_OUTPUT_MODE_SECONDARY_ALL_CHNLS) {
		irq_comp_mask |= (
			0x1 << (share_ctrl->outpath.out1.ch0 + 8) |
			0x1 << (share_ctrl->outpath.out1.ch1 + 8) |
			0x1 << (share_ctrl->outpath.out1.ch2 + 8));
		irq_mask |= VFE_IRQ_STATUS0_IMAGE_COMPOSIT_DONE1_MASK;
	}

	if (share_ctrl->outpath.output_mode &
		VFE32_OUTPUT_MODE_TERTIARY1) {
		irq_mask |= (0x1 << (share_ctrl->outpath.out2.ch0 +
			VFE_WM_OFFSET));
	}

	if (share_ctrl->outpath.output_mode &
		VFE32_OUTPUT_MODE_TERTIARY2) {
		irq_mask |= (0x1 << (share_ctrl->outpath.out3.ch0 +
			VFE_WM_OFFSET));
	}
	if (share_ctrl->outpath.output_mode &
		VFE32_OUTPUT_MODE_TERTIARY3) {
		irq_mask |= (0x1 << (share_ctrl->outpath.out4.ch0 +
			VFE_WM_OFFSET));
	}
	rdi_comp_select = (vfe_output_mode1 &&
		(share_ctrl->rdi_comp == VFE_RDI_COMPOSITE));
	if (rdi_comp_select) {
		irq_comp_mask |= (
		0x1 << (share_ctrl->outpath.out2.ch0 + 16) |
		0x1 << (share_ctrl->outpath.out3.ch0 + 16));
		irq_mask |= VFE_IRQ_STATUS0_IMAGE_COMPOSIT_DONE2_MASK;
	}

	msm_camera_io_w(irq_mask, share_ctrl->vfebase +
			VFE_IRQ_MASK_0);
	if (vfe_output_mode || rdi_comp_select)
		msm_camera_io_w(irq_comp_mask,
			share_ctrl->vfebase + VFE_IRQ_COMP_MASK);
}

static void axi_disable_wm_irq(struct vfe_share_ctrl_t *share_ctrl,
	uint16_t output_mode)
{
	uint32_t irq_mask, irq_comp_mask = 0;
	uint16_t vfe_output_mode1 = 0;
	uint32_t rdi_comp_select = 0;
	uint16_t vfe_output_mode =
		output_mode &
			~(VFE32_OUTPUT_MODE_TERTIARY1|
			VFE32_OUTPUT_MODE_TERTIARY2|
			  VFE32_OUTPUT_MODE_TERTIARY3);

	vfe_output_mode1 =
		(output_mode & VFE32_OUTPUT_MODE_TERTIARY2);

	if (vfe_output_mode || vfe_output_mode1)
		irq_comp_mask =
		msm_camera_io_r(share_ctrl->vfebase +
			VFE_IRQ_COMP_MASK);
	irq_mask = msm_camera_io_r(share_ctrl->vfebase +
				VFE_IRQ_MASK_0);

	if (output_mode &
			VFE32_OUTPUT_MODE_PRIMARY) {
		irq_comp_mask &= ~(
			0x1 << share_ctrl->outpath.out0.ch0 |
			0x1 << share_ctrl->outpath.out0.ch1);
		irq_mask &= ~VFE_IRQ_STATUS0_IMAGE_COMPOSIT_DONE0_MASK;
	} else if (output_mode &
			   VFE32_OUTPUT_MODE_PRIMARY_ALL_CHNLS) {
		irq_comp_mask &= ~(
			0x1 << share_ctrl->outpath.out0.ch0 |
			0x1 << share_ctrl->outpath.out0.ch1 |
			0x1 << share_ctrl->outpath.out0.ch2);
		irq_mask &= ~VFE_IRQ_STATUS0_IMAGE_COMPOSIT_DONE0_MASK;
	}
	if (output_mode &
			VFE32_OUTPUT_MODE_SECONDARY) {
		irq_comp_mask &= ~(
			0x1 << (share_ctrl->outpath.out1.ch0 + 8) |
			0x1 << (share_ctrl->outpath.out1.ch1 + 8));
		irq_mask &= ~VFE_IRQ_STATUS0_IMAGE_COMPOSIT_DONE1_MASK;
	} else if (output_mode &
			VFE32_OUTPUT_MODE_SECONDARY_ALL_CHNLS) {
		irq_comp_mask &= ~(
			0x1 << (share_ctrl->outpath.out1.ch0 + 8) |
			0x1 << (share_ctrl->outpath.out1.ch1 + 8) |
			0x1 << (share_ctrl->outpath.out1.ch2 + 8));
		irq_mask &= ~VFE_IRQ_STATUS0_IMAGE_COMPOSIT_DONE1_MASK;
	}
	if (output_mode &
			VFE32_OUTPUT_MODE_TERTIARY1) {
			irq_mask &= ~(0x1 << (share_ctrl->outpath.out2.ch0 +
				VFE_WM_OFFSET));
	}
	if (output_mode &
		VFE32_OUTPUT_MODE_TERTIARY2) {
		irq_mask &= ~(0x1 << (share_ctrl->outpath.out3.ch0 +
			VFE_WM_OFFSET));
	}
	if (output_mode &
		VFE32_OUTPUT_MODE_TERTIARY3) {
		irq_mask &= ~(0x1 << (share_ctrl->outpath.out4.ch0 +
			VFE_WM_OFFSET));
	}
	rdi_comp_select = (vfe_output_mode1 &&
		(share_ctrl->rdi_comp == VFE_RDI_COMPOSITE));
	if (rdi_comp_select) {
		irq_comp_mask &= ~(
		0x1 << (share_ctrl->outpath.out2.ch0 + 16) |
		0x1 << (share_ctrl->outpath.out3.ch0 + 16));
		irq_mask &= ~VFE_IRQ_STATUS0_IMAGE_COMPOSIT_DONE2_MASK;
	}
	msm_camera_io_w(irq_mask, share_ctrl->vfebase + VFE_IRQ_MASK_0);
	if (vfe_output_mode || vfe_output_mode1)
		msm_camera_io_w(irq_comp_mask,
			share_ctrl->vfebase + VFE_IRQ_COMP_MASK);
}

static void axi_enable_irq(struct vfe_share_ctrl_t *share_ctrl)
{
	uint32_t irq_mask, irq_mask1;
	uint32_t vfe_mode =
		share_ctrl->current_mode & ~(VFE_OUTPUTS_RDI0 |
			VFE_OUTPUTS_RDI1 | VFE_OUTPUTS_RDI2);

	if (share_ctrl->axi_ref_cnt == 1) {
		irq_mask1 =
			msm_camera_io_r(share_ctrl->vfebase +
				VFE_IRQ_MASK_1);

		irq_mask1 |= VFE_IMASK_WHILE_STOPPING_1;
			msm_camera_io_w(irq_mask1, share_ctrl->vfebase +
				VFE_IRQ_MASK_1);
	}

	if (share_ctrl->current_mode & (VFE_OUTPUTS_RDI0 |
		VFE_OUTPUTS_RDI1|VFE_OUTPUTS_RDI2)) {
		irq_mask1 =
			msm_camera_io_r(share_ctrl->vfebase +
				VFE_IRQ_MASK_1);

		if (share_ctrl->current_mode & VFE_OUTPUTS_RDI0)
			irq_mask1 |= VFE_IRQ_STATUS1_RDI0_REG_UPDATE_MASK;

		if (share_ctrl->current_mode & VFE_OUTPUTS_RDI1)
			irq_mask1 |= VFE_IRQ_STATUS1_RDI1_REG_UPDATE_MASK;

		if (share_ctrl->current_mode & VFE_OUTPUTS_RDI2)
			irq_mask1 |= VFE_IRQ_STATUS1_RDI2_REG_UPDATE_MASK;

		msm_camera_io_w(irq_mask1, share_ctrl->vfebase +
			VFE_IRQ_MASK_1);
	}

	if (vfe_mode) {
		irq_mask =
		msm_camera_io_r(share_ctrl->vfebase +
			VFE_IRQ_MASK_0);
		irq_mask |= 0x00000021;
		if (share_ctrl->stats_comp)
			irq_mask |= VFE_IRQ_STATUS0_STATS_COMPOSIT_MASK;
		else
			irq_mask |= 0x000FE000;
		msm_camera_io_w(irq_mask, share_ctrl->vfebase +
			VFE_IRQ_MASK_0);
		atomic_set(&share_ctrl->vstate, 1);
	}
	atomic_set(&share_ctrl->handle_common_irq, 1);
}

static void axi_clear_all_interrupts(struct vfe_share_ctrl_t *share_ctrl)
{
	atomic_set(&share_ctrl->handle_common_irq, 0);
	msm_camera_io_w(VFE_DISABLE_ALL_IRQS,
		share_ctrl->vfebase + VFE_IRQ_MASK_0);
	msm_camera_io_w(VFE_DISABLE_ALL_IRQS,
		share_ctrl->vfebase + VFE_IRQ_MASK_1);

	/* clear all pending interrupts*/
	msm_camera_io_w(VFE_CLEAR_ALL_IRQS,
		share_ctrl->vfebase + VFE_IRQ_CLEAR_0);
	msm_camera_io_w(VFE_CLEAR_ALL_IRQS,
		share_ctrl->vfebase + VFE_IRQ_CLEAR_1);
	/* Ensure the write order while writing
	*to the command register using the barrier */
	msm_camera_io_w_mb(1,
		share_ctrl->vfebase + VFE_IRQ_CMD);
}

static void axi_disable_irq(struct vfe_share_ctrl_t *share_ctrl,
	uint32_t mode)
{

	/* disable all interrupts.  */

	uint32_t irq_mask = 0, irq_mask1 = 0, clear_mask1 = 0;
	uint32_t vfe_mode =
		(mode & ~(VFE_OUTPUTS_RDI0|
			VFE_OUTPUTS_RDI1|VFE_OUTPUTS_RDI2));

	if (mode & (VFE_OUTPUTS_RDI0|VFE_OUTPUTS_RDI1|VFE_OUTPUTS_RDI2)) {
		irq_mask1 =
			msm_camera_io_r(share_ctrl->vfebase +
				VFE_IRQ_MASK_1);

		if (mode & VFE_OUTPUTS_RDI0) {
			irq_mask1 &= ~(VFE_IRQ_STATUS1_RDI0_REG_UPDATE_MASK);
			clear_mask1 |= VFE_IRQ_STATUS1_RDI0_REG_UPDATE_MASK;
		}

		if (mode & VFE_OUTPUTS_RDI1) {
			irq_mask1 &= ~(VFE_IRQ_STATUS1_RDI1_REG_UPDATE_MASK);
			clear_mask1 |= VFE_IRQ_STATUS1_RDI1_REG_UPDATE_MASK;
		}
		if (mode & VFE_OUTPUTS_RDI2) {
			irq_mask1 &= ~(VFE_IRQ_STATUS1_RDI2_REG_UPDATE_MASK);
			clear_mask1 |= VFE_IRQ_STATUS1_RDI2_REG_UPDATE_MASK;
		}

		msm_camera_io_w(irq_mask1, share_ctrl->vfebase +
			VFE_IRQ_MASK_1);
		msm_camera_io_w(clear_mask1,
			share_ctrl->vfebase + VFE_IRQ_CLEAR_1);
	}

	if (vfe_mode) {
		atomic_set(&share_ctrl->vstate, 0);
		irq_mask =
		msm_camera_io_r(share_ctrl->vfebase +
			VFE_IRQ_MASK_0);
		irq_mask &= ~(0x00000021);
		if (share_ctrl->stats_comp)
			irq_mask &= ~(VFE_IRQ_STATUS0_STATS_COMPOSIT_MASK);
		else
			irq_mask &= ~0x000FE000;
		msm_camera_io_w(irq_mask, share_ctrl->vfebase +
			VFE_IRQ_MASK_0);
	}
}

static void vfe32_stop(struct vfe32_ctrl_type *vfe32_ctrl)
{

	/* in either continuous or snapshot mode, stop command can be issued
	 * at any time. stop camif immediately. */
	if (!vfe32_ctrl->share_ctrl->dual_enabled)
		msm_camera_io_w_mb(CAMIF_COMMAND_STOP_IMMEDIATELY,
			vfe32_ctrl->share_ctrl->vfebase + VFE_CAMIF_COMMAND);
	else
		msm_camera_io_w(CAMIF_COMMAND_STOP_AT_FRAME_BOUNDARY,
			vfe32_ctrl->share_ctrl->vfebase + VFE_CAMIF_COMMAND);
	vfe32_ctrl->share_ctrl->operation_mode &=
		(VFE_OUTPUTS_RDI0|VFE_OUTPUTS_RDI1|VFE_OUTPUTS_RDI2);
}

static void vfe32_subdev_notify(int id, int path, uint32_t inst_handle,
	struct v4l2_subdev *sd, struct vfe_share_ctrl_t *share_ctrl)
{
	struct msm_vfe_resp rp;
	struct msm_frame_info frame_info;
	unsigned long flags = 0;
	spin_lock_irqsave(&share_ctrl->sd_notify_lock, flags);
	CDBG("vfe32_subdev_notify : msgId = %d\n", id);
	memset(&rp, 0, sizeof(struct msm_vfe_resp));
	rp.evt_msg.type   = MSM_CAMERA_MSG;
	frame_info.inst_handle = inst_handle;
	frame_info.path = path;
	rp.evt_msg.data = &frame_info;
	rp.type	   = id;
	v4l2_subdev_notify(sd, NOTIFY_VFE_BUF_EVT, &rp);
	spin_unlock_irqrestore(&share_ctrl->sd_notify_lock, flags);
}

static int vfe32_config_axi(
	struct axi_ctrl_t *axi_ctrl, int mode, uint32_t *ao)
{
	uint32_t *ch_info;
	uint32_t *axi_cfg = ao+V32_AXI_BUS_FMT_OFF;
	int vfe_mode = (mode & ~(OUTPUT_TERT1|OUTPUT_TERT2|OUTPUT_TERT3));
	uint32_t bus_cmd = *axi_cfg;
	int i;

	/* Update the corresponding write masters for each output*/
	ch_info = axi_cfg + V32_AXI_CFG_LEN;
	axi_ctrl->share_ctrl->outpath.out0.ch0 = 0x0000FFFF & *ch_info;
	axi_ctrl->share_ctrl->outpath.out0.ch1 =
		0x0000FFFF & (*ch_info++ >> 16);
	axi_ctrl->share_ctrl->outpath.out0.ch2 = 0x0000FFFF & *ch_info++;
	axi_ctrl->share_ctrl->outpath.out0.inst_handle = *ch_info++;

	axi_ctrl->share_ctrl->outpath.out1.ch0 = 0x0000FFFF & *ch_info;
	axi_ctrl->share_ctrl->outpath.out1.ch1 =
		0x0000FFFF & (*ch_info++ >> 16);
	axi_ctrl->share_ctrl->outpath.out1.ch2 = 0x0000FFFF & *ch_info++;
	axi_ctrl->share_ctrl->outpath.out1.inst_handle = *ch_info++;

	axi_ctrl->share_ctrl->outpath.out2.ch0 = 0x0000FFFF & *ch_info;
	axi_ctrl->share_ctrl->outpath.out2.ch1 =
		0x0000FFFF & (*ch_info++ >> 16);
	axi_ctrl->share_ctrl->outpath.out2.ch2 = 0x0000FFFF & *ch_info++;
	axi_ctrl->share_ctrl->outpath.out2.inst_handle = *ch_info++;

	axi_ctrl->share_ctrl->outpath.out3.ch0 = 0x0000FFFF & *ch_info;
	axi_ctrl->share_ctrl->outpath.out3.ch1 =
		0x0000FFFF & (*ch_info++ >> 16);
	axi_ctrl->share_ctrl->outpath.out3.ch2 = 0x0000FFFF & *ch_info++;
	axi_ctrl->share_ctrl->outpath.out3.inst_handle = *ch_info++;

	axi_ctrl->share_ctrl->outpath.out4.ch0 = 0x0000FFFF & *ch_info;
	axi_ctrl->share_ctrl->outpath.out4.ch1 =
		0x0000FFFF & (*ch_info++ >> 16);
	axi_ctrl->share_ctrl->outpath.out4.ch2 = 0x0000FFFF & *ch_info++;
	axi_ctrl->share_ctrl->outpath.out4.inst_handle = *ch_info++;

	axi_ctrl->share_ctrl->outpath.output_mode = 0;

	if (mode & OUTPUT_TERT1)
		axi_ctrl->share_ctrl->outpath.output_mode |=
			VFE32_OUTPUT_MODE_TERTIARY1;
	if (mode & OUTPUT_TERT2)
		axi_ctrl->share_ctrl->outpath.output_mode |=
			VFE32_OUTPUT_MODE_TERTIARY2;
	if (mode & OUTPUT_TERT3)
		axi_ctrl->share_ctrl->outpath.output_mode |=
			VFE32_OUTPUT_MODE_TERTIARY3;
	if (mode == OUTPUT_TERT1
		|| mode == OUTPUT_TERT2
		|| mode == OUTPUT_TERT3
		|| mode == (OUTPUT_TERT1|OUTPUT_TERT2)
		|| mode == (OUTPUT_TERT1|OUTPUT_TERT3)
		|| mode == (OUTPUT_TERT2|OUTPUT_TERT3)
		|| mode == (OUTPUT_TERT1|OUTPUT_TERT2|OUTPUT_TERT3))
			goto bus_cfg;

	switch (vfe_mode) {
	case OUTPUT_PRIM:
		axi_ctrl->share_ctrl->outpath.output_mode |=
			VFE32_OUTPUT_MODE_PRIMARY;
		break;
	case OUTPUT_PRIM_ALL_CHNLS:
		axi_ctrl->share_ctrl->outpath.output_mode |=
			VFE32_OUTPUT_MODE_PRIMARY_ALL_CHNLS;
		break;
	case OUTPUT_PRIM|OUTPUT_SEC:
		axi_ctrl->share_ctrl->outpath.output_mode |=
			VFE32_OUTPUT_MODE_PRIMARY;
		axi_ctrl->share_ctrl->outpath.output_mode |=
			VFE32_OUTPUT_MODE_SECONDARY;
		break;
	case OUTPUT_PRIM|OUTPUT_SEC_ALL_CHNLS:
		axi_ctrl->share_ctrl->outpath.output_mode |=
			VFE32_OUTPUT_MODE_PRIMARY;
		axi_ctrl->share_ctrl->outpath.output_mode |=
			VFE32_OUTPUT_MODE_SECONDARY_ALL_CHNLS;
		break;
	case OUTPUT_PRIM_ALL_CHNLS|OUTPUT_SEC:
		axi_ctrl->share_ctrl->outpath.output_mode |=
			VFE32_OUTPUT_MODE_PRIMARY_ALL_CHNLS;
		axi_ctrl->share_ctrl->outpath.output_mode |=
			VFE32_OUTPUT_MODE_SECONDARY;
		break;
	default:
		pr_err("%s Invalid AXI mode %d ", __func__, mode);
		return -EINVAL;
	}

bus_cfg:
	msm_camera_io_w(*ao, axi_ctrl->share_ctrl->vfebase +
		VFE_BUS_IO_FORMAT_CFG);
	axi_cfg++;
	msm_camera_io_memcpy(axi_ctrl->share_ctrl->vfebase +
		vfe32_cmd[VFE_CMD_AXI_OUT_CFG].offset, axi_cfg,
		V32_AXI_BUS_CFG_LEN);
	axi_cfg += V32_AXI_BUS_CFG_LEN/4;
	for (i = 0; i < ARRAY_SIZE(vfe32_AXI_WM_CFG); i++) {
		axi_cfg += 3;
		msm_camera_io_memcpy(
			axi_ctrl->share_ctrl->vfebase+vfe32_AXI_WM_CFG[i]+12,
								axi_cfg, 12);
		axi_cfg += 3;
	}
	msm_camera_io_w(bus_cmd, axi_ctrl->share_ctrl->vfebase +
					V32_AXI_BUS_CMD_OFF);
	msm_camera_io_w(*ch_info++,
		axi_ctrl->share_ctrl->vfebase + VFE_PIXEL_IF_CFG);
	if (msm_camera_io_r(axi_ctrl->share_ctrl->vfebase +
		V32_GET_HW_VERSION_OFF) ==
		VFE33_HW_NUMBER) {
		msm_camera_io_w(*ch_info++,
			axi_ctrl->share_ctrl->vfebase + VFE_RDI0_CFG);
		msm_camera_io_w(*ch_info++,
			axi_ctrl->share_ctrl->vfebase + VFE_RDI1_CFG);
	}
	return 0;
}

static void axi_reset_internal_variables(
	struct axi_ctrl_t *axi_ctrl,
	struct msm_camera_vfe_params_t vfe_params)
{
	if (vfe_params.operation_mode & VFE_OUTPUTS_RDI0) {
		atomic_set(&axi_ctrl->share_ctrl->rdi0_update_ack_pending, 0);
		axi_ctrl->share_ctrl->rdi0_capture_count = -1;
		axi_ctrl->share_ctrl->outpath.out2.capture_cnt = -1;
		axi_ctrl->share_ctrl->rdi0FrameId = 0;
		axi_ctrl->share_ctrl->comp_output_mode &=
			~VFE32_OUTPUT_MODE_TERTIARY1;
		axi_ctrl->share_ctrl->operation_mode &=
			~(VFE_OUTPUTS_RDI0);
	}

	if (vfe_params.operation_mode & VFE_OUTPUTS_RDI1) {
		atomic_set(&axi_ctrl->share_ctrl->rdi1_update_ack_pending, 0);
		axi_ctrl->share_ctrl->rdi1_capture_count = -1;
		axi_ctrl->share_ctrl->outpath.out3.capture_cnt = -1;
		axi_ctrl->share_ctrl->rdi1FrameId = 0;
		axi_ctrl->share_ctrl->comp_output_mode &=
			~VFE32_OUTPUT_MODE_TERTIARY2;
		axi_ctrl->share_ctrl->operation_mode &=
			~(VFE_OUTPUTS_RDI1);
	}

	if (vfe_params.operation_mode & VFE_OUTPUTS_RDI2) {
		atomic_set(&axi_ctrl->share_ctrl->rdi2_update_ack_pending, 0);
		axi_ctrl->share_ctrl->rdi2_capture_count = -1;
		axi_ctrl->share_ctrl->outpath.out4.capture_cnt = -1;
		axi_ctrl->share_ctrl->rdi2FrameId = 0;
		axi_ctrl->share_ctrl->comp_output_mode &=
			~VFE32_OUTPUT_MODE_TERTIARY3;
		axi_ctrl->share_ctrl->operation_mode &=
			~(VFE_OUTPUTS_RDI2);
	}
}

static void axi_global_reset_internal_variables(
	struct axi_ctrl_t *axi_ctrl)
{
	unsigned long flags = 0;
	/* state control variables */
	axi_ctrl->share_ctrl->start_ack_pending = FALSE;
	atomic_set(&irq_cnt, 0);

	spin_lock_irqsave(&axi_ctrl->share_ctrl->stop_flag_lock, flags);
	axi_ctrl->share_ctrl->stop_ack_pending  = FALSE;
	spin_unlock_irqrestore(&axi_ctrl->share_ctrl->stop_flag_lock, flags);

	init_completion(&axi_ctrl->share_ctrl->reset_complete);

	spin_lock_irqsave(&axi_ctrl->share_ctrl->update_ack_lock, flags);
	axi_ctrl->share_ctrl->update_ack_pending = FALSE;
	spin_unlock_irqrestore(&axi_ctrl->share_ctrl->update_ack_lock, flags);

	axi_ctrl->share_ctrl->recording_state = VFE_STATE_IDLE;
	axi_ctrl->share_ctrl->liveshot_state = VFE_STATE_IDLE;

	atomic_set(&axi_ctrl->share_ctrl->vstate, 0);
	atomic_set(&axi_ctrl->share_ctrl->handle_common_irq, 0);
	atomic_set(&axi_ctrl->share_ctrl->pix0_update_ack_pending, 0);
	atomic_set(&axi_ctrl->share_ctrl->rdi0_update_ack_pending, 0);
	atomic_set(&axi_ctrl->share_ctrl->rdi1_update_ack_pending, 0);
	atomic_set(&axi_ctrl->share_ctrl->rdi2_update_ack_pending, 0);

	/* 0 for continuous mode, 1 for snapshot mode */
	axi_ctrl->share_ctrl->operation_mode = 0;
	axi_ctrl->share_ctrl->current_mode = 0;
	axi_ctrl->share_ctrl->outpath.output_mode = 0;
	axi_ctrl->share_ctrl->comp_output_mode = 0;
	axi_ctrl->share_ctrl->vfe_capture_count = 0;
	axi_ctrl->share_ctrl->rdi0_capture_count = -1;
	axi_ctrl->share_ctrl->rdi1_capture_count = -1;
	axi_ctrl->share_ctrl->rdi2_capture_count = -1;
	axi_ctrl->share_ctrl->outpath.out0.capture_cnt = -1;
	axi_ctrl->share_ctrl->outpath.out1.capture_cnt = -1;
	axi_ctrl->share_ctrl->outpath.out2.capture_cnt = -1;
	axi_ctrl->share_ctrl->outpath.out3.capture_cnt = -1;
	axi_ctrl->share_ctrl->outpath.out4.capture_cnt = -1;

	/* this is unsigned 32 bit integer. */
	axi_ctrl->share_ctrl->vfeFrameId = 0;
	axi_ctrl->share_ctrl->rdi0FrameId = 0;
	axi_ctrl->share_ctrl->rdi1FrameId = 0;
	axi_ctrl->share_ctrl->rdi2FrameId = 0;
}


static void vfe32_program_dmi_cfg(
	enum VFE32_DMI_RAM_SEL bankSel,
	struct vfe32_ctrl_type *vfe32_ctrl)
{
	/* set bit 8 for auto increment. */
	uint32_t value = VFE_DMI_CFG_DEFAULT;
	value += (uint32_t)bankSel;
	CDBG("%s: banksel = %d\n", __func__, bankSel);

	msm_camera_io_w(value, vfe32_ctrl->share_ctrl->vfebase +
		VFE_DMI_CFG);
	/* by default, always starts with offset 0.*/
	msm_camera_io_w(0, vfe32_ctrl->share_ctrl->vfebase +
		VFE_DMI_ADDR);
}

static void vfe32_reset_dmi_tables(
	struct vfe32_ctrl_type *vfe32_ctrl)
{
	int i = 0;

	/* Reset Histogram LUTs */
	CDBG("Reset Bayer histogram LUT : 0\n");
	vfe32_program_dmi_cfg(STATS_BHIST_RAM0, vfe32_ctrl);
	/* Loop for configuring LUT */
	for (i = 0; i < 256; i++) {
		msm_camera_io_w(0, vfe32_ctrl->share_ctrl->vfebase +
			VFE_DMI_DATA_HI);
		msm_camera_io_w(0, vfe32_ctrl->share_ctrl->vfebase +
			VFE_DMI_DATA_LO);
	}
	vfe32_program_dmi_cfg(NO_MEM_SELECTED, vfe32_ctrl);

	CDBG("Reset Bayer Histogram LUT: 1\n");
	vfe32_program_dmi_cfg(STATS_BHIST_RAM1, vfe32_ctrl);
	/* Loop for configuring LUT */
	for (i = 0; i < 256; i++) {
		msm_camera_io_w(0, vfe32_ctrl->share_ctrl->vfebase +
			VFE_DMI_DATA_HI);
		msm_camera_io_w(0, vfe32_ctrl->share_ctrl->vfebase +
			VFE_DMI_DATA_LO);
	}
	vfe32_program_dmi_cfg(NO_MEM_SELECTED, vfe32_ctrl);

	CDBG("Reset IHistogram LUT\n");
	vfe32_program_dmi_cfg(STATS_IHIST_RAM, vfe32_ctrl);
	/* Loop for configuring LUT */
	for (i = 0; i < 256; i++) {
		msm_camera_io_w(0, vfe32_ctrl->share_ctrl->vfebase +
			VFE_DMI_DATA_HI);
		msm_camera_io_w(0, vfe32_ctrl->share_ctrl->vfebase +
			VFE_DMI_DATA_LO);
	}
	vfe32_program_dmi_cfg(NO_MEM_SELECTED, vfe32_ctrl);
}

static void vfe32_set_default_reg_values(
	struct vfe32_ctrl_type *vfe32_ctrl)
{
	msm_camera_io_w(0x800080,
		vfe32_ctrl->share_ctrl->vfebase + VFE_DEMUX_GAIN_0);
	msm_camera_io_w(0x800080,
		vfe32_ctrl->share_ctrl->vfebase + VFE_DEMUX_GAIN_1);
	/* What value should we program CGC_OVERRIDE to? */
	msm_camera_io_w(0xFFFFF,
		vfe32_ctrl->share_ctrl->vfebase + VFE_CGC_OVERRIDE);

	/* default frame drop period and pattern */
	msm_camera_io_w(0x1f,
		vfe32_ctrl->share_ctrl->vfebase + VFE_FRAMEDROP_ENC_Y_CFG);
	msm_camera_io_w(0x1f,
		vfe32_ctrl->share_ctrl->vfebase + VFE_FRAMEDROP_ENC_CBCR_CFG);
	msm_camera_io_w(0xFFFFFFFF,
		vfe32_ctrl->share_ctrl->vfebase + VFE_FRAMEDROP_ENC_Y_PATTERN);
	msm_camera_io_w(0xFFFFFFFF,
		vfe32_ctrl->share_ctrl->vfebase +
		VFE_FRAMEDROP_ENC_CBCR_PATTERN);
	msm_camera_io_w(0x1f,
		vfe32_ctrl->share_ctrl->vfebase + VFE_FRAMEDROP_VIEW_Y);
	msm_camera_io_w(0x1f,
		vfe32_ctrl->share_ctrl->vfebase + VFE_FRAMEDROP_VIEW_CBCR);
	msm_camera_io_w(0xFFFFFFFF,
		vfe32_ctrl->share_ctrl->vfebase + VFE_FRAMEDROP_VIEW_Y_PATTERN);
	msm_camera_io_w(0xFFFFFFFF,
		vfe32_ctrl->share_ctrl->vfebase +
		VFE_FRAMEDROP_VIEW_CBCR_PATTERN);
	msm_camera_io_w(0, vfe32_ctrl->share_ctrl->vfebase + VFE_CLAMP_MIN);
	msm_camera_io_w(0xFFFFFF,
		vfe32_ctrl->share_ctrl->vfebase + VFE_CLAMP_MAX);

	/* stats UB config */
	CDBG("%s: Use bayer stats = %d\n", __func__,
		 vfe32_use_bayer_stats(vfe32_ctrl));
	if (!vfe32_use_bayer_stats(vfe32_ctrl)) {
#ifndef FIXED_WM
		msm_camera_io_w(0x3980007,
			vfe32_ctrl->share_ctrl->vfebase +
				VFE_BUS_STATS_AEC_BG_UB_CFG);
		msm_camera_io_w(0x3A00007,
			vfe32_ctrl->share_ctrl->vfebase +
				VFE_BUS_STATS_AF_BF_UB_CFG);
		msm_camera_io_w(0x3A8000F,
			vfe32_ctrl->share_ctrl->vfebase +
				VFE_BUS_STATS_AWB_UB_CFG);
		msm_camera_io_w(0x3B80007,
			vfe32_ctrl->share_ctrl->vfebase +
				VFE_BUS_STATS_RS_UB_CFG);
		msm_camera_io_w(0x3C0001F,
			vfe32_ctrl->share_ctrl->vfebase +
				VFE_BUS_STATS_CS_UB_CFG);
		msm_camera_io_w(0x3E0001F,
			vfe32_ctrl->share_ctrl->vfebase +
				VFE_BUS_STATS_HIST_UB_CFG);
#else /* PIP roll-off changes */
		msm_camera_io_w(0x3C00007,
			vfe32_ctrl->share_ctrl->vfebase +
				VFE_BUS_STATS_AEC_BG_UB_CFG);

		msm_camera_io_w(0x3C8000F,
			vfe32_ctrl->share_ctrl->vfebase +
				VFE_BUS_STATS_AWB_UB_CFG);

		msm_camera_io_w(0x3D80007,
			vfe32_ctrl->share_ctrl->vfebase +
				VFE_BUS_STATS_RS_UB_CFG);

		msm_camera_io_w(0x3E0001F,
			vfe32_ctrl->share_ctrl->vfebase +
				VFE_BUS_STATS_HIST_UB_CFG);
#endif
	} else {
		msm_camera_io_w(0x350001F,
			vfe32_ctrl->share_ctrl->vfebase +
				VFE_BUS_STATS_HIST_UB_CFG);
		msm_camera_io_w(0x370002F,
			vfe32_ctrl->share_ctrl->vfebase +
				VFE_BUS_STATS_AEC_BG_UB_CFG);
		msm_camera_io_w(0x3A0002F,
			vfe32_ctrl->share_ctrl->vfebase +
				VFE_BUS_STATS_AF_BF_UB_CFG);
		msm_camera_io_w(0x3D00007,
			vfe32_ctrl->share_ctrl->vfebase +
				VFE_BUS_STATS_RS_UB_CFG);
		msm_camera_io_w(0x3D8001F,
			vfe32_ctrl->share_ctrl->vfebase +
				VFE_BUS_STATS_CS_UB_CFG);
		msm_camera_io_w(0x3F80007,
			vfe32_ctrl->share_ctrl->vfebase +
				VFE_BUS_STATS_SKIN_BHIST_UB_CFG);
	}
	vfe32_reset_dmi_tables(vfe32_ctrl);
}

static void vfe32_reset_internal_variables(
	struct vfe32_ctrl_type *vfe32_ctrl)
{
	unsigned long flags = 0;
	spin_lock_irqsave(&vfe32_ctrl->share_ctrl->update_ack_lock,
		flags);
	vfe32_ctrl->share_ctrl->update_ack_pending = FALSE;
	spin_unlock_irqrestore(&vfe32_ctrl->share_ctrl->update_ack_lock,
		flags);
	vfe32_ctrl->share_ctrl->vfe_capture_count = 0;
	/* this is unsigned 32 bit integer. */
	vfe32_ctrl->share_ctrl->vfeFrameId = 0;
	vfe32_ctrl->share_ctrl->update_counter = 0;

	/* Stats control variables. */
	memset(&(vfe32_ctrl->afbfStatsControl), 0,
		sizeof(struct vfe_stats_control));

	memset(&(vfe32_ctrl->awbStatsControl), 0,
		sizeof(struct vfe_stats_control));

	memset(&(vfe32_ctrl->aecbgStatsControl), 0,
		sizeof(struct vfe_stats_control));

	memset(&(vfe32_ctrl->bhistStatsControl), 0,
		sizeof(struct vfe_stats_control));

	memset(&(vfe32_ctrl->ihistStatsControl), 0,
		sizeof(struct vfe_stats_control));

	memset(&(vfe32_ctrl->rsStatsControl), 0,
		sizeof(struct vfe_stats_control));

	memset(&(vfe32_ctrl->csStatsControl), 0,
		sizeof(struct vfe_stats_control));
	vfe32_ctrl->share_ctrl->outpath.out0.capture_cnt = -1;
	vfe32_ctrl->share_ctrl->outpath.out1.capture_cnt = -1;
	vfe32_ctrl->share_ctrl->outpath.out2.capture_cnt = -1;
	vfe32_ctrl->share_ctrl->outpath.out3.capture_cnt = -1;
	vfe32_ctrl->share_ctrl->outpath.out4.capture_cnt = -1;

	vfe32_ctrl->share_ctrl->recording_state = VFE_STATE_IDLE;
	vfe32_ctrl->share_ctrl->liveshot_state = VFE_STATE_IDLE;

	atomic_set(&vfe32_ctrl->share_ctrl->vstate, 0);
	atomic_set(&vfe32_ctrl->share_ctrl->pix0_update_ack_pending, 0);
	atomic_set(&vfe32_ctrl->share_ctrl->rdi0_update_ack_pending, 0);
	atomic_set(&vfe32_ctrl->share_ctrl->rdi1_update_ack_pending, 0);
	atomic_set(&vfe32_ctrl->share_ctrl->rdi2_update_ack_pending, 0);
	vfe32_ctrl->frame_skip_cnt = 31;
	vfe32_ctrl->frame_skip_pattern = 0xffffffff;
	vfe32_ctrl->snapshot_frame_cnt = 0;
#if defined(CONFIG_MACH_JACTIVE_ATT) || defined(CONFIG_MACH_JACTIVE_EUR)
	atomic_set(&recovery_active, 0);
#endif
	vfe32_set_default_reg_values(vfe32_ctrl);
}


static int vfe32_reset(struct vfe32_ctrl_type *vfe32_ctrl)
{
	uint32_t irq_mask1, irq_mask;
	atomic_set(&vfe32_ctrl->share_ctrl->vstate, 0);
	msm_camera_io_w(VFE_MODULE_RESET_CMD,
		vfe32_ctrl->share_ctrl->vfebase + VFE_MODULE_RESET);
	msm_camera_io_w(0,
		vfe32_ctrl->share_ctrl->vfebase + VFE_MODULE_RESET);

	irq_mask =
		msm_camera_io_r(vfe32_ctrl->share_ctrl->vfebase +
			VFE_IRQ_MASK_0);
	irq_mask &= ~(0x000FE021|VFE_IRQ_STATUS0_STATS_COMPOSIT_MASK);

	msm_camera_io_w(irq_mask, vfe32_ctrl->share_ctrl->vfebase +
		VFE_IRQ_MASK_0);
	vfe32_ctrl->share_ctrl->operation_mode &=
		(VFE_OUTPUTS_RDI0|VFE_OUTPUTS_RDI1|VFE_OUTPUTS_RDI2);
	vfe32_ctrl->share_ctrl->comp_output_mode &=
			(VFE32_OUTPUT_MODE_TERTIARY1|
			VFE32_OUTPUT_MODE_TERTIARY2|
			VFE32_OUTPUT_MODE_TERTIARY3);

	/* enable reset_ack interrupt.  */
	irq_mask1 = msm_camera_io_r(
		vfe32_ctrl->share_ctrl->vfebase + VFE_IRQ_MASK_1);
	irq_mask1 |= VFE_IMASK_WHILE_STOPPING_1;
	msm_camera_io_w(irq_mask1,
		vfe32_ctrl->share_ctrl->vfebase + VFE_IRQ_MASK_1);
	msm_camera_io_w_mb(VFE_ONLY_RESET_CMD,
		vfe32_ctrl->share_ctrl->vfebase + VFE_GLOBAL_RESET);

	return wait_for_completion_interruptible(
			&vfe32_ctrl->share_ctrl->reset_complete);
}

static int axi_reset(struct axi_ctrl_t *axi_ctrl,
	struct msm_camera_vfe_params_t vfe_params)
{
	int rc = 0;
	if (vfe_params.skip_reset) {
		axi_reset_internal_variables(axi_ctrl, vfe_params);
		return rc;
	}
	axi_global_reset_internal_variables(axi_ctrl);
	/* disable all interrupts.  vfeImaskLocal is also reset to 0
	* to begin with. */
	msm_camera_io_w(VFE_DISABLE_ALL_IRQS,
		axi_ctrl->share_ctrl->vfebase + VFE_IRQ_MASK_0);

	msm_camera_io_w(VFE_DISABLE_ALL_IRQS,
		axi_ctrl->share_ctrl->vfebase + VFE_IRQ_MASK_1);

	/* clear all pending interrupts*/
	msm_camera_io_w(VFE_CLEAR_ALL_IRQS,
		axi_ctrl->share_ctrl->vfebase + VFE_IRQ_CLEAR_0);
	msm_camera_io_w(VFE_CLEAR_ALL_IRQS,
		axi_ctrl->share_ctrl->vfebase + VFE_IRQ_CLEAR_1);

	/* Ensure the write order while writing
	to the command register using the barrier */
	msm_camera_io_w_mb(1, axi_ctrl->share_ctrl->vfebase + VFE_IRQ_CMD);

	/* enable reset_ack interrupt.  */
	msm_camera_io_w(VFE_IMASK_WHILE_STOPPING_1,
		axi_ctrl->share_ctrl->vfebase + VFE_IRQ_MASK_1);

	/* Write to VFE_GLOBAL_RESET_CMD to reset the vfe hardware. Once reset
	 * is done, hardware interrupt will be generated.  VFE ist processes
	 * the interrupt to complete the function call.  Note that the reset
	 * function is synchronous. */

	/* Ensure the write order while writing
	to the command register using the barrier */
	msm_camera_io_w_mb(VFE_RESET_UPON_RESET_CMD,
		axi_ctrl->share_ctrl->vfebase + VFE_GLOBAL_RESET);

	return wait_for_completion_interruptible(
			&axi_ctrl->share_ctrl->reset_complete);
}

static int vfe32_operation_config(uint32_t *cmd,
			struct vfe32_ctrl_type *vfe32_ctrl)
{
	uint32_t *p = cmd;

	vfe32_ctrl->share_ctrl->stats_comp = *(++p);
	vfe32_ctrl->hfr_mode = *(++p);

	msm_camera_io_w(*(++p),
		vfe32_ctrl->share_ctrl->vfebase + VFE_CFG);
	msm_camera_io_w(*(++p),
		vfe32_ctrl->share_ctrl->vfebase + VFE_MODULE_CFG);
	msm_camera_io_w(*(++p),
		vfe32_ctrl->share_ctrl->vfebase + VFE_REALIGN_BUF);
	msm_camera_io_w(*(++p),
		vfe32_ctrl->share_ctrl->vfebase + VFE_CHROMA_UP);
	msm_camera_io_w(*(++p),
		vfe32_ctrl->share_ctrl->vfebase + VFE_STATS_CFG);
	return 0;
}

static unsigned long vfe32_stats_dqbuf(struct vfe32_ctrl_type *vfe32_ctrl,
	enum msm_stats_enum_type stats_type)
{
	struct msm_stats_meta_buf *buf = NULL;
	int rc = 0;
	rc = vfe32_ctrl->stats_ops.dqbuf(
			vfe32_ctrl->stats_ops.stats_ctrl, stats_type, &buf);
	if (rc < 0) {
		CDBG("%s: dq stats buf (type = %d) err = %d",
			__func__, stats_type, rc);
		return 0L;
	}
	return buf->paddr;
}

static unsigned long vfe32_stats_flush_enqueue(
	struct vfe32_ctrl_type *vfe32_ctrl,
	enum msm_stats_enum_type stats_type)
{
	struct msm_stats_bufq *bufq = NULL;
	struct msm_stats_meta_buf *stats_buf = NULL;
	int rc = 0;
	int i;

	/*
	 * Passing NULL for ion client as the buffers are already
	 * mapped at this stage, client is not required, flush all
	 * the buffers, and buffers move to PREPARE state
	 */

	rc = vfe32_ctrl->stats_ops.bufq_flush(
			vfe32_ctrl->stats_ops.stats_ctrl, stats_type, NULL);
	if (rc < 0) {
		pr_err("%s: dq stats buf (type = %d) err = %d",
			__func__, stats_type, rc);
		return 0L;
	}
	/* Queue all the buffers back to QUEUED state */
	bufq = vfe32_ctrl->stats_ctrl.bufq[stats_type];
	for (i = 0; i < bufq->num_bufs; i++) {
		stats_buf = &bufq->bufs[i];
		rc = vfe32_ctrl->stats_ops.enqueue_buf(
				vfe32_ctrl->stats_ops.stats_ctrl,
				&(stats_buf->info), NULL, -1);
		if (rc < 0) {
			pr_err("%s: dq stats buf (type = %d) err = %d",
				 __func__, stats_type, rc);
			return rc;
		}
	}
	return 0L;
}


static unsigned long vfe32_stats_unregbuf(
	struct vfe32_ctrl_type *vfe32_ctrl,
	struct msm_stats_reqbuf *req_buf, int domain_num)
{
	int i = 0, rc = 0;

	for (i = 0; i < req_buf->num_buf; i++) {
		rc = vfe32_ctrl->stats_ops.buf_unprepare(
			vfe32_ctrl->stats_ops.stats_ctrl,
			req_buf->stats_type, i,
			vfe32_ctrl->stats_ops.client, domain_num);
		if (rc < 0) {
			pr_err("%s: unreg stats buf (type = %d) err = %d",
				__func__, req_buf->stats_type, rc);
		return rc;
		}
	}
	return 0L;
}
static int vfe_stats_awb_buf_init(
	struct vfe32_ctrl_type *vfe32_ctrl,
	struct vfe_cmd_stats_buf *in)
{
	uint32_t addr;
	unsigned long flags = 0;

	spin_lock_irqsave(&vfe32_ctrl->stats_bufq_lock, flags);
	addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl, MSM_STATS_TYPE_AWB);
	spin_unlock_irqrestore(&vfe32_ctrl->stats_bufq_lock, flags);
	if (!addr) {
		pr_err("%s: dq awb ping buf from free buf queue", __func__);
		return -ENOMEM;
	}
	msm_camera_io_w(addr,
		vfe32_ctrl->share_ctrl->vfebase +
		VFE_BUS_STATS_AWB_WR_PING_ADDR);
	spin_lock_irqsave(&vfe32_ctrl->stats_bufq_lock, flags);
	addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl, MSM_STATS_TYPE_AWB);
	spin_unlock_irqrestore(&vfe32_ctrl->stats_bufq_lock, flags);
	if (!addr) {
		pr_err("%s: dq awb ping buf from free buf queue",
			__func__);
		return -ENOMEM;
	}
	msm_camera_io_w(addr,
		vfe32_ctrl->share_ctrl->vfebase +
		VFE_BUS_STATS_AWB_WR_PONG_ADDR);
	return 0;
}

static uint32_t vfe_stats_aec_bg_buf_init(
	struct vfe32_ctrl_type *vfe32_ctrl)
{
	uint32_t addr;
	unsigned long flags = 0;
	uint32_t stats_type;

	stats_type =
		(!vfe32_use_bayer_stats(vfe32_ctrl)) ? MSM_STATS_TYPE_AEC
			: MSM_STATS_TYPE_BG;
	spin_lock_irqsave(&vfe32_ctrl->stats_bufq_lock, flags);
	addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl, stats_type);
	spin_unlock_irqrestore(&vfe32_ctrl->stats_bufq_lock, flags);
	if (!addr) {
		pr_err("%s: dq aec ping buf from free buf queue",
			__func__);
		return -ENOMEM;
	}
	msm_camera_io_w(addr,
		vfe32_ctrl->share_ctrl->vfebase +
		VFE_BUS_STATS_AEC_BG_WR_PING_ADDR);
	spin_lock_irqsave(&vfe32_ctrl->stats_bufq_lock, flags);
	addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl, stats_type);
	spin_unlock_irqrestore(&vfe32_ctrl->stats_bufq_lock, flags);
	if (!addr) {
		pr_err("%s: dq aec pong buf from free buf queue",
			__func__);
		return -ENOMEM;
	}
	msm_camera_io_w(addr,
		vfe32_ctrl->share_ctrl->vfebase +
		VFE_BUS_STATS_AEC_BG_WR_PONG_ADDR);
	return 0;
}

static int vfe_stats_af_bf_buf_init(
	struct vfe32_ctrl_type *vfe32_ctrl)
{
	uint32_t addr;
	unsigned long flags = 0;
	int rc = 0;

	uint32_t stats_type;
	stats_type =
		(!vfe32_use_bayer_stats(vfe32_ctrl)) ? MSM_STATS_TYPE_AF
			: MSM_STATS_TYPE_BF;

	spin_lock_irqsave(&vfe32_ctrl->stats_bufq_lock, flags);
	rc = vfe32_stats_flush_enqueue(vfe32_ctrl, stats_type);
	if (rc < 0) {
		pr_err("%s: dq stats buf err = %d",
			   __func__, rc);
		spin_unlock_irqrestore(&vfe32_ctrl->stats_bufq_lock, flags);
		return -EINVAL;
	}
	addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl, stats_type);
	spin_unlock_irqrestore(&vfe32_ctrl->stats_bufq_lock, flags);
	if (!addr) {
		pr_err("%s: dq af ping buf from free buf queue", __func__);
		return -ENOMEM;
	}
	msm_camera_io_w(addr,
		vfe32_ctrl->share_ctrl->vfebase +
		VFE_BUS_STATS_AF_BF_WR_PING_ADDR);
	spin_lock_irqsave(&vfe32_ctrl->stats_bufq_lock, flags);
	addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl, stats_type);
	spin_unlock_irqrestore(&vfe32_ctrl->stats_bufq_lock, flags);
	if (!addr) {
		pr_err("%s: dq af pong buf from free buf queue", __func__);
		return -ENOMEM;
	}
	msm_camera_io_w(addr,
		vfe32_ctrl->share_ctrl->vfebase +
		VFE_BUS_STATS_AF_BF_WR_PONG_ADDR);
	return 0;
}

static uint32_t vfe_stats_bhist_buf_init(
	struct vfe32_ctrl_type *vfe32_ctrl)
{
	uint32_t addr;
	unsigned long flags = 0;

	spin_lock_irqsave(&vfe32_ctrl->stats_bufq_lock, flags);
	addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl, MSM_STATS_TYPE_BHIST);
	spin_unlock_irqrestore(&vfe32_ctrl->stats_bufq_lock, flags);
	if (!addr) {
		pr_err("%s: dq ihist ping buf from free buf queue",
			__func__);
		return -ENOMEM;
	}
	msm_camera_io_w(addr,
		vfe32_ctrl->share_ctrl->vfebase +
		VFE_BUS_STATS_SKIN_BHIST_WR_PING_ADDR);
	spin_lock_irqsave(&vfe32_ctrl->stats_bufq_lock, flags);
	addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl, MSM_STATS_TYPE_BHIST);
	spin_unlock_irqrestore(&vfe32_ctrl->stats_bufq_lock, flags);
	if (!addr) {
		pr_err("%s: dq ihist pong buf from free buf queue",
			__func__);
		return -ENOMEM;
	}
	msm_camera_io_w(addr,
		vfe32_ctrl->share_ctrl->vfebase +
		VFE_BUS_STATS_SKIN_BHIST_WR_PONG_ADDR);

	return 0;
}

static int vfe_stats_ihist_buf_init(
	struct vfe32_ctrl_type *vfe32_ctrl)
{
	uint32_t addr;
	unsigned long flags = 0;

	spin_lock_irqsave(&vfe32_ctrl->stats_bufq_lock, flags);
	addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl, MSM_STATS_TYPE_IHIST);
	spin_unlock_irqrestore(&vfe32_ctrl->stats_bufq_lock, flags);
	if (!addr) {
		pr_err("%s: dq ihist ping buf from free buf queue",
			__func__);
		return -ENOMEM;
	}
	msm_camera_io_w(addr,
		vfe32_ctrl->share_ctrl->vfebase +
		VFE_BUS_STATS_HIST_WR_PING_ADDR);
	spin_lock_irqsave(&vfe32_ctrl->stats_bufq_lock, flags);
	addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl, MSM_STATS_TYPE_IHIST);
	spin_unlock_irqrestore(&vfe32_ctrl->stats_bufq_lock, flags);
	if (!addr) {
		pr_err("%s: dq ihist pong buf from free buf queue",
			__func__);
		return -ENOMEM;
	}
	msm_camera_io_w(addr,
		vfe32_ctrl->share_ctrl->vfebase +
		VFE_BUS_STATS_HIST_WR_PONG_ADDR);

	return 0;
}

static int vfe_stats_rs_buf_init(
	struct vfe32_ctrl_type *vfe32_ctrl)
{
	uint32_t addr;
	unsigned long flags = 0;

	spin_lock_irqsave(&vfe32_ctrl->stats_bufq_lock, flags);
	addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl, MSM_STATS_TYPE_RS);
	spin_unlock_irqrestore(&vfe32_ctrl->stats_bufq_lock, flags);
	if (!addr) {
		pr_err("%s: dq rs ping buf from free buf queue", __func__);
		return -ENOMEM;
	}
	msm_camera_io_w(addr,
		vfe32_ctrl->share_ctrl->vfebase +
		VFE_BUS_STATS_RS_WR_PING_ADDR);
	spin_lock_irqsave(&vfe32_ctrl->stats_bufq_lock, flags);
	addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl, MSM_STATS_TYPE_RS);
	spin_unlock_irqrestore(&vfe32_ctrl->stats_bufq_lock, flags);
	if (!addr) {
		pr_err("%s: dq rs pong buf from free buf queue", __func__);
		return -ENOMEM;
	}
	msm_camera_io_w(addr,
		vfe32_ctrl->share_ctrl->vfebase +
		VFE_BUS_STATS_RS_WR_PONG_ADDR);
	return 0;
}

static int vfe_stats_cs_buf_init(
	struct vfe32_ctrl_type *vfe32_ctrl)
{
	uint32_t addr;
	unsigned long flags = 0;
	spin_lock_irqsave(&vfe32_ctrl->stats_bufq_lock, flags);
	addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl, MSM_STATS_TYPE_CS);
	spin_unlock_irqrestore(&vfe32_ctrl->stats_bufq_lock, flags);
	if (!addr) {
		pr_err("%s: dq cs ping buf from free buf queue", __func__);
		return -ENOMEM;
	}
	msm_camera_io_w(addr,
		vfe32_ctrl->share_ctrl->vfebase +
		VFE_BUS_STATS_CS_WR_PING_ADDR);
	spin_lock_irqsave(&vfe32_ctrl->stats_bufq_lock, flags);
	addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl, MSM_STATS_TYPE_CS);
	spin_unlock_irqrestore(&vfe32_ctrl->stats_bufq_lock, flags);
	if (!addr) {
		pr_err("%s: dq cs pong buf from free buf queue", __func__);
		return -ENOMEM;
	}
	msm_camera_io_w(addr,
		vfe32_ctrl->share_ctrl->vfebase +
		VFE_BUS_STATS_CS_WR_PONG_ADDR);
	return 0;
}

static void vfe32_start_common(struct vfe32_ctrl_type *vfe32_ctrl)
{
	CDBG("VFE opertaion mode = 0x%x, output mode = 0x%x\n",
		vfe32_ctrl->share_ctrl->operation_mode,
		vfe32_ctrl->share_ctrl->outpath.output_mode);
		msm_camera_io_w_mb(1, vfe32_ctrl->share_ctrl->vfebase +
			VFE_CAMIF_COMMAND);
		msm_camera_io_w_mb(VFE_AXI_CFG_MASK,
				vfe32_ctrl->share_ctrl->vfebase + VFE_AXI_CFG);
}

static int vfe32_start_recording(
	struct msm_cam_media_controller *pmctl,
	struct vfe32_ctrl_type *vfe32_ctrl)
{
	vfe32_ctrl->share_ctrl->recording_state = VFE_STATE_START_REQUESTED;
	msm_camera_io_w_mb(1,
		vfe32_ctrl->share_ctrl->vfebase + VFE_REG_UPDATE_CMD);
	return 0;
}

static int vfe32_stop_recording(
	struct msm_cam_media_controller *pmctl,
	struct vfe32_ctrl_type *vfe32_ctrl)
{
	vfe32_ctrl->share_ctrl->recording_state = VFE_STATE_STOP_REQUESTED;
	msm_camera_io_w_mb(1,
		vfe32_ctrl->share_ctrl->vfebase + VFE_REG_UPDATE_CMD);
	return 0;
}

static void vfe32_stop_recording_done(struct msm_cam_media_controller *pmctl)
{
	msm_camio_bus_scale_cfg(pmctl->sdata->pdata->cam_bus_scale_table,
							S_PREVIEW);
}

static void vfe32_start_liveshot(
	struct msm_cam_media_controller *pmctl,
	struct vfe32_ctrl_type *vfe32_ctrl)
{
	/* Hardcode 1 live snapshot for now. */
	vfe32_ctrl->share_ctrl->outpath.out0.capture_cnt = 1;
	vfe32_ctrl->share_ctrl->vfe_capture_count =
		vfe32_ctrl->share_ctrl->outpath.out0.capture_cnt;

	vfe32_ctrl->share_ctrl->liveshot_state = VFE_STATE_START_REQUESTED;
	msm_camera_io_w_mb(1, vfe32_ctrl->
		share_ctrl->vfebase + VFE_REG_UPDATE_CMD);
}

static void vfe32_stop_liveshot(
	struct msm_cam_media_controller *pmctl,
	struct vfe32_ctrl_type *vfe32_ctrl)
{
	vfe32_ctrl->share_ctrl->liveshot_state = VFE_STATE_STOP_REQUESTED;
	msm_camera_io_w_mb(1,
		vfe32_ctrl->share_ctrl->vfebase + VFE_REG_UPDATE_CMD);
}

static int vfe32_zsl(
	struct msm_cam_media_controller *pmctl,
	struct vfe32_ctrl_type *vfe32_ctrl)
{
	vfe32_ctrl->share_ctrl->start_ack_pending = TRUE;
	vfe32_start_common(vfe32_ctrl);

	msm_camera_io_w(1, vfe32_ctrl->share_ctrl->vfebase + 0x18C);
	msm_camera_io_w(1, vfe32_ctrl->share_ctrl->vfebase + 0x188);
	return 0;
}
static int vfe32_capture_raw(
	struct msm_cam_media_controller *pmctl,
	struct vfe32_ctrl_type *vfe32_ctrl,
	uint32_t num_frames_capture)
{
	vfe32_ctrl->share_ctrl->outpath.out0.capture_cnt = num_frames_capture;
	vfe32_ctrl->share_ctrl->vfe_capture_count = num_frames_capture;
	vfe32_start_common(vfe32_ctrl);
	return 0;
}

static int vfe32_capture(
	struct msm_cam_media_controller *pmctl,
	uint32_t num_frames_capture,
	struct vfe32_ctrl_type *vfe32_ctrl)
{
	/* capture command is valid for both idle and active state. */
	vfe32_ctrl->share_ctrl->outpath.out1.capture_cnt = num_frames_capture;
	if (vfe32_ctrl->share_ctrl->current_mode ==
			VFE_OUTPUTS_MAIN_AND_THUMB ||
		vfe32_ctrl->share_ctrl->current_mode ==
			VFE_OUTPUTS_THUMB_AND_MAIN ||
		vfe32_ctrl->share_ctrl->current_mode ==
			VFE_OUTPUTS_JPEG_AND_THUMB ||
		vfe32_ctrl->share_ctrl->current_mode ==
			VFE_OUTPUTS_THUMB_AND_JPEG) {
		vfe32_ctrl->share_ctrl->outpath.out0.capture_cnt =
			num_frames_capture;
	}

	vfe32_ctrl->share_ctrl->vfe_capture_count = num_frames_capture;

	vfe32_ctrl->share_ctrl->vfe_capture_count = num_frames_capture;

	vfe32_start_common(vfe32_ctrl);
	/* for debug */
	msm_camera_io_w(1, vfe32_ctrl->share_ctrl->vfebase + 0x18C);
	msm_camera_io_w(1, vfe32_ctrl->share_ctrl->vfebase + 0x188);
	return 0;
}

static int vfe32_start(
	struct msm_cam_media_controller *pmctl,
	struct vfe32_ctrl_type *vfe32_ctrl)
{
	vfe32_start_common(vfe32_ctrl);
	return 0;
}

static void vfe32_update(struct vfe32_ctrl_type *vfe32_ctrl)
{
	unsigned long flags = 0;
	uint32_t value = 0;
	if (vfe32_ctrl->update_linear) {
		if (!msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase +
			V32_LINEARIZATION_OFF1))
			msm_camera_io_w(1,
				vfe32_ctrl->share_ctrl->vfebase +
				V32_LINEARIZATION_OFF1);
		else
			msm_camera_io_w(0,
				vfe32_ctrl->share_ctrl->vfebase +
				V32_LINEARIZATION_OFF1);
		vfe32_ctrl->update_linear = false;
	}

	if (vfe32_ctrl->update_rolloff) {
		value = msm_camera_io_r(vfe32_ctrl->share_ctrl->vfebase +
			V33_PCA_ROLL_OFF_CFG_OFF1);
		value ^= V33_PCA_ROLL_OFF_LUT_BANK_SEL_MASK;
		msm_camera_io_w(value, vfe32_ctrl->share_ctrl->vfebase +
			V33_PCA_ROLL_OFF_CFG_OFF1);
		vfe32_ctrl->update_rolloff = false;
	}

	if (vfe32_ctrl->update_la) {
		if (!msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase + V32_LA_OFF))
			msm_camera_io_w(1,
				vfe32_ctrl->share_ctrl->vfebase + V32_LA_OFF);
		else
			msm_camera_io_w(0,
				vfe32_ctrl->share_ctrl->vfebase + V32_LA_OFF);
		vfe32_ctrl->update_la = false;
	}

	if (vfe32_ctrl->update_gamma) {
		value = msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase + V32_RGB_G_OFF);
		value ^= V32_GAMMA_LUT_BANK_SEL_MASK;
		msm_camera_io_w(value,
			vfe32_ctrl->share_ctrl->vfebase + V32_RGB_G_OFF);
		vfe32_ctrl->update_gamma = false;
	}

	spin_lock_irqsave(&vfe32_ctrl->share_ctrl->update_ack_lock, flags);
	vfe32_ctrl->share_ctrl->update_ack_pending = TRUE;
	spin_unlock_irqrestore(&vfe32_ctrl->share_ctrl->update_ack_lock, flags);
	/* Ensure the write order while writing
	to the command register using the barrier */
	msm_camera_io_w_mb(1,
		vfe32_ctrl->share_ctrl->vfebase + VFE_REG_UPDATE_CMD);
	return;
}

static void vfe32_sync_timer_stop(struct vfe32_ctrl_type *vfe32_ctrl)
{
	uint32_t value = 0;
	vfe32_ctrl->sync_timer_state = 0;
	if (vfe32_ctrl->sync_timer_number == 0)
		value = 0x10000;
	else if (vfe32_ctrl->sync_timer_number == 1)
		value = 0x20000;
	else if (vfe32_ctrl->sync_timer_number == 2)
		value = 0x40000;

	/* Timer Stop */
	msm_camera_io_w(value,
		vfe32_ctrl->share_ctrl->vfebase + V32_SYNC_TIMER_OFF);
}

static void vfe32_sync_timer_start(
	const uint32_t *tbl,
	struct vfe32_ctrl_type *vfe32_ctrl)
{
	/* set bit 8 for auto increment. */
	uint32_t value = 1;
	uint32_t val;

	vfe32_ctrl->sync_timer_state = *tbl++;
	vfe32_ctrl->sync_timer_repeat_count = *tbl++;
	vfe32_ctrl->sync_timer_number = *tbl++;
	CDBG("%s timer_state %d, repeat_cnt %d timer number %d\n",
		 __func__, vfe32_ctrl->sync_timer_state,
		 vfe32_ctrl->sync_timer_repeat_count,
		 vfe32_ctrl->sync_timer_number);

	if (vfe32_ctrl->sync_timer_state) { /* Start Timer */
		value = value << vfe32_ctrl->sync_timer_number;
	} else { /* Stop Timer */
		CDBG("Failed to Start timer\n");
		return;
	}

	/* Timer Start */
	msm_camera_io_w(value,
		vfe32_ctrl->share_ctrl->vfebase + V32_SYNC_TIMER_OFF);
	/* Sync Timer Line Start */
	value = *tbl++;
	msm_camera_io_w(value,
		vfe32_ctrl->share_ctrl->vfebase + V32_SYNC_TIMER_OFF +
		4 + ((vfe32_ctrl->sync_timer_number) * 12));
	/* Sync Timer Pixel Start */
	value = *tbl++;
	msm_camera_io_w(value,
			vfe32_ctrl->share_ctrl->vfebase + V32_SYNC_TIMER_OFF +
			 8 + ((vfe32_ctrl->sync_timer_number) * 12));
	/* Sync Timer Pixel Duration */
	value = *tbl++;
	val = vfe_clk_rate / 10000;
	val = 10000000 / val;
	val = value * 10000 / val;
	CDBG("%s: Pixel Clk Cycles!!! %d\n", __func__, val);
	msm_camera_io_w(val,
		vfe32_ctrl->share_ctrl->vfebase + V32_SYNC_TIMER_OFF +
		12 + ((vfe32_ctrl->sync_timer_number) * 12));
	/* Timer0 Active High/LOW */
	value = *tbl++;
	msm_camera_io_w(value,
		vfe32_ctrl->share_ctrl->vfebase + V32_SYNC_TIMER_POLARITY_OFF);
	/* Selects sync timer 0 output to drive onto timer1 port */
	value = 0;
	msm_camera_io_w(value,
		vfe32_ctrl->share_ctrl->vfebase + V32_TIMER_SELECT_OFF);
}


static void vfe32_write_gamma_cfg(
	enum VFE32_DMI_RAM_SEL channel_sel,
	const uint32_t *tbl,
	struct vfe32_ctrl_type *vfe32_ctrl)
{
	int i;
	uint32_t value, value1, value2;
	vfe32_program_dmi_cfg(channel_sel, vfe32_ctrl);
	for (i = 0 ; i < (VFE32_GAMMA_NUM_ENTRIES/2) ; i++) {
		value = *tbl++;
		value1 = value & 0x0000FFFF;
		value2 = (value & 0xFFFF0000)>>16;
		msm_camera_io_w((value1),
			vfe32_ctrl->share_ctrl->vfebase + VFE_DMI_DATA_LO);
		msm_camera_io_w((value2),
			vfe32_ctrl->share_ctrl->vfebase + VFE_DMI_DATA_LO);
	}
	vfe32_program_dmi_cfg(NO_MEM_SELECTED, vfe32_ctrl);
}

static void vfe32_read_gamma_cfg(
	enum VFE32_DMI_RAM_SEL channel_sel,
	uint32_t *tbl,
	struct vfe32_ctrl_type *vfe32_ctrl)
{
	int i;
	vfe32_program_dmi_cfg(channel_sel, vfe32_ctrl);
	CDBG("%s: Gamma table channel: %d\n", __func__, channel_sel);
	for (i = 0 ; i < VFE32_GAMMA_NUM_ENTRIES ; i++) {
		*tbl = msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase + VFE_DMI_DATA_LO);
		CDBG("%s: %08x\n", __func__, *tbl);
		tbl++;
	}
	vfe32_program_dmi_cfg(NO_MEM_SELECTED, vfe32_ctrl);
}

static void vfe32_write_la_cfg(
	enum VFE32_DMI_RAM_SEL channel_sel,
	const uint32_t *tbl,
	struct vfe32_ctrl_type *vfe32_ctrl)
{
	uint32_t i;
	uint32_t value, value1, value2;

	vfe32_program_dmi_cfg(channel_sel, vfe32_ctrl);
	for (i = 0 ; i < (VFE32_LA_TABLE_LENGTH/2) ; i++) {
		value = *tbl++;
		value1 = value & 0x0000FFFF;
		value2 = (value & 0xFFFF0000)>>16;
		msm_camera_io_w((value1),
			vfe32_ctrl->share_ctrl->vfebase + VFE_DMI_DATA_LO);
		msm_camera_io_w((value2),
			vfe32_ctrl->share_ctrl->vfebase + VFE_DMI_DATA_LO);
	}
	vfe32_program_dmi_cfg(NO_MEM_SELECTED, vfe32_ctrl);
}

static struct vfe32_output_ch *vfe32_get_ch(
	int path, struct vfe_share_ctrl_t *share_ctrl)
{
	struct vfe32_output_ch *ch = NULL;

	if (path == VFE_MSG_OUTPUT_PRIMARY)
		ch = &share_ctrl->outpath.out0;
	else if (path == VFE_MSG_OUTPUT_SECONDARY)
		ch = &share_ctrl->outpath.out1;
	else if (path == VFE_MSG_OUTPUT_TERTIARY1)
		ch = &share_ctrl->outpath.out2;
	else if (path == VFE_MSG_OUTPUT_TERTIARY2)
		ch = &share_ctrl->outpath.out3;
	else if (path == VFE_MSG_OUTPUT_TERTIARY3)
		ch = &share_ctrl->outpath.out4;
	else
		pr_err("%s: Invalid path %d\n", __func__,
			path);

	BUG_ON(ch == NULL);
	return ch;
}
static struct msm_free_buf *vfe32_check_free_buffer(
	int id, int path, struct axi_ctrl_t *axi_ctrl)
{
	struct vfe32_output_ch *outch = NULL;
	struct msm_free_buf *b = NULL;
	uint32_t inst_handle = 0;

	if (path == VFE_MSG_OUTPUT_PRIMARY)
		inst_handle = axi_ctrl->share_ctrl->outpath.out0.inst_handle;
	else if (path == VFE_MSG_OUTPUT_SECONDARY)
		inst_handle = axi_ctrl->share_ctrl->outpath.out1.inst_handle;
	else if (path == VFE_MSG_OUTPUT_TERTIARY1)
		inst_handle = axi_ctrl->share_ctrl->outpath.out2.inst_handle;
	else if (path == VFE_MSG_OUTPUT_TERTIARY2)
		inst_handle = axi_ctrl->share_ctrl->outpath.out3.inst_handle;
	else if (path == VFE_MSG_OUTPUT_TERTIARY3)
		inst_handle = axi_ctrl->share_ctrl->outpath.out4.inst_handle;

	vfe32_subdev_notify(id, path, inst_handle,
		&axi_ctrl->subdev, axi_ctrl->share_ctrl);
	outch = vfe32_get_ch(path, axi_ctrl->share_ctrl);
	if (outch->free_buf.ch_paddr[0])
		b = &outch->free_buf;
	return b;
}
static int configure_pingpong_buffers(
	int id, int path, struct axi_ctrl_t *axi_ctrl)
{
	struct vfe32_output_ch *outch = NULL;
	int rc = 0;
	uint32_t inst_handle = 0;
	uint32_t rdi_comp_select = 0;
	static uint32_t ping_t1_ch0_paddr, pong_t1_ch0_paddr;

	if (path == VFE_MSG_OUTPUT_PRIMARY)
		inst_handle = axi_ctrl->share_ctrl->outpath.out0.inst_handle;
	else if (path == VFE_MSG_OUTPUT_SECONDARY)
		inst_handle = axi_ctrl->share_ctrl->outpath.out1.inst_handle;
	else if (path == VFE_MSG_OUTPUT_TERTIARY1)
		inst_handle = axi_ctrl->share_ctrl->outpath.out2.inst_handle;
	else if (path == VFE_MSG_OUTPUT_TERTIARY2)
		inst_handle = axi_ctrl->share_ctrl->outpath.out3.inst_handle;
	else if (path == VFE_MSG_OUTPUT_TERTIARY3)
		inst_handle = axi_ctrl->share_ctrl->outpath.out4.inst_handle;

	CDBG("%s path %d, inst_handle 0x%x\n", __func__, path, inst_handle);
	if ((axi_ctrl->share_ctrl->rdi_comp == VFE_RDI_COMPOSITE) &&
		path == VFE_MSG_OUTPUT_TERTIARY2) {
		/* Do Nothing, since buf address will be
			fetched in TERTIARY1 case */
	} else {
		vfe32_subdev_notify(id, path, inst_handle,
			&axi_ctrl->subdev, axi_ctrl->share_ctrl);
	}

	outch = vfe32_get_ch(path, axi_ctrl->share_ctrl);

	if ((axi_ctrl->share_ctrl->rdi_comp == VFE_RDI_COMPOSITE) &&
		(path == VFE_MSG_OUTPUT_TERTIARY1)) {
		ping_t1_ch0_paddr = outch->ping.ch_paddr[1];
		pong_t1_ch0_paddr = outch->pong.ch_paddr[1];
	}
	rdi_comp_select =
		(axi_ctrl->share_ctrl->rdi_comp  == VFE_RDI_COMPOSITE) &&
		(path == VFE_MSG_OUTPUT_TERTIARY2) && ping_t1_ch0_paddr &&
		pong_t1_ch0_paddr;
	if (rdi_comp_select) {
		vfe32_put_ch_ping_addr(
			axi_ctrl->share_ctrl->vfebase, outch->ch0,
			ping_t1_ch0_paddr);
		vfe32_put_ch_pong_addr(
			axi_ctrl->share_ctrl->vfebase, outch->ch0,
			pong_t1_ch0_paddr);
		    ping_t1_ch0_paddr = 0;
			pong_t1_ch0_paddr = 0;

		memset(&outch->ping, 0, sizeof(struct msm_free_buf));
		memset(&outch->pong, 0, sizeof(struct msm_free_buf));
	} else if (outch->ping.ch_paddr[0] && outch->pong.ch_paddr[0]) {
		/* Configure Preview Ping Pong */
		CDBG("%s Configure ping/pong address for %d\n",
						__func__, path);
		CDBG("%s Ping/pong address 0x%x, 0x%x, 0x%x, 0x%x\n",
			__func__, outch->ping.ch_paddr[0],
			outch->pong.ch_paddr[0], outch->ping.ch_paddr[1],
			outch->pong.ch_paddr[1]);
		vfe32_put_ch_ping_addr(
			axi_ctrl->share_ctrl->vfebase, outch->ch0,
			outch->ping.ch_paddr[0]);
		vfe32_put_ch_pong_addr(
			axi_ctrl->share_ctrl->vfebase, outch->ch0,
			outch->pong.ch_paddr[0]);

		if ((axi_ctrl->share_ctrl->current_mode !=
			VFE_OUTPUTS_RAW) && (path != VFE_MSG_OUTPUT_TERTIARY1)
			&& (path != VFE_MSG_OUTPUT_TERTIARY2)
			&& (path != VFE_MSG_OUTPUT_TERTIARY3)) {
			vfe32_put_ch_ping_addr(
				axi_ctrl->share_ctrl->vfebase, outch->ch1,
				outch->ping.ch_paddr[1]);
			vfe32_put_ch_pong_addr(
				axi_ctrl->share_ctrl->vfebase, outch->ch1,
				outch->pong.ch_paddr[1]);
		}

		if (outch->ping.num_planes > 2)
			vfe32_put_ch_ping_addr(
				axi_ctrl->share_ctrl->vfebase, outch->ch2,
				outch->ping.ch_paddr[2]);
		if (outch->pong.num_planes > 2)
			vfe32_put_ch_pong_addr(
				axi_ctrl->share_ctrl->vfebase, outch->ch2,
				outch->pong.ch_paddr[2]);

		/* avoid stale info */
		memset(&outch->ping, 0, sizeof(struct msm_free_buf));
		memset(&outch->pong, 0, sizeof(struct msm_free_buf));
	} else {
		pr_err("%s ping/pong addr is null!!", __func__);
		rc = -EINVAL;
	}
	return rc;
}

static void vfe32_write_linear_cfg(
	enum VFE32_DMI_RAM_SEL channel_sel,
	const uint32_t *tbl, struct vfe32_ctrl_type *vfe32_ctrl)
{
	uint32_t i;

	vfe32_program_dmi_cfg(channel_sel, vfe32_ctrl);
	/* for loop for configuring LUT. */
	for (i = 0 ; i < VFE32_LINEARIZATON_TABLE_LENGTH ; i++) {
		msm_camera_io_w(*tbl,
			vfe32_ctrl->share_ctrl->vfebase + VFE_DMI_DATA_LO);
		tbl++;
	}
	CDBG("done writing to linearization table\n");
	vfe32_program_dmi_cfg(NO_MEM_SELECTED, vfe32_ctrl);
}

static void vfe32_send_isp_msg(
	struct v4l2_subdev *sd,
	uint32_t vfeFrameId,
	uint32_t isp_msg_id)
{
	struct isp_msg_event isp_msg_evt;

	isp_msg_evt.msg_id = isp_msg_id;
	isp_msg_evt.sof_count = vfeFrameId;
	v4l2_subdev_notify(sd,
			NOTIFY_ISP_MSG_EVT,
			(void *)&isp_msg_evt);
}

static int vfe32_proc_general(
	struct msm_cam_media_controller *pmctl,
	struct msm_isp_cmd *cmd,
	struct vfe32_ctrl_type *vfe32_ctrl)
{
	int i , rc = 0;
	uint32_t old_val = 0 , new_val = 0, module_val = 0;
	uint32_t *cmdp = NULL;
	uint32_t *cmdp_local = NULL;
	uint32_t snapshot_cnt = 0;
	uint32_t temp1 = 0, temp2 = 0;
	struct msm_camera_vfe_params_t vfe_params;

	CDBG("vfe32_proc_general: cmdID = %s, length = %d\n",
		vfe32_general_cmd[cmd->id], cmd->length);

	if (vfe32_ctrl->share_ctrl->vfebase == NULL || open_fail_flag) {
		pr_err("Error : vfe32_ctrl->vfebase is NULL!!\n");
		pr_err("vfe32_proc_general: cmdID = %s, length = %d\n",
			vfe32_general_cmd[cmd->id], cmd->length);
		rc = -EINVAL;
		goto proc_general_done;
	}

	switch (cmd->id) {
	case VFE_CMD_RESET:
		pr_info("vfe32_proc_general: cmdID = %s\n",
			vfe32_general_cmd[cmd->id]);
		vfe32_ctrl->share_ctrl->vfe_reset_flag = true;
		vfe32_reset(vfe32_ctrl);
		break;
	case VFE_CMD_START:
		pr_info("vfe32_proc_general: cmdID = %s\n",
			vfe32_general_cmd[cmd->id]);
		if (copy_from_user(&vfe_params,
				(void __user *)(cmd->value),
				sizeof(struct msm_camera_vfe_params_t))) {
				rc = -EFAULT;
				goto proc_general_done;
		}

		vfe32_ctrl->share_ctrl->current_mode =
			vfe_params.operation_mode;

		rc = vfe32_start(pmctl, vfe32_ctrl);
		break;
	case VFE_CMD_UPDATE:
		vfe32_update(vfe32_ctrl);
		break;
	case VFE_CMD_CAPTURE_RAW:
		pr_info("%s: cmdID = VFE_CMD_CAPTURE_RAW\n", __func__);
		if (copy_from_user(&vfe_params,
				(void __user *)(cmd->value),
				sizeof(struct msm_camera_vfe_params_t))) {
				rc = -EFAULT;
				goto proc_general_done;
		}

		snapshot_cnt = vfe_params.capture_count;
		vfe32_ctrl->share_ctrl->current_mode =
			vfe_params.operation_mode;
		rc = vfe32_capture_raw(pmctl, vfe32_ctrl, snapshot_cnt);
		break;
	case VFE_CMD_CAPTURE:
		if (copy_from_user(&vfe_params,
				(void __user *)(cmd->value),
				sizeof(struct msm_camera_vfe_params_t))) {
				rc = -EFAULT;
				goto proc_general_done;
		}

		snapshot_cnt = vfe_params.capture_count;
		vfe32_ctrl->share_ctrl->current_mode =
			vfe_params.operation_mode;

		rc = vfe32_capture(pmctl, snapshot_cnt, vfe32_ctrl);
		break;
	case VFE_CMD_START_RECORDING:
		pr_info("vfe32_proc_general: cmdID = %s\n",
			vfe32_general_cmd[cmd->id]);
		rc = vfe32_start_recording(pmctl, vfe32_ctrl);
		break;
	case VFE_CMD_STOP_RECORDING:
		pr_info("vfe32_proc_general: cmdID = %s\n",
			vfe32_general_cmd[cmd->id]);
		rc = vfe32_stop_recording(pmctl, vfe32_ctrl);
		break;
	case VFE_CMD_STOP_RECORDING_DONE:
		vfe32_stop_recording_done(pmctl);
		break;
	case VFE_CMD_OPERATION_CFG: {
		if (cmd->length != V32_OPERATION_CFG_LEN) {
			rc = -EINVAL;
			goto proc_general_done;
		}
		cmdp = kmalloc(V32_OPERATION_CFG_LEN, GFP_ATOMIC);
		if (copy_from_user(cmdp,
			(void __user *)(cmd->value),
			V32_OPERATION_CFG_LEN)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		rc = vfe32_operation_config(cmdp, vfe32_ctrl);
		}
		break;

	case VFE_CMD_STATS_AE_START: {
		if (vfe32_use_bayer_stats(vfe32_ctrl)) {
			/* Error */
			rc = -EFAULT;
			goto proc_general_done;
		}
		rc = vfe_stats_aec_bg_buf_init(vfe32_ctrl);
		if (rc < 0) {
			pr_err("%s: cannot config ping/pong address of AEC",
				 __func__);
			goto proc_general_done;
		}
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp,
			(void __user *)(cmd->value),
			cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		old_val = msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase + VFE_MODULE_CFG);
		old_val |= AE_BG_ENABLE_MASK;
		msm_camera_io_w(old_val,
			vfe32_ctrl->share_ctrl->vfebase + VFE_MODULE_CFG);
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			vfe32_cmd[cmd->id].offset,
			cmdp, (vfe32_cmd[cmd->id].length));
		}
		break;
	case VFE_CMD_STATS_AF_START: {
		if (vfe32_use_bayer_stats(vfe32_ctrl)) {
			/* Error */
			rc = -EFAULT;
			goto proc_general_done;
		}
		rc = vfe_stats_af_bf_buf_init(vfe32_ctrl);
		if (rc < 0) {
			pr_err("%s: cannot config ping/pong address of AF",
				__func__);
			goto proc_general_done;
		}
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp,
			(void __user *)(cmd->value),
			cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		old_val = msm_camera_io_r(vfe32_ctrl->share_ctrl->vfebase +
			VFE_MODULE_CFG);
		old_val |= AF_BF_ENABLE_MASK;
		msm_camera_io_w(old_val,
			vfe32_ctrl->share_ctrl->vfebase + VFE_MODULE_CFG);
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			vfe32_cmd[cmd->id].offset,
			cmdp, (vfe32_cmd[cmd->id].length));
		}
		break;
	case VFE_CMD_STATS_AWB_START: {
		if (vfe32_use_bayer_stats(vfe32_ctrl)) {
			/* Error */
			rc = -EFAULT;
			goto proc_general_done;
		}
		rc = vfe_stats_awb_buf_init(vfe32_ctrl, NULL);
		if (rc < 0) {
			pr_err("%s: cannot config ping/pong address of AWB",
				 __func__);
			goto proc_general_done;
		}
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp,
			(void __user *)(cmd->value),
			cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		old_val = msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase + VFE_MODULE_CFG);
		old_val |= AWB_ENABLE_MASK;
		msm_camera_io_w(old_val,
			vfe32_ctrl->share_ctrl->vfebase + VFE_MODULE_CFG);
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			vfe32_cmd[cmd->id].offset,
			cmdp, (vfe32_cmd[cmd->id].length));
		}
		break;

	case VFE_CMD_STATS_IHIST_START: {
		rc = vfe_stats_ihist_buf_init(vfe32_ctrl);
		if (rc < 0) {
			pr_err("%s: cannot config ping/pong address of IHIST",
				 __func__);
			goto proc_general_done;
		}
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp,
			(void __user *)(cmd->value),
			cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		old_val = msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase + VFE_MODULE_CFG);
		old_val |= IHIST_ENABLE_MASK;
		msm_camera_io_w(old_val,
			vfe32_ctrl->share_ctrl->vfebase + VFE_MODULE_CFG);
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			vfe32_cmd[cmd->id].offset,
			cmdp, (vfe32_cmd[cmd->id].length));
		}
		break;


	case VFE_CMD_STATS_RS_START: {
		rc = vfe_stats_rs_buf_init(vfe32_ctrl);
		if (rc < 0) {
			pr_err("%s: cannot config ping/pong address of RS",
				__func__);
			goto proc_general_done;
		}
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp,
			(void __user *)(cmd->value),
			cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			vfe32_cmd[cmd->id].offset,
			cmdp, (vfe32_cmd[cmd->id].length));
		}
		break;

	case VFE_CMD_STATS_CS_START: {
		rc = vfe_stats_cs_buf_init(vfe32_ctrl);
		if (rc < 0) {
			pr_err("%s: cannot config ping/pong address of CS",
				__func__);
			goto proc_general_done;
		}
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp,
			(void __user *)(cmd->value),
			cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			vfe32_cmd[cmd->id].offset,
			cmdp, (vfe32_cmd[cmd->id].length));
		}
		break;

	case VFE_CMD_STATS_BG_START:
	case VFE_CMD_STATS_BF_START:
	case VFE_CMD_STATS_BHIST_START: {
		if (!vfe32_use_bayer_stats(vfe32_ctrl)) {
			/* Error */
			rc = -EFAULT;
			goto proc_general_done;
		}
		old_val = msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase + VFE_STATS_CFG);
		module_val = msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase + VFE_MODULE_CFG);
		if (VFE_CMD_STATS_BG_START == cmd->id) {
			module_val |= AE_BG_ENABLE_MASK;
			old_val |= STATS_BG_ENABLE_MASK;
			rc = vfe_stats_aec_bg_buf_init(vfe32_ctrl);
			if (rc < 0) {
				pr_err("%s: cannot config ping/pong address of CS",
					__func__);
				goto proc_general_done;
			}
		} else if (VFE_CMD_STATS_BF_START == cmd->id) {
			module_val |= AF_BF_ENABLE_MASK;
			old_val |= STATS_BF_ENABLE_MASK;
			rc = vfe_stats_af_bf_buf_init(vfe32_ctrl);
			if (rc < 0) {
				pr_err("%s: cannot config ping/pong address of CS",
					__func__);
				goto proc_general_done;
			}
		} else {
			module_val |= SKIN_BHIST_ENABLE_MASK;
			old_val |= STATS_BHIST_ENABLE_MASK;
			rc = vfe_stats_bhist_buf_init(vfe32_ctrl);
			if (rc < 0) {
				pr_err("%s: cannot config ping/pong address of CS",
					__func__);
				goto proc_general_done;
			}
		}
		msm_camera_io_w(old_val, vfe32_ctrl->share_ctrl->vfebase +
			VFE_STATS_CFG);
		msm_camera_io_w(module_val,
			vfe32_ctrl->share_ctrl->vfebase + VFE_MODULE_CFG);
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp,
				(void __user *)(cmd->value),
				cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			vfe32_cmd[cmd->id].offset,
			cmdp, (vfe32_cmd[cmd->id].length));
		}
		break;
	case VFE_CMD_MCE_UPDATE:
	case VFE_CMD_MCE_CFG:{
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		/* Incrementing with 4 so as to point to the 2nd Register as
		the 2nd register has the mce_enable bit */
		old_val = msm_camera_io_r(vfe32_ctrl->share_ctrl->vfebase +
			V32_CHROMA_SUP_OFF + 4);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp,
			(void __user *)(cmd->value),
			cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		cmdp_local = cmdp;
		new_val = *cmdp_local;
		old_val &= MCE_EN_MASK;
		new_val = new_val | old_val;
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			V32_CHROMA_SUP_OFF + 4, &new_val, 4);
		cmdp_local += 1;

		old_val = msm_camera_io_r(vfe32_ctrl->share_ctrl->vfebase +
			V32_CHROMA_SUP_OFF + 8);
		new_val = *cmdp_local;
		old_val &= MCE_Q_K_MASK;
		new_val = new_val | old_val;
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			V32_CHROMA_SUP_OFF + 8, &new_val, 4);
		cmdp_local += 1;
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			vfe32_cmd[cmd->id].offset,
			cmdp_local, (vfe32_cmd[cmd->id].length));
		}
		break;
	case VFE_CMD_CHROMA_SUP_UPDATE:
	case VFE_CMD_CHROMA_SUP_CFG:{
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp,
			(void __user *)(cmd->value),
			cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		cmdp_local = cmdp;
		msm_camera_io_memcpy(vfe32_ctrl->share_ctrl->vfebase +
			V32_CHROMA_SUP_OFF, cmdp_local, 4);

		cmdp_local += 1;
		new_val = *cmdp_local;
		/* Incrementing with 4 so as to point to the 2nd Register as
		 * the 2nd register has the mce_enable bit
		 */
		old_val = msm_camera_io_r(vfe32_ctrl->share_ctrl->vfebase +
			V32_CHROMA_SUP_OFF + 4);
		old_val &= ~MCE_EN_MASK;
		new_val = new_val | old_val;
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			V32_CHROMA_SUP_OFF + 4, &new_val, 4);
		cmdp_local += 1;

		old_val = msm_camera_io_r(vfe32_ctrl->share_ctrl->vfebase +
			V32_CHROMA_SUP_OFF + 8);
		new_val = *cmdp_local;
		old_val &= ~MCE_Q_K_MASK;
		new_val = new_val | old_val;
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			V32_CHROMA_SUP_OFF + 8, &new_val, 4);
		}
		break;
	case VFE_CMD_BLACK_LEVEL_CFG:
		rc = -EFAULT;
		goto proc_general_done;

	case VFE_CMD_MESH_ROLL_OFF_CFG: {
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp,
			(void __user *)(cmd->value) , cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		cmdp_local = cmdp;
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			vfe32_cmd[cmd->id].offset,
			cmdp_local, 16);
		cmdp_local += 4;
		vfe32_program_dmi_cfg(ROLLOFF_RAM0_BANK0, vfe32_ctrl);
		/* for loop for extrcting init table. */
		for (i = 0; i < (V32_MESH_ROLL_OFF_INIT_TABLE_SIZE * 2); i++) {
			msm_camera_io_w(*cmdp_local ,
			vfe32_ctrl->share_ctrl->vfebase + VFE_DMI_DATA_LO);
			cmdp_local++;
		}
		CDBG("done writing init table\n");
		/* by default, always starts with offset 0. */
		msm_camera_io_w(V32_MESH_ROLL_OFF_DELTA_TABLE_OFFSET,
		vfe32_ctrl->share_ctrl->vfebase + VFE_DMI_ADDR);
		/* for loop for extracting delta table. */
		for (i = 0; i < (V32_MESH_ROLL_OFF_DELTA_TABLE_SIZE * 2); i++) {
			msm_camera_io_w(*cmdp_local,
			vfe32_ctrl->share_ctrl->vfebase + VFE_DMI_DATA_LO);
			cmdp_local++;
		}
		vfe32_program_dmi_cfg(NO_MEM_SELECTED, vfe32_ctrl);
		}
		break;

	case VFE_CMD_GET_MESH_ROLLOFF_TABLE:
		temp1 = sizeof(uint32_t) * ((V32_MESH_ROLL_OFF_INIT_TABLE_SIZE *
			2) + (V32_MESH_ROLL_OFF_DELTA_TABLE_SIZE * 2));
		if (cmd->length != temp1) {
			rc = -EINVAL;
			goto proc_general_done;
		}
		cmdp = kzalloc(temp1, GFP_KERNEL);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		cmdp_local = cmdp;
		vfe32_program_dmi_cfg(ROLLOFF_RAM0_BANK0, vfe32_ctrl);
		CDBG("%s: Mesh Rolloff init Table\n", __func__);
		for (i = 0; i < (V32_MESH_ROLL_OFF_INIT_TABLE_SIZE * 2); i++) {
			*cmdp_local =
				msm_camera_io_r(
					vfe32_ctrl->share_ctrl->vfebase +
					VFE_DMI_DATA_LO);
			CDBG("%s: %08x\n", __func__, *cmdp_local);
			cmdp_local++;
		}
		msm_camera_io_w(V32_MESH_ROLL_OFF_DELTA_TABLE_OFFSET,
			vfe32_ctrl->share_ctrl->vfebase + VFE_DMI_ADDR);
		CDBG("%s: Mesh Rolloff Delta Table\n", __func__);
		for (i = 0; i < (V32_MESH_ROLL_OFF_DELTA_TABLE_SIZE * 2); i++) {
			*cmdp_local =
				msm_camera_io_r(
					vfe32_ctrl->share_ctrl->vfebase +
					VFE_DMI_DATA_LO);
			CDBG("%s: %08x\n", __func__, *cmdp_local);
			cmdp_local++;
		}
		CDBG("done reading delta table\n");
		vfe32_program_dmi_cfg(NO_MEM_SELECTED, vfe32_ctrl);
		if (copy_to_user((void __user *)(cmd->value), cmdp,
			temp1)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		break;
	case VFE_CMD_LA_CFG:
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp,
			(void __user *)(cmd->value),
			cmd->length)) {

			rc = -EFAULT;
			goto proc_general_done;
		}
		cmdp_local = cmdp;
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			vfe32_cmd[cmd->id].offset,
			cmdp_local, (vfe32_cmd[cmd->id].length));

		cmdp_local += 1;
		vfe32_write_la_cfg(LUMA_ADAPT_LUT_RAM_BANK0,
						   cmdp_local, vfe32_ctrl);
		break;

	case VFE_CMD_LA_UPDATE: {
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp,
			(void __user *)(cmd->value),
			cmd->length)) {

			rc = -EFAULT;
			goto proc_general_done;
		}

		cmdp_local = cmdp + 1;
		old_val = msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase + V32_LA_OFF);
		if (old_val != 0x0)
			vfe32_write_la_cfg(LUMA_ADAPT_LUT_RAM_BANK0,
				cmdp_local, vfe32_ctrl);
		else
			vfe32_write_la_cfg(LUMA_ADAPT_LUT_RAM_BANK1,
				cmdp_local, vfe32_ctrl);
		}
		vfe32_ctrl->update_la = true;
		break;

	case VFE_CMD_GET_LA_TABLE:
		temp1 = sizeof(uint32_t) * VFE32_LA_TABLE_LENGTH / 2;
		if (cmd->length != temp1) {
			rc = -EINVAL;
			goto proc_general_done;
		}
		cmdp = kzalloc(temp1, GFP_KERNEL);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		cmdp_local = cmdp;
		if (msm_camera_io_r(vfe32_ctrl->
				share_ctrl->vfebase + V32_LA_OFF))
			vfe32_program_dmi_cfg(LUMA_ADAPT_LUT_RAM_BANK1,
						vfe32_ctrl);
		else
			vfe32_program_dmi_cfg(LUMA_ADAPT_LUT_RAM_BANK0,
						vfe32_ctrl);
		for (i = 0 ; i < (VFE32_LA_TABLE_LENGTH / 2) ; i++) {
			*cmdp_local =
				msm_camera_io_r(
					vfe32_ctrl->share_ctrl->vfebase +
					VFE_DMI_DATA_LO);
			*cmdp_local |= (msm_camera_io_r(
				vfe32_ctrl->share_ctrl->vfebase +
				VFE_DMI_DATA_LO)) << 16;
			cmdp_local++;
		}
		vfe32_program_dmi_cfg(NO_MEM_SELECTED, vfe32_ctrl);
		if (copy_to_user((void __user *)(cmd->value), cmdp,
			temp1)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		break;
	case VFE_CMD_SK_ENHAN_CFG:
	case VFE_CMD_SK_ENHAN_UPDATE:{
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp,
			(void __user *)(cmd->value),
			cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase + V32_SCE_OFF,
			cmdp, V32_SCE_LEN);
		}
		break;

	case VFE_CMD_LIVESHOT:
		/* Configure primary channel */
		vfe32_start_liveshot(pmctl, vfe32_ctrl);
		break;

	case VFE_CMD_LINEARIZATION_CFG:
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp, (void __user *)(cmd->value),
			cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		cmdp_local = cmdp;
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			V32_LINEARIZATION_OFF1,
			cmdp_local, V32_LINEARIZATION_LEN1);
		cmdp_local += 4;
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			V32_LINEARIZATION_OFF2,
			cmdp_local, V32_LINEARIZATION_LEN2);

		cmdp_local = cmdp + 17;
		vfe32_write_linear_cfg(BLACK_LUT_RAM_BANK0,
					cmdp_local, vfe32_ctrl);
		break;

	case VFE_CMD_LINEARIZATION_UPDATE:
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp, (void __user *)(cmd->value),
			cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		cmdp_local = cmdp;
		cmdp_local++;
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			V32_LINEARIZATION_OFF1 + 4,
			cmdp_local, (V32_LINEARIZATION_LEN1 - 4));
		cmdp_local += 3;
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			V32_LINEARIZATION_OFF2,
			cmdp_local, V32_LINEARIZATION_LEN2);
		cmdp_local = cmdp + 17;
		/*extracting the bank select*/
		old_val = msm_camera_io_r(
				vfe32_ctrl->share_ctrl->vfebase +
				V32_LINEARIZATION_OFF1);

		if (old_val != 0x0)
			vfe32_write_linear_cfg(BLACK_LUT_RAM_BANK0,
						cmdp_local, vfe32_ctrl);
		else
			vfe32_write_linear_cfg(BLACK_LUT_RAM_BANK1,
						cmdp_local, vfe32_ctrl);
		vfe32_ctrl->update_linear = true;
		break;

	case VFE_CMD_GET_LINEARIZATON_TABLE:
		temp1 = sizeof(uint32_t) * VFE32_LINEARIZATON_TABLE_LENGTH;
		if (cmd->length != temp1) {
			rc = -EINVAL;
			goto proc_general_done;
		}
		cmdp = kzalloc(temp1, GFP_KERNEL);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		cmdp_local = cmdp;
		if (msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase +
			V32_LINEARIZATION_OFF1))
			vfe32_program_dmi_cfg(BLACK_LUT_RAM_BANK1, vfe32_ctrl);
		else
			vfe32_program_dmi_cfg(BLACK_LUT_RAM_BANK0, vfe32_ctrl);
		CDBG("%s: Linearization Table\n", __func__);
		for (i = 0 ; i < VFE32_LINEARIZATON_TABLE_LENGTH ; i++) {
			*cmdp_local = msm_camera_io_r(
				vfe32_ctrl->share_ctrl->vfebase +
				VFE_DMI_DATA_LO);
			CDBG("%s: %08x\n", __func__, *cmdp_local);
			cmdp_local++;
		}
		vfe32_program_dmi_cfg(NO_MEM_SELECTED, vfe32_ctrl);
		if (copy_to_user((void __user *)(cmd->value), cmdp,
			temp1)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		break;
	case VFE_CMD_DEMOSAICV3:
		if (cmd->length !=
			V32_DEMOSAICV3_0_LEN+V32_DEMOSAICV3_1_LEN) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp,
			(void __user *)(cmd->value),
			cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		cmdp_local = cmdp;
		new_val = *cmdp_local;

		old_val = msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase + V32_DEMOSAICV3_0_OFF);
		old_val &= DEMOSAIC_MASK;
		new_val = new_val | old_val;
		*cmdp_local = new_val;

		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase + V32_DEMOSAICV3_0_OFF,
			cmdp_local, V32_DEMOSAICV3_0_LEN);
		cmdp_local += 1;
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase + V32_DEMOSAICV3_1_OFF,
			cmdp_local, V32_DEMOSAICV3_1_LEN);
		break;

	case VFE_CMD_DEMOSAICV3_UPDATE:
		if (cmd->length !=
			V32_DEMOSAICV3_0_LEN * V32_DEMOSAICV3_UP_REG_CNT) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp,
			(void __user *)(cmd->value),
			cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		cmdp_local = cmdp;
		new_val = *cmdp_local;

		old_val = msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase + V32_DEMOSAICV3_0_OFF);
		old_val &= DEMOSAIC_MASK;
		new_val = new_val | old_val;
		*cmdp_local = new_val;

		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase + V32_DEMOSAICV3_0_OFF,
			cmdp_local, V32_DEMOSAICV3_0_LEN);
		/* As the address space is not contiguous increment by 2
		 * before copying to next address space */
		cmdp_local += 1;
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase + V32_DEMOSAICV3_1_OFF,
			cmdp_local, 2 * V32_DEMOSAICV3_0_LEN);
		/* As the address space is not contiguous increment by 2
		 * before copying to next address space */
		cmdp_local += 2;
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase + V32_DEMOSAICV3_2_OFF,
			cmdp_local, 2 * V32_DEMOSAICV3_0_LEN);
		break;

	case VFE_CMD_DEMOSAICV3_ABCC_CFG:
		rc = -EFAULT;
		break;

	case VFE_CMD_DEMOSAICV3_ABF_UPDATE:/* 116 ABF update  */
	case VFE_CMD_DEMOSAICV3_ABF_CFG: { /* 108 ABF config  */
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp,
			(void __user *)(cmd->value),
			cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		cmdp_local = cmdp;
		new_val = *cmdp_local;

		old_val = msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase + V32_DEMOSAICV3_0_OFF);
		old_val &= ABF_MASK;
		new_val = new_val | old_val;
		*cmdp_local = new_val;

		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase + V32_DEMOSAICV3_0_OFF,
			cmdp_local, 4);

		cmdp_local += 1;
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			vfe32_cmd[cmd->id].offset,
			cmdp_local, (vfe32_cmd[cmd->id].length));
		}
		break;

	case VFE_CMD_DEMOSAICV3_DBCC_CFG:
	case VFE_CMD_DEMOSAICV3_DBCC_UPDATE:
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp,
			(void __user *)(cmd->value),
			cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		cmdp_local = cmdp;
		new_val = *cmdp_local;

		old_val = msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase + V32_DEMOSAICV3_0_OFF);
		old_val &= DBCC_MASK;

		new_val = new_val | old_val;
		*cmdp_local = new_val;
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase + V32_DEMOSAICV3_0_OFF,
			cmdp_local, 4);
		cmdp_local += 1;
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			vfe32_cmd[cmd->id].offset,
			cmdp_local, (vfe32_cmd[cmd->id].length));
		break;

	case VFE_CMD_DEMOSAICV3_DBPC_CFG:
	case VFE_CMD_DEMOSAICV3_DBPC_UPDATE:
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp,
			(void __user *)(cmd->value),
			cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		cmdp_local = cmdp;
		new_val = *cmdp_local;

		old_val = msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase + V32_DEMOSAICV3_0_OFF);
		old_val &= DBPC_MASK;

		new_val = new_val | old_val;
		*cmdp_local = new_val;
		msm_camera_io_memcpy(vfe32_ctrl->share_ctrl->vfebase +
			V32_DEMOSAICV3_0_OFF,
			cmdp_local, V32_DEMOSAICV3_LEN);
		cmdp_local += 1;
		msm_camera_io_memcpy(vfe32_ctrl->share_ctrl->vfebase +
			V32_DEMOSAICV3_DBPC_CFG_OFF,
			cmdp_local, V32_DEMOSAICV3_DBPC_LEN);
		cmdp_local += 1;
		msm_camera_io_memcpy(vfe32_ctrl->share_ctrl->vfebase +
			V32_DEMOSAICV3_DBPC_CFG_OFF0,
			cmdp_local, V32_DEMOSAICV3_DBPC_LEN);
		cmdp_local += 1;
		msm_camera_io_memcpy(vfe32_ctrl->share_ctrl->vfebase +
			V32_DEMOSAICV3_DBPC_CFG_OFF1,
			cmdp_local, V32_DEMOSAICV3_DBPC_LEN);
		cmdp_local += 1;
		msm_camera_io_memcpy(vfe32_ctrl->share_ctrl->vfebase +
			V32_DEMOSAICV3_DBPC_CFG_OFF2,
			cmdp_local, V32_DEMOSAICV3_DBPC_LEN);
		break;

	case VFE_CMD_RGB_G_CFG: {
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp,
			(void __user *)(cmd->value),
			cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase + V32_RGB_G_OFF,
			cmdp, 4);
		cmdp += 1;

		vfe32_write_gamma_cfg(RGBLUT_RAM_CH0_BANK0, cmdp, vfe32_ctrl);
		vfe32_write_gamma_cfg(RGBLUT_RAM_CH1_BANK0, cmdp, vfe32_ctrl);
		vfe32_write_gamma_cfg(RGBLUT_RAM_CH2_BANK0, cmdp, vfe32_ctrl);
		}
	    cmdp -= 1;
		break;

	case VFE_CMD_RGB_ALL_CFG: {
		cmdp = kmalloc((cmd->length), GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp,
			(void __user *)(cmd->value),
			cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase + V32_RGB_G_OFF,
			cmdp, 4);
		cmdp += 1;

		vfe32_write_gamma_cfg(RGBLUT_RAM_CH0_BANK0,
			cmdp + VFE32_GAMMA_CH0_G_POS, vfe32_ctrl);
		vfe32_write_gamma_cfg(RGBLUT_RAM_CH1_BANK0,
			cmdp + VFE32_GAMMA_CH1_B_POS, vfe32_ctrl);
		vfe32_write_gamma_cfg(RGBLUT_RAM_CH2_BANK0,
			cmdp + VFE32_GAMMA_CH2_R_POS, vfe32_ctrl);
		}
	    cmdp -= 1;
		break;

	case VFE_CMD_RGB_ALL_UPDATE: {
		cmdp = kmalloc((cmd->length), GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp, (void __user *)(cmd->value),
			cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		old_val = msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase + V32_RGB_G_OFF);
			cmdp += 1;
		if (old_val != 0x0) {
			vfe32_write_gamma_cfg(RGBLUT_RAM_CH0_BANK0,
				cmdp + VFE32_GAMMA_CH0_G_POS, vfe32_ctrl);
			vfe32_write_gamma_cfg(RGBLUT_RAM_CH1_BANK0,
				cmdp + VFE32_GAMMA_CH1_B_POS, vfe32_ctrl);
			vfe32_write_gamma_cfg(RGBLUT_RAM_CH2_BANK0,
				cmdp + VFE32_GAMMA_CH2_R_POS, vfe32_ctrl);
		} else {
			vfe32_write_gamma_cfg(RGBLUT_RAM_CH0_BANK1,
				cmdp + VFE32_GAMMA_CH0_G_POS, vfe32_ctrl);
			vfe32_write_gamma_cfg(RGBLUT_RAM_CH1_BANK1,
				cmdp + VFE32_GAMMA_CH1_B_POS, vfe32_ctrl);
			vfe32_write_gamma_cfg(RGBLUT_RAM_CH2_BANK1,
				cmdp + VFE32_GAMMA_CH2_R_POS, vfe32_ctrl);
		}
		}
		vfe32_ctrl->update_gamma = TRUE;
		cmdp -= 1;
		break;

	case VFE_CMD_RGB_G_UPDATE: {
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp, (void __user *)(cmd->value),
			cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}

		old_val = msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase + V32_RGB_G_OFF);
		cmdp += 1;
		if (old_val != 0x0) {
			vfe32_write_gamma_cfg(
				RGBLUT_RAM_CH0_BANK0, cmdp, vfe32_ctrl);
			vfe32_write_gamma_cfg(
				RGBLUT_RAM_CH1_BANK0, cmdp, vfe32_ctrl);
			vfe32_write_gamma_cfg(
				RGBLUT_RAM_CH2_BANK0, cmdp, vfe32_ctrl);
		} else {
			vfe32_write_gamma_cfg(
				RGBLUT_RAM_CH0_BANK1, cmdp, vfe32_ctrl);
			vfe32_write_gamma_cfg(
				RGBLUT_RAM_CH1_BANK1, cmdp, vfe32_ctrl);
			vfe32_write_gamma_cfg(
				RGBLUT_RAM_CH2_BANK1, cmdp, vfe32_ctrl);
		}
		}
		vfe32_ctrl->update_gamma = TRUE;
		cmdp -= 1;
		break;

	case VFE_CMD_GET_RGB_G_TABLE:
		temp1 = sizeof(uint32_t) * VFE32_GAMMA_NUM_ENTRIES * 3;
		if (cmd->length != temp1) {
			rc = -EINVAL;
			goto proc_general_done;
		}
		cmdp = kzalloc(temp1, GFP_KERNEL);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		cmdp_local = cmdp;

		old_val = msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase + V32_RGB_G_OFF);
		temp2 = old_val ? RGBLUT_RAM_CH0_BANK1 :
			RGBLUT_RAM_CH0_BANK0;
		for (i = 0; i < 3; i++) {
			vfe32_read_gamma_cfg(temp2,
				cmdp_local + (VFE32_GAMMA_NUM_ENTRIES * i),
				vfe32_ctrl);
			temp2 += 2;
		}
		if (copy_to_user((void __user *)(cmd->value), cmdp,
			temp1)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		break;

	case VFE_CMD_STATS_AWB_STOP: {
		if (vfe32_use_bayer_stats(vfe32_ctrl)) {
			/* Error */
			rc = -EFAULT;
			goto proc_general_done;
		}
		old_val = msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase + VFE_MODULE_CFG);
		old_val &= ~AWB_ENABLE_MASK;
		msm_camera_io_w(old_val,
			vfe32_ctrl->share_ctrl->vfebase + VFE_MODULE_CFG);
		}
		break;
	case VFE_CMD_STATS_AE_STOP: {
		if (vfe32_use_bayer_stats(vfe32_ctrl)) {
			/* Error */
			rc = -EFAULT;
			goto proc_general_done;
		}
		old_val = msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase + VFE_MODULE_CFG);
		old_val &= ~AE_BG_ENABLE_MASK;
		msm_camera_io_w(old_val,
			vfe32_ctrl->share_ctrl->vfebase + VFE_MODULE_CFG);
		}
		break;
	case VFE_CMD_STATS_AF_STOP: {
		if (vfe32_use_bayer_stats(vfe32_ctrl)) {
			/* Error */
			rc = -EFAULT;
			goto proc_general_done;
		}
		old_val = msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase + VFE_MODULE_CFG);
		old_val &= ~AF_BF_ENABLE_MASK;
		msm_camera_io_w(old_val,
			vfe32_ctrl->share_ctrl->vfebase + VFE_MODULE_CFG);
		}
		break;

	case VFE_CMD_STATS_IHIST_STOP: {
		old_val = msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase + VFE_MODULE_CFG);
		old_val &= ~IHIST_ENABLE_MASK;
		msm_camera_io_w(old_val,
			vfe32_ctrl->share_ctrl->vfebase + VFE_MODULE_CFG);
		}
		break;

	case VFE_CMD_STATS_RS_STOP: {
		old_val = msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase + VFE_MODULE_CFG);
		old_val &= ~RS_ENABLE_MASK;
		msm_camera_io_w(old_val,
			vfe32_ctrl->share_ctrl->vfebase + VFE_MODULE_CFG);
		}
		break;

	case VFE_CMD_STATS_CS_STOP: {
		old_val = msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase + VFE_MODULE_CFG);
		old_val &= ~CS_ENABLE_MASK;
		msm_camera_io_w(old_val,
			vfe32_ctrl->share_ctrl->vfebase + VFE_MODULE_CFG);
		}
		break;

	case VFE_CMD_STATS_BG_STOP:
	case VFE_CMD_STATS_BF_STOP:
	case VFE_CMD_STATS_BHIST_STOP: {
		if (!vfe32_use_bayer_stats(vfe32_ctrl)) {
			/* Error */
			rc = -EFAULT;
			goto proc_general_done;
		}
		old_val = msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase + VFE_STATS_CFG);
		if (VFE_CMD_STATS_BG_STOP == cmd->id)
			old_val &= ~STATS_BG_ENABLE_MASK;
		else if (VFE_CMD_STATS_BF_STOP == cmd->id)
			old_val &= ~STATS_BF_ENABLE_MASK;
		else
			old_val &= ~STATS_BHIST_ENABLE_MASK;
		msm_camera_io_w(old_val,
			vfe32_ctrl->share_ctrl->vfebase + VFE_STATS_CFG);
		if (VFE_CMD_STATS_BF_STOP == cmd->id) {
			rc = vfe32_stats_flush_enqueue(vfe32_ctrl,
					MSM_STATS_TYPE_BF);
			if (rc < 0) {
				pr_err("%s: dq stats buf err = %d",
					   __func__, rc);
				return -EINVAL;
			}
		}
		}
		break;

	case VFE_CMD_STOP:
		pr_info("vfe32_proc_general: cmdID = %s\n",
			vfe32_general_cmd[cmd->id]);
		if (copy_from_user(&vfe_params,
				(void __user *)(cmd->value),
				sizeof(struct msm_camera_vfe_params_t))) {
				rc = -EFAULT;
				goto proc_general_done;
		}

		vfe32_stop(vfe32_ctrl);
		break;

	case VFE_CMD_SYNC_TIMER_SETTING:
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp, (void __user *)(cmd->value),
			cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		vfe32_sync_timer_start(cmdp, vfe32_ctrl);
		break;

	case VFE_CMD_MODULE_CFG: {
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp,
			(void __user *)(cmd->value),
			cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		*cmdp &= ~STATS_ENABLE_MASK;
		old_val = msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase + VFE_MODULE_CFG);
		old_val &= STATS_ENABLE_MASK;
		*cmdp |= old_val;

		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			vfe32_cmd[cmd->id].offset,
			cmdp, (vfe32_cmd[cmd->id].length));
		}
		break;

	case VFE_CMD_ZSL:
		if (copy_from_user(&vfe_params,
				(void __user *)(cmd->value),
				sizeof(struct msm_camera_vfe_params_t))) {
				rc = -EFAULT;
				goto proc_general_done;
		}

		vfe32_ctrl->share_ctrl->current_mode =
			vfe_params.operation_mode;

		rc = vfe32_zsl(pmctl, vfe32_ctrl);
		break;

	case VFE_CMD_ASF_CFG:
	case VFE_CMD_ASF_UPDATE:
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp, (void __user *)(cmd->value),
			cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			vfe32_cmd[cmd->id].offset,
			cmdp, (vfe32_cmd[cmd->id].length));
		cmdp_local = cmdp + V32_ASF_LEN/4;
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			V32_ASF_SPECIAL_EFX_CFG_OFF,
			cmdp_local, V32_ASF_SPECIAL_EFX_CFG_LEN);
		break;

	case VFE_CMD_PCA_ROLL_OFF_CFG:
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp,
			(void __user *)(cmd->value) , cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}

		cmdp_local = cmdp;

		temp1 = *cmdp_local;
		cmdp_local++;

		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			V33_PCA_ROLL_OFF_CFG_OFF1,
			cmdp_local, V33_PCA_ROLL_OFF_CFG_LEN1);
		cmdp_local += 4;
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			V33_PCA_ROLL_OFF_CFG_OFF2,
			cmdp_local, V33_PCA_ROLL_OFF_CFG_LEN2);

		cmdp_local += 3;
		CDBG("%s: start writing RollOff Ram0 table\n", __func__);
		vfe32_program_dmi_cfg(ROLLOFF_RAM0_BANK0, vfe32_ctrl);
		msm_camera_io_w(temp1,
			vfe32_ctrl->share_ctrl->vfebase + VFE_DMI_ADDR);
		for (i = 0 ; i < V33_PCA_ROLL_OFF_TABLE_SIZE ; i++) {
			msm_camera_io_w(*(cmdp_local + 1),
				vfe32_ctrl->share_ctrl->vfebase +
				VFE33_DMI_DATA_HI);
			msm_camera_io_w(*cmdp_local,
				vfe32_ctrl->share_ctrl->vfebase +
				VFE33_DMI_DATA_LO);
			cmdp_local += 2;
		}
		CDBG("%s: end writing RollOff Ram0 table\n", __func__);

		CDBG("%s: start writing RollOff Ram1 table\n", __func__);
		vfe32_program_dmi_cfg(ROLLOFF_RAM1_BANK0, vfe32_ctrl);
		msm_camera_io_w(temp1,
			vfe32_ctrl->share_ctrl->vfebase + VFE_DMI_ADDR);
		for (i = 0 ; i < V33_PCA_ROLL_OFF_TABLE_SIZE ; i++) {
			msm_camera_io_w(*cmdp_local,
				vfe32_ctrl->share_ctrl->vfebase +
				VFE33_DMI_DATA_LO);
			cmdp_local += 2;
		}
		CDBG("%s: end writing RollOff Ram1 table\n", __func__);

		vfe32_program_dmi_cfg(NO_MEM_SELECTED, vfe32_ctrl);
		break;

	case VFE_CMD_PCA_ROLL_OFF_UPDATE:
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp,
			(void __user *)(cmd->value), cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}

		cmdp_local = cmdp;

		temp1 = *cmdp_local;
		cmdp_local += 8;

		temp2 = msm_camera_io_r(vfe32_ctrl->share_ctrl->vfebase +
			V33_PCA_ROLL_OFF_CFG_OFF1)
			& V33_PCA_ROLL_OFF_LUT_BANK_SEL_MASK;

		CDBG("%s: start writing RollOff Ram0 table\n", __func__);
		if (temp2)
			vfe32_program_dmi_cfg(ROLLOFF_RAM0_BANK0, vfe32_ctrl);
		else
			vfe32_program_dmi_cfg(ROLLOFF_RAM0_BANK1, vfe32_ctrl);

		msm_camera_io_w(temp1,
			vfe32_ctrl->share_ctrl->vfebase + VFE_DMI_ADDR);
		for (i = 0 ; i < V33_PCA_ROLL_OFF_TABLE_SIZE ; i++) {
			msm_camera_io_w(*(cmdp_local + 1),
				vfe32_ctrl->share_ctrl->vfebase +
				VFE33_DMI_DATA_HI);
			msm_camera_io_w(*cmdp_local,
				vfe32_ctrl->share_ctrl->vfebase +
				VFE33_DMI_DATA_LO);
			cmdp_local += 2;
		}
		CDBG("%s: end writing RollOff Ram0 table\n", __func__);

		CDBG("%s: start writing RollOff Ram1 table\n", __func__);
		if (temp2)
			vfe32_program_dmi_cfg(ROLLOFF_RAM1_BANK0, vfe32_ctrl);
		else
			vfe32_program_dmi_cfg(ROLLOFF_RAM1_BANK1, vfe32_ctrl);

		msm_camera_io_w(temp1,
			vfe32_ctrl->share_ctrl->vfebase + VFE_DMI_ADDR);
		for (i = 0 ; i < V33_PCA_ROLL_OFF_TABLE_SIZE ; i++) {
			msm_camera_io_w(*cmdp_local,
				vfe32_ctrl->share_ctrl->vfebase +
				VFE33_DMI_DATA_LO);
			cmdp_local += 2;
		}
		CDBG("%s: end writing RollOff Ram1 table\n", __func__);

		vfe32_program_dmi_cfg(NO_MEM_SELECTED, vfe32_ctrl);
		vfe32_ctrl->update_rolloff = true;
		break;
	case VFE_CMD_GET_PCA_ROLLOFF_TABLE:
		temp1 = sizeof(uint64_t) * V33_PCA_ROLL_OFF_TABLE_SIZE * 2;
		if (cmd->length != temp1) {
			rc = -EINVAL;
			goto proc_general_done;
		}
		cmdp = kzalloc(temp1, GFP_KERNEL);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		cmdp_local = cmdp;
		old_val = msm_camera_io_r(vfe32_ctrl->share_ctrl->vfebase +
			V33_PCA_ROLL_OFF_CFG_OFF1) &
			V33_PCA_ROLL_OFF_LUT_BANK_SEL_MASK;

		if (old_val)
			vfe32_program_dmi_cfg(ROLLOFF_RAM0_BANK1, vfe32_ctrl);
		else
			vfe32_program_dmi_cfg(ROLLOFF_RAM0_BANK0, vfe32_ctrl);

		CDBG("%s: PCA Rolloff Ram0\n", __func__);
		for (i = 0 ; i < V33_PCA_ROLL_OFF_TABLE_SIZE * 2; i++) {
			temp2 = (i == (V33_PCA_ROLL_OFF_TABLE_SIZE));
			if (old_val && temp2)
				vfe32_program_dmi_cfg(ROLLOFF_RAM1_BANK1,
				vfe32_ctrl);
			else if (!old_val && temp2)
				vfe32_program_dmi_cfg(ROLLOFF_RAM1_BANK0,
				vfe32_ctrl);

			*cmdp_local = msm_camera_io_r(
				vfe32_ctrl->share_ctrl->vfebase +
				VFE33_DMI_DATA_LO);
			*(cmdp_local + 1) =
				msm_camera_io_r(
				vfe32_ctrl->share_ctrl->vfebase +
				VFE33_DMI_DATA_HI);
			CDBG("%s: %08x%08x\n", __func__,
				*(cmdp_local + 1), *cmdp_local);
			cmdp_local += 2;
		}
		vfe32_program_dmi_cfg(NO_MEM_SELECTED, vfe32_ctrl);
		if (copy_to_user((void __user *)(cmd->value), cmdp,
			temp1)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		break;
	case VFE_CMD_GET_HW_VERSION:
		if (cmd->length != V32_GET_HW_VERSION_LEN) {
			rc = -EINVAL;
			goto proc_general_done;
		}
		cmdp = kmalloc(V32_GET_HW_VERSION_LEN, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		*cmdp = msm_camera_io_r(
			vfe32_ctrl->share_ctrl->vfebase+V32_GET_HW_VERSION_OFF);
		if (copy_to_user((void __user *)(cmd->value), cmdp,
			V32_GET_HW_VERSION_LEN)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		break;
	case VFE_CMD_GET_REG_DUMP:
		temp1 = sizeof(uint32_t) *
			vfe32_ctrl->share_ctrl->register_total;
		if (cmd->length != temp1) {
			rc = -EINVAL;
			goto proc_general_done;
		}
		cmdp = kmalloc(temp1, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		msm_camera_io_dump(vfe32_ctrl->share_ctrl->vfebase,
			vfe32_ctrl->share_ctrl->register_total*4);
		CDBG("%s: %p %p %d\n", __func__, (void *)cmdp,
			vfe32_ctrl->share_ctrl->vfebase, temp1);
		memcpy_fromio((void *)cmdp,
			vfe32_ctrl->share_ctrl->vfebase, temp1);
		if (copy_to_user((void __user *)(cmd->value), cmdp, temp1)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		break;
	case VFE_CMD_FRAME_SKIP_CFG:
		if (cmd->length != vfe32_cmd[cmd->id].length)
			return -EINVAL;

		cmdp = kmalloc(vfe32_cmd[cmd->id].length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}

		if (copy_from_user((cmdp), (void __user *)cmd->value,
				cmd->length)) {
			rc = -EFAULT;
			pr_err("%s copy from user failed for cmd %d",
				__func__, cmd->id);
			break;
		}

		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			vfe32_cmd[cmd->id].offset,
			cmdp, (vfe32_cmd[cmd->id].length));
		vfe32_ctrl->frame_skip_cnt = ((uint32_t)
			*cmdp & VFE_FRAME_SKIP_PERIOD_MASK) + 1;
		vfe32_ctrl->frame_skip_pattern = (uint32_t)(*(cmdp + 2));
		break;
	case VFE_CMD_STOP_LIVESHOT:
		CDBG("%s Stopping liveshot ", __func__);
		vfe32_stop_liveshot(pmctl, vfe32_ctrl);
		break;
	case VFE_CMD_SELECT_RDI: {
		uint32_t rdi_sel;
		if (copy_from_user(&rdi_sel,
				(void __user *)(cmd->value),
				sizeof(uint32_t))) {
				rc = -EFAULT;
				goto proc_general_done;
		}
		pr_info("%s: rdi interface: %d\n", __func__, rdi_sel);
		vfe32_ctrl->share_ctrl->rdi_comp = rdi_sel;
	}
		break;
	case VFE_CMD_SET_STATS_VER:
		cmdp = kmalloc(cmd->length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}
		if (copy_from_user(cmdp, (void __user *)(cmd->value),
			cmd->length)) {
			rc = -EFAULT;
			goto proc_general_done;
		}
		vfe32_ctrl->ver_num.main = *(uint32_t *)cmdp;
		CDBG("%s:Set Stats version %d", __func__,
			vfe32_ctrl->ver_num.main);

		break;
	default:
		if (cmd->length != vfe32_cmd[cmd->id].length)
			return -EINVAL;

		cmdp = kmalloc(vfe32_cmd[cmd->id].length, GFP_ATOMIC);
		if (!cmdp) {
			rc = -ENOMEM;
			goto proc_general_done;
		}

		if (copy_from_user((cmdp), (void __user *)cmd->value,
				cmd->length)) {
			rc = -EFAULT;
			pr_err("%s copy from user failed for cmd %d",
				__func__, cmd->id);
			goto proc_general_done;
		}
		msm_camera_io_memcpy(
			vfe32_ctrl->share_ctrl->vfebase +
			vfe32_cmd[cmd->id].offset,
			cmdp, (vfe32_cmd[cmd->id].length));
		break;

	}

proc_general_done:
	kfree(cmdp);

	return rc;
}

static inline void vfe32_read_irq_status(
	struct axi_ctrl_t *axi_ctrl, struct vfe32_irq_status *out)
{
	uint32_t *temp;
	memset(out, 0, sizeof(struct vfe32_irq_status));
	temp = (uint32_t *)(axi_ctrl->share_ctrl->vfebase + VFE_IRQ_STATUS_0);
	out->vfeIrqStatus0 = msm_camera_io_r(temp);

	temp = (uint32_t *)(axi_ctrl->share_ctrl->vfebase + VFE_IRQ_STATUS_1);
	out->vfeIrqStatus1 = msm_camera_io_r(temp);

	temp = (uint32_t *)(axi_ctrl->share_ctrl->vfebase + VFE_CAMIF_STATUS);
	out->camifStatus = msm_camera_io_r(temp);
	CDBG("camifStatus  = 0x%x\n", out->camifStatus);

	/* clear the pending interrupt of the same kind.*/
	msm_camera_io_w(out->vfeIrqStatus0,
		axi_ctrl->share_ctrl->vfebase + VFE_IRQ_CLEAR_0);
	msm_camera_io_w(out->vfeIrqStatus1,
		axi_ctrl->share_ctrl->vfebase + VFE_IRQ_CLEAR_1);

	/* Ensure the write order while writing
	to the command register using the barrier */
	msm_camera_io_w_mb(1, axi_ctrl->share_ctrl->vfebase + VFE_IRQ_CMD);

}

void axi_stop_pix(struct vfe_share_ctrl_t *share_ctrl,
	uint32_t vfe_mode, uint8_t cmd_type)
{
	uint32_t reg_update = 0x1;
	switch (cmd_type) {
	case AXI_CMD_RAW_CAPTURE:
		msm_camera_io_w(0, share_ctrl->vfebase
			+ vfe32_AXI_WM_CFG[share_ctrl->outpath.out0.ch0]);
		break;
	case AXI_CMD_PREVIEW: {
		switch (vfe_mode) {
		case VFE_OUTPUTS_PREVIEW:
		case VFE_OUTPUTS_PREVIEW_AND_VIDEO:
			if (share_ctrl->comp_output_mode &
				VFE32_OUTPUT_MODE_PRIMARY) {
				msm_camera_io_w(0, share_ctrl->vfebase
					+ vfe32_AXI_WM_CFG[share_ctrl->
					outpath.out0.ch0]);
				msm_camera_io_w(0, share_ctrl->vfebase
					+ vfe32_AXI_WM_CFG[share_ctrl->
					outpath.out0.ch1]);
			} else if (share_ctrl->comp_output_mode &
					VFE32_OUTPUT_MODE_PRIMARY_ALL_CHNLS) {
				msm_camera_io_w(0, share_ctrl->vfebase
					+ vfe32_AXI_WM_CFG[share_ctrl->
					outpath.out0.ch0]);
				msm_camera_io_w(0, share_ctrl->vfebase
					+ vfe32_AXI_WM_CFG[share_ctrl->
					outpath.out0.ch1]);
				msm_camera_io_w(0, share_ctrl->vfebase
					+ vfe32_AXI_WM_CFG[share_ctrl->
					outpath.out0.ch2]);
			}
			break;
		default:
			if (share_ctrl->comp_output_mode &
				VFE32_OUTPUT_MODE_SECONDARY) {
				msm_camera_io_w(0, share_ctrl->vfebase
					+ vfe32_AXI_WM_CFG[share_ctrl->
					outpath.out1.ch0]);
				msm_camera_io_w(0, share_ctrl->vfebase
					+ vfe32_AXI_WM_CFG[share_ctrl->
					outpath.out1.ch1]);
			} else if (share_ctrl->comp_output_mode &
				VFE32_OUTPUT_MODE_SECONDARY_ALL_CHNLS) {
				msm_camera_io_w(0, share_ctrl->vfebase
					+ vfe32_AXI_WM_CFG[share_ctrl->
					outpath.out1.ch0]);
				msm_camera_io_w(0, share_ctrl->vfebase
					+ vfe32_AXI_WM_CFG[share_ctrl->
					outpath.out1.ch1]);
				msm_camera_io_w(0, share_ctrl->vfebase
					+ vfe32_AXI_WM_CFG[share_ctrl->
					outpath.out1.ch2]);
			}
			break;
			}
		}
		break;
	default:
		if (share_ctrl->comp_output_mode &
			VFE32_OUTPUT_MODE_PRIMARY) {
			msm_camera_io_w(0, share_ctrl->vfebase +
				vfe32_AXI_WM_CFG[share_ctrl->
				outpath.out0.ch0]);
			msm_camera_io_w(0, share_ctrl->vfebase +
				vfe32_AXI_WM_CFG[share_ctrl->
				outpath.out0.ch1]);
		} else if (share_ctrl->comp_output_mode &
				VFE32_OUTPUT_MODE_PRIMARY_ALL_CHNLS) {
			msm_camera_io_w(0, share_ctrl->vfebase +
				vfe32_AXI_WM_CFG[share_ctrl->
				outpath.out0.ch0]);
			msm_camera_io_w(0, share_ctrl->vfebase +
				vfe32_AXI_WM_CFG[share_ctrl->
				outpath.out0.ch1]);
			msm_camera_io_w(0, share_ctrl->vfebase +
				vfe32_AXI_WM_CFG[share_ctrl->
				outpath.out0.ch2]);
		}

		if (share_ctrl->comp_output_mode &
			VFE32_OUTPUT_MODE_SECONDARY) {
			msm_camera_io_w(0, share_ctrl->vfebase +
				vfe32_AXI_WM_CFG[share_ctrl->
				outpath.out1.ch0]);
			msm_camera_io_w(0, share_ctrl->vfebase +
				vfe32_AXI_WM_CFG[share_ctrl->outpath.out1.ch1]);
		} else if (share_ctrl->comp_output_mode &
			VFE32_OUTPUT_MODE_SECONDARY_ALL_CHNLS) {
			msm_camera_io_w(0, share_ctrl->vfebase +
				vfe32_AXI_WM_CFG[share_ctrl->outpath.out1.ch0]);
			msm_camera_io_w(0, share_ctrl->vfebase +
				vfe32_AXI_WM_CFG[share_ctrl->outpath.out1.ch1]);
			msm_camera_io_w(0, share_ctrl->vfebase +
				vfe32_AXI_WM_CFG[share_ctrl->outpath.out1.ch2]);
		}
		break;
	}

	msm_camera_io_w_mb(reg_update,
		share_ctrl->vfebase + VFE_REG_UPDATE_CMD);
}

void axi_stop_rdi0(struct vfe_share_ctrl_t *share_ctrl)
{
	uint32_t reg_update = 0x2;
	msm_camera_io_w(0, share_ctrl->vfebase +
		vfe32_AXI_WM_CFG[share_ctrl->outpath.out2.ch0]);
	msm_camera_io_w_mb(reg_update,
		share_ctrl->vfebase + VFE_REG_UPDATE_CMD);
}

void axi_stop_rdi1(struct vfe_share_ctrl_t *share_ctrl)
{
	uint32_t reg_update = 0x4;
	msm_camera_io_w(0, share_ctrl->vfebase +
		vfe32_AXI_WM_CFG[share_ctrl->outpath.out3.ch0]);
	msm_camera_io_w_mb(reg_update,
		share_ctrl->vfebase + VFE_REG_UPDATE_CMD);
}

void axi_stop_rdi2(struct vfe_share_ctrl_t *share_ctrl)
{
	uint32_t reg_update = 0x8; /* bit 3 */
	msm_camera_io_w(0, share_ctrl->vfebase +
		vfe32_AXI_WM_CFG[share_ctrl->outpath.out4.ch0]);
	msm_camera_io_w_mb(reg_update,
		share_ctrl->vfebase + VFE_REG_UPDATE_CMD);
}

void axi_stop_process(struct vfe_share_ctrl_t *share_ctrl)
{
	uint32_t vfe_mode =
	share_ctrl->current_mode & ~(VFE_OUTPUTS_RDI0|
		VFE_OUTPUTS_RDI1|VFE_OUTPUTS_RDI2);

	if (share_ctrl->current_mode & VFE_OUTPUTS_RDI0) {
		axi_stop_rdi0(share_ctrl);
		axi_disable_wm_irq(share_ctrl,
			VFE32_OUTPUT_MODE_TERTIARY1);
		share_ctrl->comp_output_mode &= ~VFE32_OUTPUT_MODE_TERTIARY1;
		share_ctrl->operation_mode &=
			~(VFE_OUTPUTS_RDI0);
	}
	if (share_ctrl->current_mode & VFE_OUTPUTS_RDI1) {
		axi_stop_rdi1(share_ctrl);
		axi_disable_wm_irq(share_ctrl,
			VFE32_OUTPUT_MODE_TERTIARY2);
		share_ctrl->comp_output_mode &= ~VFE32_OUTPUT_MODE_TERTIARY2;
		share_ctrl->operation_mode &=
			~(VFE_OUTPUTS_RDI1);
	}
	if (share_ctrl->current_mode & VFE_OUTPUTS_RDI2) {
		axi_stop_rdi2(share_ctrl);
		axi_disable_wm_irq(share_ctrl,
			VFE32_OUTPUT_MODE_TERTIARY3);
		share_ctrl->comp_output_mode &= ~VFE32_OUTPUT_MODE_TERTIARY3;
		share_ctrl->operation_mode &=
			~(VFE_OUTPUTS_RDI2);
	}
	if (vfe_mode) {
		uint16_t mode = share_ctrl->comp_output_mode &
			~(VFE32_OUTPUT_MODE_TERTIARY1|
			VFE32_OUTPUT_MODE_TERTIARY2|
			  VFE32_OUTPUT_MODE_TERTIARY3);
		axi_stop_pix(share_ctrl, vfe_mode, share_ctrl->cmd_type);
		axi_disable_wm_irq(share_ctrl, mode);
		share_ctrl->comp_output_mode &=
				(VFE32_OUTPUT_MODE_TERTIARY1|
				VFE32_OUTPUT_MODE_TERTIARY2|
				 VFE32_OUTPUT_MODE_TERTIARY3);
	}
}

static void vfe32_process_reg_update_irq(
		struct vfe32_ctrl_type *vfe32_ctrl)
{
	unsigned long flags = 0;
	struct vfe_share_ctrl_t *share_ctrl = vfe32_ctrl->share_ctrl;
	if (atomic_read(
		&share_ctrl->pix0_update_ack_pending) == 2) {
		uint32_t vfe_mode =
				share_ctrl->operation_mode & ~(VFE_OUTPUTS_RDI0|
					VFE_OUTPUTS_RDI1|VFE_OUTPUTS_RDI2);

		if (share_ctrl->dual_enabled && !share_ctrl->update_counter) {
			axi_stop_pix(share_ctrl, vfe_mode,
				share_ctrl->cmd_type);
			share_ctrl->update_counter++;
		} else {
			uint16_t output_mode =
				share_ctrl->comp_output_mode &
				~(VFE32_OUTPUT_MODE_TERTIARY1|
				VFE32_OUTPUT_MODE_TERTIARY2|
				  VFE32_OUTPUT_MODE_TERTIARY3);
			share_ctrl->update_counter = 0;
			if (!share_ctrl->dual_enabled)
				axi_stop_pix(share_ctrl, vfe_mode,
					share_ctrl->cmd_type);
			axi_disable_wm_irq(share_ctrl, output_mode);
			axi_disable_irq(share_ctrl, vfe_mode);
			atomic_set(&share_ctrl->pix0_update_ack_pending, 0);
			msm_camera_io_w_mb(
					CAMIF_COMMAND_STOP_AT_FRAME_BOUNDARY,
					share_ctrl->vfebase +
					VFE_CAMIF_COMMAND);
			vfe32_send_isp_msg(&vfe32_ctrl->subdev,
				share_ctrl->vfeFrameId,
				MSG_ID_PIX0_UPDATE_ACK);
			share_ctrl->comp_output_mode &=
				(VFE32_OUTPUT_MODE_TERTIARY1|
				VFE32_OUTPUT_MODE_TERTIARY2|
				 VFE32_OUTPUT_MODE_TERTIARY3);
		}
	}  else {
		if (share_ctrl->recording_state == VFE_STATE_START_REQUESTED) {
			if (share_ctrl->operation_mode &
					VFE_OUTPUTS_VIDEO_AND_PREVIEW) {
				msm_camera_io_w((
					0x1 << share_ctrl->outpath.out0.ch0 |
					0x1 << share_ctrl->outpath.out0.ch1),
					share_ctrl->vfebase + VFE_BUS_CMD);
				msm_camera_io_w(1,
					share_ctrl->vfebase + vfe32_AXI_WM_CFG[
					share_ctrl->outpath.out0.ch0]);
				msm_camera_io_w(1,
					share_ctrl->vfebase + vfe32_AXI_WM_CFG[
					share_ctrl->outpath.out0.ch1]);
			} else if (share_ctrl->operation_mode &
					VFE_OUTPUTS_PREVIEW_AND_VIDEO) {
				msm_camera_io_w((
					0x1 << share_ctrl->outpath.out1.ch0 |
					0x1 << share_ctrl->outpath.out1.ch1),
					share_ctrl->vfebase + VFE_BUS_CMD);
				msm_camera_io_w(1,
					share_ctrl->vfebase + vfe32_AXI_WM_CFG[
					share_ctrl->outpath.out1.ch0]);
				msm_camera_io_w(1,
					share_ctrl->vfebase + vfe32_AXI_WM_CFG[
					share_ctrl->outpath.out1.ch1]);
			}
			share_ctrl->recording_state = VFE_STATE_STARTED;
			msm_camera_io_w_mb(1,
				share_ctrl->vfebase + VFE_REG_UPDATE_CMD);
			CDBG("start video triggered .\n");
		} else if (share_ctrl->recording_state ==
					VFE_STATE_STOP_REQUESTED) {
			if (share_ctrl->operation_mode &
					VFE_OUTPUTS_VIDEO_AND_PREVIEW) {
				msm_camera_io_w(0,
					share_ctrl->vfebase + vfe32_AXI_WM_CFG[
					share_ctrl->outpath.out0.ch0]);
				msm_camera_io_w(0,
					share_ctrl->vfebase + vfe32_AXI_WM_CFG[
					share_ctrl->outpath.out0.ch1]);
			} else if (share_ctrl->operation_mode &
					VFE_OUTPUTS_PREVIEW_AND_VIDEO) {
				msm_camera_io_w(0,
					share_ctrl->vfebase + vfe32_AXI_WM_CFG[
					share_ctrl->outpath.out1.ch0]);
				msm_camera_io_w(0,
					share_ctrl->vfebase + vfe32_AXI_WM_CFG[
					share_ctrl->outpath.out1.ch1]);
			}
			CDBG("stop video triggered .\n");
		}

		if (atomic_cmpxchg(
		&share_ctrl->pix0_update_ack_pending, 1, 0) == 1) {
			share_ctrl->comp_output_mode |=
				(share_ctrl->outpath.output_mode
				& ~(VFE32_OUTPUT_MODE_TERTIARY1|
					VFE32_OUTPUT_MODE_TERTIARY2|
					VFE32_OUTPUT_MODE_TERTIARY3));
			vfe32_send_isp_msg(&vfe32_ctrl->subdev,
				share_ctrl->vfeFrameId, MSG_ID_PIX0_UPDATE_ACK);
		} else {
			if (share_ctrl->recording_state ==
						VFE_STATE_STOP_REQUESTED) {
				share_ctrl->recording_state = VFE_STATE_STOPPED;
				/* request a reg update and send STOP_REC_ACK
				 * when we process the next reg update irq.
				 */
				msm_camera_io_w_mb(1, share_ctrl->vfebase +
							VFE_REG_UPDATE_CMD);
			} else if (share_ctrl->recording_state ==
						VFE_STATE_STOPPED) {
				vfe32_send_isp_msg(&vfe32_ctrl->subdev,
					share_ctrl->vfeFrameId,
					MSG_ID_STOP_REC_ACK);
				share_ctrl->recording_state = VFE_STATE_IDLE;
			}
			spin_lock_irqsave(
				&share_ctrl->update_ack_lock,
				flags);
			if (share_ctrl->update_ack_pending == TRUE) {
				share_ctrl->update_ack_pending = FALSE;
				spin_unlock_irqrestore(
					&share_ctrl->update_ack_lock, flags);
				vfe32_send_isp_msg(&vfe32_ctrl->subdev,
					share_ctrl->vfeFrameId,
					MSG_ID_UPDATE_ACK);
			} else {
				spin_unlock_irqrestore(
					&share_ctrl->update_ack_lock, flags);
			}
		}

		switch (share_ctrl->liveshot_state) {
		case VFE_STATE_START_REQUESTED:
			CDBG("%s enabling liveshot output\n", __func__);
			if (share_ctrl->comp_output_mode &
						VFE32_OUTPUT_MODE_PRIMARY) {
				msm_camera_io_w((
					0x1 << share_ctrl->outpath.out0.ch0 |
					0x1 << share_ctrl->outpath.out0.ch1),
					share_ctrl->vfebase + VFE_BUS_CMD);
				msm_camera_io_w(1, share_ctrl->vfebase +
					vfe32_AXI_WM_CFG[
					share_ctrl->outpath.out0.ch0]);
				msm_camera_io_w(1, share_ctrl->vfebase +
					vfe32_AXI_WM_CFG[
					share_ctrl->outpath.out0.ch1]);

				share_ctrl->liveshot_state =
					VFE_STATE_STARTED;
			}
			break;
		case VFE_STATE_STARTED:
			share_ctrl->vfe_capture_count--;
			if (!share_ctrl->vfe_capture_count &&
				(share_ctrl->comp_output_mode &
					VFE32_OUTPUT_MODE_PRIMARY)) {
				msm_camera_io_w(0, share_ctrl->vfebase +
					vfe32_AXI_WM_CFG[
					share_ctrl->outpath.out0.ch0]);
				msm_camera_io_w(0, share_ctrl->vfebase +
					vfe32_AXI_WM_CFG[
					share_ctrl->outpath.out0.ch1]);
#if defined(CONFIG_MACH_JACTIVE_ATT) || defined(CONFIG_MACH_JACTIVE_EUR)
				share_ctrl->liveshot_state = VFE_STATE_HW_STOP_REQUESTED;
				msm_camera_io_w_mb(1, share_ctrl->vfebase +
						VFE_REG_UPDATE_CMD);
#endif
			}
			break;
#if defined(CONFIG_MACH_JACTIVE_ATT) || defined(CONFIG_MACH_JACTIVE_EUR)
		case VFE_STATE_HW_STOP_REQUESTED:
			share_ctrl->liveshot_state = VFE_STATE_HW_STOPPED;
			break;
#endif
		case VFE_STATE_STOP_REQUESTED:
			if (share_ctrl->comp_output_mode &
					VFE32_OUTPUT_MODE_PRIMARY) {
				/* Stop requested, stop write masters, and
				 * trigger REG_UPDATE. Send STOP_LS_ACK in
				 * next reg update. */
				msm_camera_io_w(0, share_ctrl->vfebase +
					vfe32_AXI_WM_CFG[
					share_ctrl->outpath.out0.ch0]);
				msm_camera_io_w(0, share_ctrl->vfebase +
					vfe32_AXI_WM_CFG[
					share_ctrl->outpath.out0.ch1]);

				share_ctrl->liveshot_state = VFE_STATE_STOPPED;
				msm_camera_io_w_mb(1, share_ctrl->vfebase +
					VFE_REG_UPDATE_CMD);
			}
			break;
		case VFE_STATE_STOPPED:
			CDBG("%s Sending STOP_LS ACK\n", __func__);
			vfe32_send_isp_msg(&vfe32_ctrl->subdev,
				share_ctrl->vfeFrameId, MSG_ID_STOP_LS_ACK);
			share_ctrl->liveshot_state = VFE_STATE_IDLE;
			break;
		default:
			break;
		}

		if ((share_ctrl->operation_mode & VFE_OUTPUTS_THUMB_AND_MAIN) ||
			(share_ctrl->operation_mode &
				VFE_OUTPUTS_MAIN_AND_THUMB) ||
			(share_ctrl->operation_mode &
				VFE_OUTPUTS_THUMB_AND_JPEG) ||
			(share_ctrl->operation_mode &
				VFE_OUTPUTS_JPEG_AND_THUMB)) {
			/* in snapshot mode */
			/* later we need to add check for live snapshot mode. */
			if (vfe32_ctrl->frame_skip_pattern & (0x1 <<
				(vfe32_ctrl->snapshot_frame_cnt %
					vfe32_ctrl->frame_skip_cnt))) {
				share_ctrl->vfe_capture_count--;
				/* if last frame to be captured: */
				if (share_ctrl->vfe_capture_count == 0) {
					/* stop the bus output: */
					uint32_t vfe_mode =
						share_ctrl->operation_mode &
							~(VFE_OUTPUTS_RDI0|
							VFE_OUTPUTS_RDI1|
							VFE_OUTPUTS_RDI2);
					axi_stop_pix(share_ctrl, vfe_mode,
						AXI_CMD_CAPTURE);
					msm_camera_io_w_mb
					(CAMIF_COMMAND_STOP_AT_FRAME_BOUNDARY,
					share_ctrl->vfebase +
					VFE_CAMIF_COMMAND);
					vfe32_ctrl->snapshot_frame_cnt = -1;
					vfe32_ctrl->frame_skip_cnt = 31;
					vfe32_ctrl->frame_skip_pattern =
								0xffffffff;
				} /*if snapshot count is 0*/
			} /*if frame is not being dropped*/
			vfe32_ctrl->snapshot_frame_cnt++;
			/* then do reg_update. */
			msm_camera_io_w(1,
				share_ctrl->vfebase + VFE_REG_UPDATE_CMD);
		} /* if snapshot mode. */
	}
}

static void vfe32_process_rdi0_reg_update_irq(
	struct vfe32_ctrl_type *vfe32_ctrl)
{
	if (atomic_cmpxchg(
		&vfe32_ctrl->share_ctrl->rdi0_update_ack_pending, 1, 0) == 1) {
		vfe32_ctrl->share_ctrl->comp_output_mode |=
			VFE32_OUTPUT_MODE_TERTIARY1;
		vfe32_send_isp_msg(&vfe32_ctrl->subdev,
			vfe32_ctrl->share_ctrl->rdi0FrameId,
			MSG_ID_RDI0_UPDATE_ACK);
	}

	if ((atomic_read(
		&vfe32_ctrl->share_ctrl->rdi0_update_ack_pending) == 2)
		|| (vfe32_ctrl->share_ctrl->rdi0_capture_count == 0)) {
		axi_disable_wm_irq(vfe32_ctrl->share_ctrl,
			VFE32_OUTPUT_MODE_TERTIARY1);
		axi_disable_irq(vfe32_ctrl->share_ctrl, VFE_OUTPUTS_RDI0);
		atomic_set(&vfe32_ctrl->share_ctrl->rdi0_update_ack_pending, 0);
		vfe32_send_isp_msg(&vfe32_ctrl->subdev,
			vfe32_ctrl->share_ctrl->rdi0FrameId,
			MSG_ID_RDI0_UPDATE_ACK);

		if (vfe32_ctrl->share_ctrl->rdi0_capture_count == 0)
			vfe32_ctrl->share_ctrl->rdi0_capture_count = -1;
		if (vfe32_ctrl->share_ctrl->outpath.out2.capture_cnt
			== 0)
			vfe32_ctrl->share_ctrl->outpath.out2.capture_cnt = -1;
		vfe32_ctrl->share_ctrl->comp_output_mode &=
			~VFE32_OUTPUT_MODE_TERTIARY1;
		vfe32_ctrl->share_ctrl->operation_mode &=
			~(VFE_OUTPUTS_RDI0);
	}

	if (vfe32_ctrl->share_ctrl->rdi0_capture_count > 0) {
		vfe32_ctrl->share_ctrl->rdi0_capture_count--;
		if (!vfe32_ctrl->share_ctrl->rdi0_capture_count)
			axi_stop_rdi0(vfe32_ctrl->share_ctrl);
	}
}

static void vfe32_process_rdi1_reg_update_irq(
	struct vfe32_ctrl_type *vfe32_ctrl)
{

	if (atomic_cmpxchg(
		&vfe32_ctrl->share_ctrl->rdi1_update_ack_pending, 1, 0)
				== 1) {
		vfe32_ctrl->share_ctrl->comp_output_mode |=
			VFE32_OUTPUT_MODE_TERTIARY2;
		vfe32_send_isp_msg(&vfe32_ctrl->subdev,
			vfe32_ctrl->share_ctrl->rdi1FrameId,
			MSG_ID_RDI1_UPDATE_ACK);
	}

	if ((atomic_read(
		&vfe32_ctrl->share_ctrl->rdi1_update_ack_pending) == 2)
		|| (vfe32_ctrl->share_ctrl->rdi1_capture_count == 0)) {
		axi_disable_wm_irq(vfe32_ctrl->share_ctrl,
			VFE32_OUTPUT_MODE_TERTIARY2);
		axi_disable_irq(vfe32_ctrl->share_ctrl, VFE_OUTPUTS_RDI1);
		atomic_set(&vfe32_ctrl->share_ctrl->rdi1_update_ack_pending, 0);
		vfe32_send_isp_msg(&vfe32_ctrl->subdev,
			vfe32_ctrl->share_ctrl->rdi1FrameId,
			MSG_ID_RDI1_UPDATE_ACK);

		if (vfe32_ctrl->share_ctrl->rdi1_capture_count == 0)
			vfe32_ctrl->share_ctrl->rdi1_capture_count = -1;
		if (vfe32_ctrl->share_ctrl->outpath.out3.capture_cnt
			== 0)
			vfe32_ctrl->share_ctrl->outpath.out3.capture_cnt = -1;
		vfe32_ctrl->share_ctrl->comp_output_mode &=
			~VFE32_OUTPUT_MODE_TERTIARY2;
		vfe32_ctrl->share_ctrl->operation_mode &=
			~(VFE_OUTPUTS_RDI1);
	}

	if (vfe32_ctrl->share_ctrl->rdi1_capture_count > 0) {
		vfe32_ctrl->share_ctrl->rdi1_capture_count--;
		if (!vfe32_ctrl->share_ctrl->rdi1_capture_count)
			axi_stop_rdi1(vfe32_ctrl->share_ctrl);
	}
}

static void vfe32_process_rdi2_reg_update_irq(
	struct vfe32_ctrl_type *vfe32_ctrl)
{
	pr_err("%s: RDI2\n", __func__);
	if (atomic_cmpxchg(
		&vfe32_ctrl->share_ctrl->rdi2_update_ack_pending, 1, 0)
				== 1) {
		vfe32_ctrl->share_ctrl->comp_output_mode |=
			VFE32_OUTPUT_MODE_TERTIARY3;
		vfe32_send_isp_msg(&vfe32_ctrl->subdev,
			vfe32_ctrl->share_ctrl->rdi2FrameId,
			MSG_ID_RDI2_UPDATE_ACK);
	}

	if ((atomic_read(
		&vfe32_ctrl->share_ctrl->rdi2_update_ack_pending) == 2)
		|| (vfe32_ctrl->share_ctrl->rdi2_capture_count == 0)) {
		axi_disable_wm_irq(vfe32_ctrl->share_ctrl,
			VFE32_OUTPUT_MODE_TERTIARY3);
		axi_disable_irq(vfe32_ctrl->share_ctrl, VFE_OUTPUTS_RDI2);
		atomic_set(&vfe32_ctrl->share_ctrl->rdi2_update_ack_pending, 0);
		vfe32_send_isp_msg(&vfe32_ctrl->subdev,
			vfe32_ctrl->share_ctrl->rdi2FrameId,
			MSG_ID_RDI2_UPDATE_ACK);

		if (vfe32_ctrl->share_ctrl->rdi2_capture_count == 0)
			vfe32_ctrl->share_ctrl->rdi2_capture_count = -1;
		if (vfe32_ctrl->share_ctrl->outpath.out4.capture_cnt
			== 0)
			vfe32_ctrl->share_ctrl->outpath.out4.capture_cnt = -1;
		vfe32_ctrl->share_ctrl->comp_output_mode &=
			~VFE32_OUTPUT_MODE_TERTIARY3;
		vfe32_ctrl->share_ctrl->operation_mode &=
			~(VFE_OUTPUTS_RDI2);
	}

	if (vfe32_ctrl->share_ctrl->rdi2_capture_count > 0) {
		vfe32_ctrl->share_ctrl->rdi2_capture_count--;
		if (!vfe32_ctrl->share_ctrl->rdi2_capture_count)
			axi_stop_rdi2(vfe32_ctrl->share_ctrl);
	}
}

static void vfe32_process_reset_irq(
		struct vfe32_ctrl_type *vfe32_ctrl)
{
	unsigned long flags = 0;

#if defined(CONFIG_MACH_JACTIVE_ATT) || defined(CONFIG_MACH_JACTIVE_EUR)
	if (atomic_read(&recovery_active) == 1) {
		printk("Recovery restart stream\n");
		msm_camera_io_w(0x3FFF,
				vfe32_ctrl->share_ctrl->vfebase + VFE_BUS_CMD);
		msm_camera_io_w(recover_irq_mask0, vfe32_ctrl->share_ctrl->vfebase + VFE_IRQ_MASK_0);
		msm_camera_io_w(recover_irq_mask1, vfe32_ctrl->share_ctrl->vfebase + VFE_IRQ_MASK_1);

		if (vfe32_ctrl->share_ctrl->liveshot_state == VFE_STATE_START_REQUESTED ||
				vfe32_ctrl->share_ctrl->liveshot_state == VFE_STATE_STARTED ||
				vfe32_ctrl->share_ctrl->liveshot_state == VFE_STATE_HW_STOP_REQUESTED) {
			printk("Liveshot recovery\n");
			vfe32_ctrl->share_ctrl->outpath.out0.capture_cnt = 1;
			vfe32_ctrl->share_ctrl->vfe_capture_count =
				vfe32_ctrl->share_ctrl->outpath.out0.capture_cnt;
			vfe32_ctrl->share_ctrl->liveshot_state = VFE_STATE_START_REQUESTED;
		}
		msm_camera_io_w_mb(0x3,
				vfe32_ctrl->share_ctrl->vfebase + VFE_REG_UPDATE_CMD);
		printk("camif cfg: 0x%x\n", msm_camera_io_r(vfe32_ctrl->share_ctrl->vfebase + 0x1EC));
		msm_camera_io_w_mb(0x4, vfe32_ctrl->share_ctrl->vfebase + 0x1E0);
		msm_camera_io_w_mb(0x1, vfe32_ctrl->share_ctrl->vfebase + 0x1E0);
		atomic_set(&recovery_active, 0);
		printk("Recovery restart done\n");
		return;
	}
#endif

	atomic_set(&vfe32_ctrl->share_ctrl->vstate, 0);
	atomic_set(&vfe32_ctrl->share_ctrl->handle_common_irq, 0);

	spin_lock_irqsave(&vfe32_ctrl->share_ctrl->stop_flag_lock, flags);
	if (vfe32_ctrl->share_ctrl->stop_ack_pending) {
		vfe32_ctrl->share_ctrl->stop_ack_pending = FALSE;
		spin_unlock_irqrestore(
			&vfe32_ctrl->share_ctrl->stop_flag_lock, flags);
		if (vfe32_ctrl->share_ctrl->sync_abort)
			complete(&vfe32_ctrl->share_ctrl->reset_complete);
		else
			vfe32_send_isp_msg(&vfe32_ctrl->subdev,
				vfe32_ctrl->share_ctrl->vfeFrameId,
				MSG_ID_STOP_ACK);
	} else {
		spin_unlock_irqrestore(
			&vfe32_ctrl->share_ctrl->stop_flag_lock, flags);
		/* this is from reset command. */
		vfe32_reset_internal_variables(vfe32_ctrl);
		if (vfe32_ctrl->share_ctrl->vfe_reset_flag) {
			vfe32_ctrl->share_ctrl->vfe_reset_flag = false;
			msm_camera_io_w(0x7F80,
				vfe32_ctrl->share_ctrl->vfebase + VFE_BUS_CMD);
		} else {
			/* reload all write masters. (frame & line)*/
			msm_camera_io_w(0x7FFF,
				vfe32_ctrl->share_ctrl->vfebase + VFE_BUS_CMD);
		}
		complete(&vfe32_ctrl->share_ctrl->reset_complete);
	}
}

static void vfe32_process_camif_sof_irq(
		struct vfe32_ctrl_type *vfe32_ctrl)
{
	if (vfe32_ctrl->share_ctrl->operation_mode ==
		VFE_OUTPUTS_RAW) {
		if (atomic_cmpxchg(
			&vfe32_ctrl->share_ctrl->pix0_update_ack_pending,
					1, 0) == 1) {
			vfe32_ctrl->share_ctrl->comp_output_mode |=
				(vfe32_ctrl->share_ctrl->outpath.output_mode
				& ~(VFE32_OUTPUT_MODE_TERTIARY1|
				VFE32_OUTPUT_MODE_TERTIARY2|
				VFE32_OUTPUT_MODE_TERTIARY3));
			vfe32_send_isp_msg(&vfe32_ctrl->subdev,
				vfe32_ctrl->share_ctrl->vfeFrameId,
				MSG_ID_PIX0_UPDATE_ACK);
		}

		vfe32_ctrl->share_ctrl->vfe_capture_count--;
		/* if last frame to be captured: */
		if (vfe32_ctrl->share_ctrl->vfe_capture_count == 0) {
			/* Ensure the write order while writing
			 to the command register using the barrier */
			msm_camera_io_w_mb(CAMIF_COMMAND_STOP_AT_FRAME_BOUNDARY,
			vfe32_ctrl->share_ctrl->vfebase + VFE_CAMIF_COMMAND);
		}
	} /* if raw snapshot mode. */
	if ((vfe32_ctrl->hfr_mode != HFR_MODE_OFF) &&
		(vfe32_ctrl->share_ctrl->operation_mode ==
			VFE_MODE_OF_OPERATION_VIDEO) &&
		(vfe32_ctrl->share_ctrl->vfeFrameId %
			vfe32_ctrl->hfr_mode != 0)) {
		if (vfe32_ctrl->vfe_sof_count_enable)
			vfe32_ctrl->share_ctrl->vfeFrameId++;
		CDBG("Skip the SOF notification when HFR enabled\n");
		return;
	}
	if (vfe32_ctrl->vfe_sof_count_enable)
		vfe32_ctrl->share_ctrl->vfeFrameId++;

	vfe32_send_isp_msg(&vfe32_ctrl->subdev,
		vfe32_ctrl->share_ctrl->vfeFrameId, MSG_ID_SOF_ACK);
	CDBG("camif_sof_irq, frameId = %d\n",
		vfe32_ctrl->share_ctrl->vfeFrameId);

	if (vfe32_ctrl->sync_timer_state) {
		if (vfe32_ctrl->sync_timer_repeat_count == 0)
			vfe32_sync_timer_stop(vfe32_ctrl);
		else
			vfe32_ctrl->sync_timer_repeat_count--;
	}
}

static void vfe32_process_error_irq(
	struct axi_ctrl_t *axi_ctrl, uint32_t errStatus)
{
	uint32_t reg_value;
	if (errStatus & VFE32_IMASK_VIOLATION) {
		pr_err("vfe32_irq: violation interrupt\n");
		reg_value = msm_camera_io_r(
			axi_ctrl->share_ctrl->vfebase + VFE_VIOLATION_STATUS);
		pr_err("%s: violationStatus  = 0x%x\n", __func__, reg_value);
	}

	if (errStatus & VFE32_IMASK_CAMIF_ERROR) {
		pr_err("vfe32_irq: camif errors\n");
		reg_value = msm_camera_io_r(
			axi_ctrl->share_ctrl->vfebase + VFE_CAMIF_STATUS);

#if defined(CONFIG_MACH_JACTIVE_ATT) || defined(CONFIG_MACH_JACTIVE_EUR)
		if (reg_value & ~0x80000000) {
			v4l2_subdev_notify(&axi_ctrl->subdev,
				NOTIFY_VFE_CAMIF_ERROR, (void *)NULL);
			pr_err("camifStatus  = 0x%x\n", reg_value);
			vfe32_send_isp_msg(&axi_ctrl->subdev,
			axi_ctrl->share_ctrl->vfeFrameId, MSG_ID_CAMIF_ERROR);
		}
#else
#if 0 //for samsung esd recovery
			v4l2_subdev_notify(&axi_ctrl->subdev,
					NOTIFY_VFE_CAMIF_ERROR, (void *)NULL);
#endif
		pr_err("camifStatus  = 0x%x\n", reg_value);
		vfe32_send_isp_msg(&axi_ctrl->subdev,
			axi_ctrl->share_ctrl->vfeFrameId, MSG_ID_CAMIF_ERROR);
#endif
	}

	if (errStatus & VFE32_IMASK_BHIST_OVWR)
		pr_err("vfe32_irq: stats bhist overwrite\n");

	if (errStatus & VFE32_IMASK_STATS_CS_OVWR)
		pr_err("vfe32_irq: stats cs overwrite\n");

	if (errStatus & VFE32_IMASK_STATS_IHIST_OVWR)
		pr_err("vfe32_irq: stats ihist overwrite\n");

	if (errStatus & VFE32_IMASK_REALIGN_BUF_Y_OVFL)
		pr_err("vfe32_irq: realign bug Y overflow\n");

	if (errStatus & VFE32_IMASK_REALIGN_BUF_CB_OVFL)
		pr_err("vfe32_irq: realign bug CB overflow\n");

	if (errStatus & VFE32_IMASK_REALIGN_BUF_CR_OVFL)
		pr_err("vfe32_irq: realign bug CR overflow\n");

	if (errStatus & VFE32_IMASK_STATS_AE_BG_BUS_OVFL)
		pr_err("vfe32_irq: ae/bg stats bus overflow\n");

	if (errStatus & VFE32_IMASK_STATS_AF_BF_BUS_OVFL)
		pr_err("vfe32_irq: af/bf stats bus overflow\n");

	if (errStatus & VFE32_IMASK_STATS_AWB_BUS_OVFL)
		pr_err("vfe32_irq: awb stats bus overflow\n");

	if (errStatus & VFE32_IMASK_STATS_RS_BUS_OVFL)
		pr_err("vfe32_irq: rs stats bus overflow\n");

	if (errStatus & VFE32_IMASK_STATS_CS_BUS_OVFL)
		pr_err("vfe32_irq: cs stats bus overflow\n");

	if (errStatus & VFE32_IMASK_STATS_IHIST_BUS_OVFL)
		pr_err("vfe32_irq: ihist stats bus overflow\n");

	if (errStatus & VFE32_IMASK_STATS_SKIN_BHIST_BUS_OVFL)
		pr_err("vfe32_irq: skin/bhist stats bus overflow\n");

	if (errStatus & VFE32_IMASK_BUS_OVFL_ERROR) {
		pr_err("%s Bus Overflow. Notify error ", __func__);
		/*
		* Even though bus overflow is occurred, 
		* don't notify it because it will recover automatically.
		*/
	}	
}

static void vfe32_process_common_error_irq(
	struct axi_ctrl_t *axi_ctrl, uint32_t errStatus)
{

	if (errStatus & VFE32_IMASK_IMG_MAST_0_BUS_OVFL)
		pr_err("vfe32_irq: image master 0 bus overflow\n");

	if (errStatus & VFE32_IMASK_IMG_MAST_1_BUS_OVFL)
		pr_err("vfe32_irq: image master 1 bus overflow\n");

	if (errStatus & VFE32_IMASK_IMG_MAST_2_BUS_OVFL)
		pr_err("vfe32_irq: image master 2 bus overflow\n");

	if (errStatus & VFE32_IMASK_IMG_MAST_3_BUS_OVFL)
		pr_err("vfe32_irq: image master 3 bus overflow\n");

	if (errStatus & VFE32_IMASK_IMG_MAST_4_BUS_OVFL)
		pr_err("vfe32_irq: image master 4 bus overflow\n");

	if (errStatus & VFE32_IMASK_IMG_MAST_5_BUS_OVFL)
		pr_err("vfe32_irq: image master 5 bus overflow\n");

	if (errStatus & VFE32_IMASK_IMG_MAST_6_BUS_OVFL)
		pr_err("vfe32_irq: image master 6 bus overflow\n");

	if (errStatus & VFE32_IMASK_AXI_ERROR)
		pr_err("vfe32_irq: axi error\n");

	pr_err("axi vfe32_irq: errStatus(%d)\n", errStatus);

#if 0 // Erased for bus overflow recovery patch, automatically recover.
	v4l2_subdev_notify(&axi_ctrl->subdev, NOTIFY_VFE_CAMIF_ERROR,
		(void *)NULL);
	vfe32_send_isp_msg(&axi_ctrl->subdev,
		axi_ctrl->share_ctrl->vfeFrameId, MSG_ID_CAMIF_ERROR);
#endif	
}


static void vfe_send_outmsg(
	struct axi_ctrl_t *axi_ctrl, uint8_t msgid,
	uint32_t ch0_paddr, uint32_t ch1_paddr,
	uint32_t ch2_paddr, uint32_t inst_handle)
{
	struct isp_msg_output msg;

	msg.output_id = msgid;
	msg.buf.inst_handle = inst_handle;
	msg.buf.ch_paddr[0]	= ch0_paddr;
	msg.buf.ch_paddr[1]	= ch1_paddr;
	msg.buf.ch_paddr[2]	= ch2_paddr;
	switch (msgid) {
	case MSG_ID_OUTPUT_TERTIARY1:
		msg.frameCounter = axi_ctrl->share_ctrl->rdi0FrameId;
		break;
	case MSG_ID_OUTPUT_TERTIARY2:
		msg.frameCounter = axi_ctrl->share_ctrl->rdi1FrameId;
		break;
	case MSG_ID_OUTPUT_TERTIARY3:
		msg.frameCounter = axi_ctrl->share_ctrl->rdi2FrameId;
		break;
	default:
		msg.frameCounter = axi_ctrl->share_ctrl->vfeFrameId;
		break;
	}

	v4l2_subdev_notify(&axi_ctrl->subdev,
			NOTIFY_VFE_MSG_OUT,
			&msg);
	return;
}

static void vfe32_process_output_path_irq_0(
	struct axi_ctrl_t *axi_ctrl)
{
	uint32_t ping_pong;
	uint32_t ch0_paddr, ch1_paddr, ch2_paddr;
	uint8_t out_bool = 0;
	struct msm_free_buf *free_buf = NULL;
	free_buf = vfe32_check_free_buffer(VFE_MSG_OUTPUT_IRQ,
		VFE_MSG_OUTPUT_PRIMARY, axi_ctrl);

	/* we render frames in the following conditions:
	1. Continuous mode and the free buffer is avaialable.
	2. In snapshot shot mode, free buffer is not always available.
	when pending snapshot count is <=1,  then no need to use
	free buffer.
	*/
#if defined(CONFIG_MACH_JACTIVE_ATT) || defined(CONFIG_MACH_JACTIVE_EUR)
	out_bool = (
			(axi_ctrl->share_ctrl->operation_mode &
			 VFE_OUTPUTS_THUMB_AND_MAIN ||
			 axi_ctrl->share_ctrl->operation_mode &
			 VFE_OUTPUTS_MAIN_AND_THUMB ||
			 axi_ctrl->share_ctrl->operation_mode &
			 VFE_OUTPUTS_THUMB_AND_JPEG ||
			 axi_ctrl->share_ctrl->operation_mode &
			 VFE_OUTPUTS_JPEG_AND_THUMB ||
			 axi_ctrl->share_ctrl->operation_mode &
			 VFE_OUTPUTS_RAW ||
			 axi_ctrl->share_ctrl->liveshot_state ==
			 VFE_STATE_STARTED ||
			 axi_ctrl->share_ctrl->liveshot_state ==
			 VFE_STATE_HW_STOP_REQUESTED ||
			 axi_ctrl->share_ctrl->liveshot_state ==
			 VFE_STATE_HW_STOPPED || 
			 axi_ctrl->share_ctrl->liveshot_state ==
			 VFE_STATE_STOP_REQUESTED ||
			 axi_ctrl->share_ctrl->liveshot_state ==
			 VFE_STATE_STOPPED) &&
			(axi_ctrl->share_ctrl->vfe_capture_count <= 1)) ||
			free_buf;
#else
	out_bool = (
		(axi_ctrl->share_ctrl->operation_mode &
			VFE_OUTPUTS_THUMB_AND_MAIN ||
		axi_ctrl->share_ctrl->operation_mode &
			VFE_OUTPUTS_MAIN_AND_THUMB ||
		axi_ctrl->share_ctrl->operation_mode &
			VFE_OUTPUTS_THUMB_AND_JPEG ||
		axi_ctrl->share_ctrl->operation_mode &
			VFE_OUTPUTS_JPEG_AND_THUMB ||
		axi_ctrl->share_ctrl->operation_mode &
			VFE_OUTPUTS_RAW ||
		axi_ctrl->share_ctrl->liveshot_state ==
			VFE_STATE_STARTED ||
		axi_ctrl->share_ctrl->liveshot_state ==
			VFE_STATE_STOP_REQUESTED ||
		axi_ctrl->share_ctrl->liveshot_state ==
			VFE_STATE_STOPPED) &&
		(axi_ctrl->share_ctrl->vfe_capture_count <= 1)) ||
			free_buf;
#endif

	if (out_bool) {
		ping_pong = msm_camera_io_r(axi_ctrl->share_ctrl->vfebase +
			VFE_BUS_PING_PONG_STATUS);

		/* Channel 0*/
		ch0_paddr = vfe32_get_ch_addr(
			ping_pong, axi_ctrl->share_ctrl->vfebase,
			axi_ctrl->share_ctrl->outpath.out0.ch0);
		/* Channel 1*/
		ch1_paddr = vfe32_get_ch_addr(
			ping_pong, axi_ctrl->share_ctrl->vfebase,
			axi_ctrl->share_ctrl->outpath.out0.ch1);
		/* Channel 2*/
		ch2_paddr = vfe32_get_ch_addr(
			ping_pong, axi_ctrl->share_ctrl->vfebase,
			axi_ctrl->share_ctrl->outpath.out0.ch2);

		CDBG("output path 0, ch0 = 0x%x, ch1 = 0x%x, ch2 = 0x%x\n",
			ch0_paddr, ch1_paddr, ch2_paddr);
		if (free_buf) {
			/* Y channel */
			vfe32_put_ch_addr(ping_pong,
			axi_ctrl->share_ctrl->vfebase,
			axi_ctrl->share_ctrl->outpath.out0.ch0,
			free_buf->ch_paddr[0]);
			/* Chroma channel */
			vfe32_put_ch_addr(ping_pong,
			axi_ctrl->share_ctrl->vfebase,
			axi_ctrl->share_ctrl->outpath.out0.ch1,
			free_buf->ch_paddr[1]);
			if (free_buf->num_planes > 2)
				vfe32_put_ch_addr(ping_pong,
					axi_ctrl->share_ctrl->vfebase,
					axi_ctrl->share_ctrl->outpath.out0.ch2,
					free_buf->ch_paddr[2]);
		}
		if (axi_ctrl->share_ctrl->operation_mode &
				VFE_OUTPUTS_THUMB_AND_MAIN ||
			axi_ctrl->share_ctrl->operation_mode &
				VFE_OUTPUTS_MAIN_AND_THUMB ||
			axi_ctrl->share_ctrl->operation_mode &
				VFE_OUTPUTS_THUMB_AND_JPEG ||
			axi_ctrl->share_ctrl->operation_mode &
				VFE_OUTPUTS_JPEG_AND_THUMB ||
			axi_ctrl->share_ctrl->operation_mode &
				VFE_OUTPUTS_RAW ||
			axi_ctrl->share_ctrl->liveshot_state ==
				VFE_STATE_STOPPED)
			axi_ctrl->share_ctrl->outpath.out0.capture_cnt--;

		vfe_send_outmsg(axi_ctrl,
			MSG_ID_OUTPUT_PRIMARY, ch0_paddr,
			ch1_paddr, ch2_paddr,
			axi_ctrl->share_ctrl->outpath.out0.inst_handle);

	} else {
		axi_ctrl->share_ctrl->outpath.out0.frame_drop_cnt++;
		CDBG("path_irq_0 - no free buffer!\n");
	}
}

static void vfe32_process_output_path_irq_1(
	struct axi_ctrl_t *axi_ctrl)
{
	uint32_t ping_pong;
	uint32_t ch0_paddr, ch1_paddr, ch2_paddr;
	/* this must be snapshot main image output. */
	uint8_t out_bool = 0;
	struct msm_free_buf *free_buf = NULL;

	free_buf = vfe32_check_free_buffer(VFE_MSG_OUTPUT_IRQ,
		VFE_MSG_OUTPUT_SECONDARY, axi_ctrl);
	out_bool = ((axi_ctrl->share_ctrl->operation_mode &
				VFE_OUTPUTS_THUMB_AND_MAIN ||
			axi_ctrl->share_ctrl->operation_mode &
				VFE_OUTPUTS_MAIN_AND_THUMB ||
			axi_ctrl->share_ctrl->operation_mode &
				VFE_OUTPUTS_RAW ||
			axi_ctrl->share_ctrl->operation_mode &
				VFE_OUTPUTS_JPEG_AND_THUMB) &&
			(axi_ctrl->share_ctrl->vfe_capture_count <= 1)) ||
				free_buf;

	if (out_bool) {
		ping_pong = msm_camera_io_r(axi_ctrl->share_ctrl->vfebase +
			VFE_BUS_PING_PONG_STATUS);

		/* Y channel */
		ch0_paddr = vfe32_get_ch_addr(ping_pong,
			axi_ctrl->share_ctrl->vfebase,
			axi_ctrl->share_ctrl->outpath.out1.ch0);
		/* Chroma channel */
		ch1_paddr = vfe32_get_ch_addr(ping_pong,
			axi_ctrl->share_ctrl->vfebase,
			axi_ctrl->share_ctrl->outpath.out1.ch1);
		ch2_paddr = vfe32_get_ch_addr(ping_pong,
			axi_ctrl->share_ctrl->vfebase,
			axi_ctrl->share_ctrl->outpath.out1.ch2);

		CDBG("%s ch0 = 0x%x, ch1 = 0x%x, ch2 = 0x%x\n",
			__func__, ch0_paddr, ch1_paddr, ch2_paddr);
		if (free_buf) {
			/* Y channel */
			vfe32_put_ch_addr(ping_pong,
			axi_ctrl->share_ctrl->vfebase,
			axi_ctrl->share_ctrl->outpath.out1.ch0,
			free_buf->ch_paddr[0]);
			/* Chroma channel */
			vfe32_put_ch_addr(ping_pong,
			axi_ctrl->share_ctrl->vfebase,
			axi_ctrl->share_ctrl->outpath.out1.ch1,
			free_buf->ch_paddr[1]);
			if (free_buf->num_planes > 2)
				vfe32_put_ch_addr(ping_pong,
					axi_ctrl->share_ctrl->vfebase,
					axi_ctrl->share_ctrl->outpath.out1.ch2,
					free_buf->ch_paddr[2]);
		}
		if (axi_ctrl->share_ctrl->operation_mode &
				VFE_OUTPUTS_THUMB_AND_MAIN ||
			axi_ctrl->share_ctrl->operation_mode &
				VFE_OUTPUTS_MAIN_AND_THUMB ||
			axi_ctrl->share_ctrl->operation_mode &
				VFE_OUTPUTS_RAW ||
			axi_ctrl->share_ctrl->operation_mode &
				VFE_OUTPUTS_JPEG_AND_THUMB)
			axi_ctrl->share_ctrl->outpath.out1.capture_cnt--;

		vfe_send_outmsg(axi_ctrl,
			MSG_ID_OUTPUT_SECONDARY, ch0_paddr,
			ch1_paddr, ch2_paddr,
			axi_ctrl->share_ctrl->outpath.out1.inst_handle);

	} else {
		axi_ctrl->share_ctrl->outpath.out1.frame_drop_cnt++;
		CDBG("path_irq_1 - no free buffer!\n");
	}
}

#ifdef QCT_NOT_USED /* Disabled for SOC sensor with Y on RDI0 & CB on RDI1 */
static void vfe32_process_output_path_irq_rdi0(
			struct axi_ctrl_t *axi_ctrl)
{
	uint32_t ping_pong;
	uint32_t ch0_paddr = 0;
	/* this must be rdi image output. */
	struct msm_free_buf *free_buf = NULL;
	/*RDI0*/
	CDBG("rdi0 out irq\n");
	if (axi_ctrl->share_ctrl->operation_mode & VFE_OUTPUTS_RDI0) {
		free_buf = vfe32_check_free_buffer(VFE_MSG_OUTPUT_IRQ,
			VFE_MSG_OUTPUT_TERTIARY1, axi_ctrl);
		if (axi_ctrl->share_ctrl->outpath.out2.capture_cnt > 0 ||
							free_buf) {
			ping_pong = msm_camera_io_r(axi_ctrl->
				share_ctrl->vfebase +
				VFE_BUS_PING_PONG_STATUS);

			/* Y only channel */
			ch0_paddr = vfe32_get_ch_addr(ping_pong,
				axi_ctrl->share_ctrl->vfebase,
				axi_ctrl->share_ctrl->outpath.out2.ch0);

			pr_debug("%s ch0 = 0x%x\n",
				__func__, ch0_paddr);

			if (free_buf)
				vfe32_put_ch_addr(ping_pong,
					axi_ctrl->share_ctrl->vfebase,
					axi_ctrl->share_ctrl->outpath.out2.ch0,
					free_buf->ch_paddr[0]);
			if (axi_ctrl->share_ctrl->outpath.out2.capture_cnt == 1)
				axi_ctrl->share_ctrl->
					outpath.out2.capture_cnt = 0;

			vfe_send_outmsg(axi_ctrl,
				MSG_ID_OUTPUT_TERTIARY1, ch0_paddr,
				0, 0,
				axi_ctrl->share_ctrl->outpath.out2.inst_handle);

		} else {
			axi_ctrl->share_ctrl->outpath.out2.frame_drop_cnt++;
			pr_err("path_irq_2 irq - no free buffer for rdi0!\n");
		}
	}
}

static void vfe32_process_output_path_irq_rdi1(
	struct axi_ctrl_t *axi_ctrl)
{
	uint32_t ping_pong;
	uint32_t ch0_paddr = 0;
	/* this must be rdi image output. */
	struct msm_free_buf *free_buf = NULL;
	/*RDI1*/
	if (axi_ctrl->share_ctrl->operation_mode & VFE_OUTPUTS_RDI1) {
		free_buf = vfe32_check_free_buffer(VFE_MSG_OUTPUT_IRQ,
			VFE_MSG_OUTPUT_TERTIARY2, axi_ctrl);
		if (axi_ctrl->share_ctrl->outpath.out3.capture_cnt > 0 ||
							free_buf) {
			ping_pong = msm_camera_io_r(axi_ctrl->
				share_ctrl->vfebase +
				VFE_BUS_PING_PONG_STATUS);

			/* Y channel */
			ch0_paddr = vfe32_get_ch_addr(ping_pong,
				axi_ctrl->share_ctrl->vfebase,
				axi_ctrl->share_ctrl->outpath.out3.ch0);
			pr_debug("%s ch0 = 0x%x\n",
				__func__, ch0_paddr);

			if (free_buf)
				vfe32_put_ch_addr(ping_pong,
					axi_ctrl->share_ctrl->vfebase,
					axi_ctrl->share_ctrl->outpath.out3.ch0,
					free_buf->ch_paddr[0]);
			if (axi_ctrl->share_ctrl->
					outpath.out3.capture_cnt == 1)
				axi_ctrl->share_ctrl->
				outpath.out3.capture_cnt = 0;

			vfe_send_outmsg(axi_ctrl,
				MSG_ID_OUTPUT_TERTIARY2, ch0_paddr,
				0, 0,
				axi_ctrl->share_ctrl->outpath.out3.inst_handle);
		} else {
			axi_ctrl->share_ctrl->outpath.out3.frame_drop_cnt++;
			pr_err("path_irq irq - no free buffer for rdi1!\n");
		}
	}
}
#endif

static void vfe32_process_output_path_irq_rdi2(
	struct axi_ctrl_t *axi_ctrl)
{
	uint32_t ping_pong;
	uint32_t ch0_paddr = 0;
	/* this must be rdi image output. */
	struct msm_free_buf *free_buf = NULL;
	/*RDI2*/
	CDBG("rdi2 out irq\n");
	if (axi_ctrl->share_ctrl->operation_mode & VFE_OUTPUTS_RDI2) {
		free_buf = vfe32_check_free_buffer(VFE_MSG_OUTPUT_IRQ,
			VFE_MSG_OUTPUT_TERTIARY3, axi_ctrl);
		if (free_buf) {
			ping_pong = msm_camera_io_r(axi_ctrl->
				share_ctrl->vfebase +
				VFE_BUS_PING_PONG_STATUS);

			/* Y channel */
			ch0_paddr = vfe32_get_ch_addr(ping_pong,
				axi_ctrl->share_ctrl->vfebase,
				axi_ctrl->share_ctrl->outpath.out4.ch0);
			CDBG("%s ch0 = 0x%x\n",
				__func__, free_buf->ch_paddr[0]);

			/* Y channel */
			vfe32_put_ch_addr(ping_pong,
				axi_ctrl->share_ctrl->vfebase,
				axi_ctrl->share_ctrl->outpath.out4.ch0,
				free_buf->ch_paddr[0]);

			vfe_send_outmsg(axi_ctrl,
				MSG_ID_OUTPUT_TERTIARY3, ch0_paddr,
				0, 0,
				axi_ctrl->share_ctrl->outpath.out4.inst_handle);
		} else {
			axi_ctrl->share_ctrl->outpath.out4.frame_drop_cnt++;
			pr_err("path_irq irq - no free buffer for rdi2!\n");
		}
	}
}
static void vfe32_process_output_path_irq_rdi0_and_rdi1(
	struct axi_ctrl_t *axi_ctrl)
{
	uint32_t ping_pong;
	uint32_t ch0_paddr = 0;
	uint32_t ch1_paddr = 0;
	struct msm_free_buf *free_buf = NULL;

	if (axi_ctrl->share_ctrl->operation_mode & VFE_OUTPUTS_RDI0) {
		free_buf = vfe32_check_free_buffer(VFE_MSG_OUTPUT_IRQ,
			VFE_MSG_OUTPUT_TERTIARY1, axi_ctrl);
		if (axi_ctrl->share_ctrl->outpath.out2.capture_cnt > 0 ||
							free_buf) {
			ping_pong = msm_camera_io_r(axi_ctrl->
				share_ctrl->vfebase +
				VFE_BUS_PING_PONG_STATUS);

			/* Y only channel */
			ch0_paddr = vfe32_get_ch_addr(ping_pong,
				axi_ctrl->share_ctrl->vfebase,
				axi_ctrl->share_ctrl->outpath.out2.ch0);

			ch1_paddr = vfe32_get_ch_addr(ping_pong,
				axi_ctrl->share_ctrl->vfebase,
				axi_ctrl->share_ctrl->outpath.out3.ch0);

			CDBG("%s ch0 = 0x%x, ch1 0x%x\n", __func__, ch0_paddr,
				ch1_paddr);

			if (free_buf) {
				CDBG("%s buffer address ch0 = 0x%x, ch1 0x%x\n",
					__func__,
					free_buf->ch_paddr[0],
					free_buf->ch_paddr[1]);

				vfe32_put_ch_addr(ping_pong,
					axi_ctrl->share_ctrl->vfebase,
					axi_ctrl->share_ctrl->outpath.out2.ch0,
					free_buf->ch_paddr[0]);

				vfe32_put_ch_addr(ping_pong,
					axi_ctrl->share_ctrl->vfebase,
					axi_ctrl->share_ctrl->outpath.out3.ch0,
					free_buf->ch_paddr[1]);
				}
			if (axi_ctrl->share_ctrl->outpath.out2.capture_cnt == 1)
				axi_ctrl->share_ctrl
				->outpath.out2.capture_cnt = 0;

			if (axi_ctrl->share_ctrl->outpath.out3.capture_cnt == 1)
				axi_ctrl->share_ctrl
				->outpath.out3.capture_cnt = 0;

			vfe_send_outmsg(axi_ctrl,
				MSG_ID_OUTPUT_TERTIARY1, ch0_paddr,
				0, 0,
				axi_ctrl->share_ctrl->outpath.out2.inst_handle);

		} else {
			axi_ctrl->share_ctrl->outpath.out2.frame_drop_cnt++;
			pr_err("path_irq_2 irq - no free buffer for rdi0!\n");
		}
	}

}

static uint32_t  vfe32_process_stats_irq_common(
	struct vfe32_ctrl_type *vfe32_ctrl,
	uint32_t statsNum, uint32_t newAddr)
{
	uint32_t pingpongStatus;
	uint32_t returnAddr;
	uint32_t pingpongAddr;

	/* must be 0=ping, 1=pong */
	pingpongStatus =
		((msm_camera_io_r(vfe32_ctrl->share_ctrl->vfebase +
		VFE_BUS_PING_PONG_STATUS))
	& ((uint32_t)(1<<(statsNum + 7)))) >> (statsNum + 7);
	/* stats bits starts at 7 */
	CDBG("statsNum %d, pingpongStatus %d\n", statsNum, pingpongStatus);
	pingpongAddr =
		((uint32_t)(vfe32_ctrl->share_ctrl->vfebase +
				VFE_BUS_STATS_PING_PONG_BASE)) +
				(3*statsNum)*4 + (1-pingpongStatus)*4;
	returnAddr = msm_camera_io_r((uint32_t *)pingpongAddr);
	msm_camera_io_w(newAddr, (uint32_t *)pingpongAddr);
	return returnAddr;
}

static void vfe_send_stats_msg(
	struct vfe32_ctrl_type *vfe32_ctrl,
	uint32_t bufAddress, uint32_t statsNum)
{
	int rc = 0;
	void *vaddr = NULL;
	/* fill message with right content. */
	/* @todo This is causing issues, need further investigate */
	/* spin_lock_irqsave(&ctrl->state_lock, flags); */
	struct isp_msg_stats msgStats;
	uint32_t stats_type;
	msgStats.frameCounter = vfe32_ctrl->share_ctrl->vfeFrameId;
	if (vfe32_ctrl->simultaneous_sof_stat)
		msgStats.frameCounter--;
	msgStats.buffer = bufAddress;
	switch (statsNum) {
	case statsAeNum:{
		msgStats.id =
			(!vfe32_use_bayer_stats(vfe32_ctrl)) ? MSG_ID_STATS_AEC
				: MSG_ID_STATS_BG;
		stats_type =
			(!vfe32_use_bayer_stats(vfe32_ctrl)) ?
				MSM_STATS_TYPE_AEC : MSM_STATS_TYPE_BG;
		rc = vfe32_ctrl->stats_ops.dispatch(
				vfe32_ctrl->stats_ops.stats_ctrl,
				stats_type, bufAddress,
				&msgStats.buf_idx, &vaddr, &msgStats.fd,
				vfe32_ctrl->stats_ops.client);
		}
		break;
	case statsAfNum:{
		msgStats.id =
			(!vfe32_use_bayer_stats(vfe32_ctrl)) ? MSG_ID_STATS_AF
				: MSG_ID_STATS_BF;
		stats_type =
			(!vfe32_use_bayer_stats(vfe32_ctrl)) ? MSM_STATS_TYPE_AF
				: MSM_STATS_TYPE_BF;
		rc = vfe32_ctrl->stats_ops.dispatch(
				vfe32_ctrl->stats_ops.stats_ctrl,
				stats_type, bufAddress,
				&msgStats.buf_idx, &vaddr, &msgStats.fd,
				vfe32_ctrl->stats_ops.client);
		}
		break;
	case statsAwbNum: {
		msgStats.id = MSG_ID_STATS_AWB;
		rc = vfe32_ctrl->stats_ops.dispatch(
				vfe32_ctrl->stats_ops.stats_ctrl,
				MSM_STATS_TYPE_AWB, bufAddress,
				&msgStats.buf_idx, &vaddr, &msgStats.fd,
				vfe32_ctrl->stats_ops.client);
		}
		break;

	case statsIhistNum: {
		msgStats.id = MSG_ID_STATS_IHIST;
		rc = vfe32_ctrl->stats_ops.dispatch(
				vfe32_ctrl->stats_ops.stats_ctrl,
				MSM_STATS_TYPE_IHIST, bufAddress,
				&msgStats.buf_idx, &vaddr, &msgStats.fd,
				vfe32_ctrl->stats_ops.client);
		}
		break;
	case statsRsNum: {
		msgStats.id = MSG_ID_STATS_RS;
		rc = vfe32_ctrl->stats_ops.dispatch(
				vfe32_ctrl->stats_ops.stats_ctrl,
				MSM_STATS_TYPE_RS, bufAddress,
				&msgStats.buf_idx, &vaddr, &msgStats.fd,
				vfe32_ctrl->stats_ops.client);
		}
		break;
	case statsCsNum: {
		msgStats.id = MSG_ID_STATS_CS;
		rc = vfe32_ctrl->stats_ops.dispatch(
				vfe32_ctrl->stats_ops.stats_ctrl,
				MSM_STATS_TYPE_CS, bufAddress,
				&msgStats.buf_idx, &vaddr, &msgStats.fd,
				vfe32_ctrl->stats_ops.client);
		}
		break;
	case statsSkinNum: {
		msgStats.id = MSG_ID_STATS_BHIST;
		rc = vfe32_ctrl->stats_ops.dispatch(
				vfe32_ctrl->stats_ops.stats_ctrl,
				MSM_STATS_TYPE_BHIST, bufAddress,
				&msgStats.buf_idx, &vaddr, &msgStats.fd,
				vfe32_ctrl->stats_ops.client);
		}
		break;

	default:
		goto stats_done;
	}
	if (rc == 0) {
		msgStats.buffer = (uint32_t)vaddr;
		v4l2_subdev_notify(&vfe32_ctrl->subdev,
			NOTIFY_VFE_MSG_STATS,
			&msgStats);
	} else {
		pr_err("%s: paddr to idx mapping error, stats_id = %d, paddr = 0x%d",
			 __func__, msgStats.id, msgStats.buffer);
	}
stats_done:
	/* spin_unlock_irqrestore(&ctrl->state_lock, flags); */
	return;
}

static void vfe_send_comp_stats_msg(
	struct vfe32_ctrl_type *vfe32_ctrl, uint32_t status_bits)
{
	struct msm_stats_buf msgStats;
	uint32_t temp;

	msgStats.frame_id = vfe32_ctrl->share_ctrl->vfeFrameId;
	if (vfe32_ctrl->simultaneous_sof_stat)
		msgStats.frame_id--;

	msgStats.status_bits = status_bits;

	msgStats.aec.buff = vfe32_ctrl->aecbgStatsControl.bufToRender;
	msgStats.awb.buff = vfe32_ctrl->awbStatsControl.bufToRender;
	msgStats.af.buff = vfe32_ctrl->afbfStatsControl.bufToRender;

	msgStats.ihist.buff = vfe32_ctrl->ihistStatsControl.bufToRender;
	msgStats.rs.buff = vfe32_ctrl->rsStatsControl.bufToRender;
	msgStats.cs.buff = vfe32_ctrl->csStatsControl.bufToRender;

	temp = msm_camera_io_r(
		vfe32_ctrl->share_ctrl->vfebase + VFE_STATS_AWB_SGW_CFG);
	msgStats.awb_ymin = (0xFF00 & temp) >> 8;

	v4l2_subdev_notify(&vfe32_ctrl->subdev,
				NOTIFY_VFE_MSG_COMP_STATS,
				&msgStats);
}

static void vfe32_process_stats_ae_bg_irq(struct vfe32_ctrl_type *vfe32_ctrl)
{
	unsigned long flags = 0;
	uint32_t addr;
	uint32_t stats_type;
	stats_type =
		(!vfe32_use_bayer_stats(vfe32_ctrl)) ? MSM_STATS_TYPE_AEC
			: MSM_STATS_TYPE_BG;
	spin_lock_irqsave(&vfe32_ctrl->stats_bufq_lock, flags);
	addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl, stats_type);
	spin_unlock_irqrestore(&vfe32_ctrl->stats_bufq_lock, flags);
	if (addr) {
		vfe32_ctrl->aecbgStatsControl.bufToRender =
			vfe32_process_stats_irq_common(vfe32_ctrl, statsAeNum,
			addr);

		vfe_send_stats_msg(vfe32_ctrl,
			vfe32_ctrl->aecbgStatsControl.bufToRender, statsAeNum);
	} else{
		vfe32_ctrl->aecbgStatsControl.droppedStatsFrameCount++;
		CDBG("%s: droppedStatsFrameCount = %d", __func__,
			vfe32_ctrl->aecbgStatsControl.droppedStatsFrameCount);
	}
}

static void vfe32_process_stats_awb_irq(struct vfe32_ctrl_type *vfe32_ctrl)
{
	unsigned long flags = 0;
	uint32_t addr;
	spin_lock_irqsave(&vfe32_ctrl->stats_bufq_lock, flags);
	addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl, MSM_STATS_TYPE_AWB);
	spin_unlock_irqrestore(&vfe32_ctrl->stats_bufq_lock, flags);
	if (addr) {
		vfe32_ctrl->awbStatsControl.bufToRender =
			vfe32_process_stats_irq_common(vfe32_ctrl, statsAwbNum,
			addr);

		vfe_send_stats_msg(vfe32_ctrl,
			vfe32_ctrl->awbStatsControl.bufToRender, statsAwbNum);
	} else{
		vfe32_ctrl->awbStatsControl.droppedStatsFrameCount++;
		CDBG("%s: droppedStatsFrameCount = %d", __func__,
			vfe32_ctrl->awbStatsControl.droppedStatsFrameCount);
	}
}

static void vfe32_process_stats_af_bf_irq(struct vfe32_ctrl_type *vfe32_ctrl)
{
	unsigned long flags = 0;
	uint32_t addr;
	uint32_t stats_type;
	stats_type =
		(!vfe32_use_bayer_stats(vfe32_ctrl)) ? MSM_STATS_TYPE_AF
			: MSM_STATS_TYPE_BF;
	spin_lock_irqsave(&vfe32_ctrl->stats_bufq_lock, flags);
	addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl, stats_type);
	spin_unlock_irqrestore(&vfe32_ctrl->stats_bufq_lock, flags);
	if (addr) {
		vfe32_ctrl->afbfStatsControl.bufToRender =
			vfe32_process_stats_irq_common(vfe32_ctrl, statsAfNum,
			addr);

		vfe_send_stats_msg(vfe32_ctrl,
			vfe32_ctrl->afbfStatsControl.bufToRender, statsAfNum);
	} else{
		vfe32_ctrl->afbfStatsControl.droppedStatsFrameCount++;
		CDBG("%s: droppedStatsFrameCount = %d", __func__,
			vfe32_ctrl->afbfStatsControl.droppedStatsFrameCount);
	}
}

static void vfe32_process_stats_bhist_irq(struct vfe32_ctrl_type *vfe32_ctrl)
{
	unsigned long flags = 0;
	uint32_t addr;
	spin_lock_irqsave(&vfe32_ctrl->stats_bufq_lock, flags);
	addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl, MSM_STATS_TYPE_BHIST);
	spin_unlock_irqrestore(&vfe32_ctrl->stats_bufq_lock, flags);
	if (addr) {
		vfe32_ctrl->bhistStatsControl.bufToRender =
			vfe32_process_stats_irq_common(vfe32_ctrl,
				statsSkinNum, addr);

		vfe_send_stats_msg(vfe32_ctrl,
			vfe32_ctrl->bhistStatsControl.bufToRender,
			statsSkinNum);
	} else{
		vfe32_ctrl->bhistStatsControl.droppedStatsFrameCount++;
		CDBG("%s: droppedStatsFrameCount = %d", __func__,
			vfe32_ctrl->bhistStatsControl.droppedStatsFrameCount);
	}
}

static void vfe32_process_stats_ihist_irq(struct vfe32_ctrl_type *vfe32_ctrl)
{
	unsigned long flags = 0;
	uint32_t addr;
	spin_lock_irqsave(&vfe32_ctrl->stats_bufq_lock, flags);
	addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl, MSM_STATS_TYPE_IHIST);
	spin_unlock_irqrestore(&vfe32_ctrl->stats_bufq_lock, flags);
	if (addr) {
		vfe32_ctrl->ihistStatsControl.bufToRender =
			vfe32_process_stats_irq_common(
			vfe32_ctrl, statsIhistNum, addr);

		vfe_send_stats_msg(vfe32_ctrl,
			vfe32_ctrl->ihistStatsControl.bufToRender,
			statsIhistNum);
	} else {
		vfe32_ctrl->ihistStatsControl.droppedStatsFrameCount++;
		CDBG("%s: droppedStatsFrameCount = %d", __func__,
			vfe32_ctrl->ihistStatsControl.droppedStatsFrameCount);
	}
}

static void vfe32_process_stats_rs_irq(struct vfe32_ctrl_type *vfe32_ctrl)
{
	unsigned long flags = 0;
	uint32_t addr;
	spin_lock_irqsave(&vfe32_ctrl->stats_bufq_lock, flags);
	addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl, MSM_STATS_TYPE_RS);
	spin_unlock_irqrestore(&vfe32_ctrl->stats_bufq_lock, flags);
	if (addr) {
		vfe32_ctrl->rsStatsControl.bufToRender =
			vfe32_process_stats_irq_common(vfe32_ctrl, statsRsNum,
			addr);

		vfe_send_stats_msg(vfe32_ctrl,
			vfe32_ctrl->rsStatsControl.bufToRender, statsRsNum);
	} else {
		vfe32_ctrl->rsStatsControl.droppedStatsFrameCount++;
		CDBG("%s: droppedStatsFrameCount = %d", __func__,
			vfe32_ctrl->rsStatsControl.droppedStatsFrameCount);
	}
}

static void vfe32_process_stats_cs_irq(struct vfe32_ctrl_type *vfe32_ctrl)
{
	unsigned long flags = 0;
	uint32_t addr;
	spin_lock_irqsave(&vfe32_ctrl->stats_bufq_lock, flags);
	addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl, MSM_STATS_TYPE_CS);
	spin_unlock_irqrestore(&vfe32_ctrl->stats_bufq_lock, flags);
	if (addr) {
		vfe32_ctrl->csStatsControl.bufToRender =
			vfe32_process_stats_irq_common(vfe32_ctrl, statsCsNum,
			addr);

			vfe_send_stats_msg(vfe32_ctrl,
				vfe32_ctrl->csStatsControl.bufToRender,
				statsCsNum);
	} else {
		vfe32_ctrl->csStatsControl.droppedStatsFrameCount++;
		CDBG("%s: droppedStatsFrameCount = %d", __func__,
			vfe32_ctrl->csStatsControl.droppedStatsFrameCount);
	}
}

static void vfe32_process_stats(struct vfe32_ctrl_type *vfe32_ctrl,
	uint32_t status_bits)
{
	unsigned long flags = 0;
	int32_t process_stats = false;
	uint32_t addr;
	uint32_t stats_type;

	CDBG("%s, stats = 0x%x\n", __func__, status_bits);
	spin_lock_irqsave(&vfe32_ctrl->stats_bufq_lock, flags);
	stats_type =
		(!vfe32_use_bayer_stats(vfe32_ctrl)) ? MSM_STATS_TYPE_AEC
			: MSM_STATS_TYPE_BG;

	if (status_bits & VFE_IRQ_STATUS0_STATS_AEC_BG) {
		addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl,
				stats_type);
		if (addr) {
			vfe32_ctrl->aecbgStatsControl.bufToRender =
				vfe32_process_stats_irq_common(
				vfe32_ctrl, statsAeNum,	addr);
			process_stats = true;
		} else{
			vfe32_ctrl->aecbgStatsControl.bufToRender = 0;
			vfe32_ctrl->aecbgStatsControl.droppedStatsFrameCount++;
		}
	} else {
		vfe32_ctrl->aecbgStatsControl.bufToRender = 0;
	}

	if (status_bits & VFE_IRQ_STATUS0_STATS_AWB) {
		addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl,
			MSM_STATS_TYPE_AWB);
		if (addr) {
			vfe32_ctrl->awbStatsControl.bufToRender =
				vfe32_process_stats_irq_common(
				vfe32_ctrl, statsAwbNum,
				addr);
			process_stats = true;
		} else{
			vfe32_ctrl->awbStatsControl.droppedStatsFrameCount++;
			vfe32_ctrl->awbStatsControl.bufToRender = 0;
		}
	} else {
		vfe32_ctrl->awbStatsControl.bufToRender = 0;
	}

	stats_type =
		(!vfe32_use_bayer_stats(vfe32_ctrl)) ? MSM_STATS_TYPE_AF
			: MSM_STATS_TYPE_BF;
	if (status_bits & VFE_IRQ_STATUS0_STATS_AF_BF) {
		addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl,
					stats_type);
		if (addr) {
			vfe32_ctrl->afbfStatsControl.bufToRender =
				vfe32_process_stats_irq_common(
				vfe32_ctrl, statsAfNum,
				addr);
			process_stats = true;
		} else {
			vfe32_ctrl->afbfStatsControl.bufToRender = 0;
			vfe32_ctrl->afbfStatsControl.droppedStatsFrameCount++;
		}
	} else {
		vfe32_ctrl->afbfStatsControl.bufToRender = 0;
	}

	if (status_bits & VFE_IRQ_STATUS0_STATS_IHIST) {
		addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl,
					MSM_STATS_TYPE_IHIST);
		if (addr) {
			vfe32_ctrl->ihistStatsControl.bufToRender =
				vfe32_process_stats_irq_common(
				vfe32_ctrl, statsIhistNum,
				addr);
			process_stats = true;
		} else {
			vfe32_ctrl->ihistStatsControl.droppedStatsFrameCount++;
			vfe32_ctrl->ihistStatsControl.bufToRender = 0;
		}
	} else {
		vfe32_ctrl->ihistStatsControl.bufToRender = 0;
	}

	if (status_bits & VFE_IRQ_STATUS0_STATS_RS) {
		addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl,
					MSM_STATS_TYPE_RS);
		if (addr) {
			vfe32_ctrl->rsStatsControl.bufToRender =
				vfe32_process_stats_irq_common(
				vfe32_ctrl, statsRsNum,
				addr);
			process_stats = true;
		} else {
			vfe32_ctrl->rsStatsControl.droppedStatsFrameCount++;
			vfe32_ctrl->rsStatsControl.bufToRender = 0;
		}
	} else {
		vfe32_ctrl->rsStatsControl.bufToRender = 0;
	}

	if (status_bits & VFE_IRQ_STATUS0_STATS_CS) {
		addr = (uint32_t)vfe32_stats_dqbuf(vfe32_ctrl,
					MSM_STATS_TYPE_CS);
		if (addr) {
			vfe32_ctrl->csStatsControl.bufToRender =
				vfe32_process_stats_irq_common(
				vfe32_ctrl, statsCsNum,
				addr);
			process_stats = true;
		} else {
			vfe32_ctrl->csStatsControl.droppedStatsFrameCount++;
			vfe32_ctrl->csStatsControl.bufToRender = 0;
		}
	} else {
		vfe32_ctrl->csStatsControl.bufToRender = 0;
	}
	spin_unlock_irqrestore(&vfe32_ctrl->stats_bufq_lock, flags);
	if (process_stats)
		vfe_send_comp_stats_msg(vfe32_ctrl, status_bits);

	return;
}

static void vfe32_process_stats_irq(
	struct vfe32_ctrl_type *vfe32_ctrl, uint32_t irqstatus)
{
	uint32_t status_bits = VFE_COM_STATUS & irqstatus;
	if ((vfe32_ctrl->hfr_mode != HFR_MODE_OFF) &&
		(vfe32_ctrl->share_ctrl->vfeFrameId %
		 vfe32_ctrl->hfr_mode != 0)) {
		CDBG("Skip the stats when HFR enabled\n");
		return;
	}

	vfe32_process_stats(vfe32_ctrl, status_bits);
	return;
}

static void vfe32_process_irq(
	struct vfe32_ctrl_type *vfe32_ctrl, uint32_t irqstatus)
{
	if (irqstatus &
		VFE_IRQ_STATUS0_STATS_COMPOSIT_MASK) {
		vfe32_process_stats_irq(vfe32_ctrl, irqstatus);
		return;
	}

	switch (irqstatus) {
	case VFE_IRQ_STATUS0_CAMIF_SOF_MASK:
		CDBG("irq	camifSofIrq\n");
		vfe32_process_camif_sof_irq(vfe32_ctrl);
		break;
	case VFE_IRQ_STATUS0_REG_UPDATE_MASK:
		CDBG("irq	regUpdateIrq\n");
		vfe32_process_reg_update_irq(vfe32_ctrl);
		break;
	case VFE_IRQ_STATUS1_RDI0_REG_UPDATE:
		CDBG("irq	rdi0 regUpdateIrq\n");
		vfe32_process_rdi0_reg_update_irq(vfe32_ctrl);
		break;
	case VFE_IRQ_STATUS1_RDI1_REG_UPDATE:
		CDBG("irq	rdi1 regUpdateIrq\n");
		vfe32_process_rdi1_reg_update_irq(vfe32_ctrl);
		break;
	case VFE_IRQ_STATUS1_RDI2_REG_UPDATE:
		pr_err("irq	rdi2 regUpdateIrq\n");
		vfe32_process_rdi2_reg_update_irq(vfe32_ctrl);
		break;
	case VFE_IMASK_WHILE_STOPPING_1:
		CDBG("irq	resetAckIrq\n");
		vfe32_process_reset_irq(vfe32_ctrl);
		break;
	case VFE_IRQ_STATUS0_STATS_AEC_BG:
		CDBG("Stats AEC irq occured.\n");
		vfe32_process_stats_ae_bg_irq(vfe32_ctrl);
		break;
	case VFE_IRQ_STATUS0_STATS_AWB:
		CDBG("Stats AWB irq occured.\n");
		vfe32_process_stats_awb_irq(vfe32_ctrl);
		break;
	case VFE_IRQ_STATUS0_STATS_AF_BF:
		CDBG("Stats AF irq occured.\n");
		vfe32_process_stats_af_bf_irq(vfe32_ctrl);
		break;
	case VFE_IRQ_STATUS0_STATS_SK_BHIST:
		CDBG("Stats BHIST irq occured.\n");
		vfe32_process_stats_bhist_irq(vfe32_ctrl);
		break;
	case VFE_IRQ_STATUS0_STATS_IHIST:
		CDBG("Stats IHIST irq occured.\n");
		vfe32_process_stats_ihist_irq(vfe32_ctrl);
		break;
	case VFE_IRQ_STATUS0_STATS_RS:
		CDBG("Stats RS irq occured.\n");
		vfe32_process_stats_rs_irq(vfe32_ctrl);
		break;
	case VFE_IRQ_STATUS0_STATS_CS:
		CDBG("Stats CS irq occured.\n");
		vfe32_process_stats_cs_irq(vfe32_ctrl);
		break;
	case VFE_IRQ_STATUS0_SYNC_TIMER0:
		CDBG("SYNC_TIMER 0 irq occured.\n");
		vfe32_send_isp_msg(&vfe32_ctrl->subdev,
			vfe32_ctrl->share_ctrl->vfeFrameId,
			MSG_ID_SYNC_TIMER0_DONE);
		break;
	case VFE_IRQ_STATUS0_SYNC_TIMER1:
		CDBG("SYNC_TIMER 1 irq occured.\n");
		vfe32_send_isp_msg(&vfe32_ctrl->subdev,
			vfe32_ctrl->share_ctrl->vfeFrameId,
			MSG_ID_SYNC_TIMER1_DONE);
		break;
	case VFE_IRQ_STATUS0_SYNC_TIMER2:
		CDBG("SYNC_TIMER 2 irq occured.\n");
		vfe32_send_isp_msg(&vfe32_ctrl->subdev,
			vfe32_ctrl->share_ctrl->vfeFrameId,
			MSG_ID_SYNC_TIMER2_DONE);
		break;
	default:
		pr_err("Invalid IRQ status\n");
	}
}

static void axi32_do_tasklet(unsigned long data)
{
	unsigned long flags = 0;
#if defined(CONFIG_MACH_JACTIVE_ATT) || defined(CONFIG_MACH_JACTIVE_EUR)
	uint8_t  axi_busy_flag = true;
	uint32_t halt_timeout = 100;
#endif
	struct axi_ctrl_t *axi_ctrl = (struct axi_ctrl_t *)data;
	struct vfe32_ctrl_type *vfe32_ctrl = axi_ctrl->share_ctrl->vfe32_ctrl;
	struct vfe32_isr_queue_cmd *qcmd = NULL;
	int stat_interrupt;

	CDBG("=== axi32_do_tasklet start ===\n");

	while (atomic_read(&irq_cnt)) {
		spin_lock_irqsave(&axi_ctrl->tasklet_lock, flags);
		qcmd = list_first_entry(&axi_ctrl->tasklet_q,
			struct vfe32_isr_queue_cmd, list);
		atomic_sub(1, &irq_cnt);

		if (!qcmd) {
			spin_unlock_irqrestore(&axi_ctrl->tasklet_lock,
				flags);
			return;
		}

		list_del(&qcmd->list);
		spin_unlock_irqrestore(&axi_ctrl->tasklet_lock,
			flags);

		if (axi_ctrl->share_ctrl->stats_comp) {
			stat_interrupt = (qcmd->vfeInterruptStatus0 &
					VFE_IRQ_STATUS0_STATS_COMPOSIT_MASK);
		} else {
			stat_interrupt =
				(qcmd->vfeInterruptStatus0 &
					VFE_IRQ_STATUS0_STATS_AEC_BG) |
				(qcmd->vfeInterruptStatus0 &
					VFE_IRQ_STATUS0_STATS_AWB) |
				(qcmd->vfeInterruptStatus0 &
					VFE_IRQ_STATUS0_STATS_AF_BF) |
				(qcmd->vfeInterruptStatus0 &
					VFE_IRQ_STATUS0_STATS_IHIST) |
				(qcmd->vfeInterruptStatus0 &
					VFE_IRQ_STATUS0_STATS_RS) |
				(qcmd->vfeInterruptStatus0 &
					VFE_IRQ_STATUS0_STATS_CS);
		}

#if defined(CONFIG_MACH_JACTIVE_ATT) || defined(CONFIG_MACH_JACTIVE_EUR)
		if (!atomic_read(&recovery_active)) {
			if (qcmd->vfeInterruptStatus0 &
					VFE_IRQ_STATUS0_CAMIF_SOF_MASK) {
				if (stat_interrupt)
					vfe32_ctrl->simultaneous_sof_stat = 1;
				v4l2_subdev_notify(&vfe32_ctrl->subdev,
						NOTIFY_VFE_IRQ,
						(void *)VFE_IRQ_STATUS0_CAMIF_SOF_MASK);
			}

			/* interrupt to be processed,  *qcmd has the payload.  */
			if (qcmd->vfeInterruptStatus0 &
					VFE_IRQ_STATUS0_REG_UPDATE_MASK)
				v4l2_subdev_notify(&vfe32_ctrl->subdev,
						NOTIFY_VFE_IRQ,
						(void *)VFE_IRQ_STATUS0_REG_UPDATE_MASK);

			if (qcmd->vfeInterruptStatus1 &
					VFE_IRQ_STATUS1_RDI0_REG_UPDATE_MASK)
				v4l2_subdev_notify(&vfe32_ctrl->subdev,
						NOTIFY_VFE_IRQ,
						(void *)VFE_IRQ_STATUS1_RDI0_REG_UPDATE);

			if (qcmd->vfeInterruptStatus1 &
					VFE_IRQ_STATUS1_RDI1_REG_UPDATE_MASK)
				v4l2_subdev_notify(&vfe32_ctrl->subdev,
						NOTIFY_VFE_IRQ,
						(void *)VFE_IRQ_STATUS1_RDI1_REG_UPDATE);

			if (qcmd->vfeInterruptStatus1 &
					VFE_IRQ_STATUS1_RDI2_REG_UPDATE_MASK)
				v4l2_subdev_notify(&vfe32_ctrl->subdev,
						NOTIFY_VFE_IRQ,
						(void *)VFE_IRQ_STATUS1_RDI2_REG_UPDATE);
		}

		if ((qcmd->vfeInterruptStatus1 &
					VFE_IMASK_WHILE_STOPPING_1) &&
				atomic_read(&recovery_active) != 2)
			v4l2_subdev_notify(&vfe32_ctrl->subdev,
					NOTIFY_VFE_IRQ,
					(void *)VFE_IMASK_WHILE_STOPPING_1);

		if (atomic_read(&axi_ctrl->share_ctrl->handle_common_irq)) {
			if ((qcmd->vfeInterruptStatus1 &
						VFE32_IMASK_COMMON_ERROR_ONLY_1) &&
					atomic_read(&recovery_active) != 1) {
				pr_err("irq	errorIrq\n");
				vfe32_process_common_error_irq(
						axi_ctrl,
						qcmd->vfeInterruptStatus1 &
						VFE32_IMASK_COMMON_ERROR_ONLY_1);
			}

			if ((qcmd->vfeInterruptStatus1 & 0x3FFF00) && atomic_read(&recovery_active) == 2) {
				while (axi_busy_flag && halt_timeout--) {
					if (msm_camera_io_r(axi_ctrl->share_ctrl->vfebase + VFE_AXI_STATUS) & 0x1)
						axi_busy_flag = false;
				}
				msm_camera_io_w_mb(AXI_HALT_CLEAR, axi_ctrl->share_ctrl->vfebase + VFE_AXI_CMD);
				printk("Halt done\n");
				msm_camera_io_w(0x000003EF, axi_ctrl->share_ctrl->vfebase + 0x4);
				atomic_set(&recovery_active, 1);
			}

			if(!atomic_read(&recovery_active))
				v4l2_subdev_notify(&axi_ctrl->subdev,
						NOTIFY_AXI_IRQ,
						(void *)qcmd->vfeInterruptStatus0);
		}

		if (atomic_read(&axi_ctrl->share_ctrl->vstate) && !atomic_read(&recovery_active)) {
			if (qcmd->vfeInterruptStatus1 &
					VFE32_IMASK_VFE_ERROR_ONLY_1) {
				pr_err("irq	errorIrq\n");
				vfe32_process_error_irq(
						axi_ctrl,
						qcmd->vfeInterruptStatus1 &
						VFE32_IMASK_VFE_ERROR_ONLY_1);
			}

			/* then process stats irq. */
			if (axi_ctrl->share_ctrl->stats_comp) {
				/* process stats comb interrupt. */
				if (qcmd->vfeInterruptStatus0 &
						VFE_IRQ_STATUS0_STATS_COMPOSIT_MASK) {
					CDBG("Stats composite irq occured.\n");
					v4l2_subdev_notify(&vfe32_ctrl->subdev,
							NOTIFY_VFE_IRQ,
							(void *)qcmd->vfeInterruptStatus0);
				}
			} else {
				/* process individual stats interrupt. */
				if (qcmd->vfeInterruptStatus0 &
						VFE_IRQ_STATUS0_STATS_AEC_BG)
					v4l2_subdev_notify(&vfe32_ctrl->subdev,
							NOTIFY_VFE_IRQ,
							(void *)VFE_IRQ_STATUS0_STATS_AEC_BG);

				if (qcmd->vfeInterruptStatus0 &
						VFE_IRQ_STATUS0_STATS_AWB)
					v4l2_subdev_notify(&vfe32_ctrl->subdev,
							NOTIFY_VFE_IRQ,
							(void *)VFE_IRQ_STATUS0_STATS_AWB);

				if (qcmd->vfeInterruptStatus0 &
						VFE_IRQ_STATUS0_STATS_AF_BF)
					v4l2_subdev_notify(&vfe32_ctrl->subdev,
							NOTIFY_VFE_IRQ,
							(void *)VFE_IRQ_STATUS0_STATS_AF_BF);
				if (qcmd->vfeInterruptStatus0 &
						VFE_IRQ_STATUS0_STATS_SK_BHIST)
					v4l2_subdev_notify(&vfe32_ctrl->subdev,
							NOTIFY_VFE_IRQ,
							(void *)VFE_IRQ_STATUS0_STATS_SK_BHIST);

				if (qcmd->vfeInterruptStatus0 &
						VFE_IRQ_STATUS0_STATS_IHIST)
					v4l2_subdev_notify(&vfe32_ctrl->subdev,
							NOTIFY_VFE_IRQ,
							(void *)VFE_IRQ_STATUS0_STATS_IHIST);

				if (qcmd->vfeInterruptStatus0 &
						VFE_IRQ_STATUS0_STATS_RS)
					v4l2_subdev_notify(&vfe32_ctrl->subdev,
							NOTIFY_VFE_IRQ,
							(void *)VFE_IRQ_STATUS0_STATS_RS);

				if (qcmd->vfeInterruptStatus0 &
						VFE_IRQ_STATUS0_STATS_CS)
					v4l2_subdev_notify(&vfe32_ctrl->subdev,
							NOTIFY_VFE_IRQ,
							(void *)VFE_IRQ_STATUS0_STATS_CS);

				if (qcmd->vfeInterruptStatus0 &
						VFE_IRQ_STATUS0_SYNC_TIMER0)
					v4l2_subdev_notify(&vfe32_ctrl->subdev,
							NOTIFY_VFE_IRQ,
							(void *)VFE_IRQ_STATUS0_SYNC_TIMER0);

				if (qcmd->vfeInterruptStatus0 &
						VFE_IRQ_STATUS0_SYNC_TIMER1)
					v4l2_subdev_notify(&vfe32_ctrl->subdev,
							NOTIFY_VFE_IRQ,
							(void *)VFE_IRQ_STATUS0_SYNC_TIMER1);

				if (qcmd->vfeInterruptStatus0 &
						VFE_IRQ_STATUS0_SYNC_TIMER2)
					v4l2_subdev_notify(&vfe32_ctrl->subdev,
							NOTIFY_VFE_IRQ,
							(void *)VFE_IRQ_STATUS0_SYNC_TIMER2);
			}
		}
#else
		if (qcmd->vfeInterruptStatus0 &
				VFE_IRQ_STATUS0_CAMIF_SOF_MASK) {
			if (stat_interrupt)
				vfe32_ctrl->simultaneous_sof_stat = 1;
			v4l2_subdev_notify(&vfe32_ctrl->subdev,
				NOTIFY_VFE_IRQ,
				(void *)VFE_IRQ_STATUS0_CAMIF_SOF_MASK);
		}

		/* interrupt to be processed,  *qcmd has the payload.  */
		if (qcmd->vfeInterruptStatus0 &
				VFE_IRQ_STATUS0_REG_UPDATE_MASK)
			v4l2_subdev_notify(&vfe32_ctrl->subdev,
				NOTIFY_VFE_IRQ,
				(void *)VFE_IRQ_STATUS0_REG_UPDATE_MASK);

		if (qcmd->vfeInterruptStatus1 &
				VFE_IRQ_STATUS1_RDI0_REG_UPDATE_MASK)
			v4l2_subdev_notify(&vfe32_ctrl->subdev,
				NOTIFY_VFE_IRQ,
				(void *)VFE_IRQ_STATUS1_RDI0_REG_UPDATE);

		if (qcmd->vfeInterruptStatus1 &
				VFE_IRQ_STATUS1_RDI1_REG_UPDATE_MASK)
			v4l2_subdev_notify(&vfe32_ctrl->subdev,
				NOTIFY_VFE_IRQ,
				(void *)VFE_IRQ_STATUS1_RDI1_REG_UPDATE);

		if (qcmd->vfeInterruptStatus1 &
				VFE_IRQ_STATUS1_RDI2_REG_UPDATE_MASK)
			v4l2_subdev_notify(&vfe32_ctrl->subdev,
				NOTIFY_VFE_IRQ,
				(void *)VFE_IRQ_STATUS1_RDI2_REG_UPDATE);

		if (qcmd->vfeInterruptStatus1 &
				VFE_IMASK_WHILE_STOPPING_1)
			v4l2_subdev_notify(&vfe32_ctrl->subdev,
				NOTIFY_VFE_IRQ,
				(void *)VFE_IMASK_WHILE_STOPPING_1);

		if (atomic_read(&axi_ctrl->share_ctrl->handle_common_irq)) {
			if (qcmd->vfeInterruptStatus1 &
					VFE32_IMASK_COMMON_ERROR_ONLY_1) {
				pr_err("irq	errorIrq\n");
				vfe32_process_common_error_irq(
					axi_ctrl,
					qcmd->vfeInterruptStatus1 &
					VFE32_IMASK_COMMON_ERROR_ONLY_1);
			}

			v4l2_subdev_notify(&axi_ctrl->subdev,
				NOTIFY_AXI_IRQ,
				(void *)qcmd->vfeInterruptStatus0);
		}

		if (atomic_read(&axi_ctrl->share_ctrl->vstate)) {
			if (qcmd->vfeInterruptStatus1 &
					VFE32_IMASK_VFE_ERROR_ONLY_1) {
				pr_err("irq	errorIrq\n");
				vfe32_process_error_irq(
					axi_ctrl,
					qcmd->vfeInterruptStatus1 &
					VFE32_IMASK_VFE_ERROR_ONLY_1);
			}

			/* then process stats irq. */
			if (axi_ctrl->share_ctrl->stats_comp) {
				/* process stats comb interrupt. */
				if (qcmd->vfeInterruptStatus0 &
					VFE_IRQ_STATUS0_STATS_COMPOSIT_MASK) {
					CDBG("Stats composite irq occured.\n");
					v4l2_subdev_notify(&vfe32_ctrl->subdev,
					NOTIFY_VFE_IRQ,
					(void *)qcmd->vfeInterruptStatus0);
				}
			} else {
				/* process individual stats interrupt. */
				if (qcmd->vfeInterruptStatus0 &
						VFE_IRQ_STATUS0_STATS_AEC_BG)
					v4l2_subdev_notify(&vfe32_ctrl->subdev,
					NOTIFY_VFE_IRQ,
					(void *)VFE_IRQ_STATUS0_STATS_AEC_BG);

				if (qcmd->vfeInterruptStatus0 &
						VFE_IRQ_STATUS0_STATS_AWB)
					v4l2_subdev_notify(&vfe32_ctrl->subdev,
					NOTIFY_VFE_IRQ,
					(void *)VFE_IRQ_STATUS0_STATS_AWB);

				if (qcmd->vfeInterruptStatus0 &
						VFE_IRQ_STATUS0_STATS_AF_BF)
					v4l2_subdev_notify(&vfe32_ctrl->subdev,
					NOTIFY_VFE_IRQ,
					(void *)VFE_IRQ_STATUS0_STATS_AF_BF);
				if (qcmd->vfeInterruptStatus0 &
						VFE_IRQ_STATUS0_STATS_SK_BHIST)
					v4l2_subdev_notify(&vfe32_ctrl->subdev,
					NOTIFY_VFE_IRQ,
					(void *)VFE_IRQ_STATUS0_STATS_SK_BHIST);

				if (qcmd->vfeInterruptStatus0 &
						VFE_IRQ_STATUS0_STATS_IHIST)
					v4l2_subdev_notify(&vfe32_ctrl->subdev,
					NOTIFY_VFE_IRQ,
					(void *)VFE_IRQ_STATUS0_STATS_IHIST);

				if (qcmd->vfeInterruptStatus0 &
						VFE_IRQ_STATUS0_STATS_RS)
					v4l2_subdev_notify(&vfe32_ctrl->subdev,
					NOTIFY_VFE_IRQ,
					(void *)VFE_IRQ_STATUS0_STATS_RS);

				if (qcmd->vfeInterruptStatus0 &
						VFE_IRQ_STATUS0_STATS_CS)
					v4l2_subdev_notify(&vfe32_ctrl->subdev,
					NOTIFY_VFE_IRQ,
					(void *)VFE_IRQ_STATUS0_STATS_CS);

				if (qcmd->vfeInterruptStatus0 &
						VFE_IRQ_STATUS0_SYNC_TIMER0)
					v4l2_subdev_notify(&vfe32_ctrl->subdev,
					NOTIFY_VFE_IRQ,
					(void *)VFE_IRQ_STATUS0_SYNC_TIMER0);

				if (qcmd->vfeInterruptStatus0 &
						VFE_IRQ_STATUS0_SYNC_TIMER1)
					v4l2_subdev_notify(&vfe32_ctrl->subdev,
					NOTIFY_VFE_IRQ,
					(void *)VFE_IRQ_STATUS0_SYNC_TIMER1);

				if (qcmd->vfeInterruptStatus0 &
						VFE_IRQ_STATUS0_SYNC_TIMER2)
					v4l2_subdev_notify(&vfe32_ctrl->subdev,
					NOTIFY_VFE_IRQ,
					(void *)VFE_IRQ_STATUS0_SYNC_TIMER2);
			}
		}
#endif
		vfe32_ctrl->simultaneous_sof_stat = 0;
		kfree(qcmd);
	}
	CDBG("=== axi32_do_tasklet end ===\n");
}

static irqreturn_t vfe32_parse_irq(int irq_num, void *data)
{
	unsigned long flags = 0;
	struct vfe32_irq_status irq;
	struct vfe32_isr_queue_cmd *qcmd;
	struct axi_ctrl_t *axi_ctrl = data;

	CDBG("vfe_parse_irq\n");
	if (!axi_ctrl->share_ctrl->vfebase)
		return IRQ_HANDLED;
	vfe32_read_irq_status(axi_ctrl, &irq);

	if ((irq.vfeIrqStatus0 == 0) && (irq.vfeIrqStatus1 == 0)) {
		CDBG("vfe_parse_irq: vfeIrqStatus0 & 1 are both 0!\n");
		return IRQ_HANDLED;
	}

	qcmd = kzalloc(sizeof(struct vfe32_isr_queue_cmd),
		GFP_ATOMIC);
	if (!qcmd) {
		pr_err("vfe_parse_irq: qcmd malloc failed!\n");
		return IRQ_HANDLED;
	}

	spin_lock_irqsave(&axi_ctrl->share_ctrl->stop_flag_lock, flags);
#if defined(CONFIG_MACH_JACTIVE_ATT) || defined(CONFIG_MACH_JACTIVE_EUR)
	if (axi_ctrl->share_ctrl->stop_ack_pending || atomic_read(&recovery_active)) {
		irq.vfeIrqStatus0 &= VFE_IMASK_WHILE_STOPPING_0;
		irq.vfeIrqStatus1 &= VFE_IMASK_WHILE_STOPPING_1;
	}
#else
	if (axi_ctrl->share_ctrl->stop_ack_pending) {
		irq.vfeIrqStatus0 &= VFE_IMASK_WHILE_STOPPING_0;
		irq.vfeIrqStatus1 &= VFE_IMASK_WHILE_STOPPING_1;
	}
#endif
	spin_unlock_irqrestore(&axi_ctrl->share_ctrl->stop_flag_lock, flags);

	CDBG("vfe_parse_irq: Irq_status0 = 0x%x, Irq_status1 = 0x%x.\n",
		irq.vfeIrqStatus0, irq.vfeIrqStatus1);

	qcmd->vfeInterruptStatus0 = irq.vfeIrqStatus0;
	qcmd->vfeInterruptStatus1 = irq.vfeIrqStatus1;

#if defined(CONFIG_MACH_JACTIVE_ATT) || defined(CONFIG_MACH_JACTIVE_EUR)
	if ((qcmd->vfeInterruptStatus1 & 0x3FFF00) && !atomic_read(&recovery_active)) {
		printk("Start recovery\n");
		pr_err("axi recovery_active(%d)\n", atomic_read(&recovery_active));
		recover_irq_mask0 = msm_camera_io_r(axi_ctrl->share_ctrl->vfebase + VFE_IRQ_MASK_0);
		recover_irq_mask1 = msm_camera_io_r(axi_ctrl->share_ctrl->vfebase + VFE_IRQ_MASK_1);
		msm_camera_io_w(0x0, axi_ctrl->share_ctrl->vfebase + VFE_IRQ_MASK_0);
		msm_camera_io_w((0x1 << 23), axi_ctrl->share_ctrl->vfebase + VFE_IRQ_MASK_1);
		msm_camera_io_w(VFE_CLEAR_ALL_IRQS, axi_ctrl->share_ctrl->vfebase + VFE_IRQ_CLEAR_0);
		msm_camera_io_w(VFE_CLEAR_ALL_IRQS, axi_ctrl->share_ctrl->vfebase + VFE_IRQ_CLEAR_1);
		msm_camera_io_w(0x2, axi_ctrl->share_ctrl->vfebase + 0x1E0);
		msm_camera_io_w(AXI_HALT, axi_ctrl->share_ctrl->vfebase + VFE_AXI_CMD);
		wmb();
		atomic_set(&recovery_active, 2);
	}
#endif

	spin_lock_irqsave(&axi_ctrl->tasklet_lock, flags);
	list_add_tail(&qcmd->list, &axi_ctrl->tasklet_q);

	atomic_add(1, &irq_cnt);
	spin_unlock_irqrestore(&axi_ctrl->tasklet_lock, flags);
	tasklet_schedule(&axi_ctrl->vfe32_tasklet);
	return IRQ_HANDLED;
}

int msm_axi_subdev_isr_routine(struct v4l2_subdev *sd,
	u32 status, bool *handled)
{
	struct axi_ctrl_t *axi_ctrl = v4l2_get_subdevdata(sd);
	irqreturn_t ret;
	pr_info("%s E ", __func__);
	ret = vfe32_parse_irq(axi_ctrl->vfeirq->start, axi_ctrl);
	*handled = TRUE;
	return 0;
}

static long vfe_stats_bufq_sub_ioctl(
	struct vfe32_ctrl_type *vfe_ctrl,
	struct msm_vfe_cfg_cmd *cmd, void *ion_client, int domain_num)
{
	long rc = 0;
	switch (cmd->cmd_type) {
	case VFE_CMD_STATS_REQBUF:
	if (!vfe_ctrl->stats_ops.stats_ctrl) {
		/* stats_ctrl has not been init yet */
		rc = msm_stats_buf_ops_init(&vfe_ctrl->stats_ctrl,
				(struct ion_client *)ion_client,
				&vfe_ctrl->stats_ops);
		if (rc < 0) {
			pr_err("%s: cannot init stats ops", __func__);
			goto end;
		}
		rc = vfe_ctrl->stats_ops.stats_ctrl_init(&vfe_ctrl->stats_ctrl);
		if (rc < 0) {
			pr_err("%s: cannot init stats_ctrl ops", __func__);
			memset(&vfe_ctrl->stats_ops, 0,
				sizeof(vfe_ctrl->stats_ops));
			goto end;
		}
		if (sizeof(struct msm_stats_reqbuf) != cmd->length) {
			/* error. the length not match */
			pr_err("%s: stats reqbuf input size = %d,\n"
				"struct size = %d, mitch match\n",
				 __func__, cmd->length,
				sizeof(struct msm_stats_reqbuf));
			rc = -EINVAL ;
			goto end;
		}
	}
	rc = vfe_ctrl->stats_ops.reqbuf(
			&vfe_ctrl->stats_ctrl,
			(struct msm_stats_reqbuf *)cmd->value,
			vfe_ctrl->stats_ops.client);
	break;
	case VFE_CMD_STATS_ENQUEUEBUF:
	if (sizeof(struct msm_stats_buf_info) != cmd->length) {
		/* error. the length not match */
		pr_err("%s: stats enqueuebuf input size = %d,\n"
			"struct size = %d, mitch match\n",
			 __func__, cmd->length,
			sizeof(struct msm_stats_buf_info));
			rc = -EINVAL;
			goto end;
	}
	rc = vfe_ctrl->stats_ops.enqueue_buf(
			&vfe_ctrl->stats_ctrl,
			(struct msm_stats_buf_info *)cmd->value,
			vfe_ctrl->stats_ops.client, domain_num);
	break;
	case VFE_CMD_STATS_FLUSH_BUFQ:
	{
		struct msm_stats_flush_bufq *flush_req = NULL;
		flush_req = (struct msm_stats_flush_bufq *)cmd->value;
		if (sizeof(struct msm_stats_flush_bufq) != cmd->length) {
			/* error. the length not match */
			pr_err("%s: stats flush queue input size = %d,\n"
				"struct size = %d, mitch match\n",
				__func__, cmd->length,
				sizeof(struct msm_stats_flush_bufq));
			rc = -EINVAL;
			goto end;
	}
	rc = vfe_ctrl->stats_ops.bufq_flush(
			&vfe_ctrl->stats_ctrl,
			(enum msm_stats_enum_type)flush_req->stats_type,
			vfe_ctrl->stats_ops.client);
	}
	break;
	case VFE_CMD_STATS_UNREGBUF:
	{
		struct msm_stats_reqbuf *req_buf = NULL;
		req_buf = (struct msm_stats_reqbuf *)cmd->value;
		if (sizeof(struct msm_stats_reqbuf) != cmd->length) {
			/* error. the length not match */
			pr_err("%s: stats reqbuf input size = %d,\n"
				"struct size = %d, mitch match\n",
				 __func__, cmd->length,
				sizeof(struct msm_stats_reqbuf));
			rc = -EINVAL ;
			goto end;
		}
		rc = vfe32_stats_unregbuf(vfe_ctrl, req_buf, domain_num);
	}
	break;
	default:
		rc = -1;
		pr_err("%s: cmd_type %d not supported", __func__,
			cmd->cmd_type);
	break;
	}
end:
	return rc;
}

static long msm_vfe_subdev_ioctl(struct v4l2_subdev *sd,
			unsigned int subdev_cmd, void *arg)
{
	struct msm_cam_media_controller *pmctl =
		(struct msm_cam_media_controller *)v4l2_get_subdev_hostdata(sd);
	struct vfe32_ctrl_type *vfe32_ctrl =
		(struct vfe32_ctrl_type *)v4l2_get_subdevdata(sd);
	struct msm_isp_cmd vfecmd;
	struct msm_camvfe_params *vfe_params;
	struct msm_vfe_cfg_cmd *cmd;
	void *data;

	long rc = 0;
	struct vfe_cmd_stats_buf *scfg = NULL;
	struct vfe_cmd_stats_ack *sack = NULL;

	if (!vfe32_ctrl->share_ctrl->vfebase) {
		pr_err("%s: base address unmapped\n", __func__);
		return -EFAULT;
	}

	CDBG("%s\n", __func__);
	if (subdev_cmd == VIDIOC_MSM_VFE_INIT) {
		CDBG("%s init\n", __func__);
		return msm_vfe_subdev_init(sd);
	} else if (subdev_cmd == VIDIOC_MSM_VFE_RELEASE) {
		msm_vfe_subdev_release(sd);
		return 0;
    }
#if defined(CONFIG_MACH_JACTIVE_ATT) || defined(CONFIG_MACH_JACTIVE_EUR)
    else if (subdev_cmd == VIDIOC_MSM_VFE_STATS_VERSION) {
		vfe32_ctrl->ver_num.main = *(uint32_t *)arg;
		return 0;
	}
#endif
	vfe_params = (struct msm_camvfe_params *)arg;
	cmd = vfe_params->vfe_cfg;
	data = vfe_params->data;
	switch (cmd->cmd_type) {
	case CMD_VFE_PROCESS_IRQ:
		vfe32_process_irq(vfe32_ctrl, (uint32_t) data);
		return rc;
	case VFE_CMD_STATS_REQBUF:
	case VFE_CMD_STATS_ENQUEUEBUF:
	case VFE_CMD_STATS_FLUSH_BUFQ:
	case VFE_CMD_STATS_UNREGBUF:
		/* for easy porting put in one envelope */
		rc = vfe_stats_bufq_sub_ioctl(vfe32_ctrl,
				cmd, vfe_params->data, pmctl->domain_num);
		return rc;
	default:
		if (cmd->cmd_type != CMD_CONFIG_PING_ADDR &&
		cmd->cmd_type != CMD_CONFIG_PONG_ADDR &&
		cmd->cmd_type != CMD_CONFIG_FREE_BUF_ADDR &&
		cmd->cmd_type != CMD_STATS_AEC_BUF_RELEASE &&
		cmd->cmd_type != CMD_STATS_AWB_BUF_RELEASE &&
		cmd->cmd_type != CMD_STATS_IHIST_BUF_RELEASE &&
		cmd->cmd_type != CMD_STATS_RS_BUF_RELEASE &&
		cmd->cmd_type != CMD_STATS_CS_BUF_RELEASE &&
		cmd->cmd_type != CMD_STATS_AF_BUF_RELEASE &&
		cmd->cmd_type != CMD_STATS_BG_BUF_RELEASE &&
		cmd->cmd_type != CMD_STATS_BF_BUF_RELEASE &&
		cmd->cmd_type != CMD_STATS_BHIST_BUF_RELEASE &&
		cmd->cmd_type != CMD_VFE_PIX_SOF_COUNT_UPDATE &&
		cmd->cmd_type != CMD_VFE_COUNT_PIX_SOF_ENABLE) {
			if (copy_from_user(&vfecmd,
					(void __user *)(cmd->value),
					sizeof(vfecmd))) {
				pr_err("%s %d: copy_from_user failed\n",
					__func__, __LINE__);
				return -EFAULT;
			}
		} else {
			/* here eith stats release or frame release. */
			if (cmd->cmd_type != CMD_CONFIG_PING_ADDR &&
				cmd->cmd_type != CMD_CONFIG_PONG_ADDR &&
				cmd->cmd_type != CMD_CONFIG_FREE_BUF_ADDR) {
				/* then must be stats release. */
				if (!data) {
					pr_err("%s: data = NULL, cmd->cmd_type = %d",
						__func__, cmd->cmd_type);
					return -EFAULT;
				}
				sack = kmalloc(sizeof(struct vfe_cmd_stats_ack),
							GFP_ATOMIC);
				if (!sack) {
					pr_err("%s: no mem for cmd->cmd_type = %d",
					 __func__, cmd->cmd_type);
					return -ENOMEM;
				}
				sack->nextStatsBuf = *(uint32_t *)data;
			}
		}
	}

	CDBG("%s: cmdType = %d\n", __func__, cmd->cmd_type);

	if ((cmd->cmd_type == CMD_STATS_AF_ENABLE)    ||
		(cmd->cmd_type == CMD_STATS_AWB_ENABLE)   ||
		(cmd->cmd_type == CMD_STATS_IHIST_ENABLE) ||
		(cmd->cmd_type == CMD_STATS_RS_ENABLE)    ||
		(cmd->cmd_type == CMD_STATS_CS_ENABLE)    ||
		(cmd->cmd_type == CMD_STATS_AEC_ENABLE)   ||
		(cmd->cmd_type == CMD_STATS_BG_ENABLE)    ||
		(cmd->cmd_type == CMD_STATS_BF_ENABLE)    ||
		(cmd->cmd_type == CMD_STATS_BHIST_ENABLE)) {
		struct axidata *axid;
		axid = data;
		if (!axid) {
			rc = -EFAULT;
			goto vfe32_config_done;
		}
		CDBG("%s: cmdType = %d\n", __func__, cmd->cmd_type);

		if ((cmd->cmd_type == CMD_STATS_AF_ENABLE)    ||
			(cmd->cmd_type == CMD_STATS_AWB_ENABLE)   ||
			(cmd->cmd_type == CMD_STATS_IHIST_ENABLE) ||
			(cmd->cmd_type == CMD_STATS_RS_ENABLE)    ||
			(cmd->cmd_type == CMD_STATS_CS_ENABLE)    ||
			(cmd->cmd_type == CMD_STATS_AEC_ENABLE)) {
				scfg = NULL;
				/* individual */
				goto vfe32_config_done;
		}
		switch (cmd->cmd_type) {
		case CMD_STATS_AEC_ENABLE:
		case CMD_STATS_BG_ENABLE:
		case CMD_STATS_BF_ENABLE:
		case CMD_STATS_BHIST_ENABLE:
		case CMD_STATS_AWB_ENABLE:
		case CMD_STATS_IHIST_ENABLE:
		case CMD_STATS_RS_ENABLE:
		case CMD_STATS_CS_ENABLE:
		default:
			pr_err("%s Unsupported cmd type %d",
				__func__, cmd->cmd_type);
			break;
		}
		goto vfe32_config_done;
	}
	switch (cmd->cmd_type) {
	case CMD_GENERAL:
		rc = vfe32_proc_general(pmctl, &vfecmd, vfe32_ctrl);
	break;
	case CMD_VFE_COUNT_PIX_SOF_ENABLE: {
		int enable = *((int *)cmd->value);
		if (enable)
			vfe32_ctrl->vfe_sof_count_enable = TRUE;
		else
			vfe32_ctrl->vfe_sof_count_enable = false;
	}
	break;
	case CMD_VFE_PIX_SOF_COUNT_UPDATE:
		if (!vfe32_ctrl->vfe_sof_count_enable)
			vfe32_ctrl->share_ctrl->vfeFrameId =
			*((uint32_t *)vfe_params->data);
	break;
	case CMD_CONFIG_PING_ADDR: {
		int path = *((int *)cmd->value);
		struct vfe32_output_ch *outch =
			vfe32_get_ch(path, vfe32_ctrl->share_ctrl);
		outch->ping = *((struct msm_free_buf *)data);
	}
	break;

	case CMD_CONFIG_PONG_ADDR: {
		int path = *((int *)cmd->value);
		struct vfe32_output_ch *outch =
			vfe32_get_ch(path, vfe32_ctrl->share_ctrl);
		outch->pong = *((struct msm_free_buf *)data);
	}
	break;

	case CMD_CONFIG_FREE_BUF_ADDR: {
		int path = *((int *)cmd->value);
		struct vfe32_output_ch *outch =
			vfe32_get_ch(path, vfe32_ctrl->share_ctrl);
		outch->free_buf = *((struct msm_free_buf *)data);
	}
	break;

	case CMD_SNAP_BUF_RELEASE:
		break;

	default:
		pr_err("%s Unsupported AXI configuration %x ", __func__,
			cmd->cmd_type);
	break;
	}
vfe32_config_done:
	kfree(scfg);
	kfree(sack);
	CDBG("%s done: rc = %d\n", __func__, (int) rc);
	return rc;
}

static struct msm_cam_clk_info vfe32_clk_info[] = {
	{"vfe_clk", 228570000},
	{"vfe_pclk", -1},
	{"csi_vfe_clk", -1},
};

static int msm_axi_subdev_s_crystal_freq(struct v4l2_subdev *sd,
						u32 freq, u32 flags)
{
	int rc = 0;
	int round_rate;
	struct axi_ctrl_t *axi_ctrl = v4l2_get_subdevdata(sd);
#if 0	//temp insook0321
	if (axi_ctrl->share_ctrl->dual_enabled) {
		CDBG("%s Dual camera Enabled hence returning "\
			"without clock change\n", __func__);
		return rc;
	}
#endif 	
	round_rate = clk_round_rate(axi_ctrl->vfe_clk[0], freq);
	if (rc < 0) {
		pr_err("%s: clk_round_rate failed %d\n",
					__func__, rc);
		return rc;
	}

	vfe_clk_rate = round_rate;
	rc = clk_set_rate(axi_ctrl->vfe_clk[0], round_rate);
	if (rc < 0)
		pr_err("%s: clk_set_rate failed %d\n",
					__func__, rc);

	return rc;
}

static const struct v4l2_subdev_core_ops msm_vfe_subdev_core_ops = {
	.ioctl = msm_vfe_subdev_ioctl,
};

static const struct v4l2_subdev_ops msm_vfe_subdev_ops = {
	.core = &msm_vfe_subdev_core_ops,
};

int msm_axi_subdev_init(struct v4l2_subdev *sd,
	uint8_t dual_enabled)
{
	int rc = 0;
	struct axi_ctrl_t *axi_ctrl = v4l2_get_subdevdata(sd);
	struct msm_cam_media_controller *mctl =
		v4l2_get_subdev_hostdata(sd);
	if (mctl == NULL) {
		rc = -EINVAL;
		goto mctl_failed;
	}

	axi_ctrl->share_ctrl->axi_ref_cnt++;
	if (axi_ctrl->share_ctrl->axi_ref_cnt > 1)
		return rc;
	axi_ctrl->share_ctrl->dual_enabled = dual_enabled;
	axi_ctrl->share_ctrl->default_dual_enabled = dual_enabled;
	axi_ctrl->share_ctrl->lp_mode = 0;
	pr_err("axi %s, %d\n", __func__, __LINE__);
	spin_lock_init(&axi_ctrl->tasklet_lock);
	INIT_LIST_HEAD(&axi_ctrl->tasklet_q);
	spin_lock_init(&axi_ctrl->share_ctrl->sd_notify_lock);

	axi_ctrl->share_ctrl->vfebase = ioremap(axi_ctrl->vfemem->start,
		resource_size(axi_ctrl->vfemem));
	if (!axi_ctrl->share_ctrl->vfebase) {
		rc = -ENOMEM;
		pr_err("%s: vfe ioremap failed\n", __func__);
		goto remap_failed;
	}

	if (axi_ctrl->fs_vfe) {
		rc = regulator_enable(axi_ctrl->fs_vfe);
		if (rc) {
			pr_err("%s: Regulator enable failed\n",	__func__);
			goto fs_failed;
		}
	}

	rc = msm_cam_clk_enable(&axi_ctrl->pdev->dev, vfe32_clk_info,
			axi_ctrl->vfe_clk, ARRAY_SIZE(vfe32_clk_info), 1);
	if (rc < 0)
		goto clk_enable_failed;

#ifdef CONFIG_MSM_IOMMU
	rc = iommu_attach_device(mctl->domain, axi_ctrl->iommu_ctx_imgwr);
	if (rc < 0) {
		pr_err("%s: imgwr attach failed rc = %d\n", __func__, rc);
		rc = -ENODEV;
		goto device_imgwr_attach_failed;
	}
	rc = iommu_attach_device(mctl->domain, axi_ctrl->iommu_ctx_misc);
	if (rc < 0) {
		pr_err("%s: misc attach failed rc = %d\n", __func__, rc);
		rc = -ENODEV;
		goto device_misc_attach_failed;
	}
#endif

#if defined(CONFIG_MACH_JACTIVE_ATT) || defined(CONFIG_MACH_JACTIVE_EUR)
	dtv_update_camera_vector_override(1);
#endif

	msm_camio_bus_scale_cfg(
		mctl->sdata->pdata->cam_bus_scale_table, S_INIT);

	CDBG("%s: axi_ctrl->share_ctrl->dual_enabled ? = %d\n", __func__,
			axi_ctrl->share_ctrl->dual_enabled);
	if (axi_ctrl->share_ctrl->dual_enabled) {
		pr_info("%s: Scaling bus config for dual bus vectors\n",
			__func__);
		msm_camio_bus_scale_cfg(
			mctl->sdata->pdata->cam_bus_scale_table, S_DUAL);
	} else
		msm_camio_bus_scale_cfg(
			mctl->sdata->pdata->cam_bus_scale_table, S_PREVIEW);

	if (msm_camera_io_r(
		axi_ctrl->share_ctrl->vfebase + V32_GET_HW_VERSION_OFF) ==
		VFE32_HW_NUMBER)
		axi_ctrl->share_ctrl->register_total = VFE32_REGISTER_TOTAL;
	else
		axi_ctrl->share_ctrl->register_total = VFE33_REGISTER_TOTAL;

	spin_lock_init(&axi_ctrl->share_ctrl->stop_flag_lock);
	spin_lock_init(&axi_ctrl->share_ctrl->update_ack_lock);
	spin_lock_init(&axi_ctrl->share_ctrl->start_ack_lock);

	enable_irq(axi_ctrl->vfeirq->start);

	return rc;

#ifdef CONFIG_MSM_IOMMU
device_misc_attach_failed:
	iommu_detach_device(mctl->domain, axi_ctrl->iommu_ctx_imgwr);
device_imgwr_attach_failed:
#endif
	msm_cam_clk_enable(&axi_ctrl->pdev->dev, vfe32_clk_info,
			axi_ctrl->vfe_clk, ARRAY_SIZE(vfe32_clk_info), 0);
clk_enable_failed:
	if (axi_ctrl->fs_vfe)
		regulator_disable(axi_ctrl->fs_vfe);
fs_failed:
	iounmap(axi_ctrl->share_ctrl->vfebase);
	axi_ctrl->share_ctrl->vfebase = NULL;
remap_failed:
mctl_failed:
	return rc;
}

int msm_vfe_subdev_init(struct v4l2_subdev *sd)
{
	int rc = 0;
	struct vfe32_ctrl_type *vfe32_ctrl =
		(struct vfe32_ctrl_type *)v4l2_get_subdevdata(sd);

	spin_lock_init(&vfe32_ctrl->state_lock);
	spin_lock_init(&vfe32_ctrl->stats_bufq_lock);

	vfe32_ctrl->update_linear = false;
	vfe32_ctrl->update_rolloff = false;
	vfe32_ctrl->update_la = false;
	vfe32_ctrl->update_gamma = false;
	vfe32_ctrl->vfe_sof_count_enable = false;
	vfe32_ctrl->hfr_mode = HFR_MODE_OFF;
	vfe32_ctrl->share_ctrl->rdi_comp = VFE_RDI_COMPOSITE;

	memset(&vfe32_ctrl->stats_ctrl, 0,
		sizeof(struct msm_stats_bufq_ctrl));
	memset(&vfe32_ctrl->stats_ops, 0, sizeof(struct msm_stats_ops));

	return rc;
}

void msm_axi_subdev_release(struct v4l2_subdev *sd)
{
	struct axi_ctrl_t *axi_ctrl = v4l2_get_subdevdata(sd);
	struct msm_cam_media_controller *pmctl =
		v4l2_get_subdev_hostdata(sd);

	if (!axi_ctrl->share_ctrl->vfebase) {
		pr_err("%s: base address unmapped\n", __func__);
		return;
	}

	axi_ctrl->share_ctrl->axi_ref_cnt--;
	if (axi_ctrl->share_ctrl->axi_ref_cnt > 0)
		return;

	pr_err("axi %s, %d\n", __func__, __LINE__);
	axi_clear_all_interrupts(axi_ctrl->share_ctrl);

	axi_ctrl->share_ctrl->dual_enabled = 0;
	axi_ctrl->share_ctrl->default_dual_enabled = 0;
	disable_irq(axi_ctrl->vfeirq->start);
	tasklet_kill(&axi_ctrl->vfe32_tasklet);
#ifdef CONFIG_MSM_IOMMU
	iommu_detach_device(pmctl->domain, axi_ctrl->iommu_ctx_misc);
	iommu_detach_device(pmctl->domain, axi_ctrl->iommu_ctx_imgwr);
#endif
	msm_cam_clk_enable(&axi_ctrl->pdev->dev, vfe32_clk_info,
			axi_ctrl->vfe_clk, ARRAY_SIZE(vfe32_clk_info), 0);
	if (axi_ctrl->fs_vfe)
		regulator_disable(axi_ctrl->fs_vfe);

	iounmap(axi_ctrl->share_ctrl->vfebase);
	axi_ctrl->share_ctrl->vfebase = NULL;

	if (atomic_read(&irq_cnt))
		pr_warning("%s, Warning IRQ Count not ZERO\n", __func__);

	msm_camio_bus_scale_cfg(
		pmctl->sdata->pdata->cam_bus_scale_table, S_EXIT);
#if defined(CONFIG_MACH_JACTIVE_ATT) || defined(CONFIG_MACH_JACTIVE_EUR)
	dtv_update_camera_vector_override(0);
#endif

}

void msm_vfe_subdev_release(struct v4l2_subdev *sd)
{
	struct vfe32_ctrl_type *vfe32_ctrl =
		(struct vfe32_ctrl_type *)v4l2_get_subdevdata(sd);
	CDBG("vfe subdev release %p\n",
		vfe32_ctrl->share_ctrl->vfebase);
}

int msm_axi_set_low_power_mode(struct v4l2_subdev *sd, void *arg)
{
	uint8_t lp_mode = 0;
	int rc = 0;
	struct axi_ctrl_t *axi_ctrl = v4l2_get_subdevdata(sd);
	if (copy_from_user(&lp_mode,
		(void __user *)(arg),
		sizeof(uint8_t))) {
		rc = -EFAULT;
		return rc;
	}
	axi_ctrl->share_ctrl->lp_mode = lp_mode;
	if (lp_mode)
		axi_ctrl->share_ctrl->dual_enabled = 0;
	else
		axi_ctrl->share_ctrl->dual_enabled = 
			axi_ctrl->share_ctrl->default_dual_enabled;
	return rc;
}

void axi_abort(struct axi_ctrl_t *axi_ctrl)
{
	uint8_t  axi_busy_flag = true;
	unsigned long flags = 0;
	/* axi halt command. */

	spin_lock_irqsave(&axi_ctrl->share_ctrl->stop_flag_lock, flags);
	axi_ctrl->share_ctrl->stop_ack_pending  = TRUE;
	spin_unlock_irqrestore(&axi_ctrl->share_ctrl->stop_flag_lock, flags);
	msm_camera_io_w(AXI_HALT,
		axi_ctrl->share_ctrl->vfebase + VFE_AXI_CMD);
	wmb();
	while (axi_busy_flag) {
		if (msm_camera_io_r(
			axi_ctrl->share_ctrl->vfebase + VFE_AXI_STATUS) & 0x1)
			axi_busy_flag = false;
	}
	/* Ensure the write order while writing
	* to the command register using the barrier */
	msm_camera_io_w_mb(AXI_HALT_CLEAR,
		axi_ctrl->share_ctrl->vfebase + VFE_AXI_CMD);

	/* after axi halt, then ok to apply global reset.
	* enable reset_ack and async timer interrupt only while
	* stopping the pipeline.*/
	msm_camera_io_w(0xf0000000,
		axi_ctrl->share_ctrl->vfebase + VFE_IRQ_MASK_0);
	msm_camera_io_w(VFE_IMASK_WHILE_STOPPING_1,
		axi_ctrl->share_ctrl->vfebase + VFE_IRQ_MASK_1);

	/* Ensure the write order while writing
	* to the command register using the barrier */
	msm_camera_io_w_mb(VFE_RESET_UPON_STOP_CMD,
		axi_ctrl->share_ctrl->vfebase + VFE_GLOBAL_RESET);
	if (axi_ctrl->share_ctrl->sync_abort)
		wait_for_completion_interruptible(
			&axi_ctrl->share_ctrl->reset_complete);
}

int axi_config_buffers(struct axi_ctrl_t *axi_ctrl,
	struct msm_camera_vfe_params_t vfe_params)
{
	uint16_t vfe_mode = axi_ctrl->share_ctrl->current_mode
			& ~(VFE_OUTPUTS_RDI0|VFE_OUTPUTS_RDI1|
				VFE_OUTPUTS_RDI2);
	int rc = 0;
	pr_info("%s: cmd type %d, axi mode %d\n", __func__,
		vfe_params.cmd_type, axi_ctrl->share_ctrl->current_mode);
	switch (vfe_params.cmd_type) {
	case AXI_CMD_PREVIEW:
		if (vfe_mode) {
			if ((axi_ctrl->share_ctrl->current_mode &
				VFE_OUTPUTS_PREVIEW_AND_VIDEO) ||
				(axi_ctrl->share_ctrl->current_mode &
				VFE_OUTPUTS_PREVIEW))
				/* Configure primary channel */
				rc = configure_pingpong_buffers(
					VFE_MSG_START,
					VFE_MSG_OUTPUT_PRIMARY,
					axi_ctrl);
			else
			/* Configure secondary channel */
				rc = configure_pingpong_buffers(
					VFE_MSG_START,
					VFE_MSG_OUTPUT_SECONDARY,
					axi_ctrl);
		}
		if (axi_ctrl->share_ctrl->current_mode &
				VFE_OUTPUTS_RDI0) {
			rc = configure_pingpong_buffers(
				VFE_MSG_START, VFE_MSG_OUTPUT_TERTIARY1,
				axi_ctrl);
		}
		if (axi_ctrl->share_ctrl->current_mode &
				VFE_OUTPUTS_RDI1) {
			rc = configure_pingpong_buffers(
				VFE_MSG_START, VFE_MSG_OUTPUT_TERTIARY2,
				axi_ctrl);
		}
		if (axi_ctrl->share_ctrl->current_mode &
				VFE_OUTPUTS_RDI2)
			rc = configure_pingpong_buffers(
				VFE_MSG_START, VFE_MSG_OUTPUT_TERTIARY3,
				axi_ctrl);
		if (rc < 0) {
			pr_err("%s error configuring pingpong buffers for preview",
				__func__);
			rc = -EINVAL;
			goto config_done;
		}
		break;
	case AXI_CMD_RAW_CAPTURE:
		rc = configure_pingpong_buffers(
			VFE_MSG_CAPTURE, VFE_MSG_OUTPUT_PRIMARY,
			axi_ctrl);
		if (rc < 0) {
			pr_err("%s error configuring pingpong buffers for snapshot",
				__func__);
			rc = -EINVAL;
			goto config_done;
		}
		break;
	case AXI_CMD_ZSL:
		rc = configure_pingpong_buffers(VFE_MSG_START,
			VFE_MSG_OUTPUT_PRIMARY, axi_ctrl);
		if (rc < 0)
			goto config_done;
		rc = configure_pingpong_buffers(VFE_MSG_START,
			VFE_MSG_OUTPUT_SECONDARY, axi_ctrl);
		if (rc < 0)
			goto config_done;
		break;
	case AXI_CMD_RECORD:
		if (axi_ctrl->share_ctrl->current_mode &
			VFE_OUTPUTS_PREVIEW_AND_VIDEO) {
			axi_ctrl->share_ctrl->outpath.out1.inst_handle =
				vfe_params.inst_handle;
			rc = configure_pingpong_buffers(
				VFE_MSG_START_RECORDING,
				VFE_MSG_OUTPUT_SECONDARY,
				axi_ctrl);
		} else if (axi_ctrl->share_ctrl->current_mode &
			VFE_OUTPUTS_VIDEO_AND_PREVIEW) {
			axi_ctrl->share_ctrl->outpath.out0.inst_handle =
				vfe_params.inst_handle;
			rc = configure_pingpong_buffers(
				VFE_MSG_START_RECORDING,
				VFE_MSG_OUTPUT_PRIMARY,
				axi_ctrl);
		}
		if (rc < 0) {
			pr_err("%s error configuring pingpong buffers for video",
				__func__);
			rc = -EINVAL;
			goto config_done;
		}
		break;
	case AXI_CMD_LIVESHOT:
		axi_ctrl->share_ctrl->outpath.out0.inst_handle =
			vfe_params.inst_handle;
		rc = configure_pingpong_buffers(VFE_MSG_CAPTURE,
					VFE_MSG_OUTPUT_PRIMARY, axi_ctrl);
		if (rc < 0) {
			pr_err("%s error configuring pingpong buffers for primary output",
				__func__);
			rc = -EINVAL;
			goto config_done;
		}
		break;
	case AXI_CMD_CAPTURE:
		if (vfe_mode) {
			if (axi_ctrl->share_ctrl->current_mode ==
				VFE_OUTPUTS_JPEG_AND_THUMB ||
			axi_ctrl->share_ctrl->current_mode ==
				VFE_OUTPUTS_THUMB_AND_JPEG) {

				/* Configure primary channel for JPEG */
				rc = configure_pingpong_buffers(
					VFE_MSG_JPEG_CAPTURE,
					VFE_MSG_OUTPUT_PRIMARY,
					axi_ctrl);
			} else {
				/* Configure primary channel */
				rc = configure_pingpong_buffers(
					VFE_MSG_CAPTURE,
					VFE_MSG_OUTPUT_PRIMARY,
					axi_ctrl);
			}
			if (rc < 0) {
				pr_err("%s error configuring pingpong buffers for primary output",
					__func__);
				rc = -EINVAL;
				goto config_done;
			}
			/* Configure secondary channel */
			rc = configure_pingpong_buffers(
					VFE_MSG_CAPTURE,
					VFE_MSG_OUTPUT_SECONDARY,
					axi_ctrl);
			if (rc < 0) {
				pr_err("%s error configuring pingpong buffers for secondary output",
					__func__);
				rc = -EINVAL;
				goto config_done;
			}
		}

		if (axi_ctrl->share_ctrl->current_mode &
				VFE_OUTPUTS_RDI0)
			rc = configure_pingpong_buffers(
				VFE_MSG_CAPTURE, VFE_MSG_OUTPUT_TERTIARY1,
				axi_ctrl);
		if (axi_ctrl->share_ctrl->current_mode &
				VFE_OUTPUTS_RDI1)
			rc = configure_pingpong_buffers(
				VFE_MSG_CAPTURE, VFE_MSG_OUTPUT_TERTIARY2,
				axi_ctrl);
		if (axi_ctrl->share_ctrl->current_mode &
			VFE_OUTPUTS_RDI2)
			rc = configure_pingpong_buffers(
				VFE_MSG_CAPTURE, VFE_MSG_OUTPUT_TERTIARY3,
				axi_ctrl);
		break;
	default:
		rc = -EINVAL;
		break;

	}
config_done:
	return rc;
}

void axi_start(struct msm_cam_media_controller *pmctl,
	struct axi_ctrl_t *axi_ctrl, struct msm_camera_vfe_params_t vfe_params)
{
	int rc = 0;
	uint32_t reg_update = 0;
	uint32_t vfe_mode =
		(axi_ctrl->share_ctrl->current_mode &
		~(VFE_OUTPUTS_RDI0|VFE_OUTPUTS_RDI1|VFE_OUTPUTS_RDI2));
	CDBG("axi start = %d\n",
		axi_ctrl->share_ctrl->current_mode);
	rc = axi_config_buffers(axi_ctrl, vfe_params);
	if (rc < 0)
		return;

	switch (vfe_params.cmd_type) {
	case AXI_CMD_PREVIEW:
		if (axi_ctrl->share_ctrl->lp_mode)
			msm_camio_bus_scale_cfg(
			pmctl->sdata->pdata->cam_bus_scale_table, S_LOW_POWER);

		else if (!axi_ctrl->share_ctrl->dual_enabled)
			msm_camio_bus_scale_cfg(
			pmctl->sdata->pdata->cam_bus_scale_table, S_PREVIEW);

		break;
	case AXI_CMD_CAPTURE:
	case AXI_CMD_RAW_CAPTURE:
		if (!axi_ctrl->share_ctrl->dual_enabled)
			msm_camio_bus_scale_cfg(
			pmctl->sdata->pdata->cam_bus_scale_table, S_CAPTURE);
		break;
	case AXI_CMD_RECORD:
		if (!axi_ctrl->share_ctrl->dual_enabled)
			msm_camio_bus_scale_cfg(
			pmctl->sdata->pdata->cam_bus_scale_table, S_VIDEO);
		return;
	case AXI_CMD_ZSL:
		if (axi_ctrl->share_ctrl->lp_mode)
			msm_camio_bus_scale_cfg(
			pmctl->sdata->pdata->cam_bus_scale_table, S_LOW_POWER);

		else if (!axi_ctrl->share_ctrl->dual_enabled)
			msm_camio_bus_scale_cfg(
				pmctl->sdata->pdata->cam_bus_scale_table,
				S_ZSL);
		break;
	case AXI_CMD_LIVESHOT:
		if (!axi_ctrl->share_ctrl->dual_enabled)
			msm_camio_bus_scale_cfg(
			pmctl->sdata->pdata->cam_bus_scale_table, S_LIVESHOT);
		return;
	default:
		return;
	}
	axi_enable_wm_irq(axi_ctrl->share_ctrl);

	switch (vfe_params.cmd_type) {
	case AXI_CMD_RAW_CAPTURE:
		msm_camera_io_w((
			0x1 << axi_ctrl->share_ctrl->outpath.out0.ch0),
			axi_ctrl->share_ctrl->vfebase + VFE_BUS_CMD);
		msm_camera_io_w(1, axi_ctrl->share_ctrl->vfebase
			+ vfe32_AXI_WM_CFG[axi_ctrl->
			share_ctrl->outpath.out0.ch0]);
		break;
	case AXI_CMD_PREVIEW: {
		switch (vfe_mode) {
		case VFE_OUTPUTS_PREVIEW:
		case VFE_OUTPUTS_PREVIEW_AND_VIDEO:
			if (axi_ctrl->share_ctrl->outpath.output_mode &
				VFE32_OUTPUT_MODE_PRIMARY) {
				msm_camera_io_w((
					0x1 << axi_ctrl->share_ctrl->
							outpath.out0.ch0 |
					0x1 << axi_ctrl->share_ctrl->
							outpath.out0.ch1),
					axi_ctrl->share_ctrl->vfebase +
							VFE_BUS_CMD);
				msm_camera_io_w(1, axi_ctrl->share_ctrl->vfebase
					+ vfe32_AXI_WM_CFG[axi_ctrl->
					share_ctrl->outpath.out0.ch0]);
				msm_camera_io_w(1, axi_ctrl->share_ctrl->vfebase
					+ vfe32_AXI_WM_CFG[axi_ctrl->
					share_ctrl->outpath.out0.ch1]);


			} else if (axi_ctrl->share_ctrl->outpath.output_mode &
					VFE32_OUTPUT_MODE_PRIMARY_ALL_CHNLS) {
				msm_camera_io_w((
					0x1 << axi_ctrl->share_ctrl->
							outpath.out0.ch0 |
					0x1 << axi_ctrl->share_ctrl->
							outpath.out0.ch1 |
					0x1 << axi_ctrl->share_ctrl->
							outpath.out0.ch2),
					axi_ctrl->share_ctrl->vfebase +
							VFE_BUS_CMD);
				msm_camera_io_w(1, axi_ctrl->share_ctrl->vfebase
					+ vfe32_AXI_WM_CFG[axi_ctrl->
					share_ctrl->outpath.out0.ch0]);
				msm_camera_io_w(1, axi_ctrl->share_ctrl->vfebase
					+ vfe32_AXI_WM_CFG[axi_ctrl->
					share_ctrl->outpath.out0.ch1]);
				msm_camera_io_w(1, axi_ctrl->share_ctrl->vfebase
					+ vfe32_AXI_WM_CFG[axi_ctrl->
					share_ctrl->outpath.out0.ch2]);
			}
			break;
		default:
			if (axi_ctrl->share_ctrl->outpath.output_mode &
				VFE32_OUTPUT_MODE_SECONDARY) {
				msm_camera_io_w((
					0x1 << axi_ctrl->share_ctrl->
						outpath.out1.ch0 |
					0x1 << axi_ctrl->share_ctrl->
						outpath.out1.ch1),
					axi_ctrl->share_ctrl->vfebase +
						VFE_BUS_CMD);
				msm_camera_io_w(1, axi_ctrl->share_ctrl->vfebase
					+ vfe32_AXI_WM_CFG[axi_ctrl->
					share_ctrl->outpath.out1.ch0]);
				msm_camera_io_w(1, axi_ctrl->share_ctrl->vfebase
					+ vfe32_AXI_WM_CFG[axi_ctrl->
					share_ctrl->outpath.out1.ch1]);
			} else if (axi_ctrl->share_ctrl->outpath.output_mode &
				VFE32_OUTPUT_MODE_SECONDARY_ALL_CHNLS) {
				msm_camera_io_w((
					0x1 << axi_ctrl->share_ctrl->
							outpath.out1.ch0 |
					0x1 << axi_ctrl->share_ctrl->
							outpath.out1.ch1 |
					0x1 << axi_ctrl->share_ctrl->
							outpath.out1.ch2),
					axi_ctrl->share_ctrl->vfebase +
							VFE_BUS_CMD);
				msm_camera_io_w(1, axi_ctrl->share_ctrl->vfebase
					+ vfe32_AXI_WM_CFG[axi_ctrl->
					share_ctrl->outpath.out1.ch0]);
				msm_camera_io_w(1, axi_ctrl->share_ctrl->vfebase
					+ vfe32_AXI_WM_CFG[axi_ctrl->
					share_ctrl->outpath.out1.ch1]);
				msm_camera_io_w(1, axi_ctrl->share_ctrl->vfebase
					+ vfe32_AXI_WM_CFG[axi_ctrl->
					share_ctrl->outpath.out1.ch2]);
			}
			break;
			}
		}
		break;
	default:
		if (axi_ctrl->share_ctrl->outpath.output_mode &
			VFE32_OUTPUT_MODE_PRIMARY) {
			msm_camera_io_w((
				0x1 << axi_ctrl->share_ctrl->outpath.out0.ch0 |
				0x1 << axi_ctrl->share_ctrl->outpath.out0.ch1),
				axi_ctrl->share_ctrl->vfebase + VFE_BUS_CMD);
			msm_camera_io_w(1, axi_ctrl->share_ctrl->vfebase +
				vfe32_AXI_WM_CFG[axi_ctrl->
				share_ctrl->outpath.out0.ch0]);
			msm_camera_io_w(1, axi_ctrl->share_ctrl->vfebase +
				vfe32_AXI_WM_CFG[axi_ctrl->
				share_ctrl->outpath.out0.ch1]);
		} else if (axi_ctrl->share_ctrl->outpath.output_mode &
				VFE32_OUTPUT_MODE_PRIMARY_ALL_CHNLS) {
			msm_camera_io_w((
				0x1 << axi_ctrl->share_ctrl->outpath.out0.ch0 |
				0x1 << axi_ctrl->share_ctrl->outpath.out0.ch1 |
				0x1 << axi_ctrl->share_ctrl->outpath.out0.ch2),
				axi_ctrl->share_ctrl->vfebase + VFE_BUS_CMD);
			msm_camera_io_w(1, axi_ctrl->share_ctrl->vfebase +
				vfe32_AXI_WM_CFG[axi_ctrl->
				share_ctrl->outpath.out0.ch0]);
			msm_camera_io_w(1, axi_ctrl->share_ctrl->vfebase +
				vfe32_AXI_WM_CFG[axi_ctrl->
				share_ctrl->outpath.out0.ch1]);
			msm_camera_io_w(1, axi_ctrl->share_ctrl->vfebase +
				vfe32_AXI_WM_CFG[axi_ctrl->
				share_ctrl->outpath.out0.ch2]);
		}

		if (axi_ctrl->share_ctrl->outpath.output_mode &
			VFE32_OUTPUT_MODE_SECONDARY) {
			msm_camera_io_w((
				0x1 << axi_ctrl->share_ctrl->outpath.out1.ch0 |
				0x1 << axi_ctrl->share_ctrl->outpath.out1.ch1),
				axi_ctrl->share_ctrl->vfebase + VFE_BUS_CMD);
			msm_camera_io_w(1, axi_ctrl->share_ctrl->vfebase +
				vfe32_AXI_WM_CFG[axi_ctrl->
				share_ctrl->outpath.out1.ch0]);
			msm_camera_io_w(1, axi_ctrl->share_ctrl->vfebase +
				vfe32_AXI_WM_CFG[axi_ctrl->
				share_ctrl->outpath.out1.ch1]);
		} else if (axi_ctrl->share_ctrl->outpath.output_mode &
			VFE32_OUTPUT_MODE_SECONDARY_ALL_CHNLS) {
			msm_camera_io_w((
				0x1 << axi_ctrl->share_ctrl->outpath.out1.ch0 |
				0x1 << axi_ctrl->share_ctrl->outpath.out1.ch1 |
				0x1 << axi_ctrl->share_ctrl->outpath.out1.ch2),
				axi_ctrl->share_ctrl->vfebase + VFE_BUS_CMD);
			msm_camera_io_w(1, axi_ctrl->share_ctrl->vfebase +
				vfe32_AXI_WM_CFG[axi_ctrl->
				share_ctrl->outpath.out1.ch0]);
			msm_camera_io_w(1, axi_ctrl->share_ctrl->vfebase +
				vfe32_AXI_WM_CFG[axi_ctrl->
				share_ctrl->outpath.out1.ch1]);
			msm_camera_io_w(1, axi_ctrl->share_ctrl->vfebase +
				vfe32_AXI_WM_CFG[axi_ctrl->
				share_ctrl->outpath.out1.ch2]);
		}
		break;
	}
	if (axi_ctrl->share_ctrl->current_mode & VFE_OUTPUTS_RDI0) {
		axi_ctrl->share_ctrl->outpath.out2.capture_cnt =
						vfe_params.capture_count;
		axi_ctrl->share_ctrl->rdi0_capture_count =
						vfe_params.capture_count;
		msm_camera_io_w((
				0x1 << axi_ctrl->share_ctrl->outpath.out2.ch0),
				axi_ctrl->share_ctrl->vfebase + VFE_BUS_CMD);
		/* 0x1: Line base, 0x3: Frame base */
		msm_camera_io_w(0x3, axi_ctrl->share_ctrl->vfebase +
			vfe32_AXI_WM_CFG[axi_ctrl->share_ctrl->
			outpath.out2.ch0]);
	}
	if (axi_ctrl->share_ctrl->current_mode & VFE_OUTPUTS_RDI1) {
		axi_ctrl->share_ctrl->outpath.out3.capture_cnt =
						vfe_params.capture_count;
		axi_ctrl->share_ctrl->rdi1_capture_count =
						vfe_params.capture_count;
		msm_camera_io_w((
				0x1 << axi_ctrl->share_ctrl->outpath.out3.ch0),
				axi_ctrl->share_ctrl->vfebase + VFE_BUS_CMD);
		/* 0x1: Line base, 0x3: Frame base */
		msm_camera_io_w(0x3, axi_ctrl->share_ctrl->vfebase +
			vfe32_AXI_WM_CFG[axi_ctrl->share_ctrl->
			outpath.out3.ch0]);
	}
	if (axi_ctrl->share_ctrl->current_mode & VFE_OUTPUTS_RDI2) {
		axi_ctrl->share_ctrl->outpath.out4.capture_cnt =
						vfe_params.capture_count;
		axi_ctrl->share_ctrl->rdi2_capture_count =
						vfe_params.capture_count;
		msm_camera_io_w((
				0x1 << axi_ctrl->share_ctrl->outpath.out4.ch0),
				axi_ctrl->share_ctrl->vfebase + VFE_BUS_CMD);
		CDBG("AXI WM configured as frame based\n");
		msm_camera_io_w(0x3, axi_ctrl->share_ctrl->vfebase +
			vfe32_AXI_WM_CFG[axi_ctrl->share_ctrl->
			outpath.out4.ch0]);
	}

	pr_err("axi %s, %d", __func__, __LINE__);
	axi_enable_irq(axi_ctrl->share_ctrl);

	if (axi_ctrl->share_ctrl->current_mode & VFE_OUTPUTS_RDI0) {
		if (!atomic_cmpxchg(
			&axi_ctrl->share_ctrl->rdi0_update_ack_pending,
				0, 1))
			reg_update |= 0x2;
	}
	if (axi_ctrl->share_ctrl->current_mode & VFE_OUTPUTS_RDI1) {
		if (!atomic_cmpxchg(
			&axi_ctrl->share_ctrl->rdi1_update_ack_pending,
				0, 1))
			reg_update |= 0x4;
	}
	if (axi_ctrl->share_ctrl->current_mode & VFE_OUTPUTS_RDI2) {
		if (!atomic_cmpxchg(
			&axi_ctrl->share_ctrl->rdi2_update_ack_pending,
				0, 1))
			reg_update |= 0x8;
	}

	if (vfe_mode) {
		if (!atomic_cmpxchg(
			&axi_ctrl->share_ctrl->pix0_update_ack_pending,
				0, 1))
			reg_update |= 0x1;
	}

	msm_camera_io_w_mb(reg_update,
			axi_ctrl->share_ctrl->vfebase +
			VFE_REG_UPDATE_CMD);
	axi_ctrl->share_ctrl->operation_mode |=
		axi_ctrl->share_ctrl->current_mode;
}

void axi_stop(struct msm_cam_media_controller *pmctl,
	struct axi_ctrl_t *axi_ctrl, struct msm_camera_vfe_params_t vfe_params)
{
	uint32_t reg_update = 0;
	uint32_t vfe_mode =
	axi_ctrl->share_ctrl->current_mode & ~(VFE_OUTPUTS_RDI0|
		VFE_OUTPUTS_RDI1|VFE_OUTPUTS_RDI2);
	switch (vfe_params.cmd_type) {
	case AXI_CMD_PREVIEW:
	case AXI_CMD_CAPTURE:
	case AXI_CMD_RAW_CAPTURE:
	case AXI_CMD_ZSL:
		axi_ctrl->share_ctrl->cmd_type = vfe_params.cmd_type;
		break;
	case AXI_CMD_RECORD:
		if (!axi_ctrl->share_ctrl->dual_enabled)
			msm_camio_bus_scale_cfg(
			pmctl->sdata->pdata->cam_bus_scale_table, S_PREVIEW);
		return;
	case AXI_CMD_LIVESHOT:
		if (!axi_ctrl->share_ctrl->dual_enabled)
			msm_camio_bus_scale_cfg(
			pmctl->sdata->pdata->cam_bus_scale_table, S_VIDEO);
		return;
	default:
		return;
	}

	if (axi_ctrl->share_ctrl->stop_immediately) {
		axi_disable_irq(axi_ctrl->share_ctrl,
			axi_ctrl->share_ctrl->current_mode);
		axi_stop_process(axi_ctrl->share_ctrl);

		if (axi_ctrl->share_ctrl->stream_error == 1) {
			pr_err(" Indicate stream error");
			vfe32_send_isp_msg(
				&(axi_ctrl->share_ctrl->vfe32_ctrl->subdev),
				axi_ctrl->share_ctrl->vfe32_ctrl->
				share_ctrl->vfeFrameId,
				MSG_ID_PREV_STOP_ACK);
		}

		return;
	}

	if (axi_ctrl->share_ctrl->current_mode & VFE_OUTPUTS_RDI0) {
		msm_camera_io_w(0, axi_ctrl->share_ctrl->vfebase +
			vfe32_AXI_WM_CFG[axi_ctrl->share_ctrl->
				outpath.out2.ch0]);
		if (!atomic_cmpxchg(
			&axi_ctrl->share_ctrl->rdi0_update_ack_pending,
				0, 2))
			reg_update |= 0x2;
	}
	if (axi_ctrl->share_ctrl->current_mode & VFE_OUTPUTS_RDI1) {
		msm_camera_io_w(0, axi_ctrl->share_ctrl->vfebase +
			vfe32_AXI_WM_CFG[axi_ctrl->share_ctrl->
				outpath.out3.ch0]);
		if (!atomic_cmpxchg(
			&axi_ctrl->share_ctrl->rdi1_update_ack_pending,
				0, 2))
			reg_update |= 0x4;
	}
	if (axi_ctrl->share_ctrl->current_mode & VFE_OUTPUTS_RDI2) {
		msm_camera_io_w(0, axi_ctrl->share_ctrl->vfebase +
			vfe32_AXI_WM_CFG[axi_ctrl->share_ctrl->
				outpath.out4.ch0]);
		if (!atomic_cmpxchg(
			&axi_ctrl->share_ctrl->rdi2_update_ack_pending,
				0, 2))
			reg_update |= 0x8;
	}
	if (vfe_mode) {
		if (!atomic_cmpxchg(
			&axi_ctrl->share_ctrl->pix0_update_ack_pending,
				0, 2))
			reg_update |= 0x1;
	}
	msm_camera_io_w_mb(reg_update,
		axi_ctrl->share_ctrl->vfebase + VFE_REG_UPDATE_CMD);
}

static int msm_axi_config(struct v4l2_subdev *sd, void __user *arg)
{
	struct msm_vfe_cfg_cmd cfgcmd;
	struct msm_isp_cmd vfecmd;
	struct axi_ctrl_t *axi_ctrl = v4l2_get_subdevdata(sd);
	struct msm_cam_media_controller *pmctl =
		(struct msm_cam_media_controller *)v4l2_get_subdev_hostdata(sd);
	int rc = 0, vfe_cmd_type = 0, rdi_mode = 0;

	if (!axi_ctrl->share_ctrl->vfebase) {
		pr_err("%s: base address unmapped\n", __func__);
		return -EFAULT;
	}
	memset(&cfgcmd, 0, sizeof(struct msm_vfe_cfg_cmd));
	if (NULL != arg) {
		if (copy_from_user(&cfgcmd, arg, sizeof(cfgcmd))) {
			ERR_COPY_FROM_USER();
			return -EFAULT;
		}
	}
	memset(&vfecmd, 0, sizeof(struct msm_isp_cmd));
	if (NULL != cfgcmd.value) {
		if (copy_from_user(&vfecmd,
				(void __user *)(cfgcmd.value),
				sizeof(vfecmd))) {
			pr_err("%s %d: copy_from_user failed\n", __func__,
				__LINE__);
			return -EFAULT;
		}
	}

	vfe_cmd_type = (cfgcmd.cmd_type & ~(CMD_AXI_CFG_TERT1|
		CMD_AXI_CFG_TERT2 | CMD_AXI_CFG_TERT3));
	switch (cfgcmd.cmd_type) {
	case CMD_AXI_CFG_TERT1:{
		uint32_t *axio = NULL;
		axio = kmalloc(vfe32_cmd[VFE_CMD_AXI_OUT_CFG].length,
				GFP_ATOMIC);
		if (!axio)
			return -ENOMEM;

		if (copy_from_user(axio, (void __user *)(vfecmd.value),
				vfe32_cmd[VFE_CMD_AXI_OUT_CFG].length)) {
			kfree(axio);
			return -EFAULT;
		}
		vfe32_config_axi(axi_ctrl, OUTPUT_TERT1, axio);
		kfree(axio);
		return rc;
		}
	case CMD_AXI_CFG_TERT2:{
		uint32_t *axio = NULL;
		axio = kmalloc(vfe32_cmd[VFE_CMD_AXI_OUT_CFG].length,
				GFP_ATOMIC);
		if (!axio)
			return -ENOMEM;

		if (copy_from_user(axio, (void __user *)(vfecmd.value),
				vfe32_cmd[VFE_CMD_AXI_OUT_CFG].length)) {
			kfree(axio);
			return -EFAULT;
		}
		vfe32_config_axi(axi_ctrl, OUTPUT_TERT2, axio);
		kfree(axio);
		return rc;
		}
	case CMD_AXI_CFG_TERT3:{
		uint32_t *axio = NULL;
		axio = kmalloc(vfe32_cmd[VFE_CMD_AXI_OUT_CFG].length,
				GFP_ATOMIC);
		if (!axio)
			return -ENOMEM;

		if (copy_from_user(axio, (void __user *)(vfecmd.value),
				vfe32_cmd[VFE_CMD_AXI_OUT_CFG].length)) {
			kfree(axio);
			return -EFAULT;
		}
		vfe32_config_axi(axi_ctrl, OUTPUT_TERT3, axio);
		kfree(axio);
		return rc;
		}
	case CMD_AXI_CFG_TERT1|CMD_AXI_CFG_TERT2:{
		uint32_t *axio = NULL;
		axio = kmalloc(vfe32_cmd[VFE_CMD_AXI_OUT_CFG].length,
				GFP_ATOMIC);
		if (!axio)
			return -ENOMEM;

		if (copy_from_user(axio, (void __user *)(vfecmd.value),
				vfe32_cmd[VFE_CMD_AXI_OUT_CFG].length)) {
			kfree(axio);
			return -EFAULT;
		}
		vfe32_config_axi(axi_ctrl, OUTPUT_TERT1|OUTPUT_TERT2, axio);
		kfree(axio);
		return rc;
		}
	case CMD_AXI_CFG_TERT1|CMD_AXI_CFG_TERT3:{
		uint32_t *axio = NULL;
		axio = kmalloc(vfe32_cmd[VFE_CMD_AXI_OUT_CFG].length,
				GFP_ATOMIC);
		if (!axio)
			return -ENOMEM;

		if (copy_from_user(axio, (void __user *)(vfecmd.value),
				vfe32_cmd[VFE_CMD_AXI_OUT_CFG].length)) {
			kfree(axio);
			return -EFAULT;
		}
		vfe32_config_axi(axi_ctrl, OUTPUT_TERT1|OUTPUT_TERT3, axio);
		kfree(axio);
		return rc;
		}
	case CMD_AXI_CFG_TERT2|CMD_AXI_CFG_TERT3:{
		uint32_t *axio = NULL;
		axio = kmalloc(vfe32_cmd[VFE_CMD_AXI_OUT_CFG].length,
				GFP_ATOMIC);
		if (!axio)
			return -ENOMEM;

		if (copy_from_user(axio, (void __user *)(vfecmd.value),
				vfe32_cmd[VFE_CMD_AXI_OUT_CFG].length)) {
			kfree(axio);
			return -EFAULT;
		}
		vfe32_config_axi(axi_ctrl, OUTPUT_TERT2|OUTPUT_TERT3, axio);
		kfree(axio);
		return rc;
		}
	case CMD_AXI_CFG_TERT1|CMD_AXI_CFG_TERT2|CMD_AXI_CFG_TERT3:{
		uint32_t *axio = NULL;
		axio = kmalloc(vfe32_cmd[VFE_CMD_AXI_OUT_CFG].length,
				GFP_ATOMIC);
		if (!axio)
			return -ENOMEM;

		if (copy_from_user(axio, (void __user *)(vfecmd.value),
				vfe32_cmd[VFE_CMD_AXI_OUT_CFG].length)) {
			kfree(axio);
			return -EFAULT;
		}
		vfe32_config_axi(axi_ctrl, OUTPUT_TERT1|OUTPUT_TERT2|
			OUTPUT_TERT3, axio);
		kfree(axio);
		return rc;
		}
	default:
		if (cfgcmd.cmd_type & CMD_AXI_CFG_TERT1)
			rdi_mode |= OUTPUT_TERT1;
		if (cfgcmd.cmd_type & CMD_AXI_CFG_TERT2)
			rdi_mode |= OUTPUT_TERT2;
		if (cfgcmd.cmd_type & CMD_AXI_CFG_TERT3)
			rdi_mode |= OUTPUT_TERT3;
	}
	switch (vfe_cmd_type) {
	case CMD_AXI_CFG_PRIM: {
		uint32_t *axio = NULL;
		axio = kmalloc(vfe32_cmd[VFE_CMD_AXI_OUT_CFG].length,
				GFP_ATOMIC);
		if (!axio) {
			rc = -ENOMEM;
			break;
		}

		if (copy_from_user(axio, (void __user *)(vfecmd.value),
				vfe32_cmd[VFE_CMD_AXI_OUT_CFG].length)) {
			kfree(axio);
			rc = -EFAULT;
			break;
		}
		vfe32_config_axi(axi_ctrl, rdi_mode|OUTPUT_PRIM, axio);
		kfree(axio);
		break;
		}
	case CMD_AXI_CFG_PRIM_ALL_CHNLS: {
		uint32_t *axio = NULL;
		axio = kmalloc(vfe32_cmd[VFE_CMD_AXI_OUT_CFG].length,
				GFP_ATOMIC);
		if (!axio) {
			rc = -ENOMEM;
			break;
		}

		if (copy_from_user(axio, (void __user *)(vfecmd.value),
				vfe32_cmd[VFE_CMD_AXI_OUT_CFG].length)) {
			kfree(axio);
			rc = -EFAULT;
			break;
		}
		vfe32_config_axi(axi_ctrl, rdi_mode|OUTPUT_PRIM_ALL_CHNLS,
			axio);
		kfree(axio);
		break;
		}
	case CMD_AXI_CFG_PRIM|CMD_AXI_CFG_SEC: {
		uint32_t *axio = NULL;
		axio = kmalloc(vfe32_cmd[VFE_CMD_AXI_OUT_CFG].length,
				GFP_ATOMIC);
		if (!axio) {
			rc = -ENOMEM;
			break;
		}

		if (copy_from_user(axio, (void __user *)(vfecmd.value),
				vfe32_cmd[VFE_CMD_AXI_OUT_CFG].length)) {
			kfree(axio);
			rc = -EFAULT;
			break;
		}
		vfe32_config_axi(axi_ctrl,
			rdi_mode|OUTPUT_PRIM|OUTPUT_SEC, axio);
		kfree(axio);
		break;
		}
	case CMD_AXI_CFG_PRIM|CMD_AXI_CFG_SEC_ALL_CHNLS: {
		uint32_t *axio = NULL;
		axio = kmalloc(vfe32_cmd[VFE_CMD_AXI_OUT_CFG].length,
				GFP_ATOMIC);
		if (!axio) {
			rc = -ENOMEM;
			break;
		}

		if (copy_from_user(axio, (void __user *)(vfecmd.value),
				vfe32_cmd[VFE_CMD_AXI_OUT_CFG].length)) {
			kfree(axio);
			rc = -EFAULT;
			break;
		}
		vfe32_config_axi(axi_ctrl,
			rdi_mode|OUTPUT_PRIM|OUTPUT_SEC_ALL_CHNLS, axio);
		kfree(axio);
		break;
		}
	case CMD_AXI_CFG_PRIM_ALL_CHNLS|CMD_AXI_CFG_SEC: {
		uint32_t *axio = NULL;
		axio = kmalloc(vfe32_cmd[VFE_CMD_AXI_OUT_CFG].length,
				GFP_ATOMIC);
		if (!axio) {
			rc = -ENOMEM;
			break;
		}

		if (copy_from_user(axio, (void __user *)(vfecmd.value),
				vfe32_cmd[VFE_CMD_AXI_OUT_CFG].length)) {
			kfree(axio);
			rc = -EFAULT;
			break;
		}
		vfe32_config_axi(axi_ctrl,
			rdi_mode|OUTPUT_PRIM_ALL_CHNLS|OUTPUT_SEC, axio);
		kfree(axio);
		break;
		}

	case CMD_AXI_CFG_PRIM_ALL_CHNLS|CMD_AXI_CFG_SEC_ALL_CHNLS:
		pr_err("%s Invalid/Unsupported AXI configuration %x",
			__func__, cfgcmd.cmd_type);
		break;
	case CMD_AXI_START: {
		struct msm_camera_vfe_params_t vfe_params;
		if (copy_from_user(&vfe_params,
				(void __user *)(vfecmd.value),
				sizeof(struct msm_camera_vfe_params_t))) {
				return -EFAULT;
		}
		axi_ctrl->share_ctrl->current_mode =
			vfe_params.operation_mode;
		axi_start(pmctl, axi_ctrl, vfe_params);
		}
		break;
	case CMD_AXI_STOP: {
		struct msm_camera_vfe_params_t vfe_params;
		if (copy_from_user(&vfe_params,
				(void __user *)(vfecmd.value),
				sizeof(struct msm_camera_vfe_params_t))) {
				return -EFAULT;
		}
		axi_ctrl->share_ctrl->current_mode =
			vfe_params.operation_mode;
		axi_ctrl->share_ctrl->stop_immediately =
			vfe_params.stop_immediately;
		axi_ctrl->share_ctrl->stream_error =
			vfe_params.stream_error;
		axi_stop(pmctl, axi_ctrl, vfe_params);
		}
		break;
	case CMD_AXI_RESET: {
		struct msm_camera_vfe_params_t vfe_params;
		if (copy_from_user(&vfe_params,
				(void __user *)(vfecmd.value),
				sizeof(struct msm_camera_vfe_params_t))) {
				return -EFAULT;
		}
		axi_reset(axi_ctrl, vfe_params);
		}
		break;
	case CMD_AXI_ABORT:
		if (copy_from_user(&axi_ctrl->share_ctrl->sync_abort,
				(void __user *)(vfecmd.value),
				sizeof(uint8_t))) {
				return -EFAULT;
		}
		axi_abort(axi_ctrl);
		break;
	default:
		pr_err("%s Unsupported AXI configuration %x ", __func__,
			cfgcmd.cmd_type);
		break;
	}
	return rc;
}

static void msm_axi_process_irq(struct v4l2_subdev *sd, void *arg)
{
	struct axi_ctrl_t *axi_ctrl = v4l2_get_subdevdata(sd);
	uint32_t irqstatus = (uint32_t) arg;

	if (!axi_ctrl->share_ctrl->vfebase) {
		pr_err("%s: base address unmapped\n", __func__);
		return;
	}

	/* next, check output path related interrupts. */
	if (irqstatus &
		VFE_IRQ_STATUS0_IMAGE_COMPOSIT_DONE0_MASK) {
		CDBG("Image composite done 0 irq occured.\n");
		vfe32_process_output_path_irq_0(axi_ctrl);
	}
	if (irqstatus &
		VFE_IRQ_STATUS0_IMAGE_COMPOSIT_DONE1_MASK) {
		CDBG("Image composite done 1 irq occured.\n");
		vfe32_process_output_path_irq_1(axi_ctrl);
	}
	if (irqstatus &
		VFE_IRQ_STATUS0_IMAGE_COMPOSIT_DONE2_MASK) {
		CDBG("Image composite done 2 irq occured.\n");
		vfe32_process_output_path_irq_rdi0_and_rdi1(axi_ctrl);
	}

	if (axi_ctrl->share_ctrl->comp_output_mode &
		VFE32_OUTPUT_MODE_TERTIARY1) {
		if (irqstatus & (0x1 << (axi_ctrl->share_ctrl->outpath.out2.ch0
			+ VFE_WM_OFFSET))) {
			CDBG("VFE32_OUTPUT_MODE_TERTIARY1\n");
			/*vfe32_process_output_path_irq_rdi0(axi_ctrl);*/
		}
	}
	if (axi_ctrl->share_ctrl->comp_output_mode &
		VFE32_OUTPUT_MODE_TERTIARY2){
		if (irqstatus & (0x1 << (axi_ctrl->share_ctrl->outpath.out3.ch0
			+ VFE_WM_OFFSET))) {
			CDBG("VFE32_OUTPUT_MODE_TERTIARY2\n");
			/*vfe32_process_output_path_irq_rdi1(axi_ctrl);*/
		}
	}
	if (axi_ctrl->share_ctrl->comp_output_mode &
		VFE32_OUTPUT_MODE_TERTIARY3) {
		CDBG("Before process output path" \
			"of RDI2 irqstatus %x\n", irqstatus);
		if (irqstatus & (0x1 << (axi_ctrl->share_ctrl->outpath.out4.ch0
			+ VFE_WM_OFFSET)))
			vfe32_process_output_path_irq_rdi2(axi_ctrl);
	}

	/* in snapshot mode if done then send
	snapshot done message */
	if (
		axi_ctrl->share_ctrl->operation_mode &
			VFE_OUTPUTS_THUMB_AND_MAIN ||
		axi_ctrl->share_ctrl->operation_mode &
			VFE_OUTPUTS_MAIN_AND_THUMB ||
		axi_ctrl->share_ctrl->operation_mode &
			VFE_OUTPUTS_THUMB_AND_JPEG ||
		axi_ctrl->share_ctrl->operation_mode &
			VFE_OUTPUTS_JPEG_AND_THUMB ||
		axi_ctrl->share_ctrl->operation_mode &
			VFE_OUTPUTS_RAW) {
		if ((axi_ctrl->share_ctrl->outpath.out0.capture_cnt == 0)
				&& (axi_ctrl->share_ctrl->outpath.out1.
				capture_cnt == 0)) {
			uint32_t mode =
				(axi_ctrl->share_ctrl->operation_mode &
				~(VFE_OUTPUTS_RDI0|VFE_OUTPUTS_RDI1|
				  VFE_OUTPUTS_RDI2));
			uint16_t output_mode =
			axi_ctrl->share_ctrl->comp_output_mode &
				~(VFE32_OUTPUT_MODE_TERTIARY1|
				VFE32_OUTPUT_MODE_TERTIARY2|
				  VFE32_OUTPUT_MODE_TERTIARY3);
			if (!axi_ctrl->share_ctrl->dual_enabled)
				msm_camera_io_w_mb(
					CAMIF_COMMAND_STOP_IMMEDIATELY,
					axi_ctrl->share_ctrl->vfebase +
					VFE_CAMIF_COMMAND);
			axi_disable_wm_irq(axi_ctrl->share_ctrl, output_mode);
			axi_disable_irq(axi_ctrl->share_ctrl, mode);
			vfe32_send_isp_msg(&axi_ctrl->subdev,
				axi_ctrl->share_ctrl->vfeFrameId,
				MSG_ID_PIX0_UPDATE_ACK);
			axi_ctrl->share_ctrl->outpath.out0.
				capture_cnt = -1;
			axi_ctrl->share_ctrl->outpath.out1.
				capture_cnt = -1;
			axi_ctrl->share_ctrl->comp_output_mode &=
				(VFE32_OUTPUT_MODE_TERTIARY1|
				VFE32_OUTPUT_MODE_TERTIARY2|
				 VFE32_OUTPUT_MODE_TERTIARY3);
		}
	}

	if (axi_ctrl->share_ctrl->outpath.out2.capture_cnt == 0) {
		axi_ctrl->share_ctrl->comp_output_mode &=
				~VFE32_OUTPUT_MODE_TERTIARY1;
		axi_ctrl->share_ctrl->outpath.out2.capture_cnt = -1;
	}

	if (axi_ctrl->share_ctrl->outpath.out3.capture_cnt == 0) {
		axi_ctrl->share_ctrl->comp_output_mode &=
				~VFE32_OUTPUT_MODE_TERTIARY2;
		axi_ctrl->share_ctrl->outpath.out3.capture_cnt = -1;
	}

	if (axi_ctrl->share_ctrl->outpath.out4.capture_cnt == 0) {
		axi_ctrl->share_ctrl->comp_output_mode &=
				~VFE32_OUTPUT_MODE_TERTIARY3;
		axi_ctrl->share_ctrl->outpath.out4.capture_cnt = -1;
	}
}

static int msm_axi_buf_cfg(struct v4l2_subdev *sd, void __user *arg)
{
	struct msm_camvfe_params *vfe_params =
		(struct msm_camvfe_params *)arg;
	struct msm_vfe_cfg_cmd *cmd = vfe_params->vfe_cfg;
	struct axi_ctrl_t *axi_ctrl = v4l2_get_subdevdata(sd);
	void *data = vfe_params->data;
	int rc = 0;

	if (!axi_ctrl->share_ctrl->vfebase) {
		pr_err("%s: base address unmapped\n", __func__);
		return -EFAULT;
	}

	switch (cmd->cmd_type) {
	case CMD_CONFIG_PING_ADDR: {
		int path = *((int *)cmd->value);
		struct vfe32_output_ch *outch =
			vfe32_get_ch(path, axi_ctrl->share_ctrl);
		outch->ping = *((struct msm_free_buf *)data);
	}
		break;

	case CMD_CONFIG_PONG_ADDR: {
		int path = *((int *)cmd->value);
		struct vfe32_output_ch *outch =
			vfe32_get_ch(path, axi_ctrl->share_ctrl);
		outch->pong = *((struct msm_free_buf *)data);
	}
		break;

	case CMD_CONFIG_FREE_BUF_ADDR: {
		int path = *((int *)cmd->value);
		struct vfe32_output_ch *outch =
			vfe32_get_ch(path, axi_ctrl->share_ctrl);
		outch->free_buf = *((struct msm_free_buf *)data);
	}
		break;
	default:
		pr_err("%s Unsupported AXI Buf config %x ", __func__,
			cmd->cmd_type);
	}
	return rc;
};

static const struct v4l2_subdev_internal_ops msm_vfe_internal_ops;

static long msm_axi_subdev_ioctl(struct v4l2_subdev *sd,
			unsigned int cmd, void *arg)
{
	int rc = -ENOIOCTLCMD;
	switch (cmd) {
	case VIDIOC_MSM_AXI_INIT: {
		uint8_t dual_enabled;
		if (copy_from_user(&dual_enabled,
				(void __user *)(arg),
				sizeof(uint8_t))) {
				rc = -EFAULT;
				break;
		}
		rc = msm_axi_subdev_init(sd, dual_enabled);
		}
		break;
	case VIDIOC_MSM_AXI_LOW_POWER_MODE:
		rc = msm_axi_set_low_power_mode(sd, arg);
		break;

	case VIDIOC_MSM_AXI_CFG:
		rc = msm_axi_config(sd, arg);
		break;
	case VIDIOC_MSM_AXI_IRQ:
		msm_axi_process_irq(sd, arg);
		rc = 0;
		break;
	case VIDIOC_MSM_AXI_BUF_CFG:
		msm_axi_buf_cfg(sd, arg);
		rc = 0;
		break;
	case VIDIOC_MSM_AXI_RELEASE:
		msm_axi_subdev_release(sd);
		rc = 0;
		break;
	case VIDIOC_MSM_AXI_RDI_COUNT_UPDATE: {
		struct rdi_count_msg *msg = (struct rdi_count_msg *)arg;
		struct axi_ctrl_t *axi_ctrl = v4l2_get_subdevdata(sd);
		switch (msg->rdi_interface) {
		case RDI_0:
			axi_ctrl->share_ctrl->rdi0FrameId = msg->count;
			rc = 0;
			break;
		case RDI_1:
			axi_ctrl->share_ctrl->rdi1FrameId = msg->count;
			rc = 0;
			break;
		case RDI_2:
			axi_ctrl->share_ctrl->rdi2FrameId = msg->count;
			rc = 0;
			break;
		default:
			pr_err("%s: Incorrect interface sent\n", __func__);
			rc = -EINVAL;
			break;
		}
		break;
	}
	default:
		pr_err("%s: command %d not found\n", __func__,
						_IOC_NR(cmd));
		break;
	}
	return rc;
}

static const struct v4l2_subdev_core_ops msm_axi_subdev_core_ops = {
	.ioctl = msm_axi_subdev_ioctl,
	.interrupt_service_routine = msm_axi_subdev_isr_routine,
};

static const struct v4l2_subdev_video_ops msm_axi_subdev_video_ops = {
	.s_crystal_freq = msm_axi_subdev_s_crystal_freq,
};

static const struct v4l2_subdev_ops msm_axi_subdev_ops = {
	.core = &msm_axi_subdev_core_ops,
	.video = &msm_axi_subdev_video_ops,
};

static const struct v4l2_subdev_internal_ops msm_axi_internal_ops;

static int __devinit vfe32_probe(struct platform_device *pdev)
{
	int rc = 0;
	struct axi_ctrl_t *axi_ctrl;
	struct vfe32_ctrl_type *vfe32_ctrl;
	struct vfe_share_ctrl_t *share_ctrl;
	struct intr_table_entry irq_req;
	struct msm_cam_subdev_info sd_info;

	CDBG("%s: device id = %d\n", __func__, pdev->id);

	share_ctrl = kzalloc(sizeof(struct vfe_share_ctrl_t), GFP_KERNEL);
	if (!share_ctrl) {
		pr_err("%s: no enough memory\n", __func__);
		return -ENOMEM;
	}

	axi_ctrl = kzalloc(sizeof(struct axi_ctrl_t), GFP_KERNEL);
	if (!axi_ctrl) {
		pr_err("%s: no enough memory\n", __func__);
		kfree(share_ctrl);
		return -ENOMEM;
	}

	vfe32_ctrl = kzalloc(sizeof(struct vfe32_ctrl_type), GFP_KERNEL);
	if (!vfe32_ctrl) {
		pr_err("%s: no enough memory\n", __func__);
		kfree(share_ctrl);
		kfree(axi_ctrl);
		return -ENOMEM;
	}

	share_ctrl->axi_ctrl = axi_ctrl;
	share_ctrl->vfe32_ctrl = vfe32_ctrl;
	axi_ctrl->share_ctrl = share_ctrl;
	vfe32_ctrl->share_ctrl = share_ctrl;
	axi_ctrl->share_ctrl->axi_ref_cnt = 0;
	v4l2_subdev_init(&axi_ctrl->subdev, &msm_axi_subdev_ops);
	axi_ctrl->subdev.internal_ops = &msm_axi_internal_ops;
	axi_ctrl->subdev.flags |= V4L2_SUBDEV_FL_HAS_DEVNODE;
	snprintf(axi_ctrl->subdev.name,
			 sizeof(axi_ctrl->subdev.name), "axi");
	v4l2_set_subdevdata(&axi_ctrl->subdev, axi_ctrl);
	axi_ctrl->pdev = pdev;

	sd_info.sdev_type = AXI_DEV;
	sd_info.sd_index = 0;
	sd_info.irq_num = 0;
	msm_cam_register_subdev_node(&axi_ctrl->subdev, &sd_info);

	media_entity_init(&axi_ctrl->subdev.entity, 0, NULL, 0);
	axi_ctrl->subdev.entity.type = MEDIA_ENT_T_V4L2_SUBDEV;
	axi_ctrl->subdev.entity.group_id = AXI_DEV;
	axi_ctrl->subdev.entity.name = pdev->name;
	axi_ctrl->subdev.entity.revision = axi_ctrl->subdev.devnode->num;

	v4l2_subdev_init(&vfe32_ctrl->subdev, &msm_vfe_subdev_ops);
	vfe32_ctrl->subdev.internal_ops = &msm_vfe_internal_ops;
	vfe32_ctrl->subdev.flags |= V4L2_SUBDEV_FL_HAS_DEVNODE;
	snprintf(vfe32_ctrl->subdev.name,
			 sizeof(vfe32_ctrl->subdev.name), "vfe3.2");
	v4l2_set_subdevdata(&vfe32_ctrl->subdev, vfe32_ctrl);
	platform_set_drvdata(pdev, &vfe32_ctrl->subdev);

	axi_ctrl->vfemem = platform_get_resource_byname(pdev,
					IORESOURCE_MEM, "vfe32");
	if (!axi_ctrl->vfemem) {
		pr_err("%s: no mem resource?\n", __func__);
		rc = -ENODEV;
		goto vfe32_no_resource;
	}
	axi_ctrl->vfeirq = platform_get_resource_byname(pdev,
					IORESOURCE_IRQ, "vfe32");
	if (!axi_ctrl->vfeirq) {
		pr_err("%s: no irq resource?\n", __func__);
		rc = -ENODEV;
		goto vfe32_no_resource;
	}

	axi_ctrl->vfeio = request_mem_region(axi_ctrl->vfemem->start,
		resource_size(axi_ctrl->vfemem), pdev->name);
	if (!axi_ctrl->vfeio) {
		pr_err("%s: no valid mem region\n", __func__);
		rc = -EBUSY;
		goto vfe32_no_resource;
	}

	axi_ctrl->fs_vfe = regulator_get(&pdev->dev, "vdd");
	if (IS_ERR(axi_ctrl->fs_vfe)) {
		pr_err("%s: Regulator get failed %ld\n", __func__,
			PTR_ERR(axi_ctrl->fs_vfe));
		axi_ctrl->fs_vfe = NULL;
	}

	/* Register subdev node before requesting irq since
	 * irq_num is needed by msm_cam_server */
	sd_info.sdev_type = VFE_DEV;
	sd_info.sd_index = 0;
	sd_info.irq_num = axi_ctrl->vfeirq->start;
	msm_cam_register_subdev_node(&vfe32_ctrl->subdev, &sd_info);

	media_entity_init(&vfe32_ctrl->subdev.entity, 0, NULL, 0);
	vfe32_ctrl->subdev.entity.type = MEDIA_ENT_T_V4L2_SUBDEV;
	vfe32_ctrl->subdev.entity.group_id = VFE_DEV;
	vfe32_ctrl->subdev.entity.name = pdev->name;
	vfe32_ctrl->subdev.entity.revision = vfe32_ctrl->subdev.devnode->num;

	/* Request for this device irq from the camera server. If the
	 * IRQ Router is present on this target, the interrupt will be
	 * handled by the camera server and the interrupt service
	 * routine called. If the request_irq call returns ENXIO, then
	 * the IRQ Router hardware is not present on this target. We
	 * have to request for the irq ourselves and register the
	 * appropriate interrupt handler. */
	irq_req.cam_hw_idx       = MSM_CAM_HW_VFE0;
	irq_req.dev_name         = "vfe";
	irq_req.irq_idx          = CAMERA_SS_IRQ_8;
	irq_req.irq_num          = axi_ctrl->vfeirq->start;
	irq_req.is_composite     = 0;
	irq_req.irq_trigger_type = IRQF_TRIGGER_RISING;
	irq_req.num_hwcore       = 1;
	irq_req.subdev_list[0]   = &axi_ctrl->subdev;
	irq_req.data             = (void *)axi_ctrl;
	rc = msm_cam_server_request_irq(&irq_req);
	if (rc == -ENXIO) {
		/* IRQ Router hardware is not present on this hardware.
		 * Request for the IRQ and register the interrupt handler. */
		rc = request_irq(axi_ctrl->vfeirq->start, vfe32_parse_irq,
			IRQF_TRIGGER_RISING, "vfe", axi_ctrl);
		if (rc < 0) {
			release_mem_region(axi_ctrl->vfemem->start,
				resource_size(axi_ctrl->vfemem));
			pr_err("%s: irq request fail\n", __func__);
			rc = -EBUSY;
			goto vfe32_no_resource;
		}
		disable_irq(axi_ctrl->vfeirq->start);
	} else if (rc < 0) {
		pr_err("%s Error registering irq ", __func__);
		goto vfe32_no_resource;
	}

#ifdef CONFIG_MSM_IOMMU
	/*get device context for IOMMU*/
	axi_ctrl->iommu_ctx_imgwr =
		msm_iommu_get_ctx("vfe_imgwr"); /*re-confirm*/
	axi_ctrl->iommu_ctx_misc =
		msm_iommu_get_ctx("vfe_misc"); /*re-confirm*/
	if (!axi_ctrl->iommu_ctx_imgwr || !axi_ctrl->iommu_ctx_misc) {
		release_mem_region(axi_ctrl->vfemem->start,
			resource_size(axi_ctrl->vfemem));
		pr_err("%s: No iommu fw context found\n", __func__);
		rc = -ENODEV;
		goto vfe32_no_resource;
	}
#endif

	tasklet_init(&axi_ctrl->vfe32_tasklet,
		axi32_do_tasklet, (unsigned long)axi_ctrl);

	vfe32_ctrl->pdev = pdev;
	/*disable bayer stats by default*/
	vfe32_ctrl->ver_num.main = 0;
	return 0;

vfe32_no_resource:
	kfree(vfe32_ctrl);
	kfree(axi_ctrl);
	return rc;
}

static struct platform_driver vfe32_driver = {
	.probe = vfe32_probe,
	.driver = {
		.name = MSM_VFE_DRV_NAME,
		.owner = THIS_MODULE,
	},
};

static int __init msm_vfe32_init_module(void)
{
	return platform_driver_register(&vfe32_driver);
}

static void __exit msm_vfe32_exit_module(void)
{
	platform_driver_unregister(&vfe32_driver);
}

module_init(msm_vfe32_init_module);
module_exit(msm_vfe32_exit_module);
MODULE_DESCRIPTION("VFE 3.2 driver");
MODULE_LICENSE("GPL v2");
