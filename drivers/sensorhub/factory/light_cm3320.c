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


/*************************************************************************/
/* factory Sysfs                                                         */
/*************************************************************************/

static unsigned int is_jf_eur = 0;

static unsigned int chip_cm3323_rev = 0;

static ssize_t light_vendor_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s\n", VENDOR);
}

static ssize_t light_name_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
if (chip_cm3323_rev > 0) {
	struct ssp_data *data = dev_get_drvdata(dev);
	if (data->ap_rev >= chip_cm3323_rev)
		return sprintf(buf, "%s\n", CHIP_ID);
	else
		return sprintf(buf, "%s\n", CHIP_ID_3320);
} else
	return sprintf(buf, "%s\n", CHIP_ID_3320);
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
	if (samsung_hardware == GT_I9505)
		is_jf_eur = true;

	if (samsung_hardware == SGH_I337
		 || samsung_hardware == SGH_M919
		 || samsung_hardware == SGH_S970
		 || samsung_hardware == SM_S975
		 || samsung_hardware == GT_I9505
		 || samsung_hardware == GT_I9295)
		chip_cm3323_rev = 8;
	else if (samsung_hardware == SPH_L720
		 	 || samsung_hardware == SCH_R970
		 	 || samsung_hardware == SCH_I545
			 || samsung_hardware == SHV_E300
		 	 || samsung_hardware == SGH_N045)
		chip_cm3323_rev = 9;

	sensors_register(data->light_device, data, light_attrs, "light_sensor");
}

void remove_light_factorytest(struct ssp_data *data)
{
	sensors_unregister(data->light_device, light_attrs);
}
