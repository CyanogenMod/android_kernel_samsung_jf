/* Copyright (c) 2011-2012, Code Aurora Forum. All rights reserved.
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

#include "msm_sensor.h"
#define SENSOR_NAME "imx175"
#define PLATFORM_DRIVER_NAME "msm_camera_imx175"
#define imx175_obj imx175_##obj

DEFINE_MUTEX(imx175_mut);

#define cam_err(fmt, arg...)			\
	do {					\
		printk(KERN_ERR "[CAMERA]][%s:%d] " fmt,		\
			__func__, __LINE__, ##arg);	\
	}						\
	while (0)

static struct msm_sensor_ctrl_t imx175_s_ctrl;
struct class *camera_class;


static struct msm_camera_i2c_reg_conf imx175_start_settings[] = {
	{0x0100, 0x01},
};

static struct msm_camera_i2c_reg_conf imx175_stop_settings[] = {
	{0x0100, 0x00},
};

static struct msm_camera_i2c_reg_conf imx175_groupon_settings[] = {
	{0x104, 0x01},
};

static struct msm_camera_i2c_reg_conf imx175_groupoff_settings[] = {
	{0x104, 0x00},
};

static struct msm_camera_i2c_reg_conf imx175_prev_settings[] = {
	{0x0307, 0x2D}, /*pll_multiplier*/
	{0x0340, 0x06}, /*frame_length_lines_hi*/
	{0x0341, 0x34}, /*frame_length_lines_lo*/
	{0x0342, 0x11}, /*line_length_pclk_hi*/
	{0x0343, 0x78}, /*line_length_pclk_lo*/
	{0x0347, 0x00}, /*y_addr_start*/
	{0x034b, 0x2F}, /*y_add_end*/
	{0x034c, 0x08}, /*x_output_size_msb*/
	{0x034d, 0x38}, /*x_output_size_lsb*/
	{0x034e, 0x06}, /*y_output_size_msb*/
	{0x034f, 0x18}, /*y_output_size_lsb*/
	{0x0381, 0x01}, /*x_even_inc*/
	{0x0383, 0x03}, /*x_odd_inc*/
	{0x0385, 0x01}, /*y_even_inc*/
	{0x0387, 0x03}, /*y_odd_inc*/
	{0x3001, 0x80}, /*hmodeadd*/
	{0x3016, 0x16}, /*vmodeadd*/
	{0x3069, 0x24}, /*vapplinepos_start*/
	{0x306b, 0x53}, /*vapplinepos_end*/
	{0x3086, 0x00}, /*shutter*/
	{0x30e8, 0x80}, /*haddave*/
	{0x3301, 0x83}, /*lanesel*/
};

static struct msm_camera_i2c_reg_conf imx175_snap_settings[] = {
	{0x0307, 0x26}, /*pll_multiplier*/
	{0x0340, 0x0C}, /*frame_length_lines_hi*/
	{0x0341, 0x90}, /*frame_length_lines_lo*/
	{0x0342, 0x11}, /*line_length_pclk_hi*/
	{0x0343, 0x78}, /*line_length_pclk_lo*/
	{0x0347, 0x00}, /*y_addr_start*/
	{0x034b, 0x2F}, /*y_add_end*/
	{0x034c, 0x10}, /*x_output_size_msb*/
	{0x034d, 0x70}, /*x_output_size_lsb*/
	{0x034e, 0x0c}, /*y_output_size_msb*/
	{0x034f, 0x30}, /*y_output_size_lsb*/
	{0x0381, 0x01}, /*x_even_inc*/
	{0x0383, 0x01}, /*x_odd_inc*/
	{0x0385, 0x01}, /*y_even_inc*/
	{0x0387, 0x01}, /*y_odd_inc*/
	{0x3001, 0x00}, /*hmodeadd*/
	{0x3016, 0x06}, /*vmodeadd*/
	{0x3069, 0x24}, /*vapplinepos_start*/
	{0x306b, 0x53}, /*vapplinepos_end*/
	{0x3086, 0x00}, /*shutter*/
	{0x30e8, 0x00}, /*haddave*/
	{0x3301, 0x03}, /*lanesel*/
};

static struct msm_camera_i2c_reg_conf imx175_recommend_settings[] = {
	{0x0305, 0x02},
	{0x302b, 0x4B},
	{0x3024, 0x03},
	{0x0101, 0x00},
	{0x300a, 0x80},
	{0x3014, 0x08},
	{0x3015, 0x37},
	{0x301c, 0x01},
	{0x302c, 0x05},
	{0x3031, 0x26},
	{0x3041, 0x60},
	{0x3051, 0x24},
	{0x3053, 0x34},
	{0x3057, 0xc0},
	{0x305c, 0x09},
	{0x305d, 0x07},
	{0x3060, 0x30},
	{0x3065, 0x00},
	{0x30aa, 0x08},
	{0x30ab, 0x1c},
	{0x30b0, 0x32},
	{0x30b2, 0x83},
	{0x30d3, 0x04},
	{0x3106, 0x78},
	{0x310c, 0x82},
	{0x3304, 0x05},
	{0x3305, 0x04},
	{0x3306, 0x11},
	{0x3307, 0x02},
	{0x3308, 0x0c},
	{0x3309, 0x06},
	{0x330a, 0x08},
	{0x330b, 0x04},
	{0x330c, 0x08},
	{0x330d, 0x06},
	{0x330f, 0x01},
	{0x3381, 0x00},
};

static struct v4l2_subdev_info imx175_subdev_info[] = {
	{
	.code   = V4L2_MBUS_FMT_SBGGR10_1X10,
	.colorspace = V4L2_COLORSPACE_JPEG,
	.fmt    = 1,
	.order    = 0,
	},
	/* more can be supported, to be added later */
};

static struct msm_camera_i2c_conf_array imx175_init_conf[] = {
	{&imx175_recommend_settings[0],
	ARRAY_SIZE(imx175_recommend_settings), 0, MSM_CAMERA_I2C_BYTE_DATA}
};

static struct msm_camera_i2c_conf_array imx175_confs[] = {
	{&imx175_snap_settings[0],
	ARRAY_SIZE(imx175_snap_settings), 0, MSM_CAMERA_I2C_BYTE_DATA},
	{&imx175_prev_settings[0],
	ARRAY_SIZE(imx175_prev_settings), 0, MSM_CAMERA_I2C_BYTE_DATA},
};

static struct msm_sensor_output_info_t imx175_dimensions[] = {
	{
		.x_output = 0x1070,
		.y_output = 0xC30,
		.line_length_pclk = 0x1178,
		.frame_length_lines = 0xC90,
		.vt_pixel_clk = 182400000,
		.op_pixel_clk = 182400000,
		.binning_factor = 1,
	},
	{
		.x_output = 0x838,
		.y_output = 0x618,
		.line_length_pclk = 0x1178,
		.frame_length_lines = 0x634,
		.vt_pixel_clk = 216000000,
		.op_pixel_clk = 108000000,
		.binning_factor = 2,
	},
};

static struct msm_sensor_output_reg_addr_t imx175_reg_addr = {
	.x_output = 0x34C,
	.y_output = 0x34E,
	.line_length_pclk = 0x342,
	.frame_length_lines = 0x340,
};

static struct msm_sensor_id_info_t imx175_id_info = {
	.sensor_id_reg_addr = 0x0,
	.sensor_id = 0x0074,
};

static struct msm_sensor_exp_gain_info_t imx175_exp_gain_info = {
	.coarse_int_time_addr = 0x202,
	.global_gain_addr = 0x204,
	.vert_offset = 3,
};

static enum msm_camera_vreg_name_t imx175_veg_seq[] = {
	CAM_VDIG,
	CAM_VIO,
	CAM_VANA,
	CAM_VAF,
};

static const struct i2c_device_id imx175_i2c_id[] = {
	{SENSOR_NAME, (kernel_ulong_t)&imx175_s_ctrl},
	{ }
};

static struct i2c_driver imx175_i2c_driver = {
	.id_table = imx175_i2c_id,
	.probe  = msm_sensor_i2c_probe,
	.driver = {
		.name = SENSOR_NAME,
	},
};

static struct msm_camera_i2c_client imx175_sensor_i2c_client = {
	.addr_type = MSM_CAMERA_I2C_WORD_ADDR,
};

static int __init msm_sensor_init_module(void)
{
	struct device *cam_dev_rear = NULL;

	camera_class = class_create(THIS_MODULE, "camera");
	if (IS_ERR(camera_class)) {
		cam_err("Failed to create class(camera)!\n");
		return 0;
	}

	cam_dev_rear =
	device_create(camera_class, NULL, 0, NULL, "rear");
	if (IS_ERR(cam_dev_rear)) {
		cam_err("failed to create device cam_dev_rear!\n");
		return 0;
	}


	return i2c_add_driver(&imx175_i2c_driver);
}

int32_t imx175_sensor_power_up(struct msm_sensor_ctrl_t *s_ctrl)
{
	int rc = 0;
/*	int err;
	u32 int_factor;
	int temp = 0; */
	struct msm_camera_sensor_info *data = s_ctrl->sensordata;
	cam_err("imx175_sensor_power_up");

	rc = msm_camera_request_gpio_table(data, 1);
	if (rc < 0)
		pr_err("%s: request gpio failed\n", __func__);

	/* Power on */
	data->sensor_platform_info->sensor_power_on();

	if (data->sensor_platform_info->i2c_conf &&
		data->sensor_platform_info->i2c_conf->use_i2c_mux)
		msm_sensor_enable_i2c_mux(data->sensor_platform_info->i2c_conf);

	usleep(1200);


	/* CAM_RESET */
	data->sensor_platform_info->
		sensor_pmic_gpio_ctrl(data->sensor_platform_info->reset, 1);


	usleep(50);

	usleep(5000);
	cam_err("X");

	return rc;
}

int32_t imx175_sensor_power_down(struct msm_sensor_ctrl_t *s_ctrl)
{
	struct msm_camera_sensor_info *data = s_ctrl->sensordata;


	if (data->sensor_platform_info->i2c_conf &&
		data->sensor_platform_info->i2c_conf->use_i2c_mux)
		msm_sensor_disable_i2c_mux(
			data->sensor_platform_info->i2c_conf);

	cam_err("set GPIO_CAM_MAIN_RST to 0\n");
	data->sensor_platform_info->
		sensor_pmic_gpio_ctrl(data->sensor_platform_info->reset, 0);

	
	udelay(1000);

	

	kfree(s_ctrl->reg_ptr);
	return 0;
}



static struct v4l2_subdev_core_ops imx175_subdev_core_ops = {
	.ioctl = msm_sensor_subdev_ioctl,
	.s_power = msm_sensor_power,
};

static struct v4l2_subdev_video_ops imx175_subdev_video_ops = {
	.enum_mbus_fmt = msm_sensor_v4l2_enum_fmt,
};

static struct v4l2_subdev_ops imx175_subdev_ops = {
	.core = &imx175_subdev_core_ops,
	.video  = &imx175_subdev_video_ops,
};

static struct msm_sensor_fn_t imx175_func_tbl = {
	.sensor_start_stream = msm_sensor_start_stream,
	.sensor_stop_stream = msm_sensor_stop_stream,
	.sensor_group_hold_on = msm_sensor_group_hold_on,
	.sensor_group_hold_off = msm_sensor_group_hold_off,
	.sensor_set_fps = msm_sensor_set_fps,
	.sensor_write_exp_gain = msm_sensor_write_exp_gain1,
	.sensor_write_snapshot_exp_gain = msm_sensor_write_exp_gain1,
	.sensor_setting = msm_sensor_setting,
	.sensor_csi_setting = msm_sensor_setting1,
	.sensor_set_sensor_mode = msm_sensor_set_sensor_mode,
	.sensor_mode_init = msm_sensor_mode_init,
	.sensor_get_output_info = msm_sensor_get_output_info,
	.sensor_config = msm_sensor_config,
	.sensor_power_up = imx175_sensor_power_down,
	.sensor_power_down = imx175_sensor_power_down,
	.sensor_adjust_frame_lines = msm_sensor_adjust_frame_lines1,
	.sensor_get_csi_params = msm_sensor_get_csi_params,
};

static struct msm_sensor_reg_t imx175_regs = {
	.default_data_type = MSM_CAMERA_I2C_BYTE_DATA,
	.start_stream_conf = imx175_start_settings,
	.start_stream_conf_size = ARRAY_SIZE(imx175_start_settings),
	.stop_stream_conf = imx175_stop_settings,
	.stop_stream_conf_size = ARRAY_SIZE(imx175_stop_settings),
	.group_hold_on_conf = imx175_groupon_settings,
	.group_hold_on_conf_size = ARRAY_SIZE(imx175_groupon_settings),
	.group_hold_off_conf = imx175_groupoff_settings,
	.group_hold_off_conf_size =
		ARRAY_SIZE(imx175_groupoff_settings),
	.init_settings = &imx175_init_conf[0],
	.init_size = ARRAY_SIZE(imx175_init_conf),
	.mode_settings = &imx175_confs[0],
	.output_settings = &imx175_dimensions[0],
	.num_conf = ARRAY_SIZE(imx175_confs),
};

static struct msm_sensor_ctrl_t imx175_s_ctrl = {
	.msm_sensor_reg = &imx175_regs,
	.sensor_i2c_client = &imx175_sensor_i2c_client,
	.sensor_i2c_addr = 0x34,
	.vreg_seq = imx175_veg_seq,
	.num_vreg_seq = ARRAY_SIZE(imx175_veg_seq),
	.sensor_output_reg_addr = &imx175_reg_addr,
	.sensor_id_info = &imx175_id_info,
	.sensor_exp_gain_info = &imx175_exp_gain_info,
	.cam_mode = MSM_SENSOR_MODE_INVALID,
	.msm_sensor_mutex = &imx175_mut,
	.sensor_i2c_driver = &imx175_i2c_driver,
	.sensor_v4l2_subdev_info = imx175_subdev_info,
	.sensor_v4l2_subdev_info_size = ARRAY_SIZE(imx175_subdev_info),
	.sensor_v4l2_subdev_ops = &imx175_subdev_ops,
	.func_tbl = &imx175_func_tbl,
	.clk_rate = MSM_SENSOR_MCLK_24HZ,
};

module_init(msm_sensor_init_module);
MODULE_DESCRIPTION("Sony 13MP Bayer sensor driver");
MODULE_LICENSE("GPL v2");
