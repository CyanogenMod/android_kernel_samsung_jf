/*
 * Driver model for sensor
 *
 * Copyright (C) 2008 Samsung Electronics
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/input.h>

#ifndef __LINUX_SENSORS_CORE_H_INCLUDED
#define __LINUX_SENSORS_CORE_H_INCLUDED
extern struct device *sensors_classdev_register(char *sensors_name);
extern void sensors_classdev_unregister(struct device *dev);
#ifdef CONFIG_SENSOR_USE_SYMLINK
extern int sensors_initialize_symlink(struct input_dev *input_dev);
extern void sensors_delete_symlink(struct input_dev *input_dev);
#endif
extern struct class *sensors_class;
extern int sensors_register(struct device *dev,
	void *drvdata, struct device_attribute *attributes[], char *name);
extern void sensors_unregister(struct device *dev);

#endif	/* __LINUX_SENSORS_CORE_H_INCLUDED */
