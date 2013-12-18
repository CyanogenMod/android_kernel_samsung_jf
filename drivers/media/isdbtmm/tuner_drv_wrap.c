/*
*
* drivers/media/isdbtmm/tuner_drv_wrap.c
*
* MM Tuner Driver
*
* Copyright (C) (2013, Samsung Electronics)
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation version 2.
*
* This program is distributed "as is" WITHOUT ANY WARRANTY of any
* kind, whether express or implied; without even the implied warranty
* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
*/

/******************************************************************************
 * include
 ******************************************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <linux/mm.h>
#include <asm/uaccess.h>
#include "tuner_drv.h"
#include <asm/system_info.h>
#include "../../../arch/arm/mach-msm/board-8064.h"
#include <linux/barcode_emul.h>
#include <mach/gpio.h>
#include <mach/gpiomux.h>
#include <mach/apq8064-gpio.h>
#include <linux/regulator/consumer.h>

#ifdef TUNER_CONFIG_IRQ_PC_LINUX
#include "../../../i2c-parport-x/i2c-parport.h"
#endif  /* TUNER_CONFIG_IRQ_PC_LINUX */

/******************************************************************************/
/* function                                                                   */
/******************************************************************************/
int tuner_drv_ctl_power( int data );
int tuner_drv_set_interrupt( void );
void tuner_drv_release_interrupt( void );
void tuner_drv_gpio_config_poweron( void );
void tuner_drv_gpio_config_poweroff( void );

/******************************************************************************
 * code area
 ******************************************************************************/
/******************************************************************************
 *    function:   tuner_drv_ctl_power
 *    brief   :   power control of a driver
 *    date    :   2011.08.26
 *    author  :   M.Takahashi(*)
 *
 *    return  :    0                   normal exit
 *            :   -1                   error exit
 *    input   :   data                 setting data
 *    output  :   none
 ******************************************************************************/
int tuner_drv_ctl_power( int data )
{	
	int ret = 0;
	int result = 0;
	static struct regulator *tmm_vreg_l15;
	static struct regulator *tmm_vreg_l27;
	
	/* Get Power ON 1.8V Reulator */
	if ( NULL == tmm_vreg_l15 )
	{
		tmm_vreg_l15 = regulator_get ( NULL, "8921_l15" );
		if ( IS_ERR ( tmm_vreg_l15 )) 
		{
			ERROR_PRINT("could not get 8921_l15, rc = %ld",
						PTR_ERR(tmm_vreg_l15));

			tmm_vreg_l15 = NULL;
			return ret;
		}
		ret = regulator_set_voltage(tmm_vreg_l15, 1800000, 1800000 );
		if ( ret ) 
		{
			ERROR_PRINT("set voltage l15 failed, rc=%d", ret);
			tmm_vreg_l15 = NULL;
			return ret;
		}
	}
	
	/* Get Power ON 1.1V Reulator */
	if ( NULL == tmm_vreg_l27 )
	{
		tmm_vreg_l27 = regulator_get(NULL, "8921_l27");
		if ( IS_ERR ( tmm_vreg_l27 )) 
		{
			ERROR_PRINT("could not get 8921_l27, rc = %ld",
						PTR_ERR(tmm_vreg_l27));
			
			tmm_vreg_l27 = NULL;
			return ret;
		}
		ret = regulator_set_voltage(tmm_vreg_l27, 1100000, 1100000 );
		if ( ret ) 
		{
			ERROR_PRINT("set voltage l27 failed, rc=%d", ret);
			tmm_vreg_l27 = NULL;
			return ret;
		}
	}
	
	/* power on */
	if( data == TUNER_DRV_CTL_POWON )
	{
		DEBUG_PRINT("tuner_drv_ctl_power poweron");
		
		/* poweron gpio config setting */
		tuner_drv_gpio_config_poweron();
		
		/* P.ON = 1.8V */
		if ( NULL != tmm_vreg_l15 ) 
		{
			ret = regulator_enable(tmm_vreg_l15);
			if ( ret ) 
			{
				ERROR_PRINT("enable l15 failed, rc=%d", ret);
				return ret;
			}
		}
		
		/* P.ON = 1.1V */
		if ( NULL != tmm_vreg_l27 ) 
		{
			ret = regulator_enable(tmm_vreg_l27);
			if ( ret ) 
			{
				ERROR_PRINT("enable l27 failed, rc=%d", ret);
				return ret;
			}
		}
		
		/* TMM_PWR_EN high */
		ret = ice_gpiox_set(FPGA_GPIO_TMM_PWR_EN, 1);
		if (ret < 0)
		{
			ERROR_PRINT("FPGA_GPIO_TMM_PWR_EN high set error");
			return ret;
		}
		/* 15ms sleep */
		usleep_range(15000, 15000);
		
		/* TMM_RST high */
		ret = ice_gpiox_set(FPGA_GPIO_TMM_RST, 1);
		if (ret < 0)
		{
			ERROR_PRINT("FPGA_GPIO_TMM_RST high set error");
			return ret;
		}
		/* 2ms sleep */
		usleep_range(2000, 2000);
	}
	/* power off */
	else
	{
		DEBUG_PRINT("tuner_drv_ctl_power poweroff");
		
		/* poweroff gpio config setting */
		tuner_drv_gpio_config_poweroff();
		
		/* 1ms sleep */
		usleep_range(1000, 1000);
		
		/* TMM_RST low */
		ret = ice_gpiox_set(FPGA_GPIO_TMM_RST, 0);
		if (ret < 0)
		{
			ERROR_PRINT("FPGA_GPIO_TMM_RST low set error");
			result = ret;
		}
		/* 2ms sleep */
		usleep_range(2000, 2000);
		
		/* TMM_PWR_EN low */
		ret = ice_gpiox_set(FPGA_GPIO_TMM_PWR_EN, 0);
		if (ret < 0)
		{
			ERROR_PRINT("FPGA_GPIO_TMM_PWR_EN low set error");
			result = ret;
		}
		
		/* P.OFF = 1.8V */
		if (regulator_is_enabled(tmm_vreg_l15))
		{
			ret = regulator_disable(tmm_vreg_l15);
			if ( ret ) 
			{
				ERROR_PRINT("disable l15 failed, rc=%d", ret);
				result = -1;
			}
		}
		
		/* P.OFF = 1.1V */
		if (regulator_is_enabled(tmm_vreg_l27))
		{
			ret = regulator_disable(tmm_vreg_l27);
			if ( ret ) 
			{
				ERROR_PRINT("disable l27 failed, rc=%d", ret);
				result = -1;
			}
		}
	}
	return ( result );
}

/******************************************************************************
 *    function:   tuner_drv_set_interrupt
 *    brief   :   interruption registration control of a driver
 *    date    :   2011.08.26
 *    author  :   M.Takahashi(*)
 *
 *    return  :    0                   normal exit
 *            :   -1                   error exit
 *    input   :   pdev
 *    output  :   none
 ******************************************************************************/
int tuner_drv_set_interrupt( void )
{
#ifndef TUNER_CONFIG_IRQ_PC_LINUX
    int ret;
	
	ret = request_irq( GPIO_TMM_INT,
					   tuner_interrupt,
					   IRQF_DISABLED | IRQF_TRIGGER_RISING,
					   "mm_tuner",
					   NULL );

    if( ret != 0 )
    {
        return -1;
    }
#else  /* TUNER_CONFIG_IRQ_PC_LINUX */
    i2c_set_interrupt( tuner_interrupt );
#endif /* TUNER_CONFIG_IRQ_PC_LINUX */
    return 0;
}

/******************************************************************************
 *    function:   tuner_drv_release_interrupt
 *    brief   :   interruption registration release control of a driver
 *    date    :   2011.08.26
 *    author  :   M.Takahashi(*)
 *
 *    return  :   none
 *    input   :   none
 *    output  :   none
 ******************************************************************************/
void tuner_drv_release_interrupt( void )
{
#ifndef TUNER_CONFIG_IRQ_PC_LINUX
	free_irq( GPIO_TMM_INT, NULL );
#else  /* TUNER_CONFIG_IRQ_PC_LINUX */
    i2c_release_interrupt( NULL );
#endif /* TUNER_CONFIG_IRQ_PC_LINUX */
}

#ifdef TUNER_CONFIG_IRQ_LEVELTRIGGER
/******************************************************************************
 *    function:   tuner_drv_enable_interrupt
 *    brief   :   interruption registration enable control of a driver
 *    date    :   2011.09.18
 *    author  :   M.Takahashi(*)(*)
 *
 *    return  :   none
 *    input   :   none
 *    output  :   none
 ******************************************************************************/
void tuner_drv_enable_interrupt( void )
{
#ifndef TUNER_CONFIG_IRQ_PC_LINUX
	enable_irq( GPIO_TMM_INT);
#else  /* TUNER_CONFIG_IRQ_PC_LINUX */
    i2c_set_interrupt( tuner_interrupt );
#endif /* TUNER_CONFIG_IRQ_PC_LINUX */
	return;
}

/******************************************************************************
 *    function:   tuner_drv_disable_interrupt
 *    brief   :   interruption registration disable control of a driver
 *    date    :   2011.09.18
 *    author  :   M.Takahashi(*)(*)
 *
 *    return  :   none
 *    input   :   none
 *    output  :   none
 ******************************************************************************/
void tuner_drv_disable_interrupt( void )
{
#ifndef TUNER_CONFIG_IRQ_PC_LINUX
	disable_irq( GPIO_TMM_INT);
#else  /* TUNER_CONFIG_IRQ_PC_LINUX */
    i2c_release_interrupt( NULL );
#endif /* TUNER_CONFIG_IRQ_PC_LINUX */
	return;
}
#endif /* TUNER_CONFIG_IRQ_LEVELTRIGGER */

/******************************************************************************
 *    function:   tuner_drv_gpio_config_poweron
 *    brief   :   interruption registration disable control of a driver
 *    date    :   2012.12.18
 *    author  :   K.Matsumaru(*)(*)
 *
 *    return  :   none
 *    input   :   none
 *    output  :   none
 ******************************************************************************/
void tuner_drv_gpio_config_poweron( void )
{
	struct pm_gpio tmm_int_cfg = {
		.direction = PM_GPIO_DIR_IN,
		.pull = PM_GPIO_PULL_NO,
		.vin_sel = 2,
		.function = PM_GPIO_FUNC_NORMAL,
		.inv_int_pol = 0,
	};
	
	gpio_tlmm_config(GPIO_CFG(GPIO_TMM_SPI_DI, GPIOMUX_FUNC_1,
					 GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
					 GPIO_CFG_ENABLE);
	
	gpio_tlmm_config(GPIO_CFG(GPIO_TMM_SPI_DO, GPIOMUX_FUNC_1,
					 GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
					 GPIO_CFG_ENABLE);
	
	gpio_tlmm_config(GPIO_CFG(GPIO_TMM_SPI_CS, GPIOMUX_FUNC_1,
					 GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
					 GPIO_CFG_ENABLE);
	
	gpio_tlmm_config(GPIO_CFG(GPIO_TMM_SPI_CLK, GPIOMUX_FUNC_1,
					 GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
					 GPIO_CFG_ENABLE);
	
	pm8xxx_gpio_config(GPIO_TMM_INT, &tmm_int_cfg);
	
#if defined(CONFIG_TMM_CHG_CTRL)
	tmm_chg_ctrl_start();

	tmm_chg_log(KERN_ALERT"%s: tmm charging control start!\n", __func__);
#endif
	
	return;
}

/******************************************************************************
 *    function:   tuner_drv_gpio_config_poweroff
 *    brief   :   interruption registration disable control of a driver
 *    date    :   2012.12.18
 *    author  :   K.Matsumaru(*)(*)
 *
 *    return  :   none
 *    input   :   none
 *    output  :   none
 ******************************************************************************/
void tuner_drv_gpio_config_poweroff( void )
{
	gpio_tlmm_config(GPIO_CFG(GPIO_TMM_SPI_DI, GPIOMUX_FUNC_GPIO,
					 GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
					 GPIO_CFG_ENABLE);
	
	gpio_tlmm_config(GPIO_CFG(GPIO_TMM_SPI_DO, GPIOMUX_FUNC_GPIO,
					 GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
					 GPIO_CFG_ENABLE);
	
	gpio_tlmm_config(GPIO_CFG(GPIO_TMM_SPI_CS, GPIOMUX_FUNC_GPIO,
					 GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
					 GPIO_CFG_ENABLE);
	
	gpio_tlmm_config(GPIO_CFG(GPIO_TMM_SPI_CLK, GPIOMUX_FUNC_GPIO,
					 GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
					 GPIO_CFG_ENABLE);
	
#if defined(CONFIG_TMM_CHG_CTRL)
	tmm_chg_ctrl_stop();

	tmm_chg_log(KERN_ALERT"%s: tmm charging control stop!\n", __func__);
#endif
	
	return;
}

