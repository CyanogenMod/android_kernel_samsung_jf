/*
 * leds_an30259a.c - driver for panasonic AN30259A led control chip
 *
 * Copyright (C) 2012, Samsung Electronics Co. Ltd. All Rights Reserved.
 *
 * Contact: Kamaldeep Singla <kamal.singla@samsung.com>
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/sysfs_helpers.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/leds.h>
#include <linux/leds-an30259a.h>
#include <linux/workqueue.h>
#include <linux/wakelock.h>

/* AN30259A register map */
#define AN30259A_REG_SRESET		0x00
#define AN30259A_REG_LEDON		0x01
#define AN30259A_REG_SEL		0x02

#define AN30259A_REG_LED1CC		0x03
#define AN30259A_REG_LED2CC		0x04
#define AN30259A_REG_LED3CC		0x05

#define AN30259A_REG_LED1SLP		0x06
#define AN30259A_REG_LED2SLP		0x07
#define AN30259A_REG_LED3SLP		0x08

#define AN30259A_REG_LED1CNT1		0x09
#define AN30259A_REG_LED1CNT2		0x0a
#define AN30259A_REG_LED1CNT3		0x0b
#define AN30259A_REG_LED1CNT4		0x0c

#define AN30259A_REG_LED2CNT1		0x0d
#define AN30259A_REG_LED2CNT2		0x0e
#define AN30259A_REG_LED2CNT3		0x0f
#define AN30259A_REG_LED2CNT4		0x10

#define AN30259A_REG_LED3CNT1		0x11
#define AN30259A_REG_LED3CNT2		0x12
#define AN30259A_REG_LED3CNT3		0x13
#define AN30259A_REG_LED3CNT4		0x14
#define AN30259A_REG_MAX		0x15
/* MASK */
#define AN30259A_MASK_IMAX		0xc0
#define AN30259A_MASK_DELAY		0xf0
#define AN30259A_SRESET			0x01
#define LED_SLOPE_MODE			0x10
#define LED_ON				0x01

#define DUTYMAX_MAX_VALUE		0x7f
#define DUTYMIN_MIN_VALUE		0x00
#define SLPTT_MAX_VALUE			7500

#define AN30259A_TIME_UNIT		500

#define LED_R_MASK			0x00ff0000
#define LED_G_MASK			0x0000ff00
#define LED_B_MASK			0x000000ff
#define LED_R_SHIFT			16
#define LED_G_SHIFT			8
#define LED_IMAX_SHIFT			6
#define AN30259A_CTN_RW_FLG		0x80

#define LED_R_CURRENT		0x28
#define LED_G_CURRENT		0x28
#define LED_B_CURRENT		0x28
#define LED_MAX_CURRENT		0xFF
#define LED_OFF				0x00

#define	MAX_NUM_LEDS	3

u8 LED_DYNAMIC_CURRENT = 0x28;
u8 led_lowpower_mode = 0x0;

static struct an30259_led_conf led_conf[] = {
	{
		.name = "led_r",
		.brightness = LED_OFF,
		.max_brightness = LED_MAX_CURRENT,
		.flags = 0,
	}, {
		.name = "led_g",
		.brightness = LED_OFF,
		.max_brightness = LED_MAX_CURRENT,
		.flags = 0,
	}, {
		.name = "led_b",
		.brightness = LED_OFF,
		.max_brightness = LED_MAX_CURRENT,
		.flags = 0,
	}
};

enum an30259a_led_enum {
	LED_R,
	LED_G,
	LED_B,
};

enum an30259a_pattern {
	PATTERN_OFF,
	CHARGING,
	CHARGING_ERR,
	MISSED_NOTI,
	LOW_BATTERY,
	FULLY_CHARGED,
	POWERING,
};

struct an30259a_led {
	u8	channel;
	u8	brightness;
	struct led_classdev	cdev;
	struct work_struct	brightness_work;
	unsigned long delay_on_time_ms;
	unsigned long delay_off_time_ms;
};

struct an30259a_data {
	struct	i2c_client	*client;
	struct	mutex	mutex;
	struct	an30259a_led	leds[MAX_NUM_LEDS];
	u8		shadow_reg[AN30259A_REG_MAX];
};

struct i2c_client *b_client;

#define SEC_LED_SPECIFIC
#define LED_DEEP_DEBUG

#ifdef SEC_LED_SPECIFIC
static struct leds_control {
	u8 	current_low;
	u8 	current_high;
	int	blink_delay;
	bool	blink_fading;
	int	fade_in_time;
	int	fade_out_time;
	u8	fade_dt1;
	u8	fade_dt2;
	u8	fade_dt3;
	u8	fade_dt4;
} leds_control = {
	.current_low = 5,
	.current_high = 40,
	.blink_delay = 10,
	.blink_fading = true,
	.fade_in_time = 2,
	.fade_out_time = 8,
	.fade_dt1 = 1,
	.fade_dt2 = 1,
	.fade_dt3 = 2,
	.fade_dt4 = 3,
};

extern struct class *sec_class;
struct device *led_dev;
/*path : /sys/class/sec/led/led_pattern*/
/*path : /sys/class/sec/led/led_blink*/
/*path : /sys/class/sec/led/led_brightness*/
/*path : /sys/class/leds/led_r/brightness*/
/*path : /sys/class/leds/led_g/brightness*/
/*path : /sys/class/leds/led_b/brightness*/
#endif

static void leds_on(enum an30259a_led_enum led, bool on, bool slopemode,
					u8 ledcc);

static inline struct an30259a_led *cdev_to_led(struct led_classdev *cdev)
{
	return container_of(cdev, struct an30259a_led, cdev);
}

#ifdef LED_DEEP_DEBUG
static void an30259a_debug(struct i2c_client *client)
{
	struct an30259a_data *data = i2c_get_clientdata(client);
	int ret;
	u8 buff[21] = {0,};
	ret = i2c_smbus_read_i2c_block_data(client,
		AN30259A_REG_SRESET|AN30259A_CTN_RW_FLG,
		sizeof(buff), buff);
	if (ret != sizeof(buff)) {
		dev_err(&data->client->dev,
			"%s: failure on i2c_smbus_read_i2c_block_data\n",
			__func__);
	}
	print_hex_dump(KERN_ERR, "an30259a: ",
		DUMP_PREFIX_OFFSET, 32, 1, buff,
		sizeof(buff), false);
}
#endif

static int leds_i2c_write_all(struct i2c_client *client)
{
	struct an30259a_data *data = i2c_get_clientdata(client);
	int ret;

	/*we need to set all the configs setting first, then LEDON later*/
	mutex_lock(&data->mutex);
	ret = i2c_smbus_write_i2c_block_data(client,
			AN30259A_REG_SEL | AN30259A_CTN_RW_FLG,
			AN30259A_REG_MAX - AN30259A_REG_SEL,
			&data->shadow_reg[AN30259A_REG_SEL]);
	if (ret < 0) {
		dev_err(&client->adapter->dev,
			"%s: failure on i2c block write\n",
			__func__);
		goto exit;
	}
	ret = i2c_smbus_write_byte_data(client, AN30259A_REG_LEDON,
					data->shadow_reg[AN30259A_REG_LEDON]);
	if (ret < 0) {
		dev_err(&client->adapter->dev,
			"%s: failure on i2c byte write\n",
			__func__);
		goto exit;
	}
	mutex_unlock(&data->mutex);
	return 0;

exit:
	mutex_unlock(&data->mutex);
	return ret;
}

void an30259a_set_brightness(struct led_classdev *cdev,
			enum led_brightness brightness)
{
		struct an30259a_led *led = cdev_to_led(cdev);
		led->brightness = (u8)brightness;
		schedule_work(&led->brightness_work);
}

static void an30259a_led_brightness_work(struct work_struct *work)
{
		struct i2c_client *client = b_client;
		struct an30259a_led *led = container_of(work,
				struct an30259a_led, brightness_work);
		leds_on(led->channel, true, false, led->brightness);
		leds_i2c_write_all(client);
}

/*
 * leds_set_slope_mode() sets correct values to corresponding shadow registers.
 * led: stands for LED_R or LED_G or LED_B.
 * delay: represents for starting delay time in multiple of .5 second.
 * dutymax: led at slope lighting maximum PWM Duty setting.
 * dutymid: led at slope lighting middle PWM Duty setting.
 * dutymin: led at slope lighting minimum PWM Duty Setting.
 * slptt1: total time of slope operation 1 and 2, in multiple of .5 second.
 * slptt2: total time of slope operation 3 and 4, in multiple of .5 second.
 * dt1: detention time at each step in slope operation 1, in multiple of 4ms.
 * dt2: detention time at each step in slope operation 2, in multiple of 4ms.
 * dt3: detention time at each step in slope operation 3, in multiple of 4ms.
 * dt4: detention time at each step in slope operation 4, in multiple of 4ms.
 */
static void leds_set_slope_mode(struct i2c_client *client,
				enum an30259a_led_enum led, u8 delay,
				u8 dutymax, u8 dutymid, u8 dutymin,
				u8 slptt1, u8 slptt2,
				u8 dt1, u8 dt2, u8 dt3, u8 dt4)
{
	struct an30259a_data *data = i2c_get_clientdata(client);

	data->shadow_reg[AN30259A_REG_LED1CNT1 + led * 4] =
							dutymax << 4 | dutymid;
	data->shadow_reg[AN30259A_REG_LED1CNT2 + led * 4] =
							delay << 4 | dutymin;
	data->shadow_reg[AN30259A_REG_LED1CNT3 + led * 4] = dt2 << 4 | dt1;
	data->shadow_reg[AN30259A_REG_LED1CNT4 + led * 4] = dt4 << 4 | dt3;
	data->shadow_reg[AN30259A_REG_LED1SLP + led] = slptt2 << 4 | slptt1;
}

static void leds_on(enum an30259a_led_enum led, bool on, bool slopemode,
			u8 ledcc)
{
	struct an30259a_data *data = i2c_get_clientdata(b_client);

	if (on)
		data->shadow_reg[AN30259A_REG_LEDON] |= LED_ON << led;
	else {
		data->shadow_reg[AN30259A_REG_LEDON] &= ~(LED_ON << led);
		data->shadow_reg[AN30259A_REG_LED1CNT2 + led * 4] &=
							~AN30259A_MASK_DELAY;
	}
	if (slopemode)
		data->shadow_reg[AN30259A_REG_LEDON] |= LED_SLOPE_MODE << led;
	else
		data->shadow_reg[AN30259A_REG_LEDON] &=
						~(LED_SLOPE_MODE << led);

	data->shadow_reg[AN30259A_REG_LED1CC + led] = ledcc;
}

static int leds_set_imax(struct i2c_client *client, u8 imax)
{
	int ret;
	struct an30259a_data *data = i2c_get_clientdata(client);

	data->shadow_reg[AN30259A_REG_SEL] &= ~AN30259A_MASK_IMAX;
	data->shadow_reg[AN30259A_REG_SEL] |= imax << LED_IMAX_SHIFT;

	ret = i2c_smbus_write_byte_data(client, AN30259A_REG_SEL,
			data->shadow_reg[AN30259A_REG_SEL]);
	if (ret < 0) {
		dev_err(&client->adapter->dev,
			"%s: failure on i2c write\n",
			__func__);
	}
	return 0;
}

#ifdef SEC_LED_SPECIFIC
static void an30259a_reset_register_work(struct work_struct *work)
{
	int retval;
	struct i2c_client *client;
	client = b_client;

	leds_on(LED_R, false, false, 0);
	leds_on(LED_G, false, false, 0);
	leds_on(LED_B, false, false, 0);

	retval = leds_i2c_write_all(client);
	if (retval)
		printk(KERN_WARNING "leds_i2c_write_all failed\n");
}

static void an30259a_start_led_pattern(int mode)
{
	int retval;
	u8 cur;
	struct i2c_client *client;
	struct work_struct *reset = 0;
	client = b_client;

	if (mode > POWERING)
		return;
	/* Set all LEDs Off */
	an30259a_reset_register_work(reset);
	if (mode == LED_OFF)
		return;

	cur = (led_lowpower_mode) ? leds_control.current_low : leds_control.current_high;

	switch (mode) {
	/* leds_set_slope_mode(client, LED_SEL, DELAY,  MAX, MID, MIN,
		SLPTT1, SLPTT2, DT1, DT2, DT3, DT4) */
	case CHARGING:
		leds_on(LED_R, true, false, cur);
		break;

	case CHARGING_ERR:
		leds_on(LED_R, true, true, cur);
		leds_set_slope_mode(client, LED_R,
				1, 15, 15, 0, 1, 1, 0, 0, 0, 0);
		break;

	case MISSED_NOTI:
		leds_on(LED_B, true, true, cur);

		if (leds_control.blink_fading)
			leds_set_slope_mode(client, LED_B,
				leds_control.blink_delay, 15, 7, 0, 
				leds_control.fade_in_time,
				leds_control.fade_out_time,
				leds_control.fade_dt1,
				leds_control.fade_dt2,
				leds_control.fade_dt3,
				leds_control.fade_dt4);
		else
			leds_set_slope_mode(client, LED_B,
				leds_control.blink_delay, 15, 15, 0,
				leds_control.fade_in_time,
				leds_control.fade_out_time,
				0, 0, 0, 0);

		break;

	case LOW_BATTERY:
		leds_on(LED_R, true, true, cur);

		if (leds_control.blink_fading)
			leds_set_slope_mode(client, LED_R,
				leds_control.blink_delay, 15, 7, 0, 
				leds_control.fade_in_time,
				leds_control.fade_out_time,
				leds_control.fade_dt1,
				leds_control.fade_dt2,
				leds_control.fade_dt3,
				leds_control.fade_dt4);
		else
			leds_set_slope_mode(client, LED_R,
				leds_control.blink_delay, 15, 15, 0,
				leds_control.fade_in_time,
				leds_control.fade_out_time,
				0, 0, 0, 0);

		break;

	case FULLY_CHARGED:
		leds_on(LED_G, true, false, cur);
		break;

	case POWERING:
		pr_info("LED Powering Pattern on\n");
		leds_on(LED_G, true, true, LED_DYNAMIC_CURRENT);
		leds_on(LED_B, true, true, LED_DYNAMIC_CURRENT);
		leds_set_slope_mode(client, LED_G,
				0, 8, 4, 1, 2, 2, 3, 3, 3, 3);
		leds_set_slope_mode(client, LED_B,
				0, 15, 14, 12, 2, 2, 7, 7, 7, 7);
		break;

	default:
		return;
		break;
	}
	retval = leds_i2c_write_all(client);
	if (retval)
		printk(KERN_WARNING "leds_i2c_write_all failed\n");
}

static void an30259a_set_led_blink(enum an30259a_led_enum led,
					unsigned int delay_on_time,
					unsigned int delay_off_time,
					u8 brightness,
					bool force)
{
	struct i2c_client *client = b_client;
	static int prev_delay_on_time[3] = { 0 };
	static int prev_delay_off_time[3] = { 0 };
	static int prev_brightness[3] = { 0 };
	int max_brightness;

	if (force) {
		delay_on_time = prev_delay_on_time[led];
		delay_off_time = prev_delay_off_time[led];
		brightness = prev_brightness[led];
	} else {
		prev_delay_on_time[led] = delay_on_time;
		prev_delay_off_time[led] = delay_off_time;
		prev_brightness[led] = brightness;
	}

	if (brightness == LED_OFF) {
		leds_on(led, false, false, brightness);
		return;
	}

	max_brightness = (led_lowpower_mode) ?
			leds_control.current_low : leds_control.current_high;

	brightness = (brightness * max_brightness) / LED_MAX_CURRENT;

	if (delay_on_time > SLPTT_MAX_VALUE)
		delay_on_time = SLPTT_MAX_VALUE;

	if (delay_off_time > SLPTT_MAX_VALUE)
		delay_off_time = SLPTT_MAX_VALUE;

	leds_on(led, true, (delay_off_time > 0), brightness);

	if (leds_control.blink_fading)
		leds_set_slope_mode(client, led, 1, 15, 7, 0, 
				(delay_on_time + AN30259A_TIME_UNIT - 1) /
				AN30259A_TIME_UNIT,
				(delay_off_time + AN30259A_TIME_UNIT - 1) /
				AN30259A_TIME_UNIT,
				leds_control.fade_dt1,
				leds_control.fade_dt2,
				leds_control.fade_dt3,
				leds_control.fade_dt4);
	else
		leds_set_slope_mode(client, led, 1, 15, 15, 0,
				(delay_on_time + AN30259A_TIME_UNIT - 1) /
				AN30259A_TIME_UNIT,
				(delay_off_time + AN30259A_TIME_UNIT - 1) /
				AN30259A_TIME_UNIT,
				1, 1, 1, 1);
}

static ssize_t store_an30259a_led_lowpower(struct device *dev,
					struct device_attribute *devattr,
					const char *buf, size_t count)
{
	int retval;
	u8 led_lowpower;
	struct an30259a_data *data = dev_get_drvdata(dev);

	retval = kstrtou8(buf, 0, &led_lowpower);
	if (retval != 0) {
		dev_err(&data->client->dev, "fail to get led_lowpower.\n");
		return count;
	}

	led_lowpower_mode = led_lowpower;

	an30259a_set_led_blink(LED_R, 0, 0, 0, true);
	an30259a_set_led_blink(LED_G, 0, 0, 0, true);
	an30259a_set_led_blink(LED_B, 0, 0, 0, true);

	printk(KERN_DEBUG "led_lowpower mode set to %i\n", led_lowpower);

	return count;
}

static ssize_t store_an30259a_led_br_lev(struct device *dev,
					struct device_attribute *devattr,
					const char *buf, size_t count)
{
	int retval;
	unsigned long brightness_lev;
	struct i2c_client *client;
	struct an30259a_data *data = dev_get_drvdata(dev);
	client = b_client;

	retval = kstrtoul(buf, 16, &brightness_lev);
	if (retval != 0) {
		dev_err(&data->client->dev, "fail to get led_br_lev.\n");
		return count;
	}

	leds_set_imax(client, brightness_lev);

	return count;
}

static ssize_t store_an30259a_led_pattern(struct device *dev,
					struct device_attribute *devattr,
					const char *buf, size_t count)
{
	int retval;
	unsigned int mode = 0;
	unsigned int type = 0;
	struct an30259a_data *data = dev_get_drvdata(dev);

	retval = sscanf(buf, "%d %d", &mode, &type);

	if (retval == 0) {
		dev_err(&data->client->dev, "fail to get led_pattern mode.\n");
		return count;
	}

	an30259a_start_led_pattern(mode);
	printk(KERN_DEBUG "led pattern : %d is activated\n", mode);

	return count;
}

static ssize_t store_an30259a_led_blink(struct device *dev,
					struct device_attribute *devattr,
					const char *buf, size_t count)
{
	int retval;
	unsigned int led_brightness = 0;
	unsigned int delay_on_time = 0;
	unsigned int delay_off_time = 0;
	struct an30259a_data *data = dev_get_drvdata(dev);
	u8 led_r_brightness = 0;
	u8 led_g_brightness = 0;
	u8 led_b_brightness = 0;

	retval = sscanf(buf, "0x%x %d %d", &led_brightness,
				&delay_on_time, &delay_off_time);

	if (retval == 0) {
		dev_err(&data->client->dev, "fail to get led_blink value.\n");
		return count;
	}
	/*Reset an30259a*/
	an30259a_start_led_pattern(LED_OFF);

	/*Set LED blink mode*/
	led_r_brightness = ((u32)led_brightness & LED_R_MASK)
					>> LED_R_SHIFT;
	led_g_brightness = ((u32)led_brightness & LED_G_MASK)
					>> LED_G_SHIFT;
	led_b_brightness = ((u32)led_brightness & LED_B_MASK);

	an30259a_set_led_blink(LED_R, delay_on_time,
				delay_off_time, led_r_brightness, false);
	an30259a_set_led_blink(LED_G, delay_on_time,
				delay_off_time, led_g_brightness, false);
	an30259a_set_led_blink(LED_B, delay_on_time,
				delay_off_time, led_b_brightness, false);

	leds_i2c_write_all(data->client);

	printk(KERN_DEBUG "led_blink is called, Brightness:0x%X", led_brightness);

	return count;
}


static ssize_t store_led_r(struct device *dev,
	struct device_attribute *devattr, const char *buf, size_t count)
{
	struct an30259a_data *data = dev_get_drvdata(dev);
	char buff[10] = {0,};
	int cnt, ret;
	u8 brightness;

	cnt = count;
	cnt = (buf[cnt-1] == '\n') ? cnt-1 : cnt;
	memcpy(buff, buf, cnt);
	buff[cnt] = '\0';

	ret = kstrtou8(buff, 0, &brightness);
	if (ret != 0) {
		dev_err(&data->client->dev, "fail to get brightness.\n");
		goto out;
	}

	if (brightness == 0)
		leds_on(LED_R, false, false, 0);
	else
		leds_on(LED_R, true, false, brightness);

	leds_i2c_write_all(data->client);
	an30259a_debug(data->client);
out:
	return count;
}

static ssize_t store_led_g(struct device *dev,
	struct device_attribute *devattr, const char *buf, size_t count)
{
	struct an30259a_data *data = dev_get_drvdata(dev);
	char buff[10] = {0,};
	int cnt, ret;
	u8 brightness;

	cnt = count;
	cnt = (buf[cnt-1] == '\n') ? cnt-1 : cnt;
	memcpy(buff, buf, cnt);
	buff[cnt] = '\0';

	ret = kstrtou8(buff, 0, &brightness);
	if (ret != 0) {
		dev_err(&data->client->dev, "fail to get brightness.\n");
		goto out;
	}

	if (brightness == 0)
		leds_on(LED_G, false, false, 0);
	else
		leds_on(LED_G, true, false, brightness);

	leds_i2c_write_all(data->client);
	an30259a_debug(data->client);
out:
	return count;
}

static ssize_t store_led_b(struct device *dev,
	struct device_attribute *devattr, const char *buf, size_t count)
{
	struct an30259a_data *data = dev_get_drvdata(dev);
	char buff[10] = {0,};
	int cnt, ret;
	u8 brightness;

	cnt = count;
	cnt = (buf[cnt-1] == '\n') ? cnt-1 : cnt;
	memcpy(buff, buf, cnt);
	buff[cnt] = '\0';

	ret = kstrtou8(buff, 0, &brightness);
	if (ret != 0) {
		dev_err(&data->client->dev, "fail to get brightness.\n");
		goto out;
	}

	if (brightness == 0)
		leds_on(LED_B, false, false, 0);
	else
		leds_on(LED_B, true, false, brightness);

	leds_i2c_write_all(data->client);
	an30259a_debug(data->client);
out:
	return count;

}
#endif

/* Added for led common class */
static ssize_t led_delay_on_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct an30259a_led *led = cdev_to_led(led_cdev);

	return snprintf(buf, 10, "%lu\n", led->delay_on_time_ms);
}

static ssize_t led_delay_on_store(struct device *dev,
			struct device_attribute *attr,
			const char *buf, size_t len)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct an30259a_led *led = cdev_to_led(led_cdev);
	unsigned long time;

	if (kstrtoul(buf, 0, &time))
		return -EINVAL;

	led->delay_on_time_ms = (int)time;
	return len;
}

static ssize_t led_delay_off_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct an30259a_led *led = cdev_to_led(led_cdev);

	return snprintf(buf, 10, "%lu\n", led->delay_off_time_ms);
}

static ssize_t led_delay_off_store(struct device *dev,
			struct device_attribute *attr,
			const char *buf, size_t len)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct an30259a_led *led = cdev_to_led(led_cdev);
	unsigned long time;

	if (kstrtoul(buf, 0, &time))
		return -EINVAL;

	led->delay_off_time_ms = (int)time;

	return len;
}

static ssize_t led_blink_store(struct device *dev,
			struct device_attribute *attr,
			const char *buf, size_t len)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct an30259a_led *led = cdev_to_led(led_cdev);
	unsigned long blink_set;

	if (kstrtoul(buf, 0, &blink_set))
		return -EINVAL;

	if (!blink_set) {
		led->delay_on_time_ms = LED_OFF;
		an30259a_set_brightness(led_cdev, LED_OFF);
	}

	led_blink_set(led_cdev,
		&led->delay_on_time_ms, &led->delay_off_time_ms);

	return len;
}

/* permission for sysfs node */
static DEVICE_ATTR(delay_on, 0644, led_delay_on_show, led_delay_on_store);
static DEVICE_ATTR(delay_off, 0644, led_delay_off_show, led_delay_off_store);
static DEVICE_ATTR(blink, 0644, NULL, led_blink_store);

#ifdef SEC_LED_SPECIFIC
static ssize_t show_leds_property(struct device *dev,
			struct device_attribute *attr, char *buf);

static ssize_t store_leds_property(struct device *dev,
			struct device_attribute *attr,
			const char *buf, size_t len);

#define LEDS_ATTR(_name)				\
{							\
	.attr = {					\
		  .name = #_name,			\
		  .mode = S_IRUGO | S_IWUSR | S_IWGRP,	\
		},					\
	.show = show_leds_property,			\
	.store = store_leds_property,			\
}

static struct device_attribute leds_control_attrs[] = {
	LEDS_ATTR(led_lowpower_current),LEDS_ATTR(led_highpower_current),
	LEDS_ATTR(led_blink_delay),	LEDS_ATTR(led_fade),
	LEDS_ATTR(led_fade_in_time),	LEDS_ATTR(led_fade_out_time),
	LEDS_ATTR(led_fade_dt1),	LEDS_ATTR(led_fade_dt2),
	LEDS_ATTR(led_fade_dt3),	LEDS_ATTR(led_fade_dt4),
};

enum {
	LOWPOWER_CURRENT = 0, HIGHPOWER_CURRENT, BLINK_DELAY, BLINK_FADING,
	FADE_IN_TIME, FADE_OUT_TIME, FADE_DT1, FADE_DT2, FADE_DT3, FADE_DT4, 
};

static ssize_t show_leds_property(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	const ptrdiff_t offset = attr - leds_control_attrs;
	
	switch (offset) {
		case LOWPOWER_CURRENT:
			return sprintf(buf, "%d", leds_control.current_low);
		case HIGHPOWER_CURRENT:
			return sprintf(buf, "%d", leds_control.current_high);
		case BLINK_DELAY:
			return sprintf(buf, "%d", leds_control.blink_delay);
		case BLINK_FADING:
			return sprintf(buf, "%d", leds_control.blink_fading);
		case FADE_IN_TIME:
			return sprintf(buf, "%d", leds_control.fade_in_time);
		case FADE_OUT_TIME:
			return sprintf(buf, "%d", leds_control.fade_out_time);
		case FADE_DT1:
			return sprintf(buf, "%d", leds_control.fade_dt1);
		case FADE_DT2:
			return sprintf(buf, "%d", leds_control.fade_dt2);
		case FADE_DT3:
			return sprintf(buf, "%d", leds_control.fade_dt3);
		case FADE_DT4:
			return sprintf(buf, "%d", leds_control.fade_dt4);
	}
	
	return -EINVAL;
}

static ssize_t store_leds_property(struct device *dev,
			struct device_attribute *attr,
			const char *buf, size_t len)
{
	int val;
	const ptrdiff_t offset = attr - leds_control_attrs;

	if(sscanf(buf, "%d", &val) != 1)
		return -EINVAL;

	switch (offset) {
		case LOWPOWER_CURRENT:
			sanitize_min_max(val, 0, LED_MAX_CURRENT);
			leds_control.current_low = val;
			break;
		case HIGHPOWER_CURRENT:
			sanitize_min_max(val, 0, LED_MAX_CURRENT);
			leds_control.current_high = val;
			break;
		case BLINK_DELAY:
			sanitize_min_max(val, 1, 60);
			leds_control.blink_delay = val;
			break;
		case BLINK_FADING:
			leds_control.blink_fading = !!val;
			break;
		case FADE_IN_TIME:
			sanitize_min_max(val, 1, 20);
			leds_control.fade_in_time = val;
			break;
		case FADE_OUT_TIME:
			sanitize_min_max(val, 1, 20);
			leds_control.fade_out_time = val;
			break;
		case FADE_DT1:
			sanitize_min_max(val, 0, 10);
			leds_control.fade_dt1 = val;
			break;
		case FADE_DT2:
			sanitize_min_max(val, 0, 10);
			leds_control.fade_dt2 = val;
			break;
		case FADE_DT3:
			sanitize_min_max(val, 0, 10);
			leds_control.fade_dt3 = val;
			break;
		case FADE_DT4:
			sanitize_min_max(val, 0, 10);
			leds_control.fade_dt4 = val;
			break;
	}
	
	return len;
}


/* below nodes is SAMSUNG specific nodes */
static DEVICE_ATTR(led_r, 0664, NULL, store_led_r);
static DEVICE_ATTR(led_g, 0664, NULL, store_led_g);
static DEVICE_ATTR(led_b, 0664, NULL, store_led_b);
/* led_pattern node permission is 664 */
/* To access sysfs node from other groups */
static DEVICE_ATTR(led_pattern, 0664, NULL, \
					store_an30259a_led_pattern);
static DEVICE_ATTR(led_blink, 0664, NULL, \
					store_an30259a_led_blink);
static DEVICE_ATTR(led_br_lev, 0664, NULL, \
					store_an30259a_led_br_lev);
static DEVICE_ATTR(led_lowpower, 0664, NULL, \
					store_an30259a_led_lowpower);


#endif
static struct attribute *led_class_attrs[] = {
	&dev_attr_delay_on.attr,
	&dev_attr_delay_off.attr,
	&dev_attr_blink.attr,
	NULL,
};

static struct attribute_group common_led_attr_group = {
	.attrs = led_class_attrs,
};

#ifdef SEC_LED_SPECIFIC
static struct attribute *sec_led_attributes[] = {
	&dev_attr_led_r.attr,
	&dev_attr_led_g.attr,
	&dev_attr_led_b.attr,
	&dev_attr_led_pattern.attr,
	&dev_attr_led_blink.attr,
	&dev_attr_led_br_lev.attr,
	&dev_attr_led_lowpower.attr,
	NULL,
};

static struct attribute_group sec_led_attr_group = {
	.attrs = sec_led_attributes,
};
#endif

static int __devinit an30259a_initialize(struct i2c_client *client,
					struct an30259a_led *led, int channel)
{
	struct an30259a_data *data = i2c_get_clientdata(client);
	struct device *dev = &client->dev;
	int ret;

	/* reset an30259a*/
	ret = i2c_smbus_write_byte_data(client, AN30259A_REG_SRESET,
					AN30259A_SRESET);
	if (ret < 0) {
		dev_err(&client->adapter->dev,
			"%s: failure on i2c write (reg = 0x%2x)\n",
			__func__, AN30259A_REG_SRESET);
		return ret;
	}
	ret = i2c_smbus_read_i2c_block_data(client,
			AN30259A_REG_SRESET | AN30259A_CTN_RW_FLG,
			AN30259A_REG_MAX, data->shadow_reg);
	if (ret < 0) {
		dev_err(&client->adapter->dev,
			"%s: failure on i2c read block(ledxcc)\n",
			__func__);
		return ret;
	}
	led->channel = channel;
	led->cdev.brightness_set = an30259a_set_brightness;
	led->cdev.name = led_conf[channel].name;
	led->cdev.brightness = led_conf[channel].brightness;
	led->cdev.max_brightness = led_conf[channel].max_brightness;
	led->cdev.flags = led_conf[channel].flags;

	ret = led_classdev_register(dev, &led->cdev);

	if (ret < 0) {
		dev_err(dev, "can not register led channel : %d\n", channel);
		return ret;
	}

	ret = sysfs_create_group(&led->cdev.dev->kobj,
			&common_led_attr_group);

	if (ret < 0) {
		dev_err(dev, "can not register sysfs attribute for led channel : %d\n", channel);
		led_classdev_unregister(&led->cdev);
		return ret;
	}

	leds_set_imax(client, 0x00);

	return 0;
}

//if one led will fail to register than all led registration will fail
static void an30259a_deinitialize(struct an30259a_led *led, int channel)
{
	sysfs_remove_group(&led->cdev.dev->kobj,&common_led_attr_group);
	led_classdev_unregister(&led->cdev);
	cancel_work_sync(&led->brightness_work);
}

static int __devinit an30259a_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	struct an30259a_data *data;
	int ret, i;

	dev_dbg(&client->adapter->dev, "%s\n", __func__);
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		dev_err(&client->dev, "need I2C_FUNC_I2C.\n");
		return -ENODEV;
	}

	data = kzalloc(sizeof(*data), GFP_KERNEL);
	if (!data) {
		dev_err(&client->adapter->dev,
			"failed to allocate driver data.\n");
		return -ENOMEM;
	}

	i2c_set_clientdata(client, data);
	data->client = client;
	b_client = client;

	mutex_init(&data->mutex);
	/* initialize LED */
	for (i = 0; i < MAX_NUM_LEDS; i++) {

		ret = an30259a_initialize(client, &data->leds[i], i);

		if (ret < 0) {
			dev_err(&client->adapter->dev, "failure on initialization at led channel:%d\n", i);
			while(i>0) { 
					i--;
					an30259a_deinitialize(&data->leds[i], i);
			}
			goto exit;
		}
		INIT_WORK(&(data->leds[i].brightness_work),
				 an30259a_led_brightness_work);
	}

#ifdef SEC_LED_SPECIFIC
	led_dev = device_create(sec_class, NULL, 0, data, "led");
	if (IS_ERR(led_dev)) {
		dev_err(&client->dev,
			"Failed to create device for samsung specific led\n");
		ret = -ENODEV;
		goto exit;
	}
	ret = sysfs_create_group(&led_dev->kobj, &sec_led_attr_group);
	if (ret) {
		dev_err(&client->dev,
			"Failed to create sysfs group for samsung specific led\n");
		goto exit;
	}

	for(i = 0; i < ARRAY_SIZE(leds_control_attrs); i++) {
		ret = sysfs_create_file(&led_dev->kobj, &leds_control_attrs[i].attr);
	}
#endif
	return ret;
exit:
	mutex_destroy(&data->mutex);
	kfree(data);
	return ret;
}

static int __devexit an30259a_remove(struct i2c_client *client)
{
	struct an30259a_data *data = i2c_get_clientdata(client);
	int i;
	dev_dbg(&client->adapter->dev, "%s\n", __func__);
#ifdef SEC_LED_SPECIFIC
	sysfs_remove_group(&led_dev->kobj, &sec_led_attr_group);
#endif
	for (i = 0; i < MAX_NUM_LEDS; i++) {
		sysfs_remove_group(&data->leds[i].cdev.dev->kobj,
						&common_led_attr_group);
		led_classdev_unregister(&data->leds[i].cdev);
		cancel_work_sync(&data->leds[i].brightness_work);
	}
	mutex_destroy(&data->mutex);
	kfree(data);
	return 0;
}

static struct i2c_device_id an30259a_id[] = {
	{"an30259a", 0},
	{},
};

MODULE_DEVICE_TABLE(i2c, an30259a_id);

static struct i2c_driver an30259a_i2c_driver = {
	.driver = {
		.owner	= THIS_MODULE,
		.name	= "an30259a",
	},
	.id_table = an30259a_id,
	.probe = an30259a_probe,
	.remove = __devexit_p(an30259a_remove),
};

static int __init an30259a_init(void)
{
	return i2c_add_driver(&an30259a_i2c_driver);
}

static void __exit an30259a_exit(void)
{
	i2c_del_driver(&an30259a_i2c_driver);
}

module_init(an30259a_init);
module_exit(an30259a_exit);

MODULE_DESCRIPTION("AN30259A LED driver");
MODULE_AUTHOR("Kamaldeep Singla <kamal.singla@samsung.com");
MODULE_LICENSE("GPL v2");

