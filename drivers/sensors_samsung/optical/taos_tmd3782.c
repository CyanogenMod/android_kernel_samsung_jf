/*
 * Copyright (c) 2010 SAMSUNG
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/leds.h>
#include <linux/gpio.h>
#include <linux/wakelock.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include <linux/uaccess.h>
#include <linux/i2c/taos_tmd3782.h>
#include <linux/sensors_core.h>


/* Note about power vs enable/disable:
 *  The chip has two functions, proximity and ambient light sensing.
 *  There is no separate power enablement to the two functions (unlike
 *  the Capella CM3602/3623).
 *  This module implements two drivers: /dev/proximity and /dev/light.
 *  When either driver is enabled (via sysfs attributes), we give power
 *  to the chip.  When both are disabled, we remove power from the chip.
 *  In suspend, we remove power if light is disabled but not if proximity is
 *  enabled (proximity is allowed to wakeup from suspend).
 *
 *  There are no ioctls for either driver interfaces.  Output is via
 *  input device framework and control via sysfs attributes.
 */

/* taos debug */
/*#define DEBUG 1*/
/*#define TAOS_DEBUG*/

#define taos_dbgmsg(str, args...) pr_info("%s: " str, __func__, ##args)
#define TAOS_DEBUG
#ifdef TAOS_DEBUG
#define gprintk(fmt, x...) \
	printk(KERN_INFO "%s(%d):" fmt, __func__, __LINE__, ## x)
#else
#define gprintk(x...) do { } while (0)
#endif

#define VENDOR_NAME	"TAOS"
#define CHIP_NAME       "TMD3782"
#define CHIP_ID			0x69

#define OFFSET_FILE_PATH	"/efs/prox_cal"

/* TMD3782 register offsets */
#define CNTRL				0x00
#define ALS_TIME			0X01
#define PRX_TIME			0x02
#define WAIT_TIME			0x03
#define ALS_MINTHRESHLO			0X04
#define ALS_MINTHRESHHI			0X05
#define ALS_MAXTHRESHLO			0X06
#define ALS_MAXTHRESHHI			0X07
#define PRX_MINTHRESHLO			0X08
#define PRX_MINTHRESHHI			0X09
#define PRX_MAXTHRESHLO			0X0A
#define PRX_MAXTHRESHHI			0X0B
#define INTERRUPT			0x0C
#define PRX_CFG				0x0D
#define PRX_COUNT			0x0E
#define GAIN				0x0F
#define REVID				0x11
#define CHIPID				0x12
#define STATUS				0x13
#define CLR_CHAN0LO			0x14
#define CLR_CHAN0HI			0x15
#define RED_CHAN1LO			0x16
#define RED_CHAN1HI			0x17
#define GRN_CHAN1LO			0x18
#define GRN_CHAN1HI			0x19
#define BLU_CHAN1LO			0x1A
#define BLU_CHAN1HI			0x1B
#define PRX_LO				0x1C
#define PRX_HI				0x1D
#define PRX_OFFSET			0x1E
#define TEST_STATUS			0x1F

/*TMD3782 cmd reg masks*/
#define CMD_REG					0X80
#define CMD_BYTE_RW				0x00
#define CMD_WORD_BLK_RW			0x20
#define CMD_SPL_FN				0x60
#define CMD_PROX_INTCLR			0X05
#define CMD_ALS_INTCLR			0X06
#define CMD_PROXALS_INTCLR		0X07
#define CMD_TST_REG				0X08
#define CMD_USER_REG			0X09

/* TMD3782 cntrl reg masks */
#define CNTL_REG_CLEAR			0x00
#define CNTL_PROX_INT_ENBL		0X20
#define CNTL_ALS_INT_ENBL		0X10
#define CNTL_WAIT_TMR_ENBL		0X08
#define CNTL_PROX_DET_ENBL		0X04
#define CNTL_ADC_ENBL			0x02
#define CNTL_PWRON				0x01
#define CNTL_ALSPON_ENBL		0x03
#define CNTL_INTALSPON_ENBL		0x13
#define CNTL_PROXPON_ENBL		0x0F
#define CNTL_INTPROXPON_ENBL		0x2F

/* TMD3782 status reg masks */
#define STA_ADCVALID			0x01
#define STA_PRXVALID			0x02
#define STA_ADC_PRX_VALID		0x03
#define STA_ADCINTR				0x10
#define STA_PRXINTR				0x20

#define	MAX_ALS				0xffff
#define	MIN_ALS				10
/* power control */
#define ON				1
#define OFF				0

/* sensor type */
#define LIGHT			0
#define PROXIMITY		1
#define ALL				2

//#define ALS_GAIN			4
#define Atime_ms			504 // 50.4 ms
#define DGF				475
#define R_Coef1			535
#define G_Coef1			1000
#define B_Coef1			795
#define CT_Coef1			(2855)
#define CT_Offset1			(1973)
#define INTEGRATION_CYCLE	240

#define ADC_BUFFER_NUM	6

#define PROX_AVG_COUNT	40

#define MAX_LUX		39768

enum {
	LIGHT_ENABLED = BIT(0),
	PROXIMITY_ENABLED = BIT(1),
};

#define TAOS_PROX_MAX			1023
#define TAOS_PROX_MIN			0
#define OFFSET_ARRAY_LENGTH		10
#define WINDOW_TYPE_FILE_PATH "/sys/class/sec/sec_touch_ic/window_type"
/* driver data */
struct taos_data {
	struct input_dev *proximity_input_dev;
	struct input_dev *light_input_dev;
	struct taos_platform_data *pdata;
	struct i2c_client *i2c_client;
	struct device *light_dev;
	struct device *proximity_dev;
	int irq;
	struct work_struct work_light;
	struct work_struct work_prox;
	struct work_struct work_prox_avg;
	struct mutex prox_mutex;
/*	TAOS_PRX_DISTANCE_STATUS taos_prox_dist;*/
	struct hrtimer timer;
	struct hrtimer prox_avg_timer;
	ktime_t light_poll_delay;
	ktime_t prox_polling_time;
	int adc_value_buf[ADC_BUFFER_NUM];
	int adc_index_count;
	bool adc_buf_initialized;
	u8 power_state;
	struct mutex power_lock;
	struct wake_lock prx_wake_lock;
	struct workqueue_struct *wq;
	struct workqueue_struct *wq_avg;
	int avg[3];
	int prox_avg_enable;
	s32 clrdata;
	s32 reddata;
	s32 grndata;
	s32 bludata;
	s32 irdata;
/* Auto Calibration */
	u16 offset_value;
	int cal_result;
	int threshold_high;
	int threshold_low;
	int proximity_value;
	bool set_manual_thd;
	char windowtype[2];
};

static int lightsensor_get_adcvalue(struct taos_data *taos);
static int proximity_open_offset(struct taos_data *data);
static int proximity_adc_read(struct taos_data *taos);
static int proximity_open_window_type(struct taos_data *data);

static int opt_i2c_write(struct taos_data *taos, u8 reg, u8 *val)
{
	int ret;

	ret = i2c_smbus_write_byte_data(taos->i2c_client,
		(CMD_REG | reg), *val);

	return ret;
}

static int opt_i2c_read(struct taos_data *taos, u8 reg , u8 *val)
{
	int ret;

	i2c_smbus_write_byte(taos->i2c_client, (CMD_REG | reg));
	ret = i2c_smbus_read_byte(taos->i2c_client);
	*val = ret;

	return ret;
}

static int opt_i2c_write_command(struct taos_data *taos, u8 val)
{
	int ret;

	ret = i2c_smbus_write_byte(taos->i2c_client, val);
	gprintk("[TAOS Command] val=[0x%x] - ret=[0x%x]\n", val, ret);

	return ret;
}

static void taos_thresh_set(struct taos_data *taos)
{
	int i = 0;
	int ret = 0;
	u8 prox_int_thresh[4];

	/* Setting for proximity interrupt */
	if (taos->proximity_value == 1) {
		prox_int_thresh[0] = (taos->threshold_low) & 0xFF;
		prox_int_thresh[1] = (taos->threshold_low >> 8) & 0xFF;
		prox_int_thresh[2] = (0xFFFF) & 0xFF;
		prox_int_thresh[3] = (0xFFFF >> 8) & 0xFF;
	} else if (taos->proximity_value == 0) {
		prox_int_thresh[0] = (0x0000) & 0xFF;
		prox_int_thresh[1] = (0x0000 >> 8) & 0xFF;
		prox_int_thresh[2] = (taos->threshold_high) & 0xff;
		prox_int_thresh[3] = (taos->threshold_high >> 8) & 0xff;
	}

	for (i = 0; i < 4; i++) {
		ret = opt_i2c_write(taos,
			(CMD_REG|(PRX_MINTHRESHLO + i)),
			&prox_int_thresh[i]);
		if (ret < 0)
			gprintk("opt_i2c_write failed, err = %d\n", ret);
	}
}

static int taos_chip_on(struct taos_data *taos)
{
	int i = 0;
	int ret = 0;
	int num_try_init = 0;
	int fail_num = 0;
	u8 temp_val;
	u8 reg_cntrl;
	u8 prox_int_thresh[4];


	for (num_try_init = 0; num_try_init < 3 ; num_try_init++) {
		fail_num = 0;

		temp_val = CNTL_PWRON;
		ret = opt_i2c_write(taos, (CMD_REG|CNTRL), &temp_val);
		if (ret < 0) {
			gprintk("opt_i2c_write to clr ctrl reg failed\n");
			fail_num++;
		}

		temp_val = taos->pdata->als_time;
		ret = opt_i2c_write(taos, (CMD_REG|ALS_TIME), &temp_val);
		if (ret < 0) {
			gprintk("opt_i2c_write to als time reg failed\n");
			fail_num++;
		}

		temp_val = 0xff;
		ret = opt_i2c_write(taos, (CMD_REG|WAIT_TIME), &temp_val);
		if (ret < 0) {
			gprintk("opt_i2c_write to wait time reg failed\n");
			fail_num++;
		}

		temp_val = taos->pdata->intr_filter;
		ret = opt_i2c_write(taos, (CMD_REG|INTERRUPT), &temp_val);
		if (ret < 0) {
			gprintk("opt_i2c_write to interrupt reg failed\n");
			fail_num++;
		}

		temp_val = 0x0;
		ret = opt_i2c_write(taos, (CMD_REG|PRX_CFG), &temp_val);
		if (ret < 0) {
			gprintk("opt_i2c_write to prox cfg reg failed\n");
			fail_num++;
		}

		temp_val = taos->pdata->prox_pulsecnt;
		ret = opt_i2c_write(taos, (CMD_REG|PRX_COUNT), &temp_val);
		if (ret < 0) {
			gprintk("opt_i2c_write to prox cnt reg failed\n");
			fail_num++;
		}
		temp_val = taos->pdata->als_gain;
		ret = opt_i2c_write(taos, (CMD_REG|GAIN), &temp_val);
		if (ret < 0) {
			gprintk("opt_i2c_write to prox gain reg failed\n");
			fail_num++;
		}

		/* Setting for proximity interrupt */
		prox_int_thresh[0] = 0;
		prox_int_thresh[1] = 0;
		prox_int_thresh[2] = (taos->threshold_high) & 0xff;
		prox_int_thresh[3] = (taos->threshold_high >> 8) & 0xff;

		for (i = 0; i < 4; i++) {
			ret = opt_i2c_write(taos,
				(CMD_REG|(PRX_MINTHRESHLO + i)),
				&prox_int_thresh[i]);
			if (ret < 0) {
				gprintk("opt_i2c_write failed, err = %d\n",
					ret);
				fail_num++;
			}
		}

		reg_cntrl = CNTL_INTPROXPON_ENBL;
		ret = opt_i2c_write(taos, (CMD_REG|CNTRL), &reg_cntrl);
		if (ret < 0) {
			gprintk("opt_i2c_write to ctrl reg failed\n");
			fail_num++;
		}
		msleep(60); /*more than 58 ms*/

		if (fail_num == 0)
			break;
	}
	return ret;
}

static int taos_chip_off(struct taos_data *taos)
{
	int ret = 0;
	u8 reg_cntrl;


	reg_cntrl = CNTL_REG_CLEAR;
	ret = opt_i2c_write(taos, (CMD_REG | CNTRL), &reg_cntrl);
	if (ret < 0) {
		gprintk("opt_i2c_write to ctrl reg failed\n");
		return ret;
	}

	return ret;
}

static int taos_get_cct(struct taos_data *taos)
{
	int bp1 = taos->bludata - taos->irdata;
	int rp1 = taos->reddata - taos->irdata;
	int cct = 0;

	if(rp1 != 0)
		cct = CT_Coef1 * bp1 / rp1 + CT_Offset1;

	return cct;
}
static int taos_get_lux(struct taos_data *taos)
{
	s32 rp1, gp1, bp1;
/*	s32 cp1;	*/
	s32 clrdata = 0;
	s32 reddata = 0;
	s32 grndata = 0;
	s32 bludata = 0;
	s32 calculated_lux = 0;
	u8 reg_gain = 0x0;
	u16 temp_gain = 0x0;
	int gain = 1;
	int ret = 0;

	temp_gain = i2c_smbus_read_word_data(taos->i2c_client,
		(CMD_REG | GAIN));

	reg_gain = temp_gain & 0xff;
	clrdata = i2c_smbus_read_word_data(taos->i2c_client,
		(CMD_REG | CLR_CHAN0LO));
	reddata = i2c_smbus_read_word_data(taos->i2c_client,
		(CMD_REG | RED_CHAN1LO));
	grndata = i2c_smbus_read_word_data(taos->i2c_client,
		(CMD_REG | GRN_CHAN1LO));
	bludata = i2c_smbus_read_word_data(taos->i2c_client,
		(CMD_REG | BLU_CHAN1LO));

	taos->clrdata = clrdata;
	taos->reddata = reddata;
	taos->grndata = grndata;
	taos->bludata = bludata;

	switch(reg_gain & 0x03)
	{
/*		case 0x00:
			gain = 1;
			break;*/
		case 0x01:
			gain = 4;
			break;
		case 0x02:
			gain = 16;
			break;
/*		case 0x03:
			gain = 64;
			break;*/
		default:
			break;
	}

	if (clrdata >= 18500)
	{
		calculated_lux = MAX_LUX;
		return calculated_lux;
	}

	if(gain ==4 && clrdata < 100)
	{
		reg_gain = 0x22;
		ret = opt_i2c_write(taos, (CMD_REG | GAIN), &reg_gain);
	}
	else if(gain ==16 && clrdata > 15000)
	{
		reg_gain = 0x21;
		ret = opt_i2c_write(taos, (CMD_REG | GAIN), &reg_gain);
	}

	if (ret) {
		printk(KERN_ERR"%s i2c write error\n", __func__);
		return 0;
	}

	/* calculate lux */
	taos->irdata = (reddata + grndata + bludata - clrdata)/2;

	/* remove ir from counts*/
	rp1 = taos->reddata - taos->irdata;
	gp1 = taos->grndata - taos->irdata;
	bp1 = taos->bludata - taos->irdata;
/*	cp1 = taos->clrdata - taos->irdata; */

	calculated_lux = (rp1 * R_Coef1 + gp1 * G_Coef1 + bp1 * B_Coef1) /1000;

	if(calculated_lux < 0)
		calculated_lux = 0;
	else {
		// divide by CPL, CPL = (Atime_ms * ALS_GAIN / DGF);
		calculated_lux =calculated_lux*DGF;
		calculated_lux *= 10; // Atime_ms
		calculated_lux /= Atime_ms;
		calculated_lux /= gain;
	}

//	printk("lux : %d, gain : %x, r : %d, g : %d, b : %d, c : %d\n", calculated_lux, reg_gain, rp1, gp1, bp1, cp1 );
	return (int)calculated_lux;
}

static void taos_light_enable(struct taos_data *taos)
{
	taos_dbgmsg("starting poll timer, delay %lldns\n",
	ktime_to_ns(taos->light_poll_delay));
	taos_get_lux(taos);
	hrtimer_start(&taos->timer, taos->light_poll_delay, HRTIMER_MODE_REL);
}

static void taos_light_disable(struct taos_data *taos)
{
	taos_dbgmsg("cancelling poll timer\n");
	hrtimer_cancel(&taos->timer);
	cancel_work_sync(&taos->work_light);
}

static ssize_t poll_delay_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct taos_data *taos = dev_get_drvdata(dev);
	return sprintf(buf, "%lld\n", ktime_to_ns(taos->light_poll_delay));
}


static ssize_t poll_delay_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t size)
{
	struct taos_data *taos = dev_get_drvdata(dev);
	int64_t new_delay;
	int err;

	err = strict_strtoll(buf, 10, &new_delay);
	if (err < 0)
		return err;

	taos_dbgmsg("new delay = %lldns, old delay = %lldns\n",
		    new_delay, ktime_to_ns(taos->light_poll_delay));
	mutex_lock(&taos->power_lock);
	if (new_delay != ktime_to_ns(taos->light_poll_delay)) {
		taos->light_poll_delay = ns_to_ktime(new_delay);
		if (taos->power_state & LIGHT_ENABLED) {
			taos_light_disable(taos);
			taos_light_enable(taos);
		}
	}
	mutex_unlock(&taos->power_lock);

	return size;
}

static ssize_t light_enable_show(struct device *dev,
				 struct device_attribute *attr, char *buf)
{
	struct taos_data *taos = dev_get_drvdata(dev);
	return sprintf(buf, "%d\n",
		       (taos->power_state & LIGHT_ENABLED) ? 1 : 0);
}

static ssize_t proximity_enable_show(struct device *dev,
				 struct device_attribute *attr, char *buf)
{
	struct taos_data *taos = dev_get_drvdata(dev);
	return sprintf(buf, "%d\n",
		       (taos->power_state & PROXIMITY_ENABLED) ? 1 : 0);
}

static ssize_t light_enable_store(struct device *dev,
	struct device_attribute *attr,
	const char *buf, size_t size)
{
	struct taos_data *taos = dev_get_drvdata(dev);
	bool new_value;


	if (sysfs_streq(buf, "1")) {
		new_value = true;
	} else if (sysfs_streq(buf, "0")) {
		new_value = false;
	} else {
		pr_err("%s: invalid value %d\n", __func__, *buf);
		return -EINVAL;
	}

	mutex_lock(&taos->power_lock);
	taos_dbgmsg("new_value = %d, old state = %d\n",
		    new_value, (taos->power_state & LIGHT_ENABLED) ? 1 : 0);
	if (new_value && !(taos->power_state & LIGHT_ENABLED)) {
		if (!taos->power_state) {
			taos->pdata->power(true);
			taos_chip_on(taos);
		}
		taos->power_state |= LIGHT_ENABLED;
		taos_light_enable(taos);
	} else if (!new_value && (taos->power_state & LIGHT_ENABLED)) {
		taos_light_disable(taos);
		taos->power_state &= ~LIGHT_ENABLED;
		if (!taos->power_state) {
			taos_chip_off(taos);
			taos->pdata->power(false);
		}
	}
	mutex_unlock(&taos->power_lock);
	return size;
}

static ssize_t proximity_enable_store(struct device *dev,
				      struct device_attribute *attr,
				      const char *buf, size_t size)
{
	struct taos_data *taos = dev_get_drvdata(dev);
	bool new_value;
	int temp = 0, ret = 0;


	if (sysfs_streq(buf, "1")) {
		new_value = true;
	} else if (sysfs_streq(buf, "0")) {
		new_value = false;
	} else {
		pr_err("%s: invalid value %d\n", __func__, *buf);
		return -EINVAL;
	}

	mutex_lock(&taos->power_lock);
	taos_dbgmsg("new_value = %d, old state = %d\n",
		    new_value, (taos->power_state & PROXIMITY_ENABLED) ? 1 : 0);
	if (new_value && !(taos->power_state & PROXIMITY_ENABLED)) {
		if (!taos->power_state)
			taos->pdata->power(true);
		usleep_range(5000, 6000);
		if(taos->set_manual_thd == false) {
			ret = proximity_open_offset(taos);
			if (ret < 0 && ret != -ENOENT)
				pr_err("%s: proximity_open_offset() failed\n",
				__func__);
			ret = proximity_open_window_type(taos);
			/* set prox_threshold from board file */
			taos->threshold_high= taos->pdata->prox_thresh_hi + taos->offset_value;
			taos->threshold_low = taos->pdata->prox_thresh_low + taos->offset_value;
			pr_err("%s: th_hi = %d, th_low = %d\n", __func__,
				taos->threshold_high, taos->threshold_low);
		}
		taos->power_state |= PROXIMITY_ENABLED;

		/* interrupt clearing */
		temp = (CMD_REG|CMD_SPL_FN|CMD_PROXALS_INTCLR);
		ret = opt_i2c_write_command(taos, temp);
		if (ret < 0)
			gprintk("opt_i2c_write failed, err = %d\n", ret);
		taos_chip_on(taos);
		taos->pdata->led_on(true);
		input_report_abs(taos->proximity_input_dev, ABS_DISTANCE, 1);
		input_sync(taos->proximity_input_dev);

		enable_irq(taos->irq);
		enable_irq_wake(taos->irq);

	} else if (!new_value && (taos->power_state & PROXIMITY_ENABLED)) {
		disable_irq_wake(taos->irq);
		disable_irq(taos->irq);

		taos->pdata->led_on(false);

		taos->power_state &= ~PROXIMITY_ENABLED;
		if (!taos->power_state) {
			taos_chip_off(taos);
			taos->pdata->power(false); /*taos_chip_off(taos);*/
		}
	}
	mutex_unlock(&taos->power_lock);
	return size;
}

static ssize_t proximity_state_show(struct device *dev,
		struct device_attribute *attr,
		char *buf)
{
	struct taos_data *taos = dev_get_drvdata(dev);
	int adc = 0;

	adc = i2c_smbus_read_word_data(taos->i2c_client,
			CMD_REG | PRX_LO);
	if (adc > TAOS_PROX_MAX)
		adc = TAOS_PROX_MAX;

	return sprintf(buf, "%d\n", adc);

}
static void change_proximity_default_threshold(struct taos_data *data)
{
	switch (data->windowtype[1]) {
	case WINTYPE_WHITE:
		data->pdata->prox_thresh_hi = WHITEWINDOW_HI_THRESHOLD;
		data->pdata->prox_thresh_low = WHITEWINDOW_LOW_THRESHOLD;
		break;
	case WINTYPE_OTHERS:
		data->pdata->prox_thresh_hi = BLACKWINDOW_HI_THRESHOLD;
		data->pdata->prox_thresh_low = BLACKWINDOW_LOW_THRESHOLD;
		break;
	default:
		data->pdata->prox_thresh_hi = BLACKWINDOW_HI_THRESHOLD;
		data->pdata->prox_thresh_low = BLACKWINDOW_LOW_THRESHOLD;
		break;
	}
}
static int proximity_open_window_type(struct taos_data *data)
{
	struct file *wintype_filp = NULL;
	int err = 0;
	mm_segment_t old_fs;

	old_fs = get_fs();
	set_fs(KERNEL_DS);

	wintype_filp = filp_open(WINDOW_TYPE_FILE_PATH, O_RDONLY, 0666);
	if(IS_ERR(wintype_filp)) {
		pr_err("%s: no window_type file\n", __func__);
		err = PTR_ERR(wintype_filp);
		if(err != -ENOENT)
			pr_err("%s: Can't open window_type file\n", __func__);
		set_fs(old_fs);

		data->windowtype[0] = 0;
		data->windowtype[1] = 0;
		goto exit;
	}

	err = wintype_filp->f_op->read(wintype_filp,
		(u8 *)&data->windowtype, sizeof(u8) * 2, &wintype_filp->f_pos);
	if (err != sizeof(u8) * 2) {
		pr_err("%s: Can't read the window_type data from file\n", __func__);
		err = -EIO;
	}

	pr_err("%s: %c%c\n",
		__func__, data->windowtype[0], data->windowtype[1]);
	filp_close(wintype_filp, current->files);
	set_fs(old_fs);
exit:
	change_proximity_default_threshold(data);
	return err;
}
static int proximity_open_offset(struct taos_data *data)
{
	struct file *offset_filp = NULL;
	int err = 0;
	mm_segment_t old_fs;

	old_fs = get_fs();
	set_fs(KERNEL_DS);

	offset_filp = filp_open(OFFSET_FILE_PATH, O_RDONLY, 0666);
	if (IS_ERR(offset_filp)) {
		pr_err("%s: no offset file\n", __func__);
		err = PTR_ERR(offset_filp);
		if (err != -ENOENT)
			pr_err("%s: Can't open offset file\n", __func__);
		set_fs(old_fs);
		return err;
	}

	err = offset_filp->f_op->read(offset_filp,
		(char *)&data->offset_value, sizeof(u16), &offset_filp->f_pos);
	if (err != sizeof(u16)) {
		pr_err("%s: Can't read the offset data from file\n", __func__);
		err = -EIO;
	}

	pr_err("%s: data->offset_value = %d\n",
		__func__, data->offset_value);
	filp_close(offset_filp, current->files);
	set_fs(old_fs);

	return err;
}

static int proximity_adc_read(struct taos_data *taos)
{
	int sum[OFFSET_ARRAY_LENGTH];
	int i = 0;
	int avg = 0;
	int min = 0;
	int max = 0;
	int total = 0;

	mutex_lock(&taos->prox_mutex);
	for (i = 0; i < OFFSET_ARRAY_LENGTH; i++) {
		usleep_range(11000, 11000);
		sum[i] = i2c_smbus_read_word_data(taos->i2c_client,
			CMD_REG | PRX_LO);
		if (i == 0) {
			min = sum[i];
			max = sum[i];
		} else {
			if (sum[i] < min)
				min = sum[i];
			else if (sum[i] > max)
				max = sum[i];
		}
		total += sum[i];
	}
	mutex_unlock(&taos->prox_mutex);
	total -= (min + max);
	avg = (int)(total / (OFFSET_ARRAY_LENGTH - 2));

	return avg;
}

static int proximity_store_offset(struct device *dev, bool do_calib)
{
	struct taos_data *taos = dev_get_drvdata(dev);
	struct file *offset_filp = NULL;
	mm_segment_t old_fs;
	int err = 0;
	u16 max_ct = taos->pdata->prox_thresh_low /2;
	u16 abnormal_ct = proximity_adc_read(taos);
	u16 offset = 0;

	if(do_calib) {
		pr_err("%s: calibration start ", __func__);
		if(abnormal_ct >500) {
			pr_err("%s: crosstalk is lager than 500, this chip is bad smaple ", __func__);
		}

		if(abnormal_ct > max_ct) {
			offset = abnormal_ct - max_ct;

			if((taos->pdata->prox_thresh_hi + offset) > 900)
				pr_err("%s: detect threshold is lager than 900, this chip is bad smaple ", __func__);
		}

		taos->offset_value = offset;
		taos->threshold_high= taos->pdata->prox_thresh_hi + offset;
		taos->threshold_low = taos->pdata->prox_thresh_low+ offset;

		taos_thresh_set(taos);
		taos->set_manual_thd = false;
		taos->cal_result = 1;
	} else {
	/* tap reset button */
		pr_err("%s: reset\n", __func__);
		taos->threshold_high= taos->pdata->prox_thresh_hi;
		taos->threshold_low = taos->pdata->prox_thresh_low;
		taos_thresh_set(taos);
		taos->offset_value = 0;
		taos->cal_result = 2;
		taos->set_manual_thd = false;
	}
	printk("max_ct : %d, abnormal_ct : %d, offset : %d\n", max_ct, abnormal_ct, taos->offset_value);
	// store offset in file
	old_fs = get_fs();
	set_fs(KERNEL_DS);

	offset_filp = filp_open(OFFSET_FILE_PATH,
			O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (IS_ERR(offset_filp)) {
		pr_err("%s: Can't open prox_offset file\n", __func__);
		set_fs(old_fs);
		err = PTR_ERR(offset_filp);
		return err;
	}

	err = offset_filp->f_op->write(offset_filp,
		(char *)&taos->offset_value, sizeof(u16), &offset_filp->f_pos);
	if (err != sizeof(u16)) {
		pr_err("%s: Can't write the offset data to file\n", __func__);
/*		err = -EIO;	*/
	}

	filp_close(offset_filp, current->files);
	set_fs(old_fs);

	return 1;
}

static ssize_t proximity_cal_store(struct device *dev,
					  struct device_attribute *attr,
					  const char *buf, size_t size)
{
	bool do_calib;
	int err;

	if (sysfs_streq(buf, "1")) { /* calibrate cancelation value */
		do_calib = true;
	} else if (sysfs_streq(buf, "0")) { /* reset cancelation value */
		do_calib = false;
	} else {
		pr_err("%s: invalid value %d\n", __func__, *buf);
		return -EINVAL;
	}

	err = proximity_store_offset(dev, do_calib);
	if (err < 0) {
		pr_err("%s: proximity_store_offset() failed\n", __func__);
		return err;
	}

	return size;
}

static ssize_t proximity_cal_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct taos_data *taos = dev_get_drvdata(dev);
	int ret = 0;

	ret = proximity_open_offset(taos);
	if (ret < 0 && ret != -ENOENT) {
		pr_err("%s: proximity_open_offset() failed\n",
		__func__);
		return ret;
	}

	return sprintf(buf, "%d,%d,%d\n",
		taos->offset_value, taos->threshold_high, taos->threshold_low);
}

static ssize_t prox_offset_pass_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct taos_data *taos = dev_get_drvdata(dev);

	return sprintf(buf, "%d\n", taos->cal_result);
}

static ssize_t proximity_avg_show(struct device *dev,
				  struct device_attribute *attr, char *buf)
{

	struct taos_data *taos = dev_get_drvdata(dev);
	return sprintf(buf, "%d,%d,%d\n", taos->avg[0], taos->avg[1],
				taos->avg[2]);
}

static ssize_t proximity_avg_store(struct device *dev,
				   struct device_attribute *attr,
				   const char *buf, size_t size)
{
	struct taos_data *taos = dev_get_drvdata(dev);
	int new_value = 0;

	if (sysfs_streq(buf, "1")) {
		new_value = true;
	} else if (sysfs_streq(buf, "0")) {
		new_value = false;
	} else {
		pr_err("%s: invalid value %d\n", __func__, *buf);
		return -EINVAL;
	}
	if (taos->prox_avg_enable == new_value)
		taos_dbgmsg("%s same status\n", __func__);
	else if (new_value == 1) {
		taos_dbgmsg("starting poll timer, delay %lldns\n",
		ktime_to_ns(taos->prox_polling_time));
		hrtimer_start(&taos->prox_avg_timer,
			taos->prox_polling_time, HRTIMER_MODE_REL);
		taos->prox_avg_enable = 1;
	} else {
		taos_dbgmsg("cancelling prox avg poll timer\n");
		hrtimer_cancel(&taos->prox_avg_timer);
		cancel_work_sync(&taos->work_prox_avg);
		taos->prox_avg_enable = 0;
	}

	return 1;
}

static ssize_t proximity_thresh_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct taos_data *taos = dev_get_drvdata(dev);
	int thresh_hi = 0;

	msleep(20);
	thresh_hi = i2c_smbus_read_word_data(taos->i2c_client,
		(CMD_REG | PRX_MAXTHRESHLO));

	pr_err("%s: THRESHOLD = %d\n", __func__, thresh_hi);

	return sprintf(buf, "prox_threshold = %d\n", thresh_hi);
}

static ssize_t proximity_thresh_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	struct taos_data *taos = dev_get_drvdata(dev);
	int thresh_value = (u8)(taos->pdata->prox_thresh_hi);
	int err = 0;

	err = kstrtoint(buf, 10, &thresh_value);
	if (err < 0)
		pr_err("%s, kstrtoint failed.", __func__);

	taos->threshold_high = thresh_value;
	taos_thresh_set(taos);
	msleep(20);

	return size;
}
static ssize_t thresh_high_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct taos_data *taos = dev_get_drvdata(dev);
	int thresh_hi = 0,thresh_low = 0;

	msleep(20);
	thresh_low = i2c_smbus_read_word_data(taos->i2c_client,
		(CMD_REG | PRX_MINTHRESHLO));
	thresh_hi = i2c_smbus_read_word_data(taos->i2c_client,
		(CMD_REG | PRX_MAXTHRESHLO));

	pr_err("%s: thresh_hi = %d, thresh_low = %d\n", __func__, thresh_hi,thresh_low);

	return sprintf(buf, "%d,%d\n", thresh_hi,thresh_low);
}

static ssize_t thresh_high_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	struct taos_data *taos = dev_get_drvdata(dev);
	int thresh_value = (u8)(taos->pdata->prox_thresh_hi);
	int err = 0;

	err = kstrtoint(buf, 10, &thresh_value);
	printk(KERN_ERR "%s, thresh_value = %d\n",__func__,thresh_value);
	if (err < 0)
		pr_err("%s, kstrtoint failed.", __func__);

	taos->threshold_high = thresh_value;
	taos_thresh_set(taos);
	msleep(20);
	taos->set_manual_thd = true;

	return size;
}
static ssize_t thresh_low_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct taos_data *taos = dev_get_drvdata(dev);
	int thresh_hi = 0,thresh_low = 0;

	msleep(20);
	thresh_hi = i2c_smbus_read_word_data(taos->i2c_client,
		(CMD_REG | PRX_MAXTHRESHLO));
	thresh_low = i2c_smbus_read_word_data(taos->i2c_client,
		(CMD_REG | PRX_MINTHRESHLO));

	pr_err("%s: thresh_hi = %d, thresh_low = %d\n", __func__, thresh_hi,thresh_low);

	return sprintf(buf, "%d,%d\n", thresh_hi,thresh_low);
}

static ssize_t thresh_low_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	struct taos_data *taos = dev_get_drvdata(dev);
	int thresh_value = (u8)(taos->pdata->prox_thresh_low);
	int err = 0;

	err = kstrtoint(buf, 10, &thresh_value);
	printk(KERN_ERR "%s, thresh_value = %d\n",__func__,thresh_value);
	if (err < 0)
		pr_err("%s, kstrtoint failed.", __func__);

	taos->threshold_low = thresh_value;
	taos_thresh_set(taos);
	msleep(20);
	taos->set_manual_thd = true;

	return size;
}
static ssize_t get_vendor_name(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s\n", VENDOR_NAME);
}

static ssize_t get_chip_name(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s\n", CHIP_NAME);
}

static DEVICE_ATTR(vendor, S_IRUGO, get_vendor_name, NULL);
static DEVICE_ATTR(name, S_IRUGO, get_chip_name, NULL);

static ssize_t lightsensor_file_state_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct taos_data *taos = dev_get_drvdata(dev);
	int adc = 0;
	adc = lightsensor_get_adcvalue(taos);

	return sprintf(buf, "%d\n", adc);
}

static ssize_t lightsensor_raw_data_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct taos_data *taos = dev_get_drvdata(dev);

	return sprintf(buf, "%u,%u,%u,%u\n",
		taos->reddata, taos->grndata, taos->bludata, taos->clrdata);
}

static struct device_attribute dev_attr_light_raw_data =
	__ATTR(raw_data, S_IRUGO, lightsensor_raw_data_show, NULL);


static DEVICE_ATTR(adc, S_IRUGO, lightsensor_file_state_show, NULL);
static DEVICE_ATTR(lux, S_IRUGO, lightsensor_file_state_show, NULL);

static DEVICE_ATTR(poll_delay, S_IRUGO | S_IWUSR | S_IWGRP,
		   poll_delay_show, poll_delay_store);

static struct device_attribute dev_attr_light_enable =
	__ATTR(enable, S_IRUGO | S_IWUSR | S_IWGRP,
	       light_enable_show, light_enable_store);

static struct attribute *light_sysfs_attrs[] = {
	&dev_attr_light_enable.attr,
	&dev_attr_poll_delay.attr,
	NULL
};

static struct attribute_group light_attribute_group = {
	.attrs = light_sysfs_attrs,
};

static struct device_attribute *lightsensor_additional_attributes[] = {
	&dev_attr_adc,
	&dev_attr_lux,
	&dev_attr_vendor,
	&dev_attr_name,
	&dev_attr_light_raw_data,
	NULL
};

static struct device_attribute dev_attr_proximity_enable =
	__ATTR(enable, S_IRUGO | S_IWUSR | S_IWGRP,
	       proximity_enable_show, proximity_enable_store);

static struct attribute *proximity_sysfs_attrs[] = {
	&dev_attr_proximity_enable.attr,
	NULL
};

static struct attribute_group proximity_attribute_group = {
	.attrs = proximity_sysfs_attrs,
};


static struct device_attribute dev_attr_proximity_raw_data =
	__ATTR(raw_data, S_IRUGO, proximity_state_show, NULL);

static DEVICE_ATTR(prox_cal, S_IRUGO | S_IWUSR, proximity_cal_show,
	proximity_cal_store);
static DEVICE_ATTR(prox_avg, S_IRUGO|S_IWUSR, proximity_avg_show,
	proximity_avg_store);
static DEVICE_ATTR(state, S_IRUGO|S_IWUSR, proximity_state_show, NULL);

static DEVICE_ATTR(prox_offset_pass, S_IRUGO|S_IWUSR,
	prox_offset_pass_show, NULL);
static DEVICE_ATTR(prox_thresh, 0644, proximity_thresh_show,
	proximity_thresh_store);
static DEVICE_ATTR(thresh_high, 0644, thresh_high_show,
	thresh_high_store);
static DEVICE_ATTR(thresh_low, 0644, thresh_low_show,
	thresh_low_store);
static struct device_attribute *prox_sensor_attrs[] = {
	&dev_attr_state,
	&dev_attr_prox_avg,
	&dev_attr_vendor,
	&dev_attr_name,
	&dev_attr_proximity_raw_data,
	&dev_attr_prox_cal,
	&dev_attr_prox_offset_pass,
	&dev_attr_prox_thresh,
	&dev_attr_thresh_high,
	&dev_attr_thresh_low,
	NULL
};

static int lightsensor_get_adcvalue(struct taos_data *taos)
{
	int i = 0;
	int j = 0;
	unsigned int adc_total = 0;
	int adc_avr_value;
	unsigned int adc_index = 0;
	unsigned int adc_max = 0;
	unsigned int adc_min = 0;
	int value = 0;

	/* get ADC */
	value = taos_get_lux(taos);

	adc_index = (taos->adc_index_count++) % ADC_BUFFER_NUM;

	/*ADC buffer initialize (light sensor off ---> light sensor on) */
	if (!taos->adc_buf_initialized) {
		taos->adc_buf_initialized = true;
		for (j = 0; j < ADC_BUFFER_NUM; j++)
			taos->adc_value_buf[j] = value;
	} else
		taos->adc_value_buf[adc_index] = value;

	adc_max = taos->adc_value_buf[0];
	adc_min = taos->adc_value_buf[0];

	for (i = 0; i < ADC_BUFFER_NUM; i++) {
		adc_total += taos->adc_value_buf[i];

		if (adc_max < taos->adc_value_buf[i])
			adc_max = taos->adc_value_buf[i];

		if (adc_min > taos->adc_value_buf[i])
			adc_min = taos->adc_value_buf[i];
	}
	adc_avr_value = (adc_total-(adc_max+adc_min))/(ADC_BUFFER_NUM - 2);

	if (taos->adc_index_count == ADC_BUFFER_NUM)
		taos->adc_index_count = 0;

	return adc_avr_value;
}


static void taos_work_func_light(struct work_struct *work)
{
	struct taos_data *taos = container_of(work, struct taos_data,
					      work_light);
	int adc = taos_get_lux(taos);
	int cct = taos_get_cct(taos);

	input_report_rel(taos->light_input_dev, REL_MISC, adc + 1);
	input_report_rel(taos->light_input_dev, REL_WHEEL, cct);
	input_sync(taos->light_input_dev);
}

static void taos_work_func_prox(struct work_struct *work)
{
	struct taos_data *taos =
		container_of(work, struct taos_data, work_prox);
	u16 adc_data;
	u16 threshold_high;
	u16 threshold_low;
	u8 prox_int_thresh[4];
	u8 chipid = 0;
	int ret;
	int i =0;
	int proximity_value = 0;

	/* change Threshold */

	while(chipid != 0x69 && i < 10)
	{
		msleep(5);
		ret = opt_i2c_read(taos, CHIPID, &chipid);
		i++;
	}

	mutex_lock(&taos->prox_mutex);
	adc_data = i2c_smbus_read_word_data(taos->i2c_client, CMD_REG | PRX_LO);
	mutex_unlock(&taos->prox_mutex);
	threshold_high = i2c_smbus_read_word_data(taos->i2c_client,
		(CMD_REG | PRX_MAXTHRESHLO));
	threshold_low = i2c_smbus_read_word_data(taos->i2c_client,
		(CMD_REG | PRX_MINTHRESHLO));

	pr_err("%s: hi = %d, low = %d, adc_data = %d\n", __func__,
		taos->threshold_high, taos->threshold_low, adc_data);

	if ((threshold_high ==  (taos->threshold_high)) &&
		(adc_data >=  (taos->threshold_high))) {
		proximity_value = 1;
		input_report_abs(taos->proximity_input_dev,
			ABS_DISTANCE, !proximity_value);
		input_sync(taos->proximity_input_dev);
		pr_err("[%s] prox value = %d\n", __func__, !proximity_value);
		prox_int_thresh[0] = (taos->threshold_low) & 0xFF;
		prox_int_thresh[1] = (taos->threshold_low >> 8) & 0xFF;
		prox_int_thresh[2] = (0xFFFF) & 0xFF;
		prox_int_thresh[3] = (0xFFFF >> 8) & 0xFF;
		for (i = 0; i < 4; i++)
			opt_i2c_write(taos, (CMD_REG|(PRX_MINTHRESHLO + i)),
			&prox_int_thresh[i]);
	} else if ((threshold_high == (0xFFFF)) &&
	(adc_data <= (taos->threshold_low))) {
		proximity_value = 0;
		input_report_abs(taos->proximity_input_dev,
			ABS_DISTANCE, !proximity_value);
		input_sync(taos->proximity_input_dev);
		pr_err("[%s] prox value = %d\n", __func__, !proximity_value);
		prox_int_thresh[0] = (0x0000) & 0xFF;
		prox_int_thresh[1] = (0x0000 >> 8) & 0xFF;
		prox_int_thresh[2] = (taos->threshold_high) & 0xFF;
		prox_int_thresh[3] = (taos->threshold_high >> 8) & 0xFF;
		for (i = 0; i < 4; i++)
			opt_i2c_write(taos, (CMD_REG|(PRX_MINTHRESHLO + i)),
			&prox_int_thresh[i]);

	} else {
		pr_err("[%s]Error Case!adc=[%X], th_high=[%d], th_min=[%d]\n",
			__func__, adc_data, threshold_high, threshold_low);
	}

	taos->proximity_value = proximity_value;
	/* reset Interrupt pin */
	/* to active Interrupt, TMD2771x Interuupt pin shoud be reset. */
	i2c_smbus_write_byte(taos->i2c_client,
	(CMD_REG|CMD_SPL_FN|CMD_PROXALS_INTCLR));

	/* enable INT */
	enable_irq(taos->irq);
}

static void taos_work_func_prox_avg(struct work_struct *work)
{
	struct taos_data *taos = container_of(work, struct taos_data,
		work_prox_avg);
	u16 proximity_value = 0;
	int min = 0, max = 0, avg = 0;
	int i = 0;

	for (i = 0; i < PROX_AVG_COUNT; i++) {
		mutex_lock(&taos->prox_mutex);
		proximity_value = i2c_smbus_read_word_data(taos->i2c_client,
			CMD_REG | PRX_LO);
		mutex_unlock(&taos->prox_mutex);
		if (proximity_value > TAOS_PROX_MIN) {
			if (proximity_value > TAOS_PROX_MAX)
				proximity_value = TAOS_PROX_MAX;
			avg += proximity_value;
			if (!i)
				min = proximity_value;
			if (proximity_value < min)
				min = proximity_value;
			if (proximity_value > max)
				max = proximity_value;
		} else {
			proximity_value = TAOS_PROX_MIN;
		}
		msleep(40);
	}
	avg /= i;
	taos->avg[0] = min;
	taos->avg[1] = avg;
	taos->avg[2] = max;
}


/* This function is for light sensor.  It operates every a few seconds.
 * It asks for work to be done on a thread because i2c needs a thread
 * context (slow and blocking) and then reschedules the timer to run again.
 */
static enum hrtimer_restart taos_timer_func(struct hrtimer *timer)
{
	struct taos_data *taos = container_of(timer, struct taos_data, timer);
	queue_work(taos->wq, &taos->work_light);
	hrtimer_forward_now(&taos->timer, taos->light_poll_delay);
	return HRTIMER_RESTART;
}

static enum hrtimer_restart taos_prox_timer_func(struct hrtimer *timer)
{
	struct taos_data *taos = container_of(timer, struct taos_data,
		prox_avg_timer);
	queue_work(taos->wq_avg, &taos->work_prox_avg);
	hrtimer_forward_now(&taos->prox_avg_timer, taos->prox_polling_time);

	return HRTIMER_RESTART;
}


/* interrupt happened due to transition/change of near/far proximity state */
irqreturn_t taos_irq_handler(int irq, void *data)
{
	struct taos_data *ip = data;

	pr_err("taos interrupt handler is called\n");
	wake_lock_timeout(&ip->prx_wake_lock, 3*HZ);
	disable_irq_nosync(ip->irq);
	queue_work(ip->wq, &ip->work_prox);

	return IRQ_HANDLED;
}

static int taos_setup_irq(struct taos_data *taos)
{
	int rc;
	struct taos_platform_data *pdata = taos->pdata;
	int irq;

	taos_dbgmsg("start\n");

	rc = gpio_request(pdata->als_int, "gpio_proximity_out");
	if (rc < 0) {
		pr_err("%s: gpio %d request failed (%d)\n",
			__func__, pdata->als_int, rc);
		return rc;
	}

	rc = gpio_direction_input(pdata->als_int);
	if (rc < 0) {
		pr_err("%s: failed to set gpio %d as input (%d)\n",
			__func__, pdata->als_int, rc);
		goto err_gpio_direction_input;
	}

	irq = gpio_to_irq(pdata->als_int);

	rc = request_threaded_irq(irq, NULL,
			 taos_irq_handler,
			 IRQF_TRIGGER_FALLING,
			 "proximity_int",
			 taos);
	if (rc < 0) {
		pr_err("%s: request_irq(%d) failed for gpio %d (%d)\n",
			__func__, irq,
			pdata->als_int, rc);
		goto err_request_irq;
	}

	/* start with interrupts disabled */
	disable_irq(irq);
	taos->irq = irq;

	taos_dbgmsg("success\n");

	goto done;

err_request_irq:
err_gpio_direction_input:
	gpio_free(pdata->als_int);
done:
	return rc;
}

static int taos_get_initial_offset(struct taos_data *taos)
{
	int ret = 0;
	u8 p_offset = 0;

	if (taos->pdata->power)
		taos->pdata->power(true);

	ret = proximity_open_offset(taos);
	if(ret < 0) {
		p_offset = 0;
		 taos->offset_value = 0;
	} else
		p_offset = taos->offset_value;

	pr_err("%s: initial offset = %d\n", __func__, p_offset);

	if (taos->pdata->power)
		taos->pdata->power(false);

	return p_offset;
}

static int taos_i2c_probe(struct i2c_client *client,
			  const struct i2c_device_id *id)
{
	int ret = -ENODEV;
	int chipid = 0;
	struct input_dev *input_dev;
	struct taos_data *taos;
	struct taos_platform_data *pdata = client->dev.platform_data;

	if (!pdata) {
		pr_err("%s: missing pdata!\n", __func__);
		return ret;
	}

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		pr_err("%s: i2c functionality check failed!\n", __func__);
		return ret;
	}

	taos = kzalloc(sizeof(struct taos_data), GFP_KERNEL);
	if (!taos) {
		pr_err("%s: failed to alloc memory for module data\n",
		       __func__);
		return -ENOMEM;
	}

	chipid = i2c_smbus_read_byte_data(client, CMD_REG | CHIPID);
	if (chipid != CHIP_ID) {
		pr_err("%s: i2c read error [%X]\n", __func__, chipid);
		goto err_chip_id_or_i2c_error;
	}

	taos->pdata = pdata;
	taos->i2c_client = client;
	i2c_set_clientdata(client, taos);
	taos->offset_value = taos_get_initial_offset(taos);
	proximity_open_window_type(taos);

	taos->set_manual_thd = false;
	taos->threshold_high = taos->pdata->prox_thresh_hi + taos->offset_value;
	taos->threshold_low = taos->pdata->prox_thresh_low + taos->offset_value;

	mutex_init(&taos->prox_mutex);

	/* wake lock init */
	wake_lock_init(&taos->prx_wake_lock, WAKE_LOCK_SUSPEND,
		       "prx_wake_lock");
	mutex_init(&taos->power_lock);

	/* allocate proximity input_device */
	input_dev = input_allocate_device();
	if (!input_dev) {
		pr_err("%s: could not allocate input device\n", __func__);
		goto err_input_allocate_device_proximity;
	}
	taos->proximity_input_dev = input_dev;
	input_set_drvdata(input_dev, taos);
	input_dev->name = "proximity_sensor";
	input_set_capability(input_dev, EV_ABS, ABS_DISTANCE);
	input_set_abs_params(input_dev, ABS_DISTANCE, 0, 1, 0, 0);

	taos_dbgmsg("registering proximity input device\n");
	ret = input_register_device(input_dev);
	if (ret < 0) {
		pr_err("%s: could not register input device\n", __func__);
		input_free_device(input_dev);
		goto err_input_register_device_proximity;
	}
	ret = sysfs_create_group(&input_dev->dev.kobj,
				 &proximity_attribute_group);
	if (ret) {
		pr_err("%s: could not create sysfs group\n", __func__);
		goto err_sysfs_create_group_proximity;
	}

	/* hrtimer settings.  we poll for light values using a timer. */
	hrtimer_init(&taos->timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	taos->light_poll_delay = ns_to_ktime(200 * NSEC_PER_MSEC);
	taos->timer.function = taos_timer_func;

	/* the timer just fires off a work queue request.  we need a thread
	   to read the i2c (can be slow and blocking). */
	taos->wq = create_singlethread_workqueue("taos_wq");
	if (!taos->wq) {
		ret = -ENOMEM;
		pr_err("%s: could not create workqueue\n", __func__);
		goto err_create_workqueue;
	}


	taos->wq_avg = create_singlethread_workqueue("taos_wq_avg");
	if (!taos->wq_avg) {
		ret = -ENOMEM;
		pr_err("%s: could not create workqueue\n", __func__);
		goto err_create_avg_workqueue;
	}

	/* this is the thread function we run on the work queue */
	INIT_WORK(&taos->work_light, taos_work_func_light);
	INIT_WORK(&taos->work_prox, taos_work_func_prox);
	INIT_WORK(&taos->work_prox_avg, taos_work_func_prox_avg);
	taos->prox_avg_enable = 0;

	hrtimer_init(&taos->prox_avg_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	taos->prox_polling_time = ns_to_ktime(2000 * NSEC_PER_MSEC);
	taos->prox_avg_timer.function = taos_prox_timer_func;

	/* allocate lightsensor-level input_device */
	input_dev = input_allocate_device();
	if (!input_dev) {
		pr_err("%s: could not allocate input device\n", __func__);
		ret = -ENOMEM;
		goto err_input_allocate_device_light;
	}
	input_set_drvdata(input_dev, taos);
	input_dev->name = "light_sensor";
	input_set_capability(input_dev, EV_REL, REL_MISC);
	input_set_capability(input_dev, EV_REL, REL_WHEEL);

	taos_dbgmsg("registering lightsensor-level input device\n");
	ret = input_register_device(input_dev);
	if (ret < 0) {
		pr_err("%s: could not register input device\n", __func__);
		input_free_device(input_dev);
		goto err_input_register_device_light;
	}
	taos->light_input_dev = input_dev;
	ret = sysfs_create_group(&input_dev->dev.kobj,
				 &light_attribute_group);
	if (ret) {
		pr_err("%s: could not create sysfs group\n", __func__);
		goto err_sysfs_create_group_light;
	}
	ret = taos_setup_irq(taos);
	if (ret) {
		pr_err("%s: could not setup irq\n", __func__);
		goto err_setup_irq;
	}

/* set sysfs for proximity sensor and light sensor */

	ret = sensors_register(taos->proximity_dev,
		taos, prox_sensor_attrs,
			"proximity_sensor");
	if (ret) {
		pr_err("%s: cound not register proximity sensor device(%d).\n",
			__func__, ret);
		goto out_sensor_register_failed;
	}

#ifdef CONFIG_SENSOR_USE_SYMLINK
	ret =  sensors_initialize_symlink(taos->proximity_input_dev);
	if (ret) {
		pr_err("%s: cound not make proximity sensor symlink(%d).\n",
			__func__, ret);
		goto out_sensors_initialize_symlink;
	}
#endif

	ret = sensors_register(taos->light_dev,
		taos, lightsensor_additional_attributes,
			"light_sensor");
	if (ret) {
		pr_err("%s: cound not register light sensor device(%d).\n",
			__func__, ret);
		goto out_sensor_register_failed1;
	}

#ifdef CONFIG_SENSOR_USE_SYMLINK
	ret =  sensors_initialize_symlink(taos->light_input_dev);
	if (ret) {
		pr_err("%s: cound not make light sensor symlink(%d).\n",
			__func__, ret);
		goto out_out_sensors_initialize_symlink1;
	}
#endif

	goto done;

	/* error, unwind it all */
#ifdef CONFIG_SENSOR_USE_SYMLINK
out_out_sensors_initialize_symlink1:
#endif
out_sensor_register_failed1:
	sensors_unregister(taos->light_dev);
#ifdef CONFIG_SENSOR_USE_SYMLINK
	sensors_delete_symlink(taos->proximity_input_dev);
out_sensors_initialize_symlink:
#endif
out_sensor_register_failed:
	sensors_unregister(taos->proximity_dev);
err_setup_irq:
err_sysfs_create_group_light:
	input_unregister_device(taos->light_input_dev);
err_input_register_device_light:
err_input_allocate_device_light:
	destroy_workqueue(taos->wq_avg);
err_create_avg_workqueue:
	destroy_workqueue(taos->wq);
err_create_workqueue:
	sysfs_remove_group(&taos->proximity_input_dev->dev.kobj,
			   &proximity_attribute_group);
err_sysfs_create_group_proximity:
	input_unregister_device(taos->proximity_input_dev);
err_input_register_device_proximity:
err_input_allocate_device_proximity:
	free_irq(taos->irq, 0);
	gpio_free(taos->pdata->als_int);
	mutex_destroy(&taos->power_lock);
	wake_lock_destroy(&taos->prx_wake_lock);
err_chip_id_or_i2c_error:
	kfree(taos);
done:
	return ret;
}

static int taos_suspend(struct device *dev)
{
	/* We disable power only if proximity is disabled.  If proximity
	   is enabled, we leave power on because proximity is allowed
	   to wake up device.  We remove power without changing
	   gp2a->power_state because we use that state in resume.
	*/
	struct i2c_client *client = to_i2c_client(dev);
	struct taos_data *taos = i2c_get_clientdata(client);

	if (taos->power_state & LIGHT_ENABLED)
		taos_light_disable(taos);
	if (taos->power_state == LIGHT_ENABLED)
		taos->pdata->power(false);
	return 0;
}

static int taos_resume(struct device *dev)
{
	/* Turn power back on if we were before suspend. */
	struct i2c_client *client = to_i2c_client(dev);
	struct taos_data *taos = i2c_get_clientdata(client);

	if (taos->power_state == LIGHT_ENABLED)
		taos->pdata->power(true);
	if (taos->power_state & LIGHT_ENABLED)
		taos_light_enable(taos);
	return 0;
}

static int taos_i2c_remove(struct i2c_client *client)
{
	struct taos_data *taos = i2c_get_clientdata(client);
	sysfs_remove_group(&taos->light_input_dev->dev.kobj,
			   &light_attribute_group);
	input_unregister_device(taos->light_input_dev);
	sysfs_remove_group(&taos->proximity_input_dev->dev.kobj,
			   &proximity_attribute_group);
	input_unregister_device(taos->proximity_input_dev);
	free_irq(taos->irq, NULL);
	gpio_free(taos->pdata->als_int);

	if (taos->power_state) {
		taos->power_state = 0;
		if (taos->power_state & LIGHT_ENABLED)
			taos_light_disable(taos);
		taos->pdata->power(false);
	}
	destroy_workqueue(taos->wq);
	destroy_workqueue(taos->wq_avg);

	mutex_destroy(&taos->power_lock);
	wake_lock_destroy(&taos->prx_wake_lock);
	kfree(taos);
	return 0;
}

static const struct i2c_device_id taos_device_id[] = {
	{"taos", 0},
	{}
};
MODULE_DEVICE_TABLE(i2c, taos_device_id);

static const struct dev_pm_ops taos_pm_ops = {
	.suspend = taos_suspend,
	.resume = taos_resume
};

static struct i2c_driver taos_i2c_driver = {
	.driver = {
		.name = "taos",
		.owner = THIS_MODULE,
		.pm = &taos_pm_ops
	},
	.probe		= taos_i2c_probe,
	.remove		= taos_i2c_remove,
	.id_table	= taos_device_id,
};


static int __init taos_init(void)
{
	return i2c_add_driver(&taos_i2c_driver);
}

static void __exit taos_exit(void)
{
	i2c_del_driver(&taos_i2c_driver);
}

module_init(taos_init);
module_exit(taos_exit);

MODULE_AUTHOR("SAMSUNG");
MODULE_DESCRIPTION("Optical Sensor driver for taos");
MODULE_LICENSE("GPL");


