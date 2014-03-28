/*
	$License:
	Copyright (C) 2011 InvenSense Corporation, All Rights Reserved.

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
	$
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/input-polldev.h>
#include <linux/miscdevice.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/pm.h>
#include <linux/pm_runtime.h>
#include <linux/irq.h>
#include <linux/gpio.h>

#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif

#include <linux/mpu6050_input.h>
#include "mpu6050_selftest.h"

struct mpu6050_selftest_info {
	int gyro_sens;
	int gyro_fs;
	int packet_thresh;
	int total_timing_tol;
	int bias_thresh;
	int rms_thresh;
	unsigned int tests_per_axis;
	unsigned short accel_samples;
};

struct mpu6050_selftest {
	unsigned char pwm_mgmt[2];
	unsigned char smplrt_div;
	unsigned char user_ctrl;
	unsigned char config;
	unsigned char gyro_config;
	unsigned char int_enable;
};

static struct mpu6050_selftest mpu6050_selftest;

short mpu6050_big8_to_int16(const unsigned char *big8)
{
	short x;
	x = ((short)big8[0] << 8) | ((short)big8[1]);
	return x;
}

static int mpu6050_backup_register(struct i2c_client *client)
{
	int result = 0;

	result =
	    mpu6050_i2c_read_reg(client, MPUREG_PWR_MGMT_1,
				 2, mpu6050_selftest.pwm_mgmt);
	if (result)
		return result;

	result =
	    mpu6050_i2c_read_reg(client, MPUREG_CONFIG,
				 1, &mpu6050_selftest.config);
	if (result)
		return result;

	result =
	    mpu6050_i2c_read_reg(client, MPUREG_GYRO_CONFIG,
				 1, &mpu6050_selftest.gyro_config);
	if (result)
		return result;

	result =
	    mpu6050_i2c_read_reg(client, MPUREG_USER_CTRL,
				 1, &mpu6050_selftest.user_ctrl);
	if (result)
		return result;

	result =
	    mpu6050_i2c_read_reg(client, MPUREG_INT_ENABLE,
				 1, &mpu6050_selftest.int_enable);
	if (result)
		return result;

	result =
	    mpu6050_i2c_read_reg(client, MPUREG_SMPLRT_DIV,
				 1, &mpu6050_selftest.smplrt_div);
	if (result)
		return result;

	return result;
}

static int mpu6050_recover_register(struct i2c_client *client)
{
	int result = 0;

	result =
	    mpu6050_i2c_write_single_reg(client, MPUREG_PWR_MGMT_1,
					 mpu6050_selftest.pwm_mgmt[0]);
	if (result)
		return result;

	result =
	    mpu6050_i2c_write_single_reg(client, MPUREG_PWR_MGMT_2,
					 mpu6050_selftest.pwm_mgmt[1]);
	if (result)
		return result;

	result =
	    mpu6050_i2c_write_single_reg(client, MPUREG_CONFIG,
					 mpu6050_selftest.config);
	if (result)
		return result;

	result =
	    mpu6050_i2c_write_single_reg(client, MPUREG_GYRO_CONFIG,
					 mpu6050_selftest.gyro_config);
	if (result)
		return result;

	result =
	    mpu6050_i2c_write_single_reg(client, MPUREG_USER_CTRL,
					 mpu6050_selftest.user_ctrl);
	if (result)
		return result;

	result =
	    mpu6050_i2c_write_single_reg(client, MPUREG_SMPLRT_DIV,
					 mpu6050_selftest.smplrt_div);
	if (result)
		return result;

	result =
	    mpu6050_i2c_write_single_reg(client, MPUREG_INT_ENABLE,
					 mpu6050_selftest.int_enable);
	if (result)
		return result;

	return result;
}

u32 mpu6050_selftest_sqrt(u32 sqsum)
{
	u32 sq_rt;
	u32 g0, g1, g2, g3, g4;
	u32 seed;
	u32 next;
	u32 step;

	g4 = sqsum / 100000000;
	g3 = (sqsum - g4 * 100000000) / 1000000;
	g2 = (sqsum - g4 * 100000000 - g3 * 1000000) / 10000;
	g1 = (sqsum - g4 * 100000000 - g3 * 1000000 - g2 * 10000) / 100;
	g0 = (sqsum - g4 * 100000000 - g3 * 1000000 - g2 * 10000 - g1 * 100);

	next = g4;
	step = 0;
	seed = 0;
	while (((seed + 1) * (step + 1)) <= next) {
		step++;
		seed++;
	}

	sq_rt = seed * 10000;
	next = (next - (seed * step)) * 100 + g3;

	step = 0;
	seed = 2 * seed * 10;
	while (((seed + 1) * (step + 1)) <= next) {
		step++;
		seed++;
	}

	sq_rt = sq_rt + step * 1000;
	next = (next - seed * step) * 100 + g2;
	seed = (seed + step) * 10;
	step = 0;
	while (((seed + 1) * (step + 1)) <= next) {
		step++;
		seed++;
	}

	sq_rt = sq_rt + step * 100;
	next = (next - seed * step) * 100 + g1;
	seed = (seed + step) * 10;
	step = 0;

	while (((seed + 1) * (step + 1)) <= next) {
		step++;
		seed++;
	}

	sq_rt = sq_rt + step * 10;
	next = (next - seed * step) * 100 + g0;
	seed = (seed + step) * 10;
	step = 0;

	while (((seed + 1) * (step + 1)) <= next) {
		step++;
		seed++;
	}

	sq_rt = sq_rt + step;

	return sq_rt;
}

int mpu6050_selftest_run(struct i2c_client *client,
			 int packet_cnt[3],
			 int gyro_bias[3],
			 int gyro_rms[3], int gyro_lsb_bias[3])
{
	int ret_val = 0;
	int result;
	int total_count = 0;
	int total_count_axis[3] = { 0, 0, 0 };
	int packet_count;
	long avg[3];
	long rms[3];
	short *x;
	short *y;
	short *z;
	int buf_size = sizeof(short) * DEF_PERIOD_CAL
			* DEF_TESTS_PER_AXIS / 8 * 4;

	int i, j, tmp;
	char tmpStr[200];
	unsigned char regs[7] = { 0, };
	unsigned char dataout[20];
	int perform_full_test = 0;
	int dps_rms[3] = { 0, };
	u32 temp;
	struct mpu6050_selftest_info test_setup = {
		DEF_GYRO_SENS, DEF_GYRO_FULLSCALE, DEF_PACKET_THRESH,
		DEF_TOTAL_TIMING_TOL, (int)DEF_BIAS_THRESH_SELF,
		DEF_RMS_LSB_THRESH_SELF * DEF_RMS_LSB_THRESH_SELF,
		/* now obsolete - has no effect */
		DEF_TESTS_PER_AXIS, DEF_N_ACCEL_SAMPLES
	};

	char a_name[3][2] = { "X", "Y", "Z" };
	x = kzalloc(buf_size, GFP_KERNEL);
	y = kzalloc(buf_size, GFP_KERNEL);
	z = kzalloc(buf_size, GFP_KERNEL);

	/*backup registers */
	result = mpu6050_backup_register(client);
	if (result) {
		pr_err("register backup error=%d", result);
		goto err_state;
	}

	if (mpu6050_selftest.pwm_mgmt[0] & 0x40) {
		result =
		    mpu6050_i2c_write_single_reg(client, MPUREG_PWR_MGMT_1,
						 0x00);
		if (result) {
			pr_err("init PWR_MGMT error=%d", result);
			goto err_state;
		}
	}

	regs[0] =
		mpu6050_selftest.pwm_mgmt[1] &
			~(BIT_STBY_XG | BIT_STBY_YG | BIT_STBY_ZG);
	result =
		mpu6050_i2c_write_single_reg(client,
			MPUREG_PWR_MGMT_2, regs[0]);

	/* make sure the DMP is disabled first */
	result = mpu6050_i2c_write_single_reg(client, MPUREG_USER_CTRL, 0x00);
	if (result) {
		pr_info("DMP disable error=%d", result);
		goto err_state;
	}

	/* reset the gyro offset values */
	regs[0] = MPUREG_XG_OFFS_USRH;
	result = mpu6050_i2c_write(client, 6, regs);
	if (result)
		goto err_state;

	/* sample rate */
	if (perform_full_test) {

		/* = 8ms */
		result =
		    mpu6050_i2c_write_single_reg(client, MPUREG_SMPLRT_DIV,
						 0x07);
		test_setup.bias_thresh = DEF_BIAS_LSB_THRESH_CAL;
	} else {

		/* = 1ms */
		result =
		    mpu6050_i2c_write_single_reg(client, MPUREG_SMPLRT_DIV,
						 0x00);
		test_setup.bias_thresh = DEF_BIAS_LSB_THRESH_SELF;
	}
	if (result)
		goto err_state;

	regs[0] = 0x03;		/* filter = 42Hz, analog_sample rate = 1 KHz */
	switch (test_setup.gyro_fs) {
	case 2000:
		regs[0] |= 0x18;
		break;
	case 1000:
		regs[0] |= 0x10;
		break;
	case 500:
		regs[0] |= 0x08;
		break;
	case 250:
	default:
		regs[0] |= 0x00;
		break;
	}
	result = mpu6050_i2c_write_single_reg(client, MPUREG_CONFIG, regs[0]);
	if (result)
		goto err_state;

	switch (test_setup.gyro_fs) {
	case 2000:
		regs[0] = 0x03;
		break;
	case 1000:
		regs[0] = 0x02;
		break;
	case 500:
		regs[0] = 0x01;
		break;
	case 250:
	default:
		regs[0] = 0x00;
		break;
	}
	result =
	    mpu6050_i2c_write_single_reg(client, MPUREG_GYRO_CONFIG,
					 regs[0] << 3);
	if (result)
		goto err_state;
	result = mpu6050_i2c_write_single_reg(client, MPUREG_INT_ENABLE, 0x00);
	if (result)
		goto err_state;

	/* 1st, timing test */
	for (j = 0; j < 3; j++) {
		pr_info("Collecting gyro data from %s gyro PLL\n", a_name[j]);

		/* turn on all gyros, use gyro X for clocking
		   Set to Y and Z for 2nd and 3rd iteration */
		result =
		    mpu6050_i2c_write_single_reg(client, MPUREG_PWR_MGMT_1,
						 j + 1);
		if (result)
			goto err_state;

		/* wait for 50 ms after switching clock source */
		msleep(50);
		/* enable & reset FIFO */
		result =
		    mpu6050_i2c_write_single_reg(client, MPUREG_USER_CTRL,
						 BIT_FIFO_EN | BIT_FIFO_RST);
		if (result)
			goto err_state;

		tmp = test_setup.tests_per_axis;
		while (tmp-- > 0) {
			const unsigned char fifo_en_reg = MPUREG_FIFO_EN;

			/* enable XYZ gyro in FIFO and nothing else */
			result =
			    mpu6050_i2c_write_single_reg(client, fifo_en_reg,
							 BIT_GYRO_XOUT |
							 BIT_GYRO_YOUT |
							 BIT_GYRO_ZOUT);
			if (result)
				goto err_state;

			pr_info("%s : before sampling ...\n", __func__);
			/* wait one period for data */
			if (perform_full_test)
				msleep(DEF_PERIOD_CAL);

			else
				msleep(DEF_PERIOD_SELF);
			pr_info("%s : after sampling ...\n", __func__);

			/* stop storing gyro in the FIFO */
			result =
			    mpu6050_i2c_write_single_reg(client,
							 fifo_en_reg, 0x00);
			if (result)
				goto err_state;

			/* Getting number of bytes in FIFO */
			result =
			    mpu6050_i2c_read_reg(client, MPUREG_FIFO_COUNTH,
						 2, dataout);
			if (result)
				goto err_state;

			/* number of 6 B packets in the FIFO */
			packet_count = mpu6050_big8_to_int16(dataout) / 6;
			pr_info("%s : sampling result packet_count =%d\n",
				__func__, packet_count);
			sprintf(tmpStr, "Packet Count: %d -", packet_count);
			if (packet_count - (test_setup.packet_thresh - 3)
			    > 0) {
				for (i = 0; i < packet_count; i++) {

					/* getting FIFO data */
					result =
					    mpu6050_i2c_read_fifo(client, 6,
								  dataout);
					if (result)
						goto err_state;

					x[total_count + i] =
					    mpu6050_big8_to_int16(&dataout[0]);
					y[total_count + i] =
					    mpu6050_big8_to_int16(&dataout[2]);
					z[total_count + i] =
					    mpu6050_big8_to_int16(&dataout[4]);
				}
				total_count += packet_count;
				total_count_axis[j] += packet_count;
				packet_cnt[j] = packet_count;
				sprintf(tmpStr, "%sOK success", tmpStr);
			} else {
				ret_val |= 1 << j;
				sprintf(tmpStr, "%sNOK - samples ignored ...",
					tmpStr);
			}
			pr_info("%s\n", tmpStr);
		}

		/* remove gyros from FIFO */
		result =
		    mpu6050_i2c_write_single_reg(client, MPUREG_FIFO_EN, 0x00);
		if (result)
			goto err_state;
	}
	pr_info("\nTotal %d samples\n", total_count);

	/* 2nd, check bias from X, Y, and Z PLL clock source */
	tmp = total_count != 0 ? total_count : 1;
	for (i = 0, avg[0] = 0, avg[1] = 0, avg[2] = 0; i < total_count; i++) {
		avg[0] += x[i];
		avg[1] += y[i];
		avg[2] += z[i];
	}
	avg[0] /= tmp;
	avg[1] /= tmp;
	avg[2] /= tmp;

	pr_info("bias : %+8ld %+8ld %+8ld (LSB)\n", avg[0], avg[1], avg[2]);

	gyro_bias[0] = (avg[0] * DEF_SCALE_FOR_FLOAT) / DEF_GYRO_SENS;
	gyro_bias[1] = (avg[1] * DEF_SCALE_FOR_FLOAT) / DEF_GYRO_SENS;
	gyro_bias[2] = (avg[2] * DEF_SCALE_FOR_FLOAT) / DEF_GYRO_SENS;
	gyro_lsb_bias[0] = avg[0];
	gyro_lsb_bias[1] = avg[1];
	gyro_lsb_bias[2] = avg[2];

	if (VERBOSE_OUT) {
		pr_info("abs bias : %+8d.%03d   %+8d.%03d  %+8d.%03d (dps)\n",
			(int)abs(gyro_bias[0]) / DEF_SCALE_FOR_FLOAT,
			(int)abs(gyro_bias[0]) % DEF_SCALE_FOR_FLOAT,
			(int)abs(gyro_bias[1]) / DEF_SCALE_FOR_FLOAT,
			(int)abs(gyro_bias[1]) % DEF_SCALE_FOR_FLOAT,
			(int)abs(gyro_bias[2]) / DEF_SCALE_FOR_FLOAT,
			(int)abs(gyro_bias[2]) % DEF_SCALE_FOR_FLOAT);
	}

	for (j = 0; j < 3; j++) {
		if (abs(avg[j]) > test_setup.bias_thresh) {
			pr_info("%s-Gyro bias (%ld) exceeded threshold "
				"(threshold = %d LSB)\n", a_name[j],
				avg[j], test_setup.bias_thresh);
			ret_val |= 1 << (3 + j);
		}
	}

	/* 3rd, check RMS for dead gyros
	   If any of the RMS noise value returns zero,
	   then we might have dead gyro or FIFO/register failure,
	   the part is sleeping, or the part is not responsive */
	for (i = 0, rms[0] = 0, rms[1] = 0, rms[2] = 0; i < total_count; i++) {
		rms[0] += (long)(x[i] - avg[0]) * (x[i] - avg[0]);
		rms[1] += (long)(y[i] - avg[1]) * (y[i] - avg[1]);
		rms[2] += (long)(z[i] - avg[2]) * (z[i] - avg[2]);
	}

	if (rms[0] == 0 || rms[1] == 0 || rms[2] == 0)
		ret_val |= 1 << 6;

	if (VERBOSE_OUT) {
		pr_info("RMS ^ 2 : %+8ld %+8ld %+8ld\n",
			(long)rms[0] / total_count,
			(long)rms[1] / total_count, (long)rms[2] / total_count);
	}

	for (j = 0; j < 3; j++) {
		if (rms[j] / total_count > test_setup.rms_thresh) {
			pr_info("%s-Gyro rms (%ld) exceeded threshold "
				"(threshold = %d LSB)\n", a_name[j],
				rms[j] / total_count, test_setup.rms_thresh);
			ret_val |= 1 << (7 + j);
		}
	}

	for (i = 0; i < 3; i++) {
		if (rms[i] > 10000) {
			temp =
			    ((u32) (rms[i] / total_count)) *
			    DEF_RMS_SCALE_FOR_RMS;
		} else {
			temp =
			    ((u32) (rms[i] * DEF_RMS_SCALE_FOR_RMS)) /
			    total_count;
		}
		if (rms[i] < 0)
			temp = 1 << 31;

		dps_rms[i] = mpu6050_selftest_sqrt(temp) / DEF_GYRO_SENS;

		gyro_rms[i] =
		    dps_rms[i] * DEF_SCALE_FOR_FLOAT / DEF_SQRT_SCALE_FOR_RMS;
	}

	pr_info("RMS : %+8d.%03d	 %+8d.%03d  %+8d.%03d (dps)\n",
		(int)abs(gyro_rms[0]) / DEF_SCALE_FOR_FLOAT,
		(int)abs(gyro_rms[0]) % DEF_SCALE_FOR_FLOAT,
		(int)abs(gyro_rms[1]) / DEF_SCALE_FOR_FLOAT,
		(int)abs(gyro_rms[1]) % DEF_SCALE_FOR_FLOAT,
		(int)abs(gyro_rms[2]) / DEF_SCALE_FOR_FLOAT,
		(int)abs(gyro_rms[2]) % DEF_SCALE_FOR_FLOAT);

	/*recover registers */
	result = mpu6050_recover_register(client);
	if (result) {
		pr_err("register recovering error=%d", result);
		goto err_state;
	}
	result = ret_val;
err_state:
	kfree(x);
	kfree(y);
	kfree(z);
	return result;
}

static const int gyro_6050_st_tb[31] = {
	3275, 3425, 3583, 3748, 3920, 4100, 4289, 4486,
	4693, 4909, 5134, 5371, 5618, 5876, 6146, 6429,
	6725, 7034, 7358, 7696, 8050, 8421, 8808, 9213,
	9637, 10080, 10544, 11029, 11537, 12067, 12622
};

static int mpu6050_do_powerup(struct i2c_client *client)
{
	int result = 0;
	char reg;

	mpu6050_i2c_write_single_reg
		(client, MPUREG_PWR_MGMT_1, 0x1);

	msleep(20);

	mpu6050_i2c_read_reg
		(client, MPUREG_PWR_MGMT_2, 1, &reg);

	reg &= ~(BIT_STBY_XG | BIT_STBY_YG | BIT_STBY_ZG);
	mpu6050_i2c_write_single_reg
		(client, MPUREG_PWR_MGMT_2, reg);

	return result;
}

static int mpu6050_do_test(struct i2c_client *client, int self_test_flag,
		int *gyro_result)
{
	int result, i, j, packet_size;
	u8 data[BYTES_PER_SENSOR * 2], d;
	int fifo_count, packet_count, ind, s;

	packet_size = BYTES_PER_SENSOR;

	result = mpu6050_i2c_write_single_reg(client, MPUREG_INT_ENABLE, 0);
	if (result)
		return result;
	/* disable the sensor output to FIFO */
	result = mpu6050_i2c_write_single_reg(client, MPUREG_FIFO_EN, 0);
	if (result)
		return result;
	/* disable fifo reading */
	result = mpu6050_i2c_write_single_reg(client, MPUREG_USER_CTRL, 0);
	if (result)
		return result;
	/* clear FIFO */
	result = mpu6050_i2c_write_single_reg(client,
		MPUREG_USER_CTRL, BIT_FIFO_RST);
	if (result)
		return result;
	/* setup parameters */
	result = mpu6050_i2c_write_single_reg(client, MPUREG_CONFIG,
		MPU_FILTER_188HZ);
	if (result)
		return result;
	result = mpu6050_i2c_write_single_reg(client, MPUREG_SMPLRT_DIV,
		0x0);
	if (result)
		return result;
	result = mpu6050_i2c_write_single_reg(client, MPUREG_GYRO_CONFIG,
		self_test_flag | (MPU_FS_250DPS << 3));
	if (result)
		return result;

	/* wait for the output to get stable */
	msleep(DEF_ST_STABLE_TIME);

	/* enable FIFO reading */
	result = mpu6050_i2c_write_single_reg(client, MPUREG_USER_CTRL,
		BIT_FIFO_EN);
	if (result)
		return result;
	/* enable sensor output to FIFO */
	d = BITS_GYRO_OUT;
	result = mpu6050_i2c_write_single_reg(client, MPUREG_FIFO_EN, d);
	if (result)
		return result;

	for (i = 0; i < THREE_AXIS; i++)
		gyro_result[i] = 0;

	s = 0;
	while (s < INIT_ST_SAMPLES) {
		msleep(DEF_GYRO_WAIT_TIME);
		result = mpu6050_i2c_read_reg(client,  MPUREG_FIFO_COUNTH,
					FIFO_COUNT_BYTE, data);
		if (result)
			return result;
		fifo_count = be16_to_cpup((__be16 *)(&data[0]));
		packet_count = fifo_count / packet_size;
		result = mpu6050_i2c_read_reg(client, MPUREG_FIFO_R_W,
				packet_size, data);
		if (result)
			return result;
		i = 0;
		while ((i < packet_count) && (s < INIT_ST_SAMPLES)) {
			result = mpu6050_i2c_read_reg(client, MPUREG_FIFO_R_W,
				packet_size, data);
			if (result)
				return result;
			ind = 0;

			for (j = 0; j < THREE_AXIS; j++)
				gyro_result[j] +=
					(short)be16_to_cpup(
					(__be16 *)(&data[ind + 2 * j]));
			s++;
			i++;
		}
	}

	/* stop sending data to FIFO */
	result = mpu6050_i2c_write_single_reg(client, MPUREG_FIFO_EN, 0);
	if (result)
		return result;
	for (j = 0; j < THREE_AXIS; j++) {
		gyro_result[j] = gyro_result[j]/s;
		gyro_result[j] *= DEF_ST_PRECISION;
	}

	return 0;
}


static int mpu6050_gyro_self_test(struct i2c_client *client,
	int *reg_avg, int *st_avg, int *ratio){
	int result;
	int ret_val;
	int ct_shift_prod[3], st_shift_cust[3], st_shift_ratio[3], i;
	u8 regs[3];

	ret_val = 0;
	result = mpu6050_i2c_read_reg(client, REG_ST_GCT_X, 3, regs);
	if (result)
		return result;
	regs[0] &= 0x1f;
	regs[1] &= 0x1f;
	regs[2] &= 0x1f;

	for (i = 0; i < 3; i++) {
		if (regs[i] != 0)
			ct_shift_prod[i] = gyro_6050_st_tb[regs[i] - 1];
		else
			ct_shift_prod[i] = 0;
	}

	for (i = 0; i < 3; i++) {
		st_shift_cust[i] = abs(reg_avg[i] - st_avg[i]);
		if (ct_shift_prod[i]) {
			st_shift_ratio[i] = abs(st_shift_cust[i] /
				ct_shift_prod[i] - DEF_ST_PRECISION);
			ratio[i] = st_shift_ratio[i];
			if (st_shift_ratio[i] > DEF_GYRO_CT_SHIFT_DELTA)
				ret_val |= 1 << i;
		} else {
			if (st_shift_cust[i] < DEF_ST_PRECISION *
				DEF_GYRO_CT_SHIFT_MIN * DEF_SELFTEST_GYRO_SENS)
				ret_val |= 1 << i;
			if (st_shift_cust[i] > DEF_ST_PRECISION *
				DEF_GYRO_CT_SHIFT_MAX * DEF_SELFTEST_GYRO_SENS)
				ret_val |= 1 << i;
		}
	}

	pr_info("ct_shift_prod : %d %d %d\n",
		ct_shift_prod[0], ct_shift_prod[1], ct_shift_prod[2]);

	pr_info("st_shift_cust : %d %d %d\n",
		st_shift_cust[0], st_shift_cust[1], st_shift_cust[2]);

	pr_info("st_shift_ratio : %d %d %d\n",
		st_shift_ratio[0], st_shift_ratio[1], st_shift_ratio[2]);

	/* check for absolute value passing criterion. Using DEF_ST_TOR
	 * for certain degree of tolerance */
	for (i = 0; i < 3; i++) {
		if (abs(reg_avg[i]) > DEF_ST_TOR * DEF_ST_ABS_THRESH *
		    DEF_ST_PRECISION * DEF_GYRO_SCALE)
			ret_val |= (1 << i);
	}

	return ret_val;
}


int mpu6050_gyro_hw_self_check(struct i2c_client *client,
			 int ratio[3])
{
	int result;
	int gyro_bias_st[THREE_AXIS], gyro_bias_regular[THREE_AXIS];
	int test_times;
	int gyro_result;

	/*backup registers */
	result = mpu6050_backup_register(client);
	if (result) {
		pr_err("register backup error=%d", result);
		goto err_state;
	}

	/*power-up*/
	mpu6050_do_powerup(client);

	/*get regular bias*/
	test_times = DEF_ST_TRY_TIMES;
	while (test_times > 0) {
		result = mpu6050_do_test(client, 0, gyro_bias_regular);
		if (result == -EAGAIN)
			test_times--;
		else
			test_times = 0;
	}
	if (result)
		goto test_fail;

	/*get st bias*/
	test_times = DEF_ST_TRY_TIMES;
	while (test_times > 0) {
		result = mpu6050_do_test(client,
				BITS_SELF_TEST_EN, gyro_bias_st);
		if (result == -EAGAIN)
			test_times--;
		else
			test_times = 0;
	}
	if (result)
		goto test_fail;

	gyro_result = mpu6050_gyro_self_test(client,
		gyro_bias_regular, gyro_bias_st, ratio);
	result = gyro_result;

err_state:
test_fail:
	/*recover registers */
	if (mpu6050_recover_register(client)) {
		pr_err("register recovering error\n");
		goto err_state;
	}


	return result;
}

