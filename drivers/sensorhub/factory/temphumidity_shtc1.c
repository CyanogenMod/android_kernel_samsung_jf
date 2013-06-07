/*
 *  Copyright (C) 2012, Samsung Electronics Co. Ltd. All Rights Reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 */
#include "../ssp.h"
#include <linux/mfd/pm8xxx/pm8xxx-adc.h>

/*************************************************************************/
/* factory Sysfs                                                         */
/*************************************************************************/

#define VENDOR		"SENSIRION"
#define CHIP_ID		"SHTC1"
#define DONE_CAL	3
/*
* this defined value was requested from HW QA
* if you want to using this driver,
* you have to define MODEL_NAME
*/

#if defined(CONFIG_MACH_JF_ATT)
#define MODEL_NAME	"SGH-I337"
#elif defined(CONFIG_MACH_JF_SPR)
#define MODEL_NAME	"SPH-L720"
#elif defined(CONFIG_MACH_JF_TMO)
#define MODEL_NAME	"SGH-M919"
#elif defined(CONFIG_MACH_JF_USC)
#define MODEL_NAME	"SCH-R970"
#elif defined(CONFIG_MACH_JF_CRI)
#define MODEL_NAME	"SCH-R970C"
#elif defined(CONFIG_MACH_JF_VZW)
#define MODEL_NAME	"SCH-I545"
#elif defined(CONFIG_MACH_JF_EUR)
#define MODEL_NAME	"GT-I9505"
#elif defined(CONFIG_MACH_JACTIVE_EUR)
#define MODEL_NAME	"GT-I9295"
#elif defined(CONFIG_MACH_JF_SKT)
#define MODEL_NAME	"SHV-E300S"
#elif defined(CONFIG_MACH_JF_LGT)
#define MODEL_NAME	"SHV-E300L"
#elif defined(CONFIG_MACH_JF_KTT)
#define MODEL_NAME	"SHV-E300K"
#elif defined(CONFIG_MACH_JF_DCM)
#define MODEL_NAME	"SGH-N045"
#else
#define MODEL_NAME	"XXX-XXXX"
#endif

#define CP_THM_ADC_SAMPLING_CNT 7
#if defined(CONFIG_MACH_JF_ATT) || defined(CONFIG_MACH_JF_TMO) || \
	defined(CONFIG_MACH_JF_EUR) || defined(CONFIG_MACH_JF_SPR) || \
	defined(CONFIG_MACH_JF_USC) || defined(CONFIG_MACH_JF_VZW) || \
	defined(CONFIG_MACH_JF_LGT) || defined(CONFIG_MACH_JF_SKT) || \
	defined(CONFIG_MACH_JF_KTT) || defined(CONFIG_MACH_JF_DCM) || \
	defined(CONFIG_MACH_JF_CRI)
/* {adc, temp*10}, -20 to +70 */
static struct cp_thm_adc_table temp_table_cp[] = {
	{200, 700}, {207, 690}, {214, 680}, {221, 670}, {248, 660},
	{235, 650}, {243, 640}, {251, 630}, {259, 620}, {267, 610},
	{276, 600}, {286, 590}, {295, 580}, {304, 570}, {314, 560},
	{324, 550}, {336, 540}, {348, 530}, {360, 520}, {372, 510},
	{383, 500}, {402, 490}, {422, 480}, {441, 470}, {461, 460},
	{480, 450}, {495, 440}, {510, 430}, {526, 420}, {542, 410},
	{558, 400}, {574, 390}, {591, 380}, {607, 370}, {624, 360},
	{641, 350}, {659, 340}, {677, 330}, {696, 320}, {705, 310},
	{733, 300}, {752, 290}, {771, 280}, {790, 270}, {808, 260},
	{828, 250}, {848, 240}, {869, 230}, {890, 220}, {910, 210},
	{931, 200}, {951, 190}, {971, 180}, {992, 170}, {1012, 160},
	{1032, 150}, {1053, 140}, {1074, 130}, {1095, 120}, {1116, 110},
	{1137, 100}, {1155, 90}, {1173, 80}, {1191, 70}, {1210, 60},
	{1228, 50}, {1245, 40}, {1262, 30}, {1279, 20}, {1296, 10},
	{1313, 0},
	{1322, -10}, {1331, -20}, {1340, -30}, {1350, -40}, {1359, -50},
	{1374, -60}, {1389, -70}, {1404, -80}, {1419, -90}, {1434, -100},
	{1446, -110}, {1458, -120}, {1470, -130}, {1483, -140},{1495, -150},
	{1504, -160}, {1513, -170}, {1522, -180}, {1532, -190}, {1542, -200},
 };
#endif
static int get_cp_thm_value(struct ssp_data *data)
{
	int err = 0;
	int adc_max = 0;
	int adc_min = 0;
	int adc_total = 0;
	int i;
	struct pm8xxx_adc_chan_result result;

	for (i = 0; i < CP_THM_ADC_SAMPLING_CNT; i++) {
		mutex_lock(&data->cp_temp_adc_lock);
		err = pm8xxx_adc_mpp_config_read(
			PM8XXX_AMUX_MPP_4,
			ADC_MPP_2_AMUX6,
			&result);
		mutex_unlock(&data->cp_temp_adc_lock);
		if (err) {
			pr_err("%s : error reading mpp %d, rc = %d\n",
				__func__, PM8XXX_AMUX_MPP_4, err);
			return err;
		}

		if (i != 0) {
			if ((int)result.measurement > adc_max)
				adc_max = (int)result.measurement;
			else if ((int)result.measurement < adc_min)
				adc_min = (int)result.measurement;
		} else {
			adc_max = (int)result.measurement;
			adc_min = (int)result.measurement;
		}
		adc_total += (int)result.measurement;
	}

	return ((adc_total - adc_max - adc_min) / (CP_THM_ADC_SAMPLING_CNT - 2)) / 1000;
}

static int convert_adc_to_temp(struct ssp_data *data, unsigned int adc)
{
#if defined(CONFIG_MACH_JF_ATT) || defined(CONFIG_MACH_JF_TMO) || \
	defined(CONFIG_MACH_JF_EUR) || defined(CONFIG_MACH_JF_SPR) || \
	defined(CONFIG_MACH_JF_USC) || defined(CONFIG_MACH_JF_VZW) || \
	defined(CONFIG_MACH_JF_LGT) || defined(CONFIG_MACH_JF_SKT) || \
	defined(CONFIG_MACH_JF_KTT) || defined(CONFIG_MACH_JF_DCM) || \
	defined(CONFIG_MACH_JF_CRI)

	int low = 0;
	int high = 0;
	int mid = 0;
	u8 array_size = ARRAY_SIZE(temp_table_cp);

	if (temp_table_cp == NULL) {
		/* No voltage vs temperature table, using fake temp */
		return -990;
	}

	high = array_size - 1;

	while (low <= high) {
		mid = (low + high) / 2;
		if (temp_table_cp[mid].adc > adc)
			high = mid - 1;
		else if (temp_table_cp[mid].adc < adc)
			low = mid + 1;
		else
			break;
	}
	return temp_table_cp[mid].temperature;
#else
	/* No voltage vs temperature table, using fake temp */
	return -990;
#endif
}

static ssize_t temphumidity_vendor_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s\n", VENDOR);
}

static ssize_t temphumidity_name_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s\n", CHIP_ID);
}

static ssize_t engine_version_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct ssp_data *data = dev_get_drvdata(dev);

	pr_info("[SSP] %s - engine_ver = %s_%s\n",
		__func__, MODEL_NAME, data->comp_engine_ver);

	return sprintf(buf, "%s_%s\n",
		MODEL_NAME, data->comp_engine_ver);
}

static ssize_t engine_version_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	struct ssp_data *data = dev_get_drvdata(dev);

	kfree(data->comp_engine_ver);
	data->comp_engine_ver =
		    kzalloc(((strlen(buf)+1) * sizeof(char)), GFP_KERNEL);
	strncpy(data->comp_engine_ver, buf, strlen(buf)+1);
	pr_info("[SSP] %s - engine_ver = %s, %s\n",
		__func__, data->comp_engine_ver, buf);

	return size;
}

static ssize_t engine_version2_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct ssp_data *data = dev_get_drvdata(dev);

	pr_info("[SSP] %s - engine_ver2 = %s_%s\n",
		__func__, MODEL_NAME, data->comp_engine_ver2);

	return sprintf(buf, "%s_%s\n",
		MODEL_NAME, data->comp_engine_ver2);
}

static ssize_t engine_version2_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	struct ssp_data *data = dev_get_drvdata(dev);

	kfree(data->comp_engine_ver2);
	data->comp_engine_ver2 =
		    kzalloc(((strlen(buf)+1) * sizeof(char)), GFP_KERNEL);
	strncpy(data->comp_engine_ver2, buf, strlen(buf)+1);
	pr_info("[SSP] %s - engine_ver2 = %s, %s\n",
		__func__, data->comp_engine_ver2, buf);

	return size;
}

static ssize_t pam_adc_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct ssp_data *data = dev_get_drvdata(dev);
	int adc = 0;
	if (data->bSspShutdown == true) {
		adc = 0;
		goto exit;
	}
	if (data->ap_rev >= 5)
		adc = get_cp_thm_value(data);
	else {
		adc = 0;
		pr_info("[SSP] this rev is not support cp_thm\n");
	}
	/* pr_info("[SSP] %s cp_thm = %dmV\n", __func__, adc); */
exit:
	return sprintf(buf, "%d\n", adc);
}

static ssize_t pam_temp_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct ssp_data *data = dev_get_drvdata(dev);
	int adc, temp;

	adc = get_cp_thm_value(data);
	if (adc < 0) {
		pr_err("[SSP] %s - reading adc failed.(%d)\n", __func__, adc);
		temp = adc;
	} else
		temp = convert_adc_to_temp(data, adc);
	pr_info("[SSP] %s cp_temperature(Celsius * 10) = %d\n",
		__func__, temp);
	return sprintf(buf, "%d\n", temp);
}

static ssize_t temphumidity_crc_check(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	char chTempBuf[2] = {0, 10};
	int iDelayCnt = 0, iRet;
	struct ssp_data *data = dev_get_drvdata(dev);

	data->uFactorydata[0] = 0xff;
	data->uFactorydataReady = 0;
	iRet = send_instruction(data, FACTORY_MODE,
		TEMPHUMIDITY_CRC_FACTORY, chTempBuf, 2);

	while (!(data->uFactorydataReady &
		(1 << TEMPHUMIDITY_CRC_FACTORY))
		&& (iDelayCnt++ < 50)
		&& (iRet == SUCCESS))
		msleep(20);

	if ((iDelayCnt >= 50) || (iRet != SUCCESS)) {
		pr_err("[SSP]: %s - Temphumidity check crc Timeout!! %d\n",
			__func__, iRet);
			goto exit;
	}

	mdelay(5);

	pr_info("[SSP] : %s -Check_CRC : %d\n", __func__,
			data->uFactorydata[0]);

exit:
	if (data->uFactorydata[0] == 1)
		return sprintf(buf, "%s\n", "OK");
	else if (data->uFactorydata[0] == 2)
		return sprintf(buf, "%s\n","NG_NC");
	else
		return sprintf(buf, "%s\n","NG");
}

ssize_t temphumidity_send_accuracy(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	struct ssp_data *data = dev_get_drvdata(dev);
	u8 accuracy;

	if (kstrtou8(buf, 10, &accuracy) < 0) {
		pr_err("[SSP] %s - read buf is fail(%s)\n", __func__, buf);
		return size;
	}

	if (accuracy == DONE_CAL)
		ssp_send_cmd(data, MSG2SSP_AP_TEMPHUMIDITY_CAL_DONE);
	pr_info("[SSP] %s - accuracy = %d\n", __func__, accuracy);

	return size;
}

static DEVICE_ATTR(name, S_IRUGO, temphumidity_name_show, NULL);
static DEVICE_ATTR(vendor, S_IRUGO, temphumidity_vendor_show, NULL);
static DEVICE_ATTR(engine_ver, S_IRUGO | S_IWUSR | S_IWGRP,
	engine_version_show, engine_version_store);
static DEVICE_ATTR(engine_ver2, S_IRUGO | S_IWUSR | S_IWGRP,
	engine_version2_show, engine_version2_store);
static DEVICE_ATTR(cp_thm, S_IRUGO, pam_adc_show, NULL);
static DEVICE_ATTR(cp_temperature, S_IRUGO, pam_temp_show, NULL);
static DEVICE_ATTR(crc_check, S_IRUGO,
	temphumidity_crc_check, NULL);
static DEVICE_ATTR(send_accuracy,  S_IWUSR | S_IWGRP,
	NULL, temphumidity_send_accuracy);

static struct device_attribute *temphumidity_attrs[] = {
	&dev_attr_name,
	&dev_attr_vendor,
	&dev_attr_engine_ver,
	&dev_attr_engine_ver2,
	&dev_attr_cp_thm,
	&dev_attr_cp_temperature,
	&dev_attr_crc_check,
	&dev_attr_send_accuracy,
	NULL,
};

void initialize_temphumidity_factorytest(struct ssp_data *data)
{
	sensors_register(data->temphumidity_device,
		data, temphumidity_attrs, "temphumidity_sensor");
}

void remove_temphumidity_factorytest(struct ssp_data *data)
{
	if (data->comp_engine_ver != NULL)
		kfree(data->comp_engine_ver);
	if (data->comp_engine_ver2 != NULL)
		kfree(data->comp_engine_ver2);
	sensors_unregister(data->temphumidity_device, temphumidity_attrs);
}
