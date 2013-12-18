/*
 * LED driver for Maxim MAX77693 - leds-max77673.c
 *
 * Copyright (C) 2011 ByungChang Cha <bc.cha@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/leds.h>
#include <linux/workqueue.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/mfd/max77693.h>
#include <linux/mfd/max77693-private.h>
#include <linux/leds-max77693.h>
#include <linux/ctype.h>

#if defined(CONFIG_SEC_TORCH_FLASH)
#include <linux/sec_torch.h>
#endif

#ifdef CONFIG_LEDS_SWITCH
#include <linux/gpio.h>
#define FLASH_SWITCH_REMOVED_REVISION	0x05
#endif 

struct max77693_led_data {
	struct led_classdev led;
	struct max77693_dev *max77693;
	struct max77693_led *data;
	struct i2c_client *i2c;
	struct work_struct work;
	struct mutex lock;
	spinlock_t value_lock;
	int brightness;
	int test_brightness;
#ifdef CONFIG_MACH_JF_DCM
	int movie_brightness;
#endif 
};

static u8 led_en_mask[MAX77693_LED_MAX] = {
	MAX77693_FLASH_FLED1_EN,
	MAX77693_TORCH_FLED1_EN,
};

static u8 led_en_shift[MAX77693_LED_MAX] = {
	6,
	2,
};

static u8 reg_led_timer[MAX77693_LED_MAX] = {
	MAX77693_LED_REG_FLASH_TIMER,
	MAX77693_LED_REG_ITORCHTORCHTIMER,
};

static u8 reg_led_current[MAX77693_LED_MAX] = {
	MAX77693_LED_REG_IFLASH,
	MAX77693_LED_REG_ITORCH,
};

static u8 led_current_mask[MAX77693_LED_MAX] = {
	MAX77693_FLASH_IOUT,
	MAX77693_TORCH_IOUT,
};

static u8 led_current_shift[MAX77693_LED_MAX] = {
	0,
	0,
};

extern struct class *camera_class; /*sys/class/camera*/
struct device *flash_dev;
#if defined(CONFIG_SEC_TORCH_FLASH)
struct max77693_led_data *torchLED;
#endif

static int max77693_set_bits(struct i2c_client *client, const u8 reg,
			     const u8 mask, const u8 inval)
{
	int ret;
	u8 value;

	ret = max77693_read_reg(client, reg, &value);
	if (unlikely(ret < 0))
		return ret;

	value = (value & ~mask) | (inval & mask);

	ret = max77693_write_reg(client, reg, value);

	return ret;
}

#ifdef CONFIG_MACH_JF_DCM
static ssize_t max77693_show_movie_brightness(struct device *dev,
				     struct device_attribute *devattr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct max77693_led_data *led_data = container_of(led_cdev, struct max77693_led_data, led);

	return sprintf(buf, "%d\n", led_data->movie_brightness);
}

static ssize_t max77693_store_movie_brightness(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct max77693_led_data *led_data = container_of(led_cdev, struct max77693_led_data, led);
	unsigned long movie_brightness = simple_strtoul(buf, NULL, 0);
	int set_brightness, ret;
	u8 shift = led_current_shift[led_data->data->id];

	if (movie_brightness > 0) {
		led_data->movie_brightness = (int)movie_brightness;
		set_brightness = led_data->movie_brightness;
	} else {
		led_data->movie_brightness = 0;
		set_brightness = led_data->brightness;
	}

	pr_debug("[LED] %s:movie_brightness = %d\n", __func__, led_data->movie_brightness);

	ret = max77693_set_bits(led_data->i2c, reg_led_current[led_data->data->id],
			led_current_mask[led_data->data->id],
			set_brightness << shift);
	if (unlikely(ret))
		pr_err("%s: can't set led level %d\n", __func__, ret);

	return size;
}

static DEVICE_ATTR(movie_brightness, S_IWUSR|S_IWGRP|S_IRUGO,
	max77693_show_movie_brightness, max77693_store_movie_brightness);

#endif

/*
static void print_all_reg_value(struct i2c_client *client)
{
	u8 value;
	u8 i;

	for (i = 0; i != 0x11; ++i) {
		max77693_read_reg(client, i, &value);
		printk(KERN_ERR "LEDS_MAX77693 REG(%d) = %x\n", i, value);
	}
}
*/

static int max77693_led_get_en_value(struct max77693_led_data *led_data, int on)
{
	int mode;

	if (on)
		mode = 0x03; /*triggered via serial interface*/
	else {
		if (reg_led_timer[led_data->data->id]
        					== MAX77693_LED_REG_FLASH_TIMER)
			mode = 0x01; /*Flash triggered via FLASHEN*/
		else
			mode = 0x02; /*Torch triggered via TORCHEN*/
	}

	printk(KERN_DEBUG "[LED] %s: led mode: 0x%x, on: %d\n", __func__, mode, on);

	return mode;
}

static void max77693_led_set(struct led_classdev *led_cdev,
					enum led_brightness value)
{
	unsigned long flags;
	struct max77693_led_data *led_data
		= container_of(led_cdev, struct max77693_led_data, led);

	pr_debug("[LED] %s\n", __func__);

	spin_lock_irqsave(&led_data->value_lock, flags);
	led_data->test_brightness = min((int)value, MAX77693_FLASH_IOUT);
	spin_unlock_irqrestore(&led_data->value_lock, flags);

	schedule_work(&led_data->work);
}

static void led_set(struct max77693_led_data *led_data)
{
	int ret;
	struct max77693_led *data = led_data->data;
	int id = data->id;
	u8 shift = led_current_shift[id];
	int value;

	if (led_data->test_brightness == LED_OFF) {
		value = max77693_led_get_en_value(led_data, 0);
		ret = max77693_set_bits(led_data->i2c,
					MAX77693_LED_REG_FLASH_EN,
					led_en_mask[id],
					value << led_en_shift[id]);
		if (unlikely(ret))
			goto error_set_bits;

		ret = max77693_set_bits(led_data->i2c, reg_led_current[id],
					led_current_mask[id],
					led_data->brightness << shift);
		if (unlikely(ret))
			goto error_set_bits;

		return;
	}

	/* Set current */
	printk(KERN_DEBUG "[LED] %s: Set on current: %#x\n", __func__, led_data->test_brightness << shift);
	ret = max77693_set_bits(led_data->i2c, reg_led_current[id],
				led_current_mask[id],
				led_data->test_brightness << shift);
	if (unlikely(ret))
		goto error_set_bits;

	/* Turn on LED */
	value = max77693_led_get_en_value(led_data, 1);
	ret = max77693_set_bits(led_data->i2c, MAX77693_LED_REG_FLASH_EN,
				led_en_mask[id],
				value << led_en_shift[id]);

	if (unlikely(ret))
		goto error_set_bits;

	return;

error_set_bits:
	pr_err("%s: can't set led level %d\n", __func__, ret);
	return;
}

static void max77693_led_work(struct work_struct *work)
{
	struct max77693_led_data *led_data
		= container_of(work, struct max77693_led_data, work);

	pr_debug("[LED] %s\n", __func__);

	mutex_lock(&led_data->lock);
	led_set(led_data);
	mutex_unlock(&led_data->lock);
}

static int max77693_led_setup(struct max77693_led_data *led_data)
{
	int ret = 0;
	struct max77693_led *data = led_data->data;
	int id = data->id;
	int value;
	
#if defined(CONFIG_SEC_TORCH_FLASH)
	torchLED = led_data;
#endif	

	ret |= max77693_write_reg(led_data->i2c, MAX77693_LED_REG_VOUT_CNTL,
				  MAX77693_BOOST_FLASH_MODE_FLED1);
	ret |= max77693_write_reg(led_data->i2c, MAX77693_LED_REG_VOUT_FLASH,
				  MAX77693_BOOST_VOUT_FLASH_FROM_VOLT(3300));
	ret |= max77693_write_reg(led_data->i2c, MAX77693_CHG_REG_CHG_CNFG_11, 0x00);
	ret |= max77693_write_reg(led_data->i2c,
				MAX77693_LED_REG_MAX_FLASH1, 0xBC);
	ret |= max77693_write_reg(led_data->i2c,
				MAX77693_LED_REG_MAX_FLASH2, 0x00);

	value = max77693_led_get_en_value(led_data, 0);

	ret |= max77693_set_bits(led_data->i2c, MAX77693_LED_REG_FLASH_EN,
				 led_en_mask[id],
				 value << led_en_shift[id]);

	/* Set TORCH_TMR_DUR or FLASH_TMR_DUR */
	if (reg_led_timer[id] == MAX77693_LED_REG_FLASH_TIMER) {
		ret |= max77693_write_reg(led_data->i2c, reg_led_timer[id],
					(data->timer | data->timer_mode << 7));
	} else {
		ret |= max77693_write_reg(led_data->i2c, reg_led_timer[id],
					0xC0);
	}

	/* Set current */
	ret |= max77693_set_bits(led_data->i2c, reg_led_current[id],
				led_current_mask[id],
				led_data->brightness << led_current_shift[id]);

	return ret;
}

static ssize_t max77693_flash(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	ssize_t ret = -EINVAL;
	char *after;
	unsigned long state = simple_strtoul(buf, &after, 10);
	size_t count = after - buf;
	printk(KERN_DEBUG "[LED] %s: requested flash: %s\n", __func__, buf);

	if (isspace(*after))
		count++;

	if (count == size) {
		ret = count;

		if (state > led_cdev->max_brightness)
			state = led_cdev->max_brightness;
		led_cdev->brightness = state;
		if (!(led_cdev->flags & LED_SUSPENDED))
			led_cdev->brightness_set(led_cdev, state);
	}

	return ret;
}

static DEVICE_ATTR(rear_flash, S_IWUSR|S_IWGRP|S_IROTH,
	NULL, max77693_flash);

static int max77693_led_probe(struct platform_device *pdev)
{
	int ret = 0;
	int i;
	struct max77693_dev *max77693 = dev_get_drvdata(pdev->dev.parent);
	struct max77693_platform_data *max77693_pdata
		= dev_get_platdata(max77693->dev);
	struct max77693_led_platform_data *pdata = max77693_pdata->led_data;
	struct max77693_led_data *led_data;
	struct max77693_led *data;
	struct max77693_led_data **led_datas;

	if (pdata == NULL) {
		pr_err("[LED] no platform data for this led is found\n");
		return -EFAULT;
	}

	led_datas = kzalloc(sizeof(struct max77693_led_data *)
			    * MAX77693_LED_MAX, GFP_KERNEL);
	if (unlikely(!led_datas)) {
		pr_err("[LED] memory allocation error %s", __func__);
		return -ENOMEM;
	}
	platform_set_drvdata(pdev, led_datas);

	pr_debug("[LED] %s\n", __func__);

	for (i = 0; i != pdata->num_leds; ++i) {
		data = &(pdata->leds[i]);

		led_data = kzalloc(sizeof(struct max77693_led_data),
				   GFP_KERNEL);
		led_datas[i] = led_data;
		if (unlikely(!led_data)) {
			pr_err("[LED] memory allocation error %s\n", __func__);
			ret = -ENOMEM;
			continue;
		}

		led_data->max77693 = max77693;
		led_data->i2c = max77693->i2c;
		led_data->data = data;
		led_data->led.name = data->name;
		led_data->led.brightness_set = max77693_led_set;
		led_data->led.brightness = LED_OFF;
		led_data->brightness = data->brightness;
		led_data->led.flags = 0;
		led_data->led.max_brightness = reg_led_timer[data->id] == MAX77693_LED_REG_FLASH_TIMER
			? MAX_FLASH_DRV_LEVEL : MAX_TORCH_DRV_LEVEL;

		mutex_init(&led_data->lock);
		spin_lock_init(&led_data->value_lock);
		INIT_WORK(&led_data->work, max77693_led_work);

		ret = led_classdev_register(&pdev->dev, &led_data->led);
		if (unlikely(ret)) {
			pr_err("unable to register LED\n");
			kfree(led_data);
			ret = -EFAULT;
			continue;
		}

		ret = max77693_led_setup(led_data);
		if (unlikely(ret)) {
			pr_err("unable to register LED\n");
			mutex_destroy(&led_data->lock);
			led_classdev_unregister(&led_data->led);
			kfree(led_data);
			ret = -EFAULT;
		}
	}
	/* print_all_reg_value(max77693->i2c); */

	flash_dev = device_create(camera_class, NULL, 0, led_datas[1], "flash");
	if (flash_dev < 0)
		pr_err("Failed to create device(flash)!\n");

	if (device_create_file(flash_dev, &dev_attr_rear_flash) < 0) {
		pr_err("failed to create device file, %s\n",
				dev_attr_rear_flash.attr.name);
	}
 #ifdef CONFIG_MACH_JF_DCM
	if (device_create_file(flash_dev, &dev_attr_movie_brightness) < 0) {
		pr_err("failed to create device file, %s\n",
				dev_attr_movie_brightness.attr.name);
	}
#endif
#ifdef CONFIG_LEDS_SWITCH
	if (system_rev < FLASH_SWITCH_REMOVED_REVISION) {
		if (gpio_request(GPIO_CAM_SW_EN, "CAM_SW_EN"))
			pr_err("failed to request CAM_SW_EN\n");
		else
			gpio_direction_output(GPIO_CAM_SW_EN, 1);
	}
#endif

	return ret;
}

static int __devexit max77693_led_remove(struct platform_device *pdev)
{
	struct max77693_led_data **led_datas = platform_get_drvdata(pdev);
	int i;

	for (i = 0; i != MAX77693_LED_MAX; ++i) {
		if (led_datas[i] == NULL)
			continue;

		cancel_work_sync(&led_datas[i]->work);
		mutex_destroy(&led_datas[i]->lock);
		led_classdev_unregister(&led_datas[i]->led);
		kfree(led_datas[i]);
	}
	kfree(led_datas);

	device_remove_file(flash_dev, &dev_attr_rear_flash);
#ifdef CONFIG_MACH_JF_DCM
	device_remove_file(flash_dev, &dev_attr_movie_brightness);
#endif	
	device_destroy(camera_class, 0);
	class_destroy(camera_class);

	return 0;
}

void max77693_led_shutdown(struct device *dev)
{
	struct max77693_led_data **led_datas = dev_get_drvdata(dev);

	/* Turn off LED */
	max77693_set_bits(led_datas[1]->i2c,
		MAX77693_LED_REG_FLASH_EN,
		led_en_mask[1],
		0x02 << led_en_shift[1]);
}


#if defined(CONFIG_SEC_TORCH_FLASH)
void SEC_Torch_Flash_OnOff(unsigned long state)
{
	int ret = 0;
	int value = 0;


	if (state == 0) {
		printk("[Torch flash]OFF\n");
		max77693_set_bits(torchLED->i2c, MAX77693_LED_REG_FLASH_EN, led_en_mask[1], 0x02 << led_en_shift[1]);

	} else {
		printk("[Torch flash]ON\n");
		value = max77693_led_get_en_value(torchLED, 1);
		ret = max77693_set_bits(torchLED->i2c, MAX77693_LED_REG_FLASH_EN, led_en_mask[1], 0x03 << led_en_shift[1]);

		if (unlikely(ret))
			goto error_set_bits;
	}
	return;
	
error_set_bits:
	pr_err("%s: can't set led level %d\n", __func__, ret);
	return;
}
#endif

static struct platform_driver max77693_led_driver = {
	.probe		= max77693_led_probe,
	.remove		= __devexit_p(max77693_led_remove),
	.driver		= {
		.name	= "max77693-led",
		.owner	= THIS_MODULE,
		.shutdown = max77693_led_shutdown,
	},
};

static int __init max77693_led_init(void)
{
#if defined(CONFIG_SEC_TORCH_FLASH)
	Torch_Flash_OnOff = &SEC_Torch_Flash_OnOff;
#endif
	return platform_driver_register(&max77693_led_driver);
}
module_init(max77693_led_init);

static void __exit max77693_led_exit(void)
{
	platform_driver_unregister(&max77693_led_driver);
}
module_exit(max77693_led_exit);

MODULE_AUTHOR("ByungChang Cha <bc.cha@samsung.com.com>");
MODULE_DESCRIPTION("MAX77693 LED driver");
MODULE_LICENSE("GPL");
