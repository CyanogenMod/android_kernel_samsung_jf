/* Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/mfd/pm8xxx/spk.h>
#include <linux/mfd/pm8xxx/pm8921.h>
#include <linux/platform_device.h>
#ifdef CONFIG_SAMSUNG_JACK
#include <linux/sec_jack.h>
#endif/* CONFIG_SAMSUNG_JACK */
#if defined (CONFIG_DOCK_EN)
#include <linux/i2c/tsu6721.h>
#endif /* CONFIG_DOCK_EN */
#include <linux/gpio.h>
#include <linux/slab.h>
#include <sound/core.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/pcm.h>
#include <sound/jack.h>
#include <asm/mach-types.h>
#include <mach/socinfo.h>
#include <linux/mfd/pm8xxx/pm8038.h>
#include <linux/input.h>
#include "msm-pcm-routing.h"
#include "../codecs/wcd9304.h"
#include <mach/msm8930-gpio.h>
#include <asm/system_info.h>
/* 8930 machine driver */
#ifdef CONFIG_MFD_MAX77693
#include <linux/mfd/max77693-private.h>
#endif 
#define MSM8930_SPK_ON 1
#define MSM8930_SPK_OFF 0

#define BTSCO_RATE_8KHZ 8000
#define BTSCO_RATE_16KHZ 16000

#define SPK_AMP_POS	0x1
#define SPK_AMP_NEG	0x2
#if defined (CONFIG_DOCK_EN)
#define DOCK_POS	0x1
#define DOCK_NEG	0x2
#endif /* CONFIG_DOCK_EN */
#define PM8038_GPIO_BASE		NR_GPIO_IRQS
#define PM8038_GPIO_PM_TO_SYS(pm_gpio)  (pm_gpio - 1 + PM8038_GPIO_BASE)
#define SPKR_BOOST_GPIO 15
#define LEFT_SPKR_AMPL_GPIO 15
#define DEFAULT_PMIC_SPK_GAIN 0x0D
#define SITAR_EXT_CLK_RATE_REV10 12288000
#define SITAR_EXT_CLK_RATE 9600000
#define I2S_MCLK_RATE_REV10 12288000
#define I2S_MCLK_RATE 23040000
#define NO_OF_BITS_PER_SAMPLE  16

#define SITAR_MBHC_DEF_BUTTONS 8
#define SITAR_MBHC_DEF_RLOADS 5

#define GPIO_AUX_PCM_DOUT 63
#define GPIO_AUX_PCM_DIN 64
#define GPIO_AUX_PCM_SYNC 65
#define GPIO_AUX_PCM_CLK 66

#define GPIO_HS_US_EURO_SEL_GPIO 80
#define GPIO_HS_US_EURO_SEL_GPIO_SGLTE 66

#define GPIO_HS_DET 37
#define GPIO_HS_DET_SGLTE 50

#define PM8038_GPIO_BASE		NR_GPIO_IRQS
#define PM8038_GPIO_PM_TO_SYS(pm_gpio)  (pm_gpio - 1 + PM8038_GPIO_BASE)
#define MSM8930_JACK_TYPES		(SND_JACK_HEADSET | SND_JACK_OC_HPHL | \
					SND_JACK_OC_HPHR | SND_JACK_UNSUPPORTED)

#ifdef CONFIG_RADIO_USE_MI2S
#define GPIO_MI2S_WS     47
#define GPIO_MI2S_SCLK   48
#define GPIO_MI2S_DOUT3  49
#define GPIO_MI2S_DOUT2  50
#define GPIO_MI2S_DOUT1  51
#define GPIO_MI2S_DOUT0  52
#define GPIO_MI2S_MCLK   53
struct request_gpio {
	unsigned gpio_no;
	char *gpio_name;
};
static struct request_gpio mi2s_gpio[] = {
	{
		.gpio_no = GPIO_MI2S_WS,
		.gpio_name = "MI2S_WS",
	},
	{
		.gpio_no = GPIO_MI2S_SCLK,
		.gpio_name = "MI2S_SCLK",
	},
	{
		.gpio_no = GPIO_MI2S_DOUT0,
		.gpio_name = "MI2S_DOUT0",
	},
};
static struct clk *mi2s_osr_clk;
static struct clk *mi2s_bit_clk;
static atomic_t mi2s_rsc_ref;
#endif
static int msm8930_spk_control;
static int msm8930_slim_0_rx_ch = 1;
static int msm8930_slim_0_tx_ch = 1;
static int msm8930_i2s_rx_ch = 1;
static int msm8930_i2s_tx_ch = 1;
/* Not used
static int msm8930_ext_left_spk_pamp;
static u32 left_spk_pamp_gpio  =  3; GPIO_LEFT_SPK;
*/
static int msm8930_pmic_spk_gain = DEFAULT_PMIC_SPK_GAIN;
//static int us_euro_gpio;

static int msm8930_ext_spk_pamp;
#if defined(CONFIG_DOCK_EN)
static int msm8930_dock_pamp;
#endif /* CONFIG_DOCK_EN */
static struct clk *rx_osr_clk;
static struct clk *rx_bit_clk;
static struct clk *tx_osr_clk;
static struct clk *tx_bit_clk;
static int msm8930_btsco_rate = BTSCO_RATE_8KHZ;
static int msm8930_btsco_ch = 1;
static int hdmi_rate_variable;
static int msm_hdmi_rx_ch = 2;
static struct clk *codec_clk;
static int clk_users;

//static int msm8930_useuro_gpio_requested;
//static struct mutex cdc_mclk_mutex;
static int msm8930_audio_gpios_configured;
static struct snd_soc_jack hs_jack;
static struct snd_soc_jack button_jack;
static atomic_t auxpcm_rsc_ref;

static int msm8930_enable_codec_ext_clk(
		struct snd_soc_codec *codec, int enable,
		bool dapm);
//static bool msm8930_swap_gnd_mic(struct snd_soc_codec *codec);

//static u32 spkr_boost_enable_gpio = PM8038_GPIO_PM_TO_SYS(0x1);

struct pm_gpio SPKR_ON = {
	.direction      = PM_GPIO_DIR_OUT,
	.output_buffer  = PM_GPIO_OUT_BUF_CMOS,
	.output_value   = 1,
	.pull      = PM_GPIO_PULL_NO,
	.vin_sel   = PM_GPIO_VIN_S4,
	.out_strength   = PM_GPIO_STRENGTH_MED,
	.function       = PM_GPIO_FUNC_NORMAL,
};
struct pm_gpio SPKR_OFF = {
	.direction      = PM_GPIO_DIR_OUT,
	.output_buffer  = PM_GPIO_OUT_BUF_CMOS,
	.output_value   = 0,
	.pull      = PM_GPIO_PULL_NO,
	.vin_sel   = PM_GPIO_VIN_S4,
	.out_strength   = PM_GPIO_STRENGTH_MED,
	.function       = PM_GPIO_FUNC_NORMAL,
};

static struct sitar_mbhc_config mbhc_cfg = {
	.headset_jack = &hs_jack,
	.button_jack = &button_jack,
	.read_fw_bin = false,
	.calibration = NULL,
	.micbias = SITAR_MICBIAS2,
	.mclk_cb_fn = msm8930_enable_codec_ext_clk,
	.mclk_rate = SITAR_EXT_CLK_RATE,
	.gpio = 0,
	.gpio_irq = 0,
	.gpio_level_insert = 1,
	.swap_gnd_mic = NULL,
};

struct sec_audio_gpio {
	int gpio;
	char *name;
};

struct ext_amp_work {
	struct delayed_work dwork;
};

static struct ext_amp_work ext_amp_dwork;

static void external_speaker_amp_work(struct work_struct *work)
{
	pr_debug("%s :: Ext Speaker Amp enable\n", __func__);
	
	if (msm8930_ext_spk_pamp == 0)
		pr_debug("%s :: Ext Speaker Amp enable but msm8930_ext_spk_pamp is already 0\n", __func__);
	else {
#ifdef CONFIG_EXT_SPK_AMP
		gpio_direction_output(GPIO_SPK_AMP_EN, 1);
#else
		pm8xxx_spk_enable(MSM8930_SPK_ON);
#endif
		pr_debug("4 ms after turning on external Amp\n");
		usleep_range(4000, 4000);
	}
}
#if defined (CONFIG_WCD9304_CLK_9600)
static struct device mi2s_dev = {
	.init_name	= "msm-dai-q6.6",
};
#endif
static void msm8930_ext_control(struct snd_soc_codec *codec)
{
	struct snd_soc_dapm_context *dapm = &codec->dapm;

	pr_debug("%s: msm8930_spk_control = %d", __func__, msm8930_spk_control);
	if (msm8930_spk_control == MSM8930_SPK_ON) {
		snd_soc_dapm_enable_pin(dapm, "Ext Spk Left Pos");
		snd_soc_dapm_enable_pin(dapm, "Ext Spk left Neg");
	} else {
		snd_soc_dapm_disable_pin(dapm, "Ext Spk Left Pos");
		snd_soc_dapm_disable_pin(dapm, "Ext Spk Left Neg");
	}

	snd_soc_dapm_sync(dapm);
}

static int msm8930_get_spk(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	pr_debug("%s: msm8930_spk_control = %d", __func__, msm8930_spk_control);
	ucontrol->value.integer.value[0] = msm8930_spk_control;
	return 0;
}
static int msm8930_set_spk(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);

	pr_debug("%s()\n", __func__);
	if (msm8930_spk_control == ucontrol->value.integer.value[0])
		return 0;

	msm8930_spk_control = ucontrol->value.integer.value[0];
	msm8930_ext_control(codec);
	return 1;
}
void msm8930_enable_ear_micbias(bool state)
{
	int nRetVal = 0;
	struct snd_soc_jack *jack = &hs_jack;
	struct snd_soc_codec *codec;
	struct snd_soc_dapm_context *dapm;
	char *str
#ifdef CONFIG_EXT_EARMIC_BIAS
		= "Headset Mic";
#else
		= "MIC BIAS2 Internal1";
#endif

	if (jack->codec == NULL) { /* audrx_init not yet called */
		pr_err("%s codec==NULL\n", __func__);
		return;
	}
	codec = jack->codec;
	dapm = &codec->dapm;
	mutex_lock(&dapm->codec->mutex);

	if (state == 1) {
		nRetVal = snd_soc_dapm_force_enable_pin(dapm, str);
		pr_info("%s enable the codec  pin : %d with state :%d\n"
				, __func__, nRetVal, state);
	} else{
		nRetVal = snd_soc_dapm_disable_pin(dapm, str);
		pr_info("%s disable the codec  pin : %d with state :%d\n"
				, __func__, nRetVal, state);
	}
	snd_soc_dapm_sync(dapm);
	mutex_unlock(&dapm->codec->mutex);
}
EXPORT_SYMBOL(msm8930_enable_ear_micbias);
/*
static int msm8930_cfg_spkr_gpio(int gpio,
		int enable, const char *gpio_label)
{
	int ret = 0;

	pr_debug("%s: Configure %s GPIO %u",
		__func__, gpio_label, gpio);
	ret = gpio_request(gpio, gpio_label);
	if (ret)
		return ret;

	pr_debug("%s: Enable %s gpio %u\n",
		__func__, gpio_label, gpio);
	gpio_direction_output(gpio, enable);

	return ret;
}
*/
static void msm8930_ext_spk_power_amp_on(u32 spk)
{
	if (spk & (SPK_AMP_POS | SPK_AMP_NEG)) {

		if ((msm8930_ext_spk_pamp & SPK_AMP_POS) &&
			(msm8930_ext_spk_pamp & SPK_AMP_NEG)) {

			pr_debug("%s Speaker Amp already turned on. spk = 0x%08x\n",
					__func__, spk);
			return;
		}

		msm8930_ext_spk_pamp |= spk;

		if ((msm8930_ext_spk_pamp & SPK_AMP_POS) &&
			(msm8930_ext_spk_pamp & SPK_AMP_NEG)) {
			/* Delaying the amp power_on to remove the static noise
			during SPK_PA enable */
			schedule_delayed_work(
			&ext_amp_dwork.dwork,
			msecs_to_jiffies(50));
		}
	} else  {

		pr_err("%s: ERROR : Invalid External Speaker Ampl. spk = 0x%08x\n",
			__func__, spk);
		return;
	}
}

static void msm8930_ext_spk_power_amp_off(u32 spk)
{
	if (spk & (SPK_AMP_POS | SPK_AMP_NEG)) {

		if (!msm8930_ext_spk_pamp)
			return;
#ifdef CONFIG_EXT_SPK_AMP
		gpio_direction_output(GPIO_SPK_AMP_EN, 0);
#else
		pm8xxx_spk_enable(MSM8930_SPK_OFF);
#endif
		pr_debug("4 ms after turning off Speaker Amp\n");
		msm8930_ext_spk_pamp = 0;
		pr_debug("%s: slepping 4 ms after turning on external "
			" Left Speaker Ampl\n", __func__);
		usleep_range(4000, 4000);

	} else  {

		pr_err("%s: Invalid Ext Spk Amp. spk = 0x%08x\n",
				__func__, spk);
		return;
	}
}

static int msm8930_spkramp_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *k, int event)
{
	pr_debug("%s() %x\n", __func__, SND_SOC_DAPM_EVENT_ON(event));
	if (SND_SOC_DAPM_EVENT_ON(event)) {
		if (!strncmp(w->name, "Ext Spk Left Pos", 17))
			msm8930_ext_spk_power_amp_on(SPK_AMP_POS);
		else if (!strncmp(w->name, "Ext Spk Left Neg", 17))
			msm8930_ext_spk_power_amp_on(SPK_AMP_NEG);
		else {
			pr_err("%s() Invalid Speaker Widget = %s\n",
					__func__, w->name);
			return -EINVAL;
		}
	} else {
		if (!strncmp(w->name, "Ext Spk Left Pos", 17))
			msm8930_ext_spk_power_amp_off(SPK_AMP_POS);
		else if (!strncmp(w->name, "Ext Spk Left Neg", 17))
			msm8930_ext_spk_power_amp_off(SPK_AMP_NEG);
		else {
			pr_err("%s() Invalid Speaker Widget = %s\n",
					__func__, w->name);
			return -EINVAL;
		}
	}
	return 0;
}
#if defined(CONFIG_DOCK_EN)
static void msm8930_dock_control_gpio(u32 onoff)
{

	pr_info("%s: Enable dock en gpio %u, onoff %d\n",
				__func__, GPIO_VPS_AMP_EN, onoff);
#ifdef CONFIG_MFD_MAX77693	
	max77693_muic_set_audio_switch(onoff);
#else
	if (onoff) {

		gpio_direction_output(GPIO_VPS_AMP_EN, 1);
	} else {
		gpio_direction_output(GPIO_VPS_AMP_EN, 0);
	}
#endif
}

static void msm8930_dock_power_amp_on(u32 dock)
{

	if (dock & (DOCK_POS | DOCK_NEG)) {
		if ((msm8930_dock_pamp & DOCK_POS) &&
			(msm8930_dock_pamp & DOCK_NEG)) {

			pr_err("%s() External Dock Ampl already "
				"turned on. spk = 0x%08x\n", __func__, dock);
			return;
		}

		msm8930_dock_pamp |= dock;

		if ((msm8930_dock_pamp & DOCK_POS) &&
			(msm8930_dock_pamp & DOCK_NEG))
				msm8930_dock_control_gpio(1);

			pr_debug("%s: slepping 4 ms after turning on external "
				" dock Ampl\n", __func__);
			usleep_range(4000, 4000);

		} else  {

		pr_err("%s: ERROR : Invalid Dock Ampl. spk = 0x%08x\n",
			__func__, dock);
		return;
	}
}

static void msm8930_dock_power_amp_off(u32 dock)
{
	if (dock & (DOCK_POS | DOCK_NEG)) {
		if (!msm8930_dock_pamp)
			return;

		msm8930_dock_control_gpio(0);

		pr_debug("%s: slepping 4 ms after turning on external "
			" dock\n", __func__);
		usleep_range(4000, 4000);
		msm8930_dock_pamp = 0;

	} else  {

		pr_err("%s: ERROR : Invalid Dock Ampl. spk = 0x%08x\n",
			__func__, dock);
		return;
	}
}

static int msm8930_dock_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *k, int event)
{
	int jack_status = 0;
	jack_status = ((gpio_get_value_cansleep(GPIO_EAR_DET)) ^ 1);

	if (jack_status == 1) {
		if (msm8930_dock_pamp) {
			pr_info("earjack is attached, but dock gpio is alive\n");
		} else {
			pr_info("earjack is attached, it is not Dock\n");
			return 0;
		}
	}

#if defined(CONFIG_VIDEO_MHL_V2)
	if (dock_det() != 1) {
		if (msm8930_dock_pamp) {
			pr_info("dock is not attached, but dock gpio is alive\n");
		} else {
			pr_info("dock is not attached\n");
			return 0;
		}
	}
#endif

	pr_debug("%s() %x\n", __func__, SND_SOC_DAPM_EVENT_ON(event));
	if (SND_SOC_DAPM_EVENT_ON(event)) {
		if (!strncmp(w->name, "Dock Left Pos", 14))
			msm8930_dock_power_amp_on(DOCK_POS);
		else if (!strncmp(w->name, "Dock Left Neg", 14))
			msm8930_dock_power_amp_on(DOCK_NEG);
		else {
			pr_err("%s() Invalid Dock Widget = %s\n",
					__func__, w->name);
			return -EINVAL;
		}
	} else {
		if (!strncmp(w->name, "Dock Left Pos", 14))
			msm8930_dock_power_amp_off(DOCK_POS);
		else if (!strncmp(w->name, "Dock Left Neg", 14))
			msm8930_dock_power_amp_off(DOCK_NEG);
		else {
			pr_err("%s() Invalid Dock Widget = %s\n",
					__func__, w->name);
			return -EINVAL;
		}
	}
	return 0;
}
#endif /* CONFIG_DOCK_EN */
#ifdef CONFIG_EXT_SUBMIC_BIAS
static int msm8930_sub_micbias_event(struct snd_soc_dapm_widget *w,
		struct snd_kcontrol *k, int event)
{
	pr_info("%s\n", __func__);
	gpio_direction_output(GPIO_SUB_MIC_BIAS_EN,
				SND_SOC_DAPM_EVENT_ON(event));
	return 0;
}
#endif
#ifdef CONFIG_EXT_MAINMIC_BIAS
static int msm8930_main_micbias_event(struct snd_soc_dapm_widget *w,
		struct snd_kcontrol *k, int event)
{
	pr_info("%s\n", __func__);
	gpio_direction_output(GPIO_MAIN_MIC_BIAS_EN,
				SND_SOC_DAPM_EVENT_ON(event));
	return 0;
}
#endif /* CONFIG_EXT_MAINMIC_BIAS */
#ifdef CONFIG_EXT_EARMIC_BIAS
static int msm8930_ear_micbias_event(struct snd_soc_dapm_widget *w,
		struct snd_kcontrol *k, int event)
{
	pr_info("%s\n", __func__);
	gpio_direction_output(GPIO_EAR_MIC_BIAS_EN,
				SND_SOC_DAPM_EVENT_ON(event));
	return 0;
}
#endif
static int msm8930_enable_codec_ext_clk(
		struct snd_soc_codec *codec, int enable,
		bool dapm)
{
	int r = 0;
	int rtn;
	pr_debug("%s: enable = %d\n", __func__, enable);
	printk("%s Debashis6 \n",__func__);	

	printk("Audio Team System Revision:%d \n",system_rev);	

	//mutex_lock(&cdc_mclk_mutex);
	if (enable) {
		clk_users++;
		pr_debug("%s: clk_users = %d\n", __func__, clk_users);
		if (clk_users != 1)
			return 0;
			if (codec_clk) {
#if defined (CONFIG_WCD9304_CLK_9600)
			if (system_rev < CLK_REVISION) {
				rtn = clk_set_rate(codec_clk, SITAR_EXT_CLK_RATE_REV10);
				pr_debug("%s: clk_set_rate rtn = %x, rate = %d\n",
						 __func__, rtn, SITAR_EXT_CLK_RATE_REV10);
			} else {
				rtn = clk_set_rate(codec_clk, SITAR_EXT_CLK_RATE);
				pr_debug("%s: clk_set_rate rtn = %x, rate = %d\n",
						 __func__, rtn, SITAR_EXT_CLK_RATE);
			}
#else
			rtn = clk_set_rate(codec_clk, SITAR_EXT_CLK_RATE_REV10);
			pr_debug("%s: clk_set_rate rtn = %x\n", __func__, rtn);
#endif 
			rtn = clk_prepare_enable(codec_clk);
			pr_debug("%s: clk_prepare_enable rtn = %x\n", __func__, rtn);
				sitar_mclk_enable(codec, 1, dapm);
			} else {
				pr_err("%s: Error setting Sitar MCLK\n",
					__func__);
				clk_users--;
				r = -EINVAL;
			}
		}
	 else {
		pr_debug("%s: clk_users = %d\n", __func__, clk_users);
		if (clk_users == 0)
			return 0;
			clk_users--;
			pr_debug("%s: clk_users = %d\n", __func__, clk_users);
			if (clk_users == 0) {
				pr_debug("%s: disabling MCLK. clk_users = %d\n",
					 __func__, clk_users);
				sitar_mclk_enable(codec, 0, dapm);
				clk_disable_unprepare(codec_clk);
			}
		 else {
			pr_err("%s: Error releasing Sitar MCLK\n", __func__);
			r = -EINVAL;
		}
	}
	//mutex_unlock(&cdc_mclk_mutex);
	printk("%s Debashis6 \n",__func__);		
	return r;
}
/*
static bool msm8930_swap_gnd_mic(struct snd_soc_codec *codec)
{
	int value = 0;

	value = gpio_get_value_cansleep(us_euro_gpio);
	pr_debug("%s: US EURO select switch %d to %d\n", __func__, value,
			!value);
	gpio_set_value_cansleep(us_euro_gpio, !value);
	return true;
}
*/
static int msm8930_mclk_event(struct snd_soc_dapm_widget *w,
		struct snd_kcontrol *kcontrol, int event)
{
	int ret;
	pr_debug("%s: event = %d\n", __func__, event);

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
#if defined (CONFIG_WCD9304_CLK_9600)
		if (system_rev < CLK_REVISION)
#endif
			ret = gpio_request(GPIO_AUDIO_MCLK_REV10, "I2S_MCLK");
#if defined (CONFIG_WCD9304_CLK_9600)
		else
			ret = gpio_request(GPIO_AUDIO_MCLK, "I2S_MCLK");
#endif
		if (ret) {
			pr_err("%s: Failed to request gpio %d\n", __func__,
					GPIO_AUDIO_MCLK);
		}
printk("%s Debashis6 \n",__func__);		
		return msm8930_enable_codec_ext_clk(w->codec, 1, true);
	case SND_SOC_DAPM_POST_PMD:
#if defined (CONFIG_WCD9304_CLK_9600)
				if (system_rev < CLK_REVISION)
#endif
					gpio_free(GPIO_AUDIO_MCLK_REV10);
#if defined (CONFIG_WCD9304_CLK_9600)
				else
					gpio_free(GPIO_AUDIO_MCLK);
#endif

		return msm8930_enable_codec_ext_clk(w->codec, 0, true);
	}
	return 0;
}

static const struct snd_soc_dapm_widget msm8930_dapm_widgets[] = {

	SND_SOC_DAPM_SUPPLY("MCLK",  SND_SOC_NOPM, 0, 0,
	msm8930_mclk_event, SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD),

	SND_SOC_DAPM_SPK("Ext Spk Left Pos", msm8930_spkramp_event),
	SND_SOC_DAPM_SPK("Ext Spk Left Neg", msm8930_spkramp_event),
#if defined(CONFIG_DOCK_EN)
	SND_SOC_DAPM_SPK("Dock Left Pos", msm8930_dock_event),
	SND_SOC_DAPM_SPK("Dock Left Neg", msm8930_dock_event),
#endif /* CONFIG_DOCK_EN */
#ifdef CONFIG_EXT_SUBMIC_BIAS
	SND_SOC_DAPM_MIC("Sub Mic", msm8930_sub_micbias_event),
#else
	SND_SOC_DAPM_MIC("Sub Mic", NULL),
#endif
#ifdef CONFIG_EXT_MAINMIC_BIAS
	SND_SOC_DAPM_MIC("Handset Mic", msm8930_main_micbias_event),
#else
	SND_SOC_DAPM_MIC("Handset Mic", NULL),
#endif /* CONFIG_EXT_MAINMIC_BIAS*/

#ifdef CONFIG_EXT_EARMIC_BIAS
	SND_SOC_DAPM_MIC("Headset Mic", msm8930_ear_micbias_event),
#else
	SND_SOC_DAPM_MIC("Headset Mic", NULL),
#endif
	SND_SOC_DAPM_MIC("Digital Mic1", NULL),
	SND_SOC_DAPM_MIC("ANCRight Headset Mic", NULL),
	SND_SOC_DAPM_MIC("ANCLeft Headset Mic", NULL),
#if !defined(CONFIG_MACH_MELIUS) && !defined(CONFIG_MACH_LT02_CHN_CTC)
	SND_SOC_DAPM_MIC("Digital Mic1", NULL),
#endif
	SND_SOC_DAPM_MIC("Digital Mic2", NULL),
	SND_SOC_DAPM_MIC("Digital Mic3", NULL),
	SND_SOC_DAPM_MIC("Digital Mic4", NULL),

};

static const struct snd_soc_dapm_route common_audio_map[] = {

	{"RX_BIAS", NULL, "MCLK"},
	{"LDO_H", NULL, "MCLK"},

	{"MIC BIAS1 Internal1", NULL, "MCLK"},
	{"MIC BIAS2 Internal1", NULL, "MCLK"},

	/* Speaker path */
	{"Ext Spk Left Pos", NULL, "LINEOUT1"},
	{"Ext Spk Left Neg", NULL, "LINEOUT2"},
#if defined(CONFIG_DOCK_EN)
	/* Headset path */
	{"Dock Left Pos", NULL, "HPHL"},
	{"Dock Left Neg", NULL, "HPHR"},
#endif /* CONFIG_DOCK_EN */
	/* Headset Mic */
#ifdef CONFIG_EXT_EARMIC_BIAS
	{"AMIC2", NULL, "Headset Mic Bias"},
	{"Headset Mic Bias", NULL, "Headset Mic"},
#else
	{"AMIC2", NULL, "MIC BIAS2 Internal1"},
	{"MIC BIAS2 Internal1", NULL, "Headset Mic"},
#endif
	/* Microphone path */
/*	{"AMIC1", NULL, "MIC BIAS2 External"},
	{"MIC BIAS2 External", NULL, "ANCLeft Headset Mic"},
*/
#ifdef CONFIG_EXT_MAINMIC_BIAS
	{"AMIC1", NULL, "Main Mic Bias"},
	{"Main Mic Bias", NULL, "Handset Mic"},
#else
	{"AMIC1", NULL, "MIC BIAS1 Internal1"},
	{"MIC BIAS1 Internal1", NULL, "Handset Mic"},
#endif /* CONFIG_EXT_MAINMIC_BIAS */

/*
	{"AMIC3", NULL, "MIC BIAS2 External"},
	{"MIC BIAS2 External", NULL, "ANCRight Headset Mic"},
*/
#ifdef CONFIG_EXT_SUBMIC_BIAS
	{"AMIC3", NULL, "Sub Mic Bias"},
	{"Sub Mic Bias", NULL, "Sub Mic"},
#else
#if defined(CONFIG_MACH_LT02_CHN_CTC)
	{"AMIC3", NULL, "MIC BIAS2 Internal1"},
	{"MIC BIAS2 Internal1", NULL, "Sub Mic"},
#else
	{"AMIC3", NULL, "MIC BIAS1 Internal1"},
	{"MIC BIAS1 Internal1", NULL, "Sub Mic"},
#endif
#endif
	{"HEADPHONE", NULL, "LDO_H"},

	/**
	 * The digital Mic routes are setup considering
	 * fluid as default device.
	 */

	/**
	 * Digital Mic1. Front Bottom left Mic on Fluid and MTP.
	 * Digital Mic GM5 on CDP mainboard.
	 * Conncted to DMIC1 Input on Sitar codec.
	 */
	{"DMIC1", NULL, "MIC BIAS1 External"},
	{"MIC BIAS1 External", NULL, "Digital Mic1"},

	/**
	 * Digital Mic2. Back top MIC on Fluid.
	 * Digital Mic GM6 on CDP mainboard.
	 * Conncted to DMIC2 Input on Sitar codec.
	 */
	{"DMIC2", NULL, "MIC BIAS1 External"},
	{"MIC BIAS1 External", NULL, "Digital Mic2"},
	/**
	 * Digital Mic3. Back Bottom Digital Mic on Fluid.
	 * Digital Mic GM1 on CDP mainboard.
	 * Conncted to DMIC4 Input on Sitar codec.
	 */
	{"DMIC3", NULL, "MIC BIAS1 External"},
	{"MIC BIAS1 External", NULL, "Digital Mic3"},

	/**
	 * Digital Mic4. Back top Digital Mic on Fluid.
	 * Digital Mic GM2 on CDP mainboard.
	 * Conncted to DMIC3 Input on Sitar codec.
	 */
	{"DMIC4", NULL, "MIC BIAS1 External"},
	{"MIC BIAS1 External", NULL, "Digital Mic4"},


};

static const struct snd_soc_dapm_route common_audio_map_sglte[] = {

	{"RX_BIAS", NULL, "MCLK"},
	{"LDO_H", NULL, "MCLK"},

	{"MIC BIAS1 Internal1", NULL, "MCLK"},
	{"MIC BIAS2 Internal1", NULL, "MCLK"},

	/* Speaker path */
	{"Ext Spk Left Pos", NULL, "LINEOUT1"},
	{"Ext Spk Left Neg", NULL, "LINEOUT2"},

	/* Headset Mic */
	{"AMIC2", NULL, "MIC BIAS2 External"},
	{"MIC BIAS2 External", NULL, "Headset Mic"},

	/* Microphone path */
	{"AMIC1", NULL, "MIC BIAS1 External"},
	{"MIC BIAS1 External", NULL, "ANCLeft Headset Mic"},

	{"AMIC3", NULL, "MIC BIAS1 External"},
	{"MIC BIAS1 External", NULL, "ANCRight Headset Mic"},

	{"HEADPHONE", NULL, "LDO_H"},
};

static const char *spk_function[] = {"Off", "On"};
static const char *slim0_rx_ch_text[] = {"One", "Two"};
static const char *slim0_tx_ch_text[] = {"One", "Two", "Three", "Four"};
static char const *hdmi_rx_ch_text[] = {"Two", "Three", "Four", "Five",
					"Six", "Seven", "Eight"};
static const char * const hdmi_rate[] = {"Default", "Variable"};

static const struct soc_enum msm8930_enum[] = {
	SOC_ENUM_SINGLE_EXT(2, spk_function),
	SOC_ENUM_SINGLE_EXT(2, slim0_rx_ch_text),
	SOC_ENUM_SINGLE_EXT(4, slim0_tx_ch_text),
	SOC_ENUM_SINGLE_EXT(2, hdmi_rate),
	SOC_ENUM_SINGLE_EXT(7, hdmi_rx_ch_text),
};

static const char *btsco_rate_text[] = {"8000", "16000"};
static const struct soc_enum msm8930_btsco_enum[] = {
		SOC_ENUM_SINGLE_EXT(2, btsco_rate_text),
};
static int msm8930_i2s_rx_ch_get(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	pr_debug("%s: msm8930_i2s_rx_ch  = %d\n", __func__,
			msm8930_i2s_rx_ch);
	ucontrol->value.integer.value[0] = msm8930_i2s_rx_ch - 1;
	return 0;
}

static int msm8930_i2s_rx_ch_put(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	msm8930_i2s_rx_ch = ucontrol->value.integer.value[0] + 1;

	pr_debug("%s: msm8930_i2s_rx_ch = %d\n", __func__,
			msm8930_i2s_rx_ch);
	return 1;
}

static int msm8930_i2s_tx_ch_get(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	pr_debug("%s: msm8930_i2s_tx_ch  = %d\n", __func__,
			msm8930_i2s_tx_ch);
	ucontrol->value.integer.value[0] = msm8930_i2s_tx_ch - 1;
	return 0;
}

static int msm8930_i2s_tx_ch_put(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	msm8930_i2s_tx_ch = ucontrol->value.integer.value[0] + 1;

	pr_debug("%s: msm8930_i2s_tx_ch = %d\n", __func__,
			msm8930_i2s_tx_ch);
	return 1;
}

static int msm8930_slim_0_rx_ch_get(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	pr_debug("%s: msm8930_slim_0_rx_ch  = %d\n", __func__,
		msm8930_slim_0_rx_ch);
	ucontrol->value.integer.value[0] = msm8930_slim_0_rx_ch - 1;
	return 0;
}

static int msm8930_slim_0_rx_ch_put(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	msm8930_slim_0_rx_ch = ucontrol->value.integer.value[0] + 1;

	pr_debug("%s: msm8930_slim_0_rx_ch = %d\n", __func__,
		msm8930_slim_0_rx_ch);
	return 1;
}

static int msm8930_slim_0_tx_ch_get(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	pr_debug("%s: msm8930_slim_0_tx_ch  = %d\n", __func__,
		 msm8930_slim_0_tx_ch);
	ucontrol->value.integer.value[0] = msm8930_slim_0_tx_ch - 1;
	return 0;
}

static int msm8930_slim_0_tx_ch_put(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	msm8930_slim_0_tx_ch = ucontrol->value.integer.value[0] + 1;

	pr_debug("%s: msm8930_slim_0_tx_ch = %d\n", __func__,
		 msm8930_slim_0_tx_ch);
	return 1;
}

static int msm8930_btsco_rate_get(struct snd_kcontrol *kcontrol,
				struct snd_ctl_elem_value *ucontrol)
{
	pr_debug("%s: msm8930_btsco_rate  = %d", __func__, msm8930_btsco_rate);
	ucontrol->value.integer.value[0] = msm8930_btsco_rate;
	return 0;
}

static int msm8930_btsco_rate_put(struct snd_kcontrol *kcontrol,
				struct snd_ctl_elem_value *ucontrol)
{

	switch (ucontrol->value.integer.value[0]) {
	case 8000:
		msm8930_btsco_rate = BTSCO_RATE_8KHZ;
		break;
	case 16000:
		msm8930_btsco_rate = BTSCO_RATE_16KHZ;
		break;
	default:
		msm8930_btsco_rate = BTSCO_RATE_8KHZ;
		break;
	}
	pr_debug("%s: msm8930_btsco_rate = %d\n", __func__, msm8930_btsco_rate);
	return 0;
}
static const struct snd_kcontrol_new sitar_msm8930_i2s_controls[] = {
#if !defined(CONFIG_MACH_MELIUS) && !defined(CONFIG_MACH_LT02_CHN_CTC)
	SOC_ENUM_EXT("Speaker Function", msm8930_enum[0], msm8930_get_spk,
		msm8930_set_spk),
#endif
	SOC_ENUM_EXT("PRI_RX Channels", msm8930_enum[1],
		msm8930_i2s_rx_ch_get, msm8930_i2s_rx_ch_put),
	SOC_ENUM_EXT("PRI_TX Channels", msm8930_enum[2],
		msm8930_i2s_tx_ch_get, msm8930_i2s_tx_ch_put),
};

static const char *pmic_spk_gain_text[] = {
	"NEG_6_DB", "NEG_4_DB", "NEG_2_DB", "ZERO_DB", "POS_2_DB", "POS_4_DB",
	"POS_6_DB", "POS_8_DB", "POS_10_DB", "POS_12_DB", "POS_14_DB",
	"POS_16_DB", "POS_18_DB", "POS_20_DB", "POS_22_DB", "POS_24_DB"
};

static const struct soc_enum msm8960_pmic_spk_gain_enum[] = {
	SOC_ENUM_SINGLE_EXT(ARRAY_SIZE(pmic_spk_gain_text),
						pmic_spk_gain_text),
};

static int msm8930_pmic_gain_get(struct snd_kcontrol *kcontrol,
					struct snd_ctl_elem_value *ucontrol)
{
	pr_debug("%s: msm8930_pmic_spk_gain = %d\n", __func__,
			 msm8930_pmic_spk_gain);
	ucontrol->value.integer.value[0] = msm8930_pmic_spk_gain;
	return 0;
}

static int msm8930_pmic_gain_put(struct snd_kcontrol *kcontrol,
					struct snd_ctl_elem_value *ucontrol)
{
	int ret = 0;
	msm8930_pmic_spk_gain = ucontrol->value.integer.value[0];
	if (socinfo_get_pmic_model() != PMIC_MODEL_PM8917)
		ret = pm8xxx_spk_gain(msm8930_pmic_spk_gain);
	pr_debug("%s: msm8930_pmic_spk_gain = %d"
			 " ucontrol->value.integer.value[0] = %d\n", __func__,
			 msm8930_pmic_spk_gain,
			 (int) ucontrol->value.integer.value[0]);
	return ret;
}

static int msm_hdmi_rx_ch_get(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	pr_debug("%s: msm_hdmi_rx_ch  = %d\n", __func__,
		msm_hdmi_rx_ch);
	ucontrol->value.integer.value[0] = msm_hdmi_rx_ch - 2;
	return 0;
}

static int msm_hdmi_rx_ch_put(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	msm_hdmi_rx_ch = ucontrol->value.integer.value[0] + 2;

	pr_debug("%s: msm_hdmi_rx_ch = %d\n", __func__,
		msm_hdmi_rx_ch);
	return 1;
}

static int msm8930_hdmi_rate_put(struct snd_kcontrol *kcontrol,
				struct snd_ctl_elem_value *ucontrol)
{
	hdmi_rate_variable = ucontrol->value.integer.value[0];
	pr_debug("%s: hdmi_rate_variable = %d\n", __func__, hdmi_rate_variable);
	return 0;
}

static int msm8930_hdmi_rate_get(struct snd_kcontrol *kcontrol,
				struct snd_ctl_elem_value *ucontrol)
{
	ucontrol->value.integer.value[0] = hdmi_rate_variable;
	return 0;
}

static const struct snd_kcontrol_new sitar_msm8930_controls[] = {
	SOC_ENUM_EXT("Speaker Function", msm8930_enum[0], msm8930_get_spk,
		msm8930_set_spk),
	SOC_ENUM_EXT("SLIM_0_RX Channels", msm8930_enum[1],
		msm8930_slim_0_rx_ch_get, msm8930_slim_0_rx_ch_put),
	SOC_ENUM_EXT("SLIM_0_TX Channels", msm8930_enum[2],
		msm8930_slim_0_tx_ch_get, msm8930_slim_0_tx_ch_put),
	SOC_ENUM_EXT("PMIC SPK Gain", msm8960_pmic_spk_gain_enum[0],
		msm8930_pmic_gain_get, msm8930_pmic_gain_put),
	SOC_ENUM_EXT("Internal BTSCO SampleRate", msm8930_btsco_enum[0],
		msm8930_btsco_rate_get, msm8930_btsco_rate_put),
	SOC_ENUM_EXT("HDMI RX Rate", msm8930_enum[3],
					msm8930_hdmi_rate_get,
					msm8930_hdmi_rate_put),
	SOC_ENUM_EXT("HDMI_RX Channels", msm8930_enum[4],
				msm_hdmi_rx_ch_get, msm_hdmi_rx_ch_put),
};

static void *def_sitar_mbhc_cal(void)
{
	void *sitar_cal;
	struct sitar_mbhc_btn_detect_cfg *btn_cfg;
	u16 *btn_low, *btn_high;
	u8 *n_ready, *n_cic, *gain;

	sitar_cal = kzalloc(SITAR_MBHC_CAL_SIZE(SITAR_MBHC_DEF_BUTTONS,
				SITAR_MBHC_DEF_RLOADS),
				GFP_KERNEL);
	if (!sitar_cal) {
		pr_err("%s: out of memory\n", __func__);
		return NULL;
	}

#define S(X, Y) ((SITAR_MBHC_CAL_GENERAL_PTR(sitar_cal)->X) = (Y))
	S(t_ldoh, 100);
	S(t_bg_fast_settle, 100);
	S(t_shutdown_plug_rem, 255);
	S(mbhc_nsa, 4);
	S(mbhc_navg, 4);
#undef S
#define S(X, Y) ((SITAR_MBHC_CAL_PLUG_DET_PTR(sitar_cal)->X) = (Y))
	S(mic_current, SITAR_PID_MIC_5_UA);
	S(hph_current, SITAR_PID_MIC_5_UA);
	S(t_mic_pid, 100);
	S(t_ins_complete, 250);
	S(t_ins_retry, 200);
#undef S
#define S(X, Y) ((SITAR_MBHC_CAL_PLUG_TYPE_PTR(sitar_cal)->X) = (Y))
	S(v_no_mic, 30);
	S(v_hs_max, 1500);
#undef S
#define S(X, Y) ((SITAR_MBHC_CAL_BTN_DET_PTR(sitar_cal)->X) = (Y))
	S(c[0], 62);
	S(c[1], 124);
	S(nc, 1);
	S(n_meas, 3);
	S(mbhc_nsc, 11);
	S(n_btn_meas, 1);
	S(n_btn_con, 2);
	S(num_btn, SITAR_MBHC_DEF_BUTTONS);
	S(v_btn_press_delta_sta, 100);
	S(v_btn_press_delta_cic, 50);
#undef S
	btn_cfg = SITAR_MBHC_CAL_BTN_DET_PTR(sitar_cal);
	btn_low = sitar_mbhc_cal_btn_det_mp(btn_cfg, SITAR_BTN_DET_V_BTN_LOW);
	btn_high = sitar_mbhc_cal_btn_det_mp(btn_cfg, SITAR_BTN_DET_V_BTN_HIGH);
	btn_low[0] = -50;
	btn_high[0] = 10;
	btn_low[1] = 11;
	btn_high[1] = 38;
	btn_low[2] = 39;
	btn_high[2] = 64;
	btn_low[3] = 65;
	btn_high[3] = 91;
	btn_low[4] = 92;
	btn_high[4] = 115;
	btn_low[5] = 116;
	btn_high[5] = 141;
	btn_low[6] = 142;
	btn_high[6] = 163;
	btn_low[7] = 164;
	btn_high[7] = 250;
	n_ready = sitar_mbhc_cal_btn_det_mp(btn_cfg, SITAR_BTN_DET_N_READY);
	n_ready[0] = 48;
	n_ready[1] = 38;
	n_cic = sitar_mbhc_cal_btn_det_mp(btn_cfg, SITAR_BTN_DET_N_CIC);
	n_cic[0] = 60;
	n_cic[1] = 47;
	gain = sitar_mbhc_cal_btn_det_mp(btn_cfg, SITAR_BTN_DET_GAIN);
	gain[0] = 11;
	gain[1] = 9;

	return sitar_cal;
}



#ifdef CONFIG_SLIMBUS_MSM_CTRL
static int msm8930_hw_params(struct snd_pcm_substream *substream,
				struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	int ret = 0;
	unsigned int rx_ch[SLIM_MAX_RX_PORTS], tx_ch[SLIM_MAX_TX_PORTS];
	unsigned int rx_ch_cnt = 0, tx_ch_cnt = 0;

	pr_debug("%s: ch=%d\n", __func__,
					msm8930_slim_0_rx_ch);
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		ret = snd_soc_dai_get_channel_map(codec_dai,
				&tx_ch_cnt, tx_ch, &rx_ch_cnt , rx_ch);
		if (ret < 0) {
			pr_err("%s: failed to get codec chan map\n", __func__);
			goto end;
		}

		ret = snd_soc_dai_set_channel_map(cpu_dai, 0, 0,
				msm8930_slim_0_rx_ch, rx_ch);
		if (ret < 0) {
			pr_err("%s: failed to set cpu chan map\n", __func__);
			goto end;
		}
		ret = snd_soc_dai_set_channel_map(codec_dai, 0, 0,
				msm8930_slim_0_rx_ch, rx_ch);
		if (ret < 0) {
			pr_err("%s: failed to set codec channel map\n",
							       __func__);
			goto end;
		}
	} else {
		ret = snd_soc_dai_get_channel_map(codec_dai,
				&tx_ch_cnt, tx_ch, &rx_ch_cnt , rx_ch);
		if (ret < 0) {
			pr_err("%s: failed to get codec chan map\n", __func__);
			goto end;
		}
		ret = snd_soc_dai_set_channel_map(cpu_dai,
				msm8930_slim_0_tx_ch, tx_ch, 0 , 0);
		if (ret < 0) {
			pr_err("%s: failed to set cpu chan map\n", __func__);
			goto end;
		}
		ret = snd_soc_dai_set_channel_map(codec_dai,
				msm8930_slim_0_tx_ch, tx_ch, 0, 0);
		if (ret < 0) {
			pr_err("%s: failed to set codec channel map\n",
							       __func__);
			goto end;
		}

	}
end:
	return ret;
}

static int msm8930_audrx_init(struct snd_soc_pcm_runtime *rtd)
{
	int err, ret;
	struct snd_soc_codec *codec = rtd->codec;
	struct snd_soc_dapm_context *dapm = &codec->dapm;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;

	pr_debug("%s()\n", __func__);

	snd_soc_dapm_new_controls(dapm, msm8930_dapm_widgets,
				ARRAY_SIZE(msm8930_dapm_widgets));

	if (socinfo_get_platform_subtype() == PLATFORM_SUBTYPE_SGLTE)
		snd_soc_dapm_add_routes(dapm, common_audio_map_sglte,
			ARRAY_SIZE(common_audio_map_sglte));
	else
		snd_soc_dapm_add_routes(dapm, common_audio_map,
			ARRAY_SIZE(common_audio_map));

	snd_soc_dapm_enable_pin(dapm, "Ext Spk Left Pos");
	snd_soc_dapm_enable_pin(dapm, "Ext Spk Left Neg");

	snd_soc_dapm_sync(dapm);

	err = snd_soc_jack_new(codec, "Headset Jack",
		MSM8930_JACK_TYPES,
		&hs_jack);
	if (err) {
		pr_err("failed to create new jack\n");
		return err;
	}

	err = snd_soc_jack_new(codec, "Button Jack",
				SITAR_JACK_BUTTON_MASK, &button_jack);
	if (err) {
		pr_err("failed to create new jack\n");
		return err;
	}

	ret = snd_jack_set_key(button_jack.jack,
			       SND_JACK_BTN_0,
			       KEY_MEDIA);
	if (ret) {
		pr_err("%s: Failed to set code for btn-0\n", __func__);
		return err;
	}

	codec_clk = clk_get(cpu_dai->dev, "osr_clk");

	/*
	 * Switch is present only in 8930 CDP and SGLTE
	 */
	if (socinfo_get_platform_subtype() == PLATFORM_SUBTYPE_SGLTE ||
		machine_is_msm8930_cdp())
		mbhc_cfg.swap_gnd_mic = msm8930_swap_gnd_mic;

	if (socinfo_get_platform_subtype() == PLATFORM_SUBTYPE_SGLTE) {
		mbhc_cfg.gpio = GPIO_HS_DET_SGLTE;
		mbhc_cfg.gpio_level_insert = 0;
	} else
		mbhc_cfg.gpio = GPIO_HS_DET;

	/*
	 * GPIO for headset detect is present in all devices
	 * MTP/Fluid/CDP/SGLTE
	 */
	err = gpio_request(mbhc_cfg.gpio, "HEADSET_DETECT");
	if (err) {
		pr_err("%s: Failed to request gpio %d\n",
				__func__, mbhc_cfg.gpio);
		return err;
	}

	mbhc_cfg.gpio_irq = gpio_to_irq(mbhc_cfg.gpio);
	sitar_hs_detect(codec, &mbhc_cfg);

	if (socinfo_get_pmic_model() != PMIC_MODEL_PM8917) {
		/* Initialize default PMIC speaker gain */
		pm8xxx_spk_gain(DEFAULT_PMIC_SPK_GAIN);
	}

	return 0;
}

static int msm8930_slim_0_rx_be_hw_params_fixup(struct snd_soc_pcm_runtime *rtd,
			struct snd_pcm_hw_params *params)
{
	struct snd_interval *rate = hw_param_interval(params,
	SNDRV_PCM_HW_PARAM_RATE);

	struct snd_interval *channels = hw_param_interval(params,
			SNDRV_PCM_HW_PARAM_CHANNELS);

	pr_debug("%s()\n", __func__);
	rate->min = rate->max = 48000;
	channels->min = channels->max = msm8930_slim_0_rx_ch;

	return 0;
}

static int msm8930_slim_0_tx_be_hw_params_fixup(struct snd_soc_pcm_runtime *rtd,
			struct snd_pcm_hw_params *params)
{
	struct snd_interval *rate = hw_param_interval(params,
	SNDRV_PCM_HW_PARAM_RATE);

	struct snd_interval *channels = hw_param_interval(params,
			SNDRV_PCM_HW_PARAM_CHANNELS);

	pr_debug("%s()\n", __func__);
	rate->min = rate->max = 48000;
	channels->min = channels->max = msm8930_slim_0_tx_ch;

	return 0;
}
#else
static int msm8930_i2s_hw_params(struct snd_pcm_substream *substream,
			struct snd_pcm_hw_params *params)
{
	int rate = params_rate(params);
	int bit_clk_set = 0;
	int ret;
	pr_debug("%s Codec Clock %x Rx Bit Clock %x\n", __func__,
			(unsigned int)codec_clk, (unsigned int)rx_bit_clk);
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
#if defined (CONFIG_WCD9304_CLK_9600)
		if (system_rev < CLK_REVISION) {
#endif
		bit_clk_set = SITAR_EXT_CLK_RATE_REV10 / (rate * 2 *
						NO_OF_BITS_PER_SAMPLE);
#if defined (CONFIG_WCD9304_CLK_9600)
		} else {
		bit_clk_set = I2S_MCLK_RATE / (rate * 2 *
						NO_OF_BITS_PER_SAMPLE);
		}
#endif
		ret = clk_set_rate(rx_bit_clk, bit_clk_set);
		pr_debug("%s: clk_set_rate rx_bit rtn = %x, divider = %d\n",
				__func__, ret, bit_clk_set);
	} else if (substream->stream == SNDRV_PCM_STREAM_CAPTURE) {
#if defined (CONFIG_WCD9304_CLK_9600)
		if (system_rev < CLK_REVISION) {
#endif
		bit_clk_set = I2S_MCLK_RATE_REV10 / (rate * 2 *
					NO_OF_BITS_PER_SAMPLE);
#if defined (CONFIG_WCD9304_CLK_9600)
		} else {
			bit_clk_set = I2S_MCLK_RATE / (rate * 2 *
					NO_OF_BITS_PER_SAMPLE);		
		}
#endif
		ret = clk_set_rate(tx_bit_clk, bit_clk_set);
		pr_debug("%s: clk_set_rate tx_bit rtn = %x, divider = %d\n",
				__func__, ret, bit_clk_set);

	}
	return 1;
}

static int msm8930_i2s_audrx_init(struct snd_soc_pcm_runtime *rtd)
{
	int err;
	struct snd_soc_codec *codec = rtd->codec;
	struct snd_soc_dapm_context *dapm = &codec->dapm;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;

	pr_debug("%s()\n", __func__);


	printk("Audio Team System Revision:%d \n",system_rev);	

	rtd->pmdown_time = 0;

	err = snd_soc_add_codec_controls(codec, sitar_msm8930_i2s_controls,
				ARRAY_SIZE(sitar_msm8930_i2s_controls));
	if (err < 0)
		return err;

	snd_soc_dapm_new_controls(dapm, msm8930_dapm_widgets,
				ARRAY_SIZE(msm8930_dapm_widgets));

	snd_soc_dapm_add_routes(dapm, common_audio_map,
		ARRAY_SIZE(common_audio_map));

	snd_soc_dapm_enable_pin(dapm, "Ext Spk Left Pos");
	snd_soc_dapm_enable_pin(dapm, "Ext Spk Left Neg");

	snd_soc_dapm_sync(dapm);

	err = snd_soc_jack_new(codec, "Headset Jack",
		(SND_JACK_HEADSET | SND_JACK_OC_HPHL | SND_JACK_OC_HPHR),
		&hs_jack);
	if (err) {
		pr_err("failed to create new jack\n");
		return err;
	}

	err = snd_soc_jack_new(codec, "Button Jack",
				SND_JACK_BTN_0, &button_jack);
	if (err) {
		pr_err("failed to create new jack\n");
		return err;
	}
#if defined (CONFIG_WCD9304_CLK_9600)
	if (system_rev < CLK_REVISION) {
#ifdef CONFIG_MACH_MELIUS
		codec_clk = clk_get(&mi2s_dev, "osr_clk");
#else
		codec_clk = clk_get(cpu_dai->dev, "osr_clk");
#endif
		pr_debug("%s: clock name is %s codec_clk = %p",
				__func__,dev_name(cpu_dai->dev), codec_clk);
		snd_soc_dai_set_sysclk(cpu_dai, 0, SITAR_EXT_CLK_RATE_REV10, 0);
	} else {
#if defined (CONFIG_MACH_MELIUS_EUR_OPEN) || defined(CONFIG_MACH_MELIUS_EUR_LTE) \
	|| defined(CONFIG_MACH_MELIUS_SKT) || defined(CONFIG_MACH_MELIUS_KTT) \
	|| defined(CONFIG_MACH_MELIUS_LGT) \
	|| defined(CONFIG_MACH_MELIUS_ATT) || defined(CONFIG_MACH_MELIUS_TMO) \
	|| defined(CONFIG_MACH_MELIUS_VZW) || defined(CONFIG_MACH_MELIUS_SPR) \
	|| defined(CONFIG_MACH_MELIUS_MTR) \
	|| (defined(CONFIG_MACH_MELIUS_USC) && !defined(CONFIG_MACH_MELIUS_USC_00))
#if defined(CONFIG_WCD9304_USE_MI2S_CLK_9600)
		codec_clk = clk_get(&mi2s_dev, "osr_clk");
#else
		codec_clk = clk_get(cpu_dai->dev, "pcm_clk");
#endif
#else
		codec_clk = clk_get(&mi2s_dev, "osr_clk");
#endif
		pr_debug("%s: clock name is %s codec_clk = %p",
				__func__,dev_name(&mi2s_dev), codec_clk);
		snd_soc_dai_set_sysclk(cpu_dai, 0, SITAR_EXT_CLK_RATE, 0);
	}
	if (IS_ERR(codec_clk)) {
		pr_err("%s: Failed to get codec_clk\n", __func__);
		return PTR_ERR(codec_clk);
	}


#else
	codec_clk = clk_get(cpu_dai->dev, "osr_clk");
	pr_debug("%s: clock name is %s codec_clk = %p",
			__func__,dev_name(cpu_dai->dev), codec_clk);
	if (IS_ERR(codec_clk)) {
		pr_err("%s: Failed to get codec_clk\n", __func__);
		return PTR_ERR(codec_clk);
	}
#endif


/*
	mbhc_cfg.gpio = 37;
	mbhc_cfg.gpio_irq = gpio_to_irq(mbhc_cfg.gpio);
	sitar_hs_detect(codec, &mbhc_cfg);
*/
	if (socinfo_get_pmic_model() != PMIC_MODEL_PM8917) {
		/* Initialize default PMIC speaker gain */
		pm8xxx_spk_gain(DEFAULT_PMIC_SPK_GAIN);
	}

	return 0;
}

static int msm8930_i2s_rx_be_hw_params_fixup(struct snd_soc_pcm_runtime *rtd,
			struct snd_pcm_hw_params *params)
{
	struct snd_interval *rate = hw_param_interval(params,
	SNDRV_PCM_HW_PARAM_RATE);

	struct snd_interval *channels = hw_param_interval(params,
			SNDRV_PCM_HW_PARAM_CHANNELS);

	pr_debug("%s()\n", __func__);
	rate->min = rate->max = 48000;
	channels->min = channels->max = msm8930_i2s_rx_ch;

	return 0;
}

static int msm8930_i2s_tx_be_hw_params_fixup(struct snd_soc_pcm_runtime *rtd,
			struct snd_pcm_hw_params *params)
{
	struct snd_interval *rate = hw_param_interval(params,
	SNDRV_PCM_HW_PARAM_RATE);

	struct snd_interval *channels = hw_param_interval(params,
			SNDRV_PCM_HW_PARAM_CHANNELS);

	pr_debug("%s()\n", __func__);
	rate->min = rate->max = 48000;

	channels->min = channels->max = msm8930_i2s_tx_ch;

	return 0;
}
#endif

static int msm8930_be_hw_params_fixup(struct snd_soc_pcm_runtime *rtd,
			struct snd_pcm_hw_params *params)
{
	struct snd_interval *rate = hw_param_interval(params,
	SNDRV_PCM_HW_PARAM_RATE);

	pr_debug("%s()\n", __func__);
	rate->min = rate->max = 48000;

	return 0;
}

static int msm8930_hdmi_be_hw_params_fixup(struct snd_soc_pcm_runtime *rtd,
					struct snd_pcm_hw_params *params)
{
	struct snd_interval *rate = hw_param_interval(params,
					SNDRV_PCM_HW_PARAM_RATE);

	struct snd_interval *channels = hw_param_interval(params,
					SNDRV_PCM_HW_PARAM_CHANNELS);

	if (!hdmi_rate_variable)
		rate->min = rate->max = 48000;
	channels->min = channels->max = msm_hdmi_rx_ch;
	if (channels->max < 2)
		channels->min = channels->max = 2;

 	if (channels->min != channels->max)
		channels->min = channels->max;

	return 0;
}

static int msm8930_btsco_be_hw_params_fixup(struct snd_soc_pcm_runtime *rtd,
					struct snd_pcm_hw_params *params)
{
	struct snd_interval *rate = hw_param_interval(params,
					SNDRV_PCM_HW_PARAM_RATE);

	struct snd_interval *channels = hw_param_interval(params,
					SNDRV_PCM_HW_PARAM_CHANNELS);

	rate->min = rate->max = msm8930_btsco_rate;
	channels->min = channels->max = msm8930_btsco_ch;

	return 0;
}

static int msm8930_auxpcm_be_params_fixup(struct snd_soc_pcm_runtime *rtd,
				struct snd_pcm_hw_params *params)
{
	struct snd_interval *rate = hw_param_interval(params,
					 SNDRV_PCM_HW_PARAM_RATE);

	struct snd_interval *channels = hw_param_interval(params,
					SNDRV_PCM_HW_PARAM_CHANNELS);

	/* PCM only supports mono output with 8khz sample rate */
	rate->min = rate->max = 8000;
	channels->min = channels->max = 1;

	return 0;
}

static int msm8930_proxy_be_hw_params_fixup(struct snd_soc_pcm_runtime *rtd,
			struct snd_pcm_hw_params *params)
{
	struct snd_interval *rate = hw_param_interval(params,
	SNDRV_PCM_HW_PARAM_RATE);

	pr_debug("%s()\n", __func__);
	rate->min = rate->max = 48000;

	return 0;
}

static int msm8930_aux_pcm_get_gpios(void)
{
	int ret = 0;

	pr_debug("%s\n", __func__);

	ret = gpio_request(GPIO_AUX_PCM_DOUT, "AUX PCM DOUT");
	if (ret < 0) {
		pr_err("%s: Failed to request gpio(%d): AUX PCM DOUT",
				__func__, GPIO_AUX_PCM_DOUT);

		goto fail_dout;
	}

	ret = gpio_request(GPIO_AUX_PCM_DIN, "AUX PCM DIN");
	if (ret < 0) {
		pr_err("%s: Failed to request gpio(%d): AUX PCM DIN",
				 __func__, GPIO_AUX_PCM_DIN);
		goto fail_din;
	}

	ret = gpio_request(GPIO_AUX_PCM_SYNC, "AUX PCM SYNC");
	if (ret < 0) {
		pr_err("%s: Failed to request gpio(%d): AUX PCM SYNC",
				__func__, GPIO_AUX_PCM_SYNC);
		goto fail_sync;
	}

	ret = gpio_request(GPIO_AUX_PCM_CLK, "AUX PCM CLK");
	if (ret < 0) {
		pr_err("%s: Failed to request gpio(%d): AUX PCM CLK",
				 __func__, GPIO_AUX_PCM_CLK);
		goto fail_clk;
	}

	return 0;

fail_clk:
	gpio_free(GPIO_AUX_PCM_SYNC);
fail_sync:
	gpio_free(GPIO_AUX_PCM_DIN);
fail_din:
	gpio_free(GPIO_AUX_PCM_DOUT);
fail_dout:

	return ret;
}

static int msm8930_aux_pcm_free_gpios(void)
{
	gpio_free(GPIO_AUX_PCM_DIN);
	gpio_free(GPIO_AUX_PCM_DOUT);
	gpio_free(GPIO_AUX_PCM_SYNC);
	gpio_free(GPIO_AUX_PCM_CLK);

	return 0;
}
/*
static int msm8930_startup(struct snd_pcm_substream *substream)
{
	pr_debug("%s(): substream = %s  stream = %d\n", __func__,
		 substream->name, substream->stream);
	return 0;
}
*/
#ifdef CONFIG_RADIO_USE_MI2S
static int msm8930_mi2s_free_gpios(void)
{
	int	i;
	for (i = 0; i < ARRAY_SIZE(mi2s_gpio); i++)
		gpio_free(mi2s_gpio[i].gpio_no);
	return 0;
}
static int msm8930_mi2s_hw_params(struct snd_pcm_substream *substream,
			struct snd_pcm_hw_params *params)
{
	int rate = params_rate(params);
	int bit_clk_set = 0;
	if (system_rev < CLK_REVISION) {
	bit_clk_set = 12288000/(rate * 2 * 16);
	}
	else{
		bit_clk_set = 23040000/(rate * 2 * 16);
	}
	clk_set_rate(mi2s_bit_clk, bit_clk_set);
	return 1;
}
static void msm8930_mi2s_shutdown(struct snd_pcm_substream *substream)
{
	if (atomic_dec_return(&mi2s_rsc_ref) == 0) {
		pr_info("%s: free mi2s resources\n", __func__);
		if (mi2s_bit_clk) {
			clk_disable_unprepare(mi2s_bit_clk);
			clk_put(mi2s_bit_clk);
			mi2s_bit_clk = NULL;
		}
		if (mi2s_osr_clk) {
			clk_disable_unprepare(mi2s_osr_clk);
			clk_put(mi2s_osr_clk);
			mi2s_osr_clk = NULL;
		}
		msm8930_mi2s_free_gpios();
	}
}
static int configure_mi2s_gpio(void)
{
	int	rtn;
	int	i;
	int	j;
	for (i = 0; i < ARRAY_SIZE(mi2s_gpio); i++) {
		rtn = gpio_request(mi2s_gpio[i].gpio_no,
						   mi2s_gpio[i].gpio_name);
		printk("%s: gpio = %d, gpio name = %s, rtn = %d\n",
				 __func__,
				 mi2s_gpio[i].gpio_no,
				 mi2s_gpio[i].gpio_name,
				 rtn);
		if (rtn) {
			pr_err("%s: Failed to request gpio %d\n",
				   __func__,
				   mi2s_gpio[i].gpio_no);
			for (j = i; j >= 0; j--)
				gpio_free(mi2s_gpio[j].gpio_no);
			goto err;
		}
	}
err:
	return rtn;
}
static int msm8930_mi2s_startup(struct snd_pcm_substream *substream)
{
	int ret = 0;
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	printk("%s: dai name %s %p\n", __func__, cpu_dai->name, cpu_dai->dev);
	if (atomic_inc_return(&mi2s_rsc_ref) == 1) {
		pr_info("%s: acquire mi2s resources\n", __func__);
		configure_mi2s_gpio();
		mi2s_osr_clk = clk_get(cpu_dai->dev, "osr_clk");
		if (!IS_ERR(mi2s_osr_clk)) {
			if (system_rev < CLK_REVISION) {
			clk_set_rate(mi2s_osr_clk, 12288000);
			}
			else{
				clk_set_rate(mi2s_osr_clk, 23040000);
			}	
			clk_prepare_enable(mi2s_osr_clk);
		} else 
			pr_err("Failed to get mi2s_osr_clk\n");
		mi2s_bit_clk = clk_get(cpu_dai->dev, "bit_clk");
		if (IS_ERR(mi2s_bit_clk)) {
			pr_err("Failed to get mi2s_bit_clk\n");
			clk_disable_unprepare(mi2s_osr_clk);
			clk_put(mi2s_osr_clk);
			return PTR_ERR(mi2s_bit_clk);
		}
		if (system_rev < CLK_REVISION) {
		clk_set_rate(mi2s_bit_clk, 8);
		}
		else{
			clk_set_rate(mi2s_bit_clk, 15);
		}
		ret = clk_prepare_enable(mi2s_bit_clk);
		if (ret != 0) {
			pr_err("Unable to enable mi2s_rx_bit_clk\n");
			clk_put(mi2s_bit_clk);
			clk_disable_unprepare(mi2s_osr_clk);
			clk_put(mi2s_osr_clk);
			return ret;
		}
		ret = snd_soc_dai_set_fmt(cpu_dai, SND_SOC_DAIFMT_CBS_CFS);
		if (ret < 0)
			ret = snd_soc_dai_set_fmt(codec_dai,
						  SND_SOC_DAIFMT_CBS_CFS);
		if (ret < 0)
			pr_err("set format for codec dai failed\n");
	}
	return ret;
}
#endif

static int msm8930_auxpcm_startup(struct snd_pcm_substream *substream)
{
	int ret = 0;

	pr_debug("%s(): substream = %s, auxpcm_rsc_ref counter = %d\n",
		__func__, substream->name, atomic_read(&auxpcm_rsc_ref));
	if (atomic_inc_return(&auxpcm_rsc_ref) == 1)
		ret = msm8930_aux_pcm_get_gpios();
	if (ret < 0) {
		pr_err("%s: Aux PCM GPIO request failed\n", __func__);
		return -EINVAL;
	}
	return 0;

}

static void msm8930_auxpcm_shutdown(struct snd_pcm_substream *substream)
{
	pr_debug("%s(): substream = %s, auxpcm_rsc_ref counter = %d\n",
		__func__, substream->name, atomic_read(&auxpcm_rsc_ref));
	if (atomic_dec_return(&auxpcm_rsc_ref) == 0)
		msm8930_aux_pcm_free_gpios();
}


int configure_i2s_rx_gpio(void)
{
	u8 ret = 0;

	ret = gpio_request(GPIO_SPKR_I2S_RX_SCK, "I2S_RX_SCK");
	if (ret) {
		pr_err("%s: Failed to request gpio %d\n", __func__,
				GPIO_SPKR_I2S_RX_SCK);
		goto err;
	}
	ret = gpio_request(GPIO_SPKR_I2S_RX_DOUT, "I2S_RX_DOUT");
	if (ret) {
		pr_err("%s: Failed to request gpio %d\n", __func__,
				GPIO_SPKR_I2S_RX_DOUT);
		goto err;
	}
	ret = gpio_request(GPIO_SPKR_I2S_RX_WS, "I2S_RX_WS");
	if (ret) {
		pr_err("%s: Failed to request gpio %d\n", __func__,
			GPIO_SPKR_I2S_RX_WS);
		goto err;
	}
err:
	return ret;
}

int configure_i2s_tx_gpio(void)
{
	u8 ret = 0;

	ret = gpio_request(GPIO_SPKR_I2S_TX_SCK, "I2S_TX_SCK");
	if (ret) {
		pr_err("%s: Failed to request gpio %d\n", __func__,
				GPIO_SPKR_I2S_TX_SCK);
		goto err;
	}
	ret = gpio_request(GPIO_SPKR_I2S_TX_DIN, "I2S_TX_DIN");
	if (ret) {
		pr_err("%s: Failed to request gpio %d\n", __func__,
				GPIO_SPKR_I2S_TX_DIN);
		goto err;
	}
	ret = gpio_request(GPIO_SPKR_I2S_TX_WS, "I2S_TX_WS");
	if (ret) {
		pr_err("%s: Failed to request gpio %d\n", __func__,
			GPIO_SPKR_I2S_TX_WS);
		goto err;
	}
err:
	return ret;
}
#ifdef CONFIG_RADIO_USE_MI2S
static struct snd_soc_ops msm8930_mi2s_be_ops = {
	.startup = msm8930_mi2s_startup,
	.shutdown = msm8930_mi2s_shutdown,
	.hw_params = msm8930_mi2s_hw_params,
};
#endif
#ifdef CONFIG_SLIMBUS_MSM_CTRL
static int msm8930_startup(struct snd_pcm_substream *substream)
{
	pr_debug("%s(): substream = %s  stream = %d\n", __func__,
		 substream->name, substream->stream);
	return 0;
}


static void msm8930_shutdown(struct snd_pcm_substream *substream)
{
	pr_debug("%s(): substream = %s  stream = %d\n", __func__,
		 substream->name, substream->stream);
}

#else

static int msm8930_cdc_i2s_rx_free_gpios(void)
{
	gpio_free(GPIO_SPKR_I2S_RX_SCK);
	gpio_free(GPIO_SPKR_I2S_RX_DOUT);
	gpio_free(GPIO_SPKR_I2S_RX_WS);
	return 0;
}

static int msm8930_cdc_i2s_tx_free_gpios(void)
{
	gpio_free(GPIO_SPKR_I2S_TX_SCK);
	gpio_free(GPIO_SPKR_I2S_TX_DIN);
	gpio_free(GPIO_SPKR_I2S_TX_WS);
	return 0;
}

static int msm8930_i2s_startup(struct snd_pcm_substream *substream)
{
	int ret = 0;
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;

	pr_debug("%s\n", __func__);
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		configure_i2s_rx_gpio();
		rx_osr_clk = clk_get(cpu_dai->dev, "osr_clk");
		pr_debug("%s: rx_osr_clk name is %s\n",
				__func__, dev_name(cpu_dai->dev));
		if (IS_ERR(rx_osr_clk)) {
			pr_err("%s Error getting rx_osr_clk %x\n", __func__,
				   (unsigned int)rx_osr_clk);
			return PTR_ERR(rx_osr_clk);
		}
#if defined (CONFIG_WCD9304_CLK_9600)
		if (system_rev < CLK_REVISION) {
			ret = clk_set_rate(rx_osr_clk, SITAR_EXT_CLK_RATE_REV10);
			pr_debug("%s: clk_set_rate rx_osr rtn = %x, rate = %d\n",
					__func__, ret, SITAR_EXT_CLK_RATE_REV10);
		} else {
			ret = clk_set_rate(rx_osr_clk, I2S_MCLK_RATE);
			pr_debug("%s: clk_set_rate rx_osr rtn = %x, rate = %d\n",
					__func__, ret, I2S_MCLK_RATE);
		}
#else
		ret = clk_set_rate(rx_osr_clk, SITAR_EXT_CLK_RATE_REV10);
		pr_debug("%s: clk_set_rate rx_osr rtn = %x, rate = %d\n",
			__func__, ret, SITAR_EXT_CLK_RATE_REV10);
#endif 
		ret = clk_prepare_enable(rx_osr_clk);
		pr_info("%s: clk_prepare_enable rx_osr rtn = %x\n",__func__, ret);
		rx_bit_clk = clk_get(cpu_dai->dev, "bit_clk");
		if (IS_ERR(rx_bit_clk)) {
			pr_err("Failed to get rx_bit_clk\n");
			clk_disable_unprepare(rx_osr_clk);
			clk_put(rx_osr_clk);
			return PTR_ERR(rx_bit_clk);
		}
#if defined (CONFIG_WCD9304_CLK_9600)
		if (system_rev < CLK_REVISION) {
			ret = clk_set_rate(rx_bit_clk, 8);
			pr_debug("%s: clk_set_rate rx_bit rtn = %x, divider = 8\n",
					__func__, ret);
		} else {
			ret = clk_set_rate(rx_bit_clk, 15);
			pr_debug("%s: clk_set_rate rx_bit rtn = %x, divider = 15\n",
					__func__, ret);
		}
#else
		ret = clk_set_rate(rx_bit_clk, 8);
		pr_debug("%s: clk_set_rate rx_bit rtn = %x, divider = 8\n",
				__func__, ret);
#endif
		ret = clk_prepare_enable(rx_bit_clk);
		pr_debug("%s: clk_prepare_enable rx_bit rtn = %x\n",__func__, ret);
		if (ret != 0) {
			pr_err("Unable to enable rx_bit_clk\n");
			clk_put(rx_bit_clk);
			clk_disable_unprepare(rx_osr_clk);
			clk_put(rx_osr_clk);
			return ret;
		}
		ret = snd_soc_dai_set_fmt(cpu_dai, SND_SOC_DAIFMT_CBS_CFS);
		if (ret < 0)
			pr_err("set format for cpu dai failed\n");
		ret = snd_soc_dai_set_fmt(codec_dai, SND_SOC_DAIFMT_CBS_CFS);
		if (ret < 0)
			pr_err("set format for codec dai failed\n");

	} else if (substream->stream == SNDRV_PCM_STREAM_CAPTURE) {
		configure_i2s_tx_gpio();

		tx_osr_clk = clk_get(cpu_dai->dev, "osr_clk");
		pr_debug("%s: tx_osr_clk name is %s\n",
				__func__, dev_name(cpu_dai->dev));
		if (IS_ERR(tx_osr_clk)) {
			pr_err("%s Error getting tx_osr_clk %x\n", __func__,
				   (unsigned int)tx_osr_clk);
			return PTR_ERR(tx_osr_clk);
		}
#if defined (CONFIG_WCD9304_CLK_9600)
		if (system_rev < CLK_REVISION) {
			ret = clk_set_rate(tx_osr_clk, SITAR_EXT_CLK_RATE_REV10);
			pr_debug("%s: clk_set_rate tx_osr rtn = %x, rate = %d\n",
					__func__, ret, SITAR_EXT_CLK_RATE_REV10);
		} else {
			ret = clk_set_rate(tx_osr_clk, I2S_MCLK_RATE);
			pr_debug("%s: clk_set_rate tx_osr rtn = %x, rate = %d\n",
					__func__, ret, I2S_MCLK_RATE);
		}
#else
		ret = clk_set_rate(tx_osr_clk, SITAR_EXT_CLK_RATE_REV10);
		pr_debug("%s: clk_set_rate tx_osr rtn = %x, rate = %d\n",
			__func__, ret, SITAR_EXT_CLK_RATE_REV10);
#endif 
		ret = clk_prepare_enable(tx_osr_clk);
		pr_debug("%s: clk_prepare_enable tx_osr rtn = %x\n",__func__, ret);

		tx_bit_clk = clk_get(cpu_dai->dev, "bit_clk");
		if (IS_ERR(tx_bit_clk)) {
			pr_err("Failed to get tx_bit_clk\n");
			clk_disable_unprepare(tx_osr_clk);
			clk_put(tx_osr_clk);
			return PTR_ERR(tx_bit_clk);
		}
#if defined (CONFIG_WCD9304_CLK_9600)
		if (system_rev < CLK_REVISION){
			ret = clk_set_rate(tx_bit_clk, 8);
			pr_debug("%s: clk_set_rate tx_bit rtn = %x, divider = 8\n",
					__func__, ret);
		} else {
			ret = clk_set_rate(tx_bit_clk, 15);
			pr_debug("%s: clk_set_rate tx_bit rtn = %x, divider = 15\n",
					__func__, ret);
		}
#else
		ret = clk_set_rate(tx_bit_clk, 8);
		pr_debug("%s: clk_set_rate tx_bit rtn = %x, divider = 8\n",
				__func__, ret);
#endif
		ret = clk_prepare_enable(tx_bit_clk);
		pr_debug("%s: clk_prepare_enable tx_bit rtn = %x\n",__func__, ret);
		if (ret != 0) {
			pr_err("Unable to enable tx_bit_clk\n");
			clk_put(tx_bit_clk);
			clk_disable_unprepare(tx_osr_clk);
			clk_put(tx_osr_clk);
			return ret;
		}
		ret = snd_soc_dai_set_fmt(cpu_dai, SND_SOC_DAIFMT_CBS_CFS);
		if (ret < 0)
			pr_err("set format for cpu dai failed\n");
		ret = snd_soc_dai_set_fmt(codec_dai, SND_SOC_DAIFMT_CBS_CFS);
		if (ret < 0)
			pr_err("set format for codec dai failed\n");

	}
printk("%s Debashis5 \n",__func__);	
	return ret;
}

static void msm8930_i2s_shutdown(struct snd_pcm_substream *substream)
{
	pr_debug("%s Codec Clock %x Rx Bit Clock %x\n", __func__,
			(unsigned int)codec_clk, (unsigned int)rx_bit_clk);
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		if (rx_bit_clk) {
			clk_disable(rx_bit_clk);
			clk_unprepare(rx_bit_clk);
			clk_put(rx_bit_clk);
			rx_bit_clk = NULL;
		}
		if (rx_osr_clk) {
			clk_disable(rx_osr_clk);
			clk_unprepare(rx_osr_clk);
			clk_put(rx_osr_clk);
			rx_osr_clk = NULL;
		}
		msm8930_cdc_i2s_rx_free_gpios();
	} else if (substream->stream == SNDRV_PCM_STREAM_CAPTURE) {
		if (tx_bit_clk) {
			clk_disable(tx_bit_clk);
			clk_unprepare(tx_bit_clk);
			clk_put(tx_bit_clk);
			tx_bit_clk = NULL;
		}
		if (tx_osr_clk) {
			clk_disable(tx_osr_clk);
			clk_unprepare(tx_osr_clk);
			clk_put(tx_osr_clk);
			tx_osr_clk = NULL;
		}
		msm8930_cdc_i2s_tx_free_gpios();
	}
}
#endif

#ifdef CONFIG_SLIMBUS_MSM_CTRL
static struct snd_soc_ops msm8930_be_ops = {
	.startup = msm8930_startup,
	.hw_params = msm8930_hw_params,
	.shutdown = msm8930_shutdown,
};
#else
static struct snd_soc_ops msm8930_i2s_be_ops = {
	.startup = msm8930_i2s_startup,
	.shutdown = msm8930_i2s_shutdown,
	.hw_params = msm8930_i2s_hw_params,
};
#endif

static struct snd_soc_ops msm8930_auxpcm_be_ops = {
	.startup = msm8930_auxpcm_startup,
	.shutdown = msm8930_auxpcm_shutdown,
};


static struct snd_soc_dai_link *msm8930_dai_list;

static struct snd_soc_dai_link msm8930_i2s_be_dai[] = {
	{
		.name = LPASS_BE_PRI_I2S_RX,
		.stream_name = "Primary I2S Playback",
		.cpu_dai_name = "msm-dai-q6.0",
		.platform_name = "msm-pcm-routing",
		.codec_name     = "sitar_codec",
		.codec_dai_name	= "sitar_i2s_rx1",
		.no_pcm = 1,
		.be_id = MSM_BACKEND_DAI_PRI_I2S_RX,
		.init = &msm8930_i2s_audrx_init,
		.be_hw_params_fixup = msm8930_i2s_rx_be_hw_params_fixup,
		.ops = &msm8930_i2s_be_ops,
	},
	{
		.name = LPASS_BE_PRI_I2S_TX,
		.stream_name = "Primary I2S Capture",
		.cpu_dai_name = "msm-dai-q6.1",
		.platform_name = "msm-pcm-routing",
		.codec_name     = "sitar_codec",
		.codec_dai_name	= "sitar_i2s_tx1",
		.no_pcm = 1,
		.be_id = MSM_BACKEND_DAI_PRI_I2S_TX,
		.be_hw_params_fixup = msm8930_i2s_tx_be_hw_params_fixup,
		.ops = &msm8930_i2s_be_ops,
	},
};


/* Digital audio interface glue - connects codec <---> CPU */
static struct snd_soc_dai_link msm8930_dai[] = {
	/* FrontEnd DAI Links */
	{
		.name = "MSM8930 Media1",
		.stream_name = "MultiMedia1",
		.cpu_dai_name	= "MultiMedia1",
		.platform_name  = "msm-pcm-dsp",
		.dynamic = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.trigger = {SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1, /* this dainlink has playback support */
		.be_id = MSM_FRONTEND_DAI_MULTIMEDIA1
	},
	{
		.name = "MSM8930 Media2",
		.stream_name = "MultiMedia2",
		.cpu_dai_name	= "MultiMedia2",
		.platform_name  = "msm-multi-ch-pcm-dsp",
		.dynamic = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.trigger = {SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1, /* this dainlink has playback support */
		.be_id = MSM_FRONTEND_DAI_MULTIMEDIA2,
	},
	{
		.name = "Circuit-Switch Voice",
		.stream_name = "CS-Voice",
		.cpu_dai_name   = "CS-VOICE",
		.platform_name  = "msm-pcm-voice",
		.dynamic = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.trigger = {SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.be_id = MSM_FRONTEND_DAI_CS_VOICE,
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1, /* this dainlink has playback support */
	},
	{
		.name = "MSM VoIP",
		.stream_name = "VoIP",
		.cpu_dai_name	= "VoIP",
		.platform_name  = "msm-voip-dsp",
		.dynamic = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.trigger = {SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1, /* this dainlink has playback support */
		.be_id = MSM_FRONTEND_DAI_VOIP,
	},
	{
		.name = "MSM8930 LPA",
		.stream_name = "LPA",
		.cpu_dai_name	= "MultiMedia3",
		.platform_name  = "msm-pcm-lpa",
		.dynamic = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.trigger = {SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1, /* this dainlink has playback support */
		.be_id = MSM_FRONTEND_DAI_MULTIMEDIA3,
	},
	/* Hostless PMC purpose */
	{
		.name = "SLIMBUS_0 Hostless",
		.stream_name = "SLIMBUS_0 Hostless",
		.cpu_dai_name	= "SLIMBUS0_HOSTLESS",
		.platform_name  = "msm-pcm-hostless",
		.dynamic = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.trigger = {SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1, /* this dainlink has playback support */
		/* .be_id = do not care */
	},
#ifdef CONFIG_RADIO_USE_MI2S
	{
		.name = "MI2S_TX_HOSTLESS",
		.stream_name = "MI2S_TX_HOSTLESS",
		.cpu_dai_name	= "MI2S_TX_HOSTLESS",
		.platform_name  = "msm-pcm-hostless",
		.dynamic = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			SND_SOC_DPCM_TRIGGER_POST},
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1, /* this dainlink has playback support */
	},
#else
	{
		.name = "INT_FM Hostless",
		.stream_name = "INT_FM Hostless",
		.cpu_dai_name	= "INT_FM_HOSTLESS",
		.platform_name  = "msm-pcm-hostless",
		.dynamic = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.trigger = {SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1, /* this dainlink has playback support */
		/* .be_id = do not care */
	},
#endif
	{
		.name = "MSM AFE-PCM RX",
		.stream_name = "AFE-PROXY RX",
		.cpu_dai_name = "msm-dai-q6.241",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.platform_name  = "msm-pcm-afe",
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1, /* this dainlink has playback support */
	},
	{
		.name = "MSM AFE-PCM TX",
		.stream_name = "AFE-PROXY TX",
		.cpu_dai_name = "msm-dai-q6.240",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.platform_name  = "msm-pcm-afe",
		.ignore_suspend = 1,
	},
	{
		.name = "MSM8930 Compr",
		.stream_name = "COMPR",
		.cpu_dai_name	= "MultiMedia4",
		.platform_name  = "msm-compr-dsp",
		.dynamic = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1, /* this dainlink has playback support */
		.be_id = MSM_FRONTEND_DAI_MULTIMEDIA4,
	},
	 {
		.name = "AUXPCM Hostless",
		.stream_name = "AUXPCM Hostless",
		.cpu_dai_name   = "AUXPCM_HOSTLESS",
		.platform_name  = "msm-pcm-hostless",
		.dynamic = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1, /* this dainlink has playback support */
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
	},
	/* HDMI Hostless */
	{
		.name = "HDMI_RX_HOSTLESS",
		.stream_name = "HDMI_RX_HOSTLESS",
		.cpu_dai_name = "HDMI_HOSTLESS",
		.platform_name = "msm-pcm-hostless",
		.dynamic = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST, SND_SOC_DPCM_TRIGGER_POST},
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		.ignore_pmdown_time = 1, /* this dainlink has playback support */
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
	},
	{
		.name = "VoLTE",
		.stream_name = "VoLTE",
		.cpu_dai_name   = "VoLTE",
		.platform_name  = "msm-pcm-voice",
		.dynamic = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
				SND_SOC_DPCM_TRIGGER_POST},
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		/* this dainlink has playback support */
		.ignore_pmdown_time = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.be_id = MSM_FRONTEND_DAI_VOLTE,
	},
	{
		.name = "Voice2",
		.stream_name = "Voice2",
		.cpu_dai_name   = "Voice2",
		.platform_name  = "msm-pcm-voice",
		.dynamic = 1,
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
				SND_SOC_DPCM_TRIGGER_POST},
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		.ignore_suspend = 1,
		/* this dainlink has playback support */
		.ignore_pmdown_time = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.be_id = MSM_FRONTEND_DAI_VOICE2,
	},
	{
		.name = "MSM8960 LowLatency",
		.stream_name = "MultiMedia5",
		.cpu_dai_name	= "MultiMedia5",
		.platform_name  = "msm-lowlatency-pcm-dsp",
		.dynamic = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
				SND_SOC_DPCM_TRIGGER_POST},
		.ignore_suspend = 1,
		/* this dainlink has playback support */
		.ignore_pmdown_time = 1,
		.be_id = MSM_FRONTEND_DAI_MULTIMEDIA5,
	},
#ifdef CONFIG_PCM_ROUTE_VOICE_STUB
	{
		.name = "MSM8960 FM",
		.stream_name = "MultiMedia6",
		.cpu_dai_name	= "MultiMedia6",
		.platform_name  = "msm-pcm-loopback",
		.dynamic = 1,
		.codec_dai_name = "snd-soc-dummy-dai",
		.codec_name = "snd-soc-dummy",
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
				SND_SOC_DPCM_TRIGGER_POST},
		.ignore_suspend = 1,
		.no_host_mode = SND_SOC_DAI_LINK_NO_HOST,
		/* this dainlink has playback support */
		.ignore_pmdown_time = 1,
		.be_id = MSM_FRONTEND_DAI_MULTIMEDIA6,
	},
#endif
#ifdef CONFIG_SLIMBUS_MSM_CTRL
	/* Backend DAI Links */
	{
		.name = LPASS_BE_SLIMBUS_0_RX,
		.stream_name = "Slimbus Playback",
		.cpu_dai_name = "msm-dai-q6.16384",
		.platform_name = "msm-pcm-routing",
		.codec_name     = "sitar_codec",
		.codec_dai_name	= "sitar_rx1",
		.no_pcm = 1,
		.be_id = MSM_BACKEND_DAI_SLIMBUS_0_RX,
		.init = &msm8930_audrx_init,
		.be_hw_params_fixup = msm8930_slim_0_rx_be_hw_params_fixup,
		.ops = &msm8930_be_ops,
		.ignore_pmdown_time = 1, /* this dainlink has playback support */
	},
	{
		.name = LPASS_BE_SLIMBUS_0_TX,
		.stream_name = "Slimbus Capture",
		.cpu_dai_name = "msm-dai-q6.16385",
		.platform_name = "msm-pcm-routing",
		.codec_name     = "sitar_codec",
		.codec_dai_name	= "sitar_tx1",
		.no_pcm = 1,
		.be_id = MSM_BACKEND_DAI_SLIMBUS_0_TX,
		.be_hw_params_fixup = msm8930_slim_0_tx_be_hw_params_fixup,
		.ops = &msm8930_be_ops,
	},
#endif
	/* Backend BT/FM DAI Links */
	{
		.name = LPASS_BE_INT_BT_SCO_RX,
		.stream_name = "Internal BT-SCO Playback",
		.cpu_dai_name = "msm-dai-q6.12288",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name	= "msm-stub-rx",
		.no_pcm = 1,
		.be_id = MSM_BACKEND_DAI_INT_BT_SCO_RX,
		.be_hw_params_fixup = msm8930_btsco_be_hw_params_fixup,
		.ignore_pmdown_time = 1, /* this dainlink has playback support */
	},
	{
#ifdef CONFIG_RADIO_USE_MI2S
		.name = LPASS_BE_MI2S_RX,
		.stream_name = "MI2S Playback",
		.cpu_dai_name = "msm-dai-q6-mi2s",
		.platform_name = "msm-pcm-routing",
		.codec_name     = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.no_pcm = 1,
		.be_id = MSM_BACKEND_DAI_MI2S_RX,
		.be_hw_params_fixup = msm8930_be_hw_params_fixup,
		.ops = &msm8930_mi2s_be_ops,
	},
	{
		.name = LPASS_BE_MI2S_TX,
		.stream_name = "MI2S Capture",
		.cpu_dai_name = "msm-dai-q6-mi2s",
		.platform_name = "msm-pcm-routing",
		.codec_name     = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.no_pcm = 1,
		.be_id = MSM_BACKEND_DAI_MI2S_TX,
		.ops = &msm8930_mi2s_be_ops,
		},
	{
#endif
		.name = LPASS_BE_INT_BT_SCO_TX,
		.stream_name = "Internal BT-SCO Capture",
		.cpu_dai_name = "msm-dai-q6.12289",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name	= "msm-stub-tx",
		.no_pcm = 1,
		.be_id = MSM_BACKEND_DAI_INT_BT_SCO_TX,
		.be_hw_params_fixup = msm8930_btsco_be_hw_params_fixup,
	},
	{
		.name = LPASS_BE_INT_FM_RX,
		.stream_name = "Internal FM Playback",
		.cpu_dai_name = "msm-dai-q6.12292",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.no_pcm = 1,
		.be_id = MSM_BACKEND_DAI_INT_FM_RX,
		.be_hw_params_fixup = msm8930_be_hw_params_fixup,
		.ignore_pmdown_time = 1, /* this dainlink has playback support */
	},
	{
		.name = LPASS_BE_INT_FM_TX,
		.stream_name = "Internal FM Capture",
		.cpu_dai_name = "msm-dai-q6.12293",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.no_pcm = 1,
		.be_id = MSM_BACKEND_DAI_INT_FM_TX,
		.be_hw_params_fixup = msm8930_be_hw_params_fixup,
	},
	/* HDMI BACK END DAI Link */
	{
		.name = LPASS_BE_HDMI,
		.stream_name = "HDMI Playback",
		.cpu_dai_name = "msm-dai-q6-hdmi.8",
		.platform_name = "msm-pcm-routing",
		.codec_name     = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.no_pcm = 1,
		.be_id = MSM_BACKEND_DAI_HDMI_RX,
		.be_hw_params_fixup = msm8930_hdmi_be_hw_params_fixup,
		.ignore_pmdown_time = 1, /* this dainlink has playback support */
	},
	/* Backend AFE DAI Links */
	{
		.name = LPASS_BE_AFE_PCM_RX,
		.stream_name = "AFE Playback",
		.cpu_dai_name = "msm-dai-q6.224",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.no_pcm = 1,
		.be_id = MSM_BACKEND_DAI_AFE_PCM_RX,
		.be_hw_params_fixup = msm8930_proxy_be_hw_params_fixup,
		.ignore_pmdown_time = 1, /* this dainlink has playback support */
	},
	{
		.name = LPASS_BE_AFE_PCM_TX,
		.stream_name = "AFE Capture",
		.cpu_dai_name = "msm-dai-q6.225",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.no_pcm = 1,
		.be_id = MSM_BACKEND_DAI_AFE_PCM_TX,
		.be_hw_params_fixup = msm8930_proxy_be_hw_params_fixup,
	},
	/* AUX PCM Backend DAI Links */
	{
		.name = LPASS_BE_AUXPCM_RX,
		.stream_name = "AUX PCM Playback",
		.cpu_dai_name = "msm-dai-q6.2",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.no_pcm = 1,
		.be_id = MSM_BACKEND_DAI_AUXPCM_RX,
		.be_hw_params_fixup = msm8930_auxpcm_be_params_fixup,
		.ops = &msm8930_auxpcm_be_ops,
	},
	{
		.name = LPASS_BE_AUXPCM_TX,
		.stream_name = "AUX PCM Capture",
		.cpu_dai_name = "msm-dai-q6.3",
		.platform_name = "msm-pcm-routing",
		.codec_name = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.no_pcm = 1,
		.be_id = MSM_BACKEND_DAI_AUXPCM_TX,
		.be_hw_params_fixup = msm8930_auxpcm_be_params_fixup,
		.ops = &msm8930_auxpcm_be_ops,
	},
	/* Incall Music BACK END DAI Link */
	{
		.name = LPASS_BE_VOICE_PLAYBACK_TX,
		.stream_name = "Voice Farend Playback",
		.cpu_dai_name = "msm-dai-q6.32773",
		.platform_name = "msm-pcm-routing",
		.codec_name     = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-rx",
		.no_pcm = 1,
		.be_id = MSM_BACKEND_DAI_VOICE_PLAYBACK_TX,
		.be_hw_params_fixup = msm8930_be_hw_params_fixup,
	},
	/* Incall Record Uplink BACK END DAI Link */
	{
		.name = LPASS_BE_INCALL_RECORD_TX,
		.stream_name = "Voice Uplink Capture",
		.cpu_dai_name = "msm-dai-q6.32772",
		.platform_name = "msm-pcm-routing",
		.codec_name     = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.no_pcm = 1,
		.be_id = MSM_BACKEND_DAI_INCALL_RECORD_TX,
		.be_hw_params_fixup = msm8930_be_hw_params_fixup,
	},
	/* Incall Record Downlink BACK END DAI Link */
	{
		.name = LPASS_BE_INCALL_RECORD_RX,
		.stream_name = "Voice Downlink Capture",
		.cpu_dai_name = "msm-dai-q6.32771",
		.platform_name = "msm-pcm-routing",
		.codec_name     = "msm-stub-codec.1",
		.codec_dai_name = "msm-stub-tx",
		.no_pcm = 1,
		.be_id = MSM_BACKEND_DAI_INCALL_RECORD_RX,
		.be_hw_params_fixup = msm8930_be_hw_params_fixup,
		.ignore_pmdown_time = 1, /* this dainlink has playback support */
	},
};

struct snd_soc_card snd_soc_card_msm8930 = {
	.name		= "msm8930-sitar-snd-card",
	.dai_link	= msm8930_dai,
	.num_links	= ARRAY_SIZE(msm8930_dai),
	.controls	= sitar_msm8930_controls,
	.num_controls	= ARRAY_SIZE(sitar_msm8930_controls),
};

static struct platform_device *msm8930_snd_device;

/* Not used headset mic gpio in msm8930 sec model
static int msm8930_configure_headset_mic_gpios(void)
{
	int ret;
	int gpio_val = 0;

	if (socinfo_get_platform_subtype() == PLATFORM_SUBTYPE_SGLTE)
		gpio_val = GPIO_HS_SW_SEL;
	else
		gpio_val = 80;
	ret = gpio_request(gpio_val, "US_EURO_SWITCH");
	if (ret) {
		pr_err("%s: Failed to request gpio %d\n", __func__, gpio_val);
		return ret;
	}
	ret = gpio_direction_output(gpio_val, 0);
	if (ret) {
		pr_err("%s: Unable to set direction\n", __func__);
		gpio_free(gpio_val);
	}
	msm8930_headset_gpios_configured = 0;
	return 0;
}
static void msm8930_free_headset_mic_gpios(void)
{
	int gpio_val = 0;

	if (socinfo_get_platform_subtype() == PLATFORM_SUBTYPE_SGLTE)
		gpio_val = GPIO_HS_SW_SEL;
	else
		gpio_val = 80;
	if (msm8930_headset_gpios_configured)
		gpio_free(80);
} */

static struct  sec_audio_gpio msm8930_audio_gpio_table[] = {
#if defined(CONFIG_DOCK_EN)
#if !defined(CONFIG_MACH_MELIUS) && !defined(CONFIG_MACH_LT02_CHN_CTC)
	{
		.gpio = GPIO_VPS_AMP_EN,
		.name = "DOCK_EN",
	},
#endif
#endif /* CONFIG_DOCK_EN */
#ifdef CONFIG_EXT_SPK_AMP
	{
		.gpio = GPIO_SPK_AMP_EN,
		.name = "Speaker amplifier",
	},
#endif
#ifdef CONFIG_EXT_MAINMIC_BIAS
	{
		.gpio = GPIO_MAIN_MIC_BIAS_EN,
		.name = "Main Mic Bias",
	},
#endif /* CONFIG_EXT_MAINMIC_BIAS */

#ifdef CONFIG_EXT_EARMIC_BIAS
	{
		.gpio = GPIO_EAR_MIC_BIAS_EN,
		.name = "Ear mic bias",
	},
#endif
#ifdef CONFIG_EXT_SUBMIC_BIAS
	{
		.gpio = GPIO_SUB_MIC_BIAS_EN,
		.name = "Sub mic bias",
	},
#endif
};

static int msm8930_configure_audio_gpios(void)
{
	int ret;
	int i;
	int max_loop = ((sizeof(msm8930_audio_gpio_table))
				/(sizeof(msm8930_audio_gpio_table[0])));

	struct pm_gpio param = {
		.direction      = PM_GPIO_DIR_OUT,
		.output_buffer  = PM_GPIO_OUT_BUF_CMOS,
		.output_value   = 1,
		.pull	   = PM_GPIO_PULL_NO,
		.vin_sel	= PM_GPIO_VIN_S4,
		.out_strength   = PM_GPIO_STRENGTH_MED,
		.function       = PM_GPIO_FUNC_NORMAL,
	};

	for (i = 0; i < max_loop; i++) {
		pr_info("%s : %d\n", __func__,
				msm8930_audio_gpio_table[i].gpio);

		ret = gpio_request(msm8930_audio_gpio_table[i].gpio,
					msm8930_audio_gpio_table[i].name);
		if (ret) {
			pr_err("%s: Failed to request gpio %d\n", __func__,
				msm8930_audio_gpio_table[i].gpio);
			return ret;
		}
		/*in case of pm gpio*/
		if (msm8930_audio_gpio_table[i].gpio > NR_GPIO_IRQS) {
			ret = pm8xxx_gpio_config
				(msm8930_audio_gpio_table[i].gpio, &param);
			if (ret) {
				pr_err("%s: Failed to configure gpio %d\n",
					__func__,
					msm8930_audio_gpio_table[i].gpio);
				return ret;
			}
		} else {
			ret = gpio_tlmm_config
				(GPIO_CFG(msm8930_audio_gpio_table[i].gpio,
				0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN,
				GPIO_CFG_8MA), GPIO_CFG_ENABLE);
			if (ret) {
				pr_err("%s: Failed to configure gpio %d\n",
					__func__,
					msm8930_audio_gpio_table[i].gpio);
			}
		}
		ret = gpio_direction_output(msm8930_audio_gpio_table[i].gpio,
									0);
		if (ret) {
			pr_err("%s: Unable to set direction %d\n", __func__,
				msm8930_audio_gpio_table[i].gpio);
			gpio_free(msm8930_audio_gpio_table[i].gpio);
		}
	}
	msm8930_audio_gpios_configured = 0;
	return 0;
}

static void msm8930_free_audio_gpios(void)
{
	int i;
	int max_loop = ((sizeof(msm8930_audio_gpio_table))
				/(sizeof(msm8930_audio_gpio_table[0])));

	if (msm8930_audio_gpios_configured) {
		for (i = 0; i < max_loop; i++)
			gpio_free(msm8930_audio_gpio_table[i].gpio);
	}
}

static int __init msm8930_audio_init(void)
{
	int ret;
printk("%s Debashis\n",__func__);
	if (!soc_class_is_msm8930()) {
		pr_err("%s: Not the right machine type\n", __func__);
		return -ENODEV ;
	}
printk("%s Debashis1\n",__func__);	
#ifndef CONFIG_SLIMBUS_MSM_CTRL
	msm8930_dai_list = kzalloc(sizeof(msm8930_dai) +
			2 * sizeof(struct snd_soc_dai_link), GFP_KERNEL);

	pr_debug("%s: wcd9xxx_get_intf_type is %d",
			__func__, wcd9xxx_get_intf_type());

		memcpy(msm8930_dai_list, msm8930_dai, sizeof(msm8930_dai));
		memcpy(&msm8930_dai_list[ARRAY_SIZE(msm8930_dai)],
				msm8930_i2s_be_dai, sizeof(msm8930_i2s_be_dai));
		snd_soc_card_msm8930.dai_link = msm8930_dai_list;
		snd_soc_card_msm8930.num_links = ARRAY_SIZE(msm8930_dai) +
					ARRAY_SIZE(msm8930_i2s_be_dai);
#endif
	mbhc_cfg.calibration = def_sitar_mbhc_cal();
	if (!mbhc_cfg.calibration) {
		pr_err("Calibration data allocation failed\n");
		return -ENOMEM;
	}

	msm8930_snd_device = platform_device_alloc("soc-audio", 0);
	if (!msm8930_snd_device) {
		pr_err("Platform device allocation failed\n");
		kfree(mbhc_cfg.calibration);
		return -ENOMEM;
	}

	platform_set_drvdata(msm8930_snd_device, &snd_soc_card_msm8930);
	ret = platform_device_add(msm8930_snd_device);
	if (ret) {
		platform_device_put(msm8930_snd_device);
		kfree(mbhc_cfg.calibration);
		return ret;
	}
/* Not used headset mic gpio in msm8930 sec model
	if (msm8930_configure_headset_mic_gpios()) {
		pr_err("%s Fail to configure headset mic gpios\n", __func__);
		msm8930_headset_gpios_configured = 0;
	} else
		msm8930_headset_gpios_configured = 1; */

	if (msm8930_configure_audio_gpios()) {
		pr_err("%s Fail to configure audio gpios\n", __func__);
		msm8930_audio_gpios_configured = 0;
	} else
		msm8930_audio_gpios_configured = 1;

	atomic_set(&auxpcm_rsc_ref, 0);
	INIT_DELAYED_WORK(&ext_amp_dwork.dwork,	external_speaker_amp_work);

#ifdef CONFIG_RADIO_USE_MI2S
	atomic_set(&mi2s_rsc_ref, 0);
#endif
printk("%s Debashis3\n",__func__);
	return ret;

}
module_init(msm8930_audio_init);

static void __exit msm8930_audio_exit(void)
{
	if (!cpu_is_msm8930() && !cpu_is_msm8930aa() && !cpu_is_msm8627()) {
		pr_err("%s: Not the right machine type\n", __func__);
		return ;
	}
/* Not used headset mic gpio in msm8930 sec model
	msm8930_free_headset_mic_gpios(); */
	msm8930_free_audio_gpios();
	platform_device_unregister(msm8930_snd_device);
	kfree(mbhc_cfg.calibration);
}
module_exit(msm8930_audio_exit);

MODULE_DESCRIPTION("ALSA SoC MSM8930");
MODULE_LICENSE("GPL v2");
