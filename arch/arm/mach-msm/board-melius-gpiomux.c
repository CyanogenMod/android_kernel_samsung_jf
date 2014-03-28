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

#include <linux/gpio.h>
#include <asm/mach-types.h>
#include <mach/gpiomux.h>
#include <mach/socinfo.h>
#include <mach/msm8930-gpio.h>
#include <asm/system_info.h>
#include "devices.h"
#include "board-8930.h"

#ifdef CONFIG_SAMSUNG_CMC624
#include <linux/i2c/samsung_cmc624.h>
#endif
/* The SPI configurations apply to GSBI 1*/
static struct gpiomux_setting spi_active = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting spi_suspended_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting gsbi1 = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_NONE,
};

#if defined(CONFIG_MACH_MELIUS_CHN_CTC) || defined(CONFIG_MACH_CRATER_CHN_CTC)
static struct gpiomux_setting gsbi2 = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};
#endif

#ifdef CONFIG_2MIC_ES305
#ifdef CONFIG_2MIC_QUP_I2C
static struct gpiomux_setting a2220_gsbi5 = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_NONE,
};
#endif
static struct gpiomux_setting audience_suspend_gpio_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};
#endif

static struct gpiomux_setting gsbi3_suspended_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gsbi3_active_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

#if defined(CONFIG_GSM_MODEM_SPRD6500)
static struct gpiomux_setting gsbi5 = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting gsbi5_suspend_cs = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
};
#else
static struct gpiomux_setting gsbi5 = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};
#endif

static struct gpiomux_setting gsbi9 = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting gsbi10 = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

#if defined(CONFIG_GSM_MODEM_SPRD6500)
static struct gpiomux_setting gsbi12 = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};
#else
static struct gpiomux_setting gsbi12 = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};
#endif

#ifdef CONFIG_SLIMBUS_MSM_CTRL
static struct gpiomux_setting cdc_mclk = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting audio_mbhc = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};
#endif

#ifdef CONFIG_AUXPCM_INTERFACE
static struct gpiomux_setting audio_auxpcm[] = {
	/* Suspended state */
	{
		.func = GPIOMUX_FUNC_GPIO,
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_DOWN,
	},
	/* Active state */
	{
		.func = GPIOMUX_FUNC_1,
		.drv = GPIOMUX_DRV_2MA,
		.pull = GPIOMUX_PULL_NONE,
	},
};
#endif

static struct gpiomux_setting audio_spkr_boost = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

#if defined(CONFIG_KS8851) || defined(CONFIG_KS8851_MODULE)
static struct gpiomux_setting gpio_eth_config = {
	.pull = GPIOMUX_PULL_NONE,
	.drv = GPIOMUX_DRV_8MA,
	.func = GPIOMUX_FUNC_GPIO,
};
#endif

#ifdef CONFIG_SLIMBUS_MSM_CTRL
static struct gpiomux_setting slimbus = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_KEEPER,
};
#endif

static struct gpiomux_setting wcnss_5wire_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv  = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting wcnss_5wire_active_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv  = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_DOWN,
};

#if !defined(CONFIG_MACH_MELIUS_SKT) && !defined(CONFIG_MACH_MELIUS_KTT) && \
	!defined(CONFIG_MACH_MELIUS_LGT) && !defined(CONFIG_MACH_MELIUS_CHN_CTC) && \
    !defined(CONFIG_MACH_MELIUS_EUR_OPEN) && !defined(CONFIG_MACH_MELIUS_EUR_LTE) && !defined(CONFIG_MACH_CRATER_CHN_CTC)
#ifndef CONFIG_RADIO_USE_MI2S
static struct gpiomux_setting atmel_resout_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting atmel_resout_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_UP,
};
#endif
static struct gpiomux_setting atmel_ldo_en_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting atmel_ldo_en_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_6MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting atmel_int_act_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting atmel_int_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
#endif

#ifdef MSM8930_PHASE_2
static struct gpiomux_setting hsusb_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_OUT_LOW,
};
static struct msm_gpiomux_config msm8930_hsusb_configs[] = {
	{
		.gpio = 63,     /* HSUSB_EXTERNAL_5V_LDO_EN */
		.settings = {
			[GPIOMUX_SUSPENDED] = &hsusb_sus_cfg,
		},
	},
	{
		.gpio = 97,     /* HSUSB_5V_EN */
		.settings = {
			[GPIOMUX_SUSPENDED] = &hsusb_sus_cfg,
		},
	},
};
#endif

#if !defined(CONFIG_RADIO_USE_MI2S) && !defined(CONFIG_TDMB)
static struct gpiomux_setting hap_lvl_shft_suspended_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting hap_lvl_shft_active_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};
#endif
static struct gpiomux_setting ap2mdm_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting mdm2ap_status_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting mdm2ap_errfatal_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_16MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting ap2mdm_kpdpwr_n_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting mdp_vsync_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting mdp_vsync_active_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

#ifdef CONFIG_FB_MSM_HDMI_MSM_PANEL
static struct gpiomux_setting hdmi_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting hdmi_active_1_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct gpiomux_setting hdmi_active_2_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
#if 0
static struct gpiomux_setting hdmi_active_3_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_IN,
};

static struct gpiomux_setting hdmi_active_4_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_OUT_HIGH,
};

static struct gpiomux_setting hdmi_active_5_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_OUT_HIGH,
};
#endif
#ifdef CONFIG_RADIO_USE_MI2S
static struct gpiomux_setting  mi2s_act_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting  mi2s_sus_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting  mi2s_act_ws_cfg = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};
#endif
#endif

#if defined(CONFIG_VIDEO_MHL_V2)
static struct gpiomux_setting mhl_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting mhl_active_1_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_OUT_LOW,
};
#endif

#if defined(CONFIG_KS8851) || defined(CONFIG_KS8851_MODULE)
static struct msm_gpiomux_config msm8960_ethernet_configs[] = {
	{
		.gpio = 90,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_eth_config,
		}
	},
	{
		.gpio = 89,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_eth_config,
		}
	},
};
#endif

static struct msm_gpiomux_config msm8960_gsbi_configs[] __initdata = {
	{
		.gpio      = 6,		/* GSBI1 QUP SPI_DATA_MOSI */
		.settings = {
			[GPIOMUX_SUSPENDED] = &spi_suspended_config,
			[GPIOMUX_ACTIVE] = &spi_active,
		},
	},
	{
		.gpio      = 7,		/* GSBI1 QUP SPI_DATA_MISO */
		.settings = {
			[GPIOMUX_SUSPENDED] = &spi_suspended_config,
			[GPIOMUX_ACTIVE] = &spi_active,
		},
	},
	{
		.gpio      = 36,		/* GSBI1 QUP SPI_CS_N */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi1,
			[GPIOMUX_ACTIVE] = &gsbi1,
		},
	},
	{
		.gpio      = 37,		/* GSBI1 QUP SPI_CLK */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi1,
			[GPIOMUX_ACTIVE] = &gsbi1,
		},
	},
#ifdef CONFIG_2MIC_QUP_I2C
	{
		.gpio      = GPIO_2MIC_I2C_SDA,		/* GSBIX QUP 2MIC_SDA */
		.settings = {
			[GPIOMUX_SUSPENDED] = &a2220_gsbi5,
			[GPIOMUX_ACTIVE] = &a2220_gsbi5,
		},
	},
	{
		.gpio      = GPIO_2MIC_I2C_SCL,		/* GSBIX QUP 2MIC_SCL */
		.settings = {
			[GPIOMUX_SUSPENDED] = &a2220_gsbi5,
			[GPIOMUX_ACTIVE] = &a2220_gsbi5,
		},
	},
#endif

#if !defined(CONFIG_MACH_MELIUS_CHN_CTC)	
	{
		.gpio      = 8,		/* GSBI1 QUP SPI_CS_N */
		.settings = {
			[GPIOMUX_SUSPENDED] = &spi_suspended_config,
			[GPIOMUX_ACTIVE] = &spi_active,
		},
	},
	{
		.gpio      = 9,		/* GSBI1 QUP SPI_CLK */
		.settings = {
			[GPIOMUX_SUSPENDED] = &spi_suspended_config,
			[GPIOMUX_ACTIVE] = &spi_active,
		},
	},
#endif

#if defined(CONFIG_MACH_MELIUS_SKT) || defined(CONFIG_MACH_MELIUS_KTT) || \
	defined(CONFIG_MACH_MELIUS_LGT)
	{
		.gpio	   = 10,	/* GSBI2 QUP SPI_DATA_MOSI*/
		.settings = {
			[GPIOMUX_SUSPENDED] = &spi_suspended_config,
			[GPIOMUX_ACTIVE] = &spi_active,
		},
	},
	{
		.gpio	   = 11,	/* GSBI2 QUP SPI_DATA_MISO*/
		.settings = {
			[GPIOMUX_SUSPENDED] = &spi_suspended_config,
			[GPIOMUX_ACTIVE] = &spi_active,
		},
	},
	{
		.gpio	   = 12,	/* GSBI2 QUP SPI_CS_N*/
		.settings = {
			[GPIOMUX_SUSPENDED] = &spi_suspended_config,
			[GPIOMUX_ACTIVE] = &spi_active,
		},
	},
	{
		.gpio	   = 13,	/* GSBI2 QUP SPI_CLK*/
		.settings = {
			[GPIOMUX_SUSPENDED] = &spi_suspended_config,
			[GPIOMUX_ACTIVE] = &spi_active,
		},
	},
#endif
#if defined(CONFIG_MACH_MELIUS_CHN_CTC) || defined(CONFIG_MACH_CRATER_CHN_CTC)
	{
		.gpio      = 10,	/* GSBI5 UART2 */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi2,
		},
	},
	{
		.gpio      = 11,	/* GSBI5 UART2 */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi2,
		},
	},
#endif
	{
		.gpio      = 16,	/* GSBI3 I2C QUP SDA */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi3_suspended_cfg,
			[GPIOMUX_ACTIVE] = &gsbi3_active_cfg,
		},
	},
	{
		.gpio      = 17,	/* GSBI3 I2C QUP SCL */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi3_suspended_cfg,
			[GPIOMUX_ACTIVE] = &gsbi3_active_cfg,
		},
	},
#if defined(CONFIG_GSM_MODEM_SPRD6500)
	{
		.gpio = /* 22 */GPIO_GSM_SPI_MOSI,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi5,
		}
	},
	{
		.gpio =  /* 23 */GPIO_GSM_SPI_MISO,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi5,
		}
	},
	{
		.gpio =  /* 24 */GPIO_GSM_SPI_CS_N,
		.settings = {
			[GPIOMUX_ACTIVE] = &gsbi5,
			[GPIOMUX_SUSPENDED] = &gsbi5_suspend_cs,
		}
	},
	{
		.gpio =  /* 25 */GPIO_GSM_SPI_CLK,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi5,
		}
	},
#else
	{
		.gpio      = 22,	/* GSBI5 UART2 */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi5,
		},
	},
	{
		.gpio      = 23,	/* GSBI5 UART2 */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi5,
		},
	},
#endif

#if defined(CONFIG_GSM_MODEM_SPRD6500)
	{
		.gpio = /* 42 */GPIO_SDL_TXD,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi12,
		}
	},
	{
		.gpio =  /* 43 */GPIO_SDL_RXD,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi12,
		}
	},
	{
		.gpio =  /* 44 */GPIO_SDL_CTS,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi12,
		}
	},
	{
		.gpio =  /* 45 */GPIO_SDL_RTS,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi12,
		}
	},
#else
	{
		.gpio      = 44,	/* GSBI12 I2C QUP SDA */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi12,
		},
	},
	{
		.gpio      = 45,	/* GSBI12 I2C QUP SCL */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi12,
		},
	},
#endif

	{
		.gpio      = 95,	/* GSBI9 I2C QUP SDA */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi9,
		},
	},
	{
		.gpio      = 96,	/* GSBI12 I2C QUP SCL */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi9,
		},
	},

	{
		.gpio      = 73,	/* GSBI10 I2C QUP SDA */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi10,
		},
	},
	{
		.gpio      = 74,	/* GSBI10 I2C QUP SCL */
		.settings = {
			[GPIOMUX_SUSPENDED] = &gsbi10,
		},
	},
	{
		.gpio	   = 38,	/* GSBI11 QUP SPI_DATA_MOSI*/
		.settings = {
			[GPIOMUX_SUSPENDED] = &spi_suspended_config,
			[GPIOMUX_ACTIVE] = &spi_active,
		},
	},
	{
		.gpio	   = 39,	/* GSBI11 QUP SPI_DATA_MISO*/
		.settings = {
			[GPIOMUX_SUSPENDED] = &spi_suspended_config,
			[GPIOMUX_ACTIVE] = &spi_active,
		},
	},
	{
		.gpio	   = 40,	/* GSBI11 QUP SPI_CS_N*/
		.settings = {
			[GPIOMUX_SUSPENDED] = &spi_suspended_config,
			[GPIOMUX_ACTIVE] = &spi_active,
		},
	},
	{
		.gpio	   = 41,	/* GSBI11 QUP SPI_CLK*/
		.settings = {
			[GPIOMUX_SUSPENDED] = &spi_suspended_config,
			[GPIOMUX_ACTIVE] = &spi_active,
		},
	},
};
#ifdef CONFIG_RADIO_USE_MI2S
static struct msm_gpiomux_config msm8960_mi2s_configs[] __initdata = {
	{
		.gpio	= 47,		/* mi2s ws */
		.settings = {
			[GPIOMUX_SUSPENDED] = &mi2s_sus_cfg,
			[GPIOMUX_ACTIVE] = &mi2s_act_ws_cfg,
		},
	},
	{
		.gpio	= 48,		/* mi2s sclk */
		.settings = {
			[GPIOMUX_SUSPENDED] = &mi2s_sus_cfg,
			[GPIOMUX_ACTIVE] = &mi2s_act_cfg,
		},
	},
	{
		.gpio	= 52,		/* mi2s dout0 */
		.settings = {
			[GPIOMUX_SUSPENDED] = &mi2s_sus_cfg,
			[GPIOMUX_ACTIVE] = &mi2s_act_ws_cfg,
		},
	},
		{
		.gpio	= 66,		/* codec mclk */
		.settings = {
			[GPIOMUX_SUSPENDED] = &mi2s_sus_cfg,
			[GPIOMUX_ACTIVE] = &mi2s_act_cfg,
		},
	},
	
};
#endif
#ifdef CONFIG_SLIMBUS_MSM_CTRL
static struct msm_gpiomux_config msm8960_slimbus_config[] __initdata = {
	{
		.gpio	= 60,		/* slimbus data */
		.settings = {
			[GPIOMUX_SUSPENDED] = &slimbus,
		},
	},
	{
		.gpio	= 61,		/* slimbus clk */
		.settings = {
			[GPIOMUX_SUSPENDED] = &slimbus,
		},
	},
};

static struct msm_gpiomux_config msm8960_audio_codec_configs[] __initdata = {
	{
		.gpio = 59,
		.settings = {
			[GPIOMUX_SUSPENDED] = &cdc_mclk,
		},
	},
};

static struct msm_gpiomux_config msm8960_audio_mbhc_configs[] __initdata = {
	{
		.gpio = 37,
		.settings = {
			[GPIOMUX_SUSPENDED] = &audio_mbhc,
		},
	},
};

#else
#ifdef CONFIG_MACH_MELIUS
static struct gpiomux_setting active_cdc_i2s_mclk = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_4MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting active_cdc_i2s_rx_sck = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_4MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting active_cdc_i2s_rx_dout = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_4MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting active_cdc_i2s_rx_ws = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_4MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting suspend_cdc_i2s_mclk = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_4MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting suspend_cdc_i2s_rx_sck = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_4MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting suspend_cdc_i2s_rx_dout = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_4MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting suspend_cdc_i2s_rx_ws = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_4MA,
	.pull = GPIOMUX_PULL_DOWN,
};
#else
static struct gpiomux_setting active_cdc_i2s_mclk = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting active_cdc_i2s_rx_sck = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting active_cdc_i2s_rx_dout = {
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting active_cdc_i2s_rx_ws = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting suspend_cdc_i2s_mclk = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting suspend_cdc_i2s_rx_sck = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting suspend_cdc_i2s_rx_dout = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting suspend_cdc_i2s_rx_ws = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
};
#endif
static struct msm_gpiomux_config msm8960_audio_i2s_rx_codec_configs_rev10[] = {
	{
		.gpio = GPIO_AUDIO_MCLK_REV10,
		.settings = {
			[GPIOMUX_ACTIVE] = &active_cdc_i2s_mclk,
			[GPIOMUX_SUSPENDED] = &suspend_cdc_i2s_mclk,
		},
	},
	{
		.gpio = GPIO_SPKR_I2S_RX_SCK,
		.settings = {
			[GPIOMUX_ACTIVE] = &active_cdc_i2s_rx_sck,
			[GPIOMUX_SUSPENDED] = &suspend_cdc_i2s_rx_sck,
		},
	},
	{
		.gpio = GPIO_SPKR_I2S_RX_DOUT,
		.settings = {
			[GPIOMUX_ACTIVE] = &active_cdc_i2s_rx_dout,
			[GPIOMUX_SUSPENDED] = &suspend_cdc_i2s_rx_dout,
		},
	},
	{
		.gpio = GPIO_SPKR_I2S_RX_WS,
		.settings = {
			[GPIOMUX_ACTIVE] = &active_cdc_i2s_rx_ws,
			[GPIOMUX_SUSPENDED] = &suspend_cdc_i2s_rx_ws,
		},
	},
};
static struct msm_gpiomux_config msm8960_audio_i2s_rx_codec_configs[] = {
	{
		.gpio = GPIO_AUDIO_MCLK,
		.settings = {
			[GPIOMUX_ACTIVE] = &active_cdc_i2s_mclk,
			[GPIOMUX_SUSPENDED] = &suspend_cdc_i2s_mclk,
		},
	},
	{
		.gpio = GPIO_SPKR_I2S_RX_SCK,
		.settings = {
			[GPIOMUX_ACTIVE] = &active_cdc_i2s_rx_sck,
			[GPIOMUX_SUSPENDED] = &suspend_cdc_i2s_rx_sck,
		},
	},
	{
		.gpio = GPIO_SPKR_I2S_RX_DOUT,
		.settings = {
			[GPIOMUX_ACTIVE] = &active_cdc_i2s_rx_dout,
			[GPIOMUX_SUSPENDED] = &suspend_cdc_i2s_rx_dout,
		},
	},
	{
		.gpio = GPIO_SPKR_I2S_RX_WS,
		.settings = {
			[GPIOMUX_ACTIVE] = &active_cdc_i2s_rx_ws,
			[GPIOMUX_SUSPENDED] = &suspend_cdc_i2s_rx_ws,
		},
	},
};
#ifdef CONFIG_MACH_MELIUS
static struct gpiomux_setting active_cdc_i2s_tx_sck = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_4MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting active_cdc_i2s_tx_din = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_4MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting active_cdc_i2s_tx_ws = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_4MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting suspend_cdc_i2s_tx_sck = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_4MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting suspend_cdc_i2s_tx_din = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_4MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting suspend_cdc_i2s_tx_ws = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_4MA,
	.pull = GPIOMUX_PULL_DOWN,
};
#else
static struct gpiomux_setting active_cdc_i2s_tx_sck = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting active_cdc_i2s_tx_din = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting active_cdc_i2s_tx_ws = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting suspend_cdc_i2s_tx_sck = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting suspend_cdc_i2s_tx_din = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting suspend_cdc_i2s_tx_ws = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_DOWN,
};
#endif
static struct msm_gpiomux_config msm8960_audio_i2s_tx_codec_configs_rev10[] = {
	{
		.gpio = GPIO_AUDIO_MCLK_REV10,
		.settings = {
			[GPIOMUX_ACTIVE] = &active_cdc_i2s_mclk,
			[GPIOMUX_SUSPENDED] = &suspend_cdc_i2s_mclk,
		},
	},
	{
		.gpio = GPIO_SPKR_I2S_TX_SCK,
		.settings = {
			[GPIOMUX_ACTIVE] = &active_cdc_i2s_tx_sck,
			[GPIOMUX_SUSPENDED] = &suspend_cdc_i2s_tx_sck,
		},
	},
	{
		.gpio = GPIO_SPKR_I2S_TX_WS,
		.settings = {
			[GPIOMUX_ACTIVE] = &active_cdc_i2s_tx_ws,
			[GPIOMUX_SUSPENDED] = &suspend_cdc_i2s_tx_ws,
		},
	},
	{
		.gpio = GPIO_SPKR_I2S_TX_DIN,
		.settings = {
			[GPIOMUX_ACTIVE] = &active_cdc_i2s_tx_din,
			[GPIOMUX_SUSPENDED] = &suspend_cdc_i2s_tx_din,
		},
	},
};
static struct msm_gpiomux_config msm8960_audio_i2s_tx_codec_configs[] = {
	{
		.gpio = GPIO_AUDIO_MCLK,
		.settings = {
			[GPIOMUX_ACTIVE] = &active_cdc_i2s_mclk,
			[GPIOMUX_SUSPENDED] = &suspend_cdc_i2s_mclk,
		},
	},
	{
		.gpio = GPIO_SPKR_I2S_TX_SCK,
		.settings = {
			[GPIOMUX_ACTIVE] = &active_cdc_i2s_tx_sck,
			[GPIOMUX_SUSPENDED] = &suspend_cdc_i2s_tx_sck,
		},
	},
	{
		.gpio = GPIO_SPKR_I2S_TX_WS,
		.settings = {
			[GPIOMUX_ACTIVE] = &active_cdc_i2s_tx_ws,
			[GPIOMUX_SUSPENDED] = &suspend_cdc_i2s_tx_ws,
		},
	},
	{
		.gpio = GPIO_SPKR_I2S_TX_DIN,
		.settings = {
			[GPIOMUX_ACTIVE] = &active_cdc_i2s_tx_din,
			[GPIOMUX_SUSPENDED] = &suspend_cdc_i2s_tx_din,
		},
	},

};
#endif

static struct msm_gpiomux_config msm8960_audio_spkr_configs[] __initdata = {
	{
		.gpio = 15,
		.settings = {
			[GPIOMUX_SUSPENDED] = &audio_spkr_boost,
		},
	},
};

#ifdef CONFIG_AUXPCM_INTERFACE
static struct msm_gpiomux_config msm8960_audio_auxpcm_configs[] __initdata = {
	{
		.gpio = 63,
		.settings = {
			[GPIOMUX_SUSPENDED] = &audio_auxpcm[0],
			[GPIOMUX_ACTIVE] = &audio_auxpcm[1],
		},
	},
	{
		.gpio = 64,
		.settings = {
			[GPIOMUX_SUSPENDED] = &audio_auxpcm[0],
			[GPIOMUX_ACTIVE] = &audio_auxpcm[1],
		},
	},
	{
		.gpio = 65,
		.settings = {
			[GPIOMUX_SUSPENDED] = &audio_auxpcm[0],
			[GPIOMUX_ACTIVE] = &audio_auxpcm[1],
		},
	},
	{
		.gpio = 66,
		.settings = {
			[GPIOMUX_SUSPENDED] = &audio_auxpcm[0],
			[GPIOMUX_ACTIVE] = &audio_auxpcm[1],
		},
	},
};
#endif

static struct msm_gpiomux_config wcnss_5wire_interface[] = {
	{
		.gpio = 84,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
	{
		.gpio = 85,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
	{
		.gpio = 86,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
	{
		.gpio = 87,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
	{
		.gpio = 88,
		.settings = {
			[GPIOMUX_ACTIVE]    = &wcnss_5wire_active_cfg,
			[GPIOMUX_SUSPENDED] = &wcnss_5wire_suspend_cfg,
		},
	},
};

#if !defined(CONFIG_MACH_MELIUS_SKT) && !defined(CONFIG_MACH_MELIUS_KTT) && \
	!defined(CONFIG_MACH_MELIUS_LGT) && !defined(CONFIG_MACH_MELIUS_CHN_CTC) && \
    !defined(CONFIG_MACH_MELIUS_EUR_OPEN) && !defined(CONFIG_MACH_MELIUS_EUR_LTE) && !defined(CONFIG_MACH_CRATER_CHN_CTC)
static struct msm_gpiomux_config msm8960_atmel_configs[] __initdata = {
	{	/* TS INTERRUPT */
		.gpio = 11,
		.settings = {
			[GPIOMUX_ACTIVE]    = &atmel_int_act_cfg,
			[GPIOMUX_SUSPENDED] = &atmel_int_sus_cfg,
		},
	},
	{	/* TS LDO ENABLE */
		.gpio = 50,
		.settings = {
			[GPIOMUX_ACTIVE]    = &atmel_ldo_en_act_cfg,
			[GPIOMUX_SUSPENDED] = &atmel_ldo_en_sus_cfg,
		},
	},
#ifndef CONFIG_RADIO_USE_MI2S
	{	/* TS RESOUT */
		.gpio = 52,
		.settings = {
			[GPIOMUX_ACTIVE]    = &atmel_resout_act_cfg,
			[GPIOMUX_SUSPENDED] = &atmel_resout_sus_cfg,
		},
	},
#endif
};
#endif

#if !defined(CONFIG_RADIO_USE_MI2S) && !defined(CONFIG_TDMB)
static struct msm_gpiomux_config hap_lvl_shft_config[] __initdata = {
	{
		.gpio = 47,
		.settings = {
			[GPIOMUX_SUSPENDED] = &hap_lvl_shft_suspended_config,
			[GPIOMUX_ACTIVE] = &hap_lvl_shft_active_config,
		},
	},
};
#endif
static struct gpiomux_setting gpio_keys_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

#if defined(CONFIG_MACH_CRATER_CHN_CTC)
static struct gpiomux_setting gpio_keys_pullup_config = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_UP,
};

static struct msm_gpiomux_config gpio_keys_config_mux[] __initdata = {
	{
		.gpio = GPIO_VOLUME_UP,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_keys_config,
			[GPIOMUX_ACTIVE] = &gpio_keys_pullup_config
		}
	},
	{
		.gpio = GPIO_VOLUME_DOWN,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_keys_config,
			[GPIOMUX_ACTIVE] = &gpio_keys_pullup_config
		}
	},
	{
		.gpio = GPIO_HOME_KEY_REV01,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_keys_config,
			[GPIOMUX_ACTIVE] = &gpio_keys_pullup_config
		},
	},
};
#else
static struct msm_gpiomux_config gpio_keys_config_mux[] __initdata = {
	{
		.gpio = GPIO_VOLUME_UP,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_keys_config,
			[GPIOMUX_ACTIVE] = &gpio_keys_config
		}
	},
	{
		.gpio = GPIO_VOLUME_DOWN,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_keys_config,
			[GPIOMUX_ACTIVE] = &gpio_keys_config
		}
	},
	{
		.gpio = GPIO_HOME_KEY_REV01,
		.settings = {
			[GPIOMUX_SUSPENDED] = &gpio_keys_config,
			[GPIOMUX_ACTIVE] = &gpio_keys_config
		},
	},
};
#endif

static struct msm_gpiomux_config mdm_configs[] __initdata = {
	/* AP2MDM_STATUS */
	{
		.gpio = 94,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ap2mdm_cfg,
		}
	},
	/* MDM2AP_STATUS */
	{
		.gpio = 69,
		.settings = {
			[GPIOMUX_SUSPENDED] = &mdm2ap_status_cfg,
		}
	},
	/* MDM2AP_ERRFATAL */
	{
		.gpio = 70,
		.settings = {
			[GPIOMUX_SUSPENDED] = &mdm2ap_errfatal_cfg,
		}
	},
	/* AP2MDM_ERRFATAL */
	{
		.gpio = 95,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ap2mdm_cfg,
		}
	},
	/* AP2MDM_KPDPWR_N */
	{
		.gpio = 81,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ap2mdm_kpdpwr_n_cfg,
		}
	},
	/* AP2MDM_PMIC_RESET_N */
	{
		.gpio = 80,
		.settings = {
			[GPIOMUX_SUSPENDED] = &ap2mdm_kpdpwr_n_cfg,
		}
	}
};

static struct msm_gpiomux_config msm8960_mdp_vsync_configs[] __initdata = {
	{
		.gpio = 0,
		.settings = {
			[GPIOMUX_ACTIVE]    = &mdp_vsync_active_cfg,
			[GPIOMUX_SUSPENDED] = &mdp_vsync_suspend_cfg,
		},
	}
};

#ifdef CONFIG_FB_MSM_HDMI_MSM_PANEL
static struct msm_gpiomux_config msm8960_hdmi_configs[] __initdata = {
	{
		.gpio = 99,
		.settings = {
			[GPIOMUX_ACTIVE]    = &hdmi_active_1_cfg,
			[GPIOMUX_SUSPENDED] = &hdmi_suspend_cfg,
		},
	},
	{
		.gpio = 100,
		.settings = {
			[GPIOMUX_ACTIVE]    = &hdmi_active_1_cfg,
			[GPIOMUX_SUSPENDED] = &hdmi_suspend_cfg,
		},
	},
	{
		.gpio = 101,
		.settings = {
			[GPIOMUX_ACTIVE]    = &hdmi_active_1_cfg,
			[GPIOMUX_SUSPENDED] = &hdmi_suspend_cfg,
		},
	},
	{
		.gpio = 102,
		.settings = {
			[GPIOMUX_ACTIVE]    = &hdmi_active_2_cfg,
			[GPIOMUX_SUSPENDED] = &hdmi_suspend_cfg,
		},
	},

};
#endif
#if defined(CONFIG_VIDEO_MHL_V2)
static struct msm_gpiomux_config msm8930_mhl_configs[] __initdata = {
	{
		.gpio = GPIO_MHL_RST,
		.settings = {
			[GPIOMUX_ACTIVE]	= &mhl_active_1_cfg,
			[GPIOMUX_SUSPENDED] = &mhl_suspend_cfg,
		},
	},
	{
		.gpio = GPIO_MHL_WAKE_UP,
		.settings = {
			[GPIOMUX_ACTIVE]    = &mhl_active_1_cfg,
			[GPIOMUX_SUSPENDED] = &mhl_suspend_cfg,
		},
	},
	{
		.gpio = GPIO_MHL_SDA,
		.settings = {
			[GPIOMUX_SUSPENDED] = &mhl_suspend_cfg,
		},
	},
	{
		.gpio = GPIO_MHL_SCL,
		.settings = {
			[GPIOMUX_SUSPENDED] = &mhl_suspend_cfg,
		},
	},
};
#endif

#ifdef CONFIG_LEDS_AN30259A
static struct gpiomux_setting leds_active_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting leds_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct msm_gpiomux_config msm8930_leds_configs[] __initdata = {
	{
		.gpio = GPIO_S_LED_I2C_SDA,
		.settings = {
			[GPIOMUX_ACTIVE] = &leds_active_cfg,
			[GPIOMUX_SUSPENDED] = &leds_suspend_cfg,
		},
	},
	{
		.gpio = GPIO_S_LED_I2C_SCL,
		.settings = {
			[GPIOMUX_ACTIVE] = &leds_active_cfg,
			[GPIOMUX_SUSPENDED] = &leds_suspend_cfg,
		},
	},
};
#endif

#if defined(CONFIG_IR_REMOCON_FPGA)
static struct gpiomux_setting fpga_resetn_active_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_OUT_LOW,
};

static struct gpiomux_setting fpga_resetn_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_OUT_LOW,
};

static struct msm_gpiomux_config msm8930_fpga_resetn_configs[] __initdata = {
	{
		.gpio = 68,			/* GPIO_FPGA_RST_N */
		.settings = {
			[GPIOMUX_ACTIVE] = &fpga_resetn_active_cfg,
			[GPIOMUX_SUSPENDED] = &fpga_resetn_suspend_cfg,
		},
	},
};

static struct gpiomux_setting fpga_irq_active_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_IN,
};

static struct gpiomux_setting fpga_irq_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
	.dir = GPIOMUX_IN,
};
static struct msm_gpiomux_config msm8930_fpga_irq_configs[] = {
	{
		.gpio = 89,
		.settings = {
			[GPIOMUX_ACTIVE] = &fpga_irq_active_cfg,
			[GPIOMUX_SUSPENDED] = &fpga_irq_suspend_cfg,
		}
	},
};
#endif

static struct gpiomux_setting sd_det_line = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir = GPIOMUX_IN,
};

static struct msm_gpiomux_config msm8930_sd_det_config[] __initdata = {
	{
		.gpio = GPIO_SD_CARD_DET_N,	/* SD Card Detect Line */
		.settings = {
			[GPIOMUX_SUSPENDED] = &sd_det_line,
			[GPIOMUX_ACTIVE] = &sd_det_line,
		},
	},
};

#ifdef CONFIG_USB_SWITCH_FSA9485
static struct gpiomux_setting fsa9485_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting fsa9485_active_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct msm_gpiomux_config msm8960_fsa9485_configs[] __initdata = {
	{
		.gpio = 14,
		.settings = {
			[GPIOMUX_ACTIVE] = &fsa9485_active_cfg,
			[GPIOMUX_SUSPENDED] = &fsa9485_suspend_cfg,
		},
	},

	{
		.gpio = 73,
		.settings = {
			[GPIOMUX_ACTIVE] = &fsa9485_active_cfg,
			[GPIOMUX_SUSPENDED] = &fsa9485_suspend_cfg,
		},
	},

	{
		.gpio = 74,
		.settings = {
			[GPIOMUX_ACTIVE] = &fsa9485_active_cfg,
			[GPIOMUX_SUSPENDED] = &fsa9485_suspend_cfg,
		},
	},
};
#endif

#ifdef CONFIG_2MIC_ES305
static struct msm_gpiomux_config audience_suspend_configs[] __initdata = {
	{
		.gpio    = GPIO_2MIC_PW_DN,
		.settings = {
			[GPIOMUX_SUSPENDED] = &audience_suspend_gpio_config,
		},
	},
};
#endif

#ifdef CONFIG_SAMSUNG_CMC624
static struct gpiomux_setting cmc624_active_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir  = GPIOMUX_OUT_LOW,
};
static struct gpiomux_setting cmc624_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv  = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir  = GPIOMUX_IN,
};
static struct msm_gpiomux_config msm8x30_cmc624_configs[] __initdata = {
	{
		.gpio      = GPIO_IMA_I2C_SDA,
		.settings = {
			[GPIOMUX_ACTIVE]	= &cmc624_active_cfg,
			[GPIOMUX_SUSPENDED] = &cmc624_suspend_cfg,
		},
	},
	{
		.gpio      = GPIO_IMA_I2C_SCL,
		.settings = {
			[GPIOMUX_ACTIVE]	= &cmc624_active_cfg,
			[GPIOMUX_SUSPENDED] = &cmc624_suspend_cfg,
		},
	},
};
#endif

static struct gpiomux_setting gpio_input_pull_down_active_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_IN,
};
static struct gpiomux_setting gpio_input_pull_down_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv  = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir = GPIOMUX_IN,
};
#if 0
static struct gpiomux_setting gpio_input_pull_up_active_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting gpio_input_pull_up_suspend_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv  = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
#endif

#if defined(CONFIG_MACH_MELIUS_CHN_CTC) 
static struct gpiomux_setting uartsel_active_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
	.dir  = GPIOMUX_OUT_LOW,
};
static struct msm_gpiomux_config msm8x30_uartsel_configs[] __initdata = {
	{
		.gpio	   = GPIO_UART_SEL, 
		.settings = {
			[GPIOMUX_ACTIVE]	= &uartsel_active_cfg,
			[GPIOMUX_SUSPENDED] = &uartsel_active_cfg,
		},
	},
};
#endif

#if defined(CONFIG_MACH_MELIUS_SKT) || defined(CONFIG_MACH_MELIUS_KTT) || \
	defined(CONFIG_MACH_MELIUS_LGT)
static struct msm_gpiomux_config msm8x30_melius_gpio_configs[] __initdata = {
	{
		.gpio	   = 15, //NC
		.settings = {
			[GPIOMUX_ACTIVE]	= &gpio_input_pull_down_active_cfg,
			[GPIOMUX_SUSPENDED] = &gpio_input_pull_down_suspend_cfg,
		},
	},
};
#else
static struct msm_gpiomux_config msm8x30_melius_gpio_configs[] __initdata = {
	{
		.gpio	   = 128, //NC
		.settings = {
			[GPIOMUX_ACTIVE]	= &gpio_input_pull_down_active_cfg,
			[GPIOMUX_SUSPENDED] = &gpio_input_pull_down_suspend_cfg,
		},
	},
	{
		.gpio	   = 141, //NC
		.settings = {
			[GPIOMUX_ACTIVE]	= &gpio_input_pull_down_active_cfg,
			[GPIOMUX_SUSPENDED] = &gpio_input_pull_down_suspend_cfg,
		},
	},
	{
		.gpio	   = 145, //NC
		.settings = {
			[GPIOMUX_ACTIVE]	= &gpio_input_pull_down_active_cfg,
			[GPIOMUX_SUSPENDED] = &gpio_input_pull_down_suspend_cfg,
		},
	},
};

#if defined(CONFIG_MACH_MELIUS_EUR_LTE) || defined(CONFIG_MACH_MELIUS_EUR_OPEN)
static struct msm_gpiomux_config msm8x30_melius_03_gpio_configs[] __initdata = {
	{
		.gpio	   = 15, //NC
		.settings = {
			[GPIOMUX_ACTIVE]	= &gpio_input_pull_down_active_cfg,
			[GPIOMUX_SUSPENDED] = &gpio_input_pull_down_suspend_cfg,
		},
	},
};
#endif

#if defined(CONFIG_MACH_MELIUS_EUR_LTE)
static struct msm_gpiomux_config msm8x30_melius_05_gpio_configs[] __initdata = {
	{
		.gpio	   = 47, //NC
		.settings = {
			[GPIOMUX_ACTIVE]	= &gpio_input_pull_down_active_cfg,
			[GPIOMUX_SUSPENDED] = &gpio_input_pull_down_suspend_cfg,
		},
	},
};
#endif
#endif
#if defined(CONFIG_MACH_MELIUS_CHN_CTC)	

static struct gpiomux_setting sensor_active = {
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting sensor_suspend  = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};


static struct msm_gpiomux_config sensor_config[] = {
{
		.gpio = 8,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sensor_active,
			[GPIOMUX_SUSPENDED] = &sensor_suspend,
		},
	},
	{
		.gpio = 9,
		.settings = {
			[GPIOMUX_ACTIVE]    = &sensor_active,
			[GPIOMUX_SUSPENDED] = &sensor_suspend,
		},
	},
};

#endif

#if defined(CONFIG_MACH_MELIUS_CHN_CTC) || defined(CONFIG_MACH_CRATER_CHN_CTC)
static struct gpiomux_setting simsel_active_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
	.dir  = GPIOMUX_OUT_LOW,
};

static struct msm_gpiomux_config msm8x30_simsel_configs[] __initdata = {
	{
		.gpio	   = GPIO_SIM_SEL,
		.settings = {
			[GPIOMUX_ACTIVE]	= &simsel_active_cfg,
			[GPIOMUX_SUSPENDED] = &gpio_input_pull_down_suspend_cfg,
		},
	},
};
#endif

struct melius_init_sleep_table {
	struct msm_gpiomux_config *ptr;
	int size;
};


#define GPIO_TABLE(_ptr) \
	{.ptr = _ptr, \
	.size = ARRAY_SIZE(_ptr)} \


#define GPIO_TABLE_NULL \
	{.ptr = NULL, \
	.size = 0} \

static struct melius_init_sleep_table melius_init_sleep_table[] = {
	GPIO_TABLE(msm8x30_melius_gpio_configs),
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
#if defined(CONFIG_MACH_MELIUS_EUR_LTE) || defined(CONFIG_MACH_MELIUS_EUR_OPEN)
	GPIO_TABLE(msm8x30_melius_03_gpio_configs),
#else
	GPIO_TABLE_NULL,
#endif
	GPIO_TABLE_NULL,
#if defined(CONFIG_MACH_MELIUS_EUR_LTE)
	GPIO_TABLE(msm8x30_melius_05_gpio_configs),
#else
	GPIO_TABLE_NULL,
#endif
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
};

static void config_melius_gpio_init_sleep_gpio(void)
{

	int i = 0;
	int index = system_rev + 1;

	for (i = 0; i < index; i++) {
		if (melius_init_sleep_table[i].ptr == NULL)
			continue;

		msm_gpiomux_install(melius_init_sleep_table[i].ptr,
			melius_init_sleep_table[i].size);
	}
}
int __init msm8930_init_gpiomux(void)
{
	int rc = msm_gpiomux_init(NR_GPIO_IRQS);
	if (rc) {
		pr_err(KERN_ERR "msm_gpiomux_init failed %d\n", rc);
		return rc;
	}
#if defined(CONFIG_MACH_MELIUS_CHN_CTC)	
		msm_gpiomux_install(sensor_config,
				ARRAY_SIZE(sensor_config));
#endif

#if defined(CONFIG_KS8851) || defined(CONFIG_KS8851_MODULE)
	msm_gpiomux_install(msm8960_ethernet_configs,
			ARRAY_SIZE(msm8960_ethernet_configs));
#endif

	msm_gpiomux_install(msm8960_gsbi_configs,
			ARRAY_SIZE(msm8960_gsbi_configs));
#ifdef CONFIG_RADIO_USE_MI2S			
	msm_gpiomux_install(msm8960_mi2s_configs,
			ARRAY_SIZE(msm8960_mi2s_configs));
#endif
#if !defined(CONFIG_MACH_MELIUS_SKT) && !defined(CONFIG_MACH_MELIUS_KTT) && \
	!defined(CONFIG_MACH_MELIUS_LGT) && !defined(CONFIG_MACH_MELIUS_CHN_CTC) && \
    !defined(CONFIG_MACH_MELIUS_EUR_LTE) && !defined(CONFIG_MACH_MELIUS_EUR_OPEN) && !defined(CONFIG_MACH_CRATER_CHN_CTC) 
	msm_gpiomux_install(msm8960_atmel_configs,
			ARRAY_SIZE(msm8960_atmel_configs));
#endif
#ifdef CONFIG_2MIC_ES305
	msm_gpiomux_install(audience_suspend_configs,
		ARRAY_SIZE(audience_suspend_configs));
#endif

#ifdef CONFIG_SLIMBUS_MSM_CTRL
	msm_gpiomux_install(msm8960_slimbus_config,
			ARRAY_SIZE(msm8960_slimbus_config));

	msm_gpiomux_install(msm8960_audio_codec_configs,
			ARRAY_SIZE(msm8960_audio_codec_configs));

	msm_gpiomux_install(msm8960_audio_mbhc_configs,
			ARRAY_SIZE(msm8960_audio_mbhc_configs));
#else

	if (system_rev < CLK_REVISION) {
	msm_gpiomux_install(msm8960_audio_i2s_rx_codec_configs_rev10,
			ARRAY_SIZE(msm8960_audio_i2s_rx_codec_configs_rev10));

	msm_gpiomux_install(msm8960_audio_i2s_tx_codec_configs_rev10,
			ARRAY_SIZE(msm8960_audio_i2s_tx_codec_configs_rev10));
	} else {
	msm_gpiomux_install(msm8960_audio_i2s_rx_codec_configs,
			ARRAY_SIZE(msm8960_audio_i2s_rx_codec_configs));

	msm_gpiomux_install(msm8960_audio_i2s_tx_codec_configs,
			ARRAY_SIZE(msm8960_audio_i2s_tx_codec_configs));
	}
#endif

	msm_gpiomux_install(msm8960_audio_spkr_configs,
			ARRAY_SIZE(msm8960_audio_spkr_configs));

#ifdef CONFIG_AUXPCM_INTERFACE
	msm_gpiomux_install(msm8960_audio_auxpcm_configs,
			ARRAY_SIZE(msm8960_audio_auxpcm_configs));
#endif

	msm_gpiomux_install(wcnss_5wire_interface,
			ARRAY_SIZE(wcnss_5wire_interface));

	if (machine_is_msm8930_mtp() || machine_is_msm8930_fluid() ||
		machine_is_msm8930_cdp()) {

#if !defined(CONFIG_RADIO_USE_MI2S) && !defined(CONFIG_TDMB)
		msm_gpiomux_install(hap_lvl_shft_config,
			ARRAY_SIZE(hap_lvl_shft_config));
#endif
#ifdef MSM8930_PHASE_2
		msm_gpiomux_install(msm8930_hsusb_configs,
			ARRAY_SIZE(msm8930_hsusb_configs));
#endif
	}

	if (PLATFORM_IS_CHARM25())
		msm_gpiomux_install(mdm_configs,
			ARRAY_SIZE(mdm_configs));

#ifdef CONFIG_FB_MSM_HDMI_MSM_PANEL
	msm_gpiomux_install(msm8960_hdmi_configs,
			ARRAY_SIZE(msm8960_hdmi_configs));
#endif
#if defined(CONFIG_VIDEO_MHL_V2)
		msm_gpiomux_install(msm8930_mhl_configs,
				ARRAY_SIZE(msm8930_mhl_configs));
#endif

	msm_gpiomux_install(msm8960_mdp_vsync_configs,
			ARRAY_SIZE(msm8960_mdp_vsync_configs));
	msm_gpiomux_install(gpio_keys_config_mux,
			ARRAY_SIZE(gpio_keys_config_mux));

#ifdef CONFIG_USB_SWITCH_FSA9485
	msm_gpiomux_install(msm8960_fsa9485_configs,
			ARRAY_SIZE(msm8960_fsa9485_configs));
#endif

#ifdef CONFIG_LEDS_AN30259A
	msm_gpiomux_install(msm8930_leds_configs,
			ARRAY_SIZE(msm8930_leds_configs));
#endif
#if defined(CONFIG_IR_REMOCON_FPGA)
	msm_gpiomux_install(msm8930_fpga_resetn_configs,
			ARRAY_SIZE(msm8930_fpga_resetn_configs));

	msm_gpiomux_install(msm8930_fpga_irq_configs,
			ARRAY_SIZE(msm8930_fpga_irq_configs));
#endif
	msm_gpiomux_install(msm8930_sd_det_config,
			ARRAY_SIZE(msm8930_sd_det_config));
#ifdef CONFIG_SAMSUNG_CMC624
	if (samsung_has_cmc624()) {
		msm_gpiomux_install(msm8x30_cmc624_configs,
		ARRAY_SIZE(msm8x30_cmc624_configs));
	}
#endif

#if defined(CONFIG_MACH_MELIUS_CHN_CTC)
	msm_gpiomux_install(msm8x30_uartsel_configs,
			ARRAY_SIZE(msm8x30_uartsel_configs));
#endif

#if defined(CONFIG_MACH_MELIUS_CHN_CTC) || defined(CONFIG_MACH_CRATER_CHN_CTC)
	msm_gpiomux_install(msm8x30_simsel_configs,
		ARRAY_SIZE(msm8x30_simsel_configs));
#endif

	config_melius_gpio_init_sleep_gpio();
	return 0;
}
