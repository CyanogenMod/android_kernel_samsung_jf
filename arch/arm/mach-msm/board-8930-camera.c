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
 *
 */

#include <asm/mach-types.h>
#include <linux/gpio.h>
#include <mach/camera.h>
#include <mach/msm_bus_board.h>
#include <mach/gpiomux.h>
#include <mach/msm8930-gpio.h>
#include "devices.h"
#include "board-8930.h"
#include "devices-msm8x60.h"
#include <linux/ctype.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#ifdef CONFIG_LEDS_RT8547
#include <linux/leds-rt8547.h>
#endif /*CONFIG_LEDS_RT8547*/

#undef cam_err
#define cam_err(fmt, arg...)			\
	do {					\
		printk(KERN_ERR "[CAMERA][%s:%d] " fmt,		\
		       __func__, __LINE__, ##arg);	\
	}						\
	while (0)

struct device 	*cam_dev_back;
struct class 	*camera_class;
extern int s5k4ecgx_get_sensor_vendorid(void);

#if !defined(CONFIG_MACH_BISCOTTO)
static struct gpiomux_setting cam_settings[] = {
	{
		.func = GPIOMUX_FUNC_GPIO, /*suspend*/
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_DOWN,
	},

	{
		.func = GPIOMUX_FUNC_1, /*active 1*/
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
	},

	{
		.func = GPIOMUX_FUNC_GPIO, /*active 2*/
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
	},

	{
		.func = GPIOMUX_FUNC_1, /*active 3*/
		.drv = GPIOMUX_DRV_8MA,
		.pull = GPIOMUX_PULL_NONE,
	},

	{
		.func = GPIOMUX_FUNC_5, /*active 4*/
		.drv = GPIOMUX_DRV_8MA,
		.pull = GPIOMUX_PULL_UP,
	},

	{
		.func = GPIOMUX_FUNC_GPIO, /*active 5*/
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
	},

	{
		.func = GPIOMUX_FUNC_2, /*active 6*/
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_UP,
	},

	{
		.func = GPIOMUX_FUNC_3, /*active 7*/
		.drv = GPIOMUX_DRV_8MA,
		.pull = GPIOMUX_PULL_UP,
	},

	{
		.func = GPIOMUX_FUNC_GPIO, /*i2c suspend*/
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_DOWN,
	},
	{
		.func = GPIOMUX_FUNC_GPIO, /*active 9*/
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_DOWN,
		.dir = GPIOMUX_OUT_LOW,
	},

};
#endif
#if defined(CONFIG_IMX175)
static struct msm_gpiomux_config msm8930_cam_common_configs[] = {
#if defined(CONFIG_MACH_MELIUS)
	{
		.gpio = GPIO_CAM_SENSOR_EN,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
#elif !defined(CONFIG_MACH_KS02)
	{
		.gpio = GPIO_FLASH_LED_UNLOCK,
	.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
#endif
	{
		.gpio = GPIO_MSM_FLASH_NOW,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = GPIO_SUB_CAM_MCLK,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = GPIO_MAIN_CAM_MCLK,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = GPIO_CAM_IO_EN,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = GPIO_CAM2_RST_N,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = GPIO_CAM1_RST_N,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
};

static struct msm_gpiomux_config msm8930_cam_2d_configs[] = {
	{
		.gpio = GPIO_I2C_DATA_CAM,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[5],
		},
	},
	{
		.gpio = GPIO_I2C_CLK_CAM,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[5],
		},
	},
	{
		.gpio = GPIO_I2C_DATA_AF,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[8],
		},
	},
	{
		.gpio = GPIO_I2C_CLK_AF,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[8],
		},
	},
};
#elif defined(CONFIG_MACH_BISCOTTO)
static struct msm_gpiomux_config msm8930_cam_common_configs[] = {
};
#elif defined(CONFIG_MACH_CANE) || defined(CONFIG_MACH_LOGANRE)
static struct msm_gpiomux_config msm8930_cam_common_configs[] = {
	{
		.gpio = GPIO_CAM_CORE_EN,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = GPIO_CAM_FLASH_SET,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[9],
			[GPIOMUX_SUSPENDED] = &cam_settings[9],
		},
	},
	{
		.gpio = GPIO_CAM_FLASH_EN,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[9],
			[GPIOMUX_SUSPENDED] = &cam_settings[9],
		},
	},
	{
		.gpio = GPIO_MAIN_CAM_MCLK,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[9],
			[GPIOMUX_SUSPENDED] = &cam_settings[9],
		},
	},
	{
		.gpio = GPIO_SUB_CAM_MCLK,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[9],
			[GPIOMUX_SUSPENDED] = &cam_settings[9],
		},
	},
	{
		.gpio = GPIO_CAM2_RST_N,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = GPIO_CAM1_RST_N,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = GPIO_VT_STBY,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[9],
			[GPIOMUX_SUSPENDED] = &cam_settings[9],
		},
	},
	{
		.gpio = GPIO_MAIN_STBY,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[9],
			[GPIOMUX_SUSPENDED] = &cam_settings[9],
		},
	},
};

static struct msm_gpiomux_config msm8930_cam_2d_configs[] = {
	{
		.gpio = GPIO_I2C_DATA_CAM,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[8],
		},
	},
	{
		.gpio = GPIO_I2C_CLK_CAM,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[8],
		},
	},
};

#elif defined(CONFIG_SR352)
static struct msm_gpiomux_config msm8930_cam_common_configs[] = {
	{
		.gpio = GPIO_CAM_SENSOR_EN,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
#if !defined(CONFIG_MACH_LT02_CHN_CTC)
	{
		.gpio = GPIO_MSM_FLASH_NOW,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
#endif
	{
		.gpio = GPIO_SUB_CAM_MCLK,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = GPIO_MAIN_CAM_MCLK,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = GPIO_CAM_IO_EN,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = GPIO_CAM2_RST_N,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = GPIO_CAM1_RST_N,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
};

static struct msm_gpiomux_config msm8930_cam_2d_configs[] = {
	{
		.gpio = GPIO_I2C_DATA_CAM,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[8],
		},
	},
	{
		.gpio = GPIO_I2C_CLK_CAM,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[8],
		},
	},
};

#else
static struct msm_gpiomux_config msm8930_cam_common_configs[] = {
	{
		.gpio = GPIO_FLASH_LED_UNLOCK,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[1],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = GPIO_MSM_FLASH_NOW,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[1],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = GPIO_MAIN_CAM_MCLK,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[1],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = GPIO_SUB_CAM_MCLK,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[1],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = GPIO_CAM2_RST_N,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = GPIO_CAM1_RST_N,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
	{
		.gpio = GPIO_VT_STBY,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[2],
			[GPIOMUX_SUSPENDED] = &cam_settings[0],
		},
	},
};

static struct msm_gpiomux_config msm8930_cam_2d_configs[] = {
	{
		.gpio = GPIO_I2C_DATA_CAM,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[8],
		},
	},
	{
		.gpio = GPIO_I2C_CLK_CAM,
		.settings = {
			[GPIOMUX_ACTIVE]    = &cam_settings[3],
			[GPIOMUX_SUSPENDED] = &cam_settings[8],
		},
	},
};
#endif

#ifdef CONFIG_MSM_CAMERA
#define VFE_CAMIF_TIMER1_GPIO 2
#define VFE_CAMIF_TIMER2_GPIO 3
#define VFE_CAMIF_TIMER3_GPIO_INT 4
#if defined(CONFIG_S5K3L1YX)
static struct msm_camera_sensor_strobe_flash_data strobe_flash_xenon = {
	.flash_trigger = VFE_CAMIF_TIMER2_GPIO,
	.flash_charge = VFE_CAMIF_TIMER1_GPIO,
	.flash_charge_done = VFE_CAMIF_TIMER3_GPIO_INT,
	.flash_recharge_duration = 50000,
	.irq = MSM_GPIO_TO_INT(VFE_CAMIF_TIMER3_GPIO_INT),
};
#endif
//#ifdef CONFIG_MSM_CAMERA_FLASH
#if 0
static struct msm_camera_sensor_flash_src msm_flash_src = {
	.flash_sr_type = MSM_CAMERA_FLASH_SRC_EXT,
	._fsrc.ext_driver_src.led_en = VFE_CAMIF_TIMER1_GPIO,
	._fsrc.ext_driver_src.led_flash_en = VFE_CAMIF_TIMER2_GPIO,
	._fsrc.ext_driver_src.flash_id = MAM_CAMERA_EXT_LED_FLASH_TPS61310,
};
#endif

#if !defined(CONFIG_MACH_BISCOTTO)
#if defined(CONFIG_IMX175)
#if !defined(CONFIG_MACH_CRATER_CHN_CTC)
static struct msm_camera_sensor_flash_src imx175_flash_src = {
	.flash_sr_type = MSM_CAMERA_FLASH_SRC_LED1,
#if !defined(CONFIG_MACH_KS02)
	._fsrc.ext_driver_src.led_en = GPIO_FLASH_LED_UNLOCK,
#endif
	._fsrc.ext_driver_src.led_flash_en = GPIO_MSM_FLASH_NOW,
};
#else
static struct msm_camera_sensor_flash_src imx175_flash_src = {
	.flash_sr_type = MSM_CAMERA_FLASH_SRC_LED1,
#if !defined(CONFIG_MACH_KS02)
	._fsrc.ext_driver_src.led_en = GPIO_MSM_FLASH_NOW,//3
#endif
	._fsrc.ext_driver_src.led_flash_en = GPIO_FLASH_LED_UNLOCK,
}; 
#endif
#endif
#if defined(CONFIG_S5K4ECGX)
#if defined(CONFIG_MACH_CANE) || defined(CONFIG_MACH_LOGANRE)
static struct msm_camera_sensor_flash_src s5k4ecgx_flash_src = {
	.flash_sr_type = MSM_CAMERA_FLASH_SRC_LED1,
	._fsrc.ext_driver_src.led_en = GPIO_CAM_FLASH_SET,
	._fsrc.ext_driver_src.led_flash_en = GPIO_CAM_FLASH_EN,
};
#endif
#endif

static struct msm_bus_vectors cam_init_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
};

/*Start : shchang@qualcomm.com : 1101*/
static struct msm_bus_vectors cam_preview_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 300000000,
		.ib  = 2656000000UL,	/*110592000,*/
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
};
/*End : shchang@qualcomm.com : 1101*/

static struct msm_bus_vectors cam_video_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 800000000,
		.ib  = 2656000000UL,	/*561807360,*/
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 206807040,
		.ib  = 488816640,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
};

static struct msm_bus_vectors cam_snapshot_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 800000000,	/*274423680,*/
		.ib  = 2656000000UL,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 540000000,
		.ib  = 1350000000,
	},
};

static struct msm_bus_vectors cam_zsl_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		/*.ab  = 302071680,*/
		.ab  = 800000000,
		.ib  = 2656000000UL, /*1208286720,*/
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 1350000000,
	},
};

static struct msm_bus_vectors cam_video_ls_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 800000000,	/*348192000,*/
		.ib  = 3522000000UL,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 206807040,
		.ib  = 488816640,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 1350000000,
	},
};

static struct msm_bus_vectors cam_dual_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 302071680,
		.ib  = 2656000000UL,	/*1208286720,*/
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 206807040,
		.ib  = 488816640,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 540000000,
		.ib  = 1350000000,
	},
};

static struct msm_bus_vectors cam_adv_video_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 800000000,
		.ib  = 2656000000UL,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 206807040,
		.ib  = 488816640,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
};

static struct msm_bus_paths cam_bus_client_config[] = {
	{
		ARRAY_SIZE(cam_init_vectors),
		cam_init_vectors,
	},
	{
		ARRAY_SIZE(cam_preview_vectors),
		cam_preview_vectors,
	},
	{
		ARRAY_SIZE(cam_video_vectors),
		cam_video_vectors,
	},
	{
		ARRAY_SIZE(cam_snapshot_vectors),
		cam_snapshot_vectors,
	},
	{
		ARRAY_SIZE(cam_zsl_vectors),
		cam_zsl_vectors,
	},
	{
		ARRAY_SIZE(cam_video_ls_vectors),
		cam_video_ls_vectors,
	},
	{
		ARRAY_SIZE(cam_dual_vectors),
		cam_dual_vectors,
	},
	{
		ARRAY_SIZE(cam_adv_video_vectors),
		cam_adv_video_vectors,
	},

};
/*End : shchang@qualcomm.com : 1101*/

static struct msm_bus_scale_pdata cam_bus_client_pdata = {
		cam_bus_client_config,
		ARRAY_SIZE(cam_bus_client_config),
		.name = "msm_camera",
};

static struct msm_camera_device_platform_data msm_camera_csi_device_data[] = {
	{
		.csid_core = 0,
		.is_csiphy = 1,
		.is_csid   = 1,
		.is_ispif  = 1,
		.is_vpe    = 1,
		.cam_bus_scale_table = &cam_bus_client_pdata,
	},
	{
		.csid_core = 1,
		.is_csiphy = 1,
		.is_csid   = 1,
		.is_ispif  = 1,
		.is_vpe    = 1,
		.cam_bus_scale_table = &cam_bus_client_pdata,
	},
};
#endif
#if defined(CONFIG_S5K3L1YX)
static struct camera_vreg_t msm_8930_cam_vreg[] = {
	{"cam_vdig", REG_LDO, 1200000, 1200000, 105000},
	{"cam_vio", REG_VS, 0, 0, 0},
	{"cam_vana", REG_LDO, 2800000, 2850000, 85600},
	{"cam_vaf", REG_LDO, 2800000, 2850000, 300000},
};
#endif
#if defined(CONFIG_ISX012)
static struct camera_vreg_t msm_8930_back_cam_vreg[] = {
	{"cam_vdig", REG_LDO, 1200000, 1200000, 105000},
	{"cam_vio", REG_VS, 0, 0, 0},
	{"cam_vana", REG_LDO, 2800000, 2850000, 85600},
	{"cam_vaf", REG_LDO, 2800000, 2850000, 300000},
};
#endif
#if defined(CONFIG_S5K5CCGX)
static struct camera_vreg_t msm_8930_back_cam_vreg[] = {
	{"cam_vdig", REG_LDO, 1200000, 1200000, 105000},
	{"cam_vio", REG_VS, 0, 0, 0},
	{"cam_vana", REG_LDO, 2800000, 2850000, 85600},
	{"cam_vio", REG_VS, 0, 0, 0},
	{"cam_vaf", REG_LDO, 2800000, 2800000, 300000},
};
#endif
#if defined(CONFIG_S5K4ECGX)
static struct camera_vreg_t msm_8930_back_cam_vreg[] = {
	{"cam_vdig", REG_LDO, 1200000, 1200000, 105000},
	{"cam_vio", REG_VS, 0, 0, 0},
	{"cam_vana", REG_LDO, 2800000, 2850000, 85600},
	{"cam_vaf", REG_LDO, 2800000, 2850000, 300000},
};
#endif
#if defined(CONFIG_SR352)
static struct camera_vreg_t msm_8930_back_cam_vreg[] = {
	{"cam_vdig", REG_LDO, 1200000, 1200000, 105000},
	{"cam_vio", REG_VS, 0, 0, 0},
	{"cam_vana", REG_LDO, 2800000, 2850000, 85600},
	{"cam_vio", REG_VS, 0, 0, 0},
	{"cam_vaf", REG_LDO, 2800000, 2850000, 300000},
};//jixu
#endif
#if !defined(CONFIG_MACH_BISCOTTO)
static struct camera_vreg_t msm_8930_front_cam_vreg[] = {
	{"cam_vio", REG_VS, 0, 0, 0},
	{"cam_vana", REG_LDO, 2800000, 2850000, 85600},
	{"cam_vdig", REG_LDO, 1200000, 1200000, 105000},
};
#endif
#if defined(CONFIG_IMX175)
static struct gpio msm8930_common_cam_gpio[] = {
	{GPIO_I2C_DATA_CAM, GPIOF_DIR_IN, "CAMIF_I2C_DATA"},
	{GPIO_I2C_CLK_CAM, GPIOF_DIR_IN, "CAMIF_I2C_CLK"},
};

static struct gpio msm8930_front_cam_gpio[] = {
	{GPIO_SUB_CAM_MCLK, GPIOF_DIR_IN, "CAMIF_MCLK"},
};

static struct gpio msm8930_back_cam_gpio[] = {
	{GPIO_MAIN_CAM_MCLK, GPIOF_DIR_IN, "CAMIF_MCLK"},
};

static struct msm_gpio_set_tbl msm8930_front_cam_gpio_set_tbl[] = {
};

static struct msm_gpio_set_tbl msm8930_back_cam_gpio_set_tbl[] = {
};
#elif defined(CONFIG_MACH_BISCOTTO)
#else
static struct gpio msm8930_common_cam_gpio[] = {
	{GPIO_I2C_DATA_CAM, GPIOF_DIR_IN, "CAMIF_I2C_DATA"},
	{GPIO_I2C_CLK_CAM, GPIOF_DIR_IN, "CAMIF_I2C_CLK"},
};

static struct gpio msm8930_front_cam_gpio[] = {
	{GPIO_SUB_CAM_MCLK, GPIOF_DIR_IN, "CAMIF_MCLK"},
};

static struct gpio msm8930_back_cam_gpio[] = {
	{GPIO_MAIN_CAM_MCLK, GPIOF_DIR_IN, "CAMIF_MCLK"},
};

static struct msm_gpio_set_tbl msm8930_front_cam_gpio_set_tbl[] = {
};

static struct msm_gpio_set_tbl msm8930_back_cam_gpio_set_tbl[] = {
};
#endif
#if !defined(CONFIG_MACH_BISCOTTO)
static struct msm_camera_gpio_conf msm_8930_front_cam_gpio_conf = {
	.cam_gpiomux_conf_tbl = msm8930_cam_2d_configs,
	.cam_gpiomux_conf_tbl_size = ARRAY_SIZE(msm8930_cam_2d_configs),
	.cam_gpio_common_tbl = msm8930_common_cam_gpio,
	.cam_gpio_common_tbl_size = ARRAY_SIZE(msm8930_common_cam_gpio),
	.cam_gpio_req_tbl = msm8930_front_cam_gpio,
	.cam_gpio_req_tbl_size = ARRAY_SIZE(msm8930_front_cam_gpio),
	.cam_gpio_set_tbl = msm8930_front_cam_gpio_set_tbl,
	.cam_gpio_set_tbl_size = ARRAY_SIZE(msm8930_front_cam_gpio_set_tbl),
};

static struct msm_camera_gpio_conf msm_8930_back_cam_gpio_conf = {
	.cam_gpiomux_conf_tbl = msm8930_cam_2d_configs,
	.cam_gpiomux_conf_tbl_size = ARRAY_SIZE(msm8930_cam_2d_configs),
	.cam_gpio_common_tbl = msm8930_common_cam_gpio,
	.cam_gpio_common_tbl_size = ARRAY_SIZE(msm8930_common_cam_gpio),
	.cam_gpio_req_tbl = msm8930_back_cam_gpio,
	.cam_gpio_req_tbl_size = ARRAY_SIZE(msm8930_back_cam_gpio),
	.cam_gpio_set_tbl = msm8930_back_cam_gpio_set_tbl,
	.cam_gpio_set_tbl_size = ARRAY_SIZE(msm8930_back_cam_gpio_set_tbl),
};
#endif
#if defined(CONFIG_IMX175)
#if 0	//Check by teddy
static struct i2c_board_info msm_act_main_cam1_i2c_info = {
	I2C_BOARD_INFO("msm_actuator", 0x18),
};

static struct msm_actuator_info msm_act_main_cam_1_info = {
	.board_info     = &msm_act_main_cam1_i2c_info,
	.cam_name       = MSM_ACTUATOR_MAIN_CAM_1,
	.bus_id         = MSM_ACTUATOR_I2C_BUS_ID,
	.vcm_pwd        = 0,
	.vcm_enable     = 0,
};
#endif	//Check by teddy

#if defined(CONFIG_MACH_KS02) || defined(CONFIG_MACH_CRATER) || defined(CONFIG_MACH_BAFFIN) || defined(CONFIG_MACH_CRATER_CHN_CTC)
static struct i2c_board_info msm_act_main_cam_i2c_info = {	//Check by teddy
	I2C_BOARD_INFO("msm_actuator", 0x18),
};
#else
static struct i2c_board_info msm_act_main_cam_i2c_info = {	//Check by teddy
	I2C_BOARD_INFO("msm_actuator", 0x50),
};
#endif

#if defined(CONFIG_MACH_KS02) || defined(CONFIG_MACH_CRATER) || defined(CONFIG_MACH_BAFFIN) || defined(CONFIG_MACH_CRATER_CHN_CTC)
static struct msm_actuator_info msm_act_main_cam_2_info = {	//Check by teddy
	.board_info     = &msm_act_main_cam_i2c_info,
	.cam_name   = MSM_ACTUATOR_MAIN_CAM_1,
	.bus_id         = MSM_ACTUATOR_I2C_BUS_ID,
	.vcm_pwd        = 0,
	.vcm_enable     = 0,
};
#else
static struct msm_actuator_info msm_act_main_cam_2_info = {	//Check by teddy
	.board_info     = &msm_act_main_cam_i2c_info,
	.cam_name   = MSM_ACTUATOR_MAIN_CAM_2,
	.bus_id         = MSM_ACTUATOR_I2C_BUS_ID,
	.vcm_pwd        = 0,
	.vcm_enable     = 0,
};
#endif

static struct camera_vreg_t msm_8930_imx175_vreg[] = {
	{"cam_vdig", REG_LDO, 1500000, 1500000, 105000},
	{"cam_vio", REG_VS, 0, 0, 0},
	{"cam_vana", REG_LDO, 2800000, 2850000, 85600},
	{"cam_vaf", REG_LDO, 2800000, 2850000, 300000},
};

/*Start : shchang@qualcomm.com : 1104 -FROM*/
static struct msm_eeprom_info imx175_eeprom_info = {
	.type = MSM_EEPROM_SPI,
};
/*End : shchang@qualcomm.com : 1104 - FROM*/

static struct msm_camera_sensor_flash_data flash_imx175 = {
	.flash_type	= MSM_CAMERA_FLASH_LED,
	.flash_src	= &imx175_flash_src
};

static struct msm_camera_csi_lane_params imx175_csi_lane_params = {
	.csi_lane_assign = 0xE4,
	.csi_lane_mask = 0xF,
};
static struct msm_camera_sensor_platform_info sensor_board_info_imx175 = {
	.mount_angle	= 90,
	.cam_vreg = msm_8930_imx175_vreg,
	.num_vreg = ARRAY_SIZE(msm_8930_imx175_vreg),
	.gpio_conf = &msm_8930_back_cam_gpio_conf,
	.csi_lane_params = &imx175_csi_lane_params,
};

static struct msm_camera_sensor_info msm_camera_sensor_imx175_data = {
	.sensor_name	= "imx175",
	.pdata	= &msm_camera_csi_device_data[0],
	.flash_data	= &flash_imx175,
	.sensor_platform_info = &sensor_board_info_imx175,
	.csi_if	= 1,
	.camera_type = BACK_CAMERA_2D,
	.sensor_type = BAYER_SENSOR,
	.actuator_info = &msm_act_main_cam_2_info,
/*Start : shchang@qualcomm.com : 1104 -FROM*/
	.eeprom_info = &imx175_eeprom_info,
/*End : shchang@qualcomm.com : 1104 - FROM*/
};
#ifdef CONFIG_S5K3H5XA
static struct msm_camera_csi_lane_params s5k3h5xa_csi_lane_params = {
	.csi_lane_assign = 0xE4,
	.csi_lane_mask = 0xF,
};

static struct msm_camera_sensor_flash_data flash_s5k3h5xa = {
	.flash_type	= MSM_CAMERA_FLASH_LED,
	.flash_src	= &imx175_flash_src
};

static struct msm_camera_sensor_platform_info sensor_board_info_s5k3h5xa = {
 	.mount_angle	= 90,	//modify by teddy
 	.cam_vreg = msm_8930_imx175_vreg,	//modify by teddy
 	.num_vreg = ARRAY_SIZE(msm_8930_imx175_vreg),	//modify by teddy
	.gpio_conf = &msm_8930_back_cam_gpio_conf,
	.csi_lane_params = &s5k3h5xa_csi_lane_params,
};

static struct msm_camera_sensor_info msm_camera_sensor_s5k3h5xa_data = {
	.sensor_name = "s5k3h5xa",
	.pdata = &msm_camera_csi_device_data[0],
	.flash_data = &flash_s5k3h5xa,
	.sensor_platform_info = &sensor_board_info_s5k3h5xa,
	.csi_if = 1,
	.camera_type = BACK_CAMERA_2D,
	.sensor_type = BAYER_SENSOR,
 	.actuator_info = &msm_act_main_cam_2_info,	//modify by teddy
	.eeprom_info = &imx175_eeprom_info,			//add by teddy
};
#endif
#endif
#ifdef CONFIG_S5K6A3YX
static struct msm_camera_sensor_flash_data flash_s5k6a3yx = {
	.flash_type     = MSM_CAMERA_FLASH_NONE,
};

static struct msm_camera_csi_lane_params s5k6a3yx_csi_lane_params = {
	.csi_lane_assign = 0xE4,
	.csi_lane_mask = 0x1,
};

static struct msm_camera_sensor_platform_info sensor_board_info_s5k6a3yx = {
	.mount_angle    = 270,
	.cam_vreg = msm_8930_front_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_8930_front_cam_vreg),
	.gpio_conf = &msm_8930_front_cam_gpio_conf,
	.csi_lane_params = &s5k6a3yx_csi_lane_params,
};

static struct msm_camera_sensor_info msm_camera_sensor_s5k6a3yx_data = {
	.sensor_name	= "s5k6a3yx",
	.pdata	= &msm_camera_csi_device_data[1],
	.flash_data	= &flash_s5k6a3yx,
	.sensor_platform_info = &sensor_board_info_s5k6a3yx,
	.csi_if	= 1,
	.camera_type = FRONT_CAMERA_2D,
	.sensor_type = BAYER_SENSOR,
};
#endif

#if defined(CONFIG_S5K3L1YX)
static struct i2c_board_info msm_act_main_cam_i2c_info = {
	I2C_BOARD_INFO("msm_actuator", 0x11),
};

static struct msm_actuator_info msm_act_main_cam_0_info = {
	.board_info     = &msm_act_main_cam_i2c_info,
	.cam_name   = MSM_ACTUATOR_MAIN_CAM_0,
	.bus_id         = MSM_8930_GSBI4_QUP_I2C_BUS_ID,
	.vcm_pwd        = 0,
	.vcm_enable     = 0,
};

static struct msm_camera_sensor_flash_data flash_imx074 = {
	.flash_type	= MSM_CAMERA_FLASH_LED,
#ifdef CONFIG_MSM_CAMERA_FLASH
	.flash_src	= &msm_flash_src
#endif
};

static struct msm_camera_csi_lane_params imx074_csi_lane_params = {
	.csi_lane_assign = 0xE4,
	.csi_lane_mask = 0xF,
};

static struct msm_camera_sensor_platform_info sensor_board_info_imx074 = {
	.mount_angle	= 90,
	.cam_vreg = msm_8930_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_8930_cam_vreg),
	.gpio_conf = &msm_8930_back_cam_gpio_conf,
	.csi_lane_params = &imx074_csi_lane_params,
};

static struct msm_camera_sensor_info msm_camera_sensor_imx074_data = {
	.sensor_name	= "imx074",
	.pdata	= &msm_camera_csi_device_data[0],
	.flash_data	= &flash_imx074,
	.strobe_flash_data = &strobe_flash_xenon,
	.sensor_platform_info = &sensor_board_info_imx074,
	.csi_if	= 1,
	.camera_type = BACK_CAMERA_2D,
	.sensor_type = BAYER_SENSOR,
	.actuator_info = &msm_act_main_cam_0_info,
};

static struct msm_camera_sensor_flash_data flash_mt9m114 = {
	.flash_type = MSM_CAMERA_FLASH_NONE
};

static struct msm_camera_csi_lane_params mt9m114_csi_lane_params = {
	.csi_lane_assign = 0xE4,
	.csi_lane_mask = 0x1,
};

static struct msm_camera_sensor_platform_info sensor_board_info_mt9m114 = {
	.mount_angle = 90,
	.cam_vreg = msm_8930_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_8930_cam_vreg),
	.gpio_conf = &msm_8930_front_cam_gpio_conf,
	.csi_lane_params = &mt9m114_csi_lane_params,
};

static struct msm_camera_sensor_info msm_camera_sensor_mt9m114_data = {
	.sensor_name = "mt9m114",
	.pdata = &msm_camera_csi_device_data[1],
	.flash_data = &flash_mt9m114,
	.sensor_platform_info = &sensor_board_info_mt9m114,
	.csi_if = 1,
	.camera_type = FRONT_CAMERA_2D,
	.sensor_type = YUV_SENSOR,
};

static struct msm_camera_sensor_flash_data flash_ov2720 = {
	.flash_type	= MSM_CAMERA_FLASH_NONE,
};

static struct msm_camera_csi_lane_params ov2720_csi_lane_params = {
	.csi_lane_assign = 0xE4,
	.csi_lane_mask = 0x3,
};

static struct msm_camera_sensor_platform_info sensor_board_info_ov2720 = {
	.mount_angle	= 0,
	.cam_vreg = msm_8930_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_8930_cam_vreg),
	.gpio_conf = &msm_8930_front_cam_gpio_conf,
	.csi_lane_params = &ov2720_csi_lane_params,
};

static struct msm_camera_sensor_info msm_camera_sensor_ov2720_data = {
	.sensor_name	= "ov2720",
	.pdata	= &msm_camera_csi_device_data[1],
	.flash_data	= &flash_ov2720,
	.sensor_platform_info = &sensor_board_info_ov2720,
	.csi_if	= 1,
	.camera_type = FRONT_CAMERA_2D,
	.sensor_type = BAYER_SENSOR,
};
#endif

#if defined(CONFIG_S5K3L1YX)
static struct msm_camera_sensor_flash_data flash_s5k3l1yx = {
	.flash_type = MSM_CAMERA_FLASH_LED,
/*	.flash_src = &msm_flash_src*/
};

static struct msm_camera_csi_lane_params s5k3l1yx_csi_lane_params = {
	.csi_lane_assign = 0xE4,
	.csi_lane_mask = 0xF,
};

static struct msm_camera_sensor_platform_info sensor_board_info_s5k3l1yx = {
	.mount_angle  = 90,
	.cam_vreg = msm_8930_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_8930_cam_vreg),
	.gpio_conf = &msm_8930_back_cam_gpio_conf,
	.csi_lane_params = &s5k3l1yx_csi_lane_params,
};

static struct msm_actuator_info msm_act_main_cam_2_info = {
	.board_info     = &msm_act_main_cam_i2c_info,
	.cam_name   = MSM_ACTUATOR_MAIN_CAM_2,
	.bus_id         = MSM_8930_GSBI4_QUP_I2C_BUS_ID,
	.vcm_pwd        = 0,
	.vcm_enable     = 0,
};

static struct msm_camera_sensor_info msm_camera_sensor_s5k3l1yx_data = {
	.sensor_name          = "s5k3l1yx",
	.pdata                = &msm_camera_csi_device_data[0],
	.flash_data           = &flash_s5k3l1yx,
	.sensor_platform_info = &sensor_board_info_s5k3l1yx,
	.csi_if               = 1,
	.camera_type          = BACK_CAMERA_2D,
	.sensor_type          = BAYER_SENSOR,
	.actuator_info    = &msm_act_main_cam_2_info,
};
#endif
#ifdef CONFIG_S5K5CCGX
static struct msm_camera_sensor_flash_data flash_s5k5ccgx = {
	.flash_type	= MSM_CAMERA_FLASH_NONE,
};

static struct msm_camera_csi_lane_params s5k5ccgx_csi_lane_params = {
	.csi_lane_assign = 0xE4,
	.csi_lane_mask = 0x3,
};

static struct msm_camera_sensor_platform_info sensor_board_info_s5k5ccgx = {
	.sensor_reset	= GPIO_CAM1_RST_N,
	.mount_angle	= 90,
	.sensor_stby	= GPIO_MAIN_STBY,
	.csi_lane_params = &s5k5ccgx_csi_lane_params,
	.vt_sensor_reset	= GPIO_CAM2_RST_N,
	.vt_sensor_stby	= GPIO_VT_STBY,
	.flash_en	= GPIO_MSM_FLASH_CNTL_EN,
	.flash_set	= GPIO_MSM_FLASH_NOW,
	.mclk	= GPIO_MAIN_CAM_MCLK,
	.sensor_pwd	= GPIO_CAM_CORE_EN,
	.vcm_pwd	= 0,
	.vcm_enable	= 1,
	.sensor_power_on = cam_ldo_power_on,
	.sensor_power_off = cam_ldo_power_off,
	.cam_vreg = msm_8930_back_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_8930_back_cam_vreg),
	.gpio_conf = &msm_8930_back_cam_gpio_conf,
};

static struct msm_camera_sensor_info msm_camera_sensor_s5k5ccgx_data = {
	.sensor_name	= "s5k5ccgx",
	.pdata	= &msm_camera_csi_device_data[0],
	.flash_data	= &flash_s5k5ccgx,
	.sensor_platform_info = &sensor_board_info_s5k5ccgx,
	.csi_if	= 1,
	.camera_type = BACK_CAMERA_2D,
};
#endif
#ifdef CONFIG_ISX012
static struct msm_camera_sensor_flash_data flash_isx012 = {
	.flash_type	= MSM_CAMERA_FLASH_LED,
};

static struct msm_camera_csi_lane_params isx012_csi_lane_params = {
	.csi_lane_assign = 0xE4,
	.csi_lane_mask = 0x3,
};

static struct msm_camera_sensor_platform_info sensor_board_info_isx012 = {
	.mount_angle	= 90,
	.cam_vreg = msm_8930_back_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_8930_back_cam_vreg),
	.gpio_conf = &msm_8930_back_cam_gpio_conf,
	.csi_lane_params = &isx012_csi_lane_params,
	.sensor_reset	= GPIO_CAM1_RST_N,
	.sensor_stby	= GPIO_MAIN_STBY,
	.vt_sensor_stby	= GPIO_VT_STBY,
	.vt_sensor_reset	= GPIO_CAM2_RST_N,
	.flash_en	= GPIO_MSM_FLASH_NOW,
	.flash_set	= GPIO_FLASH_LED_UNLOCK,
	.mclk	= GPIO_MAIN_CAM_MCLK,
	.sensor_pwd	= GPIO_CAM_CORE_EN,
	.vcm_pwd	= 0,
	.vcm_enable	= 1,
	.sensor_power_on = cam_ldo_power_on,
	.sensor_power_off = cam_ldo_power_off,
	.cam_vreg = msm_8930_back_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_8930_back_cam_vreg),
	.gpio_conf = &msm_8930_back_cam_gpio_conf,
};

static struct msm_camera_sensor_info msm_camera_sensor_isx012_data = {
	.sensor_name	= "isx012",
	.pdata	= &msm_camera_csi_device_data[0],
	.flash_data	= &flash_isx012,
	.sensor_platform_info = &sensor_board_info_isx012,
	.csi_if	= 1,
	.camera_type = BACK_CAMERA_2D,
	.sensor_type = YUV_SENSOR,
};
#endif
#ifdef CONFIG_S5K4ECGX
static struct msm_camera_sensor_flash_data flash_s5k4ecgx = {
	.flash_type	= MSM_CAMERA_FLASH_LED,
	.flash_src		= &s5k4ecgx_flash_src
};

static struct msm_camera_csi_lane_params s5k4ecgx_csi_lane_params = {
	.csi_lane_assign = 0xE4,
	.csi_lane_mask = 0x3,
};

static struct msm_camera_sensor_platform_info sensor_board_info_s5k4ecgx = {
	.mount_angle	= 90,
	.cam_vreg = msm_8930_back_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_8930_back_cam_vreg),
	.gpio_conf = &msm_8930_back_cam_gpio_conf,
	.csi_lane_params = &s5k4ecgx_csi_lane_params,
	.sensor_reset	= GPIO_CAM1_RST_N,
	.sensor_stby	= GPIO_MAIN_STBY,
	.vt_sensor_stby	= GPIO_VT_STBY,
	.vt_sensor_reset	= GPIO_CAM2_RST_N,
	.flash_en	= GPIO_CAM_FLASH_EN,
	.flash_set	= GPIO_CAM_FLASH_SET,
	.mclk	= GPIO_MAIN_CAM_MCLK,
	.sensor_pwd	= GPIO_CAM_CORE_EN,
	.vcm_pwd	= 0,
	.vcm_enable	= 1,
	.sensor_power_on = cam_ldo_power_on,
	.sensor_power_off = cam_ldo_power_off,
	.cam_vreg = msm_8930_back_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_8930_back_cam_vreg),
	.gpio_conf = &msm_8930_back_cam_gpio_conf,
};

static struct msm_camera_sensor_info msm_camera_sensor_s5k4ecgx_data = {
	.sensor_name	= "s5k4ecgx",
	.pdata	= &msm_camera_csi_device_data[0],
	.flash_data	= &flash_s5k4ecgx,
	.sensor_platform_info = &sensor_board_info_s5k4ecgx,
	.csi_if	= 1,
	.camera_type = BACK_CAMERA_2D,
	.sensor_type = YUV_SENSOR,
};
#endif
#ifdef CONFIG_SR352
static struct msm_camera_sensor_flash_data flash_sr352 = {
	.flash_type     = MSM_CAMERA_FLASH_NONE,
};
static struct msm_camera_csi_lane_params sr352_csi_lane_params = {
	.csi_lane_assign = 0xE4,
	.csi_lane_mask = 0x3,
};
static struct msm_camera_sensor_platform_info sensor_board_info_sr352 = {
	.sensor_reset	= GPIO_CAM1_RST_N,
	.mount_angle	= 90,
	.sensor_stby	= GPIO_MAIN_STBY,
	.csi_lane_params = &sr352_csi_lane_params,
	.vt_sensor_reset	= GPIO_CAM2_RST_N,
	.vt_sensor_stby	= GPIO_VT_STBY,
//	.flash_en	= GPIO_MSM_FLASH_CNTL_EN,
//	.flash_set	= GPIO_MSM_FLASH_NOW,
	.mclk	= GPIO_MAIN_CAM_MCLK,
	.sensor_pwd	= GPIO_CAM_CORE_EN,//jixu
	.vcm_pwd	= 0,
	.vcm_enable	= 1,
	.sensor_power_on = cam_ldo_power_on,
	.sensor_power_off = cam_ldo_power_off,
	.cam_vreg = msm_8930_back_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_8930_back_cam_vreg),
	.gpio_conf = &msm_8930_back_cam_gpio_conf,
};
static struct msm_camera_sensor_info msm_camera_sensor_sr352_data = {
	.sensor_name    = "sr352",
	.pdata  = &msm_camera_csi_device_data[0],
	.flash_data     = &flash_sr352,
	.sensor_platform_info = &sensor_board_info_sr352,
	.csi_if = 1,
	.camera_type = BACK_CAMERA_2D,
	.sensor_type = YUV_SENSOR,
};
#endif//jixu

#ifdef CONFIG_SR130PC20
static struct msm_camera_sensor_flash_data flash_sr130pc20 = {
	.flash_type     = MSM_CAMERA_FLASH_NONE,
};

static struct msm_camera_csi_lane_params sr130pc20_csi_lane_params = {
	.csi_lane_assign = 0xE4,
	.csi_lane_mask = 0x1,
};

static struct msm_camera_sensor_platform_info sensor_board_info_sr130pc20 = {
	.mount_angle    = 270,
	.cam_vreg = msm_8930_front_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_8930_front_cam_vreg),
	.gpio_conf = &msm_8930_front_cam_gpio_conf,
	.csi_lane_params = &sr130pc20_csi_lane_params,
	.sensor_reset   = GPIO_CAM1_RST_N,
	.sensor_pwd     = GPIO_CAM_CORE_EN,
	.sensor_stby    = GPIO_MAIN_STBY,
	.vt_sensor_stby	= GPIO_VT_STBY,
	.vt_sensor_reset        = GPIO_CAM2_RST_N,
#if defined(CONFIG_MACH_EXPRESS)
	.mclk   = GPIO_MAIN_CAM_MCLK,
#else
	.mclk   = GPIO_SUB_CAM_MCLK,
#endif
	.sensor_pwd	= GPIO_CAM_CORE_EN,
	.vcm_pwd        = 0,
	.vcm_enable     = 1,
	.sensor_power_on =  cam_ldo_power_on,
	.sensor_power_off = cam_ldo_power_off,
};

static struct msm_camera_sensor_info msm_camera_sensor_sr130pc20_data = {
	.sensor_name    = "sr130pc20",
	.pdata  = &msm_camera_csi_device_data[1],
	.flash_data     = &flash_sr130pc20,
	.sensor_platform_info = &sensor_board_info_sr130pc20,
	.csi_if = 1,
	.camera_type = FRONT_CAMERA_2D,
	.sensor_type = YUV_SENSOR,
};
#endif
#ifdef CONFIG_SR200PC20M
static struct msm_camera_sensor_flash_data flash_sr200pc20m = {
	.flash_type     = MSM_CAMERA_FLASH_NONE,
};

static struct msm_camera_csi_lane_params sr200pc20m_csi_lane_params = {
	.csi_lane_assign = 0xE4,
	.csi_lane_mask = 0x1,
};

static struct msm_camera_sensor_platform_info sensor_board_info_sr200pc20m = {
	.mount_angle    = 270,
	.cam_vreg = msm_8930_front_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_8930_front_cam_vreg),
	.gpio_conf = &msm_8930_front_cam_gpio_conf,
	.csi_lane_params = &sr200pc20m_csi_lane_params,
	.sensor_reset   = GPIO_CAM1_RST_N,
//	.sensor_pwd     = GPIO_CAM_CORE_EN,
#if !defined(CONFIG_MACH_CRATER_CHN_CTC)
	.sensor_stby    = GPIO_MAIN_STBY,
#endif	
	.vt_sensor_stby	= GPIO_VT_STBY,
	.vt_sensor_reset        = GPIO_CAM2_RST_N,
	.mclk   = GPIO_SUB_CAM_MCLK,
//	.sensor_pwd	= GPIO_CAM_CORE_EN,
	.vcm_pwd        = 0,
	.vcm_enable     = 1,
//	.sensor_power_on =  cam_ldo_power_on,
//	.sensor_power_off = cam_ldo_power_off,
};

static struct msm_camera_sensor_info msm_camera_sensor_sr200pc20m_data = {
	.sensor_name    = "sr200pc20m",
	.pdata  = &msm_camera_csi_device_data[1],
	.flash_data     = &flash_sr200pc20m,
	.sensor_platform_info = &sensor_board_info_sr200pc20m,
	.csi_if = 1,
	.camera_type = FRONT_CAMERA_2D,
	.sensor_type = YUV_SENSOR,
};
#endif

#ifdef CONFIG_SR030PC50
static struct msm_camera_sensor_flash_data flash_sr030pc50 = {
	.flash_type     = MSM_CAMERA_FLASH_NONE,
};

static struct msm_camera_csi_lane_params sr030pc50_csi_lane_params = {
	.csi_lane_assign = 0xE4,
	.csi_lane_mask = 0x1,
};

static struct msm_camera_sensor_platform_info sensor_board_info_sr030pc50 = {
	.mount_angle    = 270,
	.cam_vreg = msm_8930_front_cam_vreg,
	.num_vreg = ARRAY_SIZE(msm_8930_front_cam_vreg),
	.gpio_conf = &msm_8930_front_cam_gpio_conf,
	.csi_lane_params = &sr030pc50_csi_lane_params,
	.sensor_reset   = GPIO_CAM1_RST_N,
	.sensor_pwd     = GPIO_CAM_CORE_EN,
	.sensor_stby    = GPIO_MAIN_STBY,
	.vt_sensor_stby	= GPIO_VT_STBY,
	.vt_sensor_reset        = GPIO_CAM2_RST_N,
	.mclk   = GPIO_SUB_CAM_MCLK,
	.sensor_pwd	= GPIO_CAM_CORE_EN,
	.vcm_pwd        = 0,
	.vcm_enable     = 1,
	.sensor_power_on =  cam_ldo_power_on,
	.sensor_power_off = cam_ldo_power_off,
};

static struct msm_camera_sensor_info msm_camera_sensor_sr030pc50_data = {
	.sensor_name    = "sr030pc50",
	.pdata  = &msm_camera_csi_device_data[1],
	.flash_data     = &flash_sr030pc50,
	.sensor_platform_info = &sensor_board_info_sr030pc50,
	.csi_if = 1,
	.camera_type = FRONT_CAMERA_2D,
	.sensor_type = YUV_SENSOR,
};
#endif


static struct platform_device msm_camera_server = {
	.name = "msm_cam_server",
	.id = 0,
};

static ssize_t back_camera_type_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
#if defined(CONFIG_ISX012)
	char cam_type[] = "ISX012\n";
#elif defined(CONFIG_S5K5CCGX)
	char cam_type[] = "SLSI_S5K5CCGX\n";
#elif defined(CONFIG_S5K4ECGX)
	char cam_type[] = "S5K4ECGX\n";
#elif defined(CONFIG_SR352)
	char cam_type[] = "SR352\n";
#else
	char cam_type[] = "N\n";
#endif

	return snprintf(buf, sizeof(cam_type), "%s", cam_type);
}

static ssize_t front_camera_type_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
#if defined(CONFIG_S5K6A3YX)
	char cam_type[] = "SLSI_S5K6A3YX\n";
#elif defined(CONFIG_SR130PC20)
	char cam_type[] = "SR130PC20\n";
#elif defined(CONFIG_SR200PC20M)
	char cam_type[] = "SR200PC20m\n";
#elif defined(CONFIG_SR030PC50)
	char cam_type[] = "SILICON_SR030PC50\n";
#else
	char cam_type[] = "N\n";
#endif

	return snprintf(buf, sizeof(cam_type), "%s", cam_type);
}
static DEVICE_ATTR(rear_camtype, S_IRUGO, back_camera_type_show, NULL);
static DEVICE_ATTR(front_camtype, S_IRUGO, front_camera_type_show, NULL);

static ssize_t back_camera_firmware_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
#if defined(CONFIG_ISX012)
	char cam_fw[] = "ISX012\n";
#elif defined(CONFIG_S5K5CCGX)
	char cam_fw[] = "S5K5CCGX\n";
#elif defined(CONFIG_S5K4ECGX)
	char cam_fw[] = "S5K4ECGX\n";
#elif defined(CONFIG_SR352)
	char cam_fw[] = "SR352\n";
#else
	char cam_fw[] = "N\n";
#endif

	return snprintf(buf, sizeof(cam_fw), "%s", cam_fw);

}

static ssize_t front_camera_firmware_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
#if defined(CONFIG_S5K6A3YX)
	char cam_fw[] = "S5K6A3YX S5K6A3YX\n";
#elif defined(CONFIG_SR130PC20)
	char cam_fw[] = "SR130PC20\n";
#elif defined(CONFIG_SR200PC20M)
	char cam_fw[] = "SR200PC20M\n";
#elif defined(CONFIG_SR030PC50)
	char cam_fw[] = "SR030PC50\n";
#else
	char cam_fw[] = "N\n";
#endif

	return snprintf(buf, sizeof(cam_fw), "%s", cam_fw);

}
static DEVICE_ATTR(rear_camfw, 0664, back_camera_firmware_show, NULL);
static DEVICE_ATTR(front_camfw, 0664, front_camera_firmware_show, NULL);

static ssize_t back_camera_vendorid_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
#if defined(CONFIG_ISX012)
	char cam_vendorid[] = "0x0a01\n";
	return snprintf(buf, sizeof(cam_vendorid), "%s", cam_vendorid);
#elif defined(CONFIG_S5K4ECGX)
	static int gVendorID = 0;
	gVendorID = s5k4ecgx_get_sensor_vendorid();
	return sprintf(buf, "0x0%02x\n", gVendorID);
#else
	char cam_vendorid[] = "N\n";
	return snprintf(buf, sizeof(cam_vendorid), "%s", cam_vendorid);
#endif

}
static DEVICE_ATTR(rear_vendorid, 0664, back_camera_vendorid_show, NULL);

#if defined(CONFIG_ISX012)
u8 torchonoff;
static u8 gpio_flash_en;
static u8 gpio_flash_set;
#endif

#if defined(CONFIG_LEDS_RT8547) 
u8 torchonoff;
#endif

#if defined(CONFIG_IMX175) || defined(CONFIG_ISX012)
bool Torch_On;
#if defined(CONFIG_MACH_MELIUS) || defined(CONFIG_MACH_GOLDEN)
/* KTD2692 : command time delay(us) */
#define T_DS		15	//	12
#define T_EOD_H		1000 //	350
#define T_EOD_L		4
#define T_H_LB		4
#define T_L_LB		3*T_H_LB
#define T_L_HB		4
#define T_H_HB		3*T_L_HB
#define T_RESET		800	//	700
/* KTD2692 : command address(A2:A0) */
#define LVP_SETTING		0x0 << 5
#define FLASH_TIMEOUT	0x1 << 5
#define MIN_CURRENT		0x2 << 5
#define MOVIE_CURRENT	0x3 << 5
#define FLASH_CURRENT	0x4 << 5
#define MODE_CONTROL	0x5 << 5

/* FLASH IC : KTD2692*/
static DEFINE_SPINLOCK(flash_ctrl_lock);
static void KTD2692_set_flash(unsigned int ctl_cmd)
{
	int i=0;
	int j = 0;
	int k = 0;
	unsigned long flags;

	spin_lock_irqsave(&flash_ctrl_lock, flags);
	if ( MODE_CONTROL == (MODE_CONTROL & ctl_cmd) )
		k = 8;
	else
		k = 1;

#if defined(CONFIG_MACH_GOLDEN)
	if (ctl_cmd == 0xA0 ) {
		gpio_set_value(GPIO_MSM_FLASH_NOW, 0);
		spin_unlock_irqrestore(&flash_ctrl_lock, flags);
		//udelay(800);
		return;
	}
#endif
	for(j = 0; j < k; j++) {
		cam_err("[cmd::0x%2X][MODE_CONTROL&cmd::0x%2X][k::%d]\n", ctl_cmd, (MODE_CONTROL & ctl_cmd), k);
		gpio_set_value(GPIO_MSM_FLASH_NOW, 1);
		udelay(T_DS);
		for(i = 0; i < 8; i++) {
			if(ctl_cmd & 0x80) { /* set bit to 1 */
				gpio_set_value(GPIO_MSM_FLASH_NOW, 0);
				udelay(T_L_HB);
				gpio_set_value(GPIO_MSM_FLASH_NOW, 1);
				udelay(T_H_HB);
			} else { /* set bit to 0 */
				gpio_set_value(GPIO_MSM_FLASH_NOW, 0);
				udelay(T_L_LB);
				gpio_set_value(GPIO_MSM_FLASH_NOW, 1);
				udelay(T_H_LB);
			}
			ctl_cmd = ctl_cmd << 1;
		}
		gpio_set_value(GPIO_MSM_FLASH_NOW, 0);
		udelay(T_EOD_L);
		gpio_set_value(GPIO_MSM_FLASH_NOW, 1);
		udelay(T_EOD_H);
	}	
	spin_unlock_irqrestore(&flash_ctrl_lock, flags);

#if defined(CONFIG_MACH_MELIUS)
	if (ctl_cmd == 0xA0 ) {
		gpio_set_value(GPIO_MSM_FLASH_NOW, 0);
	}
#endif
}
#if !defined(CONFIG_MACH_GOLDEN)
static void gpio_set_ENM(bool bSet)
{
	gpio_set_value_cansleep(GPIO_MSM_FLASH_NOW, bSet);
}
#endif
#elif defined (CONFIG_MACH_SERRANO) || defined (CONFIG_MACH_CRATER) || defined (CONFIG_MACH_BAFFIN)
static DEFINE_SPINLOCK(flash_ctrl_lock);
static void MIC2871YMK_set_flash(unsigned int ctl_cmd)
{
	int icnt;
	unsigned long flags;
	
	spin_lock_irqsave(&flash_ctrl_lock, flags);

	if (ctl_cmd == 0) {
		cam_err("[teddy][Torch flash]OFF\n");
		gpio_set_value(GPIO_MSM_FLASH_CNTL_EN, 0); 
	} else
	{
		cam_err("[teddy][Torch flash]ON\n");
		gpio_set_value(GPIO_MSM_FLASH_CNTL_EN, 1);
		udelay(1);

		// set address 4
		for (icnt = 0; icnt < 5  ; icnt++)
		{
			gpio_set_value(GPIO_MSM_FLASH_CNTL_EN, 0);
			udelay(1);

			gpio_set_value(GPIO_MSM_FLASH_CNTL_EN, 1);
			udelay(1);
		}
		udelay(110);

		// set data 0 - disable
		for (icnt = 0; icnt < 1; icnt++)
		{
			gpio_set_value(GPIO_MSM_FLASH_CNTL_EN, 0);
			udelay(1);

			gpio_set_value(GPIO_MSM_FLASH_CNTL_EN, 1);
			udelay(1);
		}
		udelay(410);

		// set address 5
		for (icnt = 0; icnt < 6  ; icnt++)
		{
			gpio_set_value(GPIO_MSM_FLASH_CNTL_EN, 0);
			udelay(1);

			gpio_set_value(GPIO_MSM_FLASH_CNTL_EN, 1);
			udelay(1);
		}
		udelay(110);

		// set data 5 - 300mA
		for (icnt = 0; icnt < 6; icnt++)
		{
			gpio_set_value(GPIO_MSM_FLASH_CNTL_EN, 0);
			udelay(1);

			gpio_set_value(GPIO_MSM_FLASH_CNTL_EN, 1);
			udelay(1);
		}
		udelay(410);

		//set disable for Torch mode
		// set address 2
		for (icnt = 0; icnt < 3  ; icnt++)
		{	
			gpio_set_value(GPIO_MSM_FLASH_CNTL_EN, 0);
			udelay(1);

			gpio_set_value(GPIO_MSM_FLASH_CNTL_EN, 1);
			udelay(1);
		}
		udelay(110);

		// set data 21 - Torch 56%
		for (icnt = 0; icnt < 22; icnt++)
		{	
			gpio_set_value(GPIO_MSM_FLASH_CNTL_EN, 0);
			udelay(1);

			gpio_set_value(GPIO_MSM_FLASH_CNTL_EN, 1);
			udelay(1);
		}
		udelay(410);
	}

	spin_unlock_irqrestore(&flash_ctrl_lock, flags);
}
#endif
#endif
static ssize_t cameraflash_file_cmd_store(struct device *dev,
			struct device_attribute *attr,
			const char *buf, size_t size)
{
	int value;
	int err = 1;
	/*int flash_rev = 0;*/

	/*flash_rev = get_flash_led_unlock_rev();*/

	if (strlen(buf) > 2)
		return -err;

	if (isdigit(*buf)) {
		err = kstrtoint(buf, 0, &value);
		if (err < 0)
			pr_err("%s, kstrtoint failed.", __func__);
	} else
		return -err;

#if defined(CONFIG_ISX012)
#if defined(CONFIG_MACH_GOLDEN)
	if(system_rev > 0x2) {
		gpio_tlmm_config(GPIO_CFG(GPIO_MSM_FLASH_NOW, 0,
			GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
			GPIO_CFG_ENABLE);
		if (value == 0) {
			cam_err("[Torch flash]OFF\n");
			KTD2692_set_flash(0xA0);
			Torch_On = false;
		} else {
			cam_err("[Torch flash]ON\n");
			KTD2692_set_flash(0x00);
			KTD2692_set_flash(0xA1); /* Movie mode */
			Torch_On = true;
		}
	} else
#endif
{
		gpio_set_value_cansleep(GPIO_FLASH_LED_UNLOCK, value ? 1 : 0);


	if (value == 0) {
		pr_err("[Torch flash]OFF\n");

		gpio_set_value_cansleep(gpio_flash_en, 0);

		gpio_set_value_cansleep(gpio_flash_set, 0);

		torchonoff = 0;

	} else {
		pr_err("[Torch flash]ON\n");
			gpio_set_value_cansleep(
				gpio_flash_en, 1);
			udelay(0);
			gpio_set_value_cansleep(
				gpio_flash_set, 1);
			udelay(1);

		torchonoff = 1;
	}
}
#elif defined(CONFIG_IMX175)
	cam_err("[value::%d]\n", value);
	gpio_tlmm_config(GPIO_CFG(GPIO_MSM_FLASH_NOW, 0,
		GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		GPIO_CFG_ENABLE);

#if defined(CONFIG_MACH_MELIUS)
	if (value == 0) {
		cam_err("[Torch flash]OFF\n");
#if defined(CONFIG_MACH_MELIUS_VZW) || defined(CONFIG_MACH_MELIUS_SPR) || defined(CONFIG_MACH_MELIUS_USC)
		if (system_rev < 0x01) {
#else
		if (system_rev < 0x07) {
#endif		
			gpio_set_ENM(false);
		} else {
			KTD2692_set_flash(LVP_SETTING | 0x00);
			KTD2692_set_flash(MODE_CONTROL | 0x00);
		}
		Torch_On = false;
	} else {
		cam_err("[Torch flash]ON\n");
#if defined(CONFIG_MACH_MELIUS_VZW) || defined(CONFIG_MACH_MELIUS_SPR) || defined(CONFIG_MACH_MELIUS_USC)
		if (system_rev < 0x01) {
#else
		if (system_rev < 0x07) {
#endif		
			gpio_set_ENM(true);
		} else {
			KTD2692_set_flash(LVP_SETTING | 0x00);
			KTD2692_set_flash(MOVIE_CURRENT | 0x04);
			KTD2692_set_flash(MODE_CONTROL | 0x01);			
		}
		Torch_On = true;
	}
#elif defined(CONFIG_MACH_SERRANO) || defined(CONFIG_MACH_CRATER) || defined (CONFIG_MACH_BAFFIN)
	Torch_On = value;	
	MIC2871YMK_set_flash(value);

#elif defined(CONFIG_MACH_KS02)
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM_FLASH_EN, 0,
		GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM_FLASH_SET, 0,
		GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		GPIO_CFG_ENABLE);

	if (value == 0) {
		printk("[Torch flash]OFF\n");
		gpio_set_value_cansleep(GPIO_CAM_FLASH_SET, 0);
		gpio_set_value_cansleep(GPIO_CAM_FLASH_EN, 0);
		Torch_On = false;


	} else {
		printk("[Torch flash]ON\n");
		gpio_set_value_cansleep(GPIO_CAM_FLASH_EN, 0);
		udelay(0);//1:flash_enable
		gpio_set_value_cansleep(GPIO_CAM_FLASH_SET, 1);
		udelay(1);//1:torch_enable
		Torch_On = true;
	}
#endif
#elif defined(CONFIG_S5K4ECGX) 
#if defined (CONFIG_MACH_CANE) || defined(CONFIG_MACH_LOGANRE)
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM_FLASH_SOURCE_EN, 0,
		GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM_FLASH_SET, 0,
		GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM_FLASH_EN, 0,
		GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		GPIO_CFG_ENABLE);

	if (value == 0) {
		printk("[JYH][Torch flash]OFF\n");
#if defined (CONFIG_LEDS_RT8547)
		rt8547_set_led_off();
		torchonoff = 0;
#endif
	} else {
		printk("[JYH][Torch flash]ON\n");
#if defined (CONFIG_LEDS_RT8547)
		rt8547_set_led_low();
		torchonoff = 1;
#endif
	}
#endif
#endif
	return size;
}


static DEVICE_ATTR(rear_flash, S_IRUGO | S_IWUSR | S_IWGRP,
		NULL, cameraflash_file_cmd_store);

void msm8930_cam_create_node(void)
{
	struct device 	*cam_dev_front;
	struct device 	*cam_dev_flash;

	camera_class = class_create(THIS_MODULE, "camera");

	if (IS_ERR(camera_class)) {
		pr_err("Failed to create class(camera)!\n");
		return;
	}

	cam_dev_flash = device_create(camera_class, NULL,
		0, NULL, "flash");
	if (IS_ERR(cam_dev_flash)) {
		pr_err("Failed to create cam_dev_flash device!\n");
		goto OUT9;
	}
	if (device_create_file(cam_dev_flash, &dev_attr_rear_flash) < 0) {
		pr_err("Failed to create device file!(%s)!\n",
			dev_attr_rear_flash.attr.name);
		goto OUT8;
	}

	cam_dev_back = device_create(camera_class, NULL,
		1, NULL, "rear");
	if (IS_ERR(cam_dev_back)) {
		pr_err("Failed to create cam_dev_back device!\n");
		goto OUT7;
	}
	if (device_create_file(cam_dev_back, &dev_attr_rear_vendorid) < 0) {
		pr_err("Failed to create device file!(%s)!\n",
			dev_attr_rear_vendorid.attr.name);
		goto OUT6;
	}
	if (device_create_file(cam_dev_back, &dev_attr_rear_camtype) < 0) {
		pr_err("Failed to create device file!(%s)!\n",
			dev_attr_rear_camtype.attr.name);
		goto OUT5;
	}
	if (device_create_file(cam_dev_back, &dev_attr_rear_camfw) < 0) {
		pr_err("Failed to create device file!(%s)!\n",
			dev_attr_rear_camfw.attr.name);
		goto OUT4;
	}
	cam_dev_front = device_create(camera_class, NULL,
		2, NULL, "front");
	if (IS_ERR(cam_dev_front)) {
		pr_err("Failed to create cam_dev_front device!");
		goto OUT3;
	}

	if (device_create_file(cam_dev_front, &dev_attr_front_camtype) < 0) {
		pr_err("Failed to create device file!(%s)!\n",
			dev_attr_front_camtype.attr.name);
		goto OUT2;
	}
	if (device_create_file(cam_dev_front, &dev_attr_front_camfw) < 0) {
		pr_err("Failed to create device file!(%s)!\n",
			dev_attr_front_camfw.attr.name);
		goto OUT1;
	}

	return;

OUT1:
	printk(KERN_ERR "[CAMERA_DEV]OUT1\n");
	device_remove_file(cam_dev_front, &dev_attr_front_camtype);
OUT2:
	printk(KERN_ERR "[CAMERA_DEV]OUT2\n");
	device_destroy(camera_class, 2);
OUT3:
	printk(KERN_ERR "[CAMERA_DEV]OUT3\n");
	device_remove_file(cam_dev_front, &dev_attr_front_camfw);
OUT4:
	printk(KERN_ERR "[CAMERA_DEV]OUT4\n");
	device_remove_file(cam_dev_back, &dev_attr_rear_camtype);
OUT5:
	printk(KERN_ERR "[CAMERA_DEV]OUT5\n");
	device_remove_file(cam_dev_back, &dev_attr_rear_vendorid);
OUT6:
	printk(KERN_ERR "[CAMERA_DEV]OUT6\n");
	device_destroy(camera_class, 1);
OUT7:
	printk(KERN_ERR "[CAMERA_DEV]OUT7\n");
	device_remove_file(cam_dev_flash, &dev_attr_rear_flash);
OUT8:
	printk(KERN_ERR "[CAMERA_DEV]OUT8\n");
	device_destroy(camera_class, 0);
OUT9:
	printk(KERN_ERR "[CAMERA_DEV]OUT9\n");
	return;
}

void __init msm8930_init_cam(void)
{
#if defined(CONFIG_ISX012) || defined(CONFIG_S5K5CCGX) || defined(CONFIG_S5K4ECGX) || defined(CONFIG_SR352)
	struct msm_camera_sensor_info *s_info;
#endif
	cam_err("Start\n");

	msm8930_cam_create_node();

	msm_gpiomux_install(msm8930_cam_common_configs,
			ARRAY_SIZE(msm8930_cam_common_configs));

#if defined(CONFIG_S5K3L1YX)
	if (machine_is_msm8930_cdp()) {
		s_info = &msm_camera_sensor_s5k3l1yx_data;
		s_info->sensor_platform_info->mount_angle = 0;	
	}
#endif

	/************************************************
	 * EXTERNAL GPIO config
	 ************************************************/
	msm_camera_gpio_install();

#if defined(CONFIG_ISX012)
	s_info = &msm_camera_sensor_isx012_data;

	s_info->sensor_platform_info->flash_en =
		GPIO_MSM_FLASH_NOW;
	s_info->sensor_platform_info->flash_set =
		GPIO_FLASH_LED_UNLOCK;

	gpio_flash_en = s_info->sensor_platform_info->flash_en;
	gpio_flash_set = s_info->sensor_platform_info->flash_set;
#endif
#if defined(CONFIG_MACH_MELIUS)
#if defined(CONFIG_MACH_MELIUS_SKT) || defined(CONFIG_MACH_MELIUS_KTT) || \
	defined(CONFIG_MACH_MELIUS_LGT)
	gpio_tlmm_config(GPIO_CFG(12, 1, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(13, 1, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
#else
	gpio_tlmm_config(GPIO_CFG(40, 1, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(41, 1, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
#endif
#endif
#if defined(CONFIG_S5K5CCGX)
	s_info = &msm_camera_sensor_s5k5ccgx_data;
#endif
#if defined(CONFIG_SR352)
		s_info = &msm_camera_sensor_sr352_data;
#endif
#if defined(CONFIG_S5K4ECGX)
	s_info = &msm_camera_sensor_s5k4ecgx_data;

	s_info->sensor_platform_info->flash_en =
		GPIO_CAM_FLASH_EN;
	s_info->sensor_platform_info->flash_set =
		GPIO_CAM_FLASH_SET;
#endif
	platform_device_register(&msm_camera_server);
	platform_device_register(&msm8960_device_csiphy0);
	platform_device_register(&msm8960_device_csiphy1);
	platform_device_register(&msm8960_device_csid0);
	platform_device_register(&msm8960_device_csid1);
	platform_device_register(&msm8960_device_ispif);
	platform_device_register(&msm8960_device_vfe);
	platform_device_register(&msm8960_device_vpe);
}

#ifdef CONFIG_I2C
struct i2c_board_info msm8930_camera_i2c_boardinfo[] = {
#if defined(CONFIG_S5K3H5XA)
		{
		I2C_BOARD_INFO("s5k3h5xa", 0x2D),
 		.platform_data = &msm_camera_sensor_s5k3h5xa_data,
		},
#endif
#if defined(CONFIG_IMX175)
	{
	I2C_BOARD_INFO("imx175", 0x1A),
	.platform_data = &msm_camera_sensor_imx175_data,
	},
#endif
#ifdef CONFIG_S5K6A3YX
	{
		I2C_BOARD_INFO("s5k6a3yx", 0x10),
		.platform_data = &msm_camera_sensor_s5k6a3yx_data,
	},
#endif
#ifdef CONFIG_SR352
	{
		I2C_BOARD_INFO("sr352",0x20),
		.platform_data = &msm_camera_sensor_sr352_data,
	},//jixu
#endif
#if defined(CONFIG_SR130PC20) 
#if defined(CONFIG_MACH_LT02) || defined(CONFIG_MACH_LT02_CHN_CTC)
	{
		I2C_BOARD_INFO("sr130pc20", 0x28),
		.platform_data = &msm_camera_sensor_sr130pc20_data,
	},
#else
	{
		I2C_BOARD_INFO("sr130pc20", 0x20),
		.platform_data = &msm_camera_sensor_sr130pc20_data,
	},
#endif
#endif
#ifdef CONFIG_SR200PC20M
	{
		I2C_BOARD_INFO("sr200pc20m", 0x20),
		.platform_data = &msm_camera_sensor_sr200pc20m_data,
	},
#endif
#ifdef CONFIG_SR030PC50
	{
		I2C_BOARD_INFO("sr030pc50", 0x30),
		.platform_data = &msm_camera_sensor_sr030pc50_data,
	},
#endif
#ifdef CONFIG_ISX012
	{
		I2C_BOARD_INFO("isx012", 0x3D),
		.platform_data = &msm_camera_sensor_isx012_data,
	},
#endif
#ifdef CONFIG_S5K5CCGX
	{
	I2C_BOARD_INFO("s5k5ccgx", 0x5A>>1),
	.platform_data = &msm_camera_sensor_s5k5ccgx_data,
	},
#endif
#ifdef CONFIG_S5K4ECGX
	{
		I2C_BOARD_INFO("s5k4ecgx", 0xAC >> 1),
		.platform_data = &msm_camera_sensor_s5k4ecgx_data,
	},
#endif
#if defined(CONFIG_S5K3L1YX)
	{
	I2C_BOARD_INFO("imx074", 0x1A),
	.platform_data = &msm_camera_sensor_imx074_data,
	},
	{
	I2C_BOARD_INFO("ov2720", 0x6C),
	.platform_data = &msm_camera_sensor_ov2720_data,
	},
	{
	I2C_BOARD_INFO("mt9m114", 0x48),
	.platform_data = &msm_camera_sensor_mt9m114_data,
	},
	{
	I2C_BOARD_INFO("s5k3l1yx", 0x20),
	.platform_data = &msm_camera_sensor_s5k3l1yx_data,
	},
#ifdef CONFIG_MSM_CAMERA_FLASH_SC628A
	{
	I2C_BOARD_INFO("sc628a", 0x6E),
	},
	{
	},
#endif
#endif
};

struct msm_camera_board_info msm8930_camera_board_info = {
	.board_info = msm8930_camera_i2c_boardinfo,
	.num_i2c_board_info = ARRAY_SIZE(msm8930_camera_i2c_boardinfo),
};
#if defined(CONFIG_MACH_CRATER_CHN_CTC)
struct i2c_board_info msm8930_camera_sr200pc20m_i2c_board_info[] = {
	{
		I2C_BOARD_INFO("sr200pc20m", 0x20),
		.platform_data = &msm_camera_sensor_sr200pc20m_data,
	},
};

struct msm_camera_board_info msm8930_camera_sr200pc20m_board_info = {
	.board_info = msm8930_camera_sr200pc20m_i2c_board_info,
	.num_i2c_board_info = ARRAY_SIZE(msm8930_camera_sr200pc20m_i2c_board_info),
};
#endif

#endif
#endif
