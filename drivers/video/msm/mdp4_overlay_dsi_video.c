/* Copyright (c) 2010-2013, The Linux Foundation. All rights reserved.
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
#include <linux/hrtimer.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/semaphore.h>
#include <linux/spinlock.h>
#include <linux/fb.h>
#include <linux/msm_mdp.h>
#include <linux/ktime.h>
#include <linux/wakelock.h>
#include <linux/time.h>
#include <asm/system.h>
#include <asm/mach-types.h>
#include <mach/hardware.h>
#include "mdp.h"
#include "msm_fb.h"
#include "mdp4.h"
#include "mipi_dsi.h"

#include <mach/iommu_domains.h>

#define DSI_VIDEO_BASE	0xE0000

static int first_pixel_start_x;
static int first_pixel_start_y;
static int dsi_video_enabled;
static int vsync_irq_cnt;

#define MAX_CONTROLLER	1

static struct vsycn_ctrl {
	struct device *dev;
	int inited;
	int update_ndx;
	int ov_koff;
	int ov_done;
	atomic_t suspend;
	int blt_change;
	int blt_free;
	int blt_ctrl;
	int sysfs_created;
	struct mutex update_lock;
	struct completion ov_comp;
	struct completion dmap_comp;
	spinlock_t spin_lock;
	struct msm_fb_data_type *mfd;
	struct mdp4_overlay_pipe *base_pipe;
	struct vsync_update vlist[2];
	int vsync_irq_enabled;
	ktime_t vsync_time;
	wait_queue_head_t wait_queue_internal;
	wait_queue_head_t wait_queue;
} vsync_ctrl_db[MAX_CONTROLLER];

static void vsync_irq_enable(int intr, int term)
{
	unsigned long flag;

	spin_lock_irqsave(&mdp_spin_lock, flag);
	outp32(MDP_INTR_CLEAR, intr);
	mdp_intr_mask |= intr;
	outp32(MDP_INTR_ENABLE, mdp_intr_mask);
	mdp_enable_irq(term);
	spin_unlock_irqrestore(&mdp_spin_lock, flag);
	pr_debug("%s: IRQ-en done, term=%x\n", __func__, term);
}

static void vsync_irq_disable(int intr, int term)
{
	unsigned long flag;

	spin_lock_irqsave(&mdp_spin_lock, flag);
	outp32(MDP_INTR_CLEAR, intr);
	mdp_intr_mask &= ~intr;
	outp32(MDP_INTR_ENABLE, mdp_intr_mask);
	mdp_disable_irq_nosync(term);
	spin_unlock_irqrestore(&mdp_spin_lock, flag);
	pr_debug("%s: IRQ-dis done, term=%x\n", __func__, term);
}

void mdp4_overlay_dsi_video_start(void)
{
	if (!dsi_video_enabled) {
		/* enable DSI block */
		mdp4_iommu_attach();
		mdp_pipe_ctrl(MDP_OVERLAY0_BLOCK, MDP_BLOCK_POWER_ON, FALSE);
		MDP_OUTP(MDP_BASE + DSI_VIDEO_BASE, 1);
		dsi_video_enabled = 1;
		xlog(__func__, 0, 0, 0, 0, 0);
	}
}

/*
 * mdp4_dsi_video_pipe_queue:
 * called from thread context
 */
void mdp4_dsi_video_pipe_queue(int cndx, struct mdp4_overlay_pipe *pipe)
{
	struct vsycn_ctrl *vctrl;
	struct vsync_update *vp;
	struct mdp4_overlay_pipe *pp;
	int undx;

	if (cndx >= MAX_CONTROLLER) {
		pr_err("%s: out or range: cndx=%d\n", __func__, cndx);
		return;
	}

	vctrl = &vsync_ctrl_db[cndx];

	if (atomic_read(&vctrl->suspend) > 0)
		return;

	mutex_lock(&vctrl->update_lock);
	undx =  vctrl->update_ndx;
	vp = &vctrl->vlist[undx];

	pp = &vp->plist[pipe->pipe_ndx - 1];	/* ndx start form 1 */

	pr_debug("%s: vndx=%d pipe=%x ndx=%d num=%d pid=%d\n",
		 __func__, undx, (int)pipe, pipe->pipe_ndx, pipe->pipe_num,
		current->pid);

	*pp = *pipe;	/* clone it */
	vp->update_cnt++;
	mutex_unlock(&vctrl->update_lock);
	mdp4_stat.overlay_play[pipe->mixer_num]++;
}

static void mdp4_dsi_video_pipe_clean(struct vsync_update *vp)
{
	struct mdp4_overlay_pipe *pipe;
	int i;

	pipe = vp->plist;
	for (i = 0; i < OVERLAY_PIPE_MAX; i++, pipe++) {
		if (pipe->pipe_used) {
			mdp4_overlay_iommu_pipe_free(pipe->pipe_ndx, 0);
			pipe->pipe_used = 0; /* clear */
		}
	}
	vp->update_cnt = 0;     /* empty queue */
}

static void mdp4_dsi_video_blt_ov_update(struct mdp4_overlay_pipe *pipe);
static void mdp4_dsi_video_wait4dmap(int cndx);
static void mdp4_dsi_video_wait4ov(int cndx);

int mdp4_dsi_video_pipe_commit(int cndx, int wait)
{

	int  i, undx;
	int mixer = 0;
	struct vsycn_ctrl *vctrl;
	struct vsync_update *vp;
	struct mdp4_overlay_pipe *pipe;
	struct mdp4_overlay_pipe *real_pipe;
	unsigned long flags;
	int cnt = 0;
#ifdef MDP_ODD_RESOLUTION_CTRL
	int current_pipe_ndx = 0;
#endif

	vctrl = &vsync_ctrl_db[cndx];

	mutex_lock(&vctrl->update_lock);
	undx =  vctrl->update_ndx;
	vp = &vctrl->vlist[undx];
	pipe = vctrl->base_pipe;
	if (pipe == NULL) {
		pr_err("%s: NO base pipe\n", __func__);
		mutex_unlock(&vctrl->update_lock);
		return 0;
	}

	mixer = pipe->mixer_num;

	mdp_update_pm(vctrl->mfd, vctrl->vsync_time);

/*
* allow stage_commit without pipes queued
* (vp->update_cnt == 0) to unstage pipes after
* overlay_unset
*/
	xlog(__func__, wait, vp->update_cnt, 0, 0, 0);
	vctrl->update_ndx++;
	vctrl->update_ndx &= 0x01;
	vp->update_cnt = 0;     /* reset */
	if (vctrl->blt_free) {
		vctrl->blt_free--;
		if (vctrl->blt_free == 0)
			mdp4_free_writeback_buf(vctrl->mfd, mixer);
	}
	mutex_unlock(&vctrl->update_lock);


	spin_lock_irqsave(&vctrl->spin_lock, flags);
	if (vctrl->ov_koff != vctrl->ov_done) {
		spin_unlock_irqrestore(&vctrl->spin_lock, flags);
		pr_err("%s: Error, frame dropped %d %d\n", __func__,
				vctrl->ov_koff, vctrl->ov_done);
		return 0;
	}
	spin_unlock_irqrestore(&vctrl->spin_lock, flags);

	if (vctrl->blt_change) {
		pipe = vctrl->base_pipe;
		spin_lock_irqsave(&vctrl->spin_lock, flags);
		INIT_COMPLETION(vctrl->dmap_comp);
		INIT_COMPLETION(vctrl->ov_comp);
		vsync_irq_enable(INTR_DMA_P_DONE, MDP_DMAP_TERM);
		spin_unlock_irqrestore(&vctrl->spin_lock, flags);
		mdp4_dsi_video_wait4dmap(0);
		if (pipe->ov_blt_addr)
			mdp4_dsi_video_wait4ov(0);
	}

	pipe = vp->plist;

	for (i = 0; i < OVERLAY_PIPE_MAX; i++, pipe++) {
		if (pipe->pipe_used) {
			cnt++;
			real_pipe = mdp4_overlay_ndx2pipe(pipe->pipe_ndx);
			if (real_pipe && real_pipe->pipe_used) {
				/*
				 * commit pipes which are in pending queue
				 * and not be unset yet
				 */
				mdp4_overlay_vsync_commit(pipe);
			}
 		}
	}

	mdp4_mixer_stage_commit(mixer);

	/* start timing generator & mmu if they are not started yet */
	mdp4_overlay_dsi_video_start();

	/*
	 * there has possibility that pipe commit come very close to next vsync
	 * this may cause two consecutive pie_commits happen within same vsync
	 * period which casue iommu page fault when previous iommu buffer
	 * freed. Set ION_IOMMU_UNMAP_DELAYED flag at ion_map_iommu() to
	 * add delay unmap iommu buffer to fix this problem.
	 * Also ion_unmap_iommu() may take as long as 9 ms to free an ion buffer.
	 * therefore mdp4_overlay_iommu_unmap_freelist(mixer) should be called
	 * ater stage_commit() to ensure pipe_commit (up to stage_commit)
	 * is completed within vsync period.
	 */

	/* free previous committed iommu back to pool */
	mdp4_overlay_iommu_unmap_freelist(mixer);

	pipe = vp->plist;
	for (i = 0; i < OVERLAY_PIPE_MAX; i++, pipe++) {
		if (pipe->pipe_used) {


		/* free previous iommu to freelist
		* which will be freed at next
		* pipe_commit
		*/
		mdp4_overlay_iommu_pipe_free(pipe->pipe_ndx, 0);
		pipe->pipe_used = 0; /* clear */
		}
	}

	pipe = vctrl->base_pipe;
	spin_lock_irqsave(&vctrl->spin_lock, flags);
	if (pipe->ov_blt_addr) {
		mdp4_dsi_video_blt_ov_update(pipe);
		pipe->ov_cnt++;
		INIT_COMPLETION(vctrl->ov_comp);
		vsync_irq_enable(INTR_OVERLAY0_DONE, MDP_OVERLAY0_TERM);
		mb();
		vctrl->ov_koff++;
		/* kickoff overlay engine */
		mdp4_stat.kickoff_ov0++;
		outpdw(MDP_BASE + 0x0004, 0);
	} else {
		/* schedule second phase update  at dmap */
		INIT_COMPLETION(vctrl->dmap_comp);
		vsync_irq_enable(INTR_DMA_P_DONE, MDP_DMAP_TERM);
	}
	spin_unlock_irqrestore(&vctrl->spin_lock, flags);

	mdp4_stat.overlay_commit[pipe->mixer_num]++;

	if (wait) {
		if (pipe->ov_blt_addr)
			mdp4_dsi_video_wait4ov(0);
		else
			mdp4_dsi_video_wait4vsync(0);
	}
	xlog(__func__, 0x9999, 0, 0, 0, 0);
#ifdef MDP_ODD_RESOLUTION_CTRL
	current_pipe_ndx = pipe->pipe_ndx;
	for (i = current_pipe_ndx ; i >= 0; i--, pipe--) {
		pipe->check_odd_res = 0;
	}
#endif

	return cnt;
}
static void mdp4_video_vsync_irq_ctrl(int cndx, int enable)
{
	struct vsycn_ctrl *vctrl;

	vctrl = &vsync_ctrl_db[cndx];

	mutex_lock(&vctrl->update_lock);
	if (enable) {
		if (vsync_irq_cnt == 0)
			vsync_irq_enable(INTR_PRIMARY_VSYNC,
			MDP_PRIM_VSYNC_TERM);
		vsync_irq_cnt++;
	} else {
		if (vsync_irq_cnt) {
			vsync_irq_cnt--;
			if (vsync_irq_cnt == 0)
				vsync_irq_disable(INTR_PRIMARY_VSYNC,
						MDP_PRIM_VSYNC_TERM);
		}
	}
	pr_debug("%s: enable=%d cnt=%d\n", __func__, enable, vsync_irq_cnt);
	mutex_unlock(&vctrl->update_lock);
}

void mdp4_dsi_video_vsync_ctrl(struct fb_info *info, int enable)
{
	struct vsycn_ctrl *vctrl;
	int cndx = 0;

	vctrl = &vsync_ctrl_db[cndx];

	if (vctrl->vsync_irq_enabled == enable)
		return;

	pr_debug("%s: vsync enable=%d\n", __func__, enable);

	vctrl->vsync_irq_enabled = enable;

	mdp4_video_vsync_irq_ctrl(cndx, enable);
}

void mdp4_dsi_video_wait4vsync(int cndx)
{
	struct vsycn_ctrl *vctrl;
	int ret;
	ktime_t timestamp;
	unsigned long flags;

	if (cndx >= MAX_CONTROLLER) {
		pr_err("%s: out or range: cndx=%d\n", __func__, cndx);
		return;
	}

	vctrl = &vsync_ctrl_db[cndx];

	if (atomic_read(&vctrl->suspend) > 0)
		return;
	spin_lock_irqsave(&vctrl->spin_lock, flags);
	timestamp = vctrl->vsync_time;
	spin_unlock_irqrestore(&vctrl->spin_lock, flags);

	mdp4_video_vsync_irq_ctrl(cndx, 1);

	ret = wait_event_timeout(vctrl->wait_queue_internal,
			!ktime_equal(timestamp, vctrl->vsync_time),
			msecs_to_jiffies(VSYNC_PERIOD * 8));

	if (ret <= 0)
		pr_err("%s timeout ret=%d", __func__, ret);

	mdp4_video_vsync_irq_ctrl(cndx, 0);

	mdp4_stat.wait4vsync0++;

}

static void mdp4_dsi_video_wait4dmap(int cndx)
{
	struct vsycn_ctrl *vctrl;

	if (cndx >= MAX_CONTROLLER) {
		pr_err("%s: out or range: cndx=%d\n", __func__, cndx);
		return;
	}

	vctrl = &vsync_ctrl_db[cndx];

	if (atomic_read(&vctrl->suspend) > 0)
		return;

	wait_for_completion(&vctrl->dmap_comp);
}


static void mdp4_dsi_video_wait4dmap_done(int cndx)
{
	unsigned long flags;
	struct vsycn_ctrl *vctrl;

	if (cndx >= MAX_CONTROLLER) {
		pr_err("%s: out or range: cndx=%d\n", __func__, cndx);
		return;
	}
	vctrl = &vsync_ctrl_db[cndx];

	spin_lock_irqsave(&vctrl->spin_lock, flags);
	INIT_COMPLETION(vctrl->dmap_comp);
	vsync_irq_enable(INTR_DMA_P_DONE, MDP_DMAP_TERM);
	spin_unlock_irqrestore(&vctrl->spin_lock, flags);
	mdp4_dsi_video_wait4dmap(cndx);
}

void mdp4_dsi_video_wait4dmap_for_dsi(int cndx)
{
	unsigned long flags;
	struct vsycn_ctrl *vctrl;

	if (cndx >= MAX_CONTROLLER) {
		pr_err("%s: out or range: cndx=%d\n", __func__, cndx);
		return;
	}
	vctrl = &vsync_ctrl_db[cndx];

	if (mdp_intr_mask & INTR_DMA_P_DONE)
		mdp4_dsi_video_wait4dmap(cndx);
	else {
		spin_lock_irqsave(&vctrl->spin_lock, flags);
		INIT_COMPLETION(vctrl->dmap_comp);
		vsync_irq_enable(INTR_DMA_P_DONE, MDP_DMAP_TERM);
		spin_unlock_irqrestore(&vctrl->spin_lock, flags);
		mdp4_dsi_video_wait4dmap(cndx);
	}
}

static void mdp4_dsi_video_wait4ov(int cndx)
{
	struct vsycn_ctrl *vctrl;

	if (cndx >= MAX_CONTROLLER) {
		pr_err("%s: out or range: cndx=%d\n", __func__, cndx);
		return;
	}

	vctrl = &vsync_ctrl_db[cndx];

	if (atomic_read(&vctrl->suspend) > 0)
		return;

	wait_for_completion(&vctrl->ov_comp);
}

ssize_t mdp4_dsi_video_show_event(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int cndx;
	struct vsycn_ctrl *vctrl;
	ssize_t ret = 0;
	u64 vsync_tick;
	ktime_t timestamp;
	unsigned long flags;

	cndx = 0;
	vctrl = &vsync_ctrl_db[0];

	spin_lock_irqsave(&vctrl->spin_lock, flags);
	timestamp = vctrl->vsync_time;
	spin_unlock_irqrestore(&vctrl->spin_lock, flags);
	ret = wait_event_interruptible(vctrl->wait_queue,
			!ktime_equal(timestamp, vctrl->vsync_time) &&
			vctrl->vsync_irq_enabled);
	if (ret == -ERESTARTSYS)
		return ret;

	spin_lock_irqsave(&vctrl->spin_lock, flags);
	vsync_tick = ktime_to_ns(vctrl->vsync_time);
	spin_unlock_irqrestore(&vctrl->spin_lock, flags);

	ret = scnprintf(buf, PAGE_SIZE, "VSYNC=%llu", vsync_tick);
	buf[strlen(buf) + 1] = '\0';
	sec_debug_mdp_set_value(SEC_DEBUG_VSYNC_SYSFS_EVENT, SEC_DEBUG_OUT);
	return ret;
}

void mdp4_dsi_vsync_init(int cndx)
{
	struct vsycn_ctrl *vctrl;

	if (cndx >= MAX_CONTROLLER) {
		pr_err("%s: out or range: cndx=%d\n", __func__, cndx);
		return;
	}

	pr_debug("%s: ndx=%d\n", __func__, cndx);

	vctrl = &vsync_ctrl_db[cndx];
	if (vctrl->inited)
		return;

	vctrl->inited = 1;
	vctrl->update_ndx = 0;
	mutex_init(&vctrl->update_lock);
	init_completion(&vctrl->dmap_comp);
	init_completion(&vctrl->ov_comp);
	atomic_set(&vctrl->suspend, 1);
	spin_lock_init(&vctrl->spin_lock);
	init_waitqueue_head(&vctrl->wait_queue_internal);
	init_waitqueue_head(&vctrl->wait_queue);
}

void mdp4_dsi_video_free_base_pipe(struct msm_fb_data_type *mfd)
{
	struct vsycn_ctrl *vctrl;
	struct mdp4_overlay_pipe *pipe;

	vctrl = &vsync_ctrl_db[0];
	pipe = vctrl->base_pipe;

	if (pipe == NULL)
		return ;
	/* adb stop */
	if (pipe->pipe_type == OVERLAY_TYPE_BF)
		mdp4_overlay_borderfill_stage_down(pipe);

	/* base pipe may change after borderfill_stage_down */
	pipe = vctrl->base_pipe;
	mdp4_mixer_stage_down(pipe, 1);
	mdp4_overlay_pipe_free(pipe, 1);
	vctrl->base_pipe = NULL;
}

void mdp4_dsi_video_base_swap(int cndx, struct mdp4_overlay_pipe *pipe)
{
	struct vsycn_ctrl *vctrl;

	if (cndx >= MAX_CONTROLLER) {
		pr_err("%s: out or range: cndx=%d\n", __func__, cndx);
		return;
	}

	vctrl = &vsync_ctrl_db[cndx];
	vctrl->base_pipe = pipe;
}

/* timing generator off */
static void mdp4_dsi_video_tg_off(struct vsycn_ctrl *vctrl)
{
	MDP_OUTP(MDP_BASE + DSI_VIDEO_BASE, 0); /* turn off timing generator */
	/* some delay after turning off the tg */
	msleep(20);
}

int mdp4_dsi_video_splash_done(void)
{
	struct vsycn_ctrl *vctrl;
	int cndx = 0;

	vctrl = &vsync_ctrl_db[cndx];

	mdp4_dsi_video_tg_off(vctrl);
	mipi_dsi_controller_cfg(0);

	return 0;
}

int mdp4_dsi_video_on(struct platform_device *pdev)
{
	int dsi_width;
	int dsi_height;
	int dsi_bpp;
	int dsi_border_clr;
	int dsi_underflow_clr;
	int dsi_hsync_skew;

	int hsync_period;
	int hsync_ctrl;
	int vsync_period;
	int display_hctl;
	int display_v_start;
	int display_v_end;
	int active_hctl;
	int active_h_start;
	int active_h_end;
	int active_v_start;
	int active_v_end;
	int ctrl_polarity;
	int h_back_porch;
	int h_front_porch;
	int v_back_porch;
	int v_front_porch;
	int hsync_pulse_width;
	int vsync_pulse_width;
	int hsync_polarity;
	int vsync_polarity;
	int data_en_polarity;
	int hsync_start_x;
	int hsync_end_x;
	uint8 *buf;
	unsigned int buf_offset;
	int bpp, ptype;
	struct fb_info *fbi;
	struct fb_var_screeninfo *var;
	struct msm_fb_data_type *mfd;
	struct mdp4_overlay_pipe *pipe;
	int ret = 0;
	int cndx = 0;
	struct vsycn_ctrl *vctrl;
	struct msm_panel_info *pinfo;

	vctrl = &vsync_ctrl_db[cndx];
	mfd = (struct msm_fb_data_type *)platform_get_drvdata(pdev);
	pinfo = &mfd->panel_info;

	if (!mfd)
		return -ENODEV;

	if (mfd->key != MFD_KEY)
		return -EINVAL;

	mutex_lock(&mfd->dma->ov_mutex);

	vctrl->mfd = mfd;
	vctrl->dev = mfd->fbi->dev;
	vctrl->blt_ctrl = pinfo->lcd.blt_ctrl;
	vctrl->vsync_irq_enabled = 0;
	vsync_irq_cnt = 0;

	/* mdp clock on */
	mdp_clk_ctrl(1);

	fbi = mfd->fbi;
	var = &fbi->var;

	bpp = fbi->var.bits_per_pixel / 8;
	buf = (uint8 *) fbi->fix.smem_start;
	buf_offset = calc_fb_offset(mfd, fbi, bpp);

	if (vctrl->base_pipe == NULL) {
		ptype = mdp4_overlay_format2type(mfd->fb_imgType);
		if (ptype < 0)
			printk(KERN_INFO "%s: format2type failed\n", __func__);
		pipe = mdp4_overlay_pipe_alloc(ptype, MDP4_MIXER0);
		if (pipe == NULL) {
			printk(KERN_INFO "%s: pipe_alloc failed\n", __func__);
			mutex_unlock(&mfd->dma->ov_mutex);
			return -EBUSY;
		}
		pipe->pipe_used++;
		pipe->mixer_stage  = MDP4_MIXER_STAGE_BASE;
		pipe->mixer_num  = MDP4_MIXER0;
		pipe->src_format = mfd->fb_imgType;
		mdp4_overlay_panel_mode(pipe->mixer_num, MDP4_PANEL_DSI_VIDEO);
		ret = mdp4_overlay_format2pipe(pipe);
		if (ret < 0)
			printk(KERN_INFO "%s: format2type failed\n", __func__);

		pipe->ov_blt_addr = 0;
		pipe->dma_blt_addr = 0;
		vctrl->base_pipe = pipe; /* keep it */
		mdp4_init_writeback_buf(mfd, MDP4_MIXER0);

	} else {
		pipe = vctrl->base_pipe;
	}

		atomic_set(&vctrl->suspend, 0);

	if (!(mfd->cont_splash_done)) {
		mfd->cont_splash_done = 1;
		mdp4_dsi_video_wait4vsync(0);
		MDP_OUTP(MDP_BASE + DSI_VIDEO_BASE, 0);
		mipi_dsi_controller_cfg(0);
		/* Clks are enabled in probe.
		   Disabling clocks now */
		mdp_clk_ctrl(0);
	}

	pipe->src_height = fbi->var.yres;
	pipe->src_width = fbi->var.xres;
	pipe->src_h = fbi->var.yres;
	pipe->src_w = fbi->var.xres;
	pipe->src_y = 0;
	pipe->src_x = 0;
	pipe->dst_h = fbi->var.yres;
	pipe->dst_w = fbi->var.xres;
	pipe->srcp0_ystride = fbi->fix.line_length;
	pipe->bpp = bpp;

	if (mfd->display_iova)
		pipe->srcp0_addr = mfd->display_iova + buf_offset;
	else
		pipe->srcp0_addr = (uint32)(buf + buf_offset);

	xlog(__func__, pipe->pipe_ndx, (int)pipe->srcp0_addr, pipe->srcp0_ystride, 0, 0);
	pipe->dst_h = fbi->var.yres;
	pipe->dst_w = fbi->var.xres;

	mdp4_overlay_solidfill_init(pipe);

	mdp4_overlay_mdp_pipe_req(pipe, mfd);
	mdp4_calc_blt_mdp_bw(mfd, pipe);

	mdp4_overlay_dmap_xy(pipe);	/* dma_p */
	mdp4_overlay_dmap_cfg(mfd, 1);
	mdp4_overlay_rgb_setup(pipe);
	mdp4_overlayproc_cfg(pipe);

	mdp4_overlay_reg_flush(pipe, 1);

	mdp4_mixer_stage_up(pipe, 0);
	mdp4_mixer_stage_commit(pipe->mixer_num);

	/*
	 * DSI timing setting
	 */
	h_back_porch = var->left_margin;
	h_front_porch = var->right_margin;
	v_back_porch = var->upper_margin;
	v_front_porch = var->lower_margin;
	hsync_pulse_width = var->hsync_len;
	vsync_pulse_width = var->vsync_len;
	dsi_border_clr = mfd->panel_info.lcdc.border_clr;
	dsi_underflow_clr = mfd->panel_info.lcdc.underflow_clr;
	dsi_hsync_skew = mfd->panel_info.lcdc.hsync_skew;
	dsi_width = mfd->panel_info.xres +
		mfd->panel_info.lcdc.xres_pad;
	dsi_height = mfd->panel_info.yres +
		mfd->panel_info.lcdc.yres_pad;
	dsi_bpp = mfd->panel_info.bpp;

	hsync_period = hsync_pulse_width + h_back_porch + dsi_width
				+ h_front_porch;
	hsync_ctrl = (hsync_period << 16) | hsync_pulse_width;
	hsync_start_x = h_back_porch + hsync_pulse_width;
	hsync_end_x = hsync_period - h_front_porch - 1;
	display_hctl = (hsync_end_x << 16) | hsync_start_x;

	vsync_period =
	    (vsync_pulse_width + v_back_porch + dsi_height + v_front_porch);
	display_v_start = ((vsync_pulse_width + v_back_porch) * hsync_period)
				+ dsi_hsync_skew;
	display_v_end =
	  ((vsync_period - v_front_porch) * hsync_period) + dsi_hsync_skew - 1;

	if (dsi_width != var->xres) {
		active_h_start = hsync_start_x + first_pixel_start_x;
		active_h_end = active_h_start + var->xres - 1;
		active_hctl =
		    ACTIVE_START_X_EN | (active_h_end << 16) | active_h_start;
	} else {
		active_hctl = 0;
	}

	if (dsi_height != var->yres) {
		active_v_start =
		    display_v_start + first_pixel_start_y * hsync_period;
		active_v_end = active_v_start + (var->yres) * hsync_period - 1;
		active_v_start |= ACTIVE_START_Y_EN;
	} else {
		active_v_start = 0;
		active_v_end = 0;
	}

	dsi_underflow_clr |= 0x80000000;	/* enable recovery */
	hsync_polarity = 0;
	vsync_polarity = 0;
	data_en_polarity = 0;

	ctrl_polarity =
	    (data_en_polarity << 2) | (vsync_polarity << 1) | (hsync_polarity);

	mdp_pipe_ctrl(MDP_CMD_BLOCK, MDP_BLOCK_POWER_ON, FALSE);
	MDP_OUTP(MDP_BASE + DSI_VIDEO_BASE + 0x4, hsync_ctrl);
	MDP_OUTP(MDP_BASE + DSI_VIDEO_BASE + 0x8, vsync_period * hsync_period);
	MDP_OUTP(MDP_BASE + DSI_VIDEO_BASE + 0xc,
				vsync_pulse_width * hsync_period);
	MDP_OUTP(MDP_BASE + DSI_VIDEO_BASE + 0x10, display_hctl);
	MDP_OUTP(MDP_BASE + DSI_VIDEO_BASE + 0x14, display_v_start);
	MDP_OUTP(MDP_BASE + DSI_VIDEO_BASE + 0x18, display_v_end);
	MDP_OUTP(MDP_BASE + DSI_VIDEO_BASE + 0x1c, active_hctl);
	MDP_OUTP(MDP_BASE + DSI_VIDEO_BASE + 0x20, active_v_start);
	MDP_OUTP(MDP_BASE + DSI_VIDEO_BASE + 0x24, active_v_end);
	MDP_OUTP(MDP_BASE + DSI_VIDEO_BASE + 0x28, dsi_border_clr);
	MDP_OUTP(MDP_BASE + DSI_VIDEO_BASE + 0x2c, dsi_underflow_clr);
	MDP_OUTP(MDP_BASE + DSI_VIDEO_BASE + 0x30, dsi_hsync_skew);
	MDP_OUTP(MDP_BASE + DSI_VIDEO_BASE + 0x38, ctrl_polarity);
	mdp_pipe_ctrl(MDP_CMD_BLOCK, MDP_BLOCK_POWER_OFF, FALSE);

	mdp_histogram_ctrl_all(TRUE);
//	mdp4_overlay_dsi_video_start();
	mutex_unlock(&mfd->dma->ov_mutex);

	return ret;
}

int mdp4_dsi_video_off(struct platform_device *pdev)
{
	int ret = 0;
	int cndx = 0;
	struct msm_fb_data_type *mfd;
	struct vsycn_ctrl *vctrl;
	struct mdp4_overlay_pipe *pipe;
	struct vsync_update *vp;
	unsigned long flags;
	int mixer = 0;
	int undx, need_wait = 0;

	mfd = (struct msm_fb_data_type *)platform_get_drvdata(pdev);

	mutex_lock(&mfd->dma->ov_mutex);
	vctrl = &vsync_ctrl_db[cndx];
	pipe = vctrl->base_pipe;

	mdp4_dsi_video_wait4vsync(cndx);

	if (pipe->ov_blt_addr) {
		spin_lock_irqsave(&vctrl->spin_lock, flags);
		if (vctrl->ov_koff != vctrl->ov_done)
			need_wait = 1;
		spin_unlock_irqrestore(&vctrl->spin_lock, flags);
		if (need_wait)
			mdp4_dsi_video_wait4ov(0);
	}

	mdp_histogram_ctrl_all(FALSE);

	mdp4_dsi_video_tg_off(vctrl);

	dsi_video_enabled = 0;

	undx =  vctrl->update_ndx;
	vp = &vctrl->vlist[undx];
	if (vp->update_cnt) {
		/*
		 * pipe's iommu will be freed at next overlay play
		 * and iommu_drop statistic will be increased by one
		 */
		pr_warn("%s: update_cnt=%d\n", __func__, vp->update_cnt);
		mdp4_dsi_video_pipe_clean(vp);
	}

	if (pipe) {
		xlog(__func__, pipe->pipe_ndx, mfd->ref_cnt, 0, 0, 0);
		/* sanity check, free pipes besides base layer */
		mixer = pipe->mixer_num;
		mdp4_overlay_unset_mixer(mixer);
		if (mfd->ref_cnt == 0) {
			/* adb stop */
			if (pipe->pipe_type == OVERLAY_TYPE_BF)
				mdp4_overlay_borderfill_stage_down(pipe);

			/* base pipe may change after borderfill_stage_down */
			pipe = vctrl->base_pipe;
			mdp4_mixer_stage_down(pipe, 1);
			mdp4_overlay_pipe_free(pipe, 1);
			vctrl->base_pipe = NULL;
			msleep(20);
		} else {
			/* system suspending */
			mdp4_mixer_stage_down(vctrl->base_pipe, 1);
			mdp4_overlay_iommu_pipe_free(
				vctrl->base_pipe->pipe_ndx, 1);
		}
	}

	atomic_set(&vctrl->suspend, 1);
	if (vctrl->vsync_irq_enabled) {
		vctrl->vsync_irq_enabled = 0;
		mdp4_video_vsync_irq_ctrl(cndx, 0);
	}

	/*
	 * clean up ion freelist
	 * there need two stage to empty ion free list
	 * therefore need call unmap freelist twice
	 */
	mdp4_overlay_iommu_unmap_freelist(mixer);
	mdp4_overlay_iommu_unmap_freelist(mixer);

	/* mdp clock off */
	mdp_clk_ctrl(0);
	mdp_pipe_ctrl(MDP_OVERLAY0_BLOCK, MDP_BLOCK_POWER_OFF, FALSE);
	xlog(__func__, 0x9999, 0, 0, 0, 0);
	mutex_unlock(&mfd->dma->ov_mutex);

	return ret;
}

static __u32 msm_fb_line_length(__u32 fb_index, __u32 xres, int bpp)
{
	/*
	 * The adreno GPU hardware requires that the pitch be aligned to
	 * 32 pixels for color buffers, so for the cases where the GPU
	 * is writing directly to fb0, the framebuffer pitch
	 * also needs to be 32 pixel aligned
	 */

	if (fb_index == 0)
		return ALIGN(xres, 32) * bpp;
	else
		return xres * bpp;
}

/* 3D side by side */
void mdp4_dsi_video_3d_sbys(struct msm_fb_data_type *mfd,
				struct msmfb_overlay_3d *r3d)
{
	struct fb_info *fbi;
	unsigned int buf_offset;
	int bpp;
	uint8 *buf = NULL;
	int cndx = 0;
	struct vsycn_ctrl *vctrl;
	struct mdp4_overlay_pipe *pipe;

	vctrl = &vsync_ctrl_db[cndx];
	pipe = vctrl->base_pipe;

	if (vctrl->base_pipe == NULL)
		return;

	pipe = vctrl->base_pipe;
	pipe->is_3d = r3d->is_3d;
	pipe->src_height_3d = r3d->height;
	pipe->src_width_3d = r3d->width;

	if (pipe->is_3d)
		mdp4_overlay_panel_3d(pipe->mixer_num, MDP4_3D_SIDE_BY_SIDE);
	else
		mdp4_overlay_panel_3d(pipe->mixer_num, MDP4_3D_NONE);

	fbi = mfd->fbi;

	bpp = fbi->var.bits_per_pixel / 8;
	buf = (uint8 *) fbi->fix.smem_start;
	buf_offset = calc_fb_offset(mfd, fbi, bpp);

	if (pipe->is_3d) {
		pipe->src_height = pipe->src_height_3d;
		pipe->src_width = pipe->src_width_3d;
		pipe->src_h = pipe->src_height_3d;
		pipe->src_w = pipe->src_width_3d;
		pipe->dst_h = pipe->src_height_3d;
		pipe->dst_w = pipe->src_width_3d;
		pipe->srcp0_ystride = msm_fb_line_length(0,
					pipe->src_width, bpp);
	} else {
		 /* 2D */
		pipe->src_height = fbi->var.yres;
		pipe->src_width = fbi->var.xres;
		pipe->src_h = fbi->var.yres;
		pipe->src_w = fbi->var.xres;
		pipe->dst_h = fbi->var.yres;
		pipe->dst_w = fbi->var.xres;
		pipe->srcp0_ystride = fbi->fix.line_length;
	}

	pipe->src_y = 0;
	pipe->src_x = 0;
	pipe->dst_y = 0;
	pipe->dst_x = 0;

	if (mfd->display_iova)
		pipe->srcp0_addr = mfd->display_iova + buf_offset;
	else
		pipe->srcp0_addr = (uint32)(buf + buf_offset);

	mdp4_overlay_rgb_setup(pipe);

	mdp4_overlayproc_cfg(pipe);

	mdp4_overlay_dmap_xy(pipe);

	mdp4_overlay_dmap_cfg(mfd, 1);

	mdp4_overlay_reg_flush(pipe, 1);

	mdp4_mixer_stage_up(pipe, 0);

	mdp4_mixer_stage_commit(pipe->mixer_num);

	mb();
}

static void mdp4_dsi_video_blt_ov_update(struct mdp4_overlay_pipe *pipe)
{
	uint32 off, addr;
	int bpp;
	char *overlay_base;

	if (pipe->ov_blt_addr == 0)
		return;

#ifdef BLT_RGB565
	bpp = 2; /* overlay ouput is RGB565 */
#else
	bpp = 3; /* overlay ouput is RGB888 */
#endif
	off = 0;
	if (pipe->ov_cnt & 0x01)
		off = pipe->src_height * pipe->src_width * bpp;
	addr = pipe->ov_blt_addr + off;

	/* overlay 0 */
	overlay_base = MDP_BASE + MDP4_OVERLAYPROC0_BASE;/* 0x10000 */
	outpdw(overlay_base + 0x000c, addr);
	outpdw(overlay_base + 0x001c, addr);
}

static void mdp4_dsi_video_blt_dmap_update(struct mdp4_overlay_pipe *pipe)
{
	uint32 off, addr;
	int bpp;

	if (pipe->ov_blt_addr == 0)
		return;


#ifdef BLT_RGB565
	bpp = 2; /* overlay ouput is RGB565 */
#else
	bpp = 3; /* overlay ouput is RGB888 */
#endif
	off = 0;
	if (pipe->dmap_cnt & 0x01)
		off = pipe->src_height * pipe->src_width * bpp;
	addr = pipe->dma_blt_addr + off;

	/* dmap */
	MDP_OUTP(MDP_BASE + 0x90008, addr);
}

/*
 * mdp4_primary_vsync_dsi_video: called from isr
 */
void mdp4_primary_vsync_dsi_video(void)
{
	int cndx;
	struct vsycn_ctrl *vctrl;


	cndx = 0;
	vctrl = &vsync_ctrl_db[cndx];
	pr_debug("%s: cpu=%d\n", __func__, smp_processor_id());
	xlog(__func__, 0, 0, 0, 0, 0);
	spin_lock(&vctrl->spin_lock);
	vctrl->vsync_time = ktime_get();
	wake_up_all(&vctrl->wait_queue_internal);
	wake_up_interruptible_all(&vctrl->wait_queue);
	spin_unlock(&vctrl->spin_lock);
}

 /*
 * mdp4_dmap_done_dsi_video: called from isr
 */
void mdp4_dmap_done_dsi_video(int cndx)
{
	struct vsycn_ctrl *vctrl;
	struct mdp4_overlay_pipe *pipe;

	if (cndx >= MAX_CONTROLLER) {
		pr_err("%s: out or range: cndx=%d\n", __func__, cndx);
		return;
	}
	vctrl = &vsync_ctrl_db[cndx];
	pipe = vctrl->base_pipe;

	spin_lock(&vctrl->spin_lock);
	vsync_irq_disable(INTR_DMA_P_DONE, MDP_DMAP_TERM);

	if (pipe == NULL) {
		spin_unlock(&vctrl->spin_lock);
		return;
	}

	if (vctrl->blt_change) {
		mdp4_overlayproc_cfg(pipe);
		mdp4_overlay_dmap_xy(pipe);
		if (pipe->ov_blt_addr) {
			mdp4_dsi_video_blt_ov_update(pipe);
			pipe->ov_cnt++;
			/* Prefill one frame */
			vsync_irq_enable(INTR_OVERLAY0_DONE,
						MDP_OVERLAY0_TERM);
			/* kickoff overlay0 engine */
			mdp4_stat.kickoff_ov0++;
			vctrl->ov_koff++;	/* make up for prefill */
			outpdw(MDP_BASE + 0x0004, 0);
		}
		vctrl->blt_change = 0;
	}

	complete_all(&vctrl->dmap_comp);
	mdp4_overlay_dma_commit(cndx);
	spin_unlock(&vctrl->spin_lock);
}

/*
 * mdp4_overlay0_done_dsi: called from isr
 */
void mdp4_overlay0_done_dsi_video(int cndx)
{
	struct vsycn_ctrl *vctrl;
	struct mdp4_overlay_pipe *pipe;

	vctrl = &vsync_ctrl_db[cndx];
	pipe = vctrl->base_pipe;

	spin_lock(&vctrl->spin_lock);
	vsync_irq_disable(INTR_OVERLAY0_DONE, MDP_OVERLAY0_TERM);
	vctrl->ov_done++;
	complete_all(&vctrl->ov_comp);

	if (pipe == NULL) {
		spin_unlock(&vctrl->spin_lock);
		return;
	}

	if (pipe->ov_blt_addr == 0) {
		spin_unlock(&vctrl->spin_lock);
		return;
	}

	mdp4_dsi_video_blt_dmap_update(pipe);
	pipe->dmap_cnt++;
	spin_unlock(&vctrl->spin_lock);
}

/*
 * make sure the MIPI_DSI_WRITEBACK_SIZE defined at boardfile
 * has enough space h * w * 3 * 2
 */
static void mdp4_dsi_video_do_blt(struct msm_fb_data_type *mfd, int enable)
{
	unsigned long flag;
	int cndx = 0;
	struct vsycn_ctrl *vctrl;
	struct mdp4_overlay_pipe *pipe;

	vctrl = &vsync_ctrl_db[cndx];
	pipe = vctrl->base_pipe;

	mdp4_allocate_writeback_buf(mfd, MDP4_MIXER0);

	if (mfd->ov0_wb_buf->write_addr == 0) {
		pr_info("%s: no blt_base assigned\n", __func__);
		return;
	}

	spin_lock_irqsave(&vctrl->spin_lock, flag);
	if (enable && pipe->ov_blt_addr == 0) {
		pipe->ov_blt_addr = mfd->ov0_wb_buf->write_addr;
		pipe->dma_blt_addr = mfd->ov0_wb_buf->read_addr;
		pipe->ov_cnt = 0;
		pipe->dmap_cnt = 0;
		vctrl->ov_koff = 0;
		vctrl->ov_done = 0;
		vctrl->blt_free = 0;
		mdp4_stat.blt_dsi_video++;
		vctrl->blt_change++;
	} else if (enable == 0 && pipe->ov_blt_addr) {
		pipe->ov_blt_addr = 0;
		pipe->dma_blt_addr =  0;
		vctrl->blt_free = 4;	/* 4 commits to free wb buf */
		vctrl->blt_change++;
	}
	xlog(__func__, vctrl->blt_change, enable, (int)pipe->ov_blt_addr, 0, 0);
	pr_info("%s: changed=%d enable=%d ov_blt_addr=%x\n", __func__,
		vctrl->blt_change, enable, (int)pipe->ov_blt_addr);

	if (!vctrl->blt_change) {
		spin_unlock_irqrestore(&vctrl->spin_lock, flag);
		return;
	}
	spin_unlock_irqrestore(&vctrl->spin_lock, flag);

	if (vctrl->blt_ctrl == OVERLAY_BLT_SWITCH_TG_OFF) {
		int tg_enabled;

		pr_info("%s: blt enabled by switching TG off\n", __func__);
		vctrl->blt_change = 0;
		tg_enabled = inpdw(MDP_BASE + DSI_VIDEO_BASE) & 0x01;
		if (tg_enabled) {
			mdp4_dsi_video_wait4vsync(cndx);
			MDP_OUTP(MDP_BASE + DSI_VIDEO_BASE, 0);
			mdp4_dsi_video_wait4dmap_done(0);
		}
		mdp4_overlayproc_cfg(pipe);
		mdp4_overlay_dmap_xy(pipe);
		if (pipe->ov_blt_addr) {
			mdp4_dsi_video_blt_ov_update(pipe);
			pipe->ov_cnt++;
			/* Prefill one frame */
			vsync_irq_enable(INTR_OVERLAY0_DONE,
						MDP_OVERLAY0_TERM);
			/* kickoff overlay0 engine */
			mdp4_stat.kickoff_ov0++;
			vctrl->ov_koff++;	/* make up for prefill */
			outpdw(MDP_BASE + 0x0004, 0);
		}
		if (tg_enabled) {
			/*
			 * need wait for more than 1 ms to
			 * make sure lanes' fifo is empty and
			 * lanes in stop state befroe reset
			 * controller
			 */
			usleep(2000);
			mipi_dsi_sw_reset();
			MDP_OUTP(MDP_BASE + DSI_VIDEO_BASE, 1);
		}
	}
}

void mdp4_dsi_video_overlay_blt(struct msm_fb_data_type *mfd,
					struct msmfb_overlay_blt *req)
{
	mdp4_dsi_video_do_blt(mfd, req->enable);
}

void mdp4_dsi_video_blt_start(struct msm_fb_data_type *mfd)
{
	mdp4_dsi_video_do_blt(mfd, 1);
}

void mdp4_dsi_video_blt_stop(struct msm_fb_data_type *mfd)
{
	mdp4_dsi_video_do_blt(mfd, 0);
}

void mdp4_dsi_video_overlay(struct msm_fb_data_type *mfd)
{
	struct fb_info *fbi = mfd->fbi;
	uint8 *buf;
	unsigned int buf_offset;
	int bpp;
	int cnt, cndx = 0;
	struct vsycn_ctrl *vctrl;
	struct mdp4_overlay_pipe *pipe;

	mutex_lock(&mfd->dma->ov_mutex);

	vctrl = &vsync_ctrl_db[cndx];
	pipe = vctrl->base_pipe;

	if (!pipe || !mfd->panel_power_on) {
		mutex_unlock(&mfd->dma->ov_mutex);
		return;
	}

	pr_debug("%s: cpu=%d pid=%d\n", __func__,
			smp_processor_id(), current->pid);
	if (pipe->pipe_type == OVERLAY_TYPE_RGB) {
		bpp = fbi->var.bits_per_pixel / 8;
		buf = (uint8 *) fbi->fix.smem_start;
		buf_offset = calc_fb_offset(mfd, fbi, bpp);

		if (mfd->display_iova)
			pipe->srcp0_addr = mfd->display_iova + buf_offset;
		else
			pipe->srcp0_addr = (uint32)(buf + buf_offset);

		mdp4_dsi_video_pipe_queue(0, pipe);
	}

	mdp4_overlay_mdp_perf_upd(mfd, 1);

	cnt = mdp4_dsi_video_pipe_commit(cndx, 0);
	if (cnt >= 0) {
		if (pipe->ov_blt_addr)
			mdp4_dsi_video_wait4ov(cndx);
		else
			mdp4_dsi_video_wait4vsync(cndx);
	}

	mdp4_overlay_mdp_perf_upd(mfd, 0);
	mutex_unlock(&mfd->dma->ov_mutex);
}
