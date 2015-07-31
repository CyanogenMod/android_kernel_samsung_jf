/*
 * Copyright (c) 2009-2013, The Linux Foundation. All rights reserved.
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

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/hrtimer.h>
#include <linux/clk.h>
#include <mach/hardware.h>
#include <mach/iommu_domains.h>
#include <mach/iommu.h>
#include <linux/iommu.h>
#include <linux/io.h>
#include <linux/debugfs.h>
#include <linux/fb.h>
#include <linux/msm_mdp.h>
#include <linux/file.h>
#include <linux/android_pmem.h>
#include <linux/major.h>
#include <asm/system.h>
#include <asm/mach-types.h>
#include <linux/semaphore.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <linux/msm_kgsl.h>
#include "mdp.h"
#include "msm_fb.h"
#include "mdp4.h"

struct ion_handle {
	struct kref ref;
	struct ion_client *client;
	struct ion_buffer *buffer;
	struct rb_node node;
	unsigned int kmap_cnt;
	unsigned int iommu_map_cnt;
};

#define VERSION_KEY_MASK	0xFFFFFF00

struct mdp4_overlay_ctrl {
	struct mdp4_overlay_pipe plist[OVERLAY_PIPE_MAX];
	struct mdp4_overlay_pipe *stage[MDP4_MIXER_MAX][MDP4_MIXER_STAGE_MAX];
	struct mdp4_overlay_pipe *baselayer[MDP4_MIXER_MAX];
	struct blend_cfg blend[MDP4_MIXER_MAX][MDP4_MIXER_STAGE_MAX];
	uint32 mixer_cfg[MDP4_MIXER_MAX];
	uint32 flush[MDP4_MIXER_MAX];
	struct iommu_free_list iommu_free[MDP4_MIXER_MAX];
	struct iommu_free_list iommu_free_prev[MDP4_MIXER_MAX];
	uint32 dmap_cfg[5];
	uint32 cs_controller;
	uint32 panel_3d;
	uint32 panel_mode;
	uint32 mixer0_played;
	uint32 mixer1_played;
	uint32 mixer2_played;
} mdp4_overlay_db = {
	.cs_controller = CS_CONTROLLER_0,
	.plist = {
		{
			.pipe_type = OVERLAY_TYPE_RGB,
			.pipe_num = OVERLAY_PIPE_RGB1,
			.pipe_ndx = 1,
		},
		{
			.pipe_type = OVERLAY_TYPE_RGB,
			.pipe_num = OVERLAY_PIPE_RGB2,
			.pipe_ndx = 2,
		},
		{
			.pipe_type = OVERLAY_TYPE_VIDEO,
			.pipe_num = OVERLAY_PIPE_VG1,
			.pipe_ndx = 3,
		},
		{
			.pipe_type = OVERLAY_TYPE_VIDEO,
			.pipe_num = OVERLAY_PIPE_VG2,
			.pipe_ndx = 4,
		},
		{
			.pipe_type = OVERLAY_TYPE_BF,
			.pipe_num = OVERLAY_PIPE_RGB3,
			.pipe_ndx = 5,
			.mixer_num = MDP4_MIXER0,
		},
		{
			.pipe_type = OVERLAY_TYPE_BF,
			.pipe_num = OVERLAY_PIPE_VG3,
			.pipe_ndx = 6,
			.mixer_num = MDP4_MIXER1,
		},
		{
			.pipe_type = OVERLAY_TYPE_BF,
			.pipe_num = OVERLAY_PIPE_VG4,
			.pipe_ndx = 7,
			.mixer_num = MDP4_MIXER2,
		},
	},
};

static DEFINE_MUTEX(iommu_mutex);
static struct mdp4_overlay_ctrl *ctrl = &mdp4_overlay_db;

struct mdp4_overlay_perf {
	u32 mdp_clk_rate;
	u32 use_ov_blt[MDP4_MIXER_MAX];
	u64 mdp_ov_ab_bw[MDP4_MIXER_MAX];
	u64 mdp_ov_ib_bw[MDP4_MIXER_MAX];
	u64 mdp_ab_bw;
	u64 mdp_ib_bw;
	u64 mdp_ab_port0_bw;
	u64 mdp_ib_port0_bw;
	u64 mdp_ab_port1_bw;
	u64 mdp_ib_port1_bw;
	u32 pipe_cnt;
};

struct mdp4_overlay_perf perf_request;
struct mdp4_overlay_perf perf_current;

void  mdp4_overlay_free_base_pipe(struct msm_fb_data_type *mfd)
{
	if (!hdmi_prim_display && mfd->index == 0) {
		if (ctrl->panel_mode & MDP4_PANEL_DSI_VIDEO)
			mdp4_dsi_video_free_base_pipe(mfd);
		else if (ctrl->panel_mode & MDP4_PANEL_DSI_CMD)
			mdp4_dsi_cmd_free_base_pipe(mfd);
		else if (ctrl->panel_mode & MDP4_PANEL_LCDC)
			mdp4_lcdc_free_base_pipe(mfd);
	} else if (hdmi_prim_display || mfd->index == 1) {
		mdp4_dtv_free_base_pipe(mfd);
	}
}

static struct ion_client *display_iclient;
static char *sec_underrun_log_buff;

static int mdp4_map_sec_resource(struct msm_fb_data_type *mfd)
{
	int ret = 0;

	if (!mfd) {
		pr_err("%s: mfd is invalid\n", __func__);
		return -ENODEV;
	}

	pr_debug("%s %d mfd->index=%d,mapped=%d,active=%d\n",
		__func__, __LINE__,
		 mfd->index, mfd->sec_mapped, mfd->sec_active);

	if (mfd->sec_mapped)
		return 0;

	ret = mdp_enable_iommu_clocks();
	if (ret) {
		pr_err("IOMMU clock enabled failed while open");
		return ret;
	}
	ret = msm_ion_secure_heap(ION_HEAP(ION_CP_MM_HEAP_ID));
	if (ret)
		pr_err("ION heap secure failed heap id %d ret %d\n",
			   ION_CP_MM_HEAP_ID, ret);
	else
		mfd->sec_mapped = 1;
	mdp_disable_iommu_clocks();
	return ret;
}

int mdp4_unmap_sec_resource(struct msm_fb_data_type *mfd)
{
	int ret = 0;

	if (!mfd) {
		pr_err("%s: mfd is invalid\n", __func__);
		return -ENODEV;
	}

	if ((mfd->sec_mapped == 0) || (mfd->sec_active))
		return 0;

	pr_debug("%s %d mfd->index=%d,mapped=%d,active=%d\n",
		__func__, __LINE__,
		 mfd->index, mfd->sec_mapped, mfd->sec_active);

	ret = mdp_enable_iommu_clocks();
	if (ret) {
		pr_err("IOMMU clock enabled failed while close\n");
		return ret;
	}
	msm_ion_unsecure_heap(ION_HEAP(ION_CP_MM_HEAP_ID));
	mfd->sec_mapped = 0;
	mdp_disable_iommu_clocks();
	return ret;
}

/*
 * mdp4_overlay_iommu_unmap_freelist()
 * mdp4_overlay_iommu_2freelist()
 * mdp4_overlay_iommu_pipe_free()
 * above three functiosns need to be called from same thread and
 * in order so that no mutex are needed.
 */
void mdp4_overlay_iommu_unmap_freelist(int mixer)
{
	int i;
	struct ion_handle *ihdl;
	struct iommu_free_list *flist, *pflist;

	if (mixer >= MDP4_MIXER_MAX)
		return;

	mutex_lock(&iommu_mutex);
	pflist = &ctrl->iommu_free_prev[mixer];
	flist = &ctrl->iommu_free[mixer];
	pr_debug("%s: mixer=%d fndx=%d %d\n", __func__,
			mixer, pflist->fndx, flist->fndx);
	if (pflist->fndx == 0) {
		goto flist_to_pflist;
	}
	for (i = 0; i < IOMMU_FREE_LIST_MAX; i++) {
		ihdl = pflist->ihdl[i];
		if (ihdl == NULL || ihdl->buffer == NULL || ihdl->client == NULL)
			continue;
		pr_debug("%s: mixer=%d i=%d ihdl=0x%p\n", __func__,
					mixer, i, ihdl);
		xlog(__func__, mixer, i, (int)ihdl, 0, 0);
		ion_unmap_iommu(display_iclient, ihdl, DISPLAY_READ_DOMAIN,
							GEN_POOL);
		mdp4_stat.iommu_unmap++;
		pr_debug("%s: map=%d unmap=%d drop=%d\n", __func__,
			(int)mdp4_stat.iommu_map, (int)mdp4_stat.iommu_unmap,
				(int)mdp4_stat.iommu_drop);
		ion_free(display_iclient, ihdl);
	}

flist_to_pflist:
	/* move flist to pflist*/
	memcpy(pflist, flist, sizeof(*pflist));
	memset(flist, 0, sizeof(*flist));
	mutex_unlock(&iommu_mutex);
}

void mdp4_overlay_iommu_2freelist(int mixer, struct ion_handle *ihdl)
{
	struct iommu_free_list *flist;

	flist = &ctrl->iommu_free[mixer];
	if (flist->fndx >= IOMMU_FREE_LIST_MAX) {
		pr_err("%s: Error, mixer=%d iommu fndx=%d\n",
				__func__, mixer, flist->fndx);
		mdp4_stat.iommu_drop++;
		return;
	}

	pr_debug("%s: add mixer=%d fndx=%d ihdl=0x%p\n", __func__,
				mixer, flist->fndx, ihdl);

	flist->ihdl[flist->fndx++] = ihdl;
}

void mdp4_overlay_iommu_pipe_free(int ndx, int all)
{
	struct mdp4_overlay_pipe *pipe;
	struct mdp4_iommu_pipe_info *iom;
	int plane, mixer;

	pipe = mdp4_overlay_ndx2pipe(ndx);
	if (pipe == NULL)
		return;

	if (pipe->flags & MDP_MEMORY_ID_TYPE_FB) {
		pipe->flags &= ~MDP_MEMORY_ID_TYPE_FB;

		if (pipe->put0_need) {
			fput_light(pipe->srcp0_file, pipe->put0_need);
			pipe->put0_need = 0;
		}
		if (pipe->put1_need) {
			fput_light(pipe->srcp1_file, pipe->put1_need);
			pipe->put1_need = 0;
		}
		if (pipe->put2_need) {
			fput_light(pipe->srcp2_file, pipe->put2_need);
			pipe->put2_need = 0;
		}

		pr_debug("%s: ndx=%d flags=%x put=%d\n", __func__,
			pipe->pipe_ndx, pipe->flags, pipe->put0_need);
		return;
	}

	mutex_lock(&iommu_mutex);
	mixer = pipe->mixer_num;
	iom = &pipe->iommu;
	pr_debug("%s: mixer=%d ndx=%d all=%d\n", __func__,
				mixer, pipe->pipe_ndx, all);
	for (plane = 0; plane < MDP4_MAX_PLANE; plane++) {
		if (iom->prev_ihdl[plane]) {
			mdp4_overlay_iommu_2freelist(mixer,
					iom->prev_ihdl[plane]);
			iom->prev_ihdl[plane] = NULL;
		}
		if (all && iom->ihdl[plane]) {
			mdp4_overlay_iommu_2freelist(mixer, iom->ihdl[plane]);
			iom->ihdl[plane] = NULL;
		}
	}
	mutex_unlock(&iommu_mutex);
}

int mdp4_overlay_iommu_map_buf(int mem_id,
	struct mdp4_overlay_pipe *pipe, unsigned int plane,
	unsigned long *start, unsigned long *len,
	struct ion_handle **srcp_ihdl)
{
	struct mdp4_iommu_pipe_info *iom;
	unsigned long size = 0, map_size = 0;
	int ret;

	if (!display_iclient)
		return -EINVAL;

	*srcp_ihdl = ion_import_dma_buf(display_iclient, mem_id);
	if (IS_ERR_OR_NULL(*srcp_ihdl)) {
		pr_err("ion_import_dma_buf() failed\n");
		return PTR_ERR(*srcp_ihdl);
	}
	pr_debug("%s(): ion_hdl %p, ion_buf %d\n", __func__, *srcp_ihdl, mem_id);
	pr_debug("mixer %u, pipe %u, plane %u\n", pipe->mixer_num,
		pipe->pipe_ndx, plane);

	if(mdp4_overlay_format2type(pipe->src_format) == OVERLAY_TYPE_RGB) {
		ret = ion_handle_get_size(display_iclient, *srcp_ihdl, &size);
		if (ret)
			pr_err("ion_handle_get_size failed with ret %d\n", ret);
		map_size = mdp_iommu_max_map_size;
		if(map_size < size)
			map_size = size;

		if (ion_map_iommu(display_iclient, *srcp_ihdl,
				DISPLAY_READ_DOMAIN, GEN_POOL, SZ_4K, map_size, start,
				len, 0, 0)) {
			ion_free(display_iclient, *srcp_ihdl);
			pr_err("%s(): ion_map_iommu() failed\n",
					__func__);
			return -EINVAL;
		}
	} else {

		if (ion_map_iommu(display_iclient, *srcp_ihdl,
				DISPLAY_READ_DOMAIN, GEN_POOL, SZ_4K, 0, start,
				len, 0, 0)) {
			ion_free(display_iclient, *srcp_ihdl);
			pr_err("%s(): ion_map_iommu() failed\n",
					__func__);
			return -EINVAL;
		}
	}
	mutex_lock(&iommu_mutex);
	iom = &pipe->iommu;
	if (iom->prev_ihdl[plane]) {
		mdp4_overlay_iommu_2freelist(pipe->mixer_num,
						iom->prev_ihdl[plane]);
		mdp4_stat.iommu_drop++;
		pr_err("%s: dropped, ndx=%d plane=%d\n", __func__,
						pipe->pipe_ndx, plane);
		xlog(__func__, pipe->pipe_ndx, -1, -1, -1, -1);
	}
	iom->prev_ihdl[plane] = iom->ihdl[plane];
	iom->ihdl[plane] = *srcp_ihdl;
	mdp4_stat.iommu_map++;

	pr_debug("%s: ndx=%d plane=%d prev=0x%p cur=0x%p start=0x%lx len=%lx\n",
		 __func__, pipe->pipe_ndx, plane, iom->prev_ihdl[plane],
			iom->ihdl[plane], *start, *len);
	xlog(__func__, pipe->pipe_ndx, (int)iom->prev_ihdl[plane],
			(int)iom->ihdl[plane], *start, *len);
	mutex_unlock(&iommu_mutex);
	return 0;
}

static struct mdp4_iommu_pipe_info mdp_iommu[MDP4_MIXER_MAX][OVERLAY_PIPE_MAX];

void mdp4_iommu_unmap(struct mdp4_overlay_pipe *pipe)
{
	struct mdp4_iommu_pipe_info *iom_pipe_info;
	unsigned char i, j;

	if (!display_iclient)
		return;

	for (j = 0; j < OVERLAY_PIPE_MAX; j++) {
		iom_pipe_info = &mdp_iommu[pipe->mixer_num][j];
		for (i = 0; i < MDP4_MAX_PLANE; i++) {
			if (iom_pipe_info->prev_ihdl[i] && iom_pipe_info->prev_ihdl[i]->buffer && iom_pipe_info->prev_ihdl[i]->client) {
				pr_debug("%s(): mixer %u, pipe %u, plane %u, "
					"prev_ihdl %p\n", __func__,
					pipe->mixer_num, j + 1, i,
					iom_pipe_info->prev_ihdl[i]);
				ion_unmap_iommu(display_iclient,
					iom_pipe_info->prev_ihdl[i],
					DISPLAY_READ_DOMAIN, GEN_POOL);
				ion_free(display_iclient,
					iom_pipe_info->prev_ihdl[i]);
				iom_pipe_info->prev_ihdl[i] = NULL;
			}

			if (iom_pipe_info->mark_unmap) {
				if (iom_pipe_info->ihdl[i] && iom_pipe_info->ihdl[i]->buffer && iom_pipe_info->ihdl[i]->client) {
					pr_debug("%s(): MARK, mixer %u, pipe %u, plane %u, "
						"ihdl %p\n", __func__,
						pipe->mixer_num, j + 1, i,
						iom_pipe_info->ihdl[i]);
					ion_unmap_iommu(display_iclient,
						iom_pipe_info->ihdl[i],
						DISPLAY_READ_DOMAIN, GEN_POOL);
					ion_free(display_iclient,
						iom_pipe_info->ihdl[i]);
					iom_pipe_info->ihdl[i] = NULL;
				}
			}
		}
		iom_pipe_info->mark_unmap = 0;
	}
}

int mdp4_overlay_mixer_play(int mixer_num)
{
	if (mixer_num == MDP4_MIXER2)
		return ctrl->mixer2_played;
	else if (mixer_num == MDP4_MIXER1)
		return ctrl->mixer1_played;
	else
		return ctrl->mixer0_played;
}

void mdp4_overlay_panel_3d(int mixer_num, uint32 panel_3d)
{
	ctrl->panel_3d = panel_3d;
}

void mdp4_overlay_panel_mode(int mixer_num, uint32 mode)
{
	ctrl->panel_mode |= mode;
}

void mdp4_overlay_panel_mode_unset(int mixer_num, uint32 mode)
{
	ctrl->panel_mode &= ~mode;
}

uint32 mdp4_overlay_panel_list(void)
{
	return ctrl->panel_mode;
}

int mdp4_overlay_borderfill_supported(void)
{
	return (mdp_rev >= MDP_REV_42);
}

void mdp4_overlay_dmae_cfg(struct msm_fb_data_type *mfd, int atv)
{
	uint32	dmae_cfg_reg;

	if (atv)
		dmae_cfg_reg = DMA_DEFLKR_EN;
	else
		dmae_cfg_reg = 0;

	if (mfd->fb_imgType == MDP_BGR_565)
		dmae_cfg_reg |= DMA_PACK_PATTERN_BGR;
	else
		dmae_cfg_reg |= DMA_PACK_PATTERN_RGB;


	if (mfd->panel_info.bpp == 18) {
		dmae_cfg_reg |= DMA_DSTC0G_6BITS |	/* 666 18BPP */
		    DMA_DSTC1B_6BITS | DMA_DSTC2R_6BITS;
	} else if (mfd->panel_info.bpp == 16) {
		dmae_cfg_reg |= DMA_DSTC0G_6BITS |	/* 565 16BPP */
		    DMA_DSTC1B_5BITS | DMA_DSTC2R_5BITS;
	} else {
		dmae_cfg_reg |= DMA_DSTC0G_8BITS |	/* 888 16BPP */
		    DMA_DSTC1B_8BITS | DMA_DSTC2R_8BITS;
	}

	mdp_pipe_ctrl(MDP_CMD_BLOCK, MDP_BLOCK_POWER_ON, FALSE);

	/* dma2 config register */
	MDP_OUTP(MDP_BASE + 0xb0000, dmae_cfg_reg);
	if (atv) {
		MDP_OUTP(MDP_BASE + 0xb0070, 0xeb0010);
		MDP_OUTP(MDP_BASE + 0xb0074, 0xf00010);
		MDP_OUTP(MDP_BASE + 0xb0078, 0xf00010);
		MDP_OUTP(MDP_BASE + 0xb3000, 0x80);
		MDP_OUTP(MDP_BASE + 0xb3010, 0x1800040);
		MDP_OUTP(MDP_BASE + 0xb3014, 0x1000080);
		MDP_OUTP(MDP_BASE + 0xb4004, 0x67686970);
	} else {
		mdp_vid_quant_set();
	}

	mdp_pipe_ctrl(MDP_CMD_BLOCK, MDP_BLOCK_POWER_OFF, FALSE);
}

void fill_black_screen(bool on, uint8 pipe_num, uint8 mixer_num)
{
	uint32 reg_base        = 0x010000;
	uint32 const_color_reg = reg_base * (pipe_num + 2) + 0x1008;
	uint32 src_fmt_reg     = reg_base * (pipe_num + 2) + 0x50;
	uint32 color           = 0x00000000;
	uint32 temp_src_format = 0x00000000;
	uint8  bit             = pipe_num + 2;

	mdp_pipe_ctrl(MDP_CMD_BLOCK, MDP_BLOCK_POWER_ON, FALSE);

	/* Fill constant color */
	MDP_OUTP(MDP_BASE + const_color_reg, color);

	/* Update source format for pipe */
	temp_src_format = inpdw(MDP_BASE + src_fmt_reg);

	if (on)
		MDP_OUTP(MDP_BASE + src_fmt_reg, temp_src_format | BIT(22));
	else
		MDP_OUTP(MDP_BASE + src_fmt_reg, temp_src_format | (~BIT(22)));

	/* MDP_OVERLAY_REG_FLUSH for pipe*/
	MDP_OUTP(MDP_BASE + 0x18000, BIT(bit) | BIT(mixer_num));
	mdp_pipe_ctrl(MDP_CMD_BLOCK, MDP_BLOCK_POWER_OFF, FALSE);

	return;
}

void mdp4_overlay_dmae_xy(struct mdp4_overlay_pipe *pipe)
{

	mdp_pipe_ctrl(MDP_CMD_BLOCK, MDP_BLOCK_POWER_ON, FALSE);

	MDP_OUTP(MDP_BASE + 0xb0004,
			(pipe->src_height << 16 | pipe->src_width));
	if (pipe->dma_blt_addr) {
		uint32 off, bpp;
#ifdef BLT_RGB565
		bpp = 2; /* overlay ouput is RGB565 */
#else
		bpp = 3; /* overlay ouput is RGB888 */
#endif
		off = 0;
		if (pipe->ov_cnt & 0x01)
			off = pipe->src_height * pipe->src_width * bpp;
		MDP_OUTP(MDP_BASE + 0xb0008, pipe->dma_blt_addr + off);
		/* RGB888, output of overlay blending */
		MDP_OUTP(MDP_BASE + 0xb000c, pipe->src_width * bpp);
	} else {
		/* dma_e source */
		MDP_OUTP(MDP_BASE + 0xb0008, pipe->srcp0_addr);
		MDP_OUTP(MDP_BASE + 0xb000c, pipe->srcp0_ystride);
	}
	/* dma_e dest */
	MDP_OUTP(MDP_BASE + 0xb0010, (pipe->dst_y << 16 | pipe->dst_x));

	mdp_pipe_ctrl(MDP_CMD_BLOCK, MDP_BLOCK_POWER_OFF, FALSE);
}

void mdp4_overlay_dmap_cfg(struct msm_fb_data_type *mfd, int lcdc)
{
	uint32	dma2_cfg_reg;
	uint32 mask, curr;

	dma2_cfg_reg = DMA_DITHER_EN;
#ifdef BLT_RGB565
	/* RGB888 is 0 */
	dma2_cfg_reg |= DMA_BUF_FORMAT_RGB565; /* blt only */
#endif

	if (mfd->fb_imgType == MDP_BGR_565)
		dma2_cfg_reg |= DMA_PACK_PATTERN_BGR;
	else
		dma2_cfg_reg |= DMA_PACK_PATTERN_RGB;


	if ((mfd->panel_info.type == MIPI_CMD_PANEL) ||
		(mfd->panel_info.type == MIPI_VIDEO_PANEL)) {
		dma2_cfg_reg |= DMA_DSTC0G_8BITS |	/* 888 24BPP */
		    DMA_DSTC1B_8BITS | DMA_DSTC2R_8BITS;
	} else if (mfd->panel_info.bpp == 18) {
		dma2_cfg_reg |= DMA_DSTC0G_6BITS |	/* 666 18BPP */
		    DMA_DSTC1B_6BITS | DMA_DSTC2R_6BITS;
	} else if (mfd->panel_info.bpp == 16) {
		dma2_cfg_reg |= DMA_DSTC0G_6BITS |	/* 565 16BPP */
		    DMA_DSTC1B_5BITS | DMA_DSTC2R_5BITS;
	} else {
		dma2_cfg_reg |= DMA_DSTC0G_8BITS |	/* 888 24BPP */
		    DMA_DSTC1B_8BITS | DMA_DSTC2R_8BITS;
	}

	mdp_pipe_ctrl(MDP_CMD_BLOCK, MDP_BLOCK_POWER_ON, FALSE);

#ifndef CONFIG_FB_MSM_LCDC_CHIMEI_WXGA_PANEL
	if (lcdc)
		dma2_cfg_reg |= DMA_PACK_ALIGN_MSB;
#endif

	/* dma2 config register */
	curr = inpdw(MDP_BASE + 0x90000);
	mask = 0x0FFFFFFF;
	dma2_cfg_reg = (dma2_cfg_reg & mask) | (curr & ~mask);
	MDP_OUTP(MDP_BASE + 0x90000, dma2_cfg_reg);
	ctrl->dmap_cfg[0] = dma2_cfg_reg;

	mdp_pipe_ctrl(MDP_CMD_BLOCK, MDP_BLOCK_POWER_OFF, FALSE);
}

/*
 * mdp4_overlay_dmap_xy: called form baselayer only
 */
void mdp4_overlay_dmap_xy(struct mdp4_overlay_pipe *pipe)
{
	uint32 off, bpp;

	if (!in_interrupt())
		mdp_pipe_ctrl(MDP_CMD_BLOCK, MDP_BLOCK_POWER_ON, FALSE);

	if (pipe->dma_blt_addr) {
#ifdef BLT_RGB565
		bpp = 2; /* overlay ouput is RGB565 */
#else
		bpp = 3; /* overlay ouput is RGB888 */
#endif
		off = 0;
		if (pipe->dmap_cnt & 0x01)
			off = pipe->src_height * pipe->src_width * bpp;
		ctrl->dmap_cfg[2] = pipe->dma_blt_addr + off;
		MDP_OUTP(MDP_BASE + 0x90008, pipe->dma_blt_addr + off);
		/* RGB888, output of overlay blending */
		MDP_OUTP(MDP_BASE + 0x9000c, pipe->src_width * bpp);
                ctrl->dmap_cfg[3] = pipe->src_width * bpp;
	} else {
		MDP_OUTP(MDP_BASE + 0x90008, pipe->srcp0_addr);
		ctrl->dmap_cfg[2] = pipe->srcp0_addr;
		MDP_OUTP(MDP_BASE + 0x9000c, pipe->srcp0_ystride);
		ctrl->dmap_cfg[3] = pipe->srcp0_ystride;
	}
	/* dma_p source */
	MDP_OUTP(MDP_BASE + 0x90004,
			(pipe->src_height << 16 | pipe->src_width));
	ctrl->dmap_cfg[1] = (pipe->src_height << 16 | pipe->src_width);

	/* dma_p dest */
	MDP_OUTP(MDP_BASE + 0x90010, (pipe->dst_y << 16 | pipe->dst_x));
	ctrl->dmap_cfg[4] = (pipe->dst_y << 16 | pipe->dst_x);

	if (!in_interrupt())
		mdp_pipe_ctrl(MDP_CMD_BLOCK, MDP_BLOCK_POWER_OFF, FALSE);
}

static void mdp4_overlay_dmap_reconfig(void)
{
        MDP_OUTP(MDP_BASE + 0x90000, ctrl->dmap_cfg[0]);
        MDP_OUTP(MDP_BASE + 0x90004, ctrl->dmap_cfg[1]);
        MDP_OUTP(MDP_BASE + 0x90008, ctrl->dmap_cfg[2]);
        MDP_OUTP(MDP_BASE + 0x9000c, ctrl->dmap_cfg[3]);
        MDP_OUTP(MDP_BASE + 0x90010, ctrl->dmap_cfg[4]);
}


#define MDP4_VG_PHASE_STEP_DEFAULT	0x20000000
#define MDP4_VG_PHASE_STEP_SHIFT	29

static int mdp4_leading_0(uint32 num)
{
	uint32 bit = 0x80000000;
	int i;

	for (i = 0; i < 32; i++) {
		if (bit & num)
			return i;
		bit >>= 1;
	}

	return i;
}

static uint32 mdp4_scale_phase_step(int f_num, uint32 src, uint32 dst)
{
	uint32 val, s;
	int	n;

	n = mdp4_leading_0(src);
	if (n > f_num)
		n = f_num;
	s = src << n;	/* maximum to reduce lose of resolution */
	val = s / dst;
	if (n < f_num) {
		n = f_num - n;
		val <<= n;
		val |= ((s % dst) << n) / dst;
	}

	return val;
}

static void mdp4_scale_setup(struct mdp4_overlay_pipe *pipe)
{
	int scale_w = 0;
	int scale_h = 0;
	int force_pixel_rpt = 0;

	if (pipe->pipe_type == OVERLAY_TYPE_VIDEO) {
		if (pipe->src_h > pipe->dst_h)
			scale_h = 1;
		else if (pipe->src_h < pipe->dst_h)
			scale_h = 2;

		if (pipe->src_w > pipe->dst_w)
			scale_w = 1;
		else if (pipe->src_w < pipe->dst_w)
			scale_w = 2;

		if (scale_h != 0 && scale_w != 0 && scale_h != scale_w)
			force_pixel_rpt = 1;
	}

	pipe->phasex_step = MDP4_VG_PHASE_STEP_DEFAULT;
	pipe->phasey_step = MDP4_VG_PHASE_STEP_DEFAULT;

	if (pipe->dst_h && pipe->src_h != pipe->dst_h) {
		u32 upscale_max;
		upscale_max = (mdp_rev >= MDP_REV_41) ?
			MDP4_REV41_OR_LATER_UP_SCALING_MAX :
			MDP4_REV40_UP_SCALING_MAX;
		if (pipe->dst_h > pipe->src_h * upscale_max)
			return;

		pipe->op_mode |= MDP4_OP_SCALEY_EN;

		if (pipe->pipe_type == OVERLAY_TYPE_VIDEO) {
			if (pipe->flags & MDP_BACKEND_COMPOSITION &&
				pipe->alpha_enable && pipe->dst_h > pipe->src_h)
				pipe->op_mode |= MDP4_OP_SCALEY_PIXEL_RPT;
			else if (force_pixel_rpt && pipe->dst_h > pipe->src_h)
				pipe->op_mode |= MDP4_OP_SCALEY_PIXEL_RPT;
			else if (pipe->dst_h <= (pipe->src_h / 4))
				pipe->op_mode |= MDP4_OP_SCALEY_MN_PHASE;
			else
				pipe->op_mode |= MDP4_OP_SCALEY_FIR;
		} else { /* RGB pipe */
			pipe->op_mode |= MDP4_OP_SCALE_RGB_ENHANCED |
				MDP4_OP_SCALE_RGB_BILINEAR |
				MDP4_OP_SCALE_ALPHA_BILINEAR;
		}

		pipe->phasey_step = mdp4_scale_phase_step(29,
					pipe->src_h, pipe->dst_h);
	}

	if (pipe->dst_w && pipe->src_w != pipe->dst_w) {
		u32 upscale_max;
		upscale_max = (mdp_rev >= MDP_REV_41) ?
			MDP4_REV41_OR_LATER_UP_SCALING_MAX :
			MDP4_REV40_UP_SCALING_MAX;

		if (pipe->dst_w > pipe->src_w * upscale_max)
			return;
		pipe->op_mode |= MDP4_OP_SCALEX_EN;
		if (pipe->pipe_type == OVERLAY_TYPE_VIDEO) {
			if (pipe->flags & MDP_BACKEND_COMPOSITION &&
				pipe->alpha_enable && pipe->dst_w > pipe->src_w)
				pipe->op_mode |= MDP4_OP_SCALEX_PIXEL_RPT;
			else if (force_pixel_rpt && pipe->dst_w > pipe->src_w)
				 pipe->op_mode |= MDP4_OP_SCALEX_PIXEL_RPT;
			else if (pipe->dst_w <= (pipe->src_w / 4))
				pipe->op_mode |= MDP4_OP_SCALEX_MN_PHASE;
			else
				pipe->op_mode |= MDP4_OP_SCALEX_FIR;
		} else { /* RGB pipe */
			pipe->op_mode |= MDP4_OP_SCALE_RGB_ENHANCED |
				MDP4_OP_SCALE_RGB_BILINEAR |
				MDP4_OP_SCALE_ALPHA_BILINEAR;
		}

		pipe->phasex_step = mdp4_scale_phase_step(29,
					pipe->src_w, pipe->dst_w);
	}
}

void mdp4_overlay_solidfill_init(struct mdp4_overlay_pipe *pipe)
{
	char *base;
	uint32 src_size, src_xy, dst_size, dst_xy;
	uint32 format;
	uint32 off;
	int i;

	src_size = ((pipe->src_h << 16) | pipe->src_w);
	src_xy = ((pipe->src_y << 16) | pipe->src_x);
	dst_size = ((pipe->dst_h << 16) | pipe->dst_w);
	dst_xy = ((pipe->dst_y << 16) | pipe->dst_x);

	base = MDP_BASE + MDP4_VIDEO_BASE;
	off = MDP4_VIDEO_OFF;	/* 0x10000 */
	mdp_clk_ctrl(1);
	for(i = 0; i < 4; i++) {	/* 4 pipes */
		format = inpdw(base + 0x50);
		format |= MDP4_FORMAT_SOLID_FILL;
		outpdw(base + 0x0000, src_size);/* MDP_RGB_SRC_SIZE */
		outpdw(base + 0x0004, src_xy);	/* MDP_RGB_SRC_XY */
		outpdw(base + 0x0008, dst_size);/* MDP_RGB_DST_SIZE */
		outpdw(base + 0x000c, dst_xy);	/* MDP_RGB_DST_XY */
		outpdw(base + 0x0050, format);/* MDP_RGB_SRC_FORMAT */
		outpdw(base + 0x1008, 0x0);/* Black */
		base += off;
	}
	/*
	 * keep it at primary
	 * will be picked up at first commit
	 */
	ctrl->flush[MDP4_MIXER0] = 0x3c; /* all pipes */
	mdp_clk_ctrl(0);
}

void mdp4_overlay_rgb_setup(struct mdp4_overlay_pipe *pipe)
{
	char *rgb_base;
	uint32 src_size, src_xy, dst_size, dst_xy;
	uint32 format, pattern;
	uint32 curr, mask;
	uint32 offset = 0;
	int pnum;

	pnum = pipe->pipe_num - OVERLAY_PIPE_RGB1; /* start from 0 */
	rgb_base = MDP_BASE + MDP4_RGB_BASE;
	rgb_base += (MDP4_RGB_OFF * pnum);

	src_size = ((pipe->src_h << 16) | pipe->src_w);
	src_xy = ((pipe->src_y << 16) | pipe->src_x);
	dst_size = ((pipe->dst_h << 16) | pipe->dst_w);
	dst_xy = ((pipe->dst_y << 16) | pipe->dst_x);

	if ((pipe->src_x + pipe->src_w) > 0x7FF) {
		offset += pipe->src_x * pipe->bpp;
		src_xy &= 0xFFFF0000;
	}

	if ((pipe->src_y + pipe->src_h) > 0x7FF) {
		offset += pipe->src_y * pipe->src_width * pipe->bpp;
		src_xy &= 0x0000FFFF;
	}

	format = mdp4_overlay_format(pipe);
	pattern = mdp4_overlay_unpack_pattern(pipe);

#ifdef MDP4_IGC_LUT_ENABLE
	pipe->op_mode |= MDP4_OP_IGC_LUT_EN;
#endif

	mdp4_scale_setup(pipe);

	mdp_pipe_ctrl(MDP_CMD_BLOCK, MDP_BLOCK_POWER_ON, FALSE);

	/* Ensure proper covert matrix loaded when color space swaps */
	curr = inpdw(rgb_base + 0x0058);
	/* Don't touch bits you don't want to configure*/
	mask = 0xFFFEFFFF;
	pipe->op_mode = (pipe->op_mode & mask) | (curr & ~mask);

	outpdw(rgb_base + 0x0000, src_size);	/* MDP_RGB_SRC_SIZE */
	outpdw(rgb_base + 0x0004, src_xy);	/* MDP_RGB_SRC_XY */
	outpdw(rgb_base + 0x0008, dst_size);	/* MDP_RGB_DST_SIZE */
	outpdw(rgb_base + 0x000c, dst_xy);	/* MDP_RGB_DST_XY */

	outpdw(rgb_base + 0x0010, pipe->srcp0_addr + offset);
	outpdw(rgb_base + 0x0040, pipe->srcp0_ystride);

	outpdw(rgb_base + 0x0050, format);/* MDP_RGB_SRC_FORMAT */
	outpdw(rgb_base + 0x0054, pattern);/* MDP_RGB_SRC_UNPACK_PATTERN */
	if (format & MDP4_FORMAT_SOLID_FILL) {
		u32 op_mode = pipe->op_mode;
		op_mode &= ~(MDP4_OP_FLIP_LR + MDP4_OP_SCALEX_EN);
		op_mode &= ~(MDP4_OP_FLIP_UD + MDP4_OP_SCALEY_EN);
		outpdw(rgb_base + 0x0058, op_mode);/* MDP_RGB_OP_MODE */
		outpdw(rgb_base + 0x1008, 0x0);/* Black */
	} else {
		if (pipe->op_mode & MDP4_OP_FLIP_LR && mdp_rev >= MDP_REV_42) {
			/* Enable x-scaling bit to enable LR flip */
			/* for MDP > 4.2 targets */
			pipe->op_mode |= 0x01;
		}
		outpdw(rgb_base + 0x0058, pipe->op_mode);/* MDP_RGB_OP_MODE */
	}
	outpdw(rgb_base + 0x005c, pipe->phasex_step);
	outpdw(rgb_base + 0x0060, pipe->phasey_step);

	mdp_pipe_ctrl(MDP_CMD_BLOCK, MDP_BLOCK_POWER_OFF, FALSE);

	mdp4_stat.pipe[pipe->pipe_num]++;
}


static void mdp4_overlay_vg_get_src_offset(struct mdp4_overlay_pipe *pipe,
	char *vg_base, uint32 *luma_off, uint32 *chroma_off)
{
	uint32 src_xy;
	*luma_off = 0;
	*chroma_off = 0;

	if ((pipe->src_x || pipe->src_y) && (pipe->frame_format ==
		MDP4_FRAME_FORMAT_LINEAR)) {
		src_xy = 0;
		outpdw(vg_base + 0x0004, src_xy);	/* MDP_RGB_SRC_XY */

		switch (pipe->src_format) {
		case MDP_Y_CR_CB_H2V2:
		case MDP_Y_CR_CB_GH2V2:
		case MDP_Y_CB_CR_H2V2:
			*luma_off = pipe->src_x +
				(pipe->src_y * pipe->srcp0_ystride);
			*chroma_off = pipe->src_x / 2 +
				((pipe->src_y / 2) * pipe->srcp1_ystride);
			break;

		case MDP_Y_CBCR_H2V2_TILE:
		case MDP_Y_CRCB_H2V2_TILE:
		case MDP_Y_CBCR_H2V2:
		case MDP_Y_CRCB_H2V2:
			*luma_off = pipe->src_x +
				(pipe->src_y * pipe->srcp0_ystride);
			*chroma_off = pipe->src_x +
				((pipe->src_y / 2) * pipe->srcp1_ystride);
			break;

		case MDP_Y_CRCB_H1V1:
		case MDP_Y_CBCR_H1V1:
			*luma_off = pipe->src_x +
				(pipe->src_y * pipe->srcp0_ystride);
			*chroma_off = pipe->src_x +
				((pipe->src_y * 2) * pipe->srcp1_ystride);
			break;

		case MDP_Y_CRCB_H2V1:
		case MDP_Y_CBCR_H2V1:
		case MDP_Y_CRCB_H1V2:
		case MDP_Y_CBCR_H1V2:
			*luma_off = pipe->src_x +
				(pipe->src_y * pipe->srcp0_ystride);
			*chroma_off = pipe->src_x +
				(pipe->src_y * pipe->srcp1_ystride);
			break;

		case MDP_YCBYCR_H2V1:
		case MDP_YCRYCB_H2V1:
			if (pipe->src_x & 0x1)
				pipe->src_x += 1;
			*luma_off += pipe->src_x * 2 +
				((pipe->src_y * 2) * pipe->srcp0_ystride);
			break;

		case MDP_ARGB_8888:
		case MDP_RGBA_8888:
		case MDP_BGRA_8888:
		case MDP_RGBX_8888:
		case MDP_RGB_565:
		case MDP_BGR_565:
		case MDP_XRGB_8888:
		case MDP_RGB_888:
		case MDP_YCBCR_H1V1:
		case MDP_YCRCB_H1V1:
			*luma_off = (pipe->src_x * pipe->bpp) +
					(pipe->src_y * pipe->srcp0_ystride);
			break;

		default:
			pr_err("%s: fmt %u not supported for adjustment\n",
				__func__, pipe->src_format);
			break;
		}
	}
}

void mdp4_overlay_vg_setup(struct mdp4_overlay_pipe *pipe)
{
	char *vg_base;
	uint32 frame_size, src_size, src_xy, dst_size, dst_xy;
	uint32 format, pattern, luma_offset, chroma_offset;
	uint32 mask;
	int pnum, ptype, i;
	uint32_t block;

	pnum = pipe->pipe_num - OVERLAY_PIPE_VG1; /* start from 0 */
	vg_base = MDP_BASE + MDP4_VIDEO_BASE;
	vg_base += (MDP4_VIDEO_OFF * pnum);

	frame_size = ((pipe->src_height << 16) | pipe->src_width);
	src_size = ((pipe->src_h << 16) | pipe->src_w);
	src_xy = ((pipe->src_y << 16) | pipe->src_x);
	dst_size = ((pipe->dst_h << 16) | pipe->dst_w);
	dst_xy = ((pipe->dst_y << 16) | pipe->dst_x);

	ptype = mdp4_overlay_format2type(pipe->src_format);
	format = mdp4_overlay_format(pipe);
	pattern = mdp4_overlay_unpack_pattern(pipe);

	/* CSC Post Processing enabled? */
	if (pipe->flags & MDP_OVERLAY_PP_CFG_EN) {
		if (pipe->pp_cfg.config_ops & MDP_OVERLAY_PP_CSC_CFG) {
			if (pipe->pp_cfg.csc_cfg.flags & MDP_CSC_FLAG_ENABLE)
				pipe->op_mode |= MDP4_OP_CSC_EN;
			if (pipe->pp_cfg.csc_cfg.flags & MDP_CSC_FLAG_YUV_IN)
				pipe->op_mode |= MDP4_OP_SRC_DATA_YCBCR;
			if (pipe->pp_cfg.csc_cfg.flags & MDP_CSC_FLAG_YUV_OUT)
				pipe->op_mode |= MDP4_OP_DST_DATA_YCBCR;

			mdp4_csc_write(&pipe->pp_cfg.csc_cfg,
				(uint32_t) (vg_base + MDP4_VIDEO_CSC_OFF));

			if (pipe->pipe_num == OVERLAY_PIPE_VG1)
				block = MDP_BLOCK_VG_1;
			else
				block = MDP_BLOCK_VG_2;

			for (i = 0; i < CSC_MAX_BLOCKS; i++) {
				if (block == csc_cfg_matrix[i].block) {
					memcpy(&csc_cfg_matrix[i].csc_data,
					&(pipe->pp_cfg.csc_cfg),
					sizeof(struct mdp_csc_cfg));
					break;
				}
			}
		}
		if (pipe->pp_cfg.config_ops & MDP_OVERLAY_PP_QSEED_CFG) {
			mdp4_qseed_access_cfg(&pipe->pp_cfg.qseed_cfg[0],
							(uint32_t) vg_base);
			mdp4_qseed_access_cfg(&pipe->pp_cfg.qseed_cfg[1],
							(uint32_t) vg_base);
		}
	}
	/* not RGB use VG pipe, pure VG pipe */
	if (ptype != OVERLAY_TYPE_RGB)
		pipe->op_mode |= (MDP4_OP_CSC_EN | MDP4_OP_SRC_DATA_YCBCR);

#ifdef MDP4_IGC_LUT_ENABLE
	pipe->op_mode |= MDP4_OP_IGC_LUT_EN;
#endif

	mdp4_scale_setup(pipe);

	luma_offset = 0;
	chroma_offset = 0;

	if (ptype == OVERLAY_TYPE_RGB) {
		if ((pipe->src_y + pipe->src_h) > 0x7FF) {
			luma_offset = pipe->src_y * pipe->src_width * pipe->bpp;
			src_xy &= 0x0000FFFF;
		}

		if ((pipe->src_x + pipe->src_w) > 0x7FF) {
			luma_offset += pipe->src_x * pipe->bpp;
			src_xy &= 0xFFFF0000;
		}
	}

	mdp_pipe_ctrl(MDP_CMD_BLOCK, MDP_BLOCK_POWER_ON, FALSE);

	outpdw(vg_base + 0x0000, src_size);	/* MDP_RGB_SRC_SIZE */
	outpdw(vg_base + 0x0004, src_xy);	/* MDP_RGB_SRC_XY */
	outpdw(vg_base + 0x0008, dst_size);	/* MDP_RGB_DST_SIZE */
	outpdw(vg_base + 0x000c, dst_xy);	/* MDP_RGB_DST_XY */

	xlog(__func__, (int) pipe->srcp0_addr, pipe->srcp0_ystride, 0, 0, 0);
	if (pipe->frame_format != MDP4_FRAME_FORMAT_LINEAR)
		outpdw(vg_base + 0x0048, frame_size); /* TILE frame size */

	/*
	 * Adjust src X offset to avoid MDP from overfetching pixels
	 * present before the offset. This is required for video
	 * frames coming with unused green pixels along the left margin
	 */
	/* not RGB use VG pipe, pure VG pipe */
	if (ptype != OVERLAY_TYPE_RGB) {
		mdp4_overlay_vg_get_src_offset(pipe, vg_base, &luma_offset,
			&chroma_offset);
	}

	/* luma component plane */
	outpdw(vg_base + 0x0010, pipe->srcp0_addr + luma_offset);

	/* chroma component plane or  planar color 1 */
	outpdw(vg_base + 0x0014, pipe->srcp1_addr + chroma_offset);

	/* planar color 2 */
	outpdw(vg_base + 0x0018, pipe->srcp2_addr + chroma_offset);

	outpdw(vg_base + 0x0040,
			pipe->srcp1_ystride << 16 | pipe->srcp0_ystride);

	outpdw(vg_base + 0x0044,
			pipe->srcp3_ystride << 16 | pipe->srcp2_ystride);

	outpdw(vg_base + 0x0050, format);	/* MDP_RGB_SRC_FORMAT */
	outpdw(vg_base + 0x0054, pattern);	/* MDP_RGB_SRC_UNPACK_PATTERN */
	if (format & MDP4_FORMAT_SOLID_FILL) {
		u32 op_mode = pipe->op_mode;
		op_mode &= ~(MDP4_OP_FLIP_LR + MDP4_OP_SCALEX_EN);
		op_mode &= ~(MDP4_OP_FLIP_UD + MDP4_OP_SCALEY_EN);
		outpdw(vg_base + 0x0058, op_mode);/* MDP_RGB_OP_MODE */
	} else
		outpdw(vg_base + 0x0058, pipe->op_mode);/* MDP_RGB_OP_MODE */
	outpdw(vg_base + 0x005c, pipe->phasex_step);
	outpdw(vg_base + 0x0060, pipe->phasey_step);

	if (pipe->op_mode & MDP4_OP_DITHER_EN) {
		outpdw(vg_base + 0x0068,
			pipe->r_bit << 4 | pipe->b_bit << 2 | pipe->g_bit);
	}

	if (mdp_rev > MDP_REV_41) {
		/* mdp chip select controller */
		mask = 0;
		if (pipe->pipe_num == OVERLAY_PIPE_VG1)
			mask = 0x020; /* bit 5 */
		else if (pipe->pipe_num == OVERLAY_PIPE_VG2)
			mask = 0x02000; /* bit 13 */
		if (mask) {
			if (pipe->op_mode & MDP4_OP_SCALEY_MN_PHASE)
				ctrl->cs_controller &= ~mask;
			else
				ctrl->cs_controller |= mask;
			/* NOT double buffered */
			outpdw(MDP_BASE + 0x00c0, ctrl->cs_controller);
		}
	}


	mdp_pipe_ctrl(MDP_CMD_BLOCK, MDP_BLOCK_POWER_OFF, FALSE);

	mdp4_stat.pipe[pipe->pipe_num]++;
}

int mdp4_overlay_format2type(uint32 format)
{
	switch (format) {
	case MDP_RGB_565:
	case MDP_RGB_888:
	case MDP_BGR_565:
	case MDP_XRGB_8888:
	case MDP_ARGB_8888:
	case MDP_RGBA_8888:
	case MDP_BGRA_8888:
	case MDP_RGBX_8888:
		return OVERLAY_TYPE_RGB;
	case MDP_YCBYCR_H2V1:
	case MDP_YCRYCB_H2V1:
	case MDP_Y_CRCB_H2V1:
	case MDP_Y_CBCR_H2V1:
	case MDP_Y_CRCB_H1V2:
	case MDP_Y_CBCR_H1V2:
	case MDP_Y_CRCB_H2V2:
	case MDP_Y_CBCR_H2V2:
	case MDP_Y_CBCR_H2V2_TILE:
	case MDP_Y_CRCB_H2V2_TILE:
	case MDP_Y_CR_CB_H2V2:
	case MDP_Y_CR_CB_GH2V2:
	case MDP_Y_CB_CR_H2V2:
	case MDP_Y_CRCB_H1V1:
	case MDP_Y_CBCR_H1V1:
	case MDP_YCRCB_H1V1:
	case MDP_YCBCR_H1V1:
		return OVERLAY_TYPE_VIDEO;
	case MDP_RGB_BORDERFILL:
		return OVERLAY_TYPE_BF;
	default:
		mdp4_stat.err_format++;
		return -ERANGE;
	}

}

#define C3_ALPHA	3	/* alpha */
#define C2_R_Cr		2	/* R/Cr */
#define C1_B_Cb		1	/* B/Cb */
#define C0_G_Y		0	/* G/luma */
#define YUV_444_MAX_WIDTH		1280	/* Max width for YUV 444*/

int mdp4_overlay_format2pipe(struct mdp4_overlay_pipe *pipe)
{
	switch (pipe->src_format) {
	case MDP_RGB_565:
		pipe->frame_format = MDP4_FRAME_FORMAT_LINEAR;
		pipe->fetch_plane = OVERLAY_PLANE_INTERLEAVED;
		pipe->a_bit = 0;
		pipe->r_bit = 1;	/* R, 5 bits */
		pipe->b_bit = 1;	/* B, 5 bits */
		pipe->g_bit = 2;	/* G, 6 bits */
		pipe->alpha_enable = 0;
		pipe->unpack_tight = 1;
		pipe->unpack_align_msb = 0;
		pipe->unpack_count = 2;
		pipe->element2 = C2_R_Cr;	/* R */
		pipe->element1 = C0_G_Y;	/* G */
		pipe->element0 = C1_B_Cb;	/* B */
		pipe->bpp = 2;	/* 2 bpp */
		pipe->chroma_sample = MDP4_CHROMA_RGB;
		break;
	case MDP_RGB_888:
		pipe->frame_format = MDP4_FRAME_FORMAT_LINEAR;
		pipe->fetch_plane = OVERLAY_PLANE_INTERLEAVED;
		pipe->a_bit = 0;
		pipe->r_bit = 3;	/* R, 8 bits */
		pipe->b_bit = 3;	/* B, 8 bits */
		pipe->g_bit = 3;	/* G, 8 bits */
		pipe->alpha_enable = 0;
		pipe->unpack_tight = 1;
		pipe->unpack_align_msb = 0;
		pipe->unpack_count = 2;
		pipe->element2 = C1_B_Cb;	/* B */
		pipe->element1 = C0_G_Y;	/* G */
		pipe->element0 = C2_R_Cr;	/* R */
		pipe->bpp = 3;	/* 3 bpp */
		pipe->chroma_sample = MDP4_CHROMA_RGB;
		break;
	case MDP_BGR_565:
		pipe->frame_format = MDP4_FRAME_FORMAT_LINEAR;
		pipe->fetch_plane = OVERLAY_PLANE_INTERLEAVED;
		pipe->a_bit = 0;
		pipe->r_bit = 1;	/* R, 5 bits */
		pipe->b_bit = 1;	/* B, 5 bits */
		pipe->g_bit = 2;	/* G, 6 bits */
		pipe->alpha_enable = 0;
		pipe->unpack_tight = 1;
		pipe->unpack_align_msb = 0;
		pipe->unpack_count = 2;
		pipe->element2 = C1_B_Cb;	/* B */
		pipe->element1 = C0_G_Y;	/* G */
		pipe->element0 = C2_R_Cr;	/* R */
		pipe->bpp = 2;	/* 2 bpp */
		pipe->chroma_sample = MDP4_CHROMA_RGB;
		break;
	case MDP_XRGB_8888:
		pipe->frame_format = MDP4_FRAME_FORMAT_LINEAR;
		pipe->fetch_plane = OVERLAY_PLANE_INTERLEAVED;
		pipe->a_bit = 3;	/* alpha, 4 bits */
		pipe->r_bit = 3;	/* R, 8 bits */
		pipe->b_bit = 3;	/* B, 8 bits */
		pipe->g_bit = 3;	/* G, 8 bits */
		pipe->alpha_enable = 0;
		pipe->unpack_tight = 1;
		pipe->unpack_align_msb = 0;
		pipe->unpack_count = 3;
		pipe->element3 = C1_B_Cb;	/* B */
		pipe->element2 = C0_G_Y;	/* G */
		pipe->element1 = C2_R_Cr;	/* R */
		pipe->element0 = C3_ALPHA;	/* alpha */
		pipe->bpp = 4;		/* 4 bpp */
		pipe->chroma_sample = MDP4_CHROMA_RGB;
		break;
	case MDP_ARGB_8888:
		pipe->frame_format = MDP4_FRAME_FORMAT_LINEAR;
		pipe->fetch_plane = OVERLAY_PLANE_INTERLEAVED;
		pipe->a_bit = 3;	/* alpha, 4 bits */
		pipe->r_bit = 3;	/* R, 8 bits */
		pipe->b_bit = 3;	/* B, 8 bits */
		pipe->g_bit = 3;	/* G, 8 bits */
		pipe->alpha_enable = 1;
		pipe->unpack_tight = 1;
		pipe->unpack_align_msb = 0;
		pipe->unpack_count = 3;
		pipe->element3 = C1_B_Cb;	/* B */
		pipe->element2 = C0_G_Y;	/* G */
		pipe->element1 = C2_R_Cr;	/* R */
		pipe->element0 = C3_ALPHA;	/* alpha */
		pipe->bpp = 4;		/* 4 bpp */
		pipe->chroma_sample = MDP4_CHROMA_RGB;
		break;
	case MDP_RGBA_8888:
		pipe->frame_format = MDP4_FRAME_FORMAT_LINEAR;
		pipe->fetch_plane = OVERLAY_PLANE_INTERLEAVED;
		pipe->a_bit = 3;	/* alpha, 4 bits */
		pipe->r_bit = 3;	/* R, 8 bits */
		pipe->b_bit = 3;	/* B, 8 bits */
		pipe->g_bit = 3;	/* G, 8 bits */
		pipe->alpha_enable = 1;
		pipe->unpack_tight = 1;
		pipe->unpack_align_msb = 0;
		pipe->unpack_count = 3;
		pipe->element3 = C3_ALPHA;	/* alpha */
		pipe->element2 = C1_B_Cb;	/* B */
		pipe->element1 = C0_G_Y;	/* G */
		pipe->element0 = C2_R_Cr;	/* R */
		pipe->bpp = 4;		/* 4 bpp */
		pipe->chroma_sample = MDP4_CHROMA_RGB;
		break;
	case MDP_RGBX_8888:
		pipe->frame_format = MDP4_FRAME_FORMAT_LINEAR;
		pipe->fetch_plane = OVERLAY_PLANE_INTERLEAVED;
		pipe->a_bit = 3;
		pipe->r_bit = 3;	/* R, 8 bits */
		pipe->b_bit = 3;	/* B, 8 bits */
		pipe->g_bit = 3;	/* G, 8 bits */
		pipe->alpha_enable = 0;
		pipe->unpack_tight = 1;
		pipe->unpack_align_msb = 0;
		pipe->unpack_count = 3;
		pipe->element3 = C3_ALPHA;	/* alpha */
		pipe->element2 = C1_B_Cb;	/* B */
		pipe->element1 = C0_G_Y;	/* G */
		pipe->element0 = C2_R_Cr;	/* R */
		pipe->bpp = 4;		/* 4 bpp */
		pipe->chroma_sample = MDP4_CHROMA_RGB;
		break;
	case MDP_BGRA_8888:
		pipe->frame_format = MDP4_FRAME_FORMAT_LINEAR;
		pipe->fetch_plane = OVERLAY_PLANE_INTERLEAVED;
		pipe->a_bit = 3;	/* alpha, 4 bits */
		pipe->r_bit = 3;	/* R, 8 bits */
		pipe->b_bit = 3;	/* B, 8 bits */
		pipe->g_bit = 3;	/* G, 8 bits */
		pipe->alpha_enable = 1;
		pipe->unpack_tight = 1;
		pipe->unpack_align_msb = 0;
		pipe->unpack_count = 3;
		pipe->element3 = C3_ALPHA;	/* alpha */
		pipe->element2 = C2_R_Cr;	/* R */
		pipe->element1 = C0_G_Y;	/* G */
		pipe->element0 = C1_B_Cb;	/* B */
		pipe->bpp = 4;		/* 4 bpp */
		pipe->chroma_sample = MDP4_CHROMA_RGB;
		break;
	case MDP_YCBYCR_H2V1:
	case MDP_YCRYCB_H2V1:
		pipe->frame_format = MDP4_FRAME_FORMAT_LINEAR;
		pipe->fetch_plane = OVERLAY_PLANE_INTERLEAVED;
		pipe->a_bit = 0;	/* alpha, 4 bits */
		pipe->r_bit = 3;	/* R, 8 bits */
		pipe->b_bit = 3;	/* B, 8 bits */
		pipe->g_bit = 3;	/* G, 8 bits */
		pipe->alpha_enable = 0;
		pipe->unpack_tight = 1;
		pipe->unpack_align_msb = 0;
		pipe->unpack_count = 3;
		if (pipe->src_format == MDP_YCRYCB_H2V1) {
			pipe->element3 = C0_G_Y;	/* G */
			pipe->element2 = C2_R_Cr;	/* R */
			pipe->element1 = C0_G_Y;	/* G */
			pipe->element0 = C1_B_Cb;	/* B */
		} else if (pipe->src_format == MDP_YCBYCR_H2V1) {
			pipe->element3 = C0_G_Y;	/* G */
			pipe->element2 = C1_B_Cb;	/* B */
			pipe->element1 = C0_G_Y;	/* G */
			pipe->element0 = C2_R_Cr;	/* R */
		}
		pipe->bpp = 2;		/* 2 bpp */
		pipe->chroma_sample = MDP4_CHROMA_H2V1;
		break;
	case MDP_Y_CRCB_H2V1:
	case MDP_Y_CBCR_H2V1:
	case MDP_Y_CRCB_H1V2:
	case MDP_Y_CBCR_H1V2:
	case MDP_Y_CRCB_H2V2:
	case MDP_Y_CBCR_H2V2:
	case MDP_Y_CRCB_H1V1:
	case MDP_Y_CBCR_H1V1:
		pipe->frame_format = MDP4_FRAME_FORMAT_LINEAR;
		pipe->fetch_plane = OVERLAY_PLANE_PSEUDO_PLANAR;
		pipe->a_bit = 0;
		pipe->r_bit = 3;	/* R, 8 bits */
		pipe->b_bit = 3;	/* B, 8 bits */
		pipe->g_bit = 3;	/* G, 8 bits */
		pipe->alpha_enable = 0;
		pipe->unpack_tight = 1;
		pipe->unpack_align_msb = 0;
		pipe->unpack_count = 1;		/* 2 */
		if (pipe->src_format == MDP_Y_CRCB_H2V1) {
			pipe->element1 = C1_B_Cb;
			pipe->element0 = C2_R_Cr;
			pipe->chroma_sample = MDP4_CHROMA_H2V1;
		} else if (pipe->src_format == MDP_Y_CRCB_H1V1) {
			pipe->element1 = C1_B_Cb;
			pipe->element0 = C2_R_Cr;
			if (pipe->src_width > YUV_444_MAX_WIDTH)
				pipe->chroma_sample = MDP4_CHROMA_H1V2;
			else
				pipe->chroma_sample = MDP4_CHROMA_RGB;
		} else if (pipe->src_format == MDP_Y_CBCR_H2V1) {
			pipe->element1 = C2_R_Cr;
			pipe->element0 = C1_B_Cb;
			pipe->chroma_sample = MDP4_CHROMA_H2V1;
		} else if (pipe->src_format == MDP_Y_CBCR_H1V1) {
			pipe->element1 = C2_R_Cr;
			pipe->element0 = C1_B_Cb;
			if (pipe->src_width > YUV_444_MAX_WIDTH)
				pipe->chroma_sample = MDP4_CHROMA_H1V2;
			else
				pipe->chroma_sample = MDP4_CHROMA_RGB;
		} else if (pipe->src_format == MDP_Y_CRCB_H1V2) {
			pipe->element1 = C1_B_Cb;
			pipe->element0 = C2_R_Cr;
			pipe->chroma_sample = MDP4_CHROMA_H1V2;
		} else if (pipe->src_format == MDP_Y_CBCR_H1V2) {
			pipe->element1 = C2_R_Cr;
			pipe->element0 = C1_B_Cb;
			pipe->chroma_sample = MDP4_CHROMA_H1V2;
		} else if (pipe->src_format == MDP_Y_CRCB_H2V2) {
			pipe->element1 = C1_B_Cb;
			pipe->element0 = C2_R_Cr;
			pipe->chroma_sample = MDP4_CHROMA_420;
		} else if (pipe->src_format == MDP_Y_CBCR_H2V2) {
			pipe->element1 = C2_R_Cr;
			pipe->element0 = C1_B_Cb;
			pipe->chroma_sample = MDP4_CHROMA_420;
		}
		pipe->bpp = 2;	/* 2 bpp */
		break;
	case MDP_Y_CBCR_H2V2_TILE:
	case MDP_Y_CRCB_H2V2_TILE:
		pipe->frame_format = MDP4_FRAME_FORMAT_VIDEO_SUPERTILE;
		pipe->fetch_plane = OVERLAY_PLANE_PSEUDO_PLANAR;
		pipe->a_bit = 0;
		pipe->r_bit = 3;	/* R, 8 bits */
		pipe->b_bit = 3;	/* B, 8 bits */
		pipe->g_bit = 3;	/* G, 8 bits */
		pipe->alpha_enable = 0;
		pipe->unpack_tight = 1;
		pipe->unpack_align_msb = 0;
		pipe->unpack_count = 1;		/* 2 */
		if (pipe->src_format == MDP_Y_CRCB_H2V2_TILE) {
			pipe->element1 = C1_B_Cb;	/* B */
			pipe->element0 = C2_R_Cr;	/* R */
			pipe->chroma_sample = MDP4_CHROMA_420;
		} else if (pipe->src_format == MDP_Y_CBCR_H2V2_TILE) {
			pipe->element1 = C2_R_Cr;	/* R */
			pipe->element0 = C1_B_Cb;	/* B */
			pipe->chroma_sample = MDP4_CHROMA_420;
		}
		pipe->bpp = 2;	/* 2 bpp */
		break;
	case MDP_Y_CR_CB_H2V2:
	case MDP_Y_CR_CB_GH2V2:
	case MDP_Y_CB_CR_H2V2:
		pipe->frame_format = MDP4_FRAME_FORMAT_LINEAR;
		pipe->fetch_plane = OVERLAY_PLANE_PLANAR;
		pipe->a_bit = 0;
		pipe->r_bit = 3;	/* R, 8 bits */
		pipe->b_bit = 3;	/* B, 8 bits */
		pipe->g_bit = 3;	/* G, 8 bits */
		pipe->alpha_enable = 0;
		pipe->chroma_sample = MDP4_CHROMA_420;
		pipe->bpp = 2;	/* 2 bpp */
		break;
	case MDP_YCBCR_H1V1:
	case MDP_YCRCB_H1V1:
		pipe->frame_format = MDP4_FRAME_FORMAT_LINEAR;
		pipe->fetch_plane = OVERLAY_PLANE_INTERLEAVED;
		pipe->a_bit = 0;
		pipe->r_bit = 3;    /* R, 8 bits */
		pipe->b_bit = 3;    /* B, 8 bits */
		pipe->g_bit = 3;    /* G, 8 bits */
		pipe->alpha_enable = 0;
		pipe->unpack_tight = 1;
		pipe->unpack_align_msb = 0;
		pipe->unpack_count = 2;
		pipe->element0 = C0_G_Y;    /* G */
		if (pipe->src_format == MDP_YCRCB_H1V1) {
			pipe->element1 = C2_R_Cr; /* R */
			pipe->element2 = C1_B_Cb; /* B */
		} else {
			pipe->element1 = C1_B_Cb;   /* B */
			pipe->element2 = C2_R_Cr;   /* R */
		}
		pipe->bpp = 3;  /* 3 bpp */
	case MDP_RGB_BORDERFILL:
		pipe->alpha_enable = 0;
		pipe->alpha = 0;
		break;
	default:
		/* not likely */
		mdp4_stat.err_format++;
		return -ERANGE;
	}

	return 0;
}

/*
 * color_key_convert: output with 12 bits color key
 */
static uint32 color_key_convert(int start, int num, uint32 color)
{
	uint32 data;

	data = (color >> start) & ((1 << num) - 1);

	/* convert to 8 bits */
	if (num == 5)
		data = ((data << 3) | (data >> 2));
	else if (num == 6)
		data = ((data << 2) | (data >> 4));

	/* convert 8 bits to 12 bits */
	data = (data << 4) | (data >> 4);

	return data;
}

void transp_color_key(int format, uint32 transp,
			uint32 *c0, uint32 *c1, uint32 *c2)
{
	int b_start, g_start, r_start;
	int b_num, g_num, r_num;

	switch (format) {
	case MDP_RGB_565:
		b_start = 0;
		g_start = 5;
		r_start = 11;
		r_num = 5;
		g_num = 6;
		b_num = 5;
		break;
	case MDP_RGB_888:
	case MDP_XRGB_8888:
	case MDP_ARGB_8888:
	case MDP_BGRA_8888:
		b_start = 0;
		g_start = 8;
		r_start = 16;
		r_num = 8;
		g_num = 8;
		b_num = 8;
		break;
	case MDP_RGBA_8888:
	case MDP_RGBX_8888:
		b_start = 16;
		g_start = 8;
		r_start = 0;
		r_num = 8;
		g_num = 8;
		b_num = 8;
		break;
	case MDP_BGR_565:
		b_start = 11;
		g_start = 5;
		r_start = 0;
		r_num = 5;
		g_num = 6;
		b_num = 5;
		break;
	case MDP_Y_CB_CR_H2V2:
	case MDP_Y_CBCR_H2V2:
	case MDP_Y_CBCR_H2V1:
	case MDP_YCBCR_H1V1:
		b_start = 8;
		g_start = 16;
		r_start = 0;
		r_num = 8;
		g_num = 8;
		b_num = 8;
		break;
	case MDP_Y_CR_CB_H2V2:
	case MDP_Y_CR_CB_GH2V2:
	case MDP_Y_CRCB_H2V2:
	case MDP_Y_CRCB_H2V1:
	case MDP_Y_CRCB_H1V2:
	case MDP_Y_CBCR_H1V2:
	case MDP_Y_CRCB_H1V1:
	case MDP_Y_CBCR_H1V1:
	case MDP_YCRCB_H1V1:
		b_start = 0;
		g_start = 16;
		r_start = 8;
		r_num = 8;
		g_num = 8;
		b_num = 8;
		break;
	default:
		b_start = 0;
		g_start = 8;
		r_start = 16;
		r_num = 8;
		g_num = 8;
		b_num = 8;
		break;
	}

	*c0 = color_key_convert(g_start, g_num, transp);
	*c1 = color_key_convert(b_start, b_num, transp);
	*c2 = color_key_convert(r_start, r_num, transp);
}

uint32 mdp4_overlay_format(struct mdp4_overlay_pipe *pipe)
{
	uint32	format;

	format = 0;

	if (pipe->solid_fill)
		format |= MDP4_FORMAT_SOLID_FILL;

	if (pipe->unpack_align_msb)
		format |= MDP4_FORMAT_UNPACK_ALIGN_MSB;

	if (pipe->unpack_tight)
		format |= MDP4_FORMAT_UNPACK_TIGHT;

	if (pipe->alpha_enable)
		format |= MDP4_FORMAT_ALPHA_ENABLE;

	if (pipe->flags & MDP_SOURCE_ROTATED_90)
		format |= MDP4_FORMAT_90_ROTATED;
	format |= (pipe->unpack_count << 13);
	format |= ((pipe->bpp - 1) << 9);
	format |= (pipe->a_bit << 6);
	format |= (pipe->r_bit << 4);
	format |= (pipe->b_bit << 2);
	format |= pipe->g_bit;

	format |= (pipe->frame_format << 29);

	/* video/graphic */
	format |= (pipe->fetch_plane << 19);
	format |= (pipe->chroma_site << 28);
	format |= (pipe->chroma_sample << 26);

	return format;
}

uint32 mdp4_overlay_unpack_pattern(struct mdp4_overlay_pipe *pipe)
{
	return (pipe->element3 << 24) | (pipe->element2 << 16) |
			(pipe->element1 << 8) | pipe->element0;
}

/*
 * mdp4_overlayproc_cfg: only be called from base layer
 */
void mdp4_overlayproc_cfg(struct mdp4_overlay_pipe *pipe)
{
	uint32 data, intf;
	char *overlay_base;
	uint32 curr;

	intf = 0;
	if (pipe->mixer_num == MDP4_MIXER2)
		overlay_base = MDP_BASE + MDP4_OVERLAYPROC2_BASE;
	else if (pipe->mixer_num == MDP4_MIXER1) {
		overlay_base = MDP_BASE + MDP4_OVERLAYPROC1_BASE;/* 0x18000 */
		intf = inpdw(MDP_BASE + 0x0038); /* MDP_DISP_INTF_SEL */
		intf >>= 4;
		intf &= 0x03;
	} else
		overlay_base = MDP_BASE + MDP4_OVERLAYPROC0_BASE;/* 0x10000 */

	if (!in_interrupt())
		mdp_pipe_ctrl(MDP_CMD_BLOCK, MDP_BLOCK_POWER_ON, FALSE);

	/*
	 * BLT support both primary and external external
	 */
	if (pipe->ov_blt_addr) {
		int off, bpp;
#ifdef BLT_RGB565
		bpp = 2;  /* overlay ouput is RGB565 */
#else
		bpp = 3;  /* overlay ouput is RGB888 */
#endif
		data = pipe->src_height;
		data <<= 16;
		data |= pipe->src_width;
		outpdw(overlay_base + 0x0008, data); /* ROI, height + width */
		if (pipe->mixer_num == MDP4_MIXER0 ||
		    pipe->mixer_num == MDP4_MIXER1) {
			off = 0;
			if (pipe->ov_cnt & 0x01)
				off = pipe->src_height * pipe->src_width * bpp;

			outpdw(overlay_base + 0x000c, pipe->ov_blt_addr + off);
			/* overlay ouput is RGB888 */
			outpdw(overlay_base + 0x0010, pipe->src_width * bpp);
			outpdw(overlay_base + 0x001c, pipe->ov_blt_addr + off);
			/* MDDI - BLT + on demand */
			outpdw(overlay_base + 0x0004, 0x08);

			curr = inpdw(overlay_base + 0x0014);
			curr &= 0x4;
#ifdef BLT_RGB565
			outpdw(overlay_base + 0x0014, curr | 0x1); /* RGB565 */
#else
			outpdw(overlay_base + 0x0014, curr | 0x0); /* RGB888 */
#endif
		} else if (pipe->mixer_num == MDP4_MIXER2) {
			if (ctrl->panel_mode & MDP4_PANEL_WRITEBACK) {
				off = 0;
				bpp = 1;
				if (pipe->ov_cnt & 0x01)
					off = pipe->src_height *
							pipe->src_width * bpp;

				outpdw(overlay_base + 0x000c,
						pipe->ov_blt_addr + off);
				/* overlay ouput is RGB888 */
				outpdw(overlay_base + 0x0010,
					((pipe->src_width << 16) |
					 pipe->src_width));
				outpdw(overlay_base + 0x001c,
						pipe->ov_blt_addr + off);
				off = pipe->src_height * pipe->src_width;
				/* align chroma to 2k address */
				off = (off + 2047) & ~2047;
				/* UV plane adress */
				outpdw(overlay_base + 0x0020,
						pipe->ov_blt_addr + off);
				/* MDDI - BLT + on demand */
				outpdw(overlay_base + 0x0004, 0x08);
				/* pseudo planar + writeback */
				curr = inpdw(overlay_base + 0x0014);
				curr &= 0x4;
				outpdw(overlay_base + 0x0014, curr | 0x012);
				/* rgb->yuv */
				outpdw(overlay_base + 0x0200, 0x05);
			}
		}
	} else {
		data = pipe->src_height;
		data <<= 16;
		data |= pipe->src_width;
		outpdw(overlay_base + 0x0008, data); /* ROI, height + width */
		outpdw(overlay_base + 0x000c, pipe->srcp0_addr);
		outpdw(overlay_base + 0x0010, pipe->srcp0_ystride);
		outpdw(overlay_base + 0x0004, 0x01); /* directout */
	}

	if (pipe->mixer_num == MDP4_MIXER1) {
		if (intf == TV_INTF) {
			curr = inpdw(overlay_base + 0x0014);
			curr &= 0x4;
			outpdw(overlay_base + 0x0014, 0x02); /* yuv422 */
			/* overlay1 CSC config */
			outpdw(overlay_base + 0x0200, 0x05); /* rgb->yuv */
		}
	}

#ifdef MDP4_IGC_LUT_ENABLE
	curr = inpdw(overlay_base + 0x0014);
	curr &= ~0x4;
	outpdw(overlay_base + 0x0014, curr | 0x4);	/* GC_LUT_EN, 888 */
#endif

	if (!in_interrupt())
		mdp_pipe_ctrl(MDP_CMD_BLOCK, MDP_BLOCK_POWER_OFF, FALSE);
}

int mdp4_overlay_pipe_staged(struct mdp4_overlay_pipe *pipe)
{
	uint32 data, mask;
	int mixer;

	mixer = pipe->mixer_num;
	data = ctrl->mixer_cfg[mixer];

	mask = 0x0f;
	mask <<= (4 * pipe->pipe_num);
	data &= mask;

	return data;
}

int mdp4_mixer_info(int mixer_num, struct mdp_mixer_info *info)
{

	int ndx, cnt;
	struct mdp4_overlay_pipe *pipe;

	if (mixer_num > MDP4_MIXER_MAX)
		return -ENODEV;

	cnt = 0;
	ndx = MDP4_MIXER_STAGE_BASE;
	for ( ; ndx < MDP4_MIXER_STAGE_MAX; ndx++) {
		pipe = &ctrl->plist[ndx];
		if (pipe == NULL)
			continue;

		if (!pipe->pipe_used)
			continue;

		info->z_order = pipe->mixer_stage - MDP4_MIXER_STAGE0;
		/* z_order == -1, means base layer */
		info->ptype = pipe->pipe_type;
		info->pnum = pipe->pipe_num;
		info->pndx = pipe->pipe_ndx;
		info->mixer_num = pipe->mixer_num;
		info++;
		cnt++;
	}
	return cnt;
}

void mdp4_mixer_reset(int mixer)
{
	uint32 data, data1, mask;
	int i, ndx, min, max, bit;

	mdp_clk_ctrl(1);
	/* MDP_LAYERMIXER_IN_CFG, shard by both mixer 0 and 1  */
	data = inpdw(MDP_BASE + 0x10100);
	data1 = data;

	if (mixer == 0) {
		min = 1;
		max = 8;
		bit = 0x03; /* mixer0, dmap */
	} else {
		min = 9;
		max = 0xf;
		bit = 0x0C; /* mixer1, dmae */
	}
	mask = 0x0f;
	for (i = 0 ; i < 8 ; i++) {
		ndx = data & mask;
		ndx >>= (i * 4);
		if (ndx >= min && ndx <= max)
			data1 &= ~mask;  /* unstage pipe from mixer */
		mask <<= 4;
	}
	pr_debug("%s: => MIXER_RESET, data1=%x data=%x bit=%x\n",
				__func__, data1, data, bit);
	/* unstage pipes of mixer to be reset */
	outpdw(MDP_BASE + 0x10100, data1); /* MDP_LAYERMIXER_IN_CFG */
	outpdw(MDP_BASE + 0x18000, 0);

	mdp4_sw_reset(bit); /* reset mixer */   /* 0 => mixer0, dmap */

	/* restore origianl stage */
	outpdw(MDP_BASE + 0x10100, data); /* MDP_LAYERMIXER_IN_CFG */
	outpdw(MDP_BASE + 0x18000, 0);

	mdp4_vg_csc_restore();
	mdp4_overlay_dmap_reconfig();
	mdp_clk_ctrl(0);
}

void mdp4_mixer_stage_commit(int mixer)
{
	struct mdp4_overlay_pipe *pipe;
	int i, num;
	u32 data, stage;
	int off;
	unsigned long flags;

	data = 0;
	for (i = MDP4_MIXER_STAGE_BASE; i < MDP4_MIXER_STAGE_MAX; i++) {
		pipe = ctrl->stage[mixer][i];
		if (pipe == NULL)
			continue;
		pr_debug("%s: mixer=%d ndx=%d stage=%d\n", __func__,
					mixer, pipe->pipe_ndx, i);
		stage = pipe->mixer_stage;
		if (mixer >= MDP4_MIXER1)
			stage += 8;
		stage <<= (4 * pipe->pipe_num);
		data |= stage;
	}

	/*
	 * stage_commit may be called from overlay_unset
	 * for command panel, mdp clocks may be off at this time.
	 * so mdp clock enabled is necessary
	 */
	mdp_pipe_ctrl(MDP_CMD_BLOCK, MDP_BLOCK_POWER_ON, FALSE);
	mdp_clk_ctrl(1);

	if (data)
		mdp4_mixer_blend_setup(mixer);

	off = 0;
	if (data != ctrl->mixer_cfg[mixer]) {
		ctrl->mixer_cfg[mixer] = data;
		if (mixer >= MDP4_MIXER2) {
			/* MDP_LAYERMIXER2_IN_CFG */
			off = 0x100f0;
		} else {
			/* mixer 0 or 1 */
			num = mixer + 1;
			num &= 0x01;
			data |= ctrl->mixer_cfg[num];
			off = 0x10100;
		}
		pr_debug("%s: mixer=%d data=%x flush=%x pid=%d\n", __func__,
				mixer, data, ctrl->flush[mixer], current->pid);
		xlog(__func__, mixer, data, ctrl->flush[mixer], current->pid, 0);
	}

	local_irq_save(flags);
	if (off)
		outpdw(MDP_BASE + off, data);

	if (ctrl->flush[mixer]) {
		outpdw(MDP_BASE + 0x18000, ctrl->flush[mixer]);
		ctrl->flush[mixer] = 0;
	}
	local_irq_restore(flags);
	mdp_pipe_ctrl(MDP_CMD_BLOCK, MDP_BLOCK_POWER_OFF, FALSE);
	mdp_clk_ctrl(0);
}

void mdp4_mixer_stage_up(struct mdp4_overlay_pipe *pipe, int commit)
{
	struct mdp4_overlay_pipe *pp;
	int i, mixer;

	mixer = pipe->mixer_num;

	for (i = MDP4_MIXER_STAGE_BASE; i < MDP4_MIXER_STAGE_MAX; i++) {
		pp = ctrl->stage[mixer][i];
		if (pp && pp->pipe_ndx == pipe->pipe_ndx) {
			ctrl->stage[mixer][i] = NULL;
			break;
		}
	}

	ctrl->stage[mixer][pipe->mixer_stage] = pipe;	/* keep it */

	if (commit)
		mdp4_mixer_stage_commit(mixer);
}

void mdp4_mixer_stage_down(struct mdp4_overlay_pipe *pipe, int commit)
{
	struct mdp4_overlay_pipe *pp;
	int i, mixer;

	mixer = pipe->mixer_num;

	for (i = MDP4_MIXER_STAGE_BASE; i < MDP4_MIXER_STAGE_MAX; i++) {
		pp = ctrl->stage[mixer][i];
		if (pp && pp->pipe_ndx == pipe->pipe_ndx)
			ctrl->stage[mixer][i] = NULL;  /* clear it */
	}

	if (commit || (mixer > 0 && !hdmi_prim_display))
		mdp4_mixer_stage_commit(mixer);
}
/*
 * mixer0: rgb3: border color at register 0x15004, 0x15008
 * mixer1:  vg3: border color at register 0x1D004, 0x1D008
 * mixer2:  xxx: border color at register 0x8D004, 0x8D008
 */
void mdp4_overlay_borderfill_stage_up(struct mdp4_overlay_pipe *pipe)
{
	struct mdp4_overlay_pipe *bspipe;
	int ptype, pnum, pndx, mixer;
	int format, alpha_enable, alpha;
	struct mdp4_iommu_pipe_info iom;

	if (pipe->pipe_type != OVERLAY_TYPE_BF)
		return;

	mixer = pipe->mixer_num;

	printk("%s mixer(%d) start\n", __func__, mixer);
	if (ctrl && ctrl->baselayer[mixer]) {
		pr_err("%s:%d ctl is NULL", __func__, __LINE__);
		return;
	}

	bspipe = ctrl->stage[mixer][MDP4_MIXER_STAGE_BASE];
	if (bspipe == NULL) {
			pr_err("%s: no base layer at mixer=%d\n",
							__func__, mixer);
			return;
	}

	/*
	 * bspipe is clone here
	 * get real pipe
	 */
	bspipe = mdp4_overlay_ndx2pipe(bspipe->pipe_ndx);
	if(!bspipe) {
		pr_err("%s:%d real base pipe in NULL", __func__, __LINE__);
		return;
	}

	if (bspipe == NULL) {
			pr_err("%s: mdp4_overlay_ndx2pipe returned null pipe ndx\n",
							__func__);
			return;
	}
	xlog(__func__, bspipe->pipe_ndx, pipe->pipe_ndx, 0, 0, 0);
	/* save original base layer */
	ctrl->baselayer[mixer] = bspipe;

	iom = pipe->iommu;
	pipe->alpha = 0;	/* make sure bf pipe has alpha 0 */
	ptype = pipe->pipe_type;
	pnum = pipe->pipe_num;
	pndx = pipe->pipe_ndx;
	format = pipe->src_format;
	alpha_enable = pipe->alpha_enable;
	alpha = pipe->alpha;
	*pipe = *bspipe;	/* keep base layer configuration */
	pipe->pipe_type = ptype;
	pipe->pipe_num = pnum;
	pipe->pipe_ndx = pndx;
	pipe->src_format = format;
	pipe->alpha_enable = alpha_enable;
	pipe->alpha = alpha;
	pipe->iommu = iom;

	/* free original base layer pipe to be sued as normal pipe */
	bspipe->pipe_used = 0;

	if (ctrl->panel_mode & MDP4_PANEL_DSI_VIDEO)
		mdp4_dsi_video_base_swap(0, pipe);
	else if (ctrl->panel_mode & MDP4_PANEL_DSI_CMD)
		mdp4_dsi_cmd_base_swap(0, pipe);
	else if (ctrl->panel_mode & MDP4_PANEL_LCDC)
		mdp4_lcdc_base_swap(0, pipe);
#ifdef CONFIG_FB_MSM_DTV
	else if (ctrl->panel_mode & MDP4_PANEL_DTV)
		mdp4_dtv_base_swap(0, pipe);
#endif

	mdp4_overlay_reg_flush(bspipe, 1);
	/* borderfill pipe as base layer */
	mdp4_mixer_stage_up(pipe, 0);

	printk("%s mixer(%d) end\n", __func__, mixer);
}

void mdp4_overlay_borderfill_stage_down(struct mdp4_overlay_pipe *pipe)
{
	struct mdp4_overlay_pipe *bspipe;
	int ptype, pnum, pndx, mixer;
	int format, alpha_enable, alpha;
	struct mdp4_iommu_pipe_info iom;

	if (pipe->pipe_type != OVERLAY_TYPE_BF)
		return;

	mixer = pipe->mixer_num;

	/* retrieve original base layer */
	bspipe = ctrl->baselayer[mixer];
	if (bspipe == NULL) {
		pr_err("%s: no base layer at mixer=%d\n",
				__func__, mixer);
		return;
	}
	xlog(__func__, bspipe->pipe_ndx, pipe->pipe_ndx, 0, 0, 0);
	iom = bspipe->iommu;
	ptype = bspipe->pipe_type;
	pnum = bspipe->pipe_num;
	pndx = bspipe->pipe_ndx;
	format = bspipe->src_format;
	alpha_enable = bspipe->alpha_enable;
	alpha = bspipe->alpha;
	*bspipe = *pipe;	/* restore base layer configuration */
	bspipe->pipe_type = ptype;
	bspipe->pipe_num = pnum;
	bspipe->pipe_ndx = pndx;
	bspipe->src_format = format;
	bspipe->alpha_enable = alpha_enable;
	bspipe->alpha = alpha;
	bspipe->iommu = iom;

	bspipe->pipe_used++;	/* mark base layer pipe used */

	ctrl->baselayer[mixer] = NULL;

	/* free borderfill pipe */
	pipe->pipe_used = 0;

	if (ctrl->panel_mode & MDP4_PANEL_DSI_VIDEO)
		mdp4_dsi_video_base_swap(0, bspipe);
	else if (ctrl->panel_mode & MDP4_PANEL_DSI_CMD)
		mdp4_dsi_cmd_base_swap(0, bspipe);
	else if (ctrl->panel_mode & MDP4_PANEL_LCDC)
		mdp4_lcdc_base_swap(0, bspipe);
	else if (ctrl->panel_mode & MDP4_PANEL_DTV)
		mdp4_dtv_base_swap(0, bspipe);

	/* free borderfill pipe */
	mdp4_overlay_reg_flush(pipe, 1);
	mdp4_mixer_stage_down(pipe, 0); /* commit will happen for bspipe up */
	mdp4_overlay_pipe_free(pipe, 0);

	/* stage up base layer */
	mdp4_overlay_reg_flush(bspipe, 1);
	/* restore original base layer */
	mdp4_mixer_stage_up(bspipe, 1);
}


static struct mdp4_overlay_pipe *mdp4_background_layer(int mixer,
			struct mdp4_overlay_pipe *sp)
{
	struct mdp4_overlay_pipe *pp;
	struct mdp4_overlay_pipe *kp;
	int i;

	kp = ctrl->stage[mixer][MDP4_MIXER_STAGE_BASE];
	for (i = MDP4_MIXER_STAGE_BASE; i < MDP4_MIXER_STAGE_MAX; i++) {
		pp = ctrl->stage[mixer][i];
		if (pp == NULL)
			continue;
		if (pp == sp)
			break;

		if ((pp->dst_x <= sp->dst_x) &&
			((pp->dst_x + pp->dst_w) >= (sp->dst_x + sp->dst_w))) {
			if ((pp->dst_y <= sp->dst_y) &&
				((pp->dst_y + pp->dst_h) >=
					(sp->dst_y + sp->dst_h))) {
				kp = pp;
			}
		}
	}
	return kp;
}

static void mdp4_overlay_bg_solidfill(struct blend_cfg *blend)
{
	struct mdp4_overlay_pipe *pipe;
	char *base;
	u32 op_mode, format;
	int pnum, ptype;

	pipe = blend->solidfill_pipe;
	if (pipe == NULL)
		return;

	if (pipe->pipe_type == OVERLAY_TYPE_BF)
		return;

	ptype = mdp4_overlay_format2type(pipe->src_format);
	if (ptype == OVERLAY_TYPE_RGB) {
		pnum = pipe->pipe_num - OVERLAY_PIPE_RGB1;
		base = MDP_BASE + MDP4_RGB_BASE;
		base += MDP4_RGB_OFF * pnum;
	} else {
		pnum = pipe->pipe_num - OVERLAY_PIPE_VG1;
		base = MDP_BASE + MDP4_VIDEO_BASE;
		base += MDP4_VIDEO_OFF * pnum;
	}

	format = inpdw(base + 0x50);
	format |= MDP4_FORMAT_SOLID_FILL;
	/*
	 * If solid fill is enabled, flip and scale
	 * have to be disabled. otherwise, h/w
	 * underruns.
	 */
	op_mode = inpdw(base + 0x0058);
	op_mode &= ~(MDP4_OP_FLIP_LR + MDP4_OP_SCALEX_EN);
	op_mode &= ~(MDP4_OP_FLIP_UD + MDP4_OP_SCALEY_EN);
	outpdw(base + 0x0058, op_mode);
	outpdw(base + 0x1008, 0);	/* black */
	/*
	 * Set src size and dst size same to avoid underruns
	 */
	outpdw(base + 0x0000, inpdw(base + 0x0008));
	outpdw(base + 0x50, format);

	mdp4_overlay_reg_flush(pipe, 0);
}

void mdp4_mixer_blend_cfg(int mixer)
{
	int i, off;
	unsigned char *overlay_base;
	struct blend_cfg *blend;

	if (mixer == MDP4_MIXER2)
		overlay_base = MDP_BASE + MDP4_OVERLAYPROC2_BASE;
	else if (mixer == MDP4_MIXER1)
		overlay_base = MDP_BASE + MDP4_OVERLAYPROC1_BASE;
	else
		overlay_base = MDP_BASE + MDP4_OVERLAYPROC0_BASE;

	blend = &ctrl->blend[mixer][MDP4_MIXER_STAGE_BASE];
	blend++; /* stage0 */

	for (i = MDP4_MIXER_STAGE0; i < MDP4_MIXER_STAGE_MAX; i++) {
		off = 20 * i;
		off = 0x20 * (i - MDP4_MIXER_STAGE0);
		if (i == MDP4_MIXER_STAGE3)
			off -= 4;
		outpdw(overlay_base + off + 0x104, blend->op);
		blend++;
	}
}

static void mdp4_set_blend_by_op(struct mdp4_overlay_pipe *s_pipe,
					struct mdp4_overlay_pipe *d_pipe,
					int alpha_drop,
					struct blend_cfg *blend)
{
	int d_alpha, s_alpha;
	u32 op;

	d_alpha = d_pipe->alpha_enable;
	s_alpha = s_pipe->alpha_enable;
	/* base on fg's alpha */
	blend->fg_alpha = s_pipe->alpha;
	blend->bg_alpha = 0x0ff - s_pipe->alpha;
	blend->op = MDP4_BLEND_FG_ALPHA_FG_CONST |
	MDP4_BLEND_BG_ALPHA_BG_CONST;
	blend->co3_sel = 1; /* use fg alpha */
	op = s_pipe->blend_op;
	if (op == BLEND_OP_OPAQUE) {
		blend->bg_alpha = 0;
		blend->fg_alpha = 0xff;
	} else if ((op == BLEND_OP_PREMULTIPLIED) &&
			(!alpha_drop) && s_alpha) {
		blend->op = MDP4_BLEND_FG_ALPHA_FG_CONST |
			MDP4_BLEND_BG_INV_ALPHA |
			MDP4_BLEND_BG_ALPHA_FG_PIXEL;
		if (blend->fg_alpha != 0xff) {
			blend->bg_alpha = blend->fg_alpha;
			blend->op |= MDP4_BLEND_BG_MOD_ALPHA;
		}
	} else if (!alpha_drop && s_alpha) {
		blend->op = MDP4_BLEND_FG_ALPHA_FG_PIXEL |
			MDP4_BLEND_BG_INV_ALPHA |
			MDP4_BLEND_BG_ALPHA_FG_PIXEL;
		if (blend->fg_alpha != 0xff) {
			blend->bg_alpha = blend->fg_alpha;
			blend->op |= MDP4_BLEND_FG_MOD_ALPHA |
				MDP4_BLEND_BG_MOD_ALPHA;
		}
	}
	if (!s_alpha && d_alpha)
		blend->co3_sel = 0;
	pr_debug("%s: op %d bg alpha %d, fg alpha %d blend: %x\n",
		__func__, op, blend->bg_alpha, blend->fg_alpha, blend->op);
}

static void mdp4_set_blend_by_fmt(struct mdp4_overlay_pipe *s_pipe,
					struct mdp4_overlay_pipe *d_pipe,
					int alpha_drop,
					struct blend_cfg *blend)
{
	int ptype, d_alpha, s_alpha;
	d_alpha = d_pipe->alpha_enable;
	s_alpha = s_pipe->alpha_enable;
	/* base on fg's alpha */
	blend->bg_alpha = 0x0ff - s_pipe->alpha;
	blend->fg_alpha = s_pipe->alpha;
	blend->co3_sel = 1; /* use fg alpha */

	if (s_pipe->is_fg) {
		if (s_pipe->alpha == 0xff) {
			blend->solidfill = 1;
			blend->solidfill_pipe = d_pipe;
		}
	} else if (s_alpha) {
		if (!alpha_drop) {
			blend->op = MDP4_BLEND_BG_ALPHA_FG_PIXEL;
			if (!(s_pipe->flags & MDP_BLEND_FG_PREMULT))
				blend->op |=
					MDP4_BLEND_FG_ALPHA_FG_PIXEL;
		} else
			blend->op = MDP4_BLEND_BG_ALPHA_FG_CONST;

		blend->op |= MDP4_BLEND_BG_INV_ALPHA;
	} else if (d_alpha) {
		ptype = mdp4_overlay_format2type(s_pipe->src_format);
		if (ptype == OVERLAY_TYPE_VIDEO &&
			(!(s_pipe->flags & MDP_BACKEND_COMPOSITION))) {
			blend->op = (MDP4_BLEND_FG_ALPHA_BG_PIXEL |
				MDP4_BLEND_FG_INV_ALPHA);
			if (!(s_pipe->flags & MDP_BLEND_FG_PREMULT))
				blend->op |=
					MDP4_BLEND_BG_ALPHA_BG_PIXEL;
			blend->co3_sel = 0; /* use bg alpha */
		} else {
			/* s_pipe is rgb without alpha */
			blend->op = (MDP4_BLEND_FG_ALPHA_FG_CONST |
				    MDP4_BLEND_BG_ALPHA_BG_CONST);
			blend->bg_alpha = 0;
		}
	}
}

/*
 * D(i+1) = Ks * S + Kd * D(i)
 */
void mdp4_mixer_blend_setup(int mixer)
{
	struct mdp4_overlay_pipe *d_pipe;
	struct mdp4_overlay_pipe *s_pipe;
	struct blend_cfg *blend;
	int i, off, alpha_drop;
	unsigned char *overlay_base;
	uint32 c0, c1, c2;


	d_pipe = ctrl->stage[mixer][MDP4_MIXER_STAGE_BASE];
	if (d_pipe == NULL) {
		pr_err("%s: Error: no bg_pipe at mixer=%d\n", __func__, mixer);
		return;
	}

	blend = &ctrl->blend[mixer][MDP4_MIXER_STAGE0];
	for (i = MDP4_MIXER_STAGE0; i < MDP4_MIXER_STAGE_MAX; i++) {
		blend->solidfill = 0;
		blend->op = (MDP4_BLEND_FG_ALPHA_FG_CONST |
				    MDP4_BLEND_BG_ALPHA_BG_CONST);
		s_pipe = ctrl->stage[mixer][i];
		if (s_pipe == NULL) {
			blend++;
			d_pipe = NULL;
			continue;
		}
		alpha_drop = 0;	/* per stage */
		/* alpha channel is lost on VG pipe when using QSEED or M/N */
		if (s_pipe->pipe_type == OVERLAY_TYPE_VIDEO &&
			s_pipe->alpha_enable &&
			((s_pipe->op_mode & MDP4_OP_SCALEY_EN) ||
			(s_pipe->op_mode & MDP4_OP_SCALEX_EN)) &&
			!(s_pipe->op_mode & (MDP4_OP_SCALEX_PIXEL_RPT |
			MDP4_OP_SCALEY_PIXEL_RPT)))
			alpha_drop = 1;

		d_pipe = mdp4_background_layer(mixer, s_pipe);

		pr_debug("%s: stage=%d: bg: ndx=%d da=%d dalpha=%x "
			"fg: ndx=%d sa=%d salpha=%x is_fg=%d alpha_drop=%d\n",
			__func__, i-2, d_pipe->pipe_ndx, d_pipe->alpha_enable,
			d_pipe->alpha, s_pipe->pipe_ndx, s_pipe->alpha_enable,
			s_pipe->alpha, s_pipe->is_fg, alpha_drop);
		if ((s_pipe->blend_op == BLEND_OP_NOT_DEFINED) ||
			(s_pipe->blend_op >= BLEND_OP_MAX))
			mdp4_set_blend_by_fmt(s_pipe, d_pipe,
				alpha_drop, blend);
		else
			mdp4_set_blend_by_op(s_pipe, d_pipe, alpha_drop, blend);


		if (s_pipe->transp != MDP_TRANSP_NOP) {
			if (s_pipe->is_fg) {
				transp_color_key(s_pipe->src_format,
						s_pipe->transp, &c0, &c1, &c2);
				/* Fg blocked */
				blend->op |= MDP4_BLEND_FG_TRANSP_EN;
				/* lower limit */
				blend->transp_low0 = (c1 << 16 | c0);
				blend->transp_low1 = c2;
				/* upper limit */
				blend->transp_high0 = (c1 << 16 | c0);
				blend->transp_high1 = c2;
			} else {
				transp_color_key(d_pipe->src_format,
						s_pipe->transp, &c0, &c1, &c2);
				/* Fg blocked */
				blend->op |= MDP4_BLEND_BG_TRANSP_EN;
				blend--; /* one stage back */
				/* lower limit */
				blend->transp_low0 = (c1 << 16 | c0);
				blend->transp_low1 = c2;
				/* upper limit */
				blend->transp_high0 = (c1 << 16 | c0);
				blend->transp_high1 = c2;
				blend++; /* back to original stage */
			}
		}
		blend++;
	}

	/* mixer numer, /dev/fb0, /dev/fb1, /dev/fb2 */
	if (mixer == MDP4_MIXER2)
		overlay_base = MDP_BASE + MDP4_OVERLAYPROC2_BASE;/* 0x88000 */
	else if (mixer == MDP4_MIXER1)
		overlay_base = MDP_BASE + MDP4_OVERLAYPROC1_BASE;/* 0x18000 */
	else
		overlay_base = MDP_BASE + MDP4_OVERLAYPROC0_BASE;/* 0x10000 */

	mdp_pipe_ctrl(MDP_CMD_BLOCK, MDP_BLOCK_POWER_ON, FALSE);
	blend = &ctrl->blend[mixer][MDP4_MIXER_STAGE_BASE];
	/* lower limit */
	outpdw(overlay_base + 0x180, blend->transp_low0);
	outpdw(overlay_base + 0x184,  blend->transp_low1);
	/* upper limit */
	outpdw(overlay_base + 0x188, blend->transp_high0);
	outpdw(overlay_base + 0x18c,  blend->transp_high1);
	blend++; /* stage0 */
	for (i = MDP4_MIXER_STAGE0; i < MDP4_MIXER_STAGE_MAX; i++) {
		off = 20 * i;
		off = 0x20 * (i - MDP4_MIXER_STAGE0);
		if (i == MDP4_MIXER_STAGE3)
			off -= 4;

		if (blend->solidfill_pipe && blend->solidfill)
			mdp4_overlay_bg_solidfill(blend);
		else
			blend->solidfill_pipe = NULL;

		outpdw(overlay_base + off + 0x108, blend->fg_alpha);
		outpdw(overlay_base + off + 0x10c, blend->bg_alpha);

		if (mdp_rev >= MDP_REV_42)
			outpdw(overlay_base + off + 0x104, blend->op);

		outpdw(overlay_base + (off << 5) + 0x1004, blend->co3_sel);
		outpdw(overlay_base + off + 0x110, blend->transp_low0);/* low */
		outpdw(overlay_base + off + 0x114, blend->transp_low1);/* low */
			/* upper limit */
		outpdw(overlay_base + off + 0x118, blend->transp_high0);
		outpdw(overlay_base + off + 0x11c, blend->transp_high1);
		blend++;
	}
	mdp_pipe_ctrl(MDP_CMD_BLOCK, MDP_BLOCK_POWER_OFF, FALSE);
}

void mdp4_overlay_reg_flush(struct mdp4_overlay_pipe *pipe, int all)
{
	int mixer;
	uint32 *reg;

	mixer = pipe->mixer_num;
	reg = &ctrl->flush[mixer];
	*reg |= (1 << (2 + pipe->pipe_num));

	if (all) {
		if (mixer == MDP4_MIXER0)
			*reg |= 0x01;
		else
			*reg |= 0x02;
	}
}

void mdp4_overlay_flush_piggyback(int m0, int m1)
{
	u32 data;

	data = ctrl->flush[m0] | ctrl->flush[m1];
	ctrl->flush[m0] = data;
}

void mdp4_overlay_reg_flush_reset(struct mdp4_overlay_pipe *pipe)
{
	int mixer;

	mixer = pipe->mixer_num;
	ctrl->flush[mixer] = 0;
}

struct mdp4_overlay_pipe *mdp4_overlay_stage_pipe(int mixer, int stage)
{
	return ctrl->stage[mixer][stage];
}

struct mdp4_overlay_pipe *mdp4_overlay_ndx2pipe(int ndx)
{
	struct mdp4_overlay_pipe *pipe;

	if (ndx <= 0 || ndx > OVERLAY_PIPE_MAX)
		return NULL;

	pipe = &ctrl->plist[ndx - 1];	/* ndx start from 1 */

	if (pipe->pipe_used == 0)
		return NULL;

	return pipe;
}

struct mdp4_overlay_pipe *mdp4_overlay_pipe_alloc(int ptype, int mixer)
{
	int i;
	struct mdp4_overlay_pipe *pipe;

	if (ptype == OVERLAY_TYPE_BF) {
		if (!mdp4_overlay_borderfill_supported())
			return NULL;
	}

	for (i = 0; i < OVERLAY_PIPE_MAX; i++) {
		pipe = &ctrl->plist[i];
		if ((pipe->pipe_used == 0) && ((pipe->pipe_type == ptype) ||
		    (ptype == OVERLAY_TYPE_RGB &&
		     pipe->pipe_type == OVERLAY_TYPE_VIDEO))) {
			if (ptype == OVERLAY_TYPE_BF &&
			    mixer != pipe->mixer_num)
				continue;
			init_completion(&pipe->comp);
			init_completion(&pipe->dmas_comp);
			pr_debug("%s: pipe=%x ndx=%d num=%d\n", __func__,
				(int)pipe, pipe->pipe_ndx, pipe->pipe_num);
			return pipe;
		}
	}

	pr_err("%s: ptype=%d FAILED\n", __func__, ptype);

	return NULL;
}


void mdp4_overlay_pipe_free(struct mdp4_overlay_pipe *pipe, int all)
{
	uint32 ptype, num, ndx, mixer;
	struct mdp4_iommu_pipe_info iom;
	struct mdp4_overlay_pipe *orgpipe;

	pr_debug("%s: pipe=%x ndx=%d\n", __func__, (int)pipe, pipe->pipe_ndx);

	ptype = pipe->pipe_type;
	num = pipe->pipe_num;
	ndx = pipe->pipe_ndx;
	mixer = pipe->mixer_num;

	/* No need for borderfill pipe */
	if (pipe->pipe_type != OVERLAY_TYPE_BF)
		mdp4_overlay_iommu_pipe_free(pipe->pipe_ndx, all);
	xlog(__func__, pipe->pipe_ndx, 0, 0, 0, 0);

	iom = pipe->iommu;

	memset(pipe, 0, sizeof(*pipe));
	pipe->pipe_type = ptype;
	pipe->pipe_num = num;
	pipe->pipe_ndx = ndx;
	pipe->mixer_num = mixer;
	pipe->iommu = iom;

	/*Clear real pipe attributes as well */
	orgpipe = mdp4_overlay_ndx2pipe(pipe->pipe_ndx);
	if (orgpipe != NULL)
		orgpipe->pipe_used = 0;

}

static int mdp4_overlay_req2pipe(struct mdp_overlay *req, int mixer,
			struct mdp4_overlay_pipe **ppipe,
			struct msm_fb_data_type *mfd)
{
	struct mdp4_overlay_pipe *pipe;
	int ret, ptype;

	u32 upscale_max;
	upscale_max = (mdp_rev >= MDP_REV_41) ?
		MDP4_REV41_OR_LATER_UP_SCALING_MAX :
		MDP4_REV40_UP_SCALING_MAX;

	if (mfd == NULL) {
		pr_err("%s: mfd == NULL, -ENODEV\n", __func__);
		return -ENODEV;
	}

	if (mixer >= MDP4_MIXER_MAX) {
		pr_err("%s: mixer out of range!\n", __func__);
		mdp4_stat.err_mixer++;
		return -ERANGE;
	}

	if (req->z_order < 0 || req->z_order > 3) {
		pr_err("%s: z_order=%d out of range!\n", __func__,
				req->z_order);
		mdp4_stat.err_zorder++;
		return -ERANGE;
	}

	if (req->src_rect.h > 0xFFF || req->src_rect.h < 2) {
		pr_err("%s: src_h is out of range: 0X%x!\n",
		       __func__, req->src_rect.h);
		mdp4_stat.err_size++;
		return -EINVAL;
	}

	if (req->src_rect.w > 0xFFF || req->src_rect.w < 2) {
		pr_err("%s: src_w is out of range: 0X%x!\n",
		       __func__, req->src_rect.w);
		mdp4_stat.err_size++;
		return -EINVAL;
	}

	if (req->src_rect.x > 0xFFF) {
		pr_err("%s: src_x is out of range: 0X%x!\n",
		       __func__, req->src_rect.x);
		mdp4_stat.err_size++;
		return -EINVAL;
	}

	if (req->src_rect.y > 0xFFF) {
		pr_err("%s: src_y is out of range: 0X%x!\n",
		       __func__, req->src_rect.y);
		mdp4_stat.err_size++;
		return -EINVAL;
	}

	if (req->dst_rect.h > 0xFFF || req->dst_rect.h < 2) {
		pr_err("%s: dst_h is out of range: 0X%x!\n",
		       __func__, req->dst_rect.h);
		mdp4_stat.err_size++;
		return -EINVAL;
	}

	if (req->dst_rect.w > 0xFFF || req->dst_rect.w < 2) {
		pr_err("%s: dst_w is out of range: 0X%x!\n",
		       __func__, req->dst_rect.w);
		mdp4_stat.err_size++;
		return -EINVAL;
	}

	if (req->dst_rect.x > 0xFFF) {
		pr_err("%s: dst_x is out of range: 0X%x!\n",
		       __func__, req->dst_rect.x);
		mdp4_stat.err_size++;
		return -EINVAL;
	}

	if (req->dst_rect.y > 0xFFF) {
		pr_err("%s: dst_y is out of range: 0X%x!\n",
		       __func__, req->dst_rect.y);
		mdp4_stat.err_size++;
		return -EINVAL;
	}

	if (req->src_rect.h == 0 || req->src_rect.w == 0) {
		pr_err("%s: src img of zero size!\n", __func__);
		mdp4_stat.err_size++;
		return -EINVAL;
	}

	if (req->dst_rect.h > (req->src_rect.h * upscale_max)) {
		mdp4_stat.err_scale++;
		pr_err("%s: scale up, too much (h)!\n", __func__);
		return -ERANGE;
	}

	if (req->src_rect.h > (req->dst_rect.h * 8)) {	/* too little */
		mdp4_stat.err_scale++;
		pr_err("%s: scale down, too little (h)!\n", __func__);
		return -ERANGE;
	}

	if (req->dst_rect.w > (req->src_rect.w * upscale_max)) {
		mdp4_stat.err_scale++;
		pr_err("%s: scale up, too much (w)!\n", __func__);
		return -ERANGE;
	}

	if (req->src_rect.w > (req->dst_rect.w * 8)) {	/* too little */
		mdp4_stat.err_scale++;
		pr_err("%s: scale down, too little (w)!\n", __func__);
		return -ERANGE;
	}

	if (mdp_hw_revision == MDP4_REVISION_V1) {
		/*  non integer down saceling ratio  smaller than 1/4
		 *  is not supportted
		 */
		if (req->src_rect.h > (req->dst_rect.h * 4)) {
			if (req->src_rect.h % req->dst_rect.h) {
				mdp4_stat.err_scale++;
				pr_err("%s: need integer (h)!\n", __func__);
				return -ERANGE;
			}
		}

		if (req->src_rect.w > (req->dst_rect.w * 4)) {
			if (req->src_rect.w % req->dst_rect.w) {
				mdp4_stat.err_scale++;
				pr_err("%s: need integer (w)!\n", __func__);
				return -ERANGE;
			}
		}
	}

	if (((req->src_rect.x + req->src_rect.w) > req->src.width) ||
		((req->src_rect.y + req->src_rect.h) > req->src.height)) {
		mdp4_stat.err_size++;
		pr_err("%s invalid src rectangle\n", __func__);
		return -ERANGE;
	}

	if (ctrl->panel_3d != MDP4_3D_SIDE_BY_SIDE) {
		int xres;
		int yres;

		xres = mfd->var_xres;
		yres = mfd->var_yres;

		if (((req->dst_rect.x + req->dst_rect.w) > xres) ||
			((req->dst_rect.y + req->dst_rect.h) > yres)) {
			mdp4_stat.err_size++;
			pr_err("%s invalid dst rectangle\n", __func__);
			return -ERANGE;
		}
	}

	ptype = mdp4_overlay_format2type(req->src.format);
	if (ptype < 0) {
		pr_err("%s: mdp4_overlay_format2type!\n", __func__);
		return ptype;
	}

	if (req->flags & MDP_OV_PIPE_SHARE)
		ptype = OVERLAY_TYPE_VIDEO; /* VG pipe supports both RGB+YUV */

	if (req->id == MSMFB_NEW_REQUEST)  /* new request */
		pipe = mdp4_overlay_pipe_alloc(ptype, mixer);
	else
		pipe = mdp4_overlay_ndx2pipe(req->id);

	if (pipe == NULL) {
		pr_err("%s: pipe == NULL!\n", __func__);
		return -ENOMEM;
	}

	if (!display_iclient && !IS_ERR_OR_NULL(mfd->iclient)) {
		display_iclient = mfd->iclient;
		pr_debug("%s(): display_iclient %p\n", __func__,
			display_iclient);
	}

	pipe->src_format = req->src.format;
	ret = mdp4_overlay_format2pipe(pipe);

	if (ret < 0) {
		pr_err("%s: mdp4_overlay_format2pipe!\n", __func__);
		return ret;
	}

	/*
	 * base layer == 1, reserved for frame buffer
	 * zorder 0 == stage 0 == 2
	 * zorder 1 == stage 1 == 3
	 * zorder 2 == stage 2 == 4
	 */
	if (req->id == MSMFB_NEW_REQUEST) {  /* new request */
		if (mdp4_overlay_pipe_staged(pipe)) {
			pr_err("%s: ndx=%d still staged\n", __func__,
						pipe->pipe_ndx);
			return -EPERM;
		}
		pipe->pipe_used++;
		pipe->mixer_num = mixer;
		pr_debug("%s: zorder=%d pipe ndx=%d num=%d\n", __func__,
			req->z_order, pipe->pipe_ndx, pipe->pipe_num);

	}
#ifdef MDP_ODD_RESOLUTION_CTRL
	if (req->dst_rect.w== 1) {
		pr_err("#### dst change\n");
		if (req->dst_rect.x >= 1079)
			req->dst_rect.x -=1;
		req->dst_rect.w += 1;
		pipe->check_odd_res = 1;
	} else if (req->dst_rect.w == 1079)
		pipe->check_odd_res = 1;
#endif

	pipe->mixer_stage = req->z_order + MDP4_MIXER_STAGE0;
	pipe->src_width = req->src.width & 0x1fff;	/* source img width */
	pipe->src_height = req->src.height & 0x1fff;	/* source img height */
	pipe->src_h = req->src_rect.h & 0x07ff;
	pipe->src_w = req->src_rect.w & 0x07ff;
	pipe->src_y = req->src_rect.y & 0x07ff;
	pipe->src_x = req->src_rect.x & 0x07ff;
	pipe->dst_h = req->dst_rect.h & 0x07ff;
	pipe->dst_w = req->dst_rect.w & 0x07ff;
	pipe->dst_y = req->dst_rect.y & 0x07ff;
	pipe->dst_x = req->dst_rect.x & 0x07ff;

	pipe->op_mode = 0;

	if (req->flags & MDP_FLIP_LR)
		pipe->op_mode |= MDP4_OP_FLIP_LR;

	if (req->flags & MDP_FLIP_UD)
		pipe->op_mode |= MDP4_OP_FLIP_UD;

	if (req->flags & MDP_DITHER)
		pipe->op_mode |= MDP4_OP_DITHER_EN;

	if (req->flags & MDP_DEINTERLACE)
		pipe->op_mode |= MDP4_OP_DEINT_EN;

	if (req->flags & MDP_DEINTERLACE_ODD)
		pipe->op_mode |= MDP4_OP_DEINT_ODD_REF;

	pipe->is_fg = req->is_fg;/* control alpha and color key */

	pipe->alpha = req->alpha & 0x0ff;
	pipe->blend_op = req->blend_op;

	pipe->transp = req->transp_mask;

	if ((pipe->flags & MDP_SECURE_OVERLAY_SESSION) &&
		(!(req->flags & MDP_SECURE_OVERLAY_SESSION))) {
		pr_err("%s Switch secure %d", __func__, pipe->pipe_ndx);
		mfd->sec_active = FALSE;
	}
	pipe->flags = req->flags;

	*ppipe = pipe;

	return 0;
}

/*
  [srcx,srcy,srcw,srch]->[dstx,dsty,dstw,dsth][flags]|src_format|bpp|pipe_ndx|mdp_clk|ab|ib|
  [cnt][total_ab_p0][total_ib_p0][total_ab_p0][total_ib_p0][mdp_clk]
*/
void dump_underrun_pipe_info(void)
{

	struct mdp4_overlay_pipe *pipe = ctrl->plist;
	int i = 0;
	int cnt = 0;
	static int lindex = 0;
	if(!sec_underrun_log_buff) {
		sec_underrun_log_buff = kzalloc(4001,GFP_KERNEL);
		if(!sec_underrun_log_buff) {
			pr_err("Out of Memory: Failed to alloc underrun Buffer\n");
			return;
		}

	}

	for (i = 0; i < OVERLAY_PIPE_MAX; i++, pipe++) {

		if (!pipe)
			return ;

		if (!pipe->pipe_used)
			continue;
		cnt++;
		lindex += snprintf(sec_underrun_log_buff+lindex,4000-lindex,
                       "[%d,%d,%d,%d]->[%d,%d,%d,%d][flag:%d][format:%d][bpp:%d][ndx:%d][req_clk:%d][ab:%llu][ib:%llu]\n",
                       pipe->src_x,pipe->src_y,pipe->src_w,pipe->src_h,
                       pipe->dst_x,pipe->dst_y,pipe->dst_w,pipe->dst_h,
                       pipe->flags,
                       pipe->src_format,pipe->bpp,pipe->pipe_ndx,pipe->req_clk,
                       pipe->bw_ab_quota,pipe->bw_ib_quota);
		pr_err("[%d,%d,%d,%d]->[%d,%d,%d,%d][flag:%d][format:%d][bpp:%d][ndx:%d][req_clk:%d][ab:%llu][ib:%llu]\n",
                       pipe->src_x,pipe->src_y,pipe->src_w,pipe->src_h,
                       pipe->dst_x,pipe->dst_y,pipe->dst_w,pipe->dst_h,
                       pipe->flags,
                       pipe->src_format,pipe->bpp,pipe->pipe_ndx,pipe->req_clk,
                       pipe->bw_ab_quota,pipe->bw_ib_quota);
	}
	lindex+=snprintf(sec_underrun_log_buff+lindex,4000-lindex,
		"**[%d][ab_0:%llu][ib_0:%llu][ab_1:%llu][ib_1:%llu][mdp_clk:%d]**\n",cnt,
               perf_request.mdp_ab_port0_bw,perf_request.mdp_ib_port0_bw,
               perf_request.mdp_ab_port1_bw,perf_request.mdp_ib_port1_bw,
               perf_current.mdp_clk_rate);
	pr_err("**[%d][ab_0:%llu][ib_0:%llu][ab_1:%llu][ib_1:%llu][mdp_clk:%d]**\n",cnt,
               perf_request.mdp_ab_port0_bw,perf_request.mdp_ib_port0_bw,
               perf_request.mdp_ab_port1_bw,perf_request.mdp_ib_port1_bw,
               perf_current.mdp_clk_rate);
	lindex = lindex % 4000;

}

static int mdp4_calc_req_mdp_clk(struct msm_fb_data_type *mfd,
				 u32 src_h, u32 dst_h, u32 src_w, u32 dst_w)
{
	u32 pclk, hsync;
	u32 xscale, yscale;
	u32 shift = 16;
	u64 rst;

	pr_debug("%s: pipe sets: panel res(x,y)=(%d,%d)\n",
		 __func__,  mfd->panel_info.xres, mfd->panel_info.yres);

	pr_debug("%s: src_h=%d, dst_h=%d, src_w=%d, dst_w=%d\n",
		 __func__, src_h, dst_h, src_w, dst_w);

	pclk = (mfd->panel_info.type == MIPI_VIDEO_PANEL ||
		mfd->panel_info.type == MIPI_CMD_PANEL) ?
		mfd->panel_info.mipi.dsi_pclk_rate :
		mfd->panel_info.clk_rate;

	if (!pclk) {
		pr_err("%s panel pixel clk is zero!\n", __func__);
		return mdp_max_clk;
	}

	pr_debug("%s: mdp panel pixel clk is %d.\n", __func__, pclk);

	hsync = mfd->panel_info.lcdc.h_back_porch +
		mfd->panel_info.lcdc.h_front_porch +
		mfd->panel_info.lcdc.h_pulse_width +
		mfd->panel_info.xres;

	/*
	 * For the scaling cases, adding extra 20% margin
	 */
	if ((src_h != dst_h) || (src_w != dst_w)) {
		hsync *= 100;
		hsync /= 120;
	}

	if (!hsync) {
		pr_err("%s: panel hsync is zero!\n", __func__);
		return mdp_max_clk;
	}

	pr_debug("%s: panel hsync is %d.\n", __func__, hsync);

	if (!src_h) {
		pr_err("%s: src_h is zero!\n", __func__);
		return mdp_max_clk;
	}

	if (!dst_h) {
		pr_err("%s: dst_h is zero!\n", __func__);
		return mdp_max_clk;
	}

	if (!src_w) {
		pr_err("%s: src_w is zero!\n", __func__);
		return mdp_max_clk;
	}

	if (!dst_w) {
		pr_err("%s: dst_w is zero!\n", __func__);
		return mdp_max_clk;
	}

	xscale = mfd->panel_info.xres;
	xscale += src_w;

	if (xscale < dst_w) {
		pr_err("%s: xres+src_w cannot be less than dst_w!\n",
		       __func__);
		return mdp_max_clk;
	}

	xscale -= dst_w;
	xscale <<= shift;
	xscale /= hsync;
	pr_debug("%s: the right %d shifted xscale is %d.\n",
		 __func__, shift, xscale);

	if (src_h > dst_h)
	        yscale = src_h;
	else
                yscale = dst_h;

        yscale <<= shift;
        yscale /= dst_h;
	yscale *= src_w;
	yscale /= hsync;

	pr_debug("%s: the right %d shifted yscale is %d.\n",
		 __func__, shift, yscale);

	rst = pclk;
	if (yscale > xscale)
		rst *= yscale;
	else
		rst *= xscale;

	rst >>= shift;

	/*
	 * There is one special case for the panels that have low
	 * v_back_porch (<=4), mdp clk should be fast enough to buffer
	 * 4 lines input during back porch time if scaling is
	 * required(FIR).
	 */
	if ((mfd->panel_info.lcdc.v_back_porch <= 4) &&
	    (src_h != dst_h) &&
	    (mfd->panel_info.lcdc.v_back_porch)) {
		u32 clk = 0;
		clk = 4 * (pclk >> shift) / mfd->panel_info.lcdc.v_back_porch;
		clk <<= shift;
		pr_debug("%s: mdp clk rate %d based on low vbp %d\n",
			 __func__, clk, mfd->panel_info.lcdc.v_back_porch);
		rst = (rst > clk) ? rst : clk;
	}

	/*
	 * If the calculated mdp clk is less than panel pixel clk,
	 * most likely due to upscaling, mdp clk rate will be set to
	 * greater than pclk. Now the driver uses 1.15 as the
	 * factor. Ideally this factor is passed from board file.
	 */
	if (rst < pclk) {
		rst = ((pclk >> shift) * 27 / 20) << shift;
		pr_debug("%s calculated mdp clk is less than pclk.\n",
			__func__);
	}
	pr_debug("%s: required mdp clk %d\n", __func__, (u32)rst);

	return (u32)rst;
}

static int mdp4_calc_req_blt(struct msm_fb_data_type *mfd,
			     struct mdp_overlay *req)
{
	int ret = 0;

	if (!req) {
		pr_err("%s: req is null!\n", __func__);
		return ret;
	}

	if (!mfd) {
		pr_err("%s: mfd is null!\n", __func__);
		return ret;
	}

	if (mdp4_calc_req_mdp_clk
		(mfd, req->src_rect.h, req->dst_rect.h,
		 req->src_rect.w, req->dst_rect.w) > mdp_max_clk)
		ret = -EINVAL;

	return ret;
}

static int mdp4_calc_pipe_mdp_clk(struct msm_fb_data_type *mfd,
				  struct mdp4_overlay_pipe *pipe)
{
	int ret = -EINVAL;

	if (!pipe) {
		pr_err("%s: pipe is null!\n", __func__);
		return ret;
	}
	if (!mfd) {
		pr_err("%s: mfd is null!\n", __func__);
		return ret;
	}

	/*
	 * Serveral special cases require the max mdp clk but cannot
	 * be explained by mdp clk equation.
	 */
	if (pipe->flags & MDP_DEINTERLACE) {
		pr_info("%s deinterlace requires max mdp clk.\n",
			__func__);
		pipe->req_clk = mdp_max_clk;
		return 0;
	}

	pr_debug("%s: src(w,h)(%d,%d),src(x,y)(%d,%d)\n",
		 __func__,  pipe->src_w, pipe->src_h, pipe->src_x, pipe->src_y);
	pr_debug("%s: dst(w,h)(%d,%d),dst(x,y)(%d,%d)\n",
		 __func__, pipe->dst_w, pipe->dst_h, pipe->dst_x, pipe->dst_y);

#ifdef MDP_ODD_RESOLUTION_CTRL
	if (pipe->check_odd_res) {
		pr_info("%s ENTER odd_resolution detected!! requires max mdp clk.pipe->pipe_ndx = %d\n",
			__func__, pipe->pipe_ndx);
		pipe->req_clk = mdp_max_clk;
		return 0;
	}
#endif
	pipe->req_clk = mdp4_calc_req_mdp_clk
		(mfd, pipe->src_h, pipe->dst_h, pipe->src_w, pipe->dst_w);

	pr_debug("%s: required mdp clk %d mixer %d pipe ndx %d\n",
		 __func__, pipe->req_clk, pipe->mixer_num, pipe->pipe_ndx);

	return 0;
}

static int mdp4_calc_pipe_mdp_bw(struct msm_fb_data_type *mfd,
			 struct mdp4_overlay_pipe *pipe)
{
	u32 fps;
	int ret = -EINVAL;
	u32 quota;
	u32 shift = 16;

	if (!pipe) {
		pr_err("%s: pipe is null!\n", __func__);
		return ret;
	}
	if (!mfd) {
		pr_err("%s: mfd is null!\n", __func__);
		return ret;
	}

	fps = mdp_get_panel_framerate(mfd);

	if ((pipe->bpp == 2) && (pipe->src_format == 0))
		quota = pipe->src_w * pipe->src_h * fps * pipe->bpp * 2;
	else if ((pipe->src_w == 1080) && (pipe->src_h <= 426))
		quota = pipe->src_w * pipe->src_h * fps * pipe->bpp * 3;
	else
		quota = pipe->src_w * pipe->src_h * fps * pipe->bpp;

	quota >>= shift;
	/* factor 2.00 for ab */
	pipe->bw_ab_quota = quota * MDP4_BW_AB_FACTOR / 100;
	/* factor 2.10 for ib */
	pipe->bw_ib_quota = quota * MDP4_BW_IB_FACTOR / 100;

	/* down scaling factor for ib */
	if ((pipe->dst_h) && (pipe->src_h) &&
	    (pipe->src_h > pipe->dst_h)) {
		u32 ib = quota;
		ib *= pipe->src_h;
		ib /= pipe->dst_h;
		pipe->bw_ib_quota = max((u64)ib, pipe->bw_ib_quota);
		pr_debug("%s: src_h=%d dst_h=%d mdp ib %u, ib_quota=%llu\n",
			 __func__, pipe->src_h, pipe->dst_h,
			 ib<<shift, pipe->bw_ib_quota<<shift);
	}

	pipe->bw_ab_quota <<= shift;
	pipe->bw_ib_quota <<= shift;

	pr_debug("%s: pipe ndx=%d src(h,w)(%d, %d) dst(%d, %d) fps=%d bpp=%d\n",
		 __func__, pipe->pipe_ndx,  pipe->src_h, pipe->src_w, pipe->dst_h, pipe->dst_w,
		 fps, pipe->bpp);
	pr_debug("%s: ab_quota=%llu ib_quota=%llu\n", __func__,
		 pipe->bw_ab_quota, pipe->bw_ib_quota);

	return 0;
}

int mdp4_calc_blt_mdp_bw(struct msm_fb_data_type *mfd,
			 struct mdp4_overlay_pipe *pipe)
{
	struct mdp4_overlay_perf *perf_req = &perf_request;
	u32 fps;
	int bpp;
	int ret = -EINVAL;
	u32 quota;
	u32 shift = 16;
	if (!pipe) {
		pr_err("%s: pipe is null!\n", __func__);
		return ret;
	}
	if (!mfd) {
		pr_err("%s: mfd is null!\n", __func__);
		return ret;
	}

	bpp = BLT_BPP;
	fps = mdp_get_panel_framerate(mfd);

	/* read and write bw*/
	quota = pipe->dst_w * pipe->dst_h * fps * bpp * 2;
	quota >>= shift;

	perf_req->mdp_ov_ab_bw[pipe->mixer_num] =
		quota * MDP4_BW_AB_FACTOR / 100;

	perf_req->mdp_ov_ib_bw[pipe->mixer_num] =
		quota * MDP4_BW_IB_FACTOR / 100;

	perf_req->mdp_ov_ab_bw[pipe->mixer_num] <<= shift;
	perf_req->mdp_ov_ib_bw[pipe->mixer_num] <<= shift;

	pr_debug("%s: pipe ndx=%d dst(h,w)(%d, %d) fps=%d bpp=%d\n",
		 __func__, pipe->pipe_ndx, pipe->dst_h, pipe->dst_w,
		 fps, bpp);
	pr_debug("%s: overlay=%d ab_bw=%llu ib_bw=%llu\n", __func__,
		 pipe->mixer_num,
		 perf_req->mdp_ov_ab_bw[pipe->mixer_num],
		 perf_req->mdp_ov_ib_bw[pipe->mixer_num]);

	return 0;
}
static int mdp4_axi_port_read_client_pipe(struct mdp4_overlay_pipe *pipe)
{
	u32 data = 0, port = 0;

	mdp_clk_ctrl(1);
	data = inpdw(MDP_BASE + 0x0404);
	mdp_clk_ctrl(0);

	if (pipe->pipe_ndx == 1) /* rgb1 */
		port = (data & 0x0010) ? 1 : 0;
	else if (pipe->pipe_ndx == 2) /* rgb2 */
		port = (data & 0x0080) ? 1 : 0;
	else if (pipe->pipe_ndx == 3) /* vg1 */
		port = (data & 0x0001) ? 1 : 0;
	else if (pipe->pipe_ndx == 4) /* vg2 */
		port = (data & 0x0004) ? 1 : 0;
	pr_debug("%s axi_rd=%x pipe_ndx=%d port=%d\n", __func__,
		data, pipe->pipe_ndx, port);
	return port;
}

static int mdp4_axi_port_read_client_mixer(int mixer)
{
	u32 data = 0, port = 0;

	mdp_clk_ctrl(1);
	data = inpdw(MDP_BASE + 0x0404);
	mdp_clk_ctrl(0);

	if (mixer == MDP4_MIXER0) /* dmap */
		port = (data & 0x1000) ? 1 : 0;
	else if (mixer == MDP4_MIXER1) /* dmae */
		port = (data & 0x80000) ? 1 : 0;
	pr_debug("%s axi_rd=%x mixer=%d port=%d\n",
		 __func__, data, mixer, port);
	return port;
}

static int mdp4_axi_port_write_client_mixer(int mixer)
{
	u32 data = 0, port = 0;

	mdp_clk_ctrl(1);
	data = inpdw(MDP_BASE + 0x0408);
	mdp_clk_ctrl(0);

	if (mixer == MDP4_MIXER0) /* dmap */
		port = (data & 0x0001) ? 1 : 0;
	else if (mixer == MDP4_MIXER1) /* dmae */
		port = (data & 0x0004) ? 1 : 0;
	else if (mixer == MDP4_MIXER2)
		port = (data & 0x0004) ? 1 : 0;
	pr_debug("%s axi_wr=%x mixer=%d port=%d\n",
		 __func__, data, mixer, port);
	return port;
}

int mdp4_overlay_mdp_perf_req(struct msm_fb_data_type *mfd)
{
	u32 worst_mdp_clk = 0;
	int i;
	struct mdp4_overlay_perf *perf_req = &perf_request;
	struct mdp4_overlay_pipe *pipe;
	u32 cnt = 0;
	int ret = -EINVAL;
	u64 ab_quota_total = 0, ib_quota_total = 0;
	u64 ab_quota_port0 = 0, ib_quota_port0 = 0;
	u64 ab_quota_port1 = 0, ib_quota_port1 = 0;
	u64 ib_quota_min = 0;
	int verysmallarea =0;
	int yuvcount =0;
	int src_h_total = 0;
	int src_w_total = 0;
	static u64 minimum_ab=0;
	static u64 minimum_ib=0;

	if (!mfd) {
		pr_err("%s: mfd is null!\n", __func__);
		return ret;
	}

	pipe = ctrl->plist;
	for (i = 0; i < MDP4_MIXER_MAX; i++)
		perf_req->use_ov_blt[i] = 0;

	for (i = 0; i < OVERLAY_PIPE_MAX; i++, pipe++) {

		if (!pipe)
			return ret;

		if (!pipe->pipe_used)
			continue;
		cnt++;
		if (worst_mdp_clk < pipe->req_clk)
			worst_mdp_clk = pipe->req_clk;

		if (pipe->req_clk > mdp_max_clk)
			perf_req->use_ov_blt[pipe->mixer_num] = 1;

		if (pipe->mixer_num == MDP4_MIXER2)
			perf_req->use_ov_blt[MDP4_MIXER2] = 1;

		if (pipe->pipe_type != OVERLAY_TYPE_BF) {
			ab_quota_total += pipe->bw_ab_quota;
			ib_quota_total += pipe->bw_ib_quota;
			if (mdp4_axi_port_read_client_pipe(pipe)) {
                   ab_quota_port1 += pipe->bw_ab_quota;
                   ib_quota_port1 += pipe->bw_ib_quota;
           } else {
                   ab_quota_port0 += pipe->bw_ab_quota;
                   ib_quota_port0 += pipe->bw_ib_quota;
           }
		} else {
			if (ib_quota_min == 0)
				ib_quota_min = pipe->bw_ib_quota;
			else
				ib_quota_min = min(ib_quota_min,
						   pipe->bw_ib_quota);
		}

		if(pipe->pipe_type == OVERLAY_TYPE_VIDEO){
			if(pipe->bpp==2)
				yuvcount++;
		}

		if (mfd->mdp_rev == MDP_REV_41) {
			/*
			 * writeback (blt) mode to provide work around
			 * for dsi cmd mode interface hardware bug.
			 */
			if (ctrl->panel_mode & MDP4_PANEL_DSI_CMD) {
				if (pipe->dst_x != 0)
					perf_req->use_ov_blt[MDP4_MIXER0] = 1;
			}
			if ((mfd->panel_info.xres > 1280) &&
			    (mfd->panel_info.type != DTV_PANEL)) {
				perf_req->use_ov_blt[MDP4_MIXER0] = 1;
			}
		}
	}

	perf_req->mdp_clk_rate = min(worst_mdp_clk, mdp_max_clk);
	perf_req->mdp_clk_rate = mdp_clk_round_rate(perf_req->mdp_clk_rate);

	for (i = 0; i < MDP4_MIXER_MAX; i++) {
		if (perf_req->use_ov_blt[i]) {
			ab_quota_total += perf_req->mdp_ov_ab_bw[i];
			ib_quota_total += perf_req->mdp_ov_ib_bw[i];
			if (mdp4_axi_port_read_client_mixer(i)) {
				ab_quota_port1 +=
					(perf_req->mdp_ov_ab_bw[i] >> 1);
				ib_quota_port1 +=
					(perf_req->mdp_ov_ib_bw[i] >> 1);
			} else {
				ab_quota_port0 +=
					(perf_req->mdp_ov_ab_bw[i] >> 1);
				ib_quota_port0 +=
					(perf_req->mdp_ov_ib_bw[i] >> 1);
			}
			if (mdp4_axi_port_write_client_mixer(i)) {
				ab_quota_port1 +=
					(perf_req->mdp_ov_ab_bw[i] >> 1);
				ib_quota_port1 +=
					(perf_req->mdp_ov_ib_bw[i] >> 1);
			} else {
				ab_quota_port0 +=
					(perf_req->mdp_ov_ab_bw[i] >> 1);
				ib_quota_port0 +=
					(perf_req->mdp_ov_ib_bw[i] >> 1);
			}
		}
	}

	if(minimum_ab == 0 ||minimum_ib == 0){
		minimum_ab = (1920*1080*4*60)>>16;
		minimum_ab = (minimum_ab*MDP4_BW_AB_FACTOR/100)<<16;
		minimum_ib = (1920*1080*4*60)>>16;
		minimum_ib = (minimum_ib*MDP4_BW_IB_FACTOR/100)<<16;
	}

	/*
	 * For small video + small rgb layers above them
	 * offset some bw
	 */
	if ((cnt >= 3) && (ab_quota_total < minimum_ab) && yuvcount == 1) {
		if ((verysmallarea + yuvcount) == (cnt - 1)) {
			ab_quota_total +=MDP_BUS_SCALE_AB_STEP;
			ib_quota_total +=MDP_BUS_SCALE_AB_STEP;
			ab_quota_port1 +=MDP_BUS_SCALE_AB_STEP;
			ib_quota_port1 +=MDP_BUS_SCALE_AB_STEP;
			ab_quota_port0 +=MDP_BUS_SCALE_AB_STEP;
			ib_quota_port0 +=MDP_BUS_SCALE_AB_STEP;
		} else {
			ab_quota_total= minimum_ab;
			ib_quota_total= minimum_ib;
			ab_quota_port1 = minimum_ab >> 1;
			ib_quota_port1 = minimum_ib >> 1;
			ab_quota_port0 = minimum_ab >> 1;
			ib_quota_port0 = minimum_ib >> 1;
		}
	}

	/*
	 * For Small RGB layers without video layer offset some
	 * bandwidth to prevent underruns
	 */
	if ((cnt >= 2) && (src_h_total * src_w_total < 1920 * 1080)
			&& (ab_quota_total < minimum_ab) && yuvcount == 0) {
		u64 bw_extra =  (minimum_ab - ab_quota_total) >>  1 ;
		int fact = ((int) (bw_extra>>16))/((int)(ab_quota_total>>16));

		/* Do not increase bw for layers which require more than 3 folds */
		if(fact <= 3 ) {
			ab_quota_total += bw_extra;
			ib_quota_total += bw_extra;
			ab_quota_port1 += bw_extra >> 1;
			ib_quota_port1 += bw_extra >> 1;
			ab_quota_port0 += bw_extra >> 1;
			ib_quota_port0 += bw_extra >> 1;
		}
	}

	perf_req->mdp_ab_bw = roundup(ab_quota_total, MDP_BUS_SCALE_AB_STEP);
	perf_req->mdp_ib_bw = roundup(ib_quota_total, MDP_BUS_SCALE_AB_STEP);

	perf_req->mdp_ab_port0_bw =
		roundup(ab_quota_port0, MDP_BUS_SCALE_AB_STEP);
	perf_req->mdp_ib_port0_bw =
		roundup(ib_quota_total, MDP_BUS_SCALE_AB_STEP);
	perf_req->mdp_ab_port1_bw =
		roundup(ab_quota_port1, MDP_BUS_SCALE_AB_STEP);
	perf_req->mdp_ib_port1_bw =
		roundup(ib_quota_total, MDP_BUS_SCALE_AB_STEP);

	pr_debug("%s %d: ab_quota_total=(%llu, %llu) ib_quota_total=(%llu, %llu)\n",
		 __func__, __LINE__,
		 ab_quota_total, perf_req->mdp_ab_bw,
		 ib_quota_total, perf_req->mdp_ib_bw);

	pr_debug("%s %d: ab_quota_port0=(%llu, %llu) ib_quota_port0=(%llu, %llu)\n",
		 __func__, __LINE__,
		 ab_quota_port0, perf_req->mdp_ab_port0_bw,
		 ib_quota_port0, perf_req->mdp_ib_port0_bw);

	pr_debug("%s %d: ab_quota_port1=(%llu, %llu) ib_quota_port1=(%llu, %llu)\n",
		 __func__, __LINE__,
		 ab_quota_port1, perf_req->mdp_ab_port1_bw,
		 ib_quota_port1, perf_req->mdp_ib_port1_bw);

	if (ab_quota_total > mdp_max_bw)
		pr_debug("%s: req ab bw=%llu is larger than max bw=%llu",
			__func__, ab_quota_total, mdp_max_bw);
	if (ib_quota_total > mdp_max_bw)
		pr_debug("%s: req ib bw=%llu is larger than max bw=%llu",
			__func__, ib_quota_total, mdp_max_bw);

	pr_debug("%s %d: pid %d cnt %d clk %d ov0_blt %d, ov1_blt %d\n",
		 __func__, __LINE__, current->pid, cnt,
		 perf_req->mdp_clk_rate,
		 perf_req->use_ov_blt[0],
		 perf_req->use_ov_blt[1]);

	return 0;
}

int mdp4_overlay_mdp_pipe_req(struct mdp4_overlay_pipe *pipe,
				  struct msm_fb_data_type *mfd)
{
	int ret = 0;

	if (mdp4_calc_pipe_mdp_clk(mfd, pipe)) {
		pr_err("%s unable to calc mdp pipe clk rate ret=%d\n",
		       __func__, ret);
		ret = -EINVAL;
	}
	if (mdp4_calc_pipe_mdp_bw(mfd, pipe)) {
		pr_err("%s unable to calc mdp pipe bandwidth ret=%d\n",
		       __func__, ret);
		ret = -EINVAL;
	}
	return ret;
}

void mdp4_overlay_mdp_perf_upd(struct msm_fb_data_type *mfd,
				  int flag)
{
	struct mdp4_overlay_perf *perf_req = &perf_request;
	struct mdp4_overlay_perf *perf_cur = &perf_current;

	pr_debug("%s %d: req mdp clk %d, cur mdp clk %d flag %d\n",
		 __func__, __LINE__,
		 perf_req->mdp_clk_rate,
		 perf_cur->mdp_clk_rate,
		 flag);

	if (!mdp4_extn_disp)
		perf_cur->use_ov_blt[1] = 0;

	if (flag) {
		if (perf_req->mdp_clk_rate > perf_cur->mdp_clk_rate) {
			mdp_set_core_clk(perf_req->mdp_clk_rate);
			pr_info("%s mdp clk is changed [%d] from %d to %d\n",
				__func__,
				flag,
				perf_cur->mdp_clk_rate,
				perf_req->mdp_clk_rate);
			perf_cur->mdp_clk_rate =
				perf_req->mdp_clk_rate;
		}
		if ((perf_req->mdp_ab_bw > perf_cur->mdp_ab_bw) ||
		    (perf_req->mdp_ib_bw > perf_cur->mdp_ib_bw)) {
			mdp_bus_scale_update_request
				(perf_req->mdp_ab_port0_bw,
				 perf_req->mdp_ib_port0_bw,
				 perf_req->mdp_ab_port1_bw,
				 perf_req->mdp_ib_port1_bw);
			pr_debug("%s mdp ab_bw is changed [%d] from %llu to %llu\n",
				__func__,
				flag,
				perf_cur->mdp_ab_bw,
				perf_req->mdp_ab_bw);
			pr_debug("%s mdp ib_bw is changed [%d] from %llu to %llu\n",
				__func__,
				flag,
				perf_cur->mdp_ib_bw,
				perf_req->mdp_ib_bw);
			perf_cur->mdp_ab_bw = perf_req->mdp_ab_bw;
			perf_cur->mdp_ib_bw = perf_req->mdp_ib_bw;
		}

		if ((mfd->panel_info.pdest == DISPLAY_1 &&
		     perf_req->use_ov_blt[0] && !perf_cur->use_ov_blt[0])) {
			if (mfd->panel_info.type == LCDC_PANEL ||
			    mfd->panel_info.type == LVDS_PANEL)
				mdp4_lcdc_overlay_blt_start(mfd);
			else if (mfd->panel_info.type == MIPI_VIDEO_PANEL)
				mdp4_dsi_video_blt_start(mfd);
			else if (ctrl->panel_mode & MDP4_PANEL_DSI_CMD)
				mdp4_dsi_cmd_blt_start(mfd);
			pr_info("%s mixer0 start blt [%d] from %d to %d.\n",
				__func__,
				flag,
				perf_cur->use_ov_blt[0],
				perf_req->use_ov_blt[0]);
			perf_cur->use_ov_blt[0] = perf_req->use_ov_blt[0];
		}
	} else {
		if (perf_req->mdp_clk_rate < perf_cur->mdp_clk_rate) {
			pr_info("%s mdp clk is changed [%d] from %d to %d\n",
				__func__,
				flag,
				perf_cur->mdp_clk_rate,
				perf_req->mdp_clk_rate);
			mdp_set_core_clk(perf_req->mdp_clk_rate);
			perf_cur->mdp_clk_rate =
				perf_req->mdp_clk_rate;
		}
		if (perf_req->mdp_ab_bw < perf_cur->mdp_ab_bw ||
		    perf_req->mdp_ib_bw < perf_cur->mdp_ib_bw) {
			mdp_bus_scale_update_request
				(perf_req->mdp_ab_port0_bw,
				 perf_req->mdp_ib_port0_bw,
				 perf_req->mdp_ab_port1_bw,
				 perf_req->mdp_ib_port1_bw);
			pr_debug("%s mdp ab bw is changed [%d] from %llu to %llu\n",
				__func__,
				flag,
				perf_cur->mdp_ab_bw,
				perf_req->mdp_ab_bw);
			pr_debug("%s mdp ib bw is changed [%d] from %llu to %llu\n",
				__func__,
				flag,
				perf_cur->mdp_ib_bw,
				perf_req->mdp_ib_bw);
			perf_cur->mdp_ab_bw = perf_req->mdp_ab_bw;
			perf_cur->mdp_ib_bw = perf_req->mdp_ib_bw;
		}

		if ((mfd->panel_info.pdest == DISPLAY_1 &&
		     !perf_req->use_ov_blt[0] && perf_cur->use_ov_blt[0])) {
			if (mfd->panel_info.type == LCDC_PANEL ||
			    mfd->panel_info.type == LVDS_PANEL)
				mdp4_lcdc_overlay_blt_stop(mfd);
			else if (mfd->panel_info.type == MIPI_VIDEO_PANEL)
				mdp4_dsi_video_blt_stop(mfd);
			else if (ctrl->panel_mode & MDP4_PANEL_DSI_CMD)
				mdp4_dsi_cmd_blt_stop(mfd);
			pr_info("%s mixer0 stop blt [%d] from %d to %d.\n",
				__func__,
				flag,
				perf_cur->use_ov_blt[0],
				perf_req->use_ov_blt[0]);
			perf_cur->use_ov_blt[0] = perf_req->use_ov_blt[0];
		}
	}

	return;
}

static int get_img(struct msmfb_data *img, struct fb_info *info,
	struct mdp4_overlay_pipe *pipe, unsigned int plane,
	unsigned long *start, unsigned long *len, struct file **srcp_file,
	int *p_need, struct ion_handle **srcp_ihdl)
{
	struct file *file;
	int put_needed, ret = 0, fb_num;
#ifdef CONFIG_ANDROID_PMEM
	unsigned long vstart;
#endif
	*p_need = 0;

	if (img->flags & MDP_BLIT_SRC_GEM) {
		*srcp_file = NULL;
		return kgsl_gem_obj_addr(img->memory_id, (int) img->priv,
					 start, len);
	}

	if (img->flags & MDP_MEMORY_ID_TYPE_FB) {
		file = fget_light(img->memory_id, &put_needed);
		if (file == NULL)
			return -EINVAL;

		pipe->flags |= MDP_MEMORY_ID_TYPE_FB;
		if (MAJOR(file->f_dentry->d_inode->i_rdev) == FB_MAJOR) {
			fb_num = MINOR(file->f_dentry->d_inode->i_rdev);
			if (get_fb_phys_info(start, len, fb_num,
				DISPLAY_SUBSYSTEM_ID)) {
				ret = -1;
			} else {
				*srcp_file = file;
				*p_need = put_needed;
			}
		} else
			ret = -1;
		if (ret)
			fput_light(file, put_needed);
		return ret;
	}

#ifdef CONFIG_MSM_MULTIMEDIA_USE_ION
	return mdp4_overlay_iommu_map_buf(img->memory_id, pipe, plane,
		start, len, srcp_ihdl);
#endif
#ifdef CONFIG_ANDROID_PMEM
	if (!get_pmem_file(img->memory_id, start, &vstart,
					    len, srcp_file))
		return 0;
	else
		return -EINVAL;
#endif
}

#ifdef CONFIG_FB_MSM_MIPI_DSI
int mdp4_overlay_3d_sbys(struct fb_info *info, struct msmfb_overlay_3d *req)
{
	struct msm_fb_data_type *mfd = (struct msm_fb_data_type *)info->par;
	int ret = -EPERM;

	if (mutex_lock_interruptible(&mfd->dma->ov_mutex))
		return -EINTR;

	if (ctrl->panel_mode & MDP4_PANEL_DSI_CMD) {
		mdp4_dsi_cmd_3d_sbys(mfd, req);
		ret = 0;
	} else if (ctrl->panel_mode & MDP4_PANEL_DSI_VIDEO) {
		mdp4_dsi_video_3d_sbys(mfd, req);
		ret = 0;
	}
	mutex_unlock(&mfd->dma->ov_mutex);

	return ret;
}
#else
int mdp4_overlay_3d_sbys(struct fb_info *info, struct msmfb_overlay_3d *req)
{
	/* do nothing */
	return -EPERM;
}
#endif

int mdp4_overlay_blt(struct fb_info *info, struct msmfb_overlay_blt *req)
{
	struct msm_fb_data_type *mfd = (struct msm_fb_data_type *)info->par;

	if (mfd == NULL)
		return -ENODEV;

	if (mutex_lock_interruptible(&mfd->dma->ov_mutex))
		return -EINTR;

	if (ctrl->panel_mode & MDP4_PANEL_DSI_CMD)
		mdp4_dsi_cmd_overlay_blt(mfd, req);
	else if (ctrl->panel_mode & MDP4_PANEL_DSI_VIDEO)
		mdp4_dsi_video_overlay_blt(mfd, req);
	else if (ctrl->panel_mode & MDP4_PANEL_LCDC)
		mdp4_lcdc_overlay_blt(mfd, req);
	else if (ctrl->panel_mode & MDP4_PANEL_MDDI)
		mdp4_mddi_overlay_blt(mfd, req);

	mutex_unlock(&mfd->dma->ov_mutex);

	return 0;
}

int mdp4_overlay_get(struct fb_info *info, struct mdp_overlay *req)
{
	struct mdp4_overlay_pipe *pipe;

	pipe = mdp4_overlay_ndx2pipe(req->id);
	if (pipe == NULL)
		return -ENODEV;

	*req = pipe->req_data;

	if (mdp4_overlay_borderfill_supported())
		req->flags |= MDP_BORDERFILL_SUPPORTED;

	return 0;
}

int mdp4_overlay_set(struct fb_info *info, struct mdp_overlay *req)
{
	struct msm_fb_data_type *mfd = (struct msm_fb_data_type *)info->par;
	int ret, mixer;
	struct mdp4_overlay_pipe *pipe;

	if (mfd == NULL) {
		pr_err("%s: mfd == NULL, -ENODEV\n", __func__);
		return -ENODEV;
	}

	if (info->node != 0 || mfd->cont_splash_done)	/* primary */
		if (mdp_fb_is_power_off(mfd))		/* suspended */
			return -EPERM;

	if (req->src.format == MDP_FB_FORMAT)
		req->src.format = mfd->fb_imgType;

	if (mutex_lock_interruptible(&mfd->dma->ov_mutex)) {
		pr_err("%s: mutex_lock_interruptible, -EINTR\n", __func__);
		return -EINTR;
	}

	mixer = mfd->panel_info.pdest;	/* DISPLAY_1 or DISPLAY_2 */

	ret = mdp4_calc_req_blt(mfd, req);

	if (ret < 0) {
		mutex_unlock(&mfd->dma->ov_mutex);
		pr_err("%s: blt mode is required! ret=%d\n", __func__, ret);
		return ret;
	}

	ret = mdp4_overlay_req2pipe(req, mixer, &pipe, mfd);

	if (ret < 0) {
		mutex_unlock(&mfd->dma->ov_mutex);
		pr_err("%s: mdp4_overlay_req2pipe, ret=%d\n", __func__, ret);
		return ret;
	}

	/* if blt mode is required when bypass mode, it returns error */
	if ((pipe->flags & MDP_BACKEND_COMPOSITION) &&
		mdp4_overlay_format2type(pipe->src_format) == OVERLAY_TYPE_RGB &&
		mdp4_calc_pipe_mdp_clk(mfd, pipe) == 0 &&
		pipe->req_clk > mdp_max_clk)	{
		if (req->id == MSMFB_NEW_REQUEST)
			mdp4_overlay_pipe_free(pipe, 1);//need_check
		mutex_unlock(&mfd->dma->ov_mutex);
		return -EINVAL;
	}

	if (pipe->flags & MDP_SECURE_OVERLAY_SESSION) {
		mdp4_map_sec_resource(mfd);
		mfd->sec_active = TRUE;
	}

	/* return id back to user */
	req->id = pipe->pipe_ndx;	/* pipe_ndx start from 1 */
	pipe->req_data = *req;		/* keep original req */

	if (!IS_ERR_OR_NULL(mfd->iclient)) {
		pr_debug("pipe->flags 0x%x\n", pipe->flags);
		if (pipe->flags & MDP_SECURE_OVERLAY_SESSION) {
			mfd->mem_hid &= ~BIT(ION_IOMMU_HEAP_ID);
			mfd->mem_hid |= ION_SECURE;
		} else {
			mfd->mem_hid |= BIT(ION_IOMMU_HEAP_ID);
			mfd->mem_hid &= ~ION_SECURE;
		}
	}

	mdp4_stat.overlay_set[pipe->mixer_num]++;

	if (pipe->flags & MDP_OVERLAY_PP_CFG_EN) {
		if (pipe->pipe_num <= OVERLAY_PIPE_VG2)
			memcpy(&pipe->pp_cfg, &req->overlay_pp_cfg,
					sizeof(struct mdp_overlay_pp_params));
		else
			pr_debug("%s: RGB Pipes don't support CSC/QSEED\n",
								__func__);
	}

	if (hdmi_prim_display)
		fill_black_screen(FALSE, pipe->pipe_num, pipe->mixer_num);

	mdp4_overlay_mdp_pipe_req(pipe, mfd);

	mutex_unlock(&mfd->dma->ov_mutex);

	return 0;
}

int mdp4_overlay_unset_mixer(int mixer)
{
	struct mdp4_overlay_pipe *pipe;
	int i, cnt = 0;

	/* free pipe besides base layer pipe */
	for (i = MDP4_MIXER_STAGE3; i > MDP4_MIXER_STAGE_BASE; i--) {
		pipe = ctrl->stage[mixer][i];
		if (pipe == NULL)
			continue;

		pipe->flags &= ~MDP_OV_PLAY_NOWAIT;
		mdp4_overlay_reg_flush(pipe, 1);
		mdp4_mixer_stage_down(pipe, 1);
		mdp4_overlay_pipe_free(pipe, 1);
		cnt++;
	}

	return cnt;
}

int mdp4_overlay_unset(struct fb_info *info, int ndx)
{
	struct msm_fb_data_type *mfd = (struct msm_fb_data_type *)info->par;
	struct mdp4_overlay_pipe *pipe;
	int i, mixer;

	if (mfd == NULL)
		return -ENODEV;

	if (mutex_lock_interruptible(&mfd->dma->ov_mutex))
		return -EINTR;

	pipe = mdp4_overlay_ndx2pipe(ndx);
	xlog(__func__, pipe->pipe_ndx, 0, 0, 0, 0);
	if (pipe == NULL) {
		mutex_unlock(&mfd->dma->ov_mutex);
		return -ENODEV;
	}

	if (pipe->pipe_type == OVERLAY_TYPE_BF) {
		mdp4_overlay_borderfill_stage_down(pipe);
		mutex_unlock(&mfd->dma->ov_mutex);
		return 0;
	}

	mixer = pipe->mixer_num;

	if (pipe->mixer_num == MDP4_MIXER2)
		ctrl->mixer2_played = 0;
	else if (pipe->mixer_num == MDP4_MIXER1)
		ctrl->mixer1_played = 0;
	else {
		/* mixer 0 */
		ctrl->mixer0_played = 0;
		if (ctrl->panel_mode & MDP4_PANEL_MDDI) {
			if (!mdp_fb_is_power_off(mfd))
				mdp4_mddi_blt_dmap_busy_wait(mfd);
		}
	}

	mdp4_overlay_reg_flush(pipe, 1);
	mdp4_mixer_stage_down(pipe, 0);

	if (pipe->mixer_num == MDP4_MIXER0) {
		if (ctrl->panel_mode & MDP4_PANEL_MDDI) {
			if (!mdp_fb_is_power_off(mfd))
				mdp4_mddi_overlay_restore();
		}
	} else {	/* mixer1, DTV, ATV */
		if (ctrl->panel_mode & MDP4_PANEL_DTV) {
			if (hdmi_prim_display)
				fill_black_screen(TRUE, pipe->pipe_num,
					pipe->mixer_num);
			mdp4_overlay_dtv_unset(mfd, pipe);
		}
	}

	mdp4_stat.overlay_unset[pipe->mixer_num]++;

	if (pipe->flags & MDP_SECURE_OVERLAY_SESSION)
		mfd->sec_active = FALSE;
	mdp4_overlay_pipe_free(pipe, 0);

	/* mdp4_mixer_stage_down will remove pipe for mixer 1 and 2*/
	if (mixer > MDP4_MIXER0 && !hdmi_prim_display) {
		for (i = MDP4_MIXER_STAGE_BASE; i < MDP4_MIXER_STAGE_MAX; i++) {
			pipe = ctrl->stage[mixer][i];
			if (pipe && pipe->pipe_type != OVERLAY_TYPE_BF)
				break;
		}
		/* only BF pipe is connected */
		if (i == MDP4_MIXER_STAGE_MAX) {
			/* make sure the operation has finished.*/
			msleep(20);
			msm_fb_release_timeline(mfd);
		}
	}

	mutex_unlock(&mfd->dma->ov_mutex);

	return 0;
}

int mdp4_overlay_wait4vsync(struct fb_info *info)
{
	if (!hdmi_prim_display && info->node == 0) {
		if (ctrl->panel_mode & MDP4_PANEL_DSI_VIDEO)
			mdp4_dsi_video_wait4vsync(0);
		else if (ctrl->panel_mode & MDP4_PANEL_DSI_CMD)
			mdp4_dsi_cmd_wait4vsync(0);
		else if (ctrl->panel_mode & MDP4_PANEL_LCDC)
			mdp4_lcdc_wait4vsync(0);
	} else if (hdmi_prim_display || info->node == 1) {
		mdp4_dtv_wait4vsync(0);
	}

	return 0;
}

int mdp4_overlay_vsync_ctrl(struct fb_info *info, int enable)
{
	int cmd;
	struct msm_fb_data_type *mfd = (struct msm_fb_data_type *)info->par;

	if (mfd == NULL)
		return -ENODEV;

	if (mdp_fb_is_power_off(mfd))
		return -EINVAL;

	if (enable)
		cmd = 1;
	else
		cmd = 0;

	if (!hdmi_prim_display && info->node == 0) {
		if (ctrl->panel_mode & MDP4_PANEL_DSI_VIDEO)
			mdp4_dsi_video_vsync_ctrl(info, cmd);
		else if (ctrl->panel_mode & MDP4_PANEL_DSI_CMD)
			mdp4_dsi_cmd_vsync_ctrl(info, cmd);
		else if (ctrl->panel_mode & MDP4_PANEL_LCDC)
			mdp4_lcdc_vsync_ctrl(info, cmd);
	} else if (hdmi_prim_display || info->node == 1)
		mdp4_dtv_vsync_ctrl(info, cmd);

	return 0;
}


struct tile_desc {
	uint32 width;  /* tile's width */
	uint32 height; /* tile's height */
	uint32 row_tile_w; /* tiles per row's width */
	uint32 row_tile_h; /* tiles per row's height */
};

void tile_samsung(struct tile_desc *tp)
{
	/*
	 * each row of samsung tile consists of two tiles in height
	 * and two tiles in width which means width should align to
	 * 64 x 2 bytes and height should align to 32 x 2 bytes.
	 * video decoder generate two tiles in width and one tile
	 * in height which ends up height align to 32 X 1 bytes.
	 */
	tp->width = 64;		/* 64 bytes */
	tp->row_tile_w = 2;	/* 2 tiles per row's width */
	tp->height = 32;	/* 32 bytes */
	tp->row_tile_h = 1;	/* 1 tiles per row's height */
}

uint32 tile_mem_size(struct mdp4_overlay_pipe *pipe, struct tile_desc *tp)
{
	uint32 tile_w, tile_h;
	uint32 row_num_w, row_num_h;


	tile_w = tp->width * tp->row_tile_w;
	tile_h = tp->height * tp->row_tile_h;

	row_num_w = (pipe->src_width + tile_w - 1) / tile_w;
	row_num_h = (pipe->src_height + tile_h - 1) / tile_h;
	return ((row_num_w * row_num_h * tile_w * tile_h) + 8191) & ~8191;
}

int mdp4_overlay_play_wait(struct fb_info *info, struct msmfb_overlay_data *req)
{
	return 0;
}

/*
 * mdp4_overlay_dma_commit: called from dma_done isr
 * No mutex/sleep allowed
 */
void mdp4_overlay_dma_commit(int mixer)
{
	/*
	* non double buffer register update here
	* perf level, new clock rate should be done here
	*/
}

/*
 * mdp4_overlay_vsync_commit: called from tasklet context
 * No mutex/sleep allowed
 */
void mdp4_overlay_vsync_commit(struct mdp4_overlay_pipe *pipe)
{
	if (pipe->pipe_type == OVERLAY_TYPE_VIDEO)
		mdp4_overlay_vg_setup(pipe);	/* video/graphic pipe */
	else
		mdp4_overlay_rgb_setup(pipe);	/* rgb pipe */

	pr_debug("%s: pipe=%x ndx=%d num=%d used=%d\n", __func__,
		(int) pipe, pipe->pipe_ndx, pipe->pipe_num, pipe->pipe_used);
	mdp4_overlay_reg_flush(pipe, 1);
	mdp4_mixer_stage_up(pipe, 0);
}

int mdp4_overlay_play(struct fb_info *info, struct msmfb_overlay_data *req)
{
	struct msm_fb_data_type *mfd = (struct msm_fb_data_type *)info->par;
	struct msmfb_data *img;
	struct mdp4_overlay_pipe *pipe;
	ulong start, addr;
	ulong len = 0;
	struct ion_handle *srcp0_ihdl = NULL;
	struct ion_handle *srcp1_ihdl = NULL, *srcp2_ihdl = NULL;
	uint32_t overlay_version = 0;
	int ret = 0;

	if (mfd == NULL)
		return -ENODEV;

	pipe = mdp4_overlay_ndx2pipe(req->id);
	if (pipe == NULL) {
		mdp4_stat.err_play++;
		return -ENODEV;
	}

	if (pipe->pipe_type == OVERLAY_TYPE_BF) {
		mdp4_overlay_borderfill_stage_up(pipe);
		mdp4_mixer_stage_commit(pipe->mixer_num);
		return 0;
	}

	mutex_lock(&mfd->dma->ov_mutex);

	img = &req->data;
	get_img(img, info, pipe, 0, &start, &len, &pipe->srcp0_file,
		&pipe->put0_need, &srcp0_ihdl);
	if (len == 0) {
		pr_err("%s: pmem Error\n", __func__);
		ret = -1;
		goto end;
	}

	addr = start + img->offset;
	pipe->srcp0_addr = addr;
	pipe->srcp0_ystride = pipe->src_width * pipe->bpp;


	pr_debug("%s: mixer=%d ndx=%x addr=%x flags=%x pid=%d\n", __func__,
		pipe->mixer_num, pipe->pipe_ndx, (int)addr, pipe->flags,
							current->pid);

	if ((req->version_key & VERSION_KEY_MASK) == 0xF9E8D700)
		overlay_version = (req->version_key & ~VERSION_KEY_MASK);

	if (pipe->fetch_plane == OVERLAY_PLANE_PSEUDO_PLANAR) {
		if (overlay_version > 0) {
			img = &req->plane1_data;
			get_img(img, info, pipe, 1, &start, &len,
				&pipe->srcp1_file, &pipe->put1_need,
				&srcp1_ihdl);
			if (len == 0) {
				pr_err("%s: Error to get plane1\n", __func__);
				ret = -EINVAL;
				goto end;
			}
			pipe->srcp1_addr = start + img->offset;
		} else if (pipe->frame_format ==
				MDP4_FRAME_FORMAT_VIDEO_SUPERTILE) {
			struct tile_desc tile;

			tile_samsung(&tile);
			pipe->srcp1_addr = addr + tile_mem_size(pipe, &tile);
		} else {
			pipe->srcp1_addr = addr + (pipe->src_width *
						pipe->src_height);
		}
		pipe->srcp0_ystride = pipe->src_width;
		if ((pipe->src_format == MDP_Y_CRCB_H1V1) ||
			(pipe->src_format == MDP_Y_CBCR_H1V1) ||
			(pipe->src_format == MDP_Y_CRCB_H1V2) ||
			(pipe->src_format == MDP_Y_CBCR_H1V2)) {
			if (pipe->src_width > YUV_444_MAX_WIDTH)
				pipe->srcp1_ystride = pipe->src_width << 2;
			else
				pipe->srcp1_ystride = pipe->src_width << 1;
		} else
			pipe->srcp1_ystride = pipe->src_width;

	} else if (pipe->fetch_plane == OVERLAY_PLANE_PLANAR) {
		if (overlay_version > 0) {
			img = &req->plane1_data;
			get_img(img, info, pipe, 1, &start, &len,
				&pipe->srcp1_file, &pipe->put1_need,
				&srcp1_ihdl);
			if (len == 0) {
				pr_err("%s: Error to get plane1\n", __func__);
				ret = -EINVAL;
				goto end;
			}
			pipe->srcp1_addr = start + img->offset;

			img = &req->plane2_data;
			get_img(img, info, pipe, 2, &start, &len,
				&pipe->srcp2_file, &pipe->put2_need,
				&srcp2_ihdl);
			if (len == 0) {
				pr_err("%s: Error to get plane2\n", __func__);
				ret = -EINVAL;
				goto end;
			}
			pipe->srcp2_addr = start + img->offset;
		} else {
			if (pipe->src_format == MDP_Y_CR_CB_GH2V2) {
				addr += (ALIGN(pipe->src_width, 16) *
					pipe->src_height);
				pipe->srcp1_addr = addr;
				addr += ((ALIGN((pipe->src_width / 2), 16)) *
					(pipe->src_height / 2));
				pipe->srcp2_addr = addr;
			} else {
				addr += (pipe->src_width * pipe->src_height);
				pipe->srcp1_addr = addr;
				addr += ((pipe->src_width / 2) *
					(pipe->src_height / 2));
				pipe->srcp2_addr = addr;
			}
		}
		/* mdp planar format expects Cb in srcp1 and Cr in p2 */
		if ((pipe->src_format == MDP_Y_CR_CB_H2V2) ||
			(pipe->src_format == MDP_Y_CR_CB_GH2V2))
			swap(pipe->srcp1_addr, pipe->srcp2_addr);

		if (pipe->src_format == MDP_Y_CR_CB_GH2V2) {
			pipe->srcp0_ystride = ALIGN(pipe->src_width, 16);
			pipe->srcp1_ystride = ALIGN(pipe->src_width / 2, 16);
			pipe->srcp2_ystride = ALIGN(pipe->src_width / 2, 16);
		} else {
			pipe->srcp0_ystride = pipe->src_width;
			pipe->srcp1_ystride = pipe->src_width / 2;
			pipe->srcp2_ystride = pipe->src_width / 2;
		}
	}

	mdp4_overlay_mdp_perf_req(mfd);

	if (pipe->mixer_num == MDP4_MIXER0) {
		if (ctrl->panel_mode & MDP4_PANEL_DSI_CMD) {
			/* cndx = 0 */
			mdp4_dsi_cmd_pipe_queue(0, pipe);
		} else if (ctrl->panel_mode & MDP4_PANEL_DSI_VIDEO) {
			/* cndx = 0 */
			mdp4_dsi_video_pipe_queue(0, pipe);
		} else if (ctrl->panel_mode & MDP4_PANEL_LCDC) {
			/* cndx = 0 */
			mdp4_lcdc_pipe_queue(0, pipe);
		}
	} else if (pipe->mixer_num == MDP4_MIXER1) {
		if (ctrl->panel_mode & MDP4_PANEL_DTV)
			mdp4_dtv_pipe_queue(0, pipe);/* cndx = 0 */
	} else if (pipe->mixer_num == MDP4_MIXER2) {
		ctrl->mixer2_played++;
		if (ctrl->panel_mode & MDP4_PANEL_WRITEBACK)
			mdp4_wfd_pipe_queue(0, pipe);/* cndx = 0 */
	}

end:
	mutex_unlock(&mfd->dma->ov_mutex);

	return ret;
}

int mdp4_overlay_commit(struct fb_info *info)
{
	int ret = 0;
	struct msm_fb_data_type *mfd = (struct msm_fb_data_type *)info->par;
	int mixer;

	if (mfd == NULL)
		return -ENODEV;

	if (mdp_fb_is_power_off(mfd))
		ret = -EINVAL;

	mixer = mfd->panel_info.pdest;	/* DISPLAY_1 or DISPLAY_2 */

	mutex_lock(&mfd->dma->ov_mutex);
	mdp4_overlay_mdp_perf_upd(mfd, 1);

	msm_fb_wait_for_fence(mfd);
	xlog(__func__, 1, 0, 0, 0, 0);

	switch (mfd->panel.type) {
	case MIPI_CMD_PANEL:
		mdp4_dsi_cmd_pipe_commit(0, 1);
		break;
	case MIPI_VIDEO_PANEL:
		mdp4_dsi_video_pipe_commit(0, 1);
		break;
	case LVDS_PANEL:
	case LCDC_PANEL:
		mdp4_lcdc_pipe_commit(0, 1);
		break;
	case DTV_PANEL:
		mdp4_dtv_pipe_commit(0, 1);
		break;
	case WRITEBACK_PANEL:
		mdp4_wfd_pipe_commit(mfd, 0, 1);
		break;
	default:
		pr_err("Panel Not Supported for Commit");
		ret = -EINVAL;
		break;
	}

	xlog(__func__, 0, 0, 0, 0, 0);
	msm_fb_signal_timeline(mfd);
	xlog(__func__, 1, 0, 0, 0, 0);

	mdp4_overlay_mdp_perf_upd(mfd, 0);
	mdp4_unmap_sec_resource(mfd);
	mutex_unlock(&mfd->dma->ov_mutex);
	return ret;
}

struct msm_iommu_ctx {
	char *name;
	int  domain;
};

static struct msm_iommu_ctx msm_iommu_ctx_names[] = {
	/* Display read*/
	{
		.name = "mdp_port0_cb0",
		.domain = DISPLAY_READ_DOMAIN,
	},
	/* Display read*/
	{
		.name = "mdp_port0_cb1",
		.domain = DISPLAY_READ_DOMAIN,
	},
	/* Display write */
	{
		.name = "mdp_port1_cb0",
		.domain = DISPLAY_READ_DOMAIN,
	},
	/* Display write */
	{
		.name = "mdp_port1_cb1",
		.domain = DISPLAY_READ_DOMAIN,
	},
};

static struct msm_iommu_ctx msm_iommu_split_ctx_names[] = {
	/* Display read*/
	{
		.name = "mdp_port0_cb0",
		.domain = DISPLAY_READ_DOMAIN,
	},
	/* Display read*/
	{
		.name = "mdp_port0_cb1",
		.domain = DISPLAY_WRITE_DOMAIN,
	},
	/* Display write */
	{
		.name = "mdp_port1_cb0",
		.domain = DISPLAY_READ_DOMAIN,
	},
	/* Display write */
	{
		.name = "mdp_port1_cb1",
		.domain = DISPLAY_WRITE_DOMAIN,
	},
};

void mdp4_iommu_attach(void)
{
	static int done;
	struct msm_iommu_ctx *ctx_names;
	struct iommu_domain *domain;
	int i, arr_size;

	if (!done) {
		if (mdp_iommu_split_domain) {
			ctx_names = msm_iommu_split_ctx_names;
			arr_size = ARRAY_SIZE(msm_iommu_split_ctx_names);
		} else {
			ctx_names = msm_iommu_ctx_names;
			arr_size = ARRAY_SIZE(msm_iommu_ctx_names);
		}

		for (i = 0; i < arr_size; i++) {
			int domain_idx;
			struct device *ctx = msm_iommu_get_ctx(
				ctx_names[i].name);

			if (!ctx)
				continue;

			domain_idx = ctx_names[i].domain;

			domain = msm_get_iommu_domain(domain_idx);
			if (!domain)
				continue;

			if (iommu_attach_device(domain,	ctx)) {
				WARN(1, "%s: could not attach domain %d to context %s."
					" iommu programming will not occur.\n",
					__func__, domain_idx,
					ctx_names[i].name);
				continue;
			}
		}
		done = 1;
	}
}

int mdp4_v4l2_overlay_set(struct fb_info *info, struct mdp_overlay *req,
	struct mdp4_overlay_pipe **ppipe)
{
	struct mdp4_overlay_pipe *pipe;
	int err;
	struct msm_fb_data_type *mfb = info->par;

	req->z_order = 0;
	req->id = MSMFB_NEW_REQUEST;
	req->is_fg = false;
	req->alpha = 0xff;
	xlog(__func__, req->id, req->flags, req->src.format, 0, 0);
	err = mdp4_overlay_req2pipe(req, MDP4_MIXER0, &pipe, mfb);
	if (err < 0) {
		pr_err("%s:Could not allocate MDP overlay pipe\n", __func__);
		return err;
	}

	mdp4_mixer_blend_setup(pipe->mixer_num);
	*ppipe = pipe;

	return 0;
}

void mdp4_v4l2_overlay_clear(struct mdp4_overlay_pipe *pipe)
{
	mdp4_overlay_reg_flush(pipe, 1);
	mdp4_mixer_stage_down(pipe, 1);
	mdp4_overlay_pipe_free(pipe, 1);
}

int mdp4_v4l2_overlay_play(struct fb_info *info, struct mdp4_overlay_pipe *pipe,
	unsigned long srcp0_addr, unsigned long srcp1_addr,
	unsigned long srcp2_addr)
{
	struct msm_fb_data_type *mfd = info->par;
	int err;

	if (mutex_lock_interruptible(&mfd->dma->ov_mutex))
		return -EINTR;

	switch (pipe->src_format) {
	case MDP_Y_CR_CB_H2V2:
		/* YUV420 */
		pipe->srcp0_addr = srcp0_addr;
		pipe->srcp0_ystride = pipe->src_width;
		/*
		 * For YUV420, the luma plane is 1 byte per pixel times
		 * num of pixels in the image Also, the planes are
		 * switched in MDP, srcp2 is actually first chroma plane
		 */
		pipe->srcp2_addr = srcp1_addr ? srcp1_addr :
		pipe->srcp0_addr + (pipe->src_width * pipe->src_height);
		pipe->srcp2_ystride = pipe->src_width/2;
		/*
		 * The chroma planes are half the size of the luma
		 * planes
		 */
		pipe->srcp1_addr = srcp2_addr ? srcp2_addr :
		pipe->srcp2_addr +
			(pipe->src_width * pipe->src_height / 4);
		pipe->srcp1_ystride = pipe->src_width/2;
		break;
	case MDP_Y_CRCB_H2V2:
		/* NV12 */
		pipe->srcp0_addr = srcp0_addr;
		pipe->srcp0_ystride = pipe->src_width;
		pipe->srcp1_addr = srcp1_addr ? srcp1_addr :
		pipe->srcp0_addr +
			(pipe->src_width * pipe->src_height);
		pipe->srcp1_ystride = pipe->src_width;
		break;
	default:
		pr_err("%s: format (%u) is not supported\n", __func__,
				pipe->src_format);
		err = -EINVAL;
		goto done;
	}

	pr_debug("%s: pipe ndx=%d stage=%d format=%x\n", __func__,
		pipe->pipe_ndx, pipe->mixer_stage, pipe->src_format);

	if (pipe->pipe_type == OVERLAY_TYPE_VIDEO)
		mdp4_overlay_vg_setup(pipe);
	else
		mdp4_overlay_rgb_setup(pipe);

	if (ctrl->panel_mode & MDP4_PANEL_LCDC)
		mdp4_overlay_reg_flush(pipe, 1);

	mdp4_mixer_stage_up(pipe, 0); /* mixer stage commit commits this */
	mdp4_mixer_stage_commit(pipe->mixer_num);

#ifdef V4L2_VSYNC
	/*
	 * TODO: incorporate v4l2 into vsycn driven mechanism
	 */
	if (ctrl->panel_mode & MDP4_PANEL_LCDC) {
		mdp4_overlay_lcdc_vsync_push(mfd, pipe);
	} else {
#ifdef CONFIG_FB_MSM_MIPI_DSI
		if (ctrl->panel_mode & MDP4_PANEL_DSI_CMD) {
			mdp4_dsi_cmd_dma_busy_wait(mfd);
			mdp4_dsi_cmd_kickoff_video(mfd, pipe);
		}
#else
		if (ctrl->panel_mode & MDP4_PANEL_MDDI) {
			mdp4_mddi_dma_busy_wait(mfd);
			mdp4_mddi_kickoff_video(mfd, pipe);
		}
#endif
	}
#endif

done:
	mutex_unlock(&mfd->dma->ov_mutex);
	return err;
}

#ifdef CONFIG_USE_VFB
struct ion_client *mdp4_get_ion_client(void)
{
	return display_iclient;
}
#endif

int mdp4_update_base_blend(struct msm_fb_data_type *mfd,
			struct mdp_blend_cfg *mdp_blend_cfg)
{
	int ret = 0;
	u32 mixer_num;
	struct blend_cfg *blend;
	mixer_num = mdp4_get_mixer_num(mfd->panel_info.type);
	if (!ctrl)
		return -EPERM;
	blend = &ctrl->blend[mixer_num][MDP4_MIXER_STAGE_BASE];
	if (mdp_blend_cfg->is_premultiplied) {
		blend->bg_alpha = 0xFF;
		blend->op = MDP4_BLEND_FG_ALPHA_BG_CONST;
	} else {
		blend->op = MDP4_BLEND_FG_ALPHA_FG_PIXEL;
		blend->bg_alpha = 0;
	}
	return ret;
}
int mdp4_overlay_reset()
{
#if 0
	memset(&perf_request, 0, sizeof(perf_request));
	memset(&perf_current, 0, sizeof(perf_current));
#endif
	return 0;
}
