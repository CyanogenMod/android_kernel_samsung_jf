/* drivers/i2c/chips/a2220.c - a2220 voice processor driver
 *
 * Copyright (C) 2009 HTC Corporation.
 *
 * Complete rewrite,  anish kumar (anish.singh@samsung.com)
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/irq.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <sound/a2220.h>
#include <linux/kthread.h>
#include <linux/atomic.h>
#include "a2220_firmware.h"
#include <linux/firmware.h>
#include <mach/msm_xo.h>
#include <linux/module.h>

#define ENABLE_DIAG_IOCTLS	(0)

struct a2220_data {
	struct i2c_client *this_client;
	struct mutex lock;
	struct a2220_platform_data *pdata;
	struct miscdevice device;
	atomic_t opened;
	int suspended;
};

static int execute_cmdmsg(struct a2220_data *a2220, unsigned int);
static unsigned int a2220_NS_state = A2220_NS_STATE_AUTO;
static int a2220_current_config = A2220_PATH_SUSPEND;

struct vp_ctxt {
	unsigned char *data;
	unsigned int img_size;
};

struct vp_ctxt the_vp;

static struct msm_xo_voter *xo;

static int xoclk_control(bool onoff)
{
	pr_debug("%s onoff %d\n", __func__, onoff);
	if (!xo) {
		pr_err("%s XO Clock is not available\n", __func__);
		return -EAGAIN;
	}

	if (onoff)
		msm_xo_mode_vote(xo, MSM_XO_MODE_ON);
	else
		msm_xo_mode_vote(xo, MSM_XO_MODE_OFF);
	return 0;
}


static int a2220_i2c_read(struct a2220_data *a2220, char *rxData, int length)
{
	int rc;
	struct i2c_msg msgs[] = {
		{
			.addr = a2220->this_client->addr,
			.flags = I2C_M_RD,
			.len = length,
			.buf = rxData,
		},
	};

	rc = i2c_transfer(a2220->this_client->adapter, msgs, 1);
	if (rc < 0) {
		pr_err("%s : transfer error %d\n", __func__, rc);
		return rc;
	}

	{
		int i = 0;
		for (i = 0; i < length; i++)
			pr_debug("%s: rx[%d] = %2x\n", __func__, i, rxData[i]);
	}
	return 0;
}

static int a2220_i2c_write(struct a2220_data *a2220, char *txData, int length)
{
	int rc;
	struct i2c_msg msg[] = {
		{
			.addr = a2220->this_client->addr,
			.flags = 0,
			.len = length,
			.buf = txData,
		},
	};

	rc = i2c_transfer(a2220->this_client->adapter, msg, 1);
	if (rc < 0) {
		pr_err("%s: transfer error %d\n", __func__, rc);
		return rc;
	}

	{
		int i = 0;
		for (i = 0; i < length; i++)
			pr_debug("%s: tx[%d] = %2x\n", __func__, i, txData[i]);
	}
	return 0;
}

static int a2220_open(struct inode *inode, struct file *file)
{
	int rc = 0;
	struct a2220_data *a2220 = container_of(file->private_data,
			struct a2220_data, device);

	if (!atomic_dec_and_test(&a2220->opened)) {
		pr_err("audience device busy\n");
		atomic_inc(&a2220->opened);
		return -EBUSY;
	}
	return rc;
}

static int a2220_release(struct inode *inode, struct file *file)
{
	struct a2220_data *a2220 = container_of(file->private_data,
			struct a2220_data, device);
	atomic_inc(&a2220->opened); /* release the device */
	return 0;
}

#ifdef AUDIENCE_BYPASS
#define A100_msg_mutimedia1   0x801C0000
#define A100_msg_mutimedia2   0x8026001F
#define A100_msg_mutimedia3   0x800C0B03
#define A100_msg_mutimedia4   0x800D0001
#define A100_msg_mutimedia5   0x800C0A03
#define A100_msg_mutimedia6   0x800D0001
#endif

static void a2220_i2c_sw_reset(struct a2220_data *a2220, unsigned int reset_cmd)
{
	int rc = 0;
	unsigned char msgbuf[4];

	msgbuf[0] = (reset_cmd >> 24) & 0xFF;
	msgbuf[1] = (reset_cmd >> 16) & 0xFF;
	msgbuf[2] = (reset_cmd >> 8) & 0xFF;
	msgbuf[3] = reset_cmd & 0xFF;

	pr_info("%s: %08x\n", __func__, reset_cmd);

	rc = a2220_i2c_write(a2220, msgbuf, 4);
	if (!rc)
		msleep(20);
}

	static ssize_t
a2220_hw_reset(struct a2220_data *a2220, struct a2220img *img)
{
	struct a2220img *vp = img;
	int rc = 0, i, pass = 0;
	int remaining;
	int retry = RETRY_CNT;
	unsigned char *index;
	char buf[2];

	pr_info("%s\n", __func__);
	while (retry--) {
		/* Reset A2220 chip */
		gpio_set_value(a2220->pdata->gpio_reset, 0);

		msleep(20);

		/* Take out of reset */
		gpio_set_value(a2220->pdata->gpio_reset, 1);

		msleep(50); /* Delay before send I2C command */

		/* Boot Cmd to A2220 */
		buf[0] = A2220_msg_BOOT >> 8;
		buf[1] = A2220_msg_BOOT & 0xff;

		rc = a2220_i2c_write(a2220, buf, 2);
		if (rc < 0) {
			pr_err("%s: set boot mode error (%d retries left)\n",
					__func__, retry);
			continue;
		}

		msleep(20);
		rc = a2220_i2c_read(a2220, buf, 1);
		if (rc < 0) {
			pr_err("%s: boot mode ack error (%d retries left)\n",
					__func__, retry);
			continue;
		}

		remaining = vp->img_size / 32;
		index = vp->buf;

		for (; remaining; remaining--, index += 32) {
			rc = a2220_i2c_write(a2220, index, 32);
			if (rc < 0)
				break;
		}

		if (rc >= 0 && vp->img_size % 32)
			rc = a2220_i2c_write(a2220, index, vp->img_size % 32);

		if (rc < 0) {
			pr_err("%s: fw load error %d (%d retries left)\n",
					__func__, rc, retry);
			continue;
		}

		msleep(20); /* Delay time before issue a Sync Cmd */

		for (i = 0; i < 10 ; i++)
			msleep(20);

		rc = execute_cmdmsg(a2220, A100_msg_Sync);
		if (rc < 0) {
			pr_err("%s: sync command error %d (%d retries left)\n",
					__func__, rc, retry);
			continue;
		}

		pass = 1;
		break;
	}
	return rc;
}

unsigned char bypass_multimedia[] = {
	0x80, 0x52, 0x00, 0x48,
	0x80, 0x52, 0x00, 0x5C,
	0x80, 0x10, 0x00, 0x01,
};

int a2220_bootup_init(struct a2220_data *a2220, struct a2220img *pImg)
{
	struct a2220img *vp = pImg;
	int rc, pass = 0, size, i;
	unsigned int msg;
	unsigned int i2c_write_size = 256;
	int remaining;
	int retry = RETRY_CNT;
	unsigned char *index;
	unsigned char *pMsg;
	unsigned char *i2c_cmds;
	char buf[2];
	xoclk_control(true);

	pr_info("%s\n", __func__);

	msleep(100);
	while (retry--) {
		/* Reset A2220 chip */
		gpio_set_value(a2220->pdata->gpio_reset, 0);

		msleep(20);

		/* Take out of reset */
		gpio_set_value(a2220->pdata->gpio_reset, 1);

		msleep(50); /* Delay before send I2C command */

		/* Boot Cmd to A2220 */
		buf[0] = A2220_msg_BOOT >> 8;
		buf[1] = A2220_msg_BOOT & 0xff;

		rc = a2220_i2c_write(a2220, buf, 2);
		if (rc < 0) {
			pr_err("%s: set boot mode error (%d retries left)\n",
					__func__, retry);
			continue;
		}

		msleep(20);
		rc = a2220_i2c_read(a2220, buf, 1);
		if (rc < 0) {
			pr_err("%s: boot mode ack error (%d retries left)\n",
					__func__, retry);
			continue;
		}


		remaining = vp->img_size / i2c_write_size;
		index = vp->buf;
		pr_info("%s: starting to load image (%d passes)...\n",
				__func__,
				remaining + !!(vp->img_size % i2c_write_size));

		for (; remaining; remaining--, index += i2c_write_size) {
			rc = a2220_i2c_write(a2220, index, i2c_write_size);
			if (rc < 0)
				break;
		}

		if (rc >= 0 && vp->img_size % i2c_write_size)
			rc = a2220_i2c_write(a2220, index,
					vp->img_size % i2c_write_size);

		if (rc < 0) {
			pr_err("%s: fw load error %d (%d retries left)\n",
					__func__, rc, retry);
			continue;
		}
		pr_info("%s:a2220_bootup_init 7\n", __func__);

		msleep(20); /* Delay time before issue a Sync Cmd */

		pr_info("%s:firmware loaded successfully\n", __func__);

		msleep(200);

		rc = execute_cmdmsg(a2220, A100_msg_Sync);
		if (rc < 0) {
			pr_err("%s: sync command error %d (%d retries left)\n",
					__func__, rc, retry);
			continue;
		}
		pr_info("%s:a2220_bootup_init 8\n", __func__);

		pass = 1;
		break;
	}

	/*setting bypass mode for Multi-Media*/
	pMsg = (unsigned char *)&msg;
	i2c_cmds = bypass_multimedia;
	size = sizeof(bypass_multimedia);

	for (i = 0 ; i < size ; i += 4) {
		pMsg[3] = i2c_cmds[i];
		pMsg[2] = i2c_cmds[i+1];
		pMsg[1] = i2c_cmds[i+2];
		pMsg[0] = i2c_cmds[i+3];

		do {
			rc = execute_cmdmsg(a2220, msg);
		} while ((rc < 0) && --retry);

		if (rc < 0)
			pr_err("Audience bypass mode setting Failed\n");
	}

	pr_info("%s : a2220_bootup_init - finish\n", __func__);
	xoclk_control(false);
	return rc;
}

static unsigned char phonecall_receiver_nson[] = {
	0x80, 0x31, 0x00, 0x00,
};

static unsigned char phonecall_receiver_nsonoff[] = {
	0x80, 0x17, 0x00, 0x4B,
	0x80, 0x18, 0x00, 0x03,
};
#if defined CONFIG_MACH_SERRANO_ATT
static unsigned char phonecall_receiver_nsoffon[] = {
	0x80, 0x17, 0x00, 0x4B,
	0x80, 0x18, 0x00, 0x07,
};
#else
static unsigned char phonecall_receiver_nsoffon[] = {
	0x80, 0x17, 0x00, 0x4B,
	0x80, 0x18, 0x00, 0x06,
};
#endif
static unsigned char phonecall_receiver_nson_exton[] = {
	0x80, 0x52, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x09,
	0x80, 0x18, 0x00, 0x00,
};

static unsigned char phonecall_receiver_nson_extoff[] = {
	0x80, 0x52, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x09,
	0x80, 0x18, 0x00, 0x01,
};
#if defined CONFIG_MACH_SERRANO_ATT
static unsigned char phonecall_receiver_nson_vol0[] = {
	0x80, 0x17, 0x00, 0x3D,
	0x80, 0x18, 0x00, 0x06,
};

static unsigned char phonecall_receiver_nson_vol1[] = {
	0x80, 0x17, 0x00, 0x3D,
	0x80, 0x18, 0x00, 0x09,
};
#else
static unsigned char phonecall_receiver_nson_vol0[] = {
	0x80, 0x17, 0x00, 0x3D,
	0x80, 0x18, 0x00, 0x0F,
};

static unsigned char phonecall_receiver_nson_vol1[] = {
	0x80, 0x17, 0x00, 0x3D,
	0x80, 0x18, 0x00, 0x0F,
};
#endif
static unsigned char phonecall_receiver_nson_vol2[] = {
	0x80, 0x17, 0x00, 0x3D,
	0x80, 0x18, 0x00, 0x0C,
};

static unsigned char phonecall_receiver_nson_vol3[] = {
	0x80, 0x17, 0x00, 0x3D,
	0x80, 0x18, 0x00, 0x09,
};

static unsigned char phonecall_receiver_nson_vol4[] = {
	0x80, 0x17, 0x00, 0x3D,
	0x80, 0x18, 0x00, 0x06,
};

static unsigned char phonecall_receiver_nson_vol5[] = {
	0x80, 0x17, 0x00, 0x3D,
	0x80, 0x18, 0x00, 0x03,
};
#if defined CONFIG_MACH_SERRANO_ATT
static unsigned char phonecall_receiver_nson_volmin[] = {
	0x80, 0x17, 0x00, 0x25,
	0x80, 0x18, 0xFF, 0xFE,
};

static unsigned char phonecall_receiver_nson_volmax[] = {
	0x80, 0x17, 0x00, 0x25,
	0x80, 0x18, 0x00, 0x0C,
};
#else
static unsigned char phonecall_receiver_nson_volmin[] = {
	0x80, 0x17, 0x00, 0x25,
	0x80, 0x18, 0x00, 0x06,
};

static unsigned char phonecall_receiver_nson_volmax[] = {
	0x80, 0x17, 0x00, 0x25,
	0x80, 0x18, 0x00, 0x11,
};
#endif
static unsigned char ft_loopback[] = {
	0x80, 0x31, 0x00, 0x00,
	0x80, 0x1B, 0x00, 0x00,
	0x80, 0x1B, 0x01, 0x00,
	0x80, 0x1B, 0x02, 0x00,
	0x80, 0x1B, 0x03, 0x00,
	0x80, 0x1B, 0x04, 0x00,
	0x80, 0x1B, 0x05, 0x00,
	0x80, 0x1B, 0x06, 0x00,
	0x80, 0x1B, 0x07, 0x00,
	0x80, 0x15, 0x00, 0x00,
	0x80, 0x15, 0x01, 0x00,
	0x80, 0x15, 0x02, 0x00,
	0x80, 0x15, 0x03, 0x00,
	0x80, 0x15, 0x04, 0x00,
	0x80, 0x15, 0x05, 0x00,
	0x80, 0x15, 0x06, 0x00,
	0x80, 0x15, 0x07, 0x00,
	0x80, 0x17, 0x00, 0x4B,
	0x80, 0x18, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x42,
	0x80, 0x18, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x40,
	0x80, 0x18, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x0D,
	0x80, 0x18, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x20,
	0x80, 0x18, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x1F,
	0x80, 0x18, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x30,
	0x80, 0x18, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x31,
	0x80, 0x18, 0x00, 0x00,
};

static unsigned char phonecall_receiver_nson_wb[] = {
	0x80, 0x31, 0x00, 0x01,
};

static unsigned char phonecall_receiver_nsoff[] = {
	0x80, 0x52, 0x00, 0x48,
	0x80, 0x52, 0x00, 0x5C,
	0x80, 0x10, 0x00, 0x01,
};

static unsigned char phonecall_headset[] = {
	0x80, 0x1C, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x02, 0x80, 0x18, 0x00, 0x03,
	0x80, 0x26, 0x00, 0x1A,
	0x80, 0x1B, 0x00, 0x00,
	0x80, 0x1B, 0x01, 0x00,
	0x80, 0x15, 0x04, 0x00,
	0x80, 0x15, 0x05, 0x00,
	0x80, 0x1B, 0x02, 0x00,
	0x80, 0x1B, 0x03, 0x00,
	0x80, 0x15, 0x06, 0x00,
	0x80, 0x15, 0x07, 0x00,
	0x80, 0x17, 0x00, 0x4B, 0x80, 0x18, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x15, 0x80, 0x18, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x03, 0x80, 0x18, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x12, 0x80, 0x18, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x34, 0x80, 0x18, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x04, 0x80, 0x18, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x28, 0x80, 0x18, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x09, 0x80, 0x18, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x0E, 0x80, 0x18, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x4C, 0x80, 0x18, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x20, 0x80, 0x18, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x1F, 0x80, 0x18, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x30, 0x80, 0x18, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x31, 0x80, 0x18, 0x00, 0x01,
	0x80, 0x17, 0x00, 0x1A, 0x80, 0x18, 0x00, 0x00,
};

static unsigned char phonecall_speaker[] = {
	0x80, 0x31, 0x00, 0x01,
};

static unsigned char phonecall_bt[] = {
	0x80, 0x17, 0x00, 0x02,
	0x80, 0x18, 0x00, 0x03,
	0x80, 0x26, 0x00, 0x06,
	0x80, 0x1C, 0x00, 0x00,
	0x80, 0x1B, 0x00, 0x00,
	0x80, 0x15, 0x00, 0x00,
};

static unsigned char phonecall_tty[] = {
	0x80, 0x26, 0x00, 0x15,
	0x80, 0x1C, 0x00, 0x00,
	0x80, 0x1B, 0x00, 0x00,
	0x80, 0x15, 0x00, 0xFB,
};

static unsigned char INT_MIC_recording_receiver[] = {
	0x80, 0x26, 0x00, 0x07,
	0x80, 0x1C, 0x00, 0x00,
	0x80, 0x1B, 0x00, 0x12,
	0x80, 0x15, 0x00, 0x00,
};

static unsigned char EXT_MIC_recording[] = {
	0x80, 0x26, 0x00, 0x15,
	0x80, 0x1C, 0x00, 0x00,
	0x80, 0x1B, 0x00, 0x12,
	0x80, 0x15, 0x00, 0x00,
};

static unsigned char INT_MIC_recording_speaker[] = {
	0x80, 0x17, 0x00, 0x02,
	0x80, 0x18, 0x00, 0x02,
	0x80, 0x1C, 0x00, 0x00,
	0x80, 0x1B, 0x00, 0x12,
	0x80, 0x15, 0x00, 0x00,
};

static unsigned char BACK_MIC_recording[] = {
	0x80, 0x17, 0x00, 0x02,
	0x80, 0x18, 0x00, 0x02,
	0x80, 0x26, 0x00, 0x15,
	0x80, 0x1C, 0x00, 0x01,
	0x80, 0x17, 0x00, 0x04,
	0x80, 0x18, 0x00, 0x01,
	0x80, 0x17, 0x00, 0x1A,
	0x80, 0x18, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x00,
	0x80, 0x18, 0x00, 0x00,
	0x80, 0x1B, 0x00, 0x12,
	0x80, 0x15, 0x00, 0x06,
};

static unsigned char vr_no_ns_receiver[] = {
	0x80, 0x17, 0x00, 0x02,
	0x80, 0x18, 0x00, 0x00,
	0x80, 0x1C, 0x00, 0x00,
	0x80, 0x1B, 0x00, 0x0C,
	0x80, 0x1B, 0x01, 0x0C,
	0x80, 0x15, 0x00, 0x00,
};

static unsigned char vr_no_ns_headset[] = {
	0x80, 0x17, 0x00, 0x02,
	0x80, 0x18, 0x00, 0x03,
	0x80, 0x26, 0x00, 0x15,
	0x80, 0x1C, 0x00, 0x00,
	0x80, 0x1B, 0x00, 0x12,
	0x80, 0x15, 0x00, 0x00,
};

static unsigned char vr_no_ns_speaker[] = {
	0x80, 0x17, 0x00, 0x02,
	0x80, 0x18, 0x00, 0x02,
	0x80, 0x1C, 0x00, 0x00,
	0x80, 0x1B, 0x00, 0x0C,
	0x80, 0x15, 0x00, 0x00,
};

static unsigned char vr_no_ns_bt[] = {
	0x80, 0x26, 0x00, 0x06,
	0x80, 0x1C, 0x00, 0x00,
	0x80, 0x1B, 0x00, 0x00,
	0x80, 0x15, 0x00, 0x00,
};

static unsigned char vr_ns_receiver[] = {
	0x80, 0x17, 0x00, 0x02,
	0x80, 0x18, 0x00, 0x00,
	0x80, 0x1C, 0x00, 0x01,
	0x80, 0x17, 0x00, 0x1A,
	0x80, 0x18, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x04,
	0x80, 0x18, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x00,
	0x80, 0x18, 0x00, 0x04,
	0x80, 0x1B, 0x00, 0x0C,
	0x80, 0x1B, 0x01, 0x0C,
	0x80, 0x15, 0x00, 0x00,
};

static unsigned char vr_ns_headset[] = {
	0x80, 0x17, 0x00, 0x02,
	0x80, 0x18, 0x00, 0x03,
	0x80, 0x26, 0x00, 0x15,
	0x80, 0x1C, 0x00, 0x01,
	0x80, 0x17, 0x00, 0x00,
	0x80, 0x18, 0x00, 0x02,
	0x80, 0x17, 0x00, 0x1A,
	0x80, 0x18, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x04,
	0x80, 0x18, 0x00, 0x00,
	0x80, 0x1B, 0x00, 0x12,
	0x80, 0x15, 0x00, 0x00,
};

static unsigned char vr_ns_speaker[] = {
	0x80, 0x17, 0x00, 0x02,
	0x80, 0x18, 0x00, 0x02,
	0x80, 0x1C, 0x00, 0x01,
	0x80, 0x17, 0x00, 0x00,
	0x80, 0x18, 0x00, 0x04,
	0x80, 0x17, 0x00, 0x04,
	0x80, 0x18, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x1A,
	0x80, 0x18, 0x00, 0x00,
	0x80, 0x1B, 0x00, 0x0C,
	0x80, 0x15, 0x00, 0x00,
};

static unsigned char vr_ns_bt[] = {
	0x80, 0x26, 0x00, 0x06,
	0x80, 0x1C, 0x00, 0x01,
	0x80, 0x17, 0x00, 0x00,
	0x80, 0x18, 0x00, 0x02,
	0x80, 0x17, 0x00, 0x04,
	0x80, 0x18, 0x00, 0x00,
	0x80, 0x17, 0x00, 0x1A,
	0x80, 0x18, 0x00, 0x00,
	0x80, 0x1B, 0x00, 0x00,
	0x80, 0x15, 0x00, 0x00,
};

static unsigned char suspend_mode[] = {
	0x80, 0x10, 0x00, 0x01
};

static unsigned char pcm_reset[] = {
	0x80, 0x31, 0x00, 0x00
};

static ssize_t chk_wakeup_a2220(struct a2220_data *a2220)
{
	int rc = 0, retry = 4;

	if (a2220->suspended == 1) {
		msleep(20);
		gpio_set_value(a2220->pdata->gpio_wakeup, 0);
		msleep(30);
		do {
			rc = execute_cmdmsg(a2220, A100_msg_Sync);
		} while ((rc < 0) && --retry);

		if ((retry == 0) && (rc < 0)) {
			struct a2220img img;
			img.buf = a2220_firmware_buf;
			img.img_size = sizeof(a2220_firmware_buf);
			rc = a2220_hw_reset(a2220, &img);
		}
		if (rc < 0)
			pr_err("Audience HW Reset Failed rc %d\n", rc);
		gpio_set_value(a2220->pdata->gpio_wakeup, 1);
		if (rc < 0) {
			pr_err("%s: failed (%d)\n", __func__, rc);
			goto wakeup_sync_err;
		}
		a2220->suspended = 0;
	}
wakeup_sync_err:
	return rc;
}

/* Filter commands according to noise suppression state forced by
 * A2220_SET_NS_STATE ioctl.
 *
 * For this function to operate properly, all configurations must include
 * both A100_msg_Bypass and Mic_Config commands even if default values
 * are selected or if Mic_Config is useless because VP is off
 */
int a2220_filter_vp_cmd(int cmd, int mode)
{
	int msg = (cmd >> 16) & 0xFFFF;
	int filtered_cmd = cmd;
	static int a2220_param_ID;

	if (a2220_NS_state == A2220_NS_STATE_AUTO)
		return cmd;

	switch (msg) {
	case A100_msg_Bypass:
		if (a2220_NS_state == A2220_NS_STATE_OFF)
			filtered_cmd = A2220_msg_VP_OFF;
		else
			filtered_cmd = A2220_msg_VP_ON;
		break;
	case A100_msg_SetAlgorithmParmID:
		a2220_param_ID = cmd & 0xFFFF;
		break;
	case A100_msg_SetAlgorithmParm:
		if (a2220_param_ID == Mic_Config) {
			if (a2220_NS_state == A2220_NS_STATE_CT)
				filtered_cmd = (msg << 16);
			else if (a2220_NS_state == A2220_NS_STATE_FT)
				filtered_cmd = (msg << 16) | 0x0002;
		}
		break;
	default:
		if (mode == A2220_CONFIG_VP)
			filtered_cmd = -1;
		break;
	}

	pr_info("%s: %x filtered = %x, a2220_NS_state %d, mode %d\n", __func__,
			cmd, filtered_cmd, a2220_NS_state, mode);

	return filtered_cmd;
}

int a2220_set_config(struct a2220_data *a2220, char newid, int mode)
{
	int i = 0, rc = 0, size = 0;
	int retry = 4;
	unsigned int sw_reset = 0;
	unsigned char *i2c_cmds;
	unsigned int msg;
	unsigned char *pMsg;

	pr_info("[AUD] new mode = %d\n", newid);
	if ((a2220->suspended) && (newid == A2220_PATH_SUSPEND))
		return rc;

	if ((a2220_current_config == newid) &&
		(a2220_current_config != A2220_PATH_PCMRESET)) {
		pr_info("already configured this path!!!\n");
		return rc;
	}

	rc = chk_wakeup_a2220(a2220);
	if (rc < 0)
		return rc;

	sw_reset = ((A100_msg_Reset << 16) | RESET_IMMEDIATE);

	switch (newid) {
	case A2220_PATH_INCALL_RECEIVER_NSON:
		i2c_cmds = phonecall_receiver_nson;
		size = sizeof(phonecall_receiver_nson);
		break;
	case A2220_PATH_INCALL_RECEIVER_NSONOFF:
		i2c_cmds = phonecall_receiver_nsonoff;
		size = sizeof(phonecall_receiver_nsonoff);
		break;
	case A2220_PATH_INCALL_RECEIVER_NSOFFON:
		i2c_cmds = phonecall_receiver_nsoffon;
		size = sizeof(phonecall_receiver_nsoffon);
		break;
	case A2220_PATH_INCALL_RECEIVER_EXTON:
		i2c_cmds = phonecall_receiver_nson_exton;
		size = sizeof(phonecall_receiver_nson_exton);
		break;
	case A2220_PATH_INCALL_RECEIVER_EXTOFF:
		i2c_cmds = phonecall_receiver_nson_extoff;
		size = sizeof(phonecall_receiver_nson_extoff);
		break;
	case A2220_PATH_INCALL_RECEIVER_VOL0:
		i2c_cmds = phonecall_receiver_nson_vol0;
		size = sizeof(phonecall_receiver_nson_vol0);
		break;
	case A2220_PATH_INCALL_RECEIVER_VOL1:
		i2c_cmds = phonecall_receiver_nson_vol1;
		size = sizeof(phonecall_receiver_nson_vol1);
		break;
	case A2220_PATH_INCALL_RECEIVER_VOL2:
		i2c_cmds = phonecall_receiver_nson_vol2;
		size = sizeof(phonecall_receiver_nson_vol2);
		break;
	case A2220_PATH_INCALL_RECEIVER_VOL3:
		i2c_cmds = phonecall_receiver_nson_vol3;
		size = sizeof(phonecall_receiver_nson_vol3);
		break;
	case A2220_PATH_INCALL_RECEIVER_VOL4:
		i2c_cmds = phonecall_receiver_nson_vol4;
		size = sizeof(phonecall_receiver_nson_vol4);
		break;
	case A2220_PATH_INCALL_RECEIVER_VOL5:
		i2c_cmds = phonecall_receiver_nson_vol5;
		size = sizeof(phonecall_receiver_nson_vol5);
		break;
	case A2220_PATH_INCALL_RECEIVER_NEA_VOLMIN:
		i2c_cmds = phonecall_receiver_nson_volmin;
		size = sizeof(phonecall_receiver_nson_volmin);
		break;
	case A2220_PATH_INCALL_RECEIVER_NEA_VOLMAX:
		i2c_cmds = phonecall_receiver_nson_volmax;
		size = sizeof(phonecall_receiver_nson_volmax);
		break;
	case A2220_PATH_INCALL_RECEIVER_NSON_WB:
		i2c_cmds = phonecall_receiver_nson_wb;
		size = sizeof(phonecall_receiver_nson_wb);
		break;
	case A2220_PATH_INCALL_RECEIVER_NSOFF:
		i2c_cmds = phonecall_receiver_nsoff;
		size = sizeof(phonecall_receiver_nsoff);
	break;
#ifdef AUDIENCE_BYPASS
	case A2220_PATH_BYPASS_MULTIMEDIA:
		pr_info("%s:A2220_PATH_BYPASS_MULTIMEDIA\n", __func__);
		i2c_cmds = bypass_multimedia;
		size = sizeof(bypass_multimedia);
		break;
#endif
	case A2220_PATH_INCALL_HEADSET:
		i2c_cmds = phonecall_headset;
		size = sizeof(phonecall_headset);
		break;
	case A2220_PATH_INCALL_SPEAKER:
		i2c_cmds = phonecall_speaker;
			size = sizeof(phonecall_speaker);
		break;
	case A2220_PATH_INCALL_BT:
		i2c_cmds = phonecall_bt;
			size = sizeof(phonecall_bt);
		break;
	case A2220_PATH_INCALL_TTY:
		i2c_cmds = phonecall_tty;
		size = sizeof(phonecall_tty);
		break;
	case A2220_PATH_VR_NO_NS_RECEIVER:
		i2c_cmds = vr_no_ns_receiver;
		size = sizeof(vr_no_ns_receiver);
		break;
	case A2220_PATH_VR_NO_NS_HEADSET:
		i2c_cmds = vr_no_ns_headset;
		size = sizeof(vr_no_ns_headset);
		break;
	case A2220_PATH_VR_NO_NS_SPEAKER:
		i2c_cmds = vr_no_ns_speaker;
		size = sizeof(vr_no_ns_speaker);
		break;
	case A2220_PATH_VR_NO_NS_BT:
		i2c_cmds = vr_no_ns_bt;
		size = sizeof(vr_no_ns_bt);
		break;
	case A2220_PATH_VR_NS_RECEIVER:
		i2c_cmds = vr_ns_receiver;
		size = sizeof(vr_ns_receiver);
		break;
	case A2220_PATH_VR_NS_HEADSET:
		i2c_cmds = vr_ns_headset;
		size = sizeof(vr_ns_headset);
		break;
	case A2220_PATH_VR_NS_SPEAKER:
		i2c_cmds = vr_ns_speaker;
		size = sizeof(vr_ns_speaker);
		break;
	case A2220_PATH_VR_NS_BT:
		i2c_cmds = vr_ns_bt;
		size = sizeof(vr_ns_bt);
		break;
	case A2220_PATH_RECORD_RECEIVER:
		i2c_cmds = INT_MIC_recording_receiver;
		size = sizeof(INT_MIC_recording_receiver);
		break;
	case A2220_PATH_RECORD_HEADSET:
		i2c_cmds = EXT_MIC_recording;
		size = sizeof(EXT_MIC_recording);
		break;
	case A2220_PATH_RECORD_SPEAKER:
		i2c_cmds = INT_MIC_recording_speaker;
		size = sizeof(INT_MIC_recording_speaker);
		break;
	case A2220_PATH_RECORD_BT:
		i2c_cmds = phonecall_bt;
		size = sizeof(phonecall_bt);
		break;
	case A2220_PATH_SUSPEND:
		i2c_cmds = (unsigned char *)suspend_mode;
		size = sizeof(suspend_mode);
		break;
	case A2220_PATH_CAMCORDER:
		i2c_cmds = BACK_MIC_recording;
		size = sizeof(BACK_MIC_recording);
		break;
	case A2220_PATH_PCMRESET:
		i2c_cmds = pcm_reset;
		size = sizeof(pcm_reset);
		msleep(30);
		break;
	case A2220_PATH_FT_LOOPBACK:
		i2c_cmds = ft_loopback;
		size = sizeof(ft_loopback);
		break;
	default:
		pr_err("%s: invalid cmd %d\n", __func__, newid);
		rc = -1;
		goto input_err;
		break;
	}

	a2220_current_config = newid;

	pr_info("%s: change to mode %d\n", __func__, newid);
	pr_info("%s: block write start (size = %d)\n", __func__, size);
	for (i = 1; i <= size; i++) {
		pr_info("%x ", *(i2c_cmds + i - 1));
		if (!(i % 4))
			pr_info("\n");
	}

	pMsg = (unsigned char *)&msg;

	for (i = 0 ; i < size ; i += 4) {
		pMsg[3] = i2c_cmds[i];
		pMsg[2] = i2c_cmds[i+1];
		pMsg[1] = i2c_cmds[i+2];
		pMsg[0] = i2c_cmds[i+3];

		do {
			rc = execute_cmdmsg(a2220, msg);
		} while ((rc < 0) && --retry);

		if ((retry == 0) && (rc < 0)) {
			struct a2220img img;
			img.buf = a2220_firmware_buf;
			img.img_size = sizeof(a2220_firmware_buf);
			rc = a2220_hw_reset(a2220, &img);
			if (rc < 0) {
				pr_err("Audience HW Reset Failed\n");
				return rc;
			}
		}

	}

input_err:
	return rc;
}

int execute_cmdmsg(struct a2220_data *a2220, unsigned int msg)
{
	int rc = 0;
	int retries, pass = 0;
	unsigned char msgbuf[4];
	unsigned char chkbuf[4];
	unsigned int sw_reset = 0;

if (msg == A100_msg_Sleep)
	return 0;
	sw_reset = ((A100_msg_Reset << 16) | RESET_IMMEDIATE);

	msgbuf[0] = (msg >> 24) & 0xFF;
	msgbuf[1] = (msg >> 16) & 0xFF;
	msgbuf[2] = (msg >> 8) & 0xFF;
	msgbuf[3] = msg & 0xFF;

	pr_info("%s : execute_cmdmsg :: %x %x %x %x\n" , __func__,
			msgbuf[0] , msgbuf[1] , msgbuf[2] , msgbuf[3]);
	memcpy(chkbuf, msgbuf, 4);

	rc = a2220_i2c_write(a2220, msgbuf, 4);
	if (rc < 0) {
		pr_err("%s: error %d\n", __func__, rc);
		a2220_i2c_sw_reset(a2220, sw_reset);

		if (msg == A100_msg_Sleep) {
			pr_info("execute_cmdmsg ...go to suspend first\n");
			a2220->suspended = 1;
			msleep(120);

		}
		return rc;
	}

	pr_info("execute_cmdmsg + 1\n");
	/* We don't need to get Ack after sending out a suspend command */
	if (msg == A100_msg_Sleep) {
		pr_info("%s : ...go to suspend first\n", __func__);
		a2220->suspended = 1;

		return rc;
	}
	pr_info("execute_cmdmsg + 2\n");
#if defined CONFIG_MACH_SERRANO_ATT
	usleep(2000);
#endif

	retries = POLLING_RETRY_CNT;
	while (retries--) {
		rc = 0;
		memset(msgbuf, 0, sizeof(msgbuf));
		rc = a2220_i2c_read(a2220, msgbuf, 4);
		if (rc < 0) {
			pr_err("%s: ack-read error %d (%d retries)\n", __func__,
					rc, retries);
			continue;
		}

		pr_info("execute_cmdmsg + 3\n");

		if (msgbuf[0] == 0x80  && msgbuf[1] == chkbuf[1]) {
			pass = 1;
			pr_info("execute_cmdmsg + 4\n");
			pr_info("got ACK\n");
			break;
		} else if (msgbuf[0] == 0xff && msgbuf[1] == 0xff) {
			pr_err("%s: illegal cmd %08x\n", __func__, msg);
			rc = -EINVAL;
			pr_info("execute_cmdmsg + 5\n");
		} else if (msgbuf[0] == 0x00 && msgbuf[1] == 0x00) {
			pr_info("%s: not ready (%d retries)\n", __func__,
					retries);
			pr_info("execute_cmdmsg + 6\n");
			rc = -EBUSY;
		} else {
			pr_info("%s: cmd/ack mismatch: (%d retries left)\n",
					__func__,
					retries);
			pr_err("%s: msgbuf[0] = %x\n", __func__, msgbuf[0]);
			pr_err("%s: msgbuf[1] = %x\n", __func__, msgbuf[1]);
			pr_err("%s: msgbuf[2] = %x\n", __func__, msgbuf[2]);
			pr_err("%s: msgbuf[3] = %x\n", __func__, msgbuf[3]);
			pr_err("execute_cmdmsg + 7\n");
			rc = -EBUSY;
		}
		msleep(20); /* use polling */
	}

	if (!pass) {
		pr_err("%s: failed execute cmd %08x (%d)\n", __func__,
				msg, rc);
		a2220_i2c_sw_reset(a2220, sw_reset);
	}

	pr_info("execute_cmdmsg - finish\n");

	return rc;
}

#if ENABLE_DIAG_IOCTLS
static int a2220_set_mic_state(struct a2220_data *a2220, char miccase)
{
	int rc = 0;
	unsigned int cmd_msg = 0;

	switch (miccase) {
	case 1: /* Mic-1 ON / Mic-2 OFF */
		cmd_msg = 0x80260007;
		break;
	case 2: /* Mic-1 OFF / Mic-2 ON */
		cmd_msg = 0x80260015;
		break;
	case 3: /* both ON */
		cmd_msg = 0x80260001;
		break;
	case 4: /* both OFF */
		cmd_msg = 0x80260006;
		break;
	default:
		pr_info("%s: invalid input %d\n", __func__, miccase);
		rc = -EINVAL;
		break;
	}
	rc = execute_cmdmsg(a2220, cmd_msg);
	return rc;
}

static int exe_cmd_in_file(struct a2220_data *a2220, unsigned char *incmd)
{
	int rc = 0;
	int i = 0;
	unsigned int cmd_msg = 0;
	unsigned char tmp = 0;

	for (i = 0; i < 4; i++) {
		tmp = *(incmd + i);
		cmd_msg |= (unsigned int)tmp;
		if (i != 3)
			cmd_msg = cmd_msg << 8;
	}
	rc = execute_cmdmsg(a2220, cmd_msg);
	if (rc < 0)
		pr_err("%s: cmd %08x error %d\n", __func__, cmd_msg, rc);
	return rc;
}
#endif /* ENABLE_DIAG_IOCTLS */

static int thread_start(void *a2220)
{
	int rc = 0;
	struct a2220img img;
	pr_info("%s\n", __func__);
	img.buf = a2220_firmware_buf;
	img.img_size = sizeof(a2220_firmware_buf);
	rc = a2220_bootup_init(a2220, &img);
	return rc;
}

static long a2220_ioctl(struct file *file, unsigned int cmd,
		unsigned long arg)
{
	void __user *argp = (void __user *)arg;
	struct a2220_data *a2220 = container_of(file->private_data,
			struct a2220_data, device);
	static struct task_struct *task;
	int rc = 0;
#if ENABLE_DIAG_IOCTLS
	char msg[4];
	int mic_cases = 0;
	int mic_sel = 0;
#endif
	int ns_state;
	xoclk_control(true);
	switch (cmd) {
	case A2220_BOOTUP_INIT:
		task = kthread_run(thread_start, a2220, "thread_start");
		if (IS_ERR(task)) {
			rc = PTR_ERR(task);
			task = NULL;
		}
		break;
	case A2220_SET_CONFIG:
		mutex_lock(&a2220->lock);
		rc = a2220_set_config(a2220, arg, A2220_CONFIG_FULL);
		if (rc < 0)
			goto handle_error;
		mutex_unlock(&a2220->lock);
			break;
	case A2220_SET_NS_STATE:
		mutex_lock(&a2220->lock);
		if (copy_from_user(&ns_state, argp, sizeof(ns_state))) {
			rc = -EFAULT;
			goto handle_error;
		}
		if (ns_state >= A2220_NS_NUM_STATES) {
			rc = -EINVAL;
			goto handle_error;
		}
		a2220_NS_state = ns_state;
		if (!a2220->suspended)
			a2220_set_config(a2220, a2220_current_config,
					A2220_CONFIG_VP);
		mutex_unlock(&a2220->lock);
		break;
#if ENABLE_DIAG_IOCTLS
	case A2220_SET_MIC_ONOFF:
		mutex_lock(&a2220->lock);
		rc = chk_wakeup_a2220(a2220);
		if (rc < 0)
			goto handle_error;
			if (copy_from_user(&mic_cases,
					argp, sizeof(mic_cases))) {
				rc = -EFAULT;
			goto handle_error;
		}
		rc = a2220_set_mic_state(a2220, mic_cases);
		if (rc < 0) {
			pr_err("%s: A2220_SET_MIC_ONOFF %d error %d!\n",
					__func__, mic_cases, rc);
			goto handle_error;
		}
		mutex_unlock(&a2220->lock);
		break;
	case A2220_SET_MICSEL_ONOFF:
		mutex_lock(&a2220->lock);
			rc = chk_wakeup_a2220(a2220);
		if (rc < 0)
			goto handle_error;
		if (copy_from_user(&mic_sel, argp, sizeof(mic_sel))) {
			rc = -EFAULT;
			goto handle_error;
		}
		mutex_unlock(&a2220->lock);
	break;
	case A2220_READ_DATA:
		mutex_lock(&a2220->lock);
		rc = chk_wakeup_a2220(a2220);
		if (rc < 0)
			goto handle_error;
		rc = a2220_i2c_read(a2220, msg, 4);
		if (copy_to_user(argp, &msg, 4)) {
			rc = -EFAULT;
			goto handle_error;
		}
		mutex_unlock(&a2220->lock);
		break;
	case A2220_WRITE_MSG:
		mutex_lock(&a2220->lock);
		rc = chk_wakeup_a2220(a2220);
		if (rc < 0)
			goto handle_error;
		if (copy_from_user(msg, argp, sizeof(msg))) {
				rc = -EFAULT;
			goto handle_error;
		}
		rc = a2220_i2c_write(a2220, msg, 4);
		mutex_unlock(&a2220->lock);
		break;
	case A2220_SYNC_CMD:
		mutex_lock(&a2220->lock);
		rc = chk_wakeup_a2220(a2220);
		if (rc < 0)
			goto handle_error;
		msg[0] = 0x80;
		msg[1] = 0x00;
		msg[2] = 0x00;
			msg[3] = 0x00;
		rc = a2220_i2c_write(a2220, msg, 4);
		mutex_unlock(&a2220->lock);
		break;
	case A2220_SET_CMD_FILE:
		mutex_lock(&a2220->lock);
		rc = chk_wakeup_a2220(a2220);
		if (rc < 0)
			goto handle_error;
		if (copy_from_user(msg, argp, sizeof(msg))) {
			rc = -EFAULT;
			goto handle_error;
		}
			rc = exe_cmd_in_file(msg);
		mutex_unlock(&a2220->lock);
		break;
#endif /* ENABLE_DIAG_IOCTLS */
	default:
		pr_err("%s: invalid command %d\n", __func__, _IOC_NR(cmd));
		rc = -EINVAL;
		goto handle_error;
	}
	xoclk_control(false);
	return rc;
handle_error:
	mutex_unlock(&a2220->lock);
	pr_err("%s rc[%d]\n", __func__, rc);
	return rc;
}

static const struct file_operations a2220_fops = {
	.owner = THIS_MODULE,
	.open = a2220_open,
	.release = a2220_release,
	.unlocked_ioctl = a2220_ioctl,
};

static int a2220_probe(
		struct i2c_client *client, const struct i2c_device_id *id)
{
	int rc = 0;
	struct a2220_data *a2220;
	static struct a2220_platform_data *pdata;

	pr_info("%s\n", __func__);

	pdata = client->dev.platform_data;
	if (pdata == NULL) {
		pr_err("%s: platform data is NULL\n", __func__);
		goto err_alloc_data_failed;
	}

	if (!pdata->a2220_hw_init) {
		pr_err("%s: a2220_hw_init is NULL\n", __func__);
		goto err_alloc_data_failed;
	}

	rc = pdata->a2220_hw_init();
	if (rc < 0)
		goto err_alloc_data_failed;

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		pr_err("%s: i2c check functionality error\n", __func__);
		rc = -ENODEV;
		goto err_alloc_data_failed;
	}

	a2220 = kzalloc(sizeof(*a2220), GFP_KERNEL);
	if (!a2220) {
		pr_err("failed to allocate memory for module data\n");
		rc = -ENOMEM;
		goto err_alloc_data_failed;
	}

	a2220->pdata = client->dev.platform_data;
	mutex_init(&a2220->lock);

	i2c_set_clientdata(client, a2220);
	a2220->this_client = client;

	a2220->device.minor = MISC_DYNAMIC_MINOR;
	a2220->device.name = "audience_a2220";
	a2220->device.fops = &a2220_fops;

	rc = misc_register(&a2220->device);
	if (rc) {
		pr_err("%s: a2220_device register failed\n", __func__);
		goto err_mem_alloc_failed;
	}

	atomic_set(&a2220->opened, 1);
	xo = msm_xo_get(MSM_XO_TCXO_D0, "audio_driver");
	if (!xo) {
		pr_err("please check the xo driver,something is wrong!!");
		rc = -EAGAIN;
		goto err_misc_register_failed;
	}
	kthread_run(thread_start, a2220, "thread_start");
	pr_info("a2220_probe - finish\n");
	return 0;

err_misc_register_failed:
	misc_deregister(&a2220->device);
err_mem_alloc_failed:
	mutex_destroy(&a2220->lock);
	kfree(a2220);
err_alloc_data_failed:
	pr_err("a2220_probe - failed!!!\n");
	return rc;
}

static int a2220_remove(struct i2c_client *client)
{
	struct a2220_data *a2220 = i2c_get_clientdata(client);
	misc_deregister(&a2220->device);
	mutex_destroy(&a2220->lock);
	if (xo) {
		msm_xo_put(xo);
		xo = NULL;
	}
	kfree(a2220);
	return 0;
}

static int a2220_suspend(struct i2c_client *client, pm_message_t mesg)
{
	return 0;
}

static int a2220_resume(struct i2c_client *client)
{
	return 0;
}

static const struct i2c_device_id a2220_id[] = {
	{ "audience_a2220", 0 },
	{ }
};

static struct i2c_driver a2220_driver = {
	.probe = a2220_probe,
	.remove = a2220_remove,
	.suspend = a2220_suspend,
	.resume	= a2220_resume,
	.id_table = a2220_id,
	.driver = {
		.name = "audience_a2220",
	},
};

static int __init a2220_init(void)
{
	return i2c_add_driver(&a2220_driver);
}

static void __exit a2220_exit(void)
{
	i2c_del_driver(&a2220_driver);
}

module_init(a2220_init);
module_exit(a2220_exit);

MODULE_DESCRIPTION("A2220 voice processor driver");
MODULE_LICENSE("GPL");
