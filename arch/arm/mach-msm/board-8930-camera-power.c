
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

#include <asm/mach-types.h>
#include <linux/gpio.h>
#include <mach/board.h>
#include <mach/msm_bus_board.h>
#include <mach/gpiomux.h>
#include <mach/msm8930-gpio.h>
#include "devices.h"
#include "board-8930.h"
#include <mach/socinfo.h>

#undef CAM_DEBUG
#if defined(CAM_DEBUG_HIGH)
#define CAM_DEBUG(fmt, arg...)	\
	do {					\
		printk(KERN_DEBUG "[%s:%d] " fmt,	\
			__func__, __LINE__, ##arg);	\
	}						\
	while (0)
#else
#define CAM_DEBUG(fmt, arg...)
#endif

#undef cam_err
#define cam_err(fmt, arg...)			\
	do {					\
		printk(KERN_ERR "[%s:%d] " fmt,		\
			__func__, __LINE__, ##arg);	\
	}						\
	while (0)

#if defined(CONFIG_ISX012)
void msm_camera_gpio_install(void)
{
	CAM_DEBUG("Camera GPIO install!!\n");

	/*Flash Set : GPIO 3*/
	gpio_tlmm_config(GPIO_CFG(GPIO_MSM_FLASH_NOW, 0,
		GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA),
		GPIO_CFG_ENABLE);
#if !defined(CONFIG_MACH_EXPRESS)
	/*CAM_MCLK1  : GPIO 4*/
	gpio_tlmm_config(GPIO_CFG(GPIO_SUB_CAM_MCLK, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
#endif
	/*CAM_MCLK0  : GPIO 5*/
	gpio_tlmm_config(GPIO_CFG(GPIO_MAIN_CAM_MCLK, 1, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	/*GPIO_CAM_CORE_EN  : GPIO 6*/
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM_CORE_EN, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
	/*Front cam stby : GPIO 18*/
	gpio_tlmm_config(GPIO_CFG(GPIO_VT_STBY, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
	/*Main cam stby : GPIO 54*/
	gpio_tlmm_config(GPIO_CFG(GPIO_MAIN_STBY, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(GPIO_FLASH_LED_UNLOCK, 0,
		GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA),
		GPIO_CFG_ENABLE);
	/*Front cam reset : GPIO 76*/
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM2_RST_N, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
	/*Main cam reset  : GPIO 107*/
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM1_RST_N, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);

	CAM_DEBUG("Finish!!\n");
}

static struct regulator *l11, *l32, *l34;
#if !defined(CONFIG_SR130PC20) && !defined(CONFIG_SR030PC50)
static struct regulator *l30;
#endif
#if defined(CONFIG_SR130PC20) || defined(CONFIG_SR030PC50)
static struct regulator *l29;
#endif
#define MAX_CAM_POWER_PIN 5
int error_gpio_buf[MAX_CAM_POWER_PIN];
int error_gpio_cnt;

char cam_power_pin[MAX_CAM_POWER_PIN][32] = {
	"GPIO_CAM_CORE_EN",
	"GPIO_CAM_IO_EN",
	"GPIO_CAM_A_EN",
	"8038_l12",
	"8038_l8",
};

void search_error_pin(char *name)
{
	int i = 0;

	for (i = 0; i < MAX_CAM_POWER_PIN; i++) {
		if (!strncmp(cam_power_pin[i], name, strnlen(name, 50))) {
			error_gpio_buf[error_gpio_cnt++] = i;
			break;
		}
	}
	if (i == MAX_CAM_POWER_PIN)
		cam_err("Can't find error pin : %s", name);
}
void cam_ldo_power_on(int mode)
{
	int ret = 0;
	int power_on_cnt = 0;

	cam_err("%s CAMERA POWER ON!!\n",
	       mode ? "FRONT" : "REAR");

/*5M Core 1.2V - CAM_ISP_CORE_1P2*/
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM_CORE_EN, 0, GPIO_CFG_OUTPUT,
			GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);

	gpio_set_value_cansleep(GPIO_CAM_CORE_EN, 1);
	ret = gpio_get_value(GPIO_CAM_CORE_EN);
	if (ret) {
		CAM_DEBUG("check CAM_CORE_EN : %d\n", ret);
		power_on_cnt++;
	} else
		search_error_pin("GPIO_CAM_CORE_EN");

	udelay(10);

/*Sensor IO 1.8V -CAM_SENSOR_IO_1P8  */
		l34 = regulator_get(NULL, "8917_l34");
		ret = regulator_set_voltage(l34, 1800000, 1800000);
		if (ret)
			cam_err("error setting voltage\n");
		ret = regulator_enable(l34);
		if (ret) {
			cam_err("error enabling regulator\n");
			search_error_pin("8917_l34");
		} else
			power_on_cnt++;

		udelay(1000);

/*Sensor AVDD 2.8V - CAM_SENSOR_A2P8 */
		l32 = regulator_get(NULL, "8917_l32");
		ret = regulator_set_voltage(l32, 2800000, 2800000);
		if (ret)
			cam_err("error setting voltage\n");
		ret = regulator_enable(l32);
		if (ret) {
			cam_err("error enabling regulator\n");
			search_error_pin("8917_l32");
		} else
			power_on_cnt++;

		udelay(1000);

/*VT core 1.8V - CAM_DVDD_1P8V*/
#if !defined(CONFIG_MACH_KS02)
#if defined(CONFIG_SR130PC20) || defined(CONFIG_SR030PC50)
		l29 = regulator_get(NULL, "8921_l29");
		ret = regulator_set_voltage(l29, 1800000, 1800000);
		if (ret)
			cam_err("error setting voltage\n");
		ret = regulator_enable(l29);
		if (ret) {
			cam_err("error enabling regulator\n");
			search_error_pin("8917_l29");
		} else
			power_on_cnt++;
#else
		l30 = regulator_get(NULL, "8917_l30");
		ret = regulator_set_voltage(l30, 1800000, 2800000);
		if (ret)
			cam_err("error setting voltage\n");
		ret = regulator_enable(l30);
		if (ret) {
			cam_err("error enabling regulator\n");
			search_error_pin("8917_l30");
		} else
			power_on_cnt++;
#endif
		udelay(10);
#endif

/*Sensor AF 2.8V -CAM_AF_2P8  */
	if (!mode) {
		l11 = regulator_get(NULL, "8917_l11");
		ret = regulator_set_voltage(l11, 2800000, 2850000);
		if (ret)
			cam_err("error setting voltage\n");
		ret = regulator_enable(l11);
		if (ret) {
			cam_err("error enabling regulator\n");
			search_error_pin("8917_l11");
		} else
			power_on_cnt++;
	}
		udelay(200);

	CAM_DEBUG("%s End!!\n",
	       mode ? "FRONT" : "REAR");
}

void cam_ldo_power_off(int mode)
{
	int ret = 0;

	cam_err("%s CAMERA POWER OFF!!\n",
	       mode ? "FRONT" : "REAR");

/*Sensor AF 2.8V -CAM_AF_2P8  */
	if (!mode) {
				if (l11) {
					ret = regulator_disable(l11);
			if (ret)
				cam_err("error disabling regulator\n");
		}
	}

	usleep(10);

/*VT core 1.2 - CAM_DVDD_1P5V*/
#if defined(CONFIG_SR130PC20) || defined(CONFIG_SR030PC50)
	if (l29) {
		ret = regulator_disable(l29);
		if (ret)
			cam_err("error disabling regulator\n");
	}
#else
			if (l30) {
				ret = regulator_disable(l30);
		if (ret)
			cam_err("error disabling regulator\n");
	}
#endif
	usleep(8000);

/*Sensor AVDD 2.8V - CAM_SENSOR_A2P8 */
			if (l32) {
				ret = regulator_disable(l32);
				if (ret)
					cam_err("error disabling regulator\n");
			}

	usleep(8000);

/*Sensor IO 1.8V -CAM_SENSOR_IO_1P8  */
			if (l34) {
					ret = regulator_disable(l34);
					if (ret)
						cam_err("error disabling regulator\n");
			}

	usleep(10);

/*5M Core 1.2V - CAM_ISP_CORE_1P2*/
	gpio_set_value_cansleep(GPIO_CAM_CORE_EN, 0);

	CAM_DEBUG("%s End!!\n",
	       mode ? "FRONT" : "REAR");
}
#elif defined(CONFIG_IMX175)
void msm_camera_gpio_install(void)
{
	cam_err("[Start]\n");

	/* init Camera GPIOs */
	/*Flash Set : GPIO 3*/
	gpio_tlmm_config(GPIO_CFG(GPIO_MSM_FLASH_NOW, 0,
		GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA),
		GPIO_CFG_ENABLE);

#if defined(CONFIG_MACH_MELIUS)
#if defined(CONFIG_MACH_MELIUS_VZW) || defined(CONFIG_MACH_MELIUS_SPR) || defined(CONFIG_MACH_MELIUS_USC)
	if (system_rev < 0X01) {
#else		
	if (system_rev < 0x07) {
#endif		
		/*Flash UNLOCK : GPIO 64*/
		gpio_tlmm_config(GPIO_CFG(GPIO_FLASH_LED_UNLOCK, 0,
			GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA),
			GPIO_CFG_ENABLE);
	} else {
		gpio_tlmm_config(GPIO_CFG(GPIO_CAM_SENSOR_EN, 0,
			GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
			GPIO_CFG_ENABLE);
	}
#elif !defined(CONFIG_MACH_KS02)
		/*Flash UNLOCK : GPIO 64*/
		gpio_tlmm_config(GPIO_CFG(GPIO_FLASH_LED_UNLOCK, 0,
			GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA),
			GPIO_CFG_ENABLE);
#endif
	/*GPIO_CAM_IO_EN  : GPIO 1*/
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM_IO_EN, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), GPIO_CFG_ENABLE);

#if defined(CONFIG_MACH_KS02)
	/*CAM_MCLK1  : GPIO 4*/
	gpio_tlmm_config(GPIO_CFG(GPIO_SUB_CAM_MCLK, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_6MA), GPIO_CFG_ENABLE);

	/*CAM_MCLK0  : GPIO 5*/
	gpio_tlmm_config(GPIO_CFG(GPIO_MAIN_CAM_MCLK, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_12MA), GPIO_CFG_ENABLE);
#else
	gpio_tlmm_config(GPIO_CFG(GPIO_SUB_CAM_MCLK, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), GPIO_CFG_ENABLE);

	/*CAM_MCLK0  : GPIO 5*/
	gpio_tlmm_config(GPIO_CFG(GPIO_MAIN_CAM_MCLK, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
#endif

	/*GPIO_I2C_DATA_AF  : GPIO 51*/
	gpio_tlmm_config(GPIO_CFG(GPIO_I2C_DATA_AF, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_NO_PULL, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
    
	/*GPIO_I2C_CLK_AF : GPIO 52*/
	gpio_tlmm_config(GPIO_CFG(GPIO_I2C_CLK_AF, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_NO_PULL, GPIO_CFG_16MA), GPIO_CFG_ENABLE);

	/*Front cam reset : GPIO 76*/
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM2_RST_N, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);

	/*Main cam reset  : GPIO 107*/
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM1_RST_N, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);

#if defined(CONFIG_MACH_KS02)
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM_FLASH_EN, 0,
		GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM_FLASH_SET, 0,
		GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		GPIO_CFG_ENABLE);
#endif
}
#elif defined(CONFIG_S5K5CCGX)
void msm_camera_gpio_install(void)
{
	CAM_DEBUG("Camera GPIO install LT02!!\n");

	/*Flash Set : GPIO 3*/
/*	gpio_tlmm_config(GPIO_CFG(GPIO_MSM_FLASH_NOW, 0,
		GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA),
		GPIO_CFG_ENABLE);*/

	/*CAM_MCLK1  : GPIO 4*/
	gpio_tlmm_config(GPIO_CFG(GPIO_SUB_CAM_MCLK, 2, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_6MA), GPIO_CFG_ENABLE);
	/*CAM_MCLK0  : GPIO 5*/
	gpio_tlmm_config(GPIO_CFG(GPIO_MAIN_CAM_MCLK, 1, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_6MA), GPIO_CFG_ENABLE);
	/*GPIO_CAM_CORE_EN  : GPIO 6*/
#ifdef CONFIG_MACH_LT02
	if(system_rev >= 0x2) {
		gpio_tlmm_config(GPIO_CFG(GPIO_CAM_CORE_EN, 0, GPIO_CFG_OUTPUT,
			GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
	}
#endif
	/*Front cam stby : GPIO 18*/
	gpio_tlmm_config(GPIO_CFG(GPIO_VT_STBY, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
	/*Main cam stby : GPIO 2*/
	gpio_tlmm_config(GPIO_CFG(GPIO_MAIN_STBY, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
	/*gpio_tlmm_config(GPIO_CFG(GPIO_FLASH_LED_UNLOCK, 0,
		GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA),
		GPIO_CFG_ENABLE);*/
	/*Front cam reset : GPIO 76*/
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM2_RST_N, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
	/*Main cam reset  : GPIO 107*/
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM1_RST_N, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);

	CAM_DEBUG("Finish!!\n");
}
#if defined(CONFIG_S5K5CCGX) && defined(CONFIG_SR130PC20)
static struct regulator *l32, *l34, *l8;
#ifdef CONFIG_MACH_LT02
static struct regulator *lvs5,*lvs7;
#endif
#else
static struct regulator *l11, *l32, *l34;
#endif
#if !defined(CONFIG_SR130PC20) && !defined(CONFIG_SR030PC50)
static struct regulator *l30;
#endif
#if defined(CONFIG_SR130PC20) || defined(CONFIG_SR030PC50)
static struct regulator *l29;
#endif
#define MAX_CAM_POWER_PIN 5
int error_gpio_buf[MAX_CAM_POWER_PIN];
int error_gpio_cnt;

char cam_power_pin[MAX_CAM_POWER_PIN][32] = {
	"GPIO_CAM_CORE_EN",
	"GPIO_CAM_IO_EN",
	"GPIO_CAM_A_EN",
	"8038_l12",
	"8038_l8",
};

void search_error_pin(char *name)
{
	int i = 0;

	for (i = 0; i < MAX_CAM_POWER_PIN; i++) {
		if (!strncmp(cam_power_pin[i], name, strnlen(name, 50))) {
			error_gpio_buf[error_gpio_cnt++] = i;
			break;
		}
	}
	if (i == MAX_CAM_POWER_PIN)
		cam_err("Can't find error pin : %s", name);
}
void cam_ldo_power_on(int mode)
{
	int ret = 0;
	int power_on_cnt = 0;

	cam_err("%s CAMERA POWER ON!!\n",
	       mode ? "FRONT" : "REAR");

#if defined(CONFIG_S5K5CCGX) && defined(CONFIG_SR130PC20)
#ifdef CONFIG_MACH_LT02
    if(system_rev < 0x2) {
	/*3M CAM_SENSOR_CORE_1P2*/
	l8 = regulator_get(NULL, "8917_l8");
		ret = regulator_set_voltage(l8, 1200000, 1200000);
		if (ret)
			cam_err("error setting voltage L8 Regulator\n");
		ret = regulator_enable(l8);
		if (ret) {
			cam_err("error enabling regulator L8 Regulator\n");
			search_error_pin("8917_l8");
		} else
			power_on_cnt++;
	} else {
	/*5M Core 1.2V - CAM_ISP_CORE_1P2*/
		gpio_tlmm_config(GPIO_CFG(GPIO_CAM_CORE_EN, 0, GPIO_CFG_OUTPUT,
				GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);

		gpio_set_value_cansleep(GPIO_CAM_CORE_EN, 1);
		ret = gpio_get_value(GPIO_CAM_CORE_EN);
		if (ret) {
			CAM_DEBUG("check CAM_CORE_EN : %d\n", ret);
			power_on_cnt++;
		} else
			search_error_pin("GPIO_CAM_CORE_EN");
	}
#else
	/*3M CAM_SENSOR_CORE_1P2*/
	l8 = regulator_get(NULL, "8917_l8");
		ret = regulator_set_voltage(l8, 1200000, 1200000);
		if (ret)
			cam_err("error setting voltage L8 Regulator\n");
		ret = regulator_enable(l8);
		if (ret) {
			cam_err("error enabling regulator L8 Regulator\n");
			search_error_pin("8917_l8");
		} else
			power_on_cnt++;
#endif
#else
/*5M Core 1.2V - CAM_ISP_CORE_1P2*/
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM_CORE_EN, 0, GPIO_CFG_OUTPUT,
			GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);

	gpio_set_value_cansleep(GPIO_CAM_CORE_EN, 1);
	ret = gpio_get_value(GPIO_CAM_CORE_EN);
	if (ret) {
		CAM_DEBUG("check CAM_CORE_EN : %d\n", ret);
		power_on_cnt++;
	} else
		search_error_pin("GPIO_CAM_CORE_EN");
#endif
	udelay(1000);

	/*Sensor AVDD 2.8V - CAM_SENSOR_A2P8 */
		l32 = regulator_get(NULL, "8917_l32");
		ret = regulator_set_voltage(l32, 2800000, 2800000);
		if (ret)
			cam_err("error setting voltage\n");
		ret = regulator_enable(l32);
		if (ret) {
			cam_err("error enabling regulator\n");
			search_error_pin("8917_l32");
		} else
			power_on_cnt++;
#if !defined(CONFIG_MACH_LT02_ATT) || !defined(CONFIG_MACH_LT02_SPR) || !defined(CONFIG_MACH_LT02_TMO)
		udelay(1000);
#endif

/*VT core 1.8V - CAM_DVDD_1P8V*/
#if !defined(CONFIG_MACH_KS02)
#if defined(CONFIG_SR130PC20) || defined(CONFIG_SR030PC50)
#ifdef CONFIG_MACH_LT02
    if(system_rev < 0x2) {
		l29 = regulator_get(NULL, "8921_l29");
		ret = regulator_set_voltage(l29, 1800000, 1800000);
		if (ret)
			cam_err("error setting voltage\n");
		ret = regulator_enable(l29);
		if (ret) {
			cam_err("error enabling regulator\n");
			search_error_pin("8917_l29");
		} else
			power_on_cnt++;
	} else {
		lvs7 = regulator_get(NULL, "8917_lvs7");
		ret = regulator_set_voltage(lvs7, 1800000, 1800000);
		if (ret)
			cam_err("error setting voltage\n");
		ret = regulator_enable(lvs7);
		if (ret) {
			cam_err("error enabling regulator\n");
			search_error_pin("8917_lvs7");
		} else
			power_on_cnt++;
	}
#else
		l29 = regulator_get(NULL, "8921_l29");
		ret = regulator_set_voltage(l29, 1800000, 1800000);
		if (ret)
			cam_err("error setting voltage\n");
		ret = regulator_enable(l29);
		if (ret) {
			cam_err("error enabling regulator\n");
			search_error_pin("8917_l29");
		} else
			power_on_cnt++;
#endif
#else
		l30 = regulator_get(NULL, "8917_l30");
		ret = regulator_set_voltage(l30, 1800000, 2800000);
		if (ret)
			cam_err("error setting voltage\n");
		ret = regulator_enable(l30);
		if (ret) {
			cam_err("error enabling regulator\n");
			search_error_pin("8917_l30");
		} else
			power_on_cnt++;
#endif
#if !defined(CONFIG_MACH_LT02_ATT) || !defined(CONFIG_MACH_LT02_SPR) || !defined(CONFIG_MACH_LT02_TMO)
		udelay(10);
#endif
#endif
#ifdef CONFIG_MACH_LT02
    if(system_rev < 0x2) {
/*Sensor IO 1.8V -CAM_SENSOR_IO_1P8  */
		l34 = regulator_get(NULL, "8917_l34");
		ret = regulator_set_voltage(l34, 1800000, 1800000);
		if (ret)
			cam_err("error setting voltage\n");
		ret = regulator_enable(l34);
		if (ret) {
			cam_err("error enabling regulator\n");
			search_error_pin("8917_l34");
		} else
			power_on_cnt++;
	} else {
	/*Sensor IO 1.8V -CAM_SENSOR_IO_1P8  */
		lvs5 = regulator_get(NULL, "8917_lvs5");
		ret = regulator_set_voltage(lvs5, 1800000, 1800000);
		if (ret)
			cam_err("error setting voltage\n");
		ret = regulator_enable(lvs5);
		if (ret) {
			cam_err("error enabling regulator\n");
			search_error_pin("8917_lvs5");
		} else
			power_on_cnt++;
	}
#else
/*Sensor IO 1.8V -CAM_SENSOR_IO_1P8  */
	l34 = regulator_get(NULL, "8917_l34");
	ret = regulator_set_voltage(l34, 1800000, 1800000);
	if (ret)
		cam_err("error setting voltage\n");
	ret = regulator_enable(l34);
	if (ret) {
		cam_err("error enabling regulator\n");
		search_error_pin("8917_l34");
	} else
		power_on_cnt++;
#endif
		udelay(1000);




#if 0
/*Sensor AF 2.8V -CAM_AF_2P8  */
	if (!mode) {
		l11 = regulator_get(NULL, "8917_l11");
		ret = regulator_set_voltage(l11, 2800000, 2850000);
		if (ret)
			cam_err("error setting voltage\n");
		ret = regulator_enable(l11);
		if (ret) {
			cam_err("error enabling regulator\n");
			search_error_pin("8917_l11");
		} else
			power_on_cnt++;
	}
		udelay(200);
#endif
	CAM_DEBUG("%s End!!\n",
	       mode ? "FRONT" : "REAR");
}

void cam_ldo_power_off(int mode)
{
	int ret = 0;

	cam_err("%s CAMERA POWER OFF!!\n",
	       mode ? "FRONT" : "REAR");
#if 0
/*Sensor AF 2.8V -CAM_AF_2P8  */
	if (!mode) {
				if (l11) {
					ret = regulator_disable(l11);
			if (ret)
				cam_err("error disabling regulator\n");
		}
	}
#endif

	usleep(10);
#ifdef CONFIG_MACH_LT02
   if(system_rev < 0x2) {
/*Sensor IO 1.8V -CAM_SENSOR_IO_1P8  */
			if (l34) {
					ret = regulator_disable(l34);
					if (ret)
						cam_err("error disabling regulator L34\n");
					else
						regulator_put(l34);
			}
	} else {
/*Sensor IO 1.8V -CAM_SENSOR_IO_1P8  */
			if (lvs5) {
					ret = regulator_disable(lvs5);
					if (ret)
						cam_err("error disabling regulator Lvs5\n");
					else
						regulator_put(lvs5);
			}
	}
#else
/*Sensor IO 1.8V -CAM_SENSOR_IO_1P8  */
			if (l34) {
					ret = regulator_disable(l34);
					if (ret)
						cam_err("error disabling regulator L34\n");
			}
#endif
	usleep(10);

/*VT core 1.2 - CAM_DVDD_1P5V*/
#if defined(CONFIG_SR130PC20) || defined(CONFIG_SR030PC50)
#ifdef CONFIG_MACH_LT02
	if(system_rev < 0x2) {
		if (l29) {
			ret = regulator_disable(l29);
			if (ret)
				cam_err("error disabling regulator L29\n");
			else
				regulator_put(l29);
		}
	} else {
		if (lvs7) {
			ret = regulator_disable(lvs7);
			if (ret)
				cam_err("error disabling regulator Lvs7\n");
			else
				regulator_put(lvs7);
		}
	}
#else
	if (l29) {
		ret = regulator_disable(l29);
		if (ret)
			cam_err("error disabling regulator L29\n");
	}
#endif
#else
			if (l30) {
				ret = regulator_disable(l30);
		if (ret)
			cam_err("error disabling regulator L30\n");
	}
#endif
	usleep(8000);

/*Sensor AVDD 2.8V - CAM_SENSOR_A2P8 */
			if (l32) {
				ret = regulator_disable(l32);
				if (ret)
					cam_err("error disabling regulator L32\n");
				else
					regulator_put(l32);
			}

	usleep(8000);



#if defined(CONFIG_S5K5CCGX) && defined(CONFIG_SR130PC20)
#ifdef CONFIG_MACH_LT02
    if(system_rev < 0x2) {
		if (l8) {
						ret = regulator_disable(l8);
						if (ret)
							cam_err("error disabling regulator L8\n");
						else
							regulator_put(l8);
				}
		usleep(8000);
	} else {
/*5M Core 1.2V - CAM_ISP_CORE_1P2*/
	gpio_set_value_cansleep(GPIO_CAM_CORE_EN, 0);
	}
#else
	if (l8) {
					ret = regulator_disable(l8);
					if (ret)
						cam_err("error disabling regulator L8\n");
			}
	usleep(8000);
#endif
#else
/*5M Core 1.2V - CAM_ISP_CORE_1P2*/
	gpio_set_value_cansleep(GPIO_CAM_CORE_EN, 0);
#endif
	CAM_DEBUG("%s End!!\n",
	       mode ? "FRONT" : "REAR");
}
#elif defined(CONFIG_S5K4ECGX)
void msm_camera_gpio_install(void)
{
	CAM_DEBUG("Camera GPIO install!!\n");

	/*GPIO_CAM_CORE_EN  : GPIO 1*/
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM_CORE_EN, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
	/*Flash SOURCE EN : GPIO 3*/
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM_FLASH_SOURCE_EN, 0,
		GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA),
		GPIO_CFG_ENABLE);
	/*CAM_MCLK1  : GPIO 4*/
	gpio_tlmm_config(GPIO_CFG(GPIO_SUB_CAM_MCLK, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	/*CAM_MCLK0  : GPIO 5*/
	gpio_tlmm_config(GPIO_CFG(GPIO_MAIN_CAM_MCLK, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	/*Main cam stby : GPIO 51*/
	gpio_tlmm_config(GPIO_CFG(GPIO_MAIN_STBY, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
	/*Front cam stby : GPIO 52*/
	gpio_tlmm_config(GPIO_CFG(GPIO_VT_STBY, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);

	/*Flash SET/EN : GPIO 63/64*/
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM_FLASH_SET, 0,
		GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA),
		GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM_FLASH_EN, 0,
		GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA),
		GPIO_CFG_ENABLE);

	/*Front cam reset : GPIO 76*/
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM2_RST_N, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
	/*Main cam reset  : GPIO 107*/
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM1_RST_N, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);

	CAM_DEBUG("Finish!!\n");
}

static struct regulator *l11, *l32, *l34;
#if defined(CONFIG_SR030PC50)
static struct regulator *l29;
#endif
#define MAX_CAM_POWER_PIN 5
int error_gpio_buf[MAX_CAM_POWER_PIN];
int error_gpio_cnt;

char cam_power_pin[MAX_CAM_POWER_PIN][32] = {
	"GPIO_CAM_CORE_EN",
	"8917_l11",
	"8917_l32",
	"8917_l34",
	"8917_l29",
};

void search_error_pin(char *name)
{
	int i = 0;

	for (i = 0; i < MAX_CAM_POWER_PIN; i++) {
		if (!strncmp(cam_power_pin[i], name, strnlen(name, 50))) {
			error_gpio_buf[error_gpio_cnt++] = i;
			break;
		}
	}
	if (i == MAX_CAM_POWER_PIN)
		cam_err("Can't find error pin : %s", name);
}
void cam_ldo_power_on(int mode)
{
	int ret = 0;
	int power_on_cnt = 0;

	cam_err("%s CAMERA POWER ON!!\n",
	       mode ? "FRONT" : "REAR");

/*Sensor AVDD 2.8V - CAM_SENSOR_A2P8 */
	l32 = regulator_get(NULL, "8917_l32");
	ret = regulator_set_voltage(l32, 2800000, 2800000);
	if (ret)
		cam_err("error setting voltage\n");
	ret = regulator_enable(l32);
	if (ret) {
		cam_err("error enabling regulator\n");
		search_error_pin("8917_l32");
	} else
		power_on_cnt++;

	udelay(1000);

/*Sensor IO 1.8V -CAM_SENSOR_IO_1P8  */
	l34 = regulator_get(NULL, "8917_l34");
	ret = regulator_set_voltage(l34, 1800000, 1800000);
	if (ret)
		cam_err("error setting voltage\n");
	ret = regulator_enable(l34);
	if (ret) {
		cam_err("error enabling regulator\n");
		search_error_pin("8917_l34");
	} else
		power_on_cnt++;

	udelay(70);

/*5M Core 1.2V - CAM_ISP_CORE_1P2*/
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM_CORE_EN, 0, GPIO_CFG_OUTPUT,
			GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
	gpio_set_value_cansleep(GPIO_CAM_CORE_EN, 1);
	ret = gpio_get_value(GPIO_CAM_CORE_EN);
	if (ret) {
		CAM_DEBUG("check CAM_CORE_EN : %d\n", ret);
		power_on_cnt++;
	} else
		search_error_pin("GPIO_CAM_CORE_EN");

	udelay(10);

/*Sensor AF 2.8V -CAM_AF_2P8  */
	if (!mode) {
		l11 = regulator_get(NULL, "8917_l11");
		ret = regulator_set_voltage(l11, 2800000, 2850000);
		if (ret)
			cam_err("error setting voltage\n");
		ret = regulator_enable(l11);
		if (ret) {
			cam_err("error enabling regulator\n");
			search_error_pin("8917_l11");
		} else
			power_on_cnt++;
	}
	udelay(200);

/*VT core 1.8V - CAM_DVDD_1P8*/
#if defined(CONFIG_SR030PC50)
	l29 = regulator_get(NULL, "8921_l29");
	ret = regulator_set_voltage(l29, 1800000, 1800000);
	if (ret)
		cam_err("error setting voltage\n");
	ret = regulator_enable(l29);
	if (ret) {
		cam_err("error enabling regulator\n");
		search_error_pin("8917_l29");
	} else
		power_on_cnt++;

	udelay(10);
#endif

	CAM_DEBUG("%s End!!\n",
	       mode ? "FRONT" : "REAR");
}

void cam_ldo_power_off(int mode)
{
	int ret = 0;

	cam_err("%s CAMERA POWER OFF!!\n",
	       mode ? "FRONT" : "REAR");

/*Sensor AF 2.8V -CAM_AF_2P8*/
	if (!mode) {
		if (l11) {
			ret = regulator_disable(l11);
			if (ret)
				cam_err("error disabling regulator\n");
		}
	}
	usleep(10);

/*VT core 1.8 - CAM_DVDD_1P8*/
#if defined(CONFIG_SR030PC50)
	if (l29) {
		ret = regulator_disable(l29);
		if (ret)
			cam_err("error disabling regulator\n");
	}
#endif
	usleep(8000);

/*Sensor AVDD 2.8V - CAM_SENSOR_A2P8 */
	if (l32) {
		ret = regulator_disable(l32);
		if (ret)
			cam_err("error disabling regulator\n");
	}
	usleep(8000);

/*Sensor IO 1.8V -CAM_SENSOR_IO_1P8  */
	if (l34) {
			ret = regulator_disable(l34);
			if (ret)
				cam_err("error disabling regulator\n");
	}
	usleep(10);

/*5M Core 1.2V - CAM_ISP_CORE_1P2*/
	gpio_set_value_cansleep(GPIO_CAM_CORE_EN, 0);

	CAM_DEBUG("%s End!!\n",
	       mode ? "FRONT" : "REAR");
}

#elif defined(CONFIG_SR352)
void msm_camera_gpio_install(void)
{
	CAM_DEBUG("Camera GPIO install LT02!!\n");

	/*CAM_MCLK1  : GPIO 4*/
	gpio_tlmm_config(GPIO_CFG(GPIO_SUB_CAM_MCLK, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	/*CAM_MCLK0  : GPIO 5*/
	gpio_tlmm_config(GPIO_CFG(GPIO_MAIN_CAM_MCLK, 1, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	/*GPIO_CAM_CORE_EN  : GPIO 1*/
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM_IO_EN, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);//jixu
	/*Front cam stby : GPIO 64*/
	gpio_tlmm_config(GPIO_CFG(GPIO_VT_STBY, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
	/*Main cam stby : GPIO 63*/
	gpio_tlmm_config(GPIO_CFG(GPIO_MAIN_STBY, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
	/*Front cam reset : GPIO 76*/
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM2_RST_N, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
	/*Main cam reset  : GPIO 107*/
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM1_RST_N, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
	/*Camera Analog 2.8V : GPIO 1 */
	gpio_tlmm_config(GPIO_CFG(GPIO_CAM_SENSOR_EN, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), GPIO_CFG_ENABLE);

	

	CAM_DEBUG("Finish!!\n");

}

static struct regulator *l29, *l30, *l34;


#define MAX_CAM_POWER_PIN 5
int error_gpio_buf[MAX_CAM_POWER_PIN];
int error_gpio_cnt;

char cam_power_pin[MAX_CAM_POWER_PIN][32] = {
	"GPIO_CAM_CORE_EN",
	"GPIO_CAM_IO_EN",
	"GPIO_CAM_A_EN",
	"8038_l12",
	"8038_l8",
};

void search_error_pin(char *name)
{
	int i = 0;

	for (i = 0; i < MAX_CAM_POWER_PIN; i++) {
		if (!strncmp(cam_power_pin[i], name, strnlen(name, 50))) {
			error_gpio_buf[error_gpio_cnt++] = i;
			break;
		}
	}
	if (i == MAX_CAM_POWER_PIN)
		cam_err("Can't find error pin : %s", name);
}
void cam_ldo_power_on(int mode)
{
	int ret = 0;
	int power_on_cnt = 0;

	cam_err("%s CAMERA POWER ON!!\n",
	mode ? "FRONT" : "REAR");

	/*Sensor IO 1.8V -CAM_SENSOR_IO_1P8  */
	l34 = regulator_get(NULL, "8917_l34");
	ret = regulator_set_voltage(l34, 1800000, 1800000);
	if (ret)
		cam_err("error setting voltage\n");
	ret = regulator_enable(l34);
	if (ret) {
		cam_err("error enabling regulator\n");
		search_error_pin("8917_l34");
	} else
		power_on_cnt++;

	udelay(1000);

	/*Sensor AVDD 2.8V - CAM_SENSOR_A2P8 */
	gpio_set_value_cansleep(GPIO_CAM_SENSOR_EN, 1);
	udelay(100);

	/*VT core 1.8V - CAM_DVDD_1P8V*/
	l30 = regulator_get(NULL, "8917_l30");
	ret = regulator_set_voltage(l30, 1800000, 1800000);
	if (ret)
		cam_err("error setting voltage\n");
	ret = regulator_enable(l30);
	if (ret) {
		cam_err("error enabling regulator\n");
		search_error_pin("8917_l30");
	} else
		power_on_cnt++;

	udelay(1000);

	/*MAIN core 1.8V - CAM_SENSOR_CORE_1P8V*/
	l29 = regulator_get(NULL, "8921_l29");
	ret = regulator_set_voltage(l29, 1800000, 1800000);
	if (ret)
		cam_err("error setting voltage\n");
	ret = regulator_enable(l29);
	if (ret) {
		cam_err("error enabling regulator\n");
		search_error_pin("8921_l29");//jixu
	} else
		power_on_cnt++;

	/*MAIN core 1.8V - CAM_SENSOR_CORE_1P8V  changed for revision 02. */
	
	gpio_set_value_cansleep(GPIO_CAM_CORE_EN, 1);
	ret = gpio_get_value(GPIO_CAM_CORE_EN);
	if (ret) {
		cam_err("check CAM_CORE_EN : %d\n", ret);
		power_on_cnt++;
	} else
		search_error_pin("GPIO_CAM_CORE_EN");

	udelay(10);

	udelay(1000);



	CAM_DEBUG("%s End!!\n",
	       mode ? "FRONT" : "REAR");

}

void cam_ldo_power_off(int mode)
{
	int ret = 0;

	cam_err("%s CAMERA POWER OFF!!\n",
	mode ? "FRONT" : "REAR");

	/*VT core 1.2 - CAM_DVDD_1P5V*/
	if (l30) {
		ret = regulator_disable(l30);
		if (ret)
			cam_err("error disabling regulator\n");
	}
	usleep(8000);

	/*Sensor AVDD 2.8V - CAM_SENSOR_A2P8 */
	if (l29) {
		ret = regulator_disable(l29);
		if (ret)
			cam_err("error disabling regulator\n");
	}

	gpio_set_value_cansleep(GPIO_CAM_CORE_EN, 0);  //for temp HW revision 02

	usleep(8000);

	/*Sensor IO 1.8V -CAM_SENSOR_IO_1P8  */
	if (l34) {
		ret = regulator_disable(l34);
		if (ret)
			cam_err("error disabling regulator\n");
	}

	usleep(10);
	
	gpio_set_value_cansleep(GPIO_CAM_SENSOR_EN, 0);
	udelay(100);

	CAM_DEBUG("%s End!!\n",
		mode ? "FRONT" : "REAR");

}
#else /*#if defined(CONFIG_MACH_BOOST)*/
void msm_camera_gpio_install(void)
{
	cam_err("Camera GPIO install!!\n");
	cam_err("Invalid function call.\n");
}
void cam_ldo_power_on(int mode)
{
	cam_err("%s CAMERA POWER ON!!\n",
	       mode ? "FRONT" : "REAR");

	cam_err("Invalid function call.\n");
}
void cam_ldo_power_off(int mode)
{
	cam_err("%s CAMERA POWER OFF!!\n",
	       mode ? "FRONT" : "REAR");
	cam_err("Invalid function call.\n");
}

#endif
