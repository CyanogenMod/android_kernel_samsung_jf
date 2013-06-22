/* Copyright (c) 2012, The Linux Foundation. All rights reserved.
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

#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of.h>
#include <mach/board.h>
#include <mach/camera.h>
#include <media/msm_isp.h>
#include "msm_cci.h"
#include "msm.h"
#include "msm_cam_server.h"
#include "msm_cam_cci_hwreg.h"

#define V4L2_IDENT_CCI 50005
#define CCI_I2C_QUEUE_0_SIZE 64
#define CCI_I2C_QUEUE_1_SIZE 16

#define CCI_TIMEOUT msecs_to_jiffies(100)

static void msm_cci_set_clk_param(struct cci_device *cci_dev)
{
	uint16_t THIGH = 78;
	uint16_t TLOW = 114;
	uint16_t TSUSTO = 28;
	uint16_t TSUSTA = 28;
	uint16_t THDDAT = 10;
	uint16_t THDSTA = 77;
	uint16_t TBUF = 118;
	uint8_t HW_SCL_STRETCH_EN = 0; /*enable or disable SCL clock
					* stretching */
	uint8_t HW_RDHLD = 6; /* internal hold time 1-6 cycles of SDA to bridge
			       * undefined falling SCL region */
	uint8_t HW_TSP = 1; /* glitch filter 1-3 cycles */

	msm_camera_io_w(THIGH << 16 | TLOW, cci_dev->base +
		CCI_I2C_M0_SCL_CTL_ADDR);
	msm_camera_io_w(TSUSTO << 16 | TSUSTA, cci_dev->base +
		CCI_I2C_M0_SDA_CTL_0_ADDR);
	msm_camera_io_w(THDDAT << 16 | THDSTA, cci_dev->base +
		CCI_I2C_M0_SDA_CTL_1_ADDR);
	msm_camera_io_w(TBUF, cci_dev->base +
		CCI_I2C_M0_SDA_CTL_2_ADDR);
	msm_camera_io_w(HW_SCL_STRETCH_EN << 8 | HW_RDHLD << 4 | HW_TSP,
		cci_dev->base + CCI_I2C_M0_MISC_CTL_ADDR);
	msm_camera_io_w(THIGH << 16 | TLOW, cci_dev->base +
		CCI_I2C_M1_SCL_CTL_ADDR);
	msm_camera_io_w(TSUSTO << 16 | TSUSTA, cci_dev->base +
		CCI_I2C_M1_SDA_CTL_0_ADDR);
	msm_camera_io_w(THDDAT << 16 | THDSTA, cci_dev->base +
		CCI_I2C_M1_SDA_CTL_1_ADDR);
	msm_camera_io_w(TBUF, cci_dev->base + CCI_I2C_M1_SDA_CTL_2_ADDR);
	msm_camera_io_w(HW_SCL_STRETCH_EN << 8 | HW_RDHLD << 4 | HW_TSP,
		cci_dev->base + CCI_I2C_M1_MISC_CTL_ADDR);
}

static int32_t msm_cci_i2c_config_sync_timer(struct v4l2_subdev *sd,
	struct msm_camera_cci_ctrl *c_ctrl)
{
	struct cci_device *cci_dev;
	cci_dev = v4l2_get_subdevdata(sd);
	msm_camera_io_w(c_ctrl->cci_info->cid, cci_dev->base +
		CCI_SET_CID_SYNC_TIMER_0_ADDR + (c_ctrl->cci_info->cid * 0x4));
	return 0;
}

static int32_t msm_cci_i2c_set_freq(struct v4l2_subdev *sd,
	struct msm_camera_cci_ctrl *c_ctrl)
{
	struct cci_device *cci_dev;
	uint32_t val;
	cci_dev = v4l2_get_subdevdata(sd);
	val = c_ctrl->cci_info->freq;
	msm_camera_io_w(val, cci_dev->base + CCI_I2C_M0_SCL_CTL_ADDR +
		c_ctrl->cci_info->cci_i2c_master*0x100);
	msm_camera_io_w(val, cci_dev->base + CCI_I2C_M0_SDA_CTL_0_ADDR +
		c_ctrl->cci_info->cci_i2c_master*0x100);
	msm_camera_io_w(val, cci_dev->base + CCI_I2C_M0_SDA_CTL_1_ADDR +
		c_ctrl->cci_info->cci_i2c_master*0x100);
	msm_camera_io_w(val, cci_dev->base + CCI_I2C_M0_SDA_CTL_2_ADDR +
		c_ctrl->cci_info->cci_i2c_master*0x100);
	msm_camera_io_w(val, cci_dev->base + CCI_I2C_M0_MISC_CTL_ADDR +
		c_ctrl->cci_info->cci_i2c_master*0x100);
	return 0;
}

static int32_t msm_cci_validate_queue(struct cci_device *cci_dev,
	uint32_t len,
	enum cci_i2c_master_t master,
	enum cci_i2c_queue_t queue)
{
	int32_t rc = 0;
	uint32_t read_val = 0;
	uint32_t reg_offset = master * 0x200 + queue * 0x100;
	read_val = msm_camera_io_r(cci_dev->base +
		CCI_I2C_M0_Q0_CUR_WORD_CNT_ADDR + reg_offset);
	CDBG("%s line %d CCI_I2C_M0_Q0_CUR_WORD_CNT_ADDR %d len %d max %d\n",
		__func__, __LINE__, read_val, len,
		cci_dev->cci_i2c_queue_info[master][queue].max_queue_size);
	if ((read_val + len + 1) > cci_dev->
		cci_i2c_queue_info[master][queue].max_queue_size) {
		uint32_t reg_val = 0;
		uint32_t report_val = CCI_I2C_REPORT_CMD | (1 << 8);
		CDBG("%s:%d CCI_I2C_REPORT_CMD\n", __func__, __LINE__);
		msm_camera_io_w(report_val,
			cci_dev->base + CCI_I2C_M0_Q0_LOAD_DATA_ADDR +
			reg_offset);
		read_val++;
		CDBG("%s:%d CCI_I2C_M0_Q0_EXEC_WORD_CNT_ADDR %d\n",
			__func__, __LINE__, read_val);
		msm_camera_io_w(read_val, cci_dev->base +
			CCI_I2C_M0_Q0_EXEC_WORD_CNT_ADDR + reg_offset);
		reg_val = 1 << ((master * 2) + queue);
		CDBG("%s:%d CCI_QUEUE_START_ADDR\n", __func__, __LINE__);
		msm_camera_io_w(reg_val, cci_dev->base + CCI_QUEUE_START_ADDR);
		CDBG("%s line %d wait_for_completion_interruptible\n",
			__func__, __LINE__);
		wait_for_completion_interruptible_timeout(&cci_dev->
			cci_master_info[master].reset_complete, CCI_TIMEOUT);

		rc = cci_dev->cci_master_info[master].status;
		if (rc < 0)
			pr_err("%s failed rc %d\n", __func__, rc);
	}
	return rc;
}

static int32_t msm_cci_data_queue(struct cci_device *cci_dev,
	struct msm_camera_cci_ctrl *c_ctrl, enum cci_i2c_queue_t queue)
{
	uint16_t i = 0, j = 0, k = 0, h = 0, len = 0;
	uint32_t cmd = 0;
	uint8_t data[10];
	uint16_t reg_addr = 0;
	struct msm_camera_cci_i2c_write_cfg *i2c_msg =
		&c_ctrl->cfg.cci_i2c_write_cfg;
	uint16_t cmd_size = i2c_msg->size;
	struct msm_camera_i2c_reg_conf *i2c_cmd = i2c_msg->reg_conf_tbl;
	enum cci_i2c_master_t master = c_ctrl->cci_info->cci_i2c_master;
	CDBG("%s addr type %d data type %d\n", __func__,
		i2c_msg->addr_type, i2c_msg->data_type);
	/* assume total size within the max queue */
	while (cmd_size) {
		CDBG("%s cmd_size %d addr 0x%x data 0x%x", __func__,
			cmd_size, i2c_cmd->reg_addr, i2c_cmd->reg_data);
		data[i++] = CCI_I2C_WRITE_CMD;
		if (i2c_cmd->reg_addr)
			reg_addr = i2c_cmd->reg_addr;
		/* either byte or word addr */
		if (i2c_msg->addr_type == MSM_CAMERA_I2C_BYTE_ADDR)
			data[i++] = reg_addr;
		else {
			data[i++] = (reg_addr & 0xFF00) >> 8;
			data[i++] = reg_addr & 0x00FF;
		}
		/* max of 10 data bytes */
		do {
			if (i2c_msg->data_type == MSM_CAMERA_I2C_BYTE_DATA) {
				data[i++] = i2c_cmd->reg_data;
				reg_addr++;
			} else {
				if ((i + 1) <= 10) {
					data[i++] = (i2c_cmd->reg_data &
						0xFF00) >> 8; /* MSB */
					data[i++] = i2c_cmd->reg_data &
						0x00FF; /* LSB */
					reg_addr += 2;
				} else
					break;
			}
			i2c_cmd++;
		} while (--cmd_size && !i2c_cmd->reg_addr && (i <= 10));
		data[0] |= ((i-1) << 4);
		len = ((i-1)/4) + 1;
		msm_cci_validate_queue(cci_dev, len, master, queue);
		for (h = 0, k = 0; h < len; h++) {
			cmd = 0;
			for (j = 0; (j < 4 && k < i); j++)
				cmd |= (data[k++] << (j * 8));
			CDBG("%s CCI_I2C_M0_Q0_LOAD_DATA_ADDR 0x%x\n",
				__func__, cmd);
			msm_camera_io_w(cmd, cci_dev->base +
				CCI_I2C_M0_Q0_LOAD_DATA_ADDR +
				master * 0x200 + queue * 0x100);
		}
		i = 0;
	}
	return 0;
}

static int32_t msm_cci_write_i2c_queue(struct cci_device *cci_dev,
	uint32_t val,
	enum cci_i2c_master_t master,
	enum cci_i2c_queue_t queue)
{
	int32_t rc = 0;
	uint32_t reg_offset = master * 0x200 + queue * 0x100;
	CDBG("%s:%d called\n", __func__, __LINE__);
	msm_cci_validate_queue(cci_dev, 1, master, queue);
	CDBG("%s CCI_I2C_M0_Q0_LOAD_DATA_ADDR:val %x:%x\n",
		__func__, CCI_I2C_M0_Q0_LOAD_DATA_ADDR +
		reg_offset, val);
	msm_camera_io_w(val, cci_dev->base + CCI_I2C_M0_Q0_LOAD_DATA_ADDR +
		reg_offset);
	return rc;
}

static int32_t msm_cci_i2c_read(struct v4l2_subdev *sd,
	struct msm_camera_cci_ctrl *c_ctrl)
{
	uint32_t rc = 0;
	uint32_t val = 0;
	int32_t read_words = 0, exp_words = 0;
	int32_t index = 0, first_byte = 0;
	uint32_t i = 0;
	enum cci_i2c_master_t master;
	enum cci_i2c_queue_t queue = QUEUE_1;
	struct cci_device *cci_dev = NULL;
	struct msm_camera_cci_i2c_read_cfg *read_cfg = NULL;
	CDBG("%s line %d\n", __func__, __LINE__);
	cci_dev = v4l2_get_subdevdata(sd);
	master = c_ctrl->cci_info->cci_i2c_master;
	read_cfg = &c_ctrl->cfg.cci_i2c_read_cfg;
	mutex_lock(&cci_dev->cci_master_info[master].mutex);
	CDBG("%s master %d, queue %d\n", __func__, master, queue);
	CDBG("%s set param sid 0x%x retries %d id_map %d\n", __func__,
		c_ctrl->cci_info->sid, c_ctrl->cci_info->retries,
		c_ctrl->cci_info->id_map);
	val = CCI_I2C_SET_PARAM_CMD | c_ctrl->cci_info->sid << 4 |
		c_ctrl->cci_info->retries << 16 |
		c_ctrl->cci_info->id_map << 18;
	rc = msm_cci_write_i2c_queue(cci_dev, val, master, queue);
	if (rc < 0) {
		CDBG("%s failed line %d\n", __func__, __LINE__);
		goto ERROR;
	}

	val = CCI_I2C_LOCK_CMD;
	rc = msm_cci_write_i2c_queue(cci_dev, val, master, queue);
	if (rc < 0) {
		CDBG("%s failed line %d\n", __func__, __LINE__);
		goto ERROR;
	}

	if (read_cfg->addr_type == MSM_CAMERA_I2C_BYTE_ADDR)
		val = CCI_I2C_WRITE_CMD | (read_cfg->addr_type << 4) |
			((read_cfg->addr & 0xFF) << 8);
	if (read_cfg->addr_type == MSM_CAMERA_I2C_WORD_ADDR)
		val = CCI_I2C_WRITE_CMD | (read_cfg->addr_type << 4) |
			(((read_cfg->addr & 0xFF00) >> 8) << 8) |
			((read_cfg->addr & 0xFF) << 16);
	rc = msm_cci_write_i2c_queue(cci_dev, val, master, queue);
	if (rc < 0) {
		CDBG("%s failed line %d\n", __func__, __LINE__);
		goto ERROR;
	}

	val = CCI_I2C_READ_CMD | (read_cfg->num_byte << 4);
	rc = msm_cci_write_i2c_queue(cci_dev, val, master, queue);
	if (rc < 0) {
		CDBG("%s failed line %d\n", __func__, __LINE__);
		goto ERROR;
	}

	val = CCI_I2C_UNLOCK_CMD;
	rc = msm_cci_write_i2c_queue(cci_dev, val, master, queue);
	if (rc < 0) {
		CDBG("%s failed line %d\n", __func__, __LINE__);
		goto ERROR;
	}

	val = msm_camera_io_r(cci_dev->base + CCI_I2C_M0_Q0_CUR_WORD_CNT_ADDR +
		master * 0x200 + queue * 0x100);
	CDBG("%s cur word cnt %x\n", __func__, val);
	msm_camera_io_w(val, cci_dev->base + CCI_I2C_M0_Q0_EXEC_WORD_CNT_ADDR +
		master * 0x200 + queue * 0x100);

	val = 1 << ((master * 2) + queue);
	msm_camera_io_w(val, cci_dev->base + CCI_QUEUE_START_ADDR);
	wait_for_completion_interruptible_timeout(&cci_dev->
		cci_master_info[master].reset_complete, CCI_TIMEOUT);

	read_words = msm_camera_io_r(cci_dev->base +
		CCI_I2C_M0_READ_BUF_LEVEL_ADDR + master * 0x100);
	exp_words = ((read_cfg->num_byte / 4) + 1);
	if (read_words != exp_words) {
		pr_err("%s:%d read_words = %d, exp words = %d\n", __func__,
			__LINE__, read_words, exp_words);
		memset(read_cfg->data, 0, read_cfg->num_byte);
		goto ERROR;
	}
	index = 0;
	CDBG("%s index %d num_type %d\n", __func__, index,
		read_cfg->num_byte);
	first_byte = 0;
	do {
		val = msm_camera_io_r(cci_dev->base +
			CCI_I2C_M0_READ_DATA_ADDR + master * 0x100);
		CDBG("%s read val %x\n", __func__, val);
		for (i = 0; (i < 4) && (index < read_cfg->num_byte); i++) {
			CDBG("%s i %d index %d\n", __func__, i, index);
			if (!first_byte) {
				CDBG("%s sid %x\n", __func__, val & 0xFF);
				first_byte++;
			} else {
				read_cfg->data[index] =
					(val  >> (i * 8)) & 0xFF;
				CDBG("%s data[%d] %x\n", __func__, index,
					read_cfg->data[index]);
				index++;
			}
		}
	} while (--read_words > 0);
ERROR:
	mutex_unlock(&cci_dev->cci_master_info[master].mutex);
	return rc;
}

static int32_t msm_cci_i2c_write(struct v4l2_subdev *sd,
	struct msm_camera_cci_ctrl *c_ctrl)
{
	int32_t rc = 0;
	struct cci_device *cci_dev;
	uint32_t val;
	enum cci_i2c_master_t master;
	enum cci_i2c_queue_t queue = QUEUE_0;
	cci_dev = v4l2_get_subdevdata(sd);
	master = c_ctrl->cci_info->cci_i2c_master;
	CDBG("%s master %d, queue %d\n", __func__, master, queue);
	CDBG("%s set param sid 0x%x retries %d id_map %d\n", __func__,
		c_ctrl->cci_info->sid, c_ctrl->cci_info->retries,
		c_ctrl->cci_info->id_map);
	mutex_lock(&cci_dev->cci_master_info[master].mutex);
	val = CCI_I2C_SET_PARAM_CMD | c_ctrl->cci_info->sid << 4 |
		c_ctrl->cci_info->retries << 16 |
		c_ctrl->cci_info->id_map << 18;
	CDBG("%s:%d CCI_I2C_SET_PARAM_CMD\n", __func__, __LINE__);
	rc = msm_cci_write_i2c_queue(cci_dev, val, master, queue);
	if (rc < 0) {
		CDBG("%s failed line %d\n", __func__, __LINE__);
		goto ERROR;
	}

	val = CCI_I2C_LOCK_CMD;
	CDBG("%s:%d CCI_I2C_LOCK_CMD\n", __func__, __LINE__);
	rc = msm_cci_write_i2c_queue(cci_dev, val, master, queue);
	if (rc < 0) {
		CDBG("%s failed line %d\n", __func__, __LINE__);
		goto ERROR;
	}

	msm_cci_data_queue(cci_dev, c_ctrl, queue);
	val = CCI_I2C_UNLOCK_CMD;
	CDBG("%s:%d CCI_I2C_UNLOCK_CMD\n", __func__, __LINE__);
	rc = msm_cci_write_i2c_queue(cci_dev, val, master, queue);
	if (rc < 0) {
		CDBG("%s failed line %d\n", __func__, __LINE__);
		goto ERROR;
	}

	val = CCI_I2C_REPORT_CMD | (1 << 8);
	CDBG("%s:%d CCI_I2C_REPORT_CMD\n", __func__, __LINE__);
	rc = msm_cci_write_i2c_queue(cci_dev, val, master, queue);
	if (rc < 0) {
		CDBG("%s failed line %d\n", __func__, __LINE__);
		goto ERROR;
	}

	val = msm_camera_io_r(cci_dev->base + CCI_I2C_M0_Q0_CUR_WORD_CNT_ADDR +
		master * 0x200 + queue * 0x100);
	CDBG("%s:%d cur word count %d\n", __func__, __LINE__, val);
	CDBG("%s:%d CCI_I2C_M0_Q0_EXEC_WORD_CNT_ADDR\n", __func__, __LINE__);
	msm_camera_io_w(val, cci_dev->base + CCI_I2C_M0_Q0_EXEC_WORD_CNT_ADDR +
		master * 0x200 + queue * 0x100);

	val = 1 << ((master * 2) + queue);
	CDBG("%s:%d CCI_QUEUE_START_ADDR\n", __func__, __LINE__);
	msm_camera_io_w(val, cci_dev->base + CCI_QUEUE_START_ADDR +
		master*0x200 + queue * 0x100);

	CDBG("%s line %d wait_for_completion_interruptible\n",
		__func__, __LINE__);
	wait_for_completion_interruptible_timeout(&cci_dev->
		cci_master_info[master].reset_complete, CCI_TIMEOUT);

ERROR:
	mutex_unlock(&cci_dev->cci_master_info[master].mutex);
	return rc;
}

static int msm_cci_subdev_g_chip_ident(struct v4l2_subdev *sd,
			struct v4l2_dbg_chip_ident *chip)
{
	BUG_ON(!chip);
	chip->ident = V4L2_IDENT_CCI;
	chip->revision = 0;
	return 0;
}

static struct msm_cam_clk_info cci_clk_info[] = {
	{"camss_top_ahb_clk", -1},
	{"cci_src_clk", 19200000},
	{"cci_ahb_clk", -1},
	{"cci_clk", -1},
};

static int32_t msm_cci_init(struct v4l2_subdev *sd)
{
	int rc = 0;
	struct cci_device *cci_dev;
	cci_dev = v4l2_get_subdevdata(sd);
	CDBG("%s line %d\n", __func__, __LINE__);
	if (cci_dev == NULL) {
		rc = -ENOMEM;
		return rc;
	}
	rc = msm_cam_clk_enable(&cci_dev->pdev->dev, cci_clk_info,
		cci_dev->cci_clk, ARRAY_SIZE(cci_clk_info), 1);
	if (rc < 0) {
		CDBG("%s: clk enable failed\n", __func__);
		goto clk_enable_failed;
	}

	enable_irq(cci_dev->irq->start);
	cci_dev->hw_version = msm_camera_io_r(cci_dev->base +
		CCI_HW_VERSION_ADDR);
	cci_dev->cci_master_info[MASTER_0].reset_pending = TRUE;
	msm_camera_io_w(0xFFFFFFFF, cci_dev->base + CCI_RESET_CMD_ADDR);
	msm_camera_io_w(0x1, cci_dev->base + CCI_RESET_CMD_ADDR);
	wait_for_completion_interruptible_timeout(
		&cci_dev->cci_master_info[MASTER_0].reset_complete,
		CCI_TIMEOUT);
	msm_cci_set_clk_param(cci_dev);
	msm_camera_io_w(0xFFFFFFFF, cci_dev->base + CCI_IRQ_MASK_0_ADDR);
	msm_camera_io_w(0xFFFFFFFF, cci_dev->base + CCI_IRQ_CLEAR_0_ADDR);
	msm_camera_io_w(0x1, cci_dev->base + CCI_IRQ_GLOBAL_CLEAR_CMD_ADDR);
	msm_camera_io_w(0x0, cci_dev->base + CCI_IRQ_GLOBAL_CLEAR_CMD_ADDR);
	msm_camera_io_w(0x0, cci_dev->base + CCI_IRQ_CLEAR_0_ADDR);
	return 0;

clk_enable_failed:
	return rc;
}

static int32_t msm_cci_release(struct v4l2_subdev *sd)
{
	struct cci_device *cci_dev;
	cci_dev = v4l2_get_subdevdata(sd);

	disable_irq(cci_dev->irq->start);

	msm_cam_clk_enable(&cci_dev->pdev->dev, cci_clk_info,
		cci_dev->cci_clk, ARRAY_SIZE(cci_clk_info), 0);

	return 0;
}

static int32_t msm_cci_config(struct v4l2_subdev *sd,
	struct msm_camera_cci_ctrl *cci_ctrl)
{
	int32_t rc = 0;
	CDBG("%s line %d cmd %d\n", __func__, __LINE__,
		cci_ctrl->cmd);
	switch (cci_ctrl->cmd) {
	case MSM_CCI_INIT:
		rc = msm_cci_init(sd);
		break;
	case MSM_CCI_RELEASE:
		rc = msm_cci_release(sd);
		break;
	case MSM_CCI_SET_SID:
		break;
	case MSM_CCI_SET_FREQ:
		rc = msm_cci_i2c_set_freq(sd, cci_ctrl);
		break;
	case MSM_CCI_SET_SYNC_CID:
		rc = msm_cci_i2c_config_sync_timer(sd, cci_ctrl);
		break;
	case MSM_CCI_I2C_READ:
		rc = msm_cci_i2c_read(sd, cci_ctrl);
		break;
	case MSM_CCI_I2C_WRITE:
		rc = msm_cci_i2c_write(sd, cci_ctrl);
		break;
	case MSM_CCI_GPIO_WRITE:
		break;
	default:
		rc = -ENOIOCTLCMD;
	}
	CDBG("%s line %d rc %d\n", __func__, __LINE__, rc);
	cci_ctrl->status = rc;
	return rc;
}

static irqreturn_t msm_cci_irq(int irq_num, void *data)
{
	uint32_t irq;
	struct cci_device *cci_dev = data;
	irq = msm_camera_io_r(cci_dev->base + CCI_IRQ_STATUS_0_ADDR);
	msm_camera_io_w(irq, cci_dev->base + CCI_IRQ_CLEAR_0_ADDR);
	msm_camera_io_w(0x1, cci_dev->base + CCI_IRQ_GLOBAL_CLEAR_CMD_ADDR);
	msm_camera_io_w(0x0, cci_dev->base + CCI_IRQ_GLOBAL_CLEAR_CMD_ADDR);
	CDBG("%s CCI_I2C_M0_STATUS_ADDR = 0x%x\n", __func__, irq);
	if (irq & CCI_IRQ_STATUS_0_RST_DONE_ACK_BMSK) {
		if (cci_dev->cci_master_info[MASTER_0].reset_pending == TRUE) {
			cci_dev->cci_master_info[MASTER_0].reset_pending =
				FALSE;
			complete(&cci_dev->cci_master_info[MASTER_0].
				reset_complete);
		}
		if (cci_dev->cci_master_info[MASTER_1].reset_pending == TRUE) {
			cci_dev->cci_master_info[MASTER_1].reset_pending =
				FALSE;
			complete(&cci_dev->cci_master_info[MASTER_1].
				reset_complete);
		}
	} else if ((irq & CCI_IRQ_STATUS_0_I2C_M0_RD_DONE_BMSK) ||
		(irq & CCI_IRQ_STATUS_0_I2C_M0_Q0_REPORT_BMSK) ||
		(irq & CCI_IRQ_STATUS_0_I2C_M0_Q1_REPORT_BMSK)) {
		cci_dev->cci_master_info[MASTER_0].status = 0;
		complete(&cci_dev->cci_master_info[MASTER_0].reset_complete);
	} else if ((irq & CCI_IRQ_STATUS_0_I2C_M1_RD_DONE_BMSK) ||
		(irq & CCI_IRQ_STATUS_0_I2C_M1_Q0_REPORT_BMSK) ||
		(irq & CCI_IRQ_STATUS_0_I2C_M1_Q1_REPORT_BMSK)) {
		cci_dev->cci_master_info[MASTER_1].status = 0;
		complete(&cci_dev->cci_master_info[MASTER_1].reset_complete);
	} else if ((irq & CCI_IRQ_STATUS_0_I2C_M0_Q0_NACK_ERR_BMSK) ||
		(irq & CCI_IRQ_STATUS_0_I2C_M0_Q1_NACK_ERR_BMSK)) {
		cci_dev->cci_master_info[MASTER_0].status = -EINVAL;
		msm_camera_io_w(CCI_M0_HALT_REQ_RMSK,
			cci_dev->base + CCI_HALT_REQ_ADDR);
	} else if ((irq & CCI_IRQ_STATUS_0_I2C_M1_Q0_NACK_ERR_BMSK) ||
		(irq & CCI_IRQ_STATUS_0_I2C_M1_Q1_NACK_ERR_BMSK)) {
		cci_dev->cci_master_info[MASTER_1].status = -EINVAL;
		msm_camera_io_w(CCI_M1_HALT_REQ_RMSK,
			cci_dev->base + CCI_HALT_REQ_ADDR);
	} else if (irq & CCI_IRQ_STATUS_0_I2C_M0_Q0Q1_HALT_ACK_BMSK) {
		cci_dev->cci_master_info[MASTER_0].reset_pending = TRUE;
		msm_camera_io_w(CCI_M0_RESET_RMSK,
			cci_dev->base + CCI_RESET_CMD_ADDR);
	} else if (irq & CCI_IRQ_STATUS_0_I2C_M1_Q0Q1_HALT_ACK_BMSK) {
		cci_dev->cci_master_info[MASTER_1].reset_pending = TRUE;
		msm_camera_io_w(CCI_M1_RESET_RMSK,
			cci_dev->base + CCI_RESET_CMD_ADDR);
	} else {
		pr_err("%s unhandled irq 0x%x\n", __func__, irq);
		cci_dev->cci_master_info[MASTER_0].status = 0;
		complete(&cci_dev->cci_master_info[MASTER_0].reset_complete);
		cci_dev->cci_master_info[MASTER_1].status = 0;
		complete(&cci_dev->cci_master_info[MASTER_1].reset_complete);
	}
	return IRQ_HANDLED;
}

int msm_cci_irq_routine(struct v4l2_subdev *sd, u32 status, bool *handled)
{
	struct cci_device *cci_dev = v4l2_get_subdevdata(sd);
	irqreturn_t ret;
	CDBG("%s line %d\n", __func__, __LINE__);
	ret = msm_cci_irq(cci_dev->irq->start, cci_dev);
	*handled = TRUE;
	return 0;
}

static long msm_cci_subdev_ioctl(struct v4l2_subdev *sd,
	unsigned int cmd, void *arg)
{
	int32_t rc = 0;
	CDBG("%s line %d\n", __func__, __LINE__);
	switch (cmd) {
	case VIDIOC_MSM_CCI_CFG:
		rc = msm_cci_config(sd, arg);
		break;
	default:
		rc = -ENOIOCTLCMD;
	}
	CDBG("%s line %d rc %d\n", __func__, __LINE__, rc);
	return rc;
}

static struct v4l2_subdev_core_ops msm_cci_subdev_core_ops = {
	.g_chip_ident = &msm_cci_subdev_g_chip_ident,
	.ioctl = &msm_cci_subdev_ioctl,
	.interrupt_service_routine = msm_cci_irq_routine,
};

static const struct v4l2_subdev_ops msm_cci_subdev_ops = {
	.core = &msm_cci_subdev_core_ops,
};

static const struct v4l2_subdev_internal_ops msm_cci_internal_ops;

static void msm_cci_initialize_cci_params(struct cci_device *new_cci_dev)
{
	uint8_t i = 0, j = 0;
	for (i = 0; i < NUM_MASTERS; i++) {
		new_cci_dev->cci_master_info[i].status = 0;
		mutex_init(&new_cci_dev->cci_master_info[i].mutex);
		init_completion(&new_cci_dev->
			cci_master_info[i].reset_complete);
		for (j = 0; j < NUM_QUEUES; j++) {
			if (j == QUEUE_0)
				new_cci_dev->cci_i2c_queue_info[i][j].
					max_queue_size = CCI_I2C_QUEUE_0_SIZE;
			else
				new_cci_dev->cci_i2c_queue_info[i][j].
					max_queue_size = CCI_I2C_QUEUE_1_SIZE;
			}
	}
	return;
}

static int __devinit msm_cci_probe(struct platform_device *pdev)
{
	struct cci_device *new_cci_dev;
	int rc = 0;
	struct msm_cam_subdev_info sd_info;
	struct intr_table_entry irq_req;
	CDBG("%s: pdev %p device id = %d\n", __func__, pdev, pdev->id);
	new_cci_dev = kzalloc(sizeof(struct cci_device), GFP_KERNEL);
	if (!new_cci_dev) {
		CDBG("%s: no enough memory\n", __func__);
		return -ENOMEM;
	}
	v4l2_subdev_init(&new_cci_dev->subdev, &msm_cci_subdev_ops);
	new_cci_dev->subdev.internal_ops = &msm_cci_internal_ops;
	new_cci_dev->subdev.flags |= V4L2_SUBDEV_FL_HAS_DEVNODE;
	snprintf(new_cci_dev->subdev.name,
			ARRAY_SIZE(new_cci_dev->subdev.name), "msm_cci");
	v4l2_set_subdevdata(&new_cci_dev->subdev, new_cci_dev);
	platform_set_drvdata(pdev, &new_cci_dev->subdev);
	CDBG("%s sd %p\n", __func__, &new_cci_dev->subdev);
	if (pdev->dev.of_node)
		of_property_read_u32((&pdev->dev)->of_node,
			"cell-index", &pdev->id);

	new_cci_dev->mem = platform_get_resource_byname(pdev,
					IORESOURCE_MEM, "cci");
	if (!new_cci_dev->mem) {
		CDBG("%s: no mem resource?\n", __func__);
		rc = -ENODEV;
		goto cci_no_resource;
	}
	new_cci_dev->irq = platform_get_resource_byname(pdev,
					IORESOURCE_IRQ, "cci");
	CDBG("%s line %d cci irq start %d end %d\n", __func__,
		__LINE__,
		new_cci_dev->irq->start,
		new_cci_dev->irq->end);
	if (!new_cci_dev->irq) {
		CDBG("%s: no irq resource?\n", __func__);
		rc = -ENODEV;
		goto cci_no_resource;
	}
	new_cci_dev->io = request_mem_region(new_cci_dev->mem->start,
		resource_size(new_cci_dev->mem), pdev->name);
	if (!new_cci_dev->io) {
		CDBG("%s: no valid mem region\n", __func__);
		rc = -EBUSY;
		goto cci_no_resource;
	}

	new_cci_dev->base = ioremap(new_cci_dev->mem->start,
		resource_size(new_cci_dev->mem));
	if (!new_cci_dev->base) {
		rc = -ENOMEM;
		goto cci_release_mem;
	}

	sd_info.sdev_type = CCI_DEV;
	sd_info.sd_index = pdev->id;
	sd_info.irq_num = new_cci_dev->irq->start;
	msm_cam_register_subdev_node(&new_cci_dev->subdev, &sd_info);

	irq_req.cam_hw_idx       = MSM_CAM_HW_CCI;
	irq_req.dev_name         = "msm_cci";
	irq_req.irq_idx          = CAMERA_SS_IRQ_1;
	irq_req.irq_num          = new_cci_dev->irq->start;
	irq_req.is_composite     = 0;
	irq_req.irq_trigger_type = IRQF_TRIGGER_RISING;
	irq_req.num_hwcore       = 1;
	irq_req.subdev_list[0]   = &new_cci_dev->subdev;
	irq_req.data             = (void *)new_cci_dev;
	rc = msm_cam_server_request_irq(&irq_req);
	if (rc == -ENXIO) {
		/* IRQ Router hardware is not present on this hardware.
		 * Request for the IRQ and register the interrupt handler. */
		rc = request_irq(new_cci_dev->irq->start, msm_cci_irq,
			IRQF_TRIGGER_RISING, "cci", new_cci_dev);
		if (rc < 0) {
			CDBG("%s: irq request fail\n", __func__);
			rc = -EBUSY;
			goto cci_release_mem;
		}
		disable_irq(new_cci_dev->irq->start);
	} else if (rc < 0) {
		CDBG("%s Error registering irq ", __func__);
		rc = -EBUSY;
		goto cci_release_mem;
	}

	new_cci_dev->pdev = pdev;
	msm_cci_initialize_cci_params(new_cci_dev);
	rc = of_platform_populate(pdev->dev.of_node, NULL, NULL, &pdev->dev);
	if (rc)
		pr_err("%s: failed to add child nodes, rc=%d\n", __func__, rc);
	CDBG("%s line %d\n", __func__, __LINE__);
	return 0;

cci_release_mem:
	release_mem_region(new_cci_dev->mem->start,
		resource_size(new_cci_dev->mem));
cci_no_resource:
	kfree(new_cci_dev);
	return 0;
}

static int __exit msm_cci_exit(struct platform_device *pdev)
{
	struct v4l2_subdev *subdev = platform_get_drvdata(pdev);
	struct cci_device *cci_dev =
		v4l2_get_subdevdata(subdev);
	release_mem_region(cci_dev->mem->start, resource_size(cci_dev->mem));
	kfree(cci_dev);
	return 0;
}

static const struct of_device_id msm_cci_dt_match[] = {
	{.compatible = "qcom,cci"},
	{}
};

MODULE_DEVICE_TABLE(of, msm_cci_dt_match);

static struct platform_driver cci_driver = {
	.probe = msm_cci_probe,
	.remove = msm_cci_exit,
	.driver = {
		.name = MSM_CCI_DRV_NAME,
		.owner = THIS_MODULE,
		.of_match_table = msm_cci_dt_match,
	},
};

static int __init msm_cci_init_module(void)
{
	return platform_driver_register(&cci_driver);
}

static void __exit msm_cci_exit_module(void)
{
	platform_driver_unregister(&cci_driver);
}

module_init(msm_cci_init_module);
module_exit(msm_cci_exit_module);
MODULE_DESCRIPTION("MSM CCI driver");
MODULE_LICENSE("GPL v2");
