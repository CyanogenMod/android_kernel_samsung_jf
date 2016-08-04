/*
 * Author: Paul Reioux aka Faux123 <reioux@gmail.com>
 *
 * WCD93xx sound control module
 * Copyright 2013 Paul Reioux
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/kallsyms.h>
#include <linux/mfd/wcd9xxx/core.h>
#include <linux/mfd/wcd9xxx/wcd9310_registers.h>

#define SOUND_CONTROL_MAJOR_VERSION	3
#define SOUND_CONTROL_MINOR_VERSION	6

extern struct snd_soc_codec *fauxsound_codec_ptr;
extern int wcd9xxx_hw_revision;

static int snd_ctrl_locked = 0;
static int snd_rec_ctrl_locked = 0;

unsigned int tabla_read(struct snd_soc_codec *codec, unsigned int reg);
int tabla_write(struct snd_soc_codec *codec, unsigned int reg,
		unsigned int value);


#define REG_SZ	25
static unsigned int cached_regs[] = {6, 6, 0, 0, 0, 0, 0, 0, 0, 0,
			    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			    0, 0, 0, 0, 0 };

static unsigned int *cache_select(unsigned int reg)
{
	unsigned int *out = NULL;

        switch (reg) {
                case TABLA_A_RX_HPH_L_GAIN:
			out = &cached_regs[0];
			break;
                case TABLA_A_RX_HPH_R_GAIN:
			out = &cached_regs[1];
			break;
                case TABLA_A_CDC_RX1_VOL_CTL_B2_CTL:
			out = &cached_regs[4];
			break;
                case TABLA_A_CDC_RX2_VOL_CTL_B2_CTL:
			out = &cached_regs[5];
			break;
                case TABLA_A_CDC_RX3_VOL_CTL_B2_CTL:
			out = &cached_regs[6];
			break;
                case TABLA_A_CDC_RX4_VOL_CTL_B2_CTL:
			out = &cached_regs[7];
			break;
                case TABLA_A_CDC_RX5_VOL_CTL_B2_CTL:
			out = &cached_regs[8];
			break;
                case TABLA_A_CDC_RX6_VOL_CTL_B2_CTL:
			out = &cached_regs[9];
			break;
                case TABLA_A_CDC_RX7_VOL_CTL_B2_CTL:
			out = &cached_regs[10];
			break;
                case TABLA_A_CDC_TX1_VOL_CTL_GAIN:
			out = &cached_regs[11];
			break;
                case TABLA_A_CDC_TX2_VOL_CTL_GAIN:
			out = &cached_regs[12];
			break;
                case TABLA_A_CDC_TX3_VOL_CTL_GAIN:
			out = &cached_regs[13];
			break;
                case TABLA_A_CDC_TX4_VOL_CTL_GAIN:
			out = &cached_regs[14];
			break;
                case TABLA_A_CDC_TX5_VOL_CTL_GAIN:
			out = &cached_regs[15];
			break;
                case TABLA_A_CDC_TX6_VOL_CTL_GAIN:
			out = &cached_regs[16];
			break;
                case TABLA_A_CDC_TX7_VOL_CTL_GAIN:
			out = &cached_regs[17];
			break;
                case TABLA_A_CDC_TX8_VOL_CTL_GAIN:
			out = &cached_regs[18];
			break;
                case TABLA_A_CDC_TX9_VOL_CTL_GAIN:
			out = &cached_regs[19];
			break;
                case TABLA_A_CDC_TX10_VOL_CTL_GAIN:
			out = &cached_regs[20];
			break;
                case TABLA_A_RX_LINE_1_GAIN:
			out = &cached_regs[21];
			break;
                case TABLA_A_RX_LINE_2_GAIN:
			out = &cached_regs[22];
			break;
                case TABLA_A_RX_LINE_3_GAIN:
			out = &cached_regs[23];
			break;
                case TABLA_A_RX_LINE_4_GAIN:
			out = &cached_regs[24];
			break;
        }
	return out;
}

void snd_hax_cache_write(unsigned int reg, unsigned int value)
{
	unsigned int *tmp = cache_select(reg);

	if (tmp != NULL)
		*tmp = value;
}
EXPORT_SYMBOL(snd_hax_cache_write);

unsigned int snd_hax_cache_read(unsigned int reg)
{
	if (cache_select(reg) != NULL)
		return *cache_select(reg);
	else
		return -1;
}
EXPORT_SYMBOL(snd_hax_cache_read);

int snd_hax_reg_access(unsigned int reg)
{
	int ret = 1;

	switch (reg) {
		case TABLA_A_RX_HPH_L_GAIN:
		case TABLA_A_RX_HPH_R_GAIN:
		case TABLA_A_RX_HPH_L_STATUS:
		case TABLA_A_RX_HPH_R_STATUS:
			if (snd_ctrl_locked > 1)
				ret = 0;
			break;
		case TABLA_A_CDC_RX1_VOL_CTL_B2_CTL:
		case TABLA_A_CDC_RX2_VOL_CTL_B2_CTL:
		case TABLA_A_CDC_RX3_VOL_CTL_B2_CTL:
		case TABLA_A_CDC_RX4_VOL_CTL_B2_CTL:
		case TABLA_A_CDC_RX5_VOL_CTL_B2_CTL:
		case TABLA_A_CDC_RX6_VOL_CTL_B2_CTL:
		case TABLA_A_CDC_RX7_VOL_CTL_B2_CTL:
		case TABLA_A_RX_LINE_1_GAIN:
		case TABLA_A_RX_LINE_2_GAIN:
		case TABLA_A_RX_LINE_3_GAIN:
		case TABLA_A_RX_LINE_4_GAIN:
			if (snd_ctrl_locked > 0)
				ret = 0;
			break;
		case TABLA_A_CDC_TX1_VOL_CTL_GAIN:
		case TABLA_A_CDC_TX2_VOL_CTL_GAIN:
		case TABLA_A_CDC_TX3_VOL_CTL_GAIN:
		case TABLA_A_CDC_TX4_VOL_CTL_GAIN:
		case TABLA_A_CDC_TX5_VOL_CTL_GAIN:
		case TABLA_A_CDC_TX6_VOL_CTL_GAIN:
		case TABLA_A_CDC_TX7_VOL_CTL_GAIN:
		case TABLA_A_CDC_TX8_VOL_CTL_GAIN:
		case TABLA_A_CDC_TX9_VOL_CTL_GAIN:
		case TABLA_A_CDC_TX10_VOL_CTL_GAIN:
			if (snd_rec_ctrl_locked > 0)
				ret = 0;
			break;
		default:
			break;
	}
	return ret;
}
EXPORT_SYMBOL(snd_hax_reg_access);

static bool calc_checksum(unsigned int a, unsigned int b, unsigned int c)
{
	unsigned char chksum = 0;

	chksum = ~((a & 0xff) + (b & 0xff));

	if (chksum == (c & 0xff)) {
		return true;
	} else {
		return false;
	}
}

static ssize_t cam_mic_gain_show(struct kobject *kobj,
		struct kobj_attribute *attr, char *buf)
{
        return sprintf(buf, "%u\n",
		tabla_read(fauxsound_codec_ptr,
			TABLA_A_CDC_TX6_VOL_CTL_GAIN));

}

static ssize_t cam_mic_gain_store(struct kobject *kobj,
		struct kobj_attribute *attr, const char *buf, size_t count)
{
	unsigned int lval, chksum;

	sscanf(buf, "%u %u", &lval, &chksum);

	if (calc_checksum(lval, 0, chksum)) {
		tabla_write(fauxsound_codec_ptr,
			TABLA_A_CDC_TX6_VOL_CTL_GAIN, lval);
	}
	return count;
}

static ssize_t mic_gain_show(struct kobject *kobj,
		struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n",
		tabla_read(fauxsound_codec_ptr,
			TABLA_A_CDC_TX7_VOL_CTL_GAIN));
}

static ssize_t mic_gain_store(struct kobject *kobj,
		struct kobj_attribute *attr, const char *buf, size_t count)
{
	unsigned int lval, chksum;

	sscanf(buf, "%u %u", &lval, &chksum);

	if (calc_checksum(lval, 0, chksum)) {
		tabla_write(fauxsound_codec_ptr,
			TABLA_A_CDC_TX7_VOL_CTL_GAIN, lval);
	}
	return count;

}

static ssize_t speaker_gain_show(struct kobject *kobj,
		struct kobj_attribute *attr, char *buf)
{
        return sprintf(buf, "%u %u\n",
			tabla_read(fauxsound_codec_ptr,
				TABLA_A_CDC_RX3_VOL_CTL_B2_CTL),
			tabla_read(fauxsound_codec_ptr,
				TABLA_A_CDC_RX4_VOL_CTL_B2_CTL));

}

static ssize_t speaker_gain_store(struct kobject *kobj,
		struct kobj_attribute *attr, const char *buf, size_t count)
{
	unsigned int lval, rval, chksum;

	sscanf(buf, "%u %u %u", &lval, &rval, &chksum);

	if (calc_checksum(lval, rval, chksum)) {
		tabla_write(fauxsound_codec_ptr,
			TABLA_A_CDC_RX3_VOL_CTL_B2_CTL, lval);
		tabla_write(fauxsound_codec_ptr,
			TABLA_A_CDC_RX4_VOL_CTL_B2_CTL, rval);
	}
	return count;
}

static ssize_t headphone_gain_show(struct kobject *kobj,
		struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%u %u\n",
			tabla_read(fauxsound_codec_ptr,
				TABLA_A_CDC_RX1_VOL_CTL_B2_CTL),
			tabla_read(fauxsound_codec_ptr,
				TABLA_A_CDC_RX2_VOL_CTL_B2_CTL));
}

static ssize_t headphone_gain_store(struct kobject *kobj,
		struct kobj_attribute *attr, const char *buf, size_t count)
{
	unsigned int lval, rval, chksum;

	sscanf(buf, "%u %u %u", &lval, &rval, &chksum);

	if (calc_checksum(lval, rval, chksum)) {
		tabla_write(fauxsound_codec_ptr,
			TABLA_A_CDC_RX1_VOL_CTL_B2_CTL, lval);
		tabla_write(fauxsound_codec_ptr,
			TABLA_A_CDC_RX2_VOL_CTL_B2_CTL, rval);
	}
	return count;
}

static ssize_t headphone_pa_gain_show(struct kobject *kobj,
		struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%u %u\n",
		tabla_read(fauxsound_codec_ptr, TABLA_A_RX_HPH_L_GAIN),
		tabla_read(fauxsound_codec_ptr, TABLA_A_RX_HPH_R_GAIN));
}

static ssize_t headphone_pa_gain_store(struct kobject *kobj,
		struct kobj_attribute *attr, const char *buf, size_t count)
{
	unsigned int lval, rval, chksum;
	unsigned int gain, status;
	unsigned int out;

	sscanf(buf, "%u %u %u", &lval, &rval, &chksum);

	if (calc_checksum(lval, rval, chksum)) {
	gain = tabla_read(fauxsound_codec_ptr, TABLA_A_RX_HPH_L_GAIN);
	out = (gain & 0xf0) | lval;
	tabla_write(fauxsound_codec_ptr, TABLA_A_RX_HPH_L_GAIN, out);

	status = tabla_read(fauxsound_codec_ptr, TABLA_A_RX_HPH_L_STATUS);
	out = (status & 0x0f) | (lval << 4);
	tabla_write(fauxsound_codec_ptr, TABLA_A_RX_HPH_L_STATUS, out);

	gain = tabla_read(fauxsound_codec_ptr, TABLA_A_RX_HPH_R_GAIN);
	out = (gain & 0xf0) | rval;
	tabla_write(fauxsound_codec_ptr, TABLA_A_RX_HPH_R_GAIN, out);

	status = tabla_read(fauxsound_codec_ptr, TABLA_A_RX_HPH_R_STATUS);
	out = (status & 0x0f) | (rval << 4);
	tabla_write(fauxsound_codec_ptr, TABLA_A_RX_HPH_R_STATUS, out);
	}
	return count;
}

static unsigned int selected_reg = 0xdeadbeef;

static ssize_t sound_reg_select_store(struct kobject *kobj,
                struct kobj_attribute *attr, const char *buf, size_t count)
{
        sscanf(buf, "%u", &selected_reg);

	return count;
}

static ssize_t sound_reg_read_show(struct kobject *kobj,
                struct kobj_attribute *attr, char *buf)
{
	if (selected_reg == 0xdeadbeef)
		return -1;
	else
		return sprintf(buf, "%u\n",
			tabla_read(fauxsound_codec_ptr, selected_reg));
}

static ssize_t sound_reg_write_store(struct kobject *kobj,
                struct kobj_attribute *attr, const char *buf, size_t count)
{
        unsigned int out, chksum;

	sscanf(buf, "%u %u", &out, &chksum);
	if (calc_checksum(out, 0, chksum)) {
		if (selected_reg != 0xdeadbeef)
			tabla_write(fauxsound_codec_ptr, selected_reg, out);
	}
	return count;
}

static ssize_t sound_control_hw_revision_show (struct kobject *kobj,
		struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "hw_revision: %i\n", wcd9xxx_hw_revision);
}

static ssize_t sound_control_version_show(struct kobject *kobj,
		struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "version: %u.%u\n",
			SOUND_CONTROL_MAJOR_VERSION,
			SOUND_CONTROL_MINOR_VERSION);
}

static ssize_t sound_control_locked_store(struct kobject *kobj,
                struct kobj_attribute *attr, const char *buf, size_t count)
{
	int inp;

	sscanf(buf, "%d", &inp);

	snd_ctrl_locked = inp;

	return count;
}

static ssize_t sound_control_locked_show(struct kobject *kobj,
		struct kobj_attribute *attr, char *buf)
{
        return sprintf(buf, "%d\n", snd_ctrl_locked);
}

static ssize_t sound_control_rec_locked_store(struct kobject *kobj,
                struct kobj_attribute *attr, const char *buf, size_t count)
{
	int inp;

	sscanf(buf, "%d", &inp);

	snd_rec_ctrl_locked = inp;

	return count;
}

static ssize_t sound_control_rec_locked_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", snd_rec_ctrl_locked);
}

static struct kobj_attribute sound_reg_sel_attribute =
	__ATTR(sound_reg_sel,
		0222,
		NULL,
		sound_reg_select_store);

static struct kobj_attribute sound_reg_read_attribute =
	__ATTR(sound_reg_read,
		0444,
		sound_reg_read_show,
		NULL);

static struct kobj_attribute sound_reg_write_attribute =
	__ATTR(sound_reg_write,
		0222,
		NULL,
		sound_reg_write_store);

static struct kobj_attribute cam_mic_gain_attribute =
	__ATTR(gpl_cam_mic_gain,
		0666,
		cam_mic_gain_show,
		cam_mic_gain_store);

static struct kobj_attribute mic_gain_attribute =
	__ATTR(gpl_mic_gain,
		0666,
		mic_gain_show,
		mic_gain_store);

static struct kobj_attribute speaker_gain_attribute =
	__ATTR(gpl_speaker_gain,
		0666,
		speaker_gain_show,
		speaker_gain_store);

static struct kobj_attribute headphone_gain_attribute =
	__ATTR(gpl_headphone_gain,
		0666,
		headphone_gain_show,
		headphone_gain_store);

static struct kobj_attribute headphone_pa_gain_attribute =
	__ATTR(gpl_headphone_pa_gain,
		0666,
		headphone_pa_gain_show,
		headphone_pa_gain_store);

static struct kobj_attribute sound_control_locked_attribute =
	__ATTR(gpl_sound_control_locked,
		0666,
		sound_control_locked_show,
		sound_control_locked_store);

static struct kobj_attribute sound_control_rec_locked_attribute =
	__ATTR(gpl_sound_control_rec_locked,
		0666,
		sound_control_rec_locked_show,
		sound_control_rec_locked_store);

static struct kobj_attribute sound_control_version_attribute =
	__ATTR(gpl_sound_control_version,
		0444,
		sound_control_version_show, NULL);

static struct kobj_attribute sound_hw_revision_attribute =
	__ATTR(gpl_sound_control_hw_revision,
		0444,
		sound_control_hw_revision_show, NULL);

static struct attribute *sound_control_attrs[] =
	{
		&cam_mic_gain_attribute.attr,
		&mic_gain_attribute.attr,
		&speaker_gain_attribute.attr,
		&headphone_gain_attribute.attr,
		&headphone_pa_gain_attribute.attr,
		&sound_control_locked_attribute.attr,
		&sound_control_rec_locked_attribute.attr,
		&sound_reg_sel_attribute.attr,
		&sound_reg_read_attribute.attr,
		&sound_reg_write_attribute.attr,
		&sound_hw_revision_attribute.attr,
		&sound_control_version_attribute.attr,
		NULL,
	};

static struct attribute_group sound_control_attr_group =
	{
		.attrs = sound_control_attrs,
	};

static struct kobject *sound_control_kobj;

static int sound_control_init(void)
{
	int sysfs_result;

	sound_control_kobj =
		kobject_create_and_add("sound_control_3", kernel_kobj);

	if (!sound_control_kobj) {
		pr_err("%s sound_control_kobj create failed!\n",
			__FUNCTION__);
		return -ENOMEM;
        }

	sysfs_result = sysfs_create_group(sound_control_kobj,
			&sound_control_attr_group);

	if (sysfs_result) {
		pr_info("%s sysfs create failed!\n", __FUNCTION__);
		kobject_put(sound_control_kobj);
	}
	return sysfs_result;
}

static void sound_control_exit(void)
{
	if (sound_control_kobj != NULL)
		kobject_put(sound_control_kobj);
}

module_init(sound_control_init);
module_exit(sound_control_exit);
MODULE_LICENSE("GPL and additional rights");
MODULE_AUTHOR("Paul Reioux <reioux@gmail.com>");
MODULE_DESCRIPTION("Sound Control Module 3.x");
