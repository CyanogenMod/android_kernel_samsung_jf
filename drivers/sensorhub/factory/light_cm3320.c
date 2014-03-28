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

#define	VENDOR		"CAPELLA"
#define	CHIP_ID_3320	"CM3320"
#define	CHIP_ID		"CM3323"


#if defined(CONFIG_MACH_JF_ATT) || defined(CONFIG_MACH_JF_TMO) || \
	defined(CONFIG_MACH_JF_EUR) || defined(CONFIG_MACH_JACTIVE_EUR)
#define CHIP_CM3323_REV	8
#elif defined(CONFIG_MACH_JF_SPR) || defined(CONFIG_MACH_JF_USC) || \
	defined(CONFIG_MACH_JF_VZW) || defined(CONFIG_MACH_JF_LGT) || \
	defined(CONFIG_MACH_JF_SKT) || defined(CONFIG_MACH_JF_KTT) || \
	defined(CONFIG_MACH_JF_DCM) || defined(CONFIG_MACH_JF_CRI)
#define CHIP_CM3323_REV	9
#elif defined(CONFIG_MACH_JFVE_EUR)
#define CHIP_CM3323_REV	0
#endif

/*************************************************************************/
/* factory Sysfs                                                         */
/*************************************************************************/
static ssize_t light_vendor_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s\n", VENDOR);
}

static ssize_t light_name_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
#ifdef CHIP_CM3323_REV
	struct ssp_data *data = dev_get_drvdata(dev);
	if (data->ap_rev >= CHIP_CM3323_REV)
		return sprintf(buf, "%s\n", CHIP_ID);
	else
		return sprintf(buf, "%s\n", CHIP_ID_3320);
#else
	return sprintf(buf, "%s\n", CHIP_ID_3320);
#endif
}

static ssize_t light_lux_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct ssp_data *data = dev_get_drvdata(dev);

	return sprintf(buf, "%u,%u,%u,%u\n",
		data->buf[LIGHT_SENSOR].r, data->buf[LIGHT_SENSOR].g,
		data->buf[LIGHT_SENSOR].b, data->buf[LIGHT_SENSOR].w);
}

static ssize_t light_data_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct ssp_data *data = dev_get_drvdata(dev);

	return sprintf(buf, "%u,%u,%u,%u\n",
		data->buf[LIGHT_SENSOR].r, data->buf[LIGHT_SENSOR].g,
		data->buf[LIGHT_SENSOR].b, data->buf[LIGHT_SENSOR].w);
}

static DEVICE_ATTR(vendor, S_IRUGO, light_vendor_show, NULL);
static DEVICE_ATTR(name, S_IRUGO, light_name_show, NULL);
static DEVICE_ATTR(lux, S_IRUGO, light_lux_show, NULL);
static DEVICE_ATTR(raw_data, S_IRUGO, light_data_show, NULL);

static struct device_attribute *light_attrs[] = {
	&dev_attr_vendor,
	&dev_attr_name,
	&dev_attr_lux,
	&dev_attr_raw_data,
	NULL,
};

void initialize_light_factorytest(struct ssp_data *data)
{
	sensors_register(data->light_device, data, light_attrs, "light_sensor");
}

void remove_light_factorytest(struct ssp_data *data)
{
	sensors_unregister(data->light_device, light_attrs);
}
