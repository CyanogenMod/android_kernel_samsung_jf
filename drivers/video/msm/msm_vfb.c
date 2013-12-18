/*
 *  linux/drivers/video/vfb.c -- Virtual frame buffer device
 *
 *      Copyright (C) 2002 James Simmons
 *
 *	Copyright (C) 1997 Geert Uytterhoeven
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License. See the file COPYING in the main directory of this archive for
 *  more details.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>

#include <linux/fb.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/dma-mapping.h>
#include <linux/dma-buf.h>
#include "mdp.h"
#include "msm_fb.h"
#include "mdp4.h"

    /*
     *  RAM we reserve for the frame buffer. This defines the maximum screen
     *  size
     *
     *  The default can be overridden if the driver is compiled as a module
     */

#define VIDEOMEMSIZE	(1*1024*1024)	/* 1 MB */
#define CONFIG_FB_VFB_NR_BUFFERS 3

static char *mode_option __devinitdata;
static int bpp __devinitdata = 8;

module_param(mode_option, charp, 0);
MODULE_PARM_DESC(mode_option, "Initial video mode e.g. '648x480-8@60'");
module_param(bpp, int, 0);

struct msmvfb_fd_data {
	int		y_fd;
	struct timeval	time_marker;
};

#define MSMVFB_PUT_FD		_IOW('F', 323, struct msmvfb_fd_data)
#define MSMVFB_RESET_FD		_IOW('F', 324, int)
#define MSMVFB_WAIT_UPDATE	_IOW('F', 325, int)
#define MSMVFB_COMP_SIG_WAIT	_IOW('F', 326, int)

struct msmvfb_buf_list {
	struct timeval	time_marker;
	int		buf_fd;
	struct ion_handle *buf_ihdl;
	struct dma_buf  *dmabuf;
	unsigned long   buf_size;

};

static int ref_cnt = 0;
static int buf_list_cnt = 0;
static struct msmvfb_buf_list	buf_list[CONFIG_FB_VFB_NR_BUFFERS];
static DECLARE_COMPLETION(pan_disp_comp);
static DECLARE_COMPLETION(hwc_done_comp);

static int msm_vfb_comp_sig_wait(struct fb_info *info, int opt)
{
	int ret = 0;

	pr_info("%s enter : %d\n", __func__, opt);
	if (opt == 1) {
		INIT_COMPLETION(hwc_done_comp);
		ret = wait_for_completion_killable(&hwc_done_comp);
		if (ret < 0)
			ret = -ERESTARTSYS;
	} else {
		complete_all(&hwc_done_comp);
	}

	pr_info("%s exit : %d\n", __func__, ret);
	return ret;
}


#if 0
/**********************************************************************
 *
 * Memory management
 *
 **********************************************************************/
static void *rvmalloc(unsigned long size)
{
	void *mem;
	unsigned long adr;

	size = PAGE_ALIGN(size);
	mem = vmalloc_32(size);
	if (!mem)
		return NULL;

	memset(mem, 0, size); /* Clear the ram out, no junk to the user */
	adr = (unsigned long) mem;
	while (size > 0) {
		SetPageReserved(vmalloc_to_page((void *)adr));
		adr += PAGE_SIZE;
		size -= PAGE_SIZE;
	}

	return mem;
}

static void rvfree(void *mem, unsigned long size)
{
	unsigned long adr;

	if (!mem)
		return;

	adr = (unsigned long) mem;
	while ((long) size > 0) {
		ClearPageReserved(vmalloc_to_page((void *)adr));
		adr += PAGE_SIZE;
		size -= PAGE_SIZE;
	}
	vfree(mem);
}
#endif

static struct fb_var_screeninfo msm_vfb_default __devinitdata = {
	.xres =		640,
	.yres =		480,
	.xres_virtual =	640,
	.yres_virtual =	480,
	.bits_per_pixel = 8,
	.red =		{ 0, 8, 0 },
	.green =	{ 0, 8, 0 },
	.blue =		{ 0, 8, 0 },
	.activate =	FB_ACTIVATE_TEST,
	.height =	-1,
	.width =	-1,
	.pixclock =	20000,
	.left_margin =	64,
	.right_margin =	64,
	.upper_margin =	32,
	.lower_margin =	32,
	.hsync_len =	64,
	.vsync_len =	2,
	.vmode =	FB_VMODE_NONINTERLACED,
};

static struct fb_fix_screeninfo msm_vfb_fix __devinitdata = {
	.id =		"Virtual FB",
	.type =		FB_TYPE_PACKED_PIXELS,
	.visual =	FB_VISUAL_PSEUDOCOLOR,
	.xpanstep =	1,
	.ypanstep =	1,
	.ywrapstep =	1,
	.accel =	FB_ACCEL_NONE,
};

static bool msm_vfb_enable __initdata = 0;	/* disabled by default */
module_param(msm_vfb_enable, bool, 0);

static int msm_vfb_check_var(struct fb_var_screeninfo *var,
			 struct fb_info *info);
static int msm_vfb_set_par(struct fb_info *info);
static int msm_vfb_setcolreg(u_int regno, u_int red, u_int green, u_int blue,
			 u_int transp, struct fb_info *info);
static int msm_vfb_pan_display(struct fb_var_screeninfo *var,
			   struct fb_info *info);
static int msm_vfb_mmap(struct fb_info *info,
		    struct vm_area_struct *vma);
static int msm_vfb_ioctl(struct fb_info *info, unsigned int cmd,
			unsigned long arg);

static int msm_vfb_open(struct fb_info *info, int user);
static int msm_vfb_release(struct fb_info *info, int user);

static struct fb_ops msm_vfb_ops = {
	.fb_open        = msm_vfb_open,
	.fb_release     = msm_vfb_release,

	.fb_read        = fb_sys_read,
	.fb_write       = fb_sys_write,
	.fb_check_var	= msm_vfb_check_var,
	.fb_set_par	= msm_vfb_set_par,
	.fb_setcolreg	= msm_vfb_setcolreg,
	.fb_pan_display	= msm_vfb_pan_display,
	.fb_fillrect	= sys_fillrect,
	.fb_copyarea	= sys_copyarea,
	.fb_imageblit	= sys_imageblit,
	.fb_mmap	= msm_vfb_mmap,
	.fb_ioctl       = msm_vfb_ioctl,
};

    /*
     *  Internal routines
     */

static u_long get_line_length(int xres_virtual, int bpp)
{
	u_long length;

	length = xres_virtual * bpp;
	length = (length + 31) & ~31;
	length >>= 3;
	return (length);
}

    /*
     *  Setting the video mode has been split into two parts.
     *  First part, xxxfb_check_var, must not write anything
     *  to hardware, it should only verify and adjust var.
     *  This means it doesn't alter par but it does use hardware
     *  data from it to check this var.
     */

static int msm_vfb_check_var(struct fb_var_screeninfo *var,
			 struct fb_info *info)
{
	u_long line_length;

	/*
	 *  FB_VMODE_CONUPDATE and FB_VMODE_SMOOTH_XPAN are equal!
	 *  as FB_VMODE_SMOOTH_XPAN is only used internally
	 */

	if (var->vmode & FB_VMODE_CONUPDATE) {
		var->vmode |= FB_VMODE_YWRAP;
		var->xoffset = info->var.xoffset;
		var->yoffset = info->var.yoffset;
	}

	/*
	 *  Some very basic checks
	 */
	if (!var->xres)
		var->xres = 1;
	if (!var->yres)
		var->yres = 1;
	if (var->xres > var->xres_virtual)
		var->xres_virtual = var->xres;
	if (var->yres > var->yres_virtual)
		var->yres_virtual = var->yres;
	if (var->bits_per_pixel <= 1)
		var->bits_per_pixel = 1;
	else if (var->bits_per_pixel <= 8)
		var->bits_per_pixel = 8;
	else if (var->bits_per_pixel <= 16)
		var->bits_per_pixel = 16;
	else if (var->bits_per_pixel <= 24)
		var->bits_per_pixel = 24;
	else if (var->bits_per_pixel <= 32)
		var->bits_per_pixel = 32;
	else
		return -EINVAL;

	if (var->xres_virtual < var->xoffset + var->xres)
		var->xres_virtual = var->xoffset + var->xres;
	if (var->yres_virtual < var->yoffset + var->yres)
		var->yres_virtual = var->yoffset + var->yres;

	/*
	 *  Memory limit
	 */
	line_length =
	    get_line_length(var->xres_virtual, var->bits_per_pixel);
//	if (line_length * var->yres_virtual > videomemorysize)
//		return -ENOMEM;

	/*
	 * Now that we checked it we alter var. The reason being is that the video
	 * mode passed in might not work but slight changes to it might make it
	 * work. This way we let the user know what is acceptable.
	 */
	switch (var->bits_per_pixel) {
	case 1:
	case 8:
		var->red.offset = 0;
		var->red.length = 8;
		var->green.offset = 0;
		var->green.length = 8;
		var->blue.offset = 0;
		var->blue.length = 8;
		var->transp.offset = 0;
		var->transp.length = 0;
		break;
	case 16:		/* RGBA 5551 */
		if (var->transp.length) {
			var->red.offset = 0;
			var->red.length = 5;
			var->green.offset = 5;
			var->green.length = 5;
			var->blue.offset = 10;
			var->blue.length = 5;
			var->transp.offset = 15;
			var->transp.length = 1;
		} else {	/* RGB 565 */
			var->red.offset = 0;
			var->red.length = 5;
			var->green.offset = 5;
			var->green.length = 6;
			var->blue.offset = 11;
			var->blue.length = 5;
			var->transp.offset = 0;
			var->transp.length = 0;
		}
		break;
	case 24:		/* RGB 888 */
		var->red.offset = 0;
		var->red.length = 8;
		var->green.offset = 8;
		var->green.length = 8;
		var->blue.offset = 16;
		var->blue.length = 8;
		var->transp.offset = 0;
		var->transp.length = 0;
		break;
	case 32:		/* RGBA 8888 */
		var->red.offset = 0;
		var->red.length = 8;
		var->green.offset = 8;
		var->green.length = 8;
		var->blue.offset = 16;
		var->blue.length = 8;
		var->transp.offset = 24;
		var->transp.length = 8;
		break;
	}
	var->red.msb_right = 0;
	var->green.msb_right = 0;
	var->blue.msb_right = 0;
	var->transp.msb_right = 0;

	return 0;
}

/* This routine actually sets the video mode. It's in here where we
 * the hardware state info->par and fix which can be affected by the
 * change in par. For this driver it doesn't do much.
 */
static int msm_vfb_set_par(struct fb_info *info)
{
	info->fix.line_length = get_line_length(info->var.xres_virtual,
						info->var.bits_per_pixel);
	return 0;
}

    /*
     *  Set a single color register. The values supplied are already
     *  rounded down to the hardware's capabilities (according to the
     *  entries in the var structure). Return != 0 for invalid regno.
     */

static int msm_vfb_setcolreg(u_int regno, u_int red, u_int green, u_int blue,
			 u_int transp, struct fb_info *info)
{
	if (regno >= 256)	/* no. of hw registers */
		return 1;
	/*
	 * Program hardware... do anything you want with transp
	 */

	/* grayscale works only partially under directcolor */
	if (info->var.grayscale) {
		/* grayscale = 0.30*R + 0.59*G + 0.11*B */
		red = green = blue =
		    (red * 77 + green * 151 + blue * 28) >> 8;
	}

	/* Directcolor:
	 *   var->{color}.offset contains start of bitfield
	 *   var->{color}.length contains length of bitfield
	 *   {hardwarespecific} contains width of RAMDAC
	 *   cmap[X] is programmed to (X << red.offset) | (X << green.offset) | (X << blue.offset)
	 *   RAMDAC[X] is programmed to (red, green, blue)
	 *
	 * Pseudocolor:
	 *    var->{color}.offset is 0 unless the palette index takes less than
	 *                        bits_per_pixel bits and is stored in the upper
	 *                        bits of the pixel value
	 *    var->{color}.length is set so that 1 << length is the number of available
	 *                        palette entries
	 *    cmap is not used
	 *    RAMDAC[X] is programmed to (red, green, blue)
	 *
	 * Truecolor:
	 *    does not use DAC. Usually 3 are present.
	 *    var->{color}.offset contains start of bitfield
	 *    var->{color}.length contains length of bitfield
	 *    cmap is programmed to (red << red.offset) | (green << green.offset) |
	 *                      (blue << blue.offset) | (transp << transp.offset)
	 *    RAMDAC does not exist
	 */
#define CNVT_TOHW(val,width) ((((val)<<(width))+0x7FFF-(val))>>16)
	switch (info->fix.visual) {
	case FB_VISUAL_TRUECOLOR:
	case FB_VISUAL_PSEUDOCOLOR:
		red = CNVT_TOHW(red, info->var.red.length);
		green = CNVT_TOHW(green, info->var.green.length);
		blue = CNVT_TOHW(blue, info->var.blue.length);
		transp = CNVT_TOHW(transp, info->var.transp.length);
		break;
	case FB_VISUAL_DIRECTCOLOR:
		red = CNVT_TOHW(red, 8);	/* expect 8 bit DAC */
		green = CNVT_TOHW(green, 8);
		blue = CNVT_TOHW(blue, 8);
		/* hey, there is bug in transp handling... */
		transp = CNVT_TOHW(transp, 8);
		break;
	}
#undef CNVT_TOHW
	/* Truecolor has hardware independent palette */
	if (info->fix.visual == FB_VISUAL_TRUECOLOR) {
		u32 v;

		if (regno >= 16)
			return 1;

		v = (red << info->var.red.offset) |
		    (green << info->var.green.offset) |
		    (blue << info->var.blue.offset) |
		    (transp << info->var.transp.offset);
		switch (info->var.bits_per_pixel) {
		case 8:
			break;
		case 16:
			((u32 *) (info->pseudo_palette))[regno] = v;
			break;
		case 24:
		case 32:
			((u32 *) (info->pseudo_palette))[regno] = v;
			break;
		}
		return 0;
	}
	return 0;
}

    /*
     *  Pan or Wrap the Display
     *
     *  This call looks only at xoffset, yoffset and the FB_VMODE_YWRAP flag
     */

static int msm_vfb_pan_display(struct fb_var_screeninfo *var,
			   struct fb_info *info)
{

	pr_info("%s x:%d, y:%d\n", __func__, var->xoffset, var->yoffset);

	if (var->vmode & FB_VMODE_YWRAP) {
		if (var->yoffset < 0
		    || var->yoffset >= info->var.yres_virtual
		    || var->xoffset)
			return -EINVAL;
	} else {
		if (var->xoffset + info->var.xres > info->var.xres_virtual ||
		    var->yoffset + info->var.yres > info->var.yres_virtual)
			return -EINVAL;
	}
	info->var.xoffset = var->xoffset;
	info->var.yoffset = var->yoffset;
	if (var->vmode & FB_VMODE_YWRAP)
		info->var.vmode |= FB_VMODE_YWRAP;
	else
		info->var.vmode &= ~FB_VMODE_YWRAP;

	complete_all(&pan_disp_comp);
	return 0;
}

    /*
     *  Most drivers don't need their own mmap function
     */

struct msm_vfb_mmap_data {
    struct device *dev;
    struct dma_buf *dmabuf[CONFIG_FB_VFB_NR_BUFFERS];
    struct dma_buf_attachment *attachment[CONFIG_FB_VFB_NR_BUFFERS];
    struct sg_table *sgt[CONFIG_FB_VFB_NR_BUFFERS];
    atomic_t cnt;
};

static void fb_vm_open(struct vm_area_struct *vma)
{
	struct msm_vfb_mmap_data *data = vma->vm_private_data;
	atomic_inc(&data->cnt);
	pr_info("%s !!!\n", __func__);
}

static void fb_vm_close(struct vm_area_struct *vma)
{
	struct msm_vfb_mmap_data *data = vma->vm_private_data;
	if (atomic_dec_and_test(&data->cnt)) {
		int i;

		for (i = 0; i < CONFIG_FB_VFB_NR_BUFFERS; i++) {
			dma_unmap_sg(data->dev, data->sgt[i]->sgl, data->sgt[i]->nents, DMA_FROM_DEVICE);
			dma_buf_unmap_attachment(data->attachment[i], data->sgt[i], DMA_FROM_DEVICE);
			dma_buf_detach(data->dmabuf[i], data->attachment[i]);
			dma_buf_put(data->dmabuf[i]);
		}
		kfree(data);
	}
	pr_info("%s !!!\n", __func__);
}

const struct vm_operations_struct fb_vm_ops = {
	.open = fb_vm_open,
	.close = fb_vm_close,
};

static int msm_vfb_mmap(struct fb_info *info,
		    struct vm_area_struct *vma)
{
	unsigned long cur_va = vma->vm_start;
	unsigned long vma_size = vma->vm_end - vma->vm_start;
	unsigned long off = vma->vm_pgoff << PAGE_SHIFT;
	unsigned long tot_buf_size;
	unsigned long req_buf_size;
	int i;
	int ret = 0;
	struct msm_vfb_mmap_data *data;

	pr_info("%s offset:%lx, size:%lx, real:%x\n", __func__, off, vma_size, info->fix.smem_len);

	if (off + vma_size > info->fix.smem_len) {
		return -EINVAL;
	}

	req_buf_size = vma_size / CONFIG_FB_VFB_NR_BUFFERS;

	/* size check */
	tot_buf_size = buf_list[0].buf_size +
		buf_list[1].buf_size +
		buf_list[2].buf_size;

	if (vma_size > (tot_buf_size - off)) {
		pr_err("%s : vma is too big : req:%ld, real:%ld\n", __func__,
			vma_size, tot_buf_size);
		return -EINVAL;	
	}

	vma->vm_flags    |= (VM_IO | VM_RESERVED);
	vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);

	data = kmalloc(sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	for (i = 0; i < CONFIG_FB_VFB_NR_BUFFERS; i++) {
		struct dma_buf_attachment *attachment;
		struct sg_table *sgt;
		struct scatterlist *sg;

		get_dma_buf(buf_list[i].dmabuf);

		attachment = dma_buf_attach(buf_list[i].dmabuf, info->dev);
		if (IS_ERR(attachment)) {
			ret = PTR_ERR(attachment);
			dma_buf_put(buf_list[i].dmabuf);
			goto err_map;
		}

		sgt = dma_buf_map_attachment(attachment, DMA_FROM_DEVICE);
		if (IS_ERR(sgt)) {
			dma_buf_detach(buf_list[i].dmabuf, attachment);
			dma_buf_put(buf_list[i].dmabuf);
			ret = PTR_ERR(sgt);
			goto err_map;
		}

		for (sg = sgt->sgl; sg && (off >= sg->length); sg = sg_next(sg))
			off -= sg->length;

		for (; sg && (cur_va < vma->vm_end); sg = sg_next(sg)) {
			size_t len;
			len = sg->length - off;
			if (len > (vma->vm_end - cur_va))
				len = vma->vm_end - cur_va;

			ret = remap_pfn_range(vma, cur_va, (sg_phys(sg) + off) >> PAGE_SHIFT,
					len, vma->vm_page_prot);
			if (ret) {
				dma_buf_unmap_attachment(attachment, sgt, DMA_FROM_DEVICE);
				dma_buf_detach(buf_list[i].dmabuf, attachment);
				dma_buf_put(buf_list[i].dmabuf);
				goto err_map;
			}

			cur_va += len;
			off = 0;
		}

		dma_map_sg(info->dev, sgt->sgl, sgt->nents, DMA_FROM_DEVICE);
		data->dmabuf[i] = buf_list[i].dmabuf;
		data->attachment[i] = attachment;
		data->sgt[i] = sgt;
	}

	data->dev = info->dev;
	atomic_set(&data->cnt, 0);
	vma->vm_private_data = data;
	vma->vm_ops = &fb_vm_ops;
	fb_vm_ops.open(vma);

	return 0;

err_map:
	while (i-- > 0) {
		dma_buf_unmap_attachment(data->attachment[i], data->sgt[i], DMA_FROM_DEVICE);
		dma_buf_detach(buf_list[i].dmabuf, data->attachment[i]);
		dma_buf_put(buf_list[i].dmabuf);
	}
	kfree(data);

	return ret;
}

struct ion_client *mdp4_get_ion_client(void);

static int msm_vfb_reg_buffer(struct fb_info *info, struct msmvfb_fd_data *fd_data)
{
	int i;
	int rc = 0;
	unsigned long ionflag = 0;
	struct ion_client *iclient = mdp4_get_ion_client();

	if (buf_list_cnt == CONFIG_FB_VFB_NR_BUFFERS)
		return 0;

	for (i = 0; i < CONFIG_FB_VFB_NR_BUFFERS; i++) {
		if (buf_list[i].buf_fd == 0 &&
		    buf_list[i].buf_fd != fd_data->y_fd) {
			buf_list[i].buf_fd   = fd_data->y_fd;
			buf_list[i].buf_ihdl = ion_import_dma_buf(iclient, buf_list[i].buf_fd);
			if (IS_ERR_OR_NULL(buf_list[i].buf_ihdl)) {
				pr_err("%s ion_import_dma_buf() failed\n", __func__);
				break;
			}

			rc = ion_handle_get_flags(iclient,
						buf_list[i].buf_ihdl,
						&ionflag);
			if (rc)
				return rc;

			buf_list[i].dmabuf = dma_buf_get(buf_list[i].buf_fd);
			rc = ion_handle_get_size(iclient, buf_list[i].buf_ihdl, &buf_list[i].buf_size);

			pr_info("%s (): ion_hdl %p, ion_buf %d, size %lx\n", __func__,
				buf_list[i].buf_ihdl, buf_list[i].buf_fd,
				buf_list[i].buf_size);

			buf_list_cnt++;
			if (buf_list_cnt == CONFIG_FB_VFB_NR_BUFFERS) {
				info->fix.smem_len   = buf_list[0].buf_size * CONFIG_FB_VFB_NR_BUFFERS;
			}
			break;
		}
	}

	return rc;
}

static int msm_vfb_reset(struct fb_info *info)
{
	if (buf_list_cnt != CONFIG_FB_VFB_NR_BUFFERS)
		return -1;

	buf_list_cnt = 0;
	memset(&buf_list, 0, sizeof(buf_list));
	return 0;
}

static int msm_vfb_wait_update(struct fb_info *info)
{
	int ret = 0;

	pr_info("%s enter\n", __func__);
	INIT_COMPLETION(pan_disp_comp);
	ret = wait_for_completion_killable(&pan_disp_comp);
	if (ret < 0)
		ret = -ERESTARTSYS;

	pr_info("%s exit : %d\n", __func__, ret);
	return ret;
}

static int msm_vfb_ioctl(struct fb_info *info, unsigned int cmd,
			unsigned long arg)
{
	int ret = 0;
    int opt;
	struct msmvfb_fd_data fd_data;

	switch (cmd) {
	case MSMVFB_PUT_FD:
		if (copy_from_user(&fd_data,
					(struct msmvfb_fd_data __user *)arg,
					sizeof(fd_data))) {
			pr_err("%s copy_from_user error\n", __func__);
			return -EFAULT;
		}

		if (fd_data.y_fd < 0) {
			pr_err("%s:, y_fd=%d out of value\n", __func__, fd_data.y_fd);
			return -EFAULT;
		}

		do_gettimeofday(&fd_data.time_marker);
		ret = msm_vfb_reg_buffer(info, &fd_data);
		break;

	case MSMVFB_RESET_FD:
		ret = msm_vfb_reset(info);
		break;

	case MSMVFB_WAIT_UPDATE:
		ret = msm_vfb_wait_update(info);
		break;

	case MSMVFB_COMP_SIG_WAIT:
 		ret |= copy_from_user(&opt, (int __user *)arg, sizeof(opt));
		ret |= msm_vfb_comp_sig_wait(info, opt);
		break;

	default:
		pr_err("%s : unknown ioctl (cmd=%x) received!\n", __func__, cmd);
		ret = -EINVAL;
		break;
	}

	return ret;
}

static int msm_vfb_open(struct fb_info *info, int user)
{
	if (!ref_cnt) {
		buf_list_cnt = 0;
		memset(&buf_list, 0, sizeof(buf_list));
	}

	ref_cnt++;
	pr_info("%s %d\n", __func__, ref_cnt);

	return 0;
}

static int msm_vfb_release(struct fb_info *info, int user)
{
	if (!ref_cnt) {
		pr_err("%s: try to close unopened vfb !\n", __func__);
		return -EINVAL;
	}

	ref_cnt--;
	pr_info("%s: release %d !\n", __func__, ref_cnt);

	if (!ref_cnt) {
	}

	return 0;
}


    /*
     *  Initialisation
     */

static int __devinit msm_vfb_probe(struct platform_device *dev)
{
	struct fb_info *info;
	int retval = -ENOMEM;

	info = framebuffer_alloc(sizeof(u32) * 256, &dev->dev);
	if (!info)
		goto err;

	info->screen_base = (char __iomem *)0;
	info->fbops = &msm_vfb_ops;

	retval = fb_find_mode(&info->var, info, mode_option,
			      NULL, 0, NULL, bpp);

	if (!retval || (retval == 4))
		info->var = msm_vfb_default;
	msm_vfb_fix.smem_start = (unsigned long) 0;
	msm_vfb_fix.smem_len = 0;
	info->fix = msm_vfb_fix;
	info->pseudo_palette = info->par;
	info->par = NULL;
	info->flags = FBINFO_FLAG_DEFAULT;

	retval = fb_alloc_cmap(&info->cmap, 256, 0);
	if (retval < 0)
		goto err1;

	retval = register_framebuffer(info);
	if (retval < 0)
		goto err2;
	platform_set_drvdata(dev, info);

	return 0;
err2:
	fb_dealloc_cmap(&info->cmap);
err1:
	framebuffer_release(info);
err:
	return retval;
}

static int msm_vfb_remove(struct platform_device *dev)
{
	struct fb_info *info = platform_get_drvdata(dev);

	if (info) {
		unregister_framebuffer(info);
		fb_dealloc_cmap(&info->cmap);
		framebuffer_release(info);
	}
	return 0;
}

static struct platform_driver msm_vfb_driver = {
	.probe	= msm_vfb_probe,
	.remove = msm_vfb_remove,
	.driver = {
		.name	= "msm_vfb",
	},
};

static struct platform_device *msm_vfb_device;

static int __init msm_vfb_init(void)
{
	int ret = 0;

#ifndef MODULE
	bpp = 8;
	msm_vfb_enable = 1;
	mode_option = "1920x1080-32@60";

	buf_list_cnt = 0;
	memset(&buf_list, 0, sizeof(buf_list));
#endif

	if (!msm_vfb_enable)
		return -ENXIO;

	ret = platform_driver_register(&msm_vfb_driver);

	if (!ret) {
		msm_vfb_device = platform_device_alloc("msm_vfb", 0);

		if (msm_vfb_device)
			ret = platform_device_add(msm_vfb_device);
		else
			ret = -ENOMEM;

		if (ret) {
			platform_device_put(msm_vfb_device);
			platform_driver_unregister(&msm_vfb_driver);
		}
		init_completion(&pan_disp_comp);
	}

	return ret;
}

late_initcall(msm_vfb_init);

#ifdef MODULE
static void __exit msm_vfb_exit(void)
{
	platform_device_unregister(msm_vfb_device);
	platform_driver_unregister(&msm_vfb_driver);
}

module_exit(msm_vfb_exit);

MODULE_LICENSE("GPL");
#endif				/* MODULE */

