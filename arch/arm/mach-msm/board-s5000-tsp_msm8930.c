/*
* Copyright (C) 2011 Google, Inc.
*
* This software is licensed under the terms of the GNU General Public
* License version 2, as published by the Free Software Foundation, and
* may be copied, distributed, and modified under those terms.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/

/*
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/keyreset.h>
#include <linux/gpio_event.h>
#include <linux/gpio.h>
#include <linux/i2c.h>
#include <asm/mach-types.h>
#include <linux/regulator/consumer.h>
#include "board-8064.h"
#include <linux/delay.h>
//#include <mach/apq8064-gpio.h>
#include <mach/msm8930-gpio.h>
*/
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/keyreset.h>
#include <linux/gpio_event.h>
#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/i2c-gpio.h>
#include <asm/mach-types.h>
#include <linux/regulator/consumer.h>
#include <mach/msm8930-gpio.h>
#include <linux/delay.h>
#include "board-8930.h"
#include "devices-msm8x60.h"
#include <asm/mach-types.h>


#ifdef CONFIG_MACH_JF
#include <linux/i2c/synaptics_rmi.h>
#endif

#ifdef CONFIG_MACH_MELIUS
#include <linux/i2c/synaptics_rmi_msm8930.h>
#endif


#define GPIO_TOUCH_IRQ		11
#define GPIO_TOUCH_IRQ_REV02	66
#define GPIO_TOUCH_IRQ_MAIN_REV03	53

#if defined(CONFIG_MACH_MELIUS_CHN_CTC) || defined(CONFIG_MACH_CRATER_CHN_CTC)
#define GPIO_TOUCH_IRQ_CTC_REV00	18
#endif

/* touch is on i2c3 */
#define GPIO_TOUCH_SDA		16
#define GPIO_TOUCH_SCL		17

#define MSM_TOUCHKEY_I2C_BUS_ID		16
#define MSM_8960_GSBI3_QUP_I2C_BUS_ID	3
struct synaptics_rmi_callbacks *rmi_callback;
extern int msm8930_get_board_rev(void);

void synaptics_tsp_register_callback(struct synaptics_rmi_callbacks *cb)
{
    pr_info("%s: [synaptics] register callback!\n", __func__);
    /* set rmi callback function */
    rmi_callback = cb;
}

void synaptics_inform_callbacks(int type, int value)
{
    pr_info("%s: callback called! type = %d, value = %d\n", __func__, type, value);

    switch(type)
    {
    case RMI4_CALLBACK_CHARGER:
        if (rmi_callback && rmi_callback->inform_charger)
            rmi_callback->inform_charger(rmi_callback, value);
        break;
    case RMI4_CALLBACK_HALLSENSOR:
        if (rmi_callback && rmi_callback->inform_hallsensor)
            rmi_callback->inform_hallsensor(rmi_callback, value);
        break;
    case RMI4_CALLBACK_TSP_RESET:
        if (rmi_callback && rmi_callback->inform_tsp_reset)
            rmi_callback->inform_tsp_reset(rmi_callback);
        break;
    case RMI4_CALLBACK_LCD:
        if (rmi_callback && rmi_callback->inform_lcd)
            rmi_callback->inform_lcd(rmi_callback, value);
        break;
    default:
        pr_err("%s : unsupported callback!!\n", __func__);
        break;
    }
}

#ifdef CONFIG_LEDS_CLASS
static void msm_tkey_led_vdd_on(bool onoff)
{
  	int ret;
  	static struct regulator *reg_l33;
  
  	if (!reg_l33) {
  		reg_l33 = regulator_get(NULL, "8917_l33");
  		if (IS_ERR(reg_l33)) {
  			pr_err("could not get 8917_l33, rc = %ld=n",
  				PTR_ERR(reg_l33));
  			return;
  		}
  		ret = regulator_set_voltage(reg_l33, 3300000, 3300000);
  		if (ret) {
  			pr_err("%s: unable to set ldo33 voltage to 3.3V\n",
  				__func__);
  			return;
  		}
  	}
  
  	if (onoff) {
  		if (!regulator_is_enabled(reg_l33)) {
  			ret = regulator_enable(reg_l33);
  			if (ret) {
  				pr_err("enable l33 failed, rc=%d\n", ret);
  				return;
  			}
  			pr_info("keyled 3.3V on is finished.\n");
  		} else
  			pr_info("keyled 3.3V is already on.\n");
  	} else {
  		if (regulator_is_enabled(reg_l33)) {
  			ret = regulator_disable(reg_l33);
  		if (ret) {
  			pr_err("disable l33 failed, rc=%d\n", ret);
  			return;
  		}
  		pr_info("keyled 3.3V off is finished.\n");
  		} else
  			pr_info("keyled 3.3V is already off.\n");
  	}
}
#endif

void synaptics_power_onoff(bool enable)
{
	int ret = 0;
	static struct regulator *reg_l31;		/* 3.3V */
	static struct regulator *reg_lvs6;		/* 1.8V */ 

	if (!reg_lvs6) {
		reg_lvs6 = regulator_get(NULL, "8917_lvs6");
		if (IS_ERR(reg_lvs6)) {
			printk(KERN_ERR"could not get 8917_lvs6, rc = %ld\n",
					PTR_ERR(reg_lvs6));
			return;
		}
	}
	
	if (!reg_l31) {
		reg_l31 = regulator_get(NULL, "8917_l31");
		if (IS_ERR(reg_l31)) {
			printk(KERN_ERR"could not get 8917_l31, rc = %ld\n",
					PTR_ERR(reg_l31));
			return;
		}
		ret = regulator_set_voltage(reg_l31, 3300000, 3300000);
		if (ret) {
			printk(KERN_ERR"%s: unable to set l31 voltage to 3.3V\n",
					__func__);
			return;
		}
	}

	if (enable) {
		if (!regulator_is_enabled(reg_lvs6)) {
			ret = regulator_enable(reg_lvs6);
			if (ret) {
				printk(KERN_ERR"enable lvs6 failed, rc=%d\n", ret);
			} else {
				printk(KERN_INFO "%s: enable lvs6\n", __func__);
			}
		}

		if (!regulator_is_enabled(reg_l31)) {
			ret = regulator_enable(reg_l31);
			if (ret) {
				printk(KERN_ERR"enable l31 failed, rc=%d\n", ret);
			} else {
				printk(KERN_INFO "%s: enable reg_l31\n", __func__);
			}
		}
	} else {
		if (regulator_is_enabled(reg_lvs6)) {
			ret = regulator_disable(reg_lvs6);
			if (ret) {
				printk(KERN_ERR"disable lvs6 failed, rc=%d\n", ret);
			} else {
				printk(KERN_INFO "%s: disable lvs6\n", __func__);
			}
		}

		if (regulator_is_enabled(reg_l31)) {
			ret = regulator_disable(reg_l31);
			if (ret) {
				printk(KERN_ERR"disable l31 failed, rc=%d\n", ret);
			} else {
				printk(KERN_INFO "%s: disable reg_l31\n", __func__);
			}
		}
	}
	
}

void I2C_gpio_set (bool enable)
{
	if (enable) {
		gpio_tlmm_config(GPIO_CFG(GPIO_TOUCH_SDA, 1,
			GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);
		gpio_tlmm_config(GPIO_CFG(GPIO_TOUCH_SCL, 1,
			GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);
		printk(KERN_INFO "%s: set IN/NP tsp i2c\n", __func__);
	} else {
		gpio_tlmm_config(GPIO_CFG(GPIO_TOUCH_SDA,0 ,
			GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), 1);
		gpio_tlmm_config(GPIO_CFG(GPIO_TOUCH_SCL, 0,
			GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), 1);
		printk(KERN_INFO "%s: set IN/PD tsp i2c\n", __func__);
	}
}

static unsigned char tm1940_f1a_button_codes[] = {KEY_MENU, KEY_BACK};

static struct synaptics_rmi_f1a_button_map tm1940_f1a_button_map = {
	.nbuttons = ARRAY_SIZE(tm1940_f1a_button_codes),
	.map = tm1940_f1a_button_codes,
};

#define SYNAPTICS_MAX_X_SIZE	719
#define SYNAPTICS_MAX_Y_SIZE	1279
#define SYNAPTICS_MAX_WIDTH	SYNAPTICS_MAX_Y_SIZE

static struct synaptics_rmi4_platform_data rmi4_platformdata = {
	.irq_type = IRQF_TRIGGER_FALLING,
	/*Direct IRQ for secure input*/
	//.irq_type = IRQF_TRIGGER_RISING | IRQF_ONESHOT,
	.gpio = GPIO_TOUCH_IRQ,
	.max_x = SYNAPTICS_MAX_X_SIZE,
	.max_y = SYNAPTICS_MAX_Y_SIZE,
	.max_width = SYNAPTICS_MAX_WIDTH,
	/*.gpio_config = synaptics_gpio_setup,*/
	.f1a_button_map = &tm1940_f1a_button_map,
	.power = synaptics_power_onoff,
	.i2c_set = I2C_gpio_set,
	.tkey_led_vdd_on = NULL,
	.register_cb = synaptics_tsp_register_callback,
};

static struct i2c_board_info bus2_i2c_devices[] = {
     /* SYNAPTICS I2C */
	{
	I2C_BOARD_INFO("synaptics_rmi4_i2c", 0x20),
	.platform_data = &rmi4_platformdata,
	.irq = MSM_GPIO_TO_INT(GPIO_TOUCH_IRQ),
	},
};

void __init S5000_tsp_input_init(void)
{
	int ret;
	unsigned int touch_irq_gpio;
	pr_err("[TSP] inside S5000_tsp_input_init\n");
#ifdef CONFIG_LEDS_CLASS
	rmi4_platformdata.tkey_led_vdd_on = msm_tkey_led_vdd_on;
#endif

#if defined(CONFIG_MACH_MELIUS_CHN_CTC) || defined(CONFIG_MACH_CRATER_CHN_CTC)
	rmi4_platformdata.gpio = GPIO_TOUCH_IRQ_CTC_REV00;
	bus2_i2c_devices[0].irq = MSM_GPIO_TO_INT(GPIO_TOUCH_IRQ_CTC_REV00);
	touch_irq_gpio = GPIO_TOUCH_IRQ_CTC_REV00;
#else
#if	defined(CONFIG_MACH_MELIUS_EUR_LTE) \
	|| defined(CONFIG_MACH_MELIUS_EUR_OPEN) \
	|| defined(CONFIG_MACH_MELIUS_ATT) \
	|| defined(CONFIG_MACH_MELIUS_TMO) \
	|| defined(CONFIG_MACH_MELIUS_SKT) \
	|| defined(CONFIG_MACH_MELIUS_KTT) \
	|| defined(CONFIG_MACH_MELIUS_LGT)
	if(msm8930_get_board_rev() > 6) {
		rmi4_platformdata.gpio = GPIO_TOUCH_IRQ_MAIN_REV03;
		bus2_i2c_devices[0].irq = MSM_GPIO_TO_INT(GPIO_TOUCH_IRQ_MAIN_REV03);
		touch_irq_gpio = GPIO_TOUCH_IRQ_MAIN_REV03;			
	} else if(msm8930_get_board_rev() > 1) {
#elif	defined(CONFIG_MACH_MELIUS_VZW) \
	|| defined(CONFIG_MACH_MELIUS_SPR)
	if(msm8930_get_board_rev() > 0) {
		rmi4_platformdata.gpio = GPIO_TOUCH_IRQ_MAIN_REV03;
		bus2_i2c_devices[0].irq = MSM_GPIO_TO_INT(GPIO_TOUCH_IRQ_MAIN_REV03);
		touch_irq_gpio = GPIO_TOUCH_IRQ_MAIN_REV03;
	} else {
#elif	defined(CONFIG_MACH_MELIUS_USC)
	if((msm8930_get_board_rev() > 0) && (msm8930_get_board_rev() != 5)) {
		rmi4_platformdata.gpio = GPIO_TOUCH_IRQ_MAIN_REV03;
		bus2_i2c_devices[0].irq = MSM_GPIO_TO_INT(GPIO_TOUCH_IRQ_MAIN_REV03);
		touch_irq_gpio = GPIO_TOUCH_IRQ_MAIN_REV03;
	} else {
#endif
		rmi4_platformdata.gpio = GPIO_TOUCH_IRQ_REV02;
		bus2_i2c_devices[0].irq = MSM_GPIO_TO_INT(GPIO_TOUCH_IRQ_REV02);
		touch_irq_gpio = GPIO_TOUCH_IRQ_REV02;
#if	defined(CONFIG_MACH_MELIUS_EUR_LTE) \
	|| defined(CONFIG_MACH_MELIUS_EUR_OPEN) \
	|| defined(CONFIG_MACH_MELIUS_ATT) \
	|| defined(CONFIG_MACH_MELIUS_TMO) \
	|| defined(CONFIG_MACH_MELIUS_SKT) \
	|| defined(CONFIG_MACH_MELIUS_KTT) \
	|| defined(CONFIG_MACH_MELIUS_LGT)
	} else {
		rmi4_platformdata.gpio = GPIO_TOUCH_IRQ;
		bus2_i2c_devices[0].irq = MSM_GPIO_TO_INT(GPIO_TOUCH_IRQ);
		touch_irq_gpio = GPIO_TOUCH_IRQ;
	}
#elif	defined(CONFIG_MACH_MELIUS_VZW) \
	|| defined(CONFIG_MACH_MELIUS_SPR) \
	|| defined(CONFIG_MACH_MELIUS_USC)
	}
#endif
#endif
		
	pr_err("[TSP]START S5000_tsp_input_init\n"); 
	gpio_free(touch_irq_gpio);
	ret = gpio_request(touch_irq_gpio, "tsp_int");
	if (ret != 0) {
		printk("tsp int request failed, ret=%d", ret);
		goto err_int_gpio_request;
	}
	gpio_tlmm_config(GPIO_CFG(touch_irq_gpio, 0,
		GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);
	gpio_tlmm_config(GPIO_CFG(GPIO_TOUCH_SDA, 1,
		GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);
	gpio_tlmm_config(GPIO_CFG(GPIO_TOUCH_SCL, 1,
		GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);

	i2c_register_board_info(MSM_8960_GSBI3_QUP_I2C_BUS_ID, bus2_i2c_devices,
			ARRAY_SIZE(bus2_i2c_devices));
	pr_err("[TSP]END S5000_tsp_input_init\n");
	return ;

err_int_gpio_request:
	printk("[TSP] err_int_gpio_request");
	gpio_free(touch_irq_gpio);
}