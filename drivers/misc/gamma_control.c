/*
 * Copyright 2013 Francisco Franco
 *           2014 Reworked for Samsung OLED, Luis Cruz
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/device.h>
#include <linux/miscdevice.h>

#define GAMMACONTROL_VERSION 2

		//     r      g      b
int v255_val[3]	= {    0,     0,     0};
int vt_val[3]	= {    0,     0,     0};
int v203_val[3]	= {    0,     0,     0};
int v151_val[3]	= {    0,     0,     0};
int v87_val[3]	= {    0,     0,     0};
int v51_val[3]	= {    0,     0,     0};
int v35_val[3]	= {    0,     0,     0};
int v23_val[3]	= {    0,     0,     0};
int v11_val[3]	= {    0,     0,     0};
int v3_val[3]	= {    0,     0,     0};
int tuner[3]	= {   60,    60,    60};

int red_tint[10] = {15, 20, 9, 9, 9, 9, 9, 9, 9, 9};
int grn_tint[10] = {15, 20, 9, 9, 9, 9, 9, 9, 9, 9};
int blu_tint[10] = {15, 20, 9, 9, 9, 9, 9, 9, 9, 9};

extern void panel_load_colors(void);

static ssize_t v255_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d %d %d\n", v255_val[0], v255_val[1], v255_val[2]);
}

static ssize_t v255_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_r, new_g, new_b;

	sscanf(buf, "%d %d %d", &new_r, &new_g, &new_b);

	if (new_r != v255_val[0] || new_g != v255_val[1] || new_b != v255_val[2]) {
		pr_debug("New v255: %d %d %d\n", new_r, new_g, new_b);
		v255_val[0] = new_r;
		v255_val[1] = new_g;
		v255_val[2] = new_b;
		panel_load_colors();
	}

	return size;
}

static ssize_t vt_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d %d %d\n", vt_val[0], vt_val[1], vt_val[2]);
}

static ssize_t vt_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_r, new_g, new_b;

	sscanf(buf, "%d %d %d", &new_r, &new_g, &new_b);

	if (new_r != vt_val[0] || new_g != vt_val[1] || new_b != vt_val[2]) {
		pr_debug("New vt: %d %d %d\n", new_r, new_g, new_b);
		vt_val[0] = new_r;
		vt_val[1] = new_g;
		vt_val[2] = new_b;
		panel_load_colors();
	}

	return size;
}

static ssize_t v203_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d %d %d\n", v203_val[0], v203_val[1], v203_val[2]);
}

static ssize_t v203_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_r, new_g, new_b;

	sscanf(buf, "%d %d %d", &new_r, &new_g, &new_b);

	if (new_r != v203_val[0] || new_g != v203_val[1] || new_b != v203_val[2]) {
		pr_debug("New v203: %d %d %d\n", new_r, new_g, new_b);
		v203_val[0] = new_r;
		v203_val[1] = new_g;
		v203_val[2] = new_b;
		panel_load_colors();
	}

	return size;
}

static ssize_t v151_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d %d %d\n", v151_val[0], v151_val[1], v151_val[2]);
}

static ssize_t v151_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_r, new_g, new_b;

	sscanf(buf, "%d %d %d", &new_r, &new_g, &new_b);

	if (new_r != v151_val[0] || new_g != v151_val[1] || new_b != v151_val[2]) {
		pr_debug("New v151: %d %d %d\n", new_r, new_g, new_b);
		v151_val[0] = new_r;
		v151_val[1] = new_g;
		v151_val[2] = new_b;
		panel_load_colors();
	}

	return size;
}

static ssize_t v87_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d %d %d\n", v87_val[0], v87_val[1], v87_val[2]);
}

static ssize_t v87_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_r, new_g, new_b;

	sscanf(buf, "%d %d %d", &new_r, &new_g, &new_b);

	if (new_r != v87_val[0] || new_g != v87_val[1] || new_b != v87_val[2]) {
		pr_debug("New v87: %d %d %d\n", new_r, new_g, new_b);
		v87_val[0] = new_r;
		v87_val[1] = new_g;
		v87_val[2] = new_b;
		panel_load_colors();
	}

	return size;
}

static ssize_t v51_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d %d %d\n", v51_val[0], v51_val[1], v51_val[2]);
}

static ssize_t v51_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_r, new_g, new_b;

	sscanf(buf, "%d %d %d", &new_r, &new_g, &new_b);

	if (new_r != v51_val[0] || new_g != v51_val[1] || new_b != v51_val[2]) {
		pr_debug("New v51: %d %d %d\n", new_r, new_g, new_b);
		v51_val[0] = new_r;
		v51_val[1] = new_g;
		v51_val[2] = new_b;
		panel_load_colors();
	}

	return size;
}

static ssize_t v35_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d %d %d\n", v35_val[0], v35_val[1], v35_val[2]);
}

static ssize_t v35_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_r, new_g, new_b;

	sscanf(buf, "%d %d %d", &new_r, &new_g, &new_b);

	if (new_r != v35_val[0] || new_g != v35_val[1] || new_b != v35_val[2]) {
		pr_debug("New v35: %d %d %d\n", new_r, new_g, new_b);
		v35_val[0] = new_r;
		v35_val[1] = new_g;
		v35_val[2] = new_b;
		panel_load_colors();
	}

	return size;
}

static ssize_t v23_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d %d %d\n", v23_val[0], v23_val[1], v23_val[2]);
}

static ssize_t v23_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_r, new_g, new_b;

	sscanf(buf, "%d %d %d", &new_r, &new_g, &new_b);

	if (new_r != v23_val[0] || new_g != v23_val[1] || new_b != v23_val[2]) {
		pr_debug("New v23: %d %d %d\n", new_r, new_g, new_b);
		v23_val[0] = new_r;
		v23_val[1] = new_g;
		v23_val[2] = new_b;
		panel_load_colors();
	}

	return size;
}

static ssize_t v11_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d %d %d\n", v11_val[0], v11_val[1], v11_val[2]);
}

static ssize_t v11_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_r, new_g, new_b;

	sscanf(buf, "%d %d %d", &new_r, &new_g, &new_b);

	if (new_r != v11_val[0] || new_g != v11_val[1] || new_b != v11_val[2]) {
		pr_debug("New v11: %d %d %d\n", new_r, new_g, new_b);
		v11_val[0] = new_r;
		v11_val[1] = new_g;
		v11_val[2] = new_b;
		panel_load_colors();
	}

	return size;
}

static ssize_t v3_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d %d %d\n", v3_val[0], v3_val[1], v3_val[2]);
}

static ssize_t v3_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_r, new_g, new_b;

	sscanf(buf, "%d %d %d", &new_r, &new_g, &new_b);

	if (new_r != v3_val[0] || new_g != v3_val[1] || new_b != v3_val[2]) {
		pr_debug("New v3: %d %d %d\n", new_r, new_g, new_b);
		v3_val[0] = new_r;
		v3_val[1] = new_g;
		v3_val[2] = new_b;
		panel_load_colors();
	}

	return size;
}

static ssize_t tuner_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d %d %d\n", tuner[0], tuner[1], tuner[2]);
}

#define calc_r_shift(n) \
	(red_tint[n] * (tuner[0] - 60) / 60)
#define calc_g_shift(n) \
	(grn_tint[n] * (tuner[1] - 60) / 60)
#define calc_b_shift(n) \
	(blu_tint[n] * (tuner[2] - 60) / 60)
static ssize_t tuner_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_r, new_g, new_b;

	sscanf(buf, "%d %d %d", &new_r, &new_g, &new_b);

	if (new_r > 120 || new_r < 0 || new_g > 120 || new_g < 0 || new_b > 120 || new_b < 0) {
		new_r = new_g = new_b = 60;
		pr_err("Master tuner out of bounds, reset!\n");
	}

	if (new_r != tuner[0]) {
		tuner[0] = new_r;

		v255_val[0] = calc_r_shift(0);
		vt_val[0] = calc_r_shift(1);
		v203_val[0] = calc_r_shift(2);
		v151_val[0] = calc_r_shift(3);
		v87_val[0] = calc_r_shift(4);
		v51_val[0] = calc_r_shift(5);
		v35_val[0] = calc_r_shift(6);
		v23_val[0] = calc_r_shift(7);
		v11_val[0] = calc_r_shift(8);
		v3_val[0] = calc_r_shift(9);
		if (new_g == tuner[1] && new_b == tuner[2])
			goto load_colors;
		if (new_g == tuner[1])
			goto blue;
	}

	if (new_g != tuner[1]) {
		tuner[1] = new_g;

		v255_val[1] = calc_g_shift(0);
		vt_val[1] = calc_g_shift(1);
		v203_val[1] = calc_g_shift(2);
		v151_val[1] = calc_g_shift(3);
		v87_val[1] = calc_g_shift(4);
		v51_val[1] = calc_g_shift(5);
		v35_val[1] = calc_g_shift(6);
		v23_val[1] = calc_g_shift(7);
		v11_val[1] = calc_g_shift(8);
		v3_val[1] = calc_g_shift(9);
		if (new_b == tuner[2])
			goto load_colors;
	}

blue:
	if (new_b != tuner[2]) {
		tuner[2] = new_b;

		v255_val[2] = calc_b_shift(0);
		vt_val[2] = calc_b_shift(1);
		v203_val[2] = calc_b_shift(2);
		v151_val[2] = calc_b_shift(3);
		v87_val[2] = calc_b_shift(4);
		v51_val[2] = calc_b_shift(5);
		v35_val[2] = calc_b_shift(6);
		v23_val[2] = calc_b_shift(7);
		v11_val[2] = calc_b_shift(8);
		v3_val[2] = calc_b_shift(9);
load_colors:
		pr_debug("New master tuner: %d %d %d\n", new_r, new_g, new_b);
		panel_load_colors();
	}

	return size;
}

static ssize_t gammacontrol_version(struct device * dev, struct device_attribute * attr, char * buf)
{
	return sprintf(buf, "%u\n", GAMMACONTROL_VERSION);
}

static DEVICE_ATTR(v255rgb, 0644, v255_show, v255_store);
static DEVICE_ATTR(vtrgb, 0644, vt_show, vt_store);
static DEVICE_ATTR(v203rgb, 0644, v203_show, v203_store);
static DEVICE_ATTR(v151rgb, 0644, v151_show, v151_store);
static DEVICE_ATTR(v87rgb, 0644, v87_show, v87_store);
static DEVICE_ATTR(v51rgb, 0644, v51_show, v51_store);
static DEVICE_ATTR(v35rgb, 0644, v35_show, v35_store);
static DEVICE_ATTR(v23rgb, 0644, v23_show, v23_store);
static DEVICE_ATTR(v11rgb, 0644, v11_show, v11_store);
static DEVICE_ATTR(v3rgb, 0644, v3_show, v3_store);
static DEVICE_ATTR(version, 0644, gammacontrol_version, NULL);
static DEVICE_ATTR(tuner, 0644, tuner_show, tuner_store);

static struct attribute *gammacontrol_attributes[] =
{
	&dev_attr_v255rgb.attr,
	&dev_attr_vtrgb.attr,
	&dev_attr_v203rgb.attr,
	&dev_attr_v151rgb.attr,
	&dev_attr_v87rgb.attr,
	&dev_attr_v51rgb.attr,
	&dev_attr_v35rgb.attr,
	&dev_attr_v23rgb.attr,
	&dev_attr_v11rgb.attr,
	&dev_attr_v3rgb.attr,
	&dev_attr_tuner.attr,
	&dev_attr_version.attr,
	NULL
};

static struct attribute_group gammacontrol_group =
{
	.attrs  = gammacontrol_attributes,
};

static struct miscdevice gammacontrol_device =
{
	.minor = MISC_DYNAMIC_MINOR,
	.name = "gammacontrol",
};

static int __init gammacontrol_init(void)
{
	int ret;

	pr_info("%s misc_register(%s)\n", __FUNCTION__, gammacontrol_device.name);

	ret = misc_register(&gammacontrol_device);

	if (ret) {
		pr_err("%s misc_register(%s) fail\n", __FUNCTION__, gammacontrol_device.name);
		return 1;
	}

	if (sysfs_create_group(&gammacontrol_device.this_device->kobj, &gammacontrol_group) < 0) {
		pr_err("%s sysfs_create_group fail\n", __FUNCTION__);
		pr_err("Failed to create sysfs group for device (%s)!\n", gammacontrol_device.name);
	}

	return 0;
}

device_initcall(gammacontrol_init);
