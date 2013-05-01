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
#include <linux/i2c/mxts.h>
#include <linux/delay.h>

#define GPIO_TOUCH_IRQ		57

/* touch is on i2c3 */
#define GPIO_TOUCH_SDA		8
#define GPIO_TOUCH_SCL		9

#define MXT_BOOT_ADDRESS	0x24
#define MXT_APP_ADDRESS		0x4A



static struct charging_status_callbacks {
	void (*tsp_set_charging_cable) (int type);
} charging_cbs;

void tsp_register_callback(void *function)
{
	charging_cbs.tsp_set_charging_cable = function;
}

static bool ts_read_chg(void)
{
	return gpio_get_value(GPIO_TOUCH_IRQ);
}

static int mxt540s_power_on(void)
{
	int ret = 0;
	/* 3.3V */
	static struct regulator *reg_l17;
	/* 3.0V */
	static struct regulator *reg_l16;
	/* 1.8V */
	static struct regulator *reg_l22;

	if (!reg_l17) {
		reg_l17 = regulator_get(NULL, "8921_l17");
		if (IS_ERR(reg_l17)) {
			printk(KERN_ERR"could not get 8921_l17, rc = %ld\n",
					PTR_ERR(reg_l17));
			return -EIO;
		}
		ret = regulator_set_voltage(reg_l17, 3300000, 3300000);
		if (ret) {
			printk(KERN_ERR"%s: unable to set ldo17 voltage to 3.3V\n",
					__func__);
			return -EIO;
		}
	}
	ret = regulator_enable(reg_l17);
	if (ret) {
		printk(KERN_ERR"enable l17 failed, rc=%d\n", ret);
		return -EIO;
	}

	if (!reg_l16) {
		reg_l16 = regulator_get(NULL, "8921_l16");
		if (IS_ERR(reg_l16)) {
			printk(KERN_ERR"could not get 8921_l16, rc = %ld\n",
				PTR_ERR(reg_l16));
			return -EIO;
			}
		ret = regulator_set_voltage(reg_l16, 3000000, 3000000);
		if (ret) {
			printk(KERN_ERR"%s: unable to set ldo16 voltage to 3.0V\n",
				__func__);
			return -EIO;
			}
		}
	ret = regulator_enable(reg_l16);
	if (ret) {
		printk(KERN_ERR"enable l16 failed, rc=%d\n", ret);
		return -EIO;
		}

	if (!reg_l22) {
		reg_l22 = regulator_get(NULL, "8921_l22");
		if (IS_ERR(reg_l22)) {
			printk(KERN_ERR"could not get 8921_l22, rc = %ld\n",
					PTR_ERR(reg_l22));
			return -EIO;
		}
	}
	ret = regulator_set_voltage(reg_l22, 1800000, 1800000);
	if (ret) {
		printk(KERN_ERR"%s: unable to set ldo22 voltage to 1.8V\n",
				__func__);
		return -EIO;
	}

	ret = regulator_enable(reg_l22);
	if (ret) {
		printk(KERN_ERR"enable lvs22 failed, rc=%d\n", ret);
		return -EIO;
	}

	msleep(130);

	printk(KERN_DEBUG "mxt540E_power_on is finished\n");

	return 0;
}

static int mxt540s_power_off(void)
{
	int ret = 0;
	/* 3.3V */
	static struct regulator *reg_l17;
	/* 3.0V */
	static struct regulator *reg_l16;
	/* 1.8V */
	static struct regulator *reg_l22;

	if (!reg_l22) {
		reg_l22 = regulator_get(NULL, "8921_l22");
		if (IS_ERR(reg_l22)) {
			printk(KERN_ERR"could not get 8921_l22, rc = %ld\n",
					PTR_ERR(reg_l22));
			return -EIO;
		}
	}
	ret = regulator_set_voltage(reg_l22, 1800000, 1800000);
	if (ret) {
		printk(KERN_ERR"%s: unable to set ldo22 voltage to 1.8V\n",
				__func__);
		return -EIO;
	}

	if (regulator_is_enabled(reg_l22))
		ret = regulator_disable(reg_l22);
	if (ret) {
		printk(KERN_ERR"enable lvs16 failed, rc=%d\n", ret);
		return -EIO;
	}
	 printk(KERN_ERR "tsp 1.8V off is finished.\n");

	if (!reg_l16) {
		reg_l16 = regulator_get(NULL, "8921_l16");
		if (IS_ERR(reg_l16)) {
			printk(KERN_ERR"could not get 8921_l22, rc = %ld\n",
					PTR_ERR(reg_l16));
			return -EIO;
		}
	}

	ret = regulator_set_voltage(reg_l16, 3000000, 3000000);
	if (ret) {
		printk(KERN_ERR"%s: unable to set ldo22 voltage to 1.8V\n",
				__func__);
		return -EIO;
	}

	if (regulator_is_enabled(reg_l16))
		ret = regulator_disable(reg_l16);
	if (ret) {
		printk(KERN_ERR"enable lvs16 failed, rc=%d\n", ret);
		return -EIO;
	}
	 printk(KERN_DEBUG "tsp 3.0V off is finished.\n");

	if (!reg_l17) {
		reg_l17 = regulator_get(NULL, "8921_l17");
		if (IS_ERR(reg_l17)) {
			printk(KERN_ERR"could not get 8921_l17, rc = %ld\n",
					PTR_ERR(reg_l17));
			return -EIO;
		}
		ret = regulator_set_voltage(reg_l17, 3300000, 3300000);
		if (ret) {
			printk(KERN_ERR"%s: unable to set ldo17 voltage to 3.3V\n",
					__func__);
			return -EIO;
		}
	}

	if (regulator_is_enabled(reg_l17))
		ret = regulator_disable(reg_l17);

	if (ret) {
		printk(KERN_ERR"disable l17 failed, rc=%d\n", ret);
		return -EIO;
	}
	printk(KERN_DEBUG "tsp 3.3V off is finished.\n");

	return 0;
}

#define MXT_FIRMWARE_NAME	"mXT540S.fw"
#define MXT_FIRMWARE_I_NAME	"mXT540Si.fw"

#define XNODE 27
#define YNODE 15
#define XNODE_REVI 28
#define YNODE_REVI 16


#define MXT_PROJECT_NAME	"JF"

static void ts_register_callback(void *cb)
{
	/*charger_callbacks = cb;*/
	pr_debug("[TSP] ts_register_callback\n");
}

static struct mxt_platform_data mxt_data = {

	.num_xnode = XNODE,
	.num_ynode = YNODE,
	.max_x = 4095,
	.max_y = 4095,
	.irqflags = IRQF_TRIGGER_LOW | IRQF_ONESHOT,
	.boot_address = MXT_BOOT_ADDRESS,
	.firmware_name = MXT_FIRMWARE_NAME,
	.project_name = MXT_PROJECT_NAME,
	.revision = MXT_REVISION_I,
	.read_chg = ts_read_chg,
	.power_on = mxt540s_power_on,
	.power_off = mxt540s_power_off,
	.register_cb = ts_register_callback,
};

static struct i2c_board_info __initdata i2c_devs3_a[] = {
	{
		I2C_BOARD_INFO(MXT_DEV_NAME, 0x4a),
		.platform_data = &mxt_data,
		.irq = MSM_GPIO_TO_INT(GPIO_TOUCH_IRQ),
	},
};

void __init mxt540s_tsp_input_init(void)
{
	int ret;

	ret = gpio_request(GPIO_TOUCH_IRQ, "tsp_int");
	if (ret != 0) {
		printk(KERN_ERR"tsp int request failed, ret=%d", ret);
		goto err_int_gpio_request;
	}

	gpio_tlmm_config(GPIO_CFG(GPIO_TOUCH_IRQ, 0,
		GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);

		i2c_register_board_info(APQ_8064_GSBI3_QUP_I2C_BUS_ID,
			i2c_devs3_a, ARRAY_SIZE(i2c_devs3_a));

		if (system_rev >= 4) {
			mxt_data.firmware_name = MXT_FIRMWARE_I_NAME;
			mxt_data.revision = MXT_REVISION_I;
			mxt_data.num_xnode = XNODE_REVI;
			mxt_data.num_ynode = YNODE_REVI;
		}

		else {
			mxt_data.firmware_name = MXT_FIRMWARE_NAME;
			mxt_data.revision = MXT_REVISION_G;
			mxt_data.num_xnode = XNODE;
			mxt_data.num_ynode = YNODE;
		}

		printk(KERN_DEBUG "[TSP]MXT %s, Xnode, Ynode %d %d\n",
				mxt_data.firmware_name,
				mxt_data.num_xnode, mxt_data.num_ynode);

	return ;

err_int_gpio_request:
	gpio_free(GPIO_TOUCH_IRQ);
}
