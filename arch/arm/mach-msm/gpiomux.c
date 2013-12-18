/* Copyright (c) 2010, The Linux Foundation. All rights reserved.
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
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <mach/gpiomux.h>

#include <linux/gpio.h>
#include <mach/irqs.h>

#ifdef CONFIG_SEC_GPIO_DVS
#include <linux/errno.h>
#include <linux/io.h>
#include <mach/msm_iomap.h>

#include <linux/secgpio_dvs.h>
#include <linux/platform_device.h>
#endif

struct msm_gpiomux_rec {
	struct gpiomux_setting *sets[GPIOMUX_NSETTINGS];
	int ref;
};
static DEFINE_SPINLOCK(gpiomux_lock);
static struct msm_gpiomux_rec *msm_gpiomux_recs;
static struct gpiomux_setting *msm_gpiomux_sets;
static unsigned msm_gpiomux_ngpio;

#ifdef CONFIG_SEC_GPIO_DVS

/****************************************************************/
/* Define value in accordance with
	the specification of each BB vendor. */
#define AP_GPIO_COUNT	90
/****************************************************************/

enum {
	GPIO_IN_BIT  = 0,
	GPIO_OUT_BIT = 1
};

#define GPIO_IN_OUT(gpio)        (MSM_TLMM_BASE + 0x1004 + (0x10 * (gpio)))

#define GET_RESULT_GPIO(a, b, c)	\
	((a<<4 & 0xF0) | (b<<1 & 0xE) | (c & 0x1))

/****************************************************************/
/* Pre-defined variables. (DO NOT CHANGE THIS!!) */
static unsigned char checkgpiomap_result[GDVS_PHONE_STATUS_MAX][AP_GPIO_COUNT];
static struct gpiomap_result gpiomap_result = {
	.init = checkgpiomap_result[PHONE_INIT],
	.sleep = checkgpiomap_result[PHONE_SLEEP]
};
/****************************************************************/

static unsigned __msm_gpio_get_inout_lh(unsigned gpio)
{
	return __raw_readl(GPIO_IN_OUT(gpio)) & BIT(GPIO_IN_BIT);
}

static void msm8x30_check_gpio_status(unsigned char phonestate)
{
	struct gpiomux_setting val;

	u32 i;

	u8 temp_io = 0, temp_pdpu = 0, temp_lh = 0;

	pr_info("[secgpio_dvs][%s] state : %s\n", __func__,
		(phonestate == PHONE_INIT) ? "init" : "sleep");

	for (i = 0; i < AP_GPIO_COUNT; i++) {
		msm_gpiomux_read(i, &val);

		if (val.func == GPIOMUX_FUNC_GPIO) {
			if (val.dir == GPIOMUX_IN)
				temp_io = 0x01;	/* GPIO_IN */
			else if (val.dir == GPIOMUX_OUT_HIGH ||
					val.dir == GPIOMUX_OUT_LOW)
				temp_io = 0x02;	/* GPIO_OUT */
			else {
				temp_io = 0xF;	/* not alloc. */
				pr_err("[secgpio_dvs] gpio : %d, val.dir : %d, temp_io = 0x3",
					i, val.dir);
			}
		} else {
			temp_io = 0x0;		/* FUNC */
		}

		if (val.pull  == GPIOMUX_PULL_NONE)
			temp_pdpu = 0x00;
		else if (val.pull  == GPIOMUX_PULL_DOWN)
			temp_pdpu = 0x01;
		else if (val.pull == GPIOMUX_PULL_UP)
			temp_pdpu = 0x02;
		else if (val.pull == GPIOMUX_PULL_KEEPER)
			temp_pdpu = 0x03;
		else {
			temp_pdpu = 0x07;
			pr_err("[secgpio_dvs] gpio : %d, val.pull : %d, temp_pdpu : %d",
				i, val.pull, temp_pdpu);
		}

		if (val.func == GPIOMUX_FUNC_GPIO) {
			if (val.dir == GPIOMUX_OUT_LOW)
				temp_lh = 0x00;
			else if (val.dir == GPIOMUX_OUT_HIGH)
				temp_lh = 0x01;
			else if (val.dir == GPIOMUX_IN)
				temp_lh = __msm_gpio_get_inout_lh(i);
		} else
			temp_lh = 0;


		checkgpiomap_result[phonestate][i] =
			GET_RESULT_GPIO(temp_io, temp_pdpu, temp_lh);
	}

	pr_info("[secgpio_dvs][%s]-\n", __func__);

	return;
}

/****************************************************************/
/* Define appropriate variable in accordance with
	the specification of each BB vendor */
static struct gpio_dvs msm8x30_gpio_dvs = {
	.result = &gpiomap_result,
	.check_gpio_status = msm8x30_check_gpio_status,
	.count = AP_GPIO_COUNT,
};
/****************************************************************/
#endif

int msm_gpiomux_write(unsigned gpio, enum msm_gpiomux_setting which,
	struct gpiomux_setting *setting, struct gpiomux_setting *old_setting)
{
	struct msm_gpiomux_rec *rec = msm_gpiomux_recs + gpio;
	unsigned set_slot = gpio * GPIOMUX_NSETTINGS + which;
	unsigned long irq_flags;
	struct gpiomux_setting *new_set;
	int status = 0;

	if (!msm_gpiomux_recs)
		return -EFAULT;

	if (gpio >= msm_gpiomux_ngpio)
		return -EINVAL;

	spin_lock_irqsave(&gpiomux_lock, irq_flags);

	if (old_setting) {
		if (rec->sets[which] == NULL)
			status = 1;
		else
			*old_setting =  *(rec->sets[which]);
	}

	if (setting) {
		msm_gpiomux_sets[set_slot] = *setting;
		rec->sets[which] = &msm_gpiomux_sets[set_slot];
	} else {
		rec->sets[which] = NULL;
	}

	new_set = rec->ref ? rec->sets[GPIOMUX_ACTIVE] :
		rec->sets[GPIOMUX_SUSPENDED];
	if (new_set)
		__msm_gpiomux_write(gpio, *new_set);

	spin_unlock_irqrestore(&gpiomux_lock, irq_flags);
	return status;
}
EXPORT_SYMBOL(msm_gpiomux_write);

int msm_gpiomux_get(unsigned gpio)
{
	struct msm_gpiomux_rec *rec = msm_gpiomux_recs + gpio;
	unsigned long irq_flags;

	if (!msm_gpiomux_recs)
		return -EFAULT;

	if (gpio >= msm_gpiomux_ngpio)
		return -EINVAL;

	spin_lock_irqsave(&gpiomux_lock, irq_flags);
	if (rec->ref++ == 0 && rec->sets[GPIOMUX_ACTIVE])
		__msm_gpiomux_write(gpio, *rec->sets[GPIOMUX_ACTIVE]);
	spin_unlock_irqrestore(&gpiomux_lock, irq_flags);
	return 0;
}
EXPORT_SYMBOL(msm_gpiomux_get);

int msm_gpiomux_put(unsigned gpio)
{
	struct msm_gpiomux_rec *rec = msm_gpiomux_recs + gpio;
	unsigned long irq_flags;

	if (!msm_gpiomux_recs)
		return -EFAULT;

	if (gpio >= msm_gpiomux_ngpio)
		return -EINVAL;

	spin_lock_irqsave(&gpiomux_lock, irq_flags);
	BUG_ON(rec->ref == 0);
	if (--rec->ref == 0 && rec->sets[GPIOMUX_SUSPENDED])
		__msm_gpiomux_write(gpio, *rec->sets[GPIOMUX_SUSPENDED]);
	spin_unlock_irqrestore(&gpiomux_lock, irq_flags);
	return 0;
}
EXPORT_SYMBOL(msm_gpiomux_put);

static const char * const gpiomux_drv_str[] = {
	"DRV_2mA",
	"DRV_4mA",
	"DRV_6mA",
	"DRV_8mA",
	"DRV_10mA",
	"DRV_12mA",
	"DRV_14mA",
	"DRV_16mA",
};

static const char * const gpiomux_func_str[] = {
	"GPIO",
	"Func_1",
	"Func_2",
	"Func_3",
	"Func_4",
	"Func_5",
	"Func_6",
	"Func_7",
	"Func_8",
	"Func_9",
	"Func_a",
	"Func_b",
	"Func_c",
	"Func_d",
	"Func_e",
	"Func_f",
};

static const char * const gpiomux_pull_str[] = {
	"PULL_NONE",
	"PULL_DOWN",
	"PULL_KEEPER",
	"PULL_UP",
};

static const char * const gpiomux_dir_str[] = {
	"IN",
	"OUT_HIGH",
	"OUT_LOW",
};

static const char * const gpiomux_val_str[] = {
	"VAL_LOW",
	"VAL_HIGH",
};

void msm_gpio_print_enabled(void)
{
	unsigned long flags;
	struct gpiomux_setting set;
	unsigned val = 0;
	unsigned gpio;
	unsigned begin = 0;

	spin_lock_irqsave(&gpiomux_lock, flags);

	for (gpio = begin; gpio < NR_GPIO_IRQS; ++gpio) {
		msm_gpiomux_read(gpio, &set);
		val = gpio_get_value(gpio);
		printk(KERN_INFO"GPIO[%u] \t%s \t%s \t%s \t%s \t%s\n", gpio,
			gpiomux_func_str[set.func],
			gpiomux_dir_str[set.dir],
			gpiomux_pull_str[set.pull],
			gpiomux_drv_str[set.drv],
			gpiomux_val_str[val]);
	}

	spin_unlock_irqrestore(&gpiomux_lock, flags);
}

int msm_gpiomux_init(size_t ngpio)
{
	if (!ngpio)
		return -EINVAL;

	if (msm_gpiomux_recs)
		return -EPERM;

	msm_gpiomux_recs = kzalloc(sizeof(struct msm_gpiomux_rec) * ngpio,
				   GFP_KERNEL);
	if (!msm_gpiomux_recs)
		return -ENOMEM;

	/* There is no need to zero this memory, as clients will be blindly
	 * installing settings on top of it.
	 */
	msm_gpiomux_sets = kmalloc(sizeof(struct gpiomux_setting) * ngpio *
		GPIOMUX_NSETTINGS, GFP_KERNEL);
	if (!msm_gpiomux_sets) {
		kfree(msm_gpiomux_recs);
		msm_gpiomux_recs = NULL;
		return -ENOMEM;
	}

	msm_gpiomux_ngpio = ngpio;

	return 0;
}
EXPORT_SYMBOL(msm_gpiomux_init);

void msm_gpiomux_install(struct msm_gpiomux_config *configs, unsigned nconfigs)
{
	unsigned c, s;
	int rc;

	for (c = 0; c < nconfigs; ++c) {
		for (s = 0; s < GPIOMUX_NSETTINGS; ++s) {
			rc = msm_gpiomux_write(configs[c].gpio, s,
				configs[c].settings[s], NULL);
			if (rc)
				pr_err("%s: write failure: %d\n", __func__, rc);
		}
	}
}
EXPORT_SYMBOL(msm_gpiomux_install);

#ifdef CONFIG_SEC_GPIO_DVS
static struct platform_device secgpio_dvs_device = {
	.name	= "secgpio_dvs",
	.id		= -1,
/****************************************************************/
/* Designate appropriate variable pointer
	in accordance with the specification of each BB vendor. */
	.dev.platform_data = &msm8x30_gpio_dvs,
/****************************************************************/
};

static struct platform_device *secgpio_dvs_devices[] __initdata = {
	&secgpio_dvs_device,
};

static int __init secgpio_dvs_device_init(void)
{
	return platform_add_devices(
		secgpio_dvs_devices, ARRAY_SIZE(secgpio_dvs_devices));
}
arch_initcall(secgpio_dvs_device_init);
#endif
