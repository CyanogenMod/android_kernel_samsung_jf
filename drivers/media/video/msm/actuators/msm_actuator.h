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
#ifndef MSM_ACTUATOR_H
#define MSM_ACTUATOR_H

#include <linux/i2c.h>
#include <mach/camera.h>
#include <mach/gpio.h>
#include <media/v4l2-subdev.h>
#include <media/msm_camera.h>
#include "msm_camera_i2c.h"

#ifdef LERROR
#undef LERROR
#endif

#ifdef LINFO
#undef LINFO
#endif

#define LERROR(fmt, args...) pr_err(fmt, ##args)

#define CONFIG_MSM_CAMERA_ACT_DBG 0

#if CONFIG_MSM_CAMERA_ACT_DBG
#define LINFO(fmt, args...) printk(fmt, ##args)
#else
#define LINFO(fmt, args...) CDBG(fmt, ##args)
#endif

struct msm_actuator_ctrl_t;

struct msm_actuator_func_tbl {
	int32_t (*actuator_i2c_write_b_af)(struct msm_actuator_ctrl_t *,
			uint8_t,
			uint8_t);
	int32_t (*actuator_init_step_table)(struct msm_actuator_ctrl_t *,
		struct msm_actuator_set_info_t *);
	int32_t (*actuator_init_focus)(struct msm_actuator_ctrl_t *,
		uint16_t, enum msm_actuator_data_type, struct reg_settings_t *);
	int32_t (*actuator_set_default_focus) (struct msm_actuator_ctrl_t *,
			struct msm_actuator_move_params_t *);
	int32_t (*actuator_move_focus) (struct msm_actuator_ctrl_t *,
			struct msm_actuator_move_params_t *);
	int32_t (*actuator_parse_i2c_params)(struct msm_actuator_ctrl_t *,
			int16_t, uint32_t, uint16_t);
	int32_t (*actuator_write_focus)(struct msm_actuator_ctrl_t *,
			uint16_t,
			struct damping_params_t *,
			int8_t,
			int16_t);
};

struct msm_actuator {
	enum actuator_type act_type;
	struct msm_actuator_func_tbl func_tbl;
};

struct msm_actuator_ctrl_t {
	struct i2c_driver *i2c_driver;
	struct msm_camera_i2c_client i2c_client;
	struct mutex *actuator_mutex;
	struct msm_actuator_func_tbl *func_tbl;
	enum msm_actuator_data_type i2c_data_type;
	struct v4l2_subdev sdev;
	struct v4l2_subdev_ops *act_v4l2_subdev_ops;

	int16_t curr_step_pos;
	uint16_t curr_region_index;
	uint16_t *step_position_table;
	struct region_params_t region_params[MAX_ACTUATOR_REGION];
	uint16_t reg_tbl_size;
	struct msm_actuator_reg_params_t reg_tbl[MAX_ACTUATOR_REG_TBL_SIZE];
	uint16_t region_size;
	void *user_data;
	uint32_t vcm_pwd;
	uint32_t vcm_enable;
	uint32_t total_steps;
	uint16_t pwd_step;
	uint16_t initial_code;
	struct msm_camera_i2c_reg_tbl *i2c_reg_tbl;
	uint16_t i2c_tbl_index;
	uint32_t curr_hwparams;
};

struct msm_actuator_ctrl_t *get_actrl(struct v4l2_subdev *sd);

#define VIDIOC_MSM_ACTUATOR_CFG \
	_IOWR('V', BASE_VIDIOC_PRIVATE + 11, void __user *)

#endif
