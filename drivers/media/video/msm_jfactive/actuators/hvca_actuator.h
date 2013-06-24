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
 *
 */
#ifndef HYBRID_VCA_H
#define HYBRID_VCA_H

#include <linux/types.h>
#include <linux/i2c.h>
#include "msm_actuator.h"

//#define HVCA_DEBUG

#ifdef HVCA_DEBUG
#define CDBG_HVCA(fmt, args...)	pr_err(fmt, ##args)
#else
#define CDBG_HVCA(fmt, args...)
#endif

int32_t hvca_actuator_init_focus(
	struct msm_actuator_ctrl_t *a_ctrl,
	uint16_t size, enum msm_actuator_data_type type,
	struct reg_settings_t *settings);

int32_t hvca_actuator_init_step_table(
	struct msm_actuator_ctrl_t *a_ctrl,
	struct msm_actuator_set_info_t *set_info);

int32_t hvca_actuator_set_default_focus(
	struct msm_actuator_ctrl_t *a_ctrl,
	struct msm_actuator_move_params_t *move_params);

int32_t hvca_actuator_move_focus(
	struct msm_actuator_ctrl_t *a_ctrl,
	struct msm_actuator_move_params_t *move_params);


#endif


