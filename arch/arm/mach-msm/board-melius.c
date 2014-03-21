/* Copyright (c) 2011-2012, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/i2c.h>
#include <linux/i2c/sx150x.h>
#include <linux/i2c/isl9519.h>
#include <linux/gpio.h>
#include <linux/msm_ssbi.h>
#include <linux/regulator/msm-gpio-regulator.h>
#include <linux/mfd/pm8xxx/pm8921.h>
#include <linux/mfd/pm8xxx/pm8xxx-adc.h>
#include <linux/regulator/consumer.h>
#include <linux/spi/spi.h>
#include <linux/slimbus/slimbus.h>
#include <linux/bootmem.h>
#include <mach/msm_bus.h>
#ifdef CONFIG_ANDROID_PMEM
#include <linux/android_pmem.h>
#endif
#include <linux/dma-contiguous.h>
#include <linux/dma-mapping.h>
#include <linux/platform_data/qcom_crypto_device.h>
#include <linux/platform_data/qcom_wcnss_device.h>
#include <linux/leds.h>
#include <linux/leds-pm8xxx.h>
#include <linux/msm_tsens.h>

#ifdef CONFIG_ANDROID_RAM_CONSOLE
#include <linux/persistent_ram.h>
#include <linux/platform_data/ram_console.h>
#endif

#ifdef CONFIG_KS8851
#include <linux/ks8851.h>
#endif
#include <linux/gpio_keys.h>
#include <linux/memory.h>
#include <linux/memblock.h>
#include <linux/msm_thermal.h>

#include <linux/slimbus/slimbus.h>
#include <linux/mfd/wcd9xxx/core.h>
#include <linux/mfd/wcd9xxx/pdata.h>

#ifdef CONFIG_STM_LIS3DH
#include <linux/input/lis3dh.h>
#endif

#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/setup.h>
#include <asm/hardware/gic.h>
#include <asm/mach/mmc.h>
#include <asm/system_info.h>

#include <mach/board.h>
#include <mach/msm_iomap.h>
#include <mach/msm_spi.h>
#ifdef CONFIG_USB_MSM_OTG_72K
#include <mach/msm_hsusb.h>
#else
#include <linux/usb/msm_hsusb.h>
#endif
#include <linux/usb/android.h>
#include <mach/usbdiag.h>
#include <mach/socinfo.h>
#include <mach/rpm.h>
#include <mach/gpiomux.h>
#include <mach/msm_bus_board.h>
#include <mach/msm_memtypes.h>
#include <mach/dma.h>
#include <mach/msm_xo.h>
#include <mach/restart.h>

#if defined(CONFIG_VIDEO_MHL_V2)
#include <linux/sii9234.h>
#endif
#include <linux/msm_ion.h>
#include <mach/ion.h>
#include <mach/mdm2.h>
#include <mach/msm_rtb.h>
#include <linux/fmem.h>
#include <mach/msm_cache_dump.h>
#include <linux/clk.h>
#include <mach/kgsl.h>
#ifdef CONFIG_INPUT_MPU3050
#include <linux/input/mpu3050.h>
#endif
#if defined(CONFIG_INPUT_MPU6050) || defined(CONFIG_INPUT_MPU6500)
#include <linux/mpu6k_input.h>
#endif
#ifdef CONFIG_INPUT_YAS_SENSORS
#include <linux/yas.h>
#endif
#ifdef CONFIG_OPTICAL_TAOS_TRITON
#include <linux/i2c/taos.h>
#endif
#ifdef CONFIG_OPTICAL_TAOS_TMD3782
#include <linux/i2c/taos_tmd3782.h>
#endif
#ifdef CONFIG_SENSORS_GP2A030
#include <linux/gp2ap030.h>
#endif
#ifdef CONFIG_BCM2079X_NFC_I2C
#include <linux/nfc/bcm2079x.h>
#endif
#if defined(CONFIG_TDMB) || defined(CONFIG_TDMB_MODULE)
#include <mach/tdmb_pdata.h>
#endif
#include <linux/i2c-gpio.h>
#include <mach/msm8930-gpio.h>
#ifdef CONFIG_USB_SWITCH_TSU6721
#include <linux/i2c/tsu6721.h>
#include <linux/switch.h>
#endif

#ifdef CONFIG_MFD_MAX77693
#include <linux/mfd/max77693.h>
#include <linux/mfd/max77693-private.h>
#endif

#ifdef CONFIG_SEC_THERMISTOR
#include <mach/sec_thermistor.h>
#include <mach/msm8930-thermistor.h>
#endif

#include "timer.h"
#include "devices.h"
#include "devices-msm8x60.h"
#include "spm.h"
#include "pm.h"
#include <mach/cpuidle.h>
#include "rpm_resources.h"
#include <mach/mpm.h>
#include "clock.h"
#include "smd_private.h"
#include "pm-boot.h"
#include "msm_watchdog.h"
#include "board-8930.h"
#include "acpuclock-krait.h"

#ifdef CONFIG_SAMSUNG_JACK
#include <linux/sec_jack.h>
#endif
#ifdef CONFIG_2MIC_ES305
#include <sound/a2220.h>
#endif
#ifdef CONFIG_VIBETONZ
#include <linux/vibrator_msm8930.h>
#endif

#include <linux/power_supply.h>
// #include <linux/battery/sec_charger.h>
/*
#define SEC_CHARGER_I2C_SLAVEADDR	0x6B
*/
#ifdef CONFIG_PM8921_CHARGER
#include <linux/mfd/pm8xxx/pm8921-charger.h>
#endif
#ifdef CONFIG_PM8921_SEC_CHARGER
#include <linux/mfd/pm8xxx/pm8921-sec-charger.h>
#endif
#ifdef CONFIG_SEC_FPGA
#define GPIO_FPGA_CDONE		0
#include <linux/barcode_emul.h>
#endif
#ifdef CONFIG_FM_SI4705
#include <linux/i2c/si47xx_common.h>
#endif
#if defined(CONFIG_GSM_MODEM_SPRD6500)
#include <mach/msm_serial_hs.h>
#endif
extern void msm8960_init_battery(void);
bool ovp_state;
static struct platform_device msm_fm_platform_init = {
	.name = "iris_fm",
	.id   = -1,
};

#ifdef CONFIG_SEC_DEBUG
#include <mach/sec_debug.h>
#endif

#ifdef CONFIG_PROC_AVC
#include <linux/proc_avc.h>
#endif

#ifdef CONFIG_KS8851
#define KS8851_RST_GPIO		89
#define KS8851_IRQ_GPIO		90
#endif
#define HAP_SHIFT_LVL_OE_GPIO	47

#define HDMI_MHL_MUX_GPIO       73
#define MHL_GPIO_INT            72
#define MHL_GPIO_RESET          71
#define MHL_GPIO_PWR_EN         5
#ifdef CONFIG_TOUCHSCREEN_MXT224S
struct tsp_callbacks *charger_callbacks;
struct tsp_callbacks {
	void (*inform_charger)(struct tsp_callbacks *tsp_cb, bool mode);
};
#endif

#if defined(CONFIG_TOUCHSCREEN_ZINITIX_BT532)
static struct i2c_board_info __initdata zinitix_i2c_info[]  = {
	{
	I2C_BOARD_INFO("zinitix_touch", 0x20),
	.irq = MSM_GPIO_TO_INT(18),
	.platform_data = "zinitix_touch",
	},
};
#endif

unsigned int gpio_table[][GPIO_REV_MAX] = {
/* GPIO_INDEX	Rev	{#00,#01,#02,#03,#04,#05,#06,#07,#08,#09,#10 }, */
};

int gpio_rev(unsigned int index)
{
	int last;
	/* cur_lastest_device must be the last revision value. */
	int cur_latest_device = BOARD_REV02;

	if (system_rev >= GPIO_REV_MAX)
		return -EINVAL;

	if (system_rev <= cur_latest_device)
		return gpio_table[index][system_rev];

	/* Return last value for the next revision device */
	for (last = 0; last < GPIO_REV_MAX; last++) {
		if (gpio_table[index][last] == 0) {
			last--;
			break;
		}
	}

	return gpio_table[index][last];
}

#if defined(CONFIG_GPIO_SX150X) || defined(CONFIG_GPIO_SX150X_MODULE)

struct sx150x_platform_data msm8930_sx150x_data[] = {
	[SX150X_CAM] = {
		.gpio_base         = GPIO_CAM_EXPANDER_BASE,
		.oscio_is_gpo      = false,
		.io_pullup_ena     = 0x0,
		.io_pulldn_ena     = 0xc0,
		.io_open_drain_ena = 0x0,
		.irq_summary       = -1,
	},
};

#endif

#define MSM_PMEM_ADSP_SIZE         0x7800000
#define MSM_PMEM_AUDIO_SIZE        0x4CF000
#ifdef CONFIG_FB_MSM_HDMI_AS_PRIMARY
#define MSM_PMEM_SIZE 0x4000000 /* 64 Mbytes */
#else
#define MSM_PMEM_SIZE 0x2800000 /* 40 Mbytes */
#endif
#define MSM_LIQUID_PMEM_SIZE 0x4000000 /* 64 Mbytes */

#ifdef CONFIG_MSM_MULTIMEDIA_USE_ION
#define HOLE_SIZE	0x20000
#define MSM_CONTIG_MEM_SIZE  0x65000
#ifdef CONFIG_MSM_IOMMU
#define MSM_ION_MM_SIZE            0x4600000 /* 56MB(0x3800000) -> 70MB */
#define MSM_ION_SF_SIZE            0x0
#define MSM_ION_QSECOM_SIZE	0x1700000 /* 7.5MB(0x780000) -> 23MB */
#define MSM_ION_HEAP_NUM	8
#else
#define MSM_ION_SF_SIZE		MSM_PMEM_SIZE
#define MSM_ION_MM_SIZE		MSM_PMEM_ADSP_SIZE
#define MSM_ION_QSECOM_SIZE	0x600000 /* (6MB) */
#define MSM_ION_HEAP_NUM	8
#endif
#define MSM_ION_MM_FW_SIZE	(0x200000 - HOLE_SIZE) /* 2MB - 128Kb */
#define MSM_ION_MFC_SIZE	SZ_8K
#define MSM_ION_AUDIO_SIZE	MSM_PMEM_AUDIO_SIZE

#define MSM_LIQUID_ION_MM_SIZE (MSM_ION_MM_SIZE + 0x600000)
#define MSM_LIQUID_ION_SF_SIZE MSM_LIQUID_PMEM_SIZE
#define MSM_HDMI_PRIM_ION_SF_SIZE MSM_HDMI_PRIM_PMEM_SIZE

#define MSM_MM_FW_SIZE	(0x200000 - HOLE_SIZE) /*2MB -128Kb */
#define MSM8930_FIXED_AREA_START (0xa0000000 - (MSM_ION_MM_FW_SIZE + \
								HOLE_SIZE))
#define MAX_FIXED_AREA_SIZE	0x10000000
#define MSM8930_FW_START	MSM8930_FIXED_AREA_START
#define MSM_ION_ADSP_SIZE	SZ_8M
#else
#define MSM_CONTIG_MEM_SIZE  0x110C000
#define MSM_ION_HEAP_NUM	1
#endif
#ifdef CONFIG_2MIC_ES305
#define MSM_A2220_I2C_BUS_ID            18
#define A2220_WAKEUP				GPIO_2MIC_PW_DN
#define A2220_RESET					PMIC_GPIO_2MIC_RST
#endif

#if defined(CONFIG_VIBETONZ) && defined(CONFIG_MOTOR_DRV_MAX77693)
static struct max77693_haptic_platform_data max77693_haptic_pdata = {
	.max_timeout = 10000,
	.duty = 44000,
	.period = 44642,
	.reg2 = MOTOR_LRA | EXT_PWM | DIVIDER_256,
	.init_hw = NULL,
	.motor_en = NULL,
	.pwm_id = 1,
	.regulator_name = NULL,
};

static void max77693_haptic_power_onoff(int onoff)
{
	int ret;
	static struct regulator *reg_l8;

	if (!reg_l8) {
		reg_l8 = regulator_get(NULL, "haptic_pwr");
		ret = regulator_set_voltage(reg_l8, 3000000, 3000000);

		if (IS_ERR(reg_l8)) {
			printk(KERN_ERR"could not get haptic_pwr, rc = %ld\n",
				PTR_ERR(reg_l8));
			return;
		}
	}

	if (onoff) {
		ret = regulator_enable(reg_l8);
		if (ret) {
			printk(KERN_ERR"enable l8 failed, rc=%d\n", ret);
			return;
		}
		printk(KERN_DEBUG"haptic power_on is finished.\n");
	} else {
		if (regulator_is_enabled(reg_l8)) {
			ret = regulator_disable(reg_l8);
			if (ret) {
				printk(KERN_ERR"disable l8 failed, rc=%d\n",
									ret);
				return;
			}
		}
		printk(KERN_DEBUG"haptic power_off is finished.\n");
	}
}
#endif
#ifdef CONFIG_MFD_MAX77693
extern sec_battery_platform_data_t sec_battery_pdata;
struct max77693_platform_data msm8960_max77693_info = {
	.irq_base       = IF_PMIC_IRQ_BASE,
	.irq_gpio       = GPIO_IF_PMIC_IRQ,
	.wakeup         = 1,
	.wc_irq_gpio	= 0,
	.muic           = &max77693_muic,
#if defined(CONFIG_REGULATOR_MAX77693)
	.regulators     = max77693_regulators,
	.num_regulators = MAX77693_REG_MAX,
#endif
#if defined(CONFIG_VIBETONZ) && defined(CONFIG_MOTOR_DRV_MAX77693)
	.haptic_data = &max77693_haptic_pdata,
#endif
#ifdef CONFIG_LEDS_MAX77693
	.led_data = &max77693_led_pdata,
#endif
#ifdef CONFIG_CHARGER_MAX77XXX
	.charger_data = &sec_battery_pdata,
#endif
};

static struct i2c_gpio_platform_data max77693_i2c_gpio_data = {
	.sda_pin                = GPIO_IF_PMIC_SDA,
	.scl_pin                = GPIO_IF_PMIC_SCL,
	.udelay                 = 2,
	.sda_is_open_drain      = 0,
	.scl_is_open_drain      = 0,
	.scl_is_output_only     = 0,
};

static struct platform_device max77693_i2c_gpio_device = {
	.name                   = "i2c-gpio",
	.id                     = MSM_MUSB_I2C_BUS_ID,
	.dev.platform_data      = &max77693_i2c_gpio_data,
};

static struct i2c_board_info max77693_i2c_board_info[] = {

	{
		I2C_BOARD_INFO("max77693",  (0xCC >> 1)),
		.platform_data  = &msm8960_max77693_info,
		.irq    = MSM_GPIO_TO_INT(GPIO_IF_PMIC_IRQ),
	},

};
#endif

#if  defined(CONFIG_IR_REMOCON_FPGA)
static void irda_wake_en(bool onoff)
{
	gpio_direction_output(GPIO_FPGA_RST_N, onoff);
	printk(KERN_ERR "%s: %d\n", __func__, onoff);
}

static void irda_device_init(void)
{
	printk(KERN_ERR "%s called!\n", __func__);
	gpio_tlmm_config(GPIO_CFG(GPIO_IRDA_SDA, 0,
		GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);
	gpio_tlmm_config(GPIO_CFG(GPIO_IRDA_SCL, 0,
		GPIO_CFG_OUTPUT,	GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);

	gpio_request(GPIO_CRESET_B, "irda_creset");
	gpio_direction_output(GPIO_CRESET_B, 0);
	gpio_request(GPIO_FPGA_RST_N, "irda_wake");
//	gpio_direction_output(GPIO_FPGA_RST_N, 0);
	gpio_tlmm_config(GPIO_CFG(GPIO_FPGA_RST_N, 0,
		GPIO_CFG_OUTPUT,	GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), 1);

	gpio_request(GPIO_IRDA_IRQ, "irda_irq");
//	gpio_direction_input(GPIO_IRDA_IRQ);
	gpio_tlmm_config(GPIO_CFG(GPIO_IRDA_IRQ, 0,
		GPIO_CFG_INPUT,	GPIO_CFG_PULL_UP, GPIO_CFG_2MA), 1);

	gpio_request(GPIO_IRDA_EN, "irda_en");
//	gpio_direction_output(GPIO_IRDA_EN, 0);
	gpio_tlmm_config(GPIO_CFG(GPIO_IRDA_EN, 0,
		GPIO_CFG_OUTPUT,	GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), 1);

}

static void irda_vdd_onoff(bool onoff)
{
	if (onoff)
		gpio_set_value(GPIO_IRDA_EN, 1);
	else
		gpio_set_value(GPIO_IRDA_EN, 0);
}
#endif

#ifdef CONFIG_SENSORS_HALL
#define GPIO_HALL_IC_IRQ	33
#endif
#ifdef CONFIG_KERNEL_MSM_CONTIG_MEM_REGION
static unsigned msm_contig_mem_size = MSM_CONTIG_MEM_SIZE;
static int __init msm_contig_mem_size_setup(char *p)
{
	msm_contig_mem_size = memparse(p, NULL);
	return 0;
}
early_param("msm_contig_mem_size", msm_contig_mem_size_setup);
#endif

int msm8930_get_board_rev(void) {return (int)system_rev; }
int msm8930_get_cable_status(void) {return 1; }
#ifdef CONFIG_USB_SWITCH_TSU6721
static enum cable_type_t set_cable_status;

static struct switch_dev switch_dock = {
	.name = "dock",
};

static int tsu6721_dock_init(void)
{
	int ret;
	/* for CarDock, DeskDock */
	ret = switch_dev_register(&switch_dock);
	if (ret < 0) {
		pr_err("Failed to register dock switch. %d\n", ret);
		return ret;
	}
	return 0;
}

static struct i2c_gpio_platform_data tsu_i2c_gpio_data = {
	.sda_pin		= GPIO_MUS_SDA,
	.scl_pin		= GPIO_MUS_SCL,
	.udelay			= 2,
	.sda_is_open_drain	= 0,
	.scl_is_open_drain	= 0,
	.scl_is_output_only	= 0,
};

static struct platform_device tsu_i2c_gpio_device = {
	.name			= "i2c-gpio",
	.id			= MSM_TSU6721_I2C_BUS_ID,
	.dev.platform_data	= &tsu_i2c_gpio_data,
};

#define BATT_SEARCH_CNT_MAX	10
static void tsu6721_oxp_callback(int state)
{
	if (state == 1) {
		ovp_state = true;
		/*ENABLE*/
	} else if (state == 0) {
		ovp_state = false;
		/*DISABLE*/
	}
}

static void tsu6721_callback(enum cable_type_t cable_type, int attached)
{
	union power_supply_propval value;
	int i, ret = 0;
	struct power_supply *psy;

	set_cable_status = attached ? cable_type : CABLE_TYPE_NONE;

	switch (cable_type) {
	case CABLE_TYPE_USB:
		pr_info("%s USB Cable is %s\n",
			__func__, attached ? "attached" : "detached");
		sec_otg_set_vbus_state(attached);
		break;
	case CABLE_TYPE_AC:
		pr_info("%s Charger is %s\n",
			__func__, attached ? "attached" : "detached");
		break;
	case CABLE_TYPE_UARTOFF:
		pr_info("%s Uart is %s\n",
			__func__, attached ? "attached" : "detached");
		break;
	case CABLE_TYPE_JIG:
		pr_info("%s Jig is %s\n",
			__func__, attached ? "attached" : "detached");
		return;
	case CABLE_TYPE_CDP:
		pr_info("%s USB CDP is %s\n",
			__func__, attached ? "attached" : "detached");
		break;
	case CABLE_TYPE_OTG:
		pr_info("%s OTG is %s\n",
			__func__, attached ? "attached" : "detached");

		if (attached)
			sec_otg_set_id_state(attached);
		return;
	case CABLE_TYPE_AUDIO_DOCK:
		pr_info("%s Audiodock is %s\n",
			__func__, attached ? "attached" : "detached");
		sec_otg_set_dock_state(attached);
		return;
	case CABLE_TYPE_CARDOCK:
		pr_info("%s Cardock is %s\n",
			__func__, attached ? "attached" : "detached");
		switch_set_state(&switch_dock, attached ? 2 : 0);
		break;
	case CABLE_TYPE_DESK_DOCK:
		pr_info("%s Deskdock is %s\n",
			__func__, attached ? "attached" : "detached");
		switch_set_state(&switch_dock, attached);
		break;
	case CABLE_TYPE_INCOMPATIBLE:
		pr_info("%s Incompatible Charger is %s\n",
			__func__, attached ? "attached" : "detached");
		break;
	default:
		break;
	}

	for (i = 0; i < BATT_SEARCH_CNT_MAX; i++) {
		psy = power_supply_get_by_name("battery");
		if (psy)
			break;
	}
	if (i == BATT_SEARCH_CNT_MAX) {
		pr_err("%s: fail to get battery ps\n", __func__);
		return;
	}
	if (psy == NULL || psy->set_property == NULL) {
		pr_err("%s: battery ps doesn't support set_property()\n",
				__func__);
		return;
	}

	switch (set_cable_status) {
	case CABLE_TYPE_MISC:
		value.intval = POWER_SUPPLY_TYPE_MISC;
		break;
	case CABLE_TYPE_USB:
		value.intval = POWER_SUPPLY_TYPE_USB;
		break;
	case CABLE_TYPE_AC:
		value.intval = POWER_SUPPLY_TYPE_MAINS;
		break;
	case CABLE_TYPE_UARTOFF:
		value.intval = POWER_SUPPLY_TYPE_UARTOFF;
		break;
	case CABLE_TYPE_CARDOCK:
		value.intval = POWER_SUPPLY_TYPE_CARDOCK;
		break;
	case CABLE_TYPE_CDP:
		value.intval = POWER_SUPPLY_TYPE_USB_CDP;
		break;
	case CABLE_TYPE_INCOMPATIBLE:
		value.intval = POWER_SUPPLY_TYPE_MAINS;
		break;
	case CABLE_TYPE_NONE:
		value.intval = POWER_SUPPLY_TYPE_BATTERY;
		if (cable_type == CABLE_TYPE_UARTOFF)
			value.intval = POWER_SUPPLY_TYPE_UARTOFF;
		break;
	default:
		pr_err("%s: invalid cable :%d\n", __func__, set_cable_status);
		return;
	}

	ret = psy->set_property(psy, POWER_SUPPLY_PROP_ONLINE, &value);
	if (ret) {
		pr_err("%s: fail to set power_suppy ONLINE property(%d)\n",
			__func__, ret);
	}
}

static struct tsu6721_platform_data tsu6721_pdata = {
	.callback = tsu6721_callback,
	.dock_init = tsu6721_dock_init,
	.oxp_callback = tsu6721_oxp_callback,
	.mhl_sel = NULL,

};

static struct i2c_board_info micro_usb_i2c_devices_info[] __initdata = {
	{
		I2C_BOARD_INFO("tsu6721", 0x4A >> 1),
		.platform_data = &tsu6721_pdata,
		.irq = MSM_GPIO_TO_INT(14),
	},
};
#endif
#if defined(CONFIG_VIDEO_MHL_V2)
#ifdef CONFIG_MHL_NEW_CBUS_MSC_CMD
static void muic_mhl_cb(bool attached, int charger)
{
	union power_supply_propval value;
	int i, ret = 0;
	struct power_supply *psy;
	int current_cable_type = POWER_SUPPLY_TYPE_MISC;
	int sub_type = ONLINE_SUB_TYPE_MHL;
	int power_type = ONLINE_POWER_TYPE_UNKNOWN;
#ifdef CONFIG_MFD_MAX77693
	int muic_cable_type = max77693_muic_get_charging_type();

	pr_info("%s: muic cable_type = %d\n",
			__func__, muic_cable_type);
	switch (muic_cable_type) {
	case CABLE_TYPE_SMARTDOCK_MUIC:
	case CABLE_TYPE_SMARTDOCK_TA_MUIC:
	case CABLE_TYPE_SMARTDOCK_USB_MUIC:
		return;
	default:
		 break;
	}
#endif
	if (charger == 0x00) {
		pr_info("TA charger\n");
		power_type = ONLINE_POWER_TYPE_MHL_500;
	} else if (charger == 0x01) {
		pr_info("TA powered charger\n");
		power_type = ONLINE_POWER_TYPE_MHL_900;
	} else if (charger == 0x02) {
		pr_info("TA powered charger\n");
		power_type = ONLINE_POWER_TYPE_MHL_1500;
	} else if (charger == 0x03) {
		pr_info("USB charger\n");
		power_type = ONLINE_POWER_TYPE_USB;
	} else
		current_cable_type = POWER_SUPPLY_TYPE_BATTERY;

	for (i = 0; i < 10; i++) {
		psy = power_supply_get_by_name("battery");
		if (psy)
			break;
	}
	if (i == 10) {
		pr_err("%s: fail to get battery ps\n", __func__);
		return;
	}

	value.intval = current_cable_type<<ONLINE_TYPE_MAIN_SHIFT
		| sub_type<<ONLINE_TYPE_SUB_SHIFT
		| power_type<<ONLINE_TYPE_PWR_SHIFT;
	ret = psy->set_property(psy, POWER_SUPPLY_PROP_ONLINE,
		&value);
	if (ret) {
		pr_err("%s: fail to set power_suppy ONLINE property(%d)\n",
			__func__, ret);
	}
}
#else
#define CABLE_TYPE_MISC 3
#define CABLE_TYPE_NONE 0
static void muic_mhl_cb(bool attached)
{
	union power_supply_propval value;
	int i, set_cable_status, ret = 0;
	struct power_supply *psy;
	
	pr_info("mhl_cb attached %d\n", attached);
	set_cable_status = attached ? CABLE_TYPE_MISC : CABLE_TYPE_NONE;

	for (i = 0; i < 10; i++) {
		psy = power_supply_get_by_name("battery");
		if (psy)
			break;
	}
	if (i == 10) {
		pr_err("%s: fail to get battery ps\n", __func__);
		return;
	}

	switch (set_cable_status) {
	case CABLE_TYPE_MISC:
		value.intval = POWER_SUPPLY_TYPE_MISC;
		break;
	case CABLE_TYPE_NONE:
		value.intval = POWER_SUPPLY_TYPE_BATTERY;
		break;
	default:
		pr_err("%s: invalid cable :%d\n", __func__, set_cable_status);
		return;
	}

	ret = psy->set_property(psy, POWER_SUPPLY_PROP_ONLINE,
		&value);
	if (ret) {
		pr_err("%s: fail to set power_suppy ONLINE property(%d)\n",
			__func__, ret);
	}
}
#endif
static void msm8930_mhl_gpio_init(void)
{
#if !defined(CONFIG_MACH_MELIUS_CHN_CTC)
	int ret;
	ret = gpio_request(GPIO_MHL_RST, "mhl_rst");
	if (ret < 0) {
		pr_err("mhl_rst gpio_request is failed\n");
		return;
	}
	ret = gpio_request(GPIO_MHL_WAKE_UP, "mhl_wakeup");
	if (ret < 0) {
		pr_err("mhl_en gpio_request is failed\n");
		return;
	}
#endif
}
static void mhl_gpio_config(void)
{
#if !defined(CONFIG_MACH_MELIUS_CHN_CTC)
	gpio_tlmm_config(GPIO_CFG(GPIO_MHL_RST, 0, GPIO_CFG_OUTPUT,
				GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), 1);
	gpio_tlmm_config(GPIO_CFG(GPIO_MHL_WAKE_UP, 0, GPIO_CFG_OUTPUT,
				GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), 1);
#endif
}

static struct i2c_gpio_platform_data mhl_i2c_gpio_data = {
	.sda_pin   = GPIO_MHL_SDA,
	.scl_pin    = GPIO_MHL_SCL,
	.udelay    = 3,/*(i2c clk speed: 500khz / udelay)*/
};

static struct platform_device mhl_i2c_gpio_device = {
	.name       = "i2c-gpio",
	.id     = MSM_MHL_I2C_BUS_ID,
	.dev        = {
		.platform_data  = &mhl_i2c_gpio_data,
	},
};

/*
gpio_interrupt pin is very changable each different h/w_rev or  board.
*/
int get_mhl_int_irq(void)
{
	return  MSM_GPIO_TO_INT(GPIO_MHL_INT);
}

static struct regulator *mhl_l12, *mhl_l35, *mhl_lvs7;

static bool sii9234_pre_power_config(void)
{
	int rc;
	static bool is_pre_cfg_done;
	if (is_pre_cfg_done)
		return is_pre_cfg_done;

	mhl_l12 = regulator_get(NULL, "8917_l12");
	rc = regulator_set_voltage(mhl_l12, 1200000, 1200000);
	if (rc)
		pr_err("error setting voltage for mhl_l12\n");


	mhl_l35 = regulator_get(NULL, "8917_l35");
	rc = regulator_set_voltage(mhl_l35, 3300000, 3300000);
	if (rc)
		pr_err("error setting voltage for mhl_l35\n");

	mhl_lvs7 = regulator_get(NULL, "8917_lvs7");

	is_pre_cfg_done = true;
	return is_pre_cfg_done;
}


static void sii9234_hw_onoff(bool onoff)
{
	int rc;
	/*VPH_PWR : mhl_power_source
	VMHL_3.3V, VSIL_A_1.2V, VMHL_1.8V
	just power control with HDMI_EN pin or control Regulator12*/
	if (!sii9234_pre_power_config())
		return;

	if (onoff) {
		rc = regulator_enable(mhl_l12);
		if (rc)
			pr_err("error enabling regulator mhl_l12\n");

		rc = regulator_enable(mhl_l35);
		if (rc)
			pr_err("error enabling regulator mhl_l35\n");

		rc = regulator_enable(mhl_lvs7);
		if (rc)
			pr_err("error enabling regulator mhl_lvs7\n");

		usleep(1000);

	} else {
		rc = regulator_disable(mhl_l12);
		if (rc)
			pr_err("error disabling regulator mhl_l12\n");

		rc = regulator_disable(mhl_l35);
		if (rc)
			pr_err("error disabling regulator mhl_l35\n");

		rc = regulator_disable(mhl_lvs7);
		if (rc)
			pr_err("error disabling regulator mhl_lvs7\n");

		usleep_range(10000, 20000);
		if (gpio_direction_output(GPIO_MHL_RST, 0)) {
			pr_err("%s error in making GPIO_MHL_RST Low\n"
				, __func__);
		}
	}
}

static void sii9234_hw_reset(void)
{
	usleep_range(10000, 20000);
	if (gpio_direction_output(GPIO_MHL_RST, 1))
		printk(KERN_ERR "%s error in making GPIO_MHL_RST HIGH\n",
			 __func__);

	usleep_range(5000, 20000);
	if (gpio_direction_output(GPIO_MHL_RST, 0))
		printk(KERN_ERR "%s error in making GPIO_MHL_RST Low\n",
			 __func__);

	usleep_range(10000, 20000);
	if (gpio_direction_output(GPIO_MHL_RST, 1))
		printk(KERN_ERR "%s error in making GPIO_MHL_RST HIGH\n",
			 __func__);
	msleep(30);
}

struct sii9234_platform_data sii9234_pdata = {
	.get_irq = get_mhl_int_irq,
	.hw_onoff = sii9234_hw_onoff,
	.hw_reset = sii9234_hw_reset,
	.gpio_cfg = mhl_gpio_config,
	.swing_level = 0xF6,
	.vbus_present = muic_mhl_cb,
};

static struct i2c_board_info mhl_i2c_board_info[] = {
	{
		I2C_BOARD_INFO("sii9234_mhl_tx", 0x72>>1),
		.platform_data = &sii9234_pdata,
	},
	{
		I2C_BOARD_INFO("sii9234_tpi", 0x7A>>1),
		.platform_data = &sii9234_pdata,
	},
	{
		I2C_BOARD_INFO("sii9234_hdmi_rx", 0x92>>1),
		.platform_data = &sii9234_pdata,
	},
	{
		I2C_BOARD_INFO("sii9234_cbus", 0xC8>>1),
		.platform_data = &sii9234_pdata,
	},
};
#endif /*defined(CONFIG_VIDEO_MHL_V2)*/

#if defined(CONFIG_INPUT_YAS_SENSORS) || defined(CONFIG_OPTICAL_TAOS_TMD3782)
enum {
	SNS_PWR_OFF,
	SNS_PWR_ON,
	SNS_PWR_KEEP
};

static void sensor_power_on_vdd(int, int);
#endif

#ifdef CONFIG_MSM_ACTUATOR /* For Camera Actuator By Teddy */
static struct i2c_gpio_platform_data actuator_i2c_gpio_data = {
	.sda_pin = GPIO_I2C_DATA_AF,
	.scl_pin = GPIO_I2C_CLK_AF,
	.udelay = 5,
};
static struct platform_device actuator_i2c_gpio_device = {
	.name = "i2c-gpio",
	.id = MSM_ACTUATOR_I2C_BUS_ID,
	.dev = {
		.platform_data = &actuator_i2c_gpio_data,
	},
};
#endif

#if defined(CONFIG_INPUT_YAS_SENSORS)
static void sensors_regulator_on(bool onoff)
{
	int flag = onoff ? 1 : 0;
	sensor_power_on_vdd(flag, flag);
}

static struct mag_platform_data magnetic_pdata = {
	.position = 1,
};

#if 0 /* Removed as actuator was under sensors flag */
#ifdef CONFIG_MSM_ACTUATOR /* For Camera Actuator By Teddy */
static struct i2c_gpio_platform_data actuator_i2c_gpio_data = {
	.sda_pin = GPIO_I2C_DATA_AF,
	.scl_pin = GPIO_I2C_CLK_AF,
	.udelay = 5,
};

static struct platform_device actuator_i2c_gpio_device = {
	.name = "i2c-gpio",
	.id = MSM_ACTUATOR_I2C_BUS_ID,
	.dev = {
		.platform_data = &actuator_i2c_gpio_data,
	},
};
#endif
#endif

#if defined(CONFIG_MACH_MELIUS_CHN_CTC) ||  defined(CONFIG_MACH_CRATER_CHN_CTC)
static struct i2c_gpio_platform_data sns_i2c_gpio_data = {
	.sda_pin   = GPIO_SENSOR_SDA_1_8V,
	.scl_pin    = GPIO_SENSOR_SCL_1_8V,
	.udelay    = 1,/*(i2c clk speed: 500khz / udelay)*/
};

static struct platform_device sns_i2c_gpio_device = {
	.name       = "i2c-gpio",
	.id     = MSM_SNS_I2C_BUS_ID,
	.dev        = {
		.platform_data  = &sns_i2c_gpio_data,
	},
};
#endif

#if defined(CONFIG_MACH_CRATER_CHN_CTC)||defined(CONFIG_MACH_MELIUS_CHN_CTC)
void sensor_gpio_init(void){
	pr_info("sensor gpio init \n");
	gpio_tlmm_config(GPIO_CFG(GPIO_SENSOR_SDA_1_8V, 0, GPIO_CFG_INPUT,
		GPIO_CFG_NO_PULL, GPIO_CFG_2MA),GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(GPIO_SENSOR_SCL_1_8V, 0, GPIO_CFG_INPUT,
		GPIO_CFG_NO_PULL, GPIO_CFG_2MA),GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(GPIO_GYRO_INT_N, 0, GPIO_CFG_INPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_8MA),GPIO_CFG_ENABLE);
}
#endif
#if defined(CONFIG_INPUT_MPU6050) || defined(CONFIG_INPUT_MPU6500)
static struct mpu6k_input_platform_data mpu6k_pdata = {
	.power_on = sensors_regulator_on,
	.orientation = {0, 1, 0,
			1, 0, 0,
			0, 0, -1},
	.acc_cal_path = "/efs/calibration_data",
	.gyro_cal_path = "/efs/gyro_cal_data",
};
#endif

static struct i2c_board_info sns_i2c_board_info[] = {
	{
		I2C_BOARD_INFO("geomagnetic", 0x2e),
		.platform_data = &magnetic_pdata,
	},
#ifdef CONFIG_INPUT_MPU6050
	{
		I2C_BOARD_INFO("mpu6050_input", 0x68),
		.platform_data = &mpu6k_pdata,
		.irq = MSM_GPIO_TO_INT(GPIO_GYRO_INT_N),
	},
#endif
#ifdef CONFIG_INPUT_MPU6500
	{
		I2C_BOARD_INFO("mpu6500_input", 0x62), /*dummy address*/
		.platform_data = &mpu6k_pdata,
		.irq = MSM_GPIO_TO_INT(GPIO_GYRO_INT_N),
	},
#endif
};
#endif

#if defined(CONFIG_INPUT_YAS_SENSORS) ||\
	defined(CONFIG_OPTICAL_GP2AP020A00F) ||\
	defined(CONFIG_OPTICAL_TAOS_TMD3782)
static int __init sensor_device_init(void)
{
	sensor_power_on_vdd(SNS_PWR_ON, SNS_PWR_KEEP);
	msleep(20);
	sensor_power_on_vdd(SNS_PWR_KEEP, SNS_PWR_ON);

	gpio_request(GPIO_GYRO_INT_N, "GYRO_INT");
	gpio_direction_input(GPIO_GYRO_INT_N);
#ifdef CONFIG_MACH_CRATER_CHN_CTC
	magnetic_pdata.position = 3;
#else
	magnetic_pdata.position = 6;
#endif
#if defined(CONFIG_MACH_MELIUS_VZW) || defined(CONFIG_MACH_MELIUS_USC) \
	|| defined(CONFIG_MACH_MELIUS_SPR)
	if(system_rev > BOARD_REV00) {
		mpu6k_pdata.orientation[0] = -1;
		mpu6k_pdata.orientation[1] = 0;
		mpu6k_pdata.orientation[2] = 0;
		mpu6k_pdata.orientation[3] = 0;
		mpu6k_pdata.orientation[4] = 1;
		mpu6k_pdata.orientation[5] = 0;
		mpu6k_pdata.orientation[6] = 0;
		mpu6k_pdata.orientation[7] = 0;
		mpu6k_pdata.orientation[8] = -1;
	}
#elif defined(CONFIG_MACH_MELIUS_SKT) || defined(CONFIG_MACH_MELIUS_LGT) \
	|| defined(CONFIG_MACH_MELIUS_KTT)
	if(system_rev == BOARD_REV10) {
		mpu6k_pdata.orientation[0] = 1;
		mpu6k_pdata.orientation[1] = 0;
		mpu6k_pdata.orientation[2] = 0;
		mpu6k_pdata.orientation[3] = 0;
		mpu6k_pdata.orientation[4] = -1;
		mpu6k_pdata.orientation[5] = 0;
		mpu6k_pdata.orientation[6] = 0;
		mpu6k_pdata.orientation[7] = 0;
		mpu6k_pdata.orientation[8] = -1;
	}else{
		mpu6k_pdata.orientation[0] = 0;
		mpu6k_pdata.orientation[1] = 1;
		mpu6k_pdata.orientation[2] = 0;
		mpu6k_pdata.orientation[3] = 1;
		mpu6k_pdata.orientation[4] = 0;
		mpu6k_pdata.orientation[5] = 0;
		mpu6k_pdata.orientation[6] = 0;
		mpu6k_pdata.orientation[7] = 0;
		mpu6k_pdata.orientation[8] = -1;
	}
#else
	mpu6k_pdata.orientation[0] = 0;
	mpu6k_pdata.orientation[1] = 1;
	mpu6k_pdata.orientation[2] = 0;
	mpu6k_pdata.orientation[3] = 1;
	mpu6k_pdata.orientation[4] = 0;
	mpu6k_pdata.orientation[5] = 0;
	mpu6k_pdata.orientation[6] = 0;
	mpu6k_pdata.orientation[7] = 0;
	mpu6k_pdata.orientation[8] = -1;
#endif
	return 0;
}

static struct regulator *vsensor_2p85, *vsensor_1p8;
static int sensor_power_2p85_cnt, sensor_power_1p8_cnt;

static void sensor_power_on_vdd(int onoff_l9, int onoff_lvs2)
{
	int ret;

	if (vsensor_2p85 == NULL) {
		vsensor_2p85 = regulator_get(NULL, "sensor_opt");
		if (IS_ERR(vsensor_2p85))
			return ;

		ret = regulator_set_voltage(vsensor_2p85, 2850000, 2850000);
		if (ret)
			pr_err("%s: error vsensor_2p85 setting voltage ret=%d\n",
				__func__, ret);
	}

	if (vsensor_1p8 == NULL) {
		vsensor_1p8 = regulator_get(NULL, "sensor_pwr");
		if (IS_ERR(vsensor_1p8))
			return ;
	}

	if (onoff_l9 == SNS_PWR_ON) {
		sensor_power_2p85_cnt++;
		ret = regulator_enable(vsensor_2p85);
		if (ret)
			pr_err("%s: error enablinig regulator\n", __func__);
	} else if (onoff_l9 == SNS_PWR_OFF) {
		sensor_power_2p85_cnt--;
		if (regulator_is_enabled(vsensor_2p85)) {
			ret = regulator_disable(vsensor_2p85);
			if (ret)
				pr_err("%s: error vsensor_2p85 enabling regulator\n",
					__func__);
		}
	}

	if (onoff_lvs2 == SNS_PWR_ON) {
		sensor_power_1p8_cnt++;
		ret = regulator_enable(vsensor_1p8);
		if (ret)
			pr_err("%s: error enabling regulator\n", __func__);
	} else if (onoff_lvs2 == SNS_PWR_OFF) {
		sensor_power_1p8_cnt--;
		if (regulator_is_enabled(vsensor_1p8)) {
			ret = regulator_disable(vsensor_1p8);
			if (ret)
				pr_err("%s: error vsensor_1p8 enabling regulator\n",
					__func__);
		}
	}
}
#endif
#ifdef CONFIG_SENSORS_GP2A030



static void gp2a_power_on(int on);
static int gp2a_led_onoff(int on);
static int gp2a_get_threshold(void);

static struct gp2a_platform_data gp2a_platform_data = {
	.power_on = gp2a_power_on,
	.gp2a_led_on = gp2a_led_onoff,
	.gp2a_get_threshold = gp2a_get_threshold,
	.p_out = GPIO_RGB_SENSOR_INT,
	//.power_gpio = GPIO_PROX_LDO_EN,
};


static int gp2a_get_threshold(void)
{
	int new_threshold = 7; /* LTH value */

//	new_threshold = 100;

	return new_threshold;
}


static struct regulator *v_lsensor_2p85;
static struct regulator *v_psensor_2p85;

static void gp2a_power_on(int on)
{
	int onoff = on ? SNS_PWR_ON : SNS_PWR_OFF;
	int ret;

	if ( v_lsensor_2p85 == NULL) {
		v_lsensor_2p85 = regulator_get(NULL, "8917_l16");
		if (IS_ERR(v_lsensor_2p85))
			return ;

		ret = regulator_set_voltage(v_lsensor_2p85,
				2850000, 2850000);
		if (ret)
			pr_err("%s: error v_lsensor_2p85 setting voltage ret=%d\n",
					__func__, ret);
	}

	if (onoff == SNS_PWR_ON) {
		ret = regulator_enable(v_lsensor_2p85);
		if (ret)
			pr_err("%s: error enablinig regulator\n",
					__func__);
	} else if (onoff == SNS_PWR_OFF) {
		if (regulator_is_enabled(v_lsensor_2p85)) {
			ret = regulator_disable(v_lsensor_2p85);
			if (ret)
				pr_err("%s: error v_lsensor_2p85 enabling regulator\n",
						__func__);
		}
	}
	return ;
}


static int gp2a_led_onoff(int on)
{
int onoff = on ? SNS_PWR_ON : SNS_PWR_OFF;
int ret;

if (v_psensor_2p85 == NULL) {
	v_psensor_2p85 = regulator_get(NULL, "8917_l16");
	if (IS_ERR(v_psensor_2p85))
		return 1;

	ret = regulator_set_voltage(v_psensor_2p85,
			2850000,2850000 );
	if (ret)
		pr_err("%s: error v_psensor_2p85 setting voltage ret=%d\n",
				__func__, ret);
}

if (onoff == SNS_PWR_ON) {
	ret = regulator_enable(v_psensor_2p85);
	if (ret)
		pr_err("%s: error enablinig regulator\n",
				__func__);
} else if (onoff == SNS_PWR_OFF) {
	if (regulator_is_enabled(v_psensor_2p85)) {
		ret = regulator_disable(v_psensor_2p85);
		if (ret)
			pr_err("%s: error vsensor_3p0 enabling regulator\n",
					__func__);
	}
}
return 1;

}


static struct i2c_board_info opt_i2c_board_info[] = {
	{
		I2C_BOARD_INFO("gp2a", 0x39),
		.platform_data = &gp2a_platform_data,
	},
};


static void opt_init(void)
{
	int ret = 0;
	int prox_int = GPIO_RGB_SENSOR_INT;

	gpio_tlmm_config(GPIO_CFG(prox_int, 0, GPIO_CFG_INPUT,
				GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);

	ret = gpio_request(prox_int, "PSVOUT");
	if (ret) {
		pr_err("%s gpio request %d err\n", __func__, prox_int);
		return;
	} else {
		gpio_direction_input(prox_int);
		gpio_free(prox_int);
	}
}


static struct platform_device opt_gp2a = {
	.name = "gp2a-opt",
	.id = -1,
	.dev = {
		.platform_data = &gp2a_platform_data,
	},
};
#endif

#ifdef CONFIG_OPTICAL_TAOS_TRITON
static void taos_power_on(bool on);
static void taos_led_onoff(bool on);

static struct taos_platform_data taos_pdata = {
/*	.power = taos_power_on,*/
	.led_on = taos_led_onoff,
	.als_int = GPIO_RGB_SENSOR_INT,
	.prox_thresh_hi = 590,
	.prox_thresh_low = 460,
	.prox_th_hi_cal = 480,
	.prox_th_low_cal = 380,
	.als_time = 0xED,
	.intr_filter = 0x33,
	.prox_pulsecnt = 0x08,
	.prox_gain = 0x28,
	.coef_atime = 50,
	.ga = 98,
	.coef_a = 1000,
	.coef_b = 1910,
	.coef_c = 714,
	.coef_d = 1288,
};

static struct i2c_board_info opt_i2c_board_info[] = {
	{
		I2C_BOARD_INFO("taos", 0x39),
		.platform_data = &taos_pdata,
	},
};

static void opt_init(void)
{
	int ret = 0;

	gpio_tlmm_config(GPIO_CFG(GPIO_RGB_SENSOR_INT, 0, GPIO_CFG_INPUT,
				GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);

	gpio_tlmm_config(GPIO_CFG(GPIO_SENSOR_ALS_SDA, 0, GPIO_CFG_INPUT,
		GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);
	gpio_tlmm_config(GPIO_CFG(GPIO_SENSOR_ALS_SCL, 0, GPIO_CFG_INPUT,
		GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);

	ret = gpio_request(GPIO_RGB_SENSOR_INT, "PSVOUT");
	if (ret) {
		pr_err("%s gpio request %d err\n", __func__,
				GPIO_RGB_SENSOR_INT);
		return;
	} else {
		gpio_direction_input(GPIO_RGB_SENSOR_INT);
		gpio_free(GPIO_RGB_SENSOR_INT);
	}

	ret = gpio_request(GPIO_LEDA_EN, "leda_en");
	if (ret) {
		pr_err("%s gpio request %d err\n", __func__, GPIO_LEDA_EN);
		return;
	}
	gpio_tlmm_config(GPIO_CFG(GPIO_LEDA_EN, 0, GPIO_CFG_OUTPUT,
				GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);
	gpio_free(GPIO_LEDA_EN);
}

static void taos_power_on(bool on)
{
	int onoff = on ? SNS_PWR_ON : SNS_PWR_OFF;
	sensor_power_on_vdd(onoff, SNS_PWR_KEEP);
	if (onoff == SNS_PWR_ON)
		msleep(20);
}

static void taos_led_onoff(bool on)
{
	int onoff = on ? SNS_PWR_ON : SNS_PWR_OFF;
	static int prev_on = -1;
	int ret;

	if (prev_on == onoff)
		return;
	ret = gpio_request(GPIO_LEDA_EN, "leda_en");
	if (ret) {
		pr_err("%s: unable to request leda_en [%d]\n",
				__func__, GPIO_LEDA_EN);
		return;
	}
	ret = gpio_direction_output(GPIO_LEDA_EN, onoff);
	if (ret) {
		pr_err("%s: unable to set_direction for leda_en [%d]\n",
				__func__, GPIO_LEDA_EN);
		goto err_irq_gpio_leda_req;
	}
	prev_on = onoff;

err_irq_gpio_leda_req:
	gpio_free(GPIO_LEDA_EN);
}
#endif
#ifdef CONFIG_OPTICAL_TAOS_TMD3782
static void taos_power_on(bool on);
static void taos_led_onoff(bool on);

static struct taos_platform_data taos_pdata = {
	.power = taos_power_on,
	.led_on = taos_led_onoff,
	.als_int = GPIO_RGB_SENSOR_INT,
	.prox_thresh_hi = BLACKWINDOW_HI_THRESHOLD,
	.prox_thresh_low = BLACKWINDOW_LOW_THRESHOLD,
	.prox_th_hi_cal = BLACKWINDOW_HI_THRESHOLD,
	.prox_th_low_cal = BLACKWINDOW_LOW_THRESHOLD,
	.als_time = 0xEB,
	.intr_filter = 0x23,
	.prox_pulsecnt = 0x08,
	.als_gain = 0x21,
	.coef_atime = 50,
	.ga = 118,
	.coef_a = 1000,
	.coef_b = 1850,
	.coef_c = 510,
	.coef_d = 840,
};

static struct i2c_board_info opt_i2c_board_info[] = {
	{
		I2C_BOARD_INFO("taos", 0x39),
		.platform_data = &taos_pdata,
	},
};

static void opt_init(void)
{
	int ret = 0;
	int prox_int = GPIO_RGB_SENSOR_INT;

	gpio_tlmm_config(GPIO_CFG(prox_int, 0, GPIO_CFG_INPUT,
				GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);

	ret = gpio_request(prox_int, "PSVOUT");
	if (ret) {
		pr_err("%s gpio request %d err\n", __func__, prox_int);
		return;
	} else {
		gpio_direction_input(prox_int);
		gpio_free(prox_int);
	}
}

static void taos_power_on(bool on)
{
	int onoff = on ? SNS_PWR_ON : SNS_PWR_OFF;
	sensor_power_on_vdd(onoff, SNS_PWR_KEEP);
//	if (onoff == SNS_PWR_ON)
//		msleep(20);
}

static struct regulator *vsensor_3p0;
static void taos_led_onoff(bool on)
{
	int onoff = on ? SNS_PWR_ON : SNS_PWR_OFF;
	int ret;

	if (vsensor_3p0 == NULL) {
		vsensor_3p0 = regulator_get(NULL, "8917_l16");
		if (IS_ERR(vsensor_3p0))
			return ;

		ret = regulator_set_voltage(vsensor_3p0,
				3000000, 3000000);
		if (ret)
			pr_err("%s: error vsensor_3p00 setting voltage ret=%d\n",
					__func__, ret);
	}

	if (onoff == SNS_PWR_ON) {
		ret = regulator_enable(vsensor_3p0);
		if (ret)
			pr_err("%s: error enablinig regulator\n",
					__func__);
	} else if (onoff == SNS_PWR_OFF) {
		if (regulator_is_enabled(vsensor_3p0)) {
			ret = regulator_disable(vsensor_3p0);
			if (ret)
				pr_err("%s: error vsensor_3p0 enabling regulator\n",
						__func__);
		}
	}
	return;
}

#endif

#ifdef CONFIG_BCM2079X_NFC_I2C

static int __init bcm2079x_init(void)
{
	gpio_tlmm_config(GPIO_CFG(GPIO_NFC_IRQ, 0, GPIO_CFG_INPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), 1);
	gpio_tlmm_config(GPIO_CFG(GPIO_NFC_EN, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), 1);
	gpio_tlmm_config(GPIO_CFG(GPIO_NFC_FIRMWARE, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);

	gpio_tlmm_config(GPIO_CFG(GPIO_NFC_SDA_1_8V, 0, GPIO_CFG_INPUT,
		GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);
	gpio_tlmm_config(GPIO_CFG(GPIO_NFC_SCL_1_8V, 0, GPIO_CFG_INPUT,
		GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);

	pr_info("[NFC] %s done\n", __func__);
	return 0;
}


static struct i2c_gpio_platform_data bcm2079x_i2c_gpio_data = {
	.sda_pin = GPIO_NFC_SDA_1_8V,
	.scl_pin = GPIO_NFC_SCL_1_8V,
};

static struct platform_device bcm2079x_i2c_gpio_device = {
	.name = "i2c-gpio",
	.id = MSM_NFC_I2C_BUS_ID,
	.dev = {
		.platform_data  = &bcm2079x_i2c_gpio_data,
	},
};

static struct bcm2079x_platform_data bcm2079x_i2c_pdata = {
	.irq_gpio = GPIO_NFC_IRQ,
	.en_gpio = GPIO_NFC_EN,
	.wake_gpio = GPIO_NFC_FIRMWARE,
};

static struct i2c_board_info nfc_bcm2079x_info[] __initdata = {
	{
		I2C_BOARD_INFO("bcm2079x-i2c", 0x77),
		.irq = GPIO_NFC_IRQ,
		.platform_data = &bcm2079x_i2c_pdata,
	},
};
#endif /* CONFIG_BCM2079X_NFC_I2C */

struct class *sec_class;
EXPORT_SYMBOL(sec_class);

static void samsung_sys_class_init(void)
{
	pr_info("samsung sys class init.\n");

	sec_class = class_create(THIS_MODULE, "sec");

	if (IS_ERR(sec_class)) {
		pr_err("Failed to create class(sec)!\n");
		return;
	}

	pr_info("samsung sys class end.\n");
};

#ifdef CONFIG_ANDROID_PMEM
static unsigned pmem_size = MSM_PMEM_SIZE;
static int __init pmem_size_setup(char *p)
{
	pmem_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_size", pmem_size_setup);

static unsigned pmem_adsp_size = MSM_PMEM_ADSP_SIZE;

static int __init pmem_adsp_size_setup(char *p)
{
	pmem_adsp_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_adsp_size", pmem_adsp_size_setup);

static unsigned pmem_audio_size = MSM_PMEM_AUDIO_SIZE;

static int __init pmem_audio_size_setup(char *p)
{
	pmem_audio_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_audio_size", pmem_audio_size_setup);
#endif

#ifdef CONFIG_ANDROID_PMEM
#ifndef CONFIG_MSM_MULTIMEDIA_USE_ION
static struct android_pmem_platform_data android_pmem_pdata = {
	.name = "pmem",
	.allocator_type = PMEM_ALLOCATORTYPE_ALLORNOTHING,
	.cached = 1,
	.memory_type = MEMTYPE_EBI1,
};

static struct platform_device msm8930_android_pmem_device = {
	.name = "android_pmem",
	.id = 0,
	.dev = {.platform_data = &android_pmem_pdata},
};

static struct android_pmem_platform_data android_pmem_adsp_pdata = {
	.name = "pmem_adsp",
	.allocator_type = PMEM_ALLOCATORTYPE_BITMAP,
	.cached = 0,
	.memory_type = MEMTYPE_EBI1,
};
static struct platform_device msm8930_android_pmem_adsp_device = {
	.name = "android_pmem",
	.id = 2,
	.dev = { .platform_data = &android_pmem_adsp_pdata },
};

static struct android_pmem_platform_data android_pmem_audio_pdata = {
	.name = "pmem_audio",
	.allocator_type = PMEM_ALLOCATORTYPE_BITMAP,
	.cached = 0,
	.memory_type = MEMTYPE_EBI1,
};

static struct platform_device msm8930_android_pmem_audio_device = {
	.name = "android_pmem",
	.id = 4,
	.dev = { .platform_data = &android_pmem_audio_pdata },
};
#endif /* CONFIG_MSM_MULTIMEDIA_USE_ION */
#endif /* CONFIG_ANDROID_PMEM */

struct fmem_platform_data msm8930_fmem_pdata = {
};

#define DSP_RAM_BASE_8960 0x8da00000
#define DSP_RAM_SIZE_8960 0x1800000
static int dspcrashd_pdata_8960 = 0xDEADDEAD;

static struct resource resources_dspcrashd_8960[] = {
	{
		.name   = "msm_dspcrashd",
		.start  = DSP_RAM_BASE_8960,
		.end    = DSP_RAM_BASE_8960 + DSP_RAM_SIZE_8960,
		.flags  = IORESOURCE_DMA,
	},
};

static struct platform_device msm_device_dspcrashd_8960 = {
	.name           = "msm_dspcrashd",
	.num_resources  = ARRAY_SIZE(resources_dspcrashd_8960),
	.resource       = resources_dspcrashd_8960,
	.dev = { .platform_data = &dspcrashd_pdata_8960 },
};

static struct memtype_reserve msm8930_reserve_table[] __initdata = {
	[MEMTYPE_SMI] = {
	},
	[MEMTYPE_EBI0] = {
		.flags	=	MEMTYPE_FLAGS_1M_ALIGN,
	},
	[MEMTYPE_EBI1] = {
		.flags	=	MEMTYPE_FLAGS_1M_ALIGN,
	},
};


static void __init reserve_rtb_memory(void)
{
#if defined(CONFIG_MSM_RTB)
	msm8930_reserve_table[MEMTYPE_EBI1].size += msm8930_rtb_pdata.size;
#endif
}

static void __init size_pmem_devices(void)
{
#ifdef CONFIG_ANDROID_PMEM
#ifndef CONFIG_MSM_MULTIMEDIA_USE_ION
	android_pmem_adsp_pdata.size = pmem_adsp_size;
	android_pmem_pdata.size = pmem_size;
	android_pmem_audio_pdata.size = MSM_PMEM_AUDIO_SIZE;
#endif /*CONFIG_MSM_MULTIMEDIA_USE_ION*/
#endif /*CONFIG_ANDROID_PMEM*/
}

#ifdef CONFIG_ANDROID_PMEM
#ifndef CONFIG_MSM_MULTIMEDIA_USE_ION
static void __init reserve_memory_for(struct android_pmem_platform_data *p)
{
	msm8930_reserve_table[p->memory_type].size += p->size;
}
#endif /*CONFIG_MSM_MULTIMEDIA_USE_ION*/
#endif /*CONFIG_ANDROID_PMEM*/

static void __init reserve_pmem_memory(void)
{
#ifdef CONFIG_ANDROID_PMEM
#ifndef CONFIG_MSM_MULTIMEDIA_USE_ION
	reserve_memory_for(&android_pmem_adsp_pdata);
	reserve_memory_for(&android_pmem_pdata);
	reserve_memory_for(&android_pmem_audio_pdata);
#endif /*CONFIG_MSM_MULTIMEDIA_USE_ION*/
#endif /*CONFIG_ANDROID_PMEM*/
}

static int msm8930_paddr_to_memtype(unsigned int paddr)
{
	return MEMTYPE_EBI1;
}

#define FMEM_ENABLED 0
#ifdef CONFIG_ION_MSM
#ifdef CONFIG_MSM_MULTIMEDIA_USE_ION
static struct ion_cp_heap_pdata cp_mm_msm8930_ion_pdata = {
	.permission_type = IPT_TYPE_MM_CARVEOUT,
	.align = PAGE_SIZE,
	.reusable = FMEM_ENABLED,
	.mem_is_fmem = FMEM_ENABLED,
	.fixed_position = FIXED_MIDDLE,
	.is_cma	= 1,
	.no_nonsecure_alloc = 1,
};

static struct ion_cp_heap_pdata cp_mfc_msm8930_ion_pdata = {
	.permission_type = IPT_TYPE_MFC_SHAREDMEM,
	.align = PAGE_SIZE,
	.reusable = 0,
	.mem_is_fmem = FMEM_ENABLED,
	.fixed_position = FIXED_HIGH,
	.no_nonsecure_alloc = 1,
};

static struct ion_co_heap_pdata co_msm8930_ion_pdata = {
	.adjacent_mem_id = INVALID_HEAP_ID,
	.align = PAGE_SIZE,
	.mem_is_fmem = 0,
};

static struct ion_co_heap_pdata fw_co_msm8930_ion_pdata = {
	.adjacent_mem_id = ION_CP_MM_HEAP_ID,
	.align = SZ_128K,
	.mem_is_fmem = FMEM_ENABLED,
	.fixed_position = FIXED_LOW,
};
#endif


static u64 msm_dmamask = DMA_BIT_MASK(32);

static struct platform_device ion_mm_heap_device = {
	.name = "ion-mm-heap-device",
	.id = -1,
	.dev = {
		.dma_mask = &msm_dmamask,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	}
};

/**
 * These heaps are listed in the order they will be allocated. Due to
 * video hardware restrictions and content protection the FW heap has to
 * be allocated adjacent (below) the MM heap and the MFC heap has to be
 * allocated after the MM heap to ensure MFC heap is not more than 256MB
 * away from the base address of the FW heap.
 * However, the order of FW heap and MM heap doesn't matter since these
 * two heaps are taken care of by separate code to ensure they are adjacent
 * to each other.
 * Don't swap the order unless you know what you are doing!
 */
struct ion_platform_heap msm8930_heaps[] = {
		{
			.id	= ION_SYSTEM_HEAP_ID,
			.type	= ION_HEAP_TYPE_SYSTEM,
			.name	= ION_VMALLOC_HEAP_NAME,
		},
#ifdef CONFIG_MSM_MULTIMEDIA_USE_ION
		{
			.id	= ION_CP_MM_HEAP_ID,
			.type	= ION_HEAP_TYPE_CP,
			.name	= ION_MM_HEAP_NAME,
			.size	= MSM_ION_MM_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = (void *) &cp_mm_msm8930_ion_pdata,
			.priv	= &ion_mm_heap_device.dev
		},
		{
			.id	= ION_MM_FIRMWARE_HEAP_ID,
			.type	= ION_HEAP_TYPE_CARVEOUT,
			.name	= ION_MM_FIRMWARE_HEAP_NAME,
			.size	= MSM_ION_MM_FW_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = (void *) &fw_co_msm8930_ion_pdata,
		},
		{
			.id	= ION_CP_MFC_HEAP_ID,
			.type	= ION_HEAP_TYPE_CP,
			.name	= ION_MFC_HEAP_NAME,
			.size	= MSM_ION_MFC_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = (void *) &cp_mfc_msm8930_ion_pdata,
		},
#ifndef CONFIG_MSM_IOMMU
		{
			.id	= ION_SF_HEAP_ID,
			.type	= ION_HEAP_TYPE_CARVEOUT,
			.name	= ION_SF_HEAP_NAME,
			.size	= MSM_ION_SF_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = (void *) &co_msm8930_ion_pdata,
		},
#endif
		{
			.id	= ION_IOMMU_HEAP_ID,
			.type	= ION_HEAP_TYPE_IOMMU,
			.name	= ION_IOMMU_HEAP_NAME,
		},
		{
			.id	= ION_QSECOM_HEAP_ID,
			.type	= ION_HEAP_TYPE_CARVEOUT,
			.name	= ION_QSECOM_HEAP_NAME,
			.size	= MSM_ION_QSECOM_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = (void *) &co_msm8930_ion_pdata,
		},
		{
			.id	= ION_AUDIO_HEAP_ID,
			.type	= ION_HEAP_TYPE_CARVEOUT,
			.name	= ION_AUDIO_HEAP_NAME,
			.size	= MSM_ION_AUDIO_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = (void *) &co_msm8930_ion_pdata,
		},
		{
			.id	= ION_ADSP_HEAP_ID,
			.type	= ION_HEAP_TYPE_CARVEOUT,
			.name	= ION_ADSP_HEAP_NAME,
			.size	= MSM_ION_ADSP_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = (void *) &co_msm8930_ion_pdata,
		},
#endif
};

static struct ion_platform_data msm8930_ion_pdata = {
	.nr = MSM_ION_HEAP_NUM,
	.heaps = msm8930_heaps,

};

static struct platform_device msm8930_ion_dev = {
	.name = "ion-msm",
	.id = 1,
	.dev = { .platform_data = &msm8930_ion_pdata },
};
#endif

struct platform_device msm8930_fmem_device = {
	.name = "fmem",
	.id = 1,
	.dev = { .platform_data = &msm8930_fmem_pdata },
};

static void __init reserve_mem_for_ion(enum ion_memory_types mem_type,
				      unsigned long size)
{
	msm8930_reserve_table[mem_type].size += size;
}

#ifdef CONFIG_ANDROID_RAM_CONSOLE
static struct ram_console_platform_data ram_console_pdata = {
	.bootinfo = NULL,
};

static struct platform_device ram_console_device = {
	.name = "ram_console",
	.id = -1,
	.dev    = {
		.platform_data = &ram_console_pdata,
	},
};
#endif

#ifdef CONFIG_ANDROID_PERSISTENT_RAM
#define RAMCONSOLE_PHYS_ADDR 0x8FE00000
static struct persistent_ram_descriptor per_ram_descs[] __initdata = {
	{
		.name = "ram_console",
		.size = SZ_1M,
	}
};

static struct persistent_ram per_ram __initdata = {
	.descs = per_ram_descs,
	.num_descs = ARRAY_SIZE(per_ram_descs),
	.start = RAMCONSOLE_PHYS_ADDR,
	.size = SZ_1M
};
#endif

static void __init msm8930_reserve_fixed_area(unsigned long fixed_area_size)
{
#if defined(CONFIG_ION_MSM) && defined(CONFIG_MSM_MULTIMEDIA_USE_ION)
	int ret;

	if (fixed_area_size > MAX_FIXED_AREA_SIZE)
		panic("fixed area size is larger than %dM\n",
			MAX_FIXED_AREA_SIZE >> 20);

	reserve_info->fixed_area_size = fixed_area_size;
	reserve_info->fixed_area_start = MSM8930_FW_START;

	ret = memblock_remove(reserve_info->fixed_area_start,
		reserve_info->fixed_area_size);
	BUG_ON(ret);
#endif
}

/**
 * Reserve memory for ION and calculate amount of reusable memory for fmem.
 * We only reserve memory for heaps that are not reusable. However, we only
 * support one reusable heap at the moment so we ignore the reusable flag for
 * other than the first heap with reusable flag set. Also handle special case
 * for video heaps (MM,FW, and MFC). Video requires heaps MM and MFC to be
 * at a higher address than FW in addition to not more than 256MB away from the
 * base address of the firmware. This means that if MM is reusable the other
 * two heaps must be allocated in the same region as FW. This is handled by the
 * mem_is_fmem flag in the platform data. In addition the MM heap must be
 * adjacent to the FW heap for content protection purposes.
 */
static void __init reserve_ion_memory(void)
{
#if defined(CONFIG_ION_MSM) && defined(CONFIG_MSM_MULTIMEDIA_USE_ION)
	unsigned int i;
	int ret;
	unsigned int fixed_size = 0;
	unsigned int fixed_low_size, fixed_middle_size, fixed_high_size;
	unsigned long fixed_low_start, fixed_middle_start, fixed_high_start;
	unsigned long cma_alignment;
	unsigned int low_use_cma = 0;
	unsigned int middle_use_cma = 0;
	unsigned int high_use_cma = 0;

	fixed_low_size = 0;
	fixed_middle_size = 0;
	fixed_high_size = 0;

	cma_alignment = PAGE_SIZE << max(MAX_ORDER, pageblock_order);

	for (i = 0; i < msm8930_ion_pdata.nr; ++i) {
		struct ion_platform_heap *heap =
						&(msm8930_ion_pdata.heaps[i]);

		int use_cma = 0;

		if (heap->extra_data) {
			int fixed_position = NOT_FIXED;

			switch ((int) heap->type) {
			case ION_HEAP_TYPE_CP:
				if (((struct ion_cp_heap_pdata *)
					heap->extra_data)->is_cma) {
					heap->size = ALIGN(heap->size,
						cma_alignment);
						use_cma = 1;
				}
				fixed_position = ((struct ion_cp_heap_pdata *)
					heap->extra_data)->fixed_position;
				break;
			case ION_HEAP_TYPE_DMA:
				use_cma = 1;
				/* Purposely fall through here */
			case ION_HEAP_TYPE_CARVEOUT:
				fixed_position = ((struct ion_co_heap_pdata *)
					heap->extra_data)->fixed_position;
				break;
			default:
				break;
			}

			if (fixed_position != NOT_FIXED)
				fixed_size += heap->size;
			else
				reserve_mem_for_ion(MEMTYPE_EBI1, heap->size);

			if (fixed_position == FIXED_LOW) {
				fixed_low_size += heap->size;
				low_use_cma = use_cma;
			} else if (fixed_position == FIXED_MIDDLE) {
				fixed_middle_size += heap->size;
				middle_use_cma = use_cma;
			} else if (fixed_position == FIXED_HIGH) {
				fixed_high_size += heap->size;
				high_use_cma = use_cma;
			} else if (use_cma) {
				/*
				 * Heaps that use CMA but are not part of the
				 * fixed set. Create wherever.
				 */
				dma_declare_contiguous(
					heap->priv,
					heap->size,
					0,
					0xb0000000);
			}
		}
	}

	if (!fixed_size)
		return;
	/*
	 * Given the setup for the fixed area, we can't round up all sizes.
	 * Some sizes must be set up exactly and aligned correctly. Incorrect
	 * alignments are considered a configuration issue
	 */

	fixed_low_start = MSM8930_FIXED_AREA_START;
	if (low_use_cma) {
		BUG_ON(!IS_ALIGNED(fixed_low_size + HOLE_SIZE, cma_alignment));
		BUG_ON(!IS_ALIGNED(fixed_low_start, cma_alignment));
	} else {
		BUG_ON(!IS_ALIGNED(fixed_low_size + HOLE_SIZE, SECTION_SIZE));
		ret = memblock_remove(fixed_low_start,
				      fixed_low_size + HOLE_SIZE);
		BUG_ON(ret);
	}

	fixed_middle_start = fixed_low_start + fixed_low_size + HOLE_SIZE;
	if (middle_use_cma) {
		BUG_ON(!IS_ALIGNED(fixed_middle_start, cma_alignment));
		BUG_ON(!IS_ALIGNED(fixed_middle_size, cma_alignment));
	} else {
		BUG_ON(!IS_ALIGNED(fixed_middle_size, SECTION_SIZE));
		ret = memblock_remove(fixed_middle_start, fixed_middle_size);
		BUG_ON(ret);
	}

	fixed_high_start = fixed_middle_start + fixed_middle_size;
	if (high_use_cma) {
		fixed_high_size = ALIGN(fixed_high_size, cma_alignment);
		BUG_ON(!IS_ALIGNED(fixed_high_start, cma_alignment));
	} else {
		/* This is the end of the fixed area so it's okay to round up */
		fixed_high_size = ALIGN(fixed_high_size, SECTION_SIZE);
		ret = memblock_remove(fixed_high_start, fixed_high_size);
		BUG_ON(ret);
	}

	for (i = 0; i < msm8930_ion_pdata.nr; ++i) {
		struct ion_platform_heap *heap = &(msm8930_ion_pdata.heaps[i]);

		if (heap->extra_data) {
			int fixed_position = NOT_FIXED;
			struct ion_cp_heap_pdata *pdata = NULL;

			switch ((int) heap->type) {
			case ION_HEAP_TYPE_CP:
				pdata =
				(struct ion_cp_heap_pdata *)heap->extra_data;
				fixed_position = pdata->fixed_position;
				break;
			case ION_HEAP_TYPE_DMA:
			case ION_HEAP_TYPE_CARVEOUT:
				fixed_position = ((struct ion_co_heap_pdata *)
					heap->extra_data)->fixed_position;
				break;
			default:
				break;
			}

			switch (fixed_position) {
			case FIXED_LOW:
				heap->base = fixed_low_start;
				break;
			case FIXED_MIDDLE:
				heap->base = fixed_middle_start;
				if (middle_use_cma)
					dma_declare_contiguous(
						&ion_mm_heap_device.dev,
						heap->size,
						fixed_middle_start,
						0xa0000000);
				pdata->secure_base = fixed_middle_start
							- HOLE_SIZE;
				pdata->secure_size = HOLE_SIZE + heap->size;
				break;
			case FIXED_HIGH:
				heap->base = fixed_high_start;
				break;
			default:
				break;
			}
		}
	}
#endif
}

static void __init reserve_mdp_memory(void)
{
	msm8930_mdp_writeback(msm8930_reserve_table);
}

#ifdef CONFIG_MSM_CACHE_DUMP
static void __init reserve_cache_dump_memory(void)
{
	unsigned int total;

	total = msm8930_cache_dump_pdata.l1_size +
		msm8930_cache_dump_pdata.l2_size;
	msm8930_reserve_table[MEMTYPE_EBI1].size += total;
}
#else
static void __init reserve_cache_dump_memory(void) { }
#endif

static void __init msm8930_calculate_reserve_sizes(void)
{
	size_pmem_devices();
	reserve_pmem_memory();
	reserve_ion_memory();
	reserve_mdp_memory();
	reserve_rtb_memory();
	reserve_cache_dump_memory();
}

static struct reserve_info msm8930_reserve_info __initdata = {
	.memtype_reserve_table = msm8930_reserve_table,
	.calculate_reserve_sizes = msm8930_calculate_reserve_sizes,
	.reserve_fixed_area = msm8930_reserve_fixed_area,
	.paddr_to_memtype = msm8930_paddr_to_memtype,
};

static void __init msm8930_early_memory(void)
{
	reserve_info = &msm8930_reserve_info;
}

static void __init msm8930_reserve(void)
{
	msm_reserve();
#ifdef CONFIG_ANDROID_PERSISTENT_RAM
	persistent_ram_early_init(&per_ram);
#endif
}

static void __init msm8930_allocate_memory_regions(void)
{
	msm8930_allocate_fb_region();
}

#ifdef CONFIG_WCD9304_CODEC

#define SITAR_INTERRUPT_BASE (NR_MSM_IRQS + NR_GPIO_IRQS + NR_PM8921_IRQS)

/* Micbias setting is based on 8660 CDP/MTP/FLUID requirement
 * 4 micbiases are used to power various analog and digital
 * microphones operating at 1800 mV. Technically, all micbiases
 * can source from single cfilter since all microphones operate
 * at the same voltage level. The arrangement below is to make
 * sure all cfilters are exercised. LDO_H regulator ouput level
 * does not need to be as high as 2.85V. It is choosen for
 * microphone sensitivity purpose.
 */
#ifndef CONFIG_SLIMBUS_MSM_CTRL
static struct wcd9xxx_pdata sitar_i2c_platform_data = {
	.irq = MSM_GPIO_TO_INT(GPIO_CODEC_INT),
	.irq_base = SITAR_INTERRUPT_BASE,
	.num_irqs = NR_WCD9XXX_IRQS,
	.reset_gpio = GPIO_PM_WCD9304_RESET,
	.micbias = {
		.ldoh_v = SITAR_LDOH_1P95_V,
		.cfilt1_mv = 1800,
		.cfilt2_mv = 1800,
		.bias1_cfilt_sel = SITAR_CFILT1_SEL,
		.bias2_cfilt_sel = SITAR_CFILT2_SEL,
	},
	.regulator = {
	{
		.name = "CDC_VDD_CP",
		.min_uV = 1800000,
		.max_uV = 1800000,
		.optimum_uA = WCD9XXX_CDC_VDDA_CP_CUR_MAX,
	},
	{
		.name = "CDC_VDDA_RX",
		.min_uV = 1800000,
		.max_uV = 1800000,
		.optimum_uA = WCD9XXX_CDC_VDDA_RX_CUR_MAX,
	},
	{
		.name = "CDC_VDDA_TX",
		.min_uV = 1800000,
		.max_uV = 1800000,
		.optimum_uA = WCD9XXX_CDC_VDDA_TX_CUR_MAX,
	},
	{
		.name = "VDDIO_CDC",
		.min_uV = 1800000,
		.max_uV = 1800000,
		.optimum_uA = WCD9XXX_VDDIO_CDC_CUR_MAX,
	},
	{
		.name = "VDDD_CDC_D",
		.min_uV = 1250000,
		.max_uV = 1250000,
		.optimum_uA = WCD9XXX_VDDD_CDC_D_CUR_MAX,
	},
	{
		.name = "CDC_VDDA_A_1P2V",
		.min_uV = 1250000,
		.max_uV = 1250000,
		.optimum_uA = WCD9XXX_VDDD_CDC_A_CUR_MAX,
	},
	},

};
#else
static struct wcd9xxx_pdata sitar_platform_data = {
		.slimbus_slave_device = {
		.name = "sitar-slave",
		.e_addr = {0, 0, 0x00, 0, 0x17, 2},
	},
	.irq = MSM_GPIO_TO_INT(62),
	.irq_base = SITAR_INTERRUPT_BASE,
	.num_irqs = NR_WCD9XXX_IRQS,
	.reset_gpio = 42,
#if defined(CONFIG_WCD9304_USE_MI2S_CLK_9600)
	.mclk_rate = 9600000,
#endif
	.micbias = {
		.ldoh_v = SITAR_LDOH_2P85_V,
		.cfilt1_mv = 1800,
		.cfilt2_mv = 1800,
		.bias1_cfilt_sel = SITAR_CFILT1_SEL,
		.bias2_cfilt_sel = SITAR_CFILT2_SEL,
		.bias1_cap_mode = MICBIAS_EXT_BYP_CAP,
		.bias2_cap_mode = MICBIAS_NO_EXT_BYP_CAP,
	},
	.regulator = {
	{
		.name = "CDC_VDD_CP",
		.min_uV = 2200000,
		.max_uV = 2200000,
		.optimum_uA = WCD9XXX_CDC_VDDA_CP_CUR_MAX,
	},
	{
		.name = "CDC_VDDA_RX",
		.min_uV = 1800000,
		.max_uV = 1800000,
		.optimum_uA = WCD9XXX_CDC_VDDA_RX_CUR_MAX,
	},
	{
		.name = "CDC_VDDA_TX",
		.min_uV = 1800000,
		.max_uV = 1800000,
		.optimum_uA = WCD9XXX_CDC_VDDA_TX_CUR_MAX,
	},
	{
		.name = "VDDIO_CDC",
		.min_uV = 1800000,
		.max_uV = 1800000,
		.optimum_uA = WCD9XXX_VDDIO_CDC_CUR_MAX,
	},
	{
		.name = "VDDD_CDC_D",
		.min_uV = 1200000,
		.max_uV = 1250000,
		.optimum_uA = WCD9XXX_VDDD_CDC_D_CUR_MAX,
	},
	{
		.name = "CDC_VDDA_A_1P2V",
		.min_uV = 1200000,
		.max_uV = 1250000,
		.optimum_uA = WCD9XXX_VDDD_CDC_A_CUR_MAX,
	},
	},
};
#endif

#ifdef CONFIG_SLIMBUS_MSM_CTRL
static struct slim_device msm_slim_sitar = {
	.name = "sitar-slim",
	.e_addr = {0, 1, 0x00, 0, 0x17, 2},
	.dev = {
	.platform_data = &sitar_platform_data,
	},
};

static struct wcd9xxx_pdata sitar1p1_platform_data = {
		.slimbus_slave_device = {
		.name = "sitar-slave",
		.e_addr = {0, 0, 0x70, 0, 0x17, 2},
	},
	.irq = MSM_GPIO_TO_INT(62),
	.irq_base = SITAR_INTERRUPT_BASE,
	.num_irqs = NR_WCD9XXX_IRQS,
	.reset_gpio = 42,
	.micbias = {
		.ldoh_v = SITAR_LDOH_2P85_V,
		.cfilt1_mv = 1800,
		.cfilt2_mv = 1800,
		.bias1_cfilt_sel = SITAR_CFILT1_SEL,
		.bias2_cfilt_sel = SITAR_CFILT2_SEL,
		.bias1_cap_mode = MICBIAS_EXT_BYP_CAP,
		.bias2_cap_mode = MICBIAS_NO_EXT_BYP_CAP,
	},
	.regulator = {
	{
		.name = "CDC_VDD_CP",
		.min_uV = 2200000,
		.max_uV = 2200000,
		.optimum_uA = WCD9XXX_CDC_VDDA_CP_CUR_MAX,
	},
	{
		.name = "CDC_VDDA_RX",
		.min_uV = 1800000,
		.max_uV = 1800000,
		.optimum_uA = WCD9XXX_CDC_VDDA_RX_CUR_MAX,
	},
	{
		.name = "CDC_VDDA_TX",
		.min_uV = 1800000,
		.max_uV = 1800000,
		.optimum_uA = WCD9XXX_CDC_VDDA_TX_CUR_MAX,
	},
	{
		.name = "VDDIO_CDC",
		.min_uV = 1800000,
		.max_uV = 1800000,
		.optimum_uA = WCD9XXX_VDDIO_CDC_CUR_MAX,
	},
	{
		.name = "VDDD_CDC_D",
		.min_uV = 1200000,
		.max_uV = 1250000,
		.optimum_uA = WCD9XXX_VDDD_CDC_D_CUR_MAX,
	},
	{
		.name = "CDC_VDDA_A_1P2V",
		.min_uV = 1200000,
		.max_uV = 1250000,
		.optimum_uA = WCD9XXX_VDDD_CDC_A_CUR_MAX,
	},
	},
};

static struct slim_device msm_slim_sitar1p1 = {
	.name = "sitar1p1-slim",
	.e_addr = {0, 1, 0x70, 0, 0x17, 2},
	.dev = {
	.platform_data = &sitar1p1_platform_data,
	},
};
#endif
#endif

#ifdef CONFIG_SLIMBUS_MSM_CTRL
static struct slim_boardinfo msm_slim_devices[] = {
#ifdef CONFIG_WCD9304_CODEC
	{
		.bus_num = 1,
		.slim_slave = &msm_slim_sitar,
	},
	{
		.bus_num = 1,
		.slim_slave = &msm_slim_sitar1p1,
	},
#endif
	/* add more slimbus slaves as needed */
};
#endif

#ifndef CONFIG_SLIMBUS_MSM_CTRL
#define SITAR_I2C_SLAVE_ADDR	0x0d
#define SITAR_ANALOG_I2C_SLAVE_ADDR	0x77
#define SITAR_DIGITAL1_I2C_SLAVE_ADDR	0x66
#define SITAR_DIGITAL2_I2C_SLAVE_ADDR	0x55

static struct i2c_board_info sitar_device_info[] __initdata = {
	{
		I2C_BOARD_INFO("sitar top level", SITAR_I2C_SLAVE_ADDR),
		.platform_data = &sitar_i2c_platform_data,
	},
	{
		I2C_BOARD_INFO("sitar analog", SITAR_ANALOG_I2C_SLAVE_ADDR),
		.platform_data = &sitar_i2c_platform_data,
	},
	{
		I2C_BOARD_INFO("sitar digital1", SITAR_DIGITAL1_I2C_SLAVE_ADDR),
		.platform_data = &sitar_i2c_platform_data,
	},
	{
		I2C_BOARD_INFO("sitar digital2", SITAR_DIGITAL2_I2C_SLAVE_ADDR),
		.platform_data = &sitar_i2c_platform_data,
	},
};
#endif

#define MSM_WCNSS_PHYS	0x03000000
#define MSM_WCNSS_SIZE	0x280000

static struct resource resources_wcnss_wlan[] = {
	{
		.start	= RIVA_APPS_WLAN_RX_DATA_AVAIL_IRQ,
		.end	= RIVA_APPS_WLAN_RX_DATA_AVAIL_IRQ,
		.name	= "wcnss_wlanrx_irq",
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= RIVA_APPS_WLAN_DATA_XFER_DONE_IRQ,
		.end	= RIVA_APPS_WLAN_DATA_XFER_DONE_IRQ,
		.name	= "wcnss_wlantx_irq",
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= MSM_WCNSS_PHYS,
		.end	= MSM_WCNSS_PHYS + MSM_WCNSS_SIZE - 1,
		.name	= "wcnss_mmio",
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= 84,
		.end	= 88,
		.name	= "wcnss_gpios_5wire",
		.flags	= IORESOURCE_IO,
	},
};

static struct qcom_wcnss_opts qcom_wcnss_pdata = {
	.has_48mhz_xo	= 1,
};

static struct platform_device msm_device_wcnss_wlan = {
	.name		= "wcnss_wlan",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(resources_wcnss_wlan),
	.resource	= resources_wcnss_wlan,
	.dev		= {.platform_data = &qcom_wcnss_pdata},
};



#ifdef CONFIG_QSEECOM
/* qseecom bus scaling */
static struct msm_bus_vectors qseecom_clks_init_vectors[] = {
	{
		.src = MSM_BUS_MASTER_SPS,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ib = 0,
		.ab = 0,
	},
	{
		.src = MSM_BUS_MASTER_SPS,
		.dst = MSM_BUS_SLAVE_SPS,
		.ib = 0,
		.ab = 0,
	},
	{
		.src = MSM_BUS_MASTER_SPDM,
		.dst = MSM_BUS_SLAVE_SPDM,
		.ib = 0,
		.ab = 0,
	},
};

static struct msm_bus_vectors qseecom_enable_dfab_vectors[] = {
	{
		.src = MSM_BUS_MASTER_SPS,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ib = (492 * 8) * 1000000UL,
		.ab = (492 * 8) *  100000UL,
	},
	{
		.src = MSM_BUS_MASTER_SPS,
		.dst = MSM_BUS_SLAVE_SPS,
		.ib = (492 * 8) * 1000000UL,
		.ab = (492 * 8) *  100000UL,
	},
	{
		.src = MSM_BUS_MASTER_SPDM,
		.dst = MSM_BUS_SLAVE_SPDM,
		.ib = 0,
		.ab = 0,
	},
};

static struct msm_bus_vectors qseecom_enable_sfpb_vectors[] = {
	{
		.src = MSM_BUS_MASTER_SPS,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ib = 0,
		.ab = 0,
	},
	{
		.src = MSM_BUS_MASTER_SPS,
		.dst = MSM_BUS_SLAVE_SPS,
		.ib = 0,
		.ab = 0,
	},
	{
		.src = MSM_BUS_MASTER_SPDM,
		.dst = MSM_BUS_SLAVE_SPDM,
		.ib = (64 * 8) * 1000000UL,
		.ab = (64 * 8) *  100000UL,
	},
};

static struct msm_bus_vectors qseecom_enable_dfab_sfpb_vectors[] = {
	{
		.src = MSM_BUS_MASTER_SPS,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ib = (492 * 8) * 1000000UL,
		.ab = (492 * 8) *  100000UL,
	},
	{
		.src = MSM_BUS_MASTER_SPS,
		.dst = MSM_BUS_SLAVE_SPS,
		.ib = (492 * 8) * 1000000UL,
		.ab = (492 * 8) *  100000UL,
	},
	{
		.src = MSM_BUS_MASTER_SPDM,
		.dst = MSM_BUS_SLAVE_SPDM,
		.ib = (64 * 8) * 1000000UL,
		.ab = (64 * 8) *  100000UL,
	},
};

static struct msm_bus_paths qseecom_hw_bus_scale_usecases[] = {
	{
		ARRAY_SIZE(qseecom_clks_init_vectors),
		qseecom_clks_init_vectors,
	},
	{
		ARRAY_SIZE(qseecom_enable_dfab_vectors),
		qseecom_enable_sfpb_vectors,
	},
	{
		ARRAY_SIZE(qseecom_enable_sfpb_vectors),
		qseecom_enable_sfpb_vectors,
	},
	{
		ARRAY_SIZE(qseecom_enable_dfab_sfpb_vectors),
		qseecom_enable_dfab_sfpb_vectors,
	},
};

static struct msm_bus_scale_pdata qseecom_bus_pdata = {
	qseecom_hw_bus_scale_usecases,
	ARRAY_SIZE(qseecom_hw_bus_scale_usecases),
	.name = "qsee",
};

static struct platform_device qseecom_device = {
	.name		= "qseecom",
	.id		= 0,
	.dev		= {
		.platform_data = &qseecom_bus_pdata,
	},
};
#endif

#if defined(CONFIG_CRYPTO_DEV_QCRYPTO) || \
		defined(CONFIG_CRYPTO_DEV_QCRYPTO_MODULE) || \
		defined(CONFIG_CRYPTO_DEV_QCEDEV) || \
		defined(CONFIG_CRYPTO_DEV_QCEDEV_MODULE)

#define QCE_SIZE		0x10000
#define QCE_0_BASE		0x18500000

#define QCE_HW_KEY_SUPPORT	0
#define QCE_SHA_HMAC_SUPPORT	1
#define QCE_SHARE_CE_RESOURCE	1
#define QCE_CE_SHARED		0

/* Begin Bus scaling definitions */
static struct msm_bus_vectors crypto_hw_init_vectors[] = {
	{
		.src = MSM_BUS_MASTER_ADM_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 0,
		.ib = 0,
	},
	{
		.src = MSM_BUS_MASTER_ADM_PORT1,
		.dst = MSM_BUS_SLAVE_GSBI1_UART,
		.ab = 0,
		.ib = 0,
	},
};

static struct msm_bus_vectors crypto_hw_active_vectors[] = {
	{
		.src = MSM_BUS_MASTER_ADM_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 70000000UL,
		.ib = 70000000UL,
	},
	{
		.src = MSM_BUS_MASTER_ADM_PORT1,
		.dst = MSM_BUS_SLAVE_GSBI1_UART,
		.ab = 2480000000UL,
		.ib = 2480000000UL,
	},
};

static struct msm_bus_paths crypto_hw_bus_scale_usecases[] = {
	{
		ARRAY_SIZE(crypto_hw_init_vectors),
		crypto_hw_init_vectors,
	},
	{
		ARRAY_SIZE(crypto_hw_active_vectors),
		crypto_hw_active_vectors,
	},
};

static struct msm_bus_scale_pdata crypto_hw_bus_scale_pdata = {
		crypto_hw_bus_scale_usecases,
		ARRAY_SIZE(crypto_hw_bus_scale_usecases),
		.name = "cryptohw",
};
/* End Bus Scaling Definitions*/

static struct resource qcrypto_resources[] = {
	[0] = {
		.start = QCE_0_BASE,
		.end = QCE_0_BASE + QCE_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.name = "crypto_channels",
		.start = DMOV_CE_IN_CHAN,
		.end = DMOV_CE_OUT_CHAN,
		.flags = IORESOURCE_DMA,
	},
	[2] = {
		.name = "crypto_crci_in",
		.start = DMOV_CE_IN_CRCI,
		.end = DMOV_CE_IN_CRCI,
		.flags = IORESOURCE_DMA,
	},
	[3] = {
		.name = "crypto_crci_out",
		.start = DMOV_CE_OUT_CRCI,
		.end = DMOV_CE_OUT_CRCI,
		.flags = IORESOURCE_DMA,
	},
};

static struct resource qcedev_resources[] = {
	[0] = {
		.start = QCE_0_BASE,
		.end = QCE_0_BASE + QCE_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.name = "crypto_channels",
		.start = DMOV_CE_IN_CHAN,
		.end = DMOV_CE_OUT_CHAN,
		.flags = IORESOURCE_DMA,
	},
	[2] = {
		.name = "crypto_crci_in",
		.start = DMOV_CE_IN_CRCI,
		.end = DMOV_CE_IN_CRCI,
		.flags = IORESOURCE_DMA,
	},
	[3] = {
		.name = "crypto_crci_out",
		.start = DMOV_CE_OUT_CRCI,
		.end = DMOV_CE_OUT_CRCI,
		.flags = IORESOURCE_DMA,
	},
};

#endif

#if defined(CONFIG_CRYPTO_DEV_QCRYPTO) || \
		defined(CONFIG_CRYPTO_DEV_QCRYPTO_MODULE)

static struct msm_ce_hw_support qcrypto_ce_hw_suppport = {
	.ce_shared = QCE_CE_SHARED,
	.shared_ce_resource = QCE_SHARE_CE_RESOURCE,
	.hw_key_support = QCE_HW_KEY_SUPPORT,
	.sha_hmac = QCE_SHA_HMAC_SUPPORT,
	.bus_scale_table = &crypto_hw_bus_scale_pdata,
};

static struct platform_device qcrypto_device = {
	.name		= "qcrypto",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(qcrypto_resources),
	.resource	= qcrypto_resources,
	.dev		= {
		.coherent_dma_mask = DMA_BIT_MASK(32),
		.platform_data = &qcrypto_ce_hw_suppport,
	},
};
#endif

#if defined(CONFIG_CRYPTO_DEV_QCEDEV) || \
		defined(CONFIG_CRYPTO_DEV_QCEDEV_MODULE)

static struct msm_ce_hw_support qcedev_ce_hw_suppport = {
	.ce_shared = QCE_CE_SHARED,
	.shared_ce_resource = QCE_SHARE_CE_RESOURCE,
	.hw_key_support = QCE_HW_KEY_SUPPORT,
	.sha_hmac = QCE_SHA_HMAC_SUPPORT,
	.bus_scale_table = &crypto_hw_bus_scale_pdata,
};

static struct platform_device qcedev_device = {
	.name		= "qce",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(qcedev_resources),
	.resource	= qcedev_resources,
	.dev		= {
		.coherent_dma_mask = DMA_BIT_MASK(32),
		.platform_data = &qcedev_ce_hw_suppport,
	},
};
#endif
#if defined(CONFIG_TDMB) || defined(CONFIG_TDMB_MODULE)
static struct msm_bus_vectors tdmb_init_vectors[] = {
	{
		.src = MSM_BUS_MASTER_SPS,
		.dst = MSM_BUS_SLAVE_SPS,
		.ab = 0,
		.ib = 0,
	},
};

static struct msm_bus_vectors tdmb_max_vectors[] = {
	{
		.src = MSM_BUS_MASTER_SPS,
		.dst = MSM_BUS_SLAVE_SPS,
		.ab = 0,
		.ib = (5.4 * 8) * 1000000,
	},
};

static struct msm_bus_paths tdmb_bus_scale_usecases[] = {
	{
		ARRAY_SIZE(tdmb_init_vectors),
		tdmb_init_vectors,
	},
	{
		ARRAY_SIZE(tdmb_max_vectors),
		tdmb_max_vectors,
	},
};

static struct msm_bus_scale_pdata tdmb_bus_scale_pdata = {
	tdmb_bus_scale_usecases,
	ARRAY_SIZE(tdmb_bus_scale_usecases),
	.name = "tdmb",
};

uint32_t tdmb_bus_client_handle;
static void tdmb_gpio_init(void)
{
	tdmb_bus_client_handle = msm_bus_scale_register_client(&tdmb_bus_scale_pdata);
	if (!tdmb_bus_client_handle) {
		printk(KERN_DEBUG "%s: Unable to get bus scale handle\n", __func__);
		return;
	}

	gpio_request(GPIO_TDMB_EN, "tdmb_en");
	gpio_request(GPIO_TDMB_RST, "tdmb_rst");

	gpio_tlmm_config(GPIO_CFG(GPIO_TDMB_EN, GPIOMUX_FUNC_GPIO,
		GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
		GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(GPIO_TDMB_RST, GPIOMUX_FUNC_GPIO,
		GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
		GPIO_CFG_ENABLE);

	gpio_set_value(GPIO_TDMB_EN, 0);
	gpio_set_value(GPIO_TDMB_RST, 0);
}
static void tdmb_gpio_on(void)
{
	printk(KERN_DEBUG "tdmb_gpio_on\n");

	if (tdmb_bus_client_handle)
		msm_bus_scale_client_update_request(tdmb_bus_client_handle, 1);

	gpio_tlmm_config(GPIO_CFG(GPIO_TDMB_EN, GPIOMUX_FUNC_GPIO,
		GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
		GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(GPIO_TDMB_RST, GPIOMUX_FUNC_GPIO,
		GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
		GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(GPIO_TDMB_INT, GPIOMUX_FUNC_GPIO,
		GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
		GPIO_CFG_ENABLE);
	if (system_rev >= 0x9) {
		gpio_tlmm_config(GPIO_CFG(GPIO_TDMB_SPI_MOSI, GPIOMUX_FUNC_1,
			GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
			GPIO_CFG_ENABLE);
		gpio_tlmm_config(GPIO_CFG(GPIO_TDMB_SPI_MISO, GPIOMUX_FUNC_1,
			GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
			GPIO_CFG_ENABLE);
		gpio_tlmm_config(GPIO_CFG(GPIO_TDMB_SPI_CS, GPIOMUX_FUNC_1,
			GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
			GPIO_CFG_ENABLE);
		gpio_tlmm_config(GPIO_CFG(GPIO_TDMB_SPI_CLK, GPIOMUX_FUNC_1,
			GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
			GPIO_CFG_ENABLE);
	} else {
		gpio_tlmm_config(GPIO_CFG(GPIO_TDMB_SPI_MOSI_REV00, GPIOMUX_FUNC_1,
			GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
			GPIO_CFG_ENABLE);
		gpio_tlmm_config(GPIO_CFG(GPIO_TDMB_SPI_MISO_REV00, GPIOMUX_FUNC_1,
			GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
			GPIO_CFG_ENABLE);
		gpio_tlmm_config(GPIO_CFG(GPIO_TDMB_SPI_CS_REV00, GPIOMUX_FUNC_1,
			GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
			GPIO_CFG_ENABLE);
		gpio_tlmm_config(GPIO_CFG(GPIO_TDMB_SPI_CLK_REV00, GPIOMUX_FUNC_1,
			GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
			GPIO_CFG_ENABLE);
	}
	gpio_set_value(GPIO_TDMB_EN, 1);
	usleep_range(10000, 10000);
	gpio_set_value(GPIO_TDMB_RST, 0);
	usleep_range(2000, 2000);
	gpio_set_value(GPIO_TDMB_RST, 1);
	usleep_range(10000, 10000);
}

static void tdmb_gpio_off(void)
{
	printk(KERN_DEBUG "tdmb_gpio_off\n");

	if (tdmb_bus_client_handle) {
		msm_bus_scale_client_update_request(tdmb_bus_client_handle, 0);
	}

	gpio_set_value(GPIO_TDMB_RST, 0);
	usleep_range(1000, 1000);
	gpio_set_value(GPIO_TDMB_EN, 0);

	gpio_tlmm_config(GPIO_CFG(GPIO_TDMB_EN, GPIOMUX_FUNC_GPIO,
		GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
		GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(GPIO_TDMB_RST, GPIOMUX_FUNC_GPIO,
		GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
		GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(GPIO_TDMB_INT, GPIOMUX_FUNC_GPIO,
		GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		GPIO_CFG_ENABLE);
	if (system_rev >= 0x9) {
		gpio_tlmm_config(GPIO_CFG(GPIO_TDMB_SPI_MOSI, GPIOMUX_FUNC_GPIO,
			GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_8MA),
			GPIO_CFG_ENABLE);
		gpio_tlmm_config(GPIO_CFG(GPIO_TDMB_SPI_MISO, GPIOMUX_FUNC_GPIO,
			GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_8MA),
			GPIO_CFG_ENABLE);
		gpio_tlmm_config(GPIO_CFG(GPIO_TDMB_SPI_CS, GPIOMUX_FUNC_GPIO,
			GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_8MA),
			GPIO_CFG_ENABLE);
		gpio_tlmm_config(GPIO_CFG(GPIO_TDMB_SPI_CLK, GPIOMUX_FUNC_GPIO,
			GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_8MA),
			GPIO_CFG_ENABLE);
	} else {
		gpio_tlmm_config(GPIO_CFG(GPIO_TDMB_SPI_MOSI_REV00, GPIOMUX_FUNC_GPIO,
			GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_8MA),
			GPIO_CFG_ENABLE);
		gpio_tlmm_config(GPIO_CFG(GPIO_TDMB_SPI_MISO_REV00, GPIOMUX_FUNC_GPIO,
			GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_8MA),
			GPIO_CFG_ENABLE);
		gpio_tlmm_config(GPIO_CFG(GPIO_TDMB_SPI_CS_REV00, GPIOMUX_FUNC_GPIO,
			GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_8MA),
			GPIO_CFG_ENABLE);
		gpio_tlmm_config(GPIO_CFG(GPIO_TDMB_SPI_CLK_REV00, GPIOMUX_FUNC_GPIO,
			GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_8MA),
			GPIO_CFG_ENABLE);
	}
}

static struct tdmb_platform_data tdmb_pdata = {
	.gpio_on = tdmb_gpio_on,
	.gpio_off = tdmb_gpio_off,
	.irq = MSM_GPIO_TO_INT(GPIO_TDMB_INT),
#if defined(CONFIG_TDMB_ANT_DET)
	.gpio_ant_det = GPIO_PM_TDMB_ANT_DET,
	.irq_ant_det = PM8921_GPIO_IRQ(PM8917_IRQ_BASE, 28),
#endif
};

static struct platform_device tdmb_device = {
	.name		= "tdmb",
	.id		= -1,
	.dev		= {
		.platform_data = &tdmb_pdata,
	},
};

static struct spi_board_info tdmb_spi_info_rev00[] __initdata= {
	{
		.modalias	= "tdmbspi",
		.mode		= SPI_MODE_0,
		.bus_num	= 2,
		.chip_select	= 0,
		.max_speed_hz	= 5400000,
	}
};

static struct spi_board_info tdmb_spi_info[] __initdata = {
	{
		.modalias	= "tdmbspi",
		.mode		= SPI_MODE_0,
		.bus_num	= 11,
		.chip_select	= 0,
		.max_speed_hz	= 5400000,
	}
};

static int __init tdmb_dev_init(void)
{
#if defined(CONFIG_TDMB_ANT_DET)
	struct pm_gpio tdmb_ant_det = {
		.direction	= PM_GPIO_DIR_IN,
		.pull		= PM_GPIO_PULL_NO,
		.vin_sel	= PM_GPIO_VIN_S4,
		.function	= PM_GPIO_FUNC_NORMAL,
		.inv_int_pol	=0,
	};
	pm8xxx_gpio_config(GPIO_PM_TDMB_ANT_DET, &tdmb_ant_det);
#endif
	platform_device_register(&tdmb_device);
	tdmb_gpio_init();

	if (system_rev >= 0x9)
		spi_register_board_info(tdmb_spi_info, ARRAY_SIZE(tdmb_spi_info));
	else
		spi_register_board_info(tdmb_spi_info_rev00, ARRAY_SIZE(tdmb_spi_info_rev00));

	return 0;
}
#endif
#define MDM2AP_ERRFATAL			70
#define AP2MDM_ERRFATAL			95
#define MDM2AP_STATUS			69
#define AP2MDM_STATUS			94
#define AP2MDM_PMIC_RESET_N		80
#define AP2MDM_KPDPWR_N			81

static struct resource mdm_resources[] = {
	{
		.start	= MDM2AP_ERRFATAL,
		.end	= MDM2AP_ERRFATAL,
		.name	= "MDM2AP_ERRFATAL",
		.flags	= IORESOURCE_IO,
	},
	{
		.start	= AP2MDM_ERRFATAL,
		.end	= AP2MDM_ERRFATAL,
		.name	= "AP2MDM_ERRFATAL",
		.flags	= IORESOURCE_IO,
	},
	{
		.start	= MDM2AP_STATUS,
		.end	= MDM2AP_STATUS,
		.name	= "MDM2AP_STATUS",
		.flags	= IORESOURCE_IO,
	},
	{
		.start	= AP2MDM_STATUS,
		.end	= AP2MDM_STATUS,
		.name	= "AP2MDM_STATUS",
		.flags	= IORESOURCE_IO,
	},
	{
		.start	= AP2MDM_PMIC_RESET_N,
		.end	= AP2MDM_PMIC_RESET_N,
		.name	= "AP2MDM_PMIC_RESET_N",
		.flags	= IORESOURCE_IO,
	},
	{
		.start	= AP2MDM_KPDPWR_N,
		.end	= AP2MDM_KPDPWR_N,
		.name	= "AP2MDM_KPDPWR_N",
		.flags	= IORESOURCE_IO,
	},
};

static struct mdm_platform_data mdm_platform_data = {
	.mdm_version = "2.5",
};

static struct platform_device mdm_device = {
	.name		= "mdm2_modem",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(mdm_resources),
	.resource	= mdm_resources,
	.dev		= {
		.platform_data = &mdm_platform_data,
	},
};

static struct platform_device *mdm_devices[] __initdata = {
	&mdm_device,
};

#ifdef CONFIG_MSM_MPM
static uint16_t msm_mpm_irqs_m2a[MSM_MPM_NR_MPM_IRQS] __initdata = {
	[1] = MSM_GPIO_TO_INT(46),
	[2] = MSM_GPIO_TO_INT(150),
	[4] = MSM_GPIO_TO_INT(103),
	[5] = MSM_GPIO_TO_INT(104),
	[6] = MSM_GPIO_TO_INT(105),
	[7] = MSM_GPIO_TO_INT(106),
	[8] = MSM_GPIO_TO_INT(107),
	[9] = MSM_GPIO_TO_INT(7),
	[10] = MSM_GPIO_TO_INT(11),
	[11] = MSM_GPIO_TO_INT(15),
	[12] = MSM_GPIO_TO_INT(19),
	[13] = MSM_GPIO_TO_INT(23),
	[14] = MSM_GPIO_TO_INT(27),
	[15] = MSM_GPIO_TO_INT(31),
	[16] = MSM_GPIO_TO_INT(35),
	[19] = MSM_GPIO_TO_INT(90),
	[20] = MSM_GPIO_TO_INT(92),
	[23] = MSM_GPIO_TO_INT(85),
	[24] = MSM_GPIO_TO_INT(83),
	[25] = USB1_HS_IRQ,
	[26] = MSM_GPIO_TO_INT(6),
	[27] = HDMI_IRQ,
	[29] = MSM_GPIO_TO_INT(10),
	[30] = MSM_GPIO_TO_INT(102),
	[31] = MSM_GPIO_TO_INT(81),
	[32] = MSM_GPIO_TO_INT(78),
	[33] = MSM_GPIO_TO_INT(94),
	[34] = MSM_GPIO_TO_INT(72),
	[35] = MSM_GPIO_TO_INT(39),
	[36] = MSM_GPIO_TO_INT(43),
	[37] = MSM_GPIO_TO_INT(61),
	[38] = MSM_GPIO_TO_INT(50),
	[39] = MSM_GPIO_TO_INT(42),
	[41] = MSM_GPIO_TO_INT(62),
	[42] = MSM_GPIO_TO_INT(8),
	[43] = MSM_GPIO_TO_INT(33),
	[44] = MSM_GPIO_TO_INT(70),
	[45] = MSM_GPIO_TO_INT(69),
	[46] = MSM_GPIO_TO_INT(67),
	[47] = MSM_GPIO_TO_INT(65),
	[48] = MSM_GPIO_TO_INT(55),
	[49] = MSM_GPIO_TO_INT(74),
	[50] = MSM_GPIO_TO_INT(98),
	[51] = MSM_GPIO_TO_INT(49),
	[52] = MSM_GPIO_TO_INT(40),
	[53] = MSM_GPIO_TO_INT(37),
	[54] = MSM_GPIO_TO_INT(24),
	[55] = MSM_GPIO_TO_INT(14),
};

static uint16_t msm_mpm_bypassed_apps_irqs[] __initdata = {
	TLMM_MSM_SUMMARY_IRQ,
	RPM_APCC_CPU0_GP_HIGH_IRQ,
	RPM_APCC_CPU0_GP_MEDIUM_IRQ,
	RPM_APCC_CPU0_GP_LOW_IRQ,
	RPM_APCC_CPU0_WAKE_UP_IRQ,
	RPM_APCC_CPU1_GP_HIGH_IRQ,
	RPM_APCC_CPU1_GP_MEDIUM_IRQ,
	RPM_APCC_CPU1_GP_LOW_IRQ,
	RPM_APCC_CPU1_WAKE_UP_IRQ,
	MSS_TO_APPS_IRQ_0,
	MSS_TO_APPS_IRQ_1,
	MSS_TO_APPS_IRQ_2,
	MSS_TO_APPS_IRQ_3,
	MSS_TO_APPS_IRQ_4,
	MSS_TO_APPS_IRQ_5,
	MSS_TO_APPS_IRQ_6,
	MSS_TO_APPS_IRQ_7,
	MSS_TO_APPS_IRQ_8,
	MSS_TO_APPS_IRQ_9,
	LPASS_SCSS_GP_LOW_IRQ,
	LPASS_SCSS_GP_MEDIUM_IRQ,
	LPASS_SCSS_GP_HIGH_IRQ,
	SPS_MTI_30,
	SPS_MTI_31,
	RIVA_APSS_SPARE_IRQ,
	RIVA_APPS_WLAN_SMSM_IRQ,
	RIVA_APPS_WLAN_RX_DATA_AVAIL_IRQ,
	RIVA_APPS_WLAN_DATA_XFER_DONE_IRQ,
};

struct msm_mpm_device_data msm8930_mpm_dev_data __initdata = {
	.irqs_m2a = msm_mpm_irqs_m2a,
	.irqs_m2a_size = ARRAY_SIZE(msm_mpm_irqs_m2a),
	.bypassed_apps_irqs = msm_mpm_bypassed_apps_irqs,
	.bypassed_apps_irqs_size = ARRAY_SIZE(msm_mpm_bypassed_apps_irqs),
	.mpm_request_reg_base = MSM_RPM_BASE + 0x9d8,
	.mpm_status_reg_base = MSM_RPM_BASE + 0xdf8,
	.mpm_apps_ipc_reg = MSM_APCS_GCC_BASE + 0x008,
	.mpm_apps_ipc_val =  BIT(1),
	.mpm_ipc_irq = RPM_APCC_CPU0_GP_MEDIUM_IRQ,

};
#endif

#define MSM_SHARED_RAM_PHYS 0x80000000

static void __init msm8930_map_io(void)
{
	msm_shared_ram_phys = MSM_SHARED_RAM_PHYS;
	msm_map_msm8930_io();

	if (socinfo_init() < 0)
		pr_err("socinfo_init() failed!\n");
#ifdef CONFIG_SEC_DEBUG
	sec_getlog_supply_meminfo(0x40000000, 0x80000000, 0x00, 0x00);
#endif
}

static void __init msm8930_init_irq(void)
{
	struct msm_mpm_device_data *data = NULL;
#ifdef CONFIG_MSM_MPM
	data = &msm8930_mpm_dev_data;
#endif

	msm_mpm_irq_extn_init(data);
	gic_init(0, GIC_PPI_START, MSM_QGIC_DIST_BASE,
						(void *)MSM_QGIC_CPU_BASE);
}

#if defined(CONFIG_GSM_MODEM_SPRD6500)
static struct msm_spi_platform_data msm8960_qup_spi_gsbi5_pdata = {
	.max_clock_speed = 10800000,
};

static int configure_uart_gpios(int on)
{
	int ret = 0, i;
	int uart_gpios[] = {42, 43, 44, 45};
	for (i = 0; i < ARRAY_SIZE(uart_gpios); i++) {
		if (on) {
			ret = msm_gpiomux_get(uart_gpios[i]);
			if (unlikely(ret))
				break;
		} else {
			ret = msm_gpiomux_put(uart_gpios[i]);
			if (unlikely(ret))
				return ret;
		}
	}
	if (ret)
		for (; i >= 0; i--)
			msm_gpiomux_put(uart_gpios[i]);
	return ret;
}
static struct msm_serial_hs_platform_data msm_uart_dm12_pdata = {
       .inject_rx_on_wakeup = 1,
       .rx_to_inject = 0xFD,
       .gpio_config = configure_uart_gpios,
};
#endif

static void __init msm8930_init_buses(void)
{
#ifdef CONFIG_MSM_BUS_SCALING
	msm_bus_rpm_set_mt_mask();
	msm_bus_8930_apps_fabric_pdata.rpm_enabled = 1;
	msm_bus_8930_sys_fabric_pdata.rpm_enabled = 1;
	msm_bus_8930_mm_fabric_pdata.rpm_enabled = 1;
	msm_bus_8930_apps_fabric.dev.platform_data =
		&msm_bus_8930_apps_fabric_pdata;
	msm_bus_8930_sys_fabric.dev.platform_data =
		&msm_bus_8930_sys_fabric_pdata;
	msm_bus_8930_mm_fabric.dev.platform_data =
		&msm_bus_8930_mm_fabric_pdata;
	msm_bus_8930_sys_fpb.dev.platform_data = &msm_bus_8930_sys_fpb_pdata;
	msm_bus_8930_cpss_fpb.dev.platform_data = &msm_bus_8930_cpss_fpb_pdata;
#if defined(CONFIG_GSM_MODEM_SPRD6500)
	msm_uart_dm12_pdata.wakeup_irq = gpio_to_irq(43); /* GSBI12 (2) */
	msm8960_device_uart_gsbi12.dev.platform_data = &msm_uart_dm12_pdata;
#endif

	
#endif
}
#if !defined(CONFIG_GSM_MODEM_SPRD6500)
static struct msm_spi_platform_data msm8960_qup_spi_gsbi1_pdata = {
	.max_clock_speed = 15060000,
};
#endif

//static struct msm_spi_platform_data msm8960_qup_spi_gsbi2_pdata = {
//	.max_clock_speed = 15060000,
//};
static struct msm_spi_platform_data msm8960_qup_spi_gsbi11_pdata = {
	.max_clock_speed = 15060000,
};


#ifdef CONFIG_USB_MSM_OTG_72K
static struct msm_otg_platform_data msm_otg_pdata;
#else
#ifdef CONFIG_MSM_BUS_SCALING
/* Bandwidth requests (zero) if no vote placed */
static struct msm_bus_vectors usb_init_vectors[] = {
	{
		.src = MSM_BUS_MASTER_SPS,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 0,
		.ib = 0,
	},
};

/* Bus bandwidth requests in Bytes/sec */
static struct msm_bus_vectors usb_max_vectors[] = {
	{
		.src = MSM_BUS_MASTER_SPS,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 60000000,		/* At least 480Mbps on bus. */
		.ib = 960000000,	/* MAX bursts rate */
	},
};

static struct msm_bus_paths usb_bus_scale_usecases[] = {
	{
		ARRAY_SIZE(usb_init_vectors),
		usb_init_vectors,
	},
	{
		ARRAY_SIZE(usb_max_vectors),
		usb_max_vectors,
	},
};

static struct msm_bus_scale_pdata usb_bus_scale_pdata = {
	usb_bus_scale_usecases,
	ARRAY_SIZE(usb_bus_scale_usecases),
	.name = "usb",
};
#endif

#ifdef CONFIG_MFD_MAX77693
static bool vbus_is_on;
static int msm_hsusb_vbus_power_max77693(bool on)
{
	pr_info("%s, attached %d, vbus_is_on %d\n", __func__, on, vbus_is_on);

	/* If VBUS is already on (or off), do nothing. */
	if (vbus_is_on == on)
		return -EBUSY;

	otg_control(on);
	vbus_is_on = on;
	return 0;
}
#endif

static int msm_hsusb_vbus_power(bool on)
{
#ifdef CONFIG_MFD_MAX77693
		return msm_hsusb_vbus_power_max77693(on);
#endif
	return 0;
}

static int hsusb_phy_init_seq[] = {
	0x44, 0x80, /* set VBUS valid threshold
			and disconnect valid threshold */
	0x5F, 0x81, /* update DC voltage level */
	0x3C, 0x82, /* set preemphasis and rise/fall time */
	0x13, 0x83, /* set source impedance adjusment */
	-1};

#define MSM_MPM_PIN_USB1_OTGSESSVLD	40

static struct msm_otg_platform_data msm_otg_pdata = {
	.mode				= USB_OTG,
	.otg_control		= OTG_PMIC_CONTROL,
	.phy_type			= SNPS_28NM_INTEGRATED_PHY,
	.pmic_id_irq		= PM8038_USB_ID_IN_IRQ(PM8038_IRQ_BASE),
	.power_budget		= 750,
#ifdef CONFIG_MSM_BUS_SCALING
	.bus_scale_table	= &usb_bus_scale_pdata,
#endif
#ifdef CONFIG_FB_MSM_HDMI_MHL_8334
	.mhl_dev_name		= "sii8334",
#endif
	.mpm_otgsessvld_int	= MSM_MPM_PIN_USB1_OTGSESSVLD,
	.vbus_power		= msm_hsusb_vbus_power,
};
// #include "board-8930-otg.c"
#endif

#define PID_MAGIC_ID		0x71432909
#define SERIAL_NUM_MAGIC_ID	0x61945374
#define SERIAL_NUMBER_LENGTH	127
#define DLOAD_USB_BASE_ADD	0x2A03F0C8

struct magic_num_struct {
	uint32_t pid;
	uint32_t serial_num;
};

struct dload_struct {
	uint32_t	reserved1;
	uint32_t	reserved2;
	uint32_t	reserved3;
	uint16_t	reserved4;
	uint16_t	pid;
	char		serial_number[SERIAL_NUMBER_LENGTH];
	uint16_t	reserved5;
	struct magic_num_struct magic_struct;
};

static int usb_diag_update_pid_and_serial_num(uint32_t pid, const char *snum)
{
	struct dload_struct __iomem *dload = 0;

	dload = ioremap(DLOAD_USB_BASE_ADD, sizeof(*dload));
	if (!dload) {
		pr_err("%s: cannot remap I/O memory region: %08x\n",
					__func__, DLOAD_USB_BASE_ADD);
		return -ENXIO;
	}

	pr_debug("%s: dload:%p pid:%x serial_num:%s\n",
				__func__, dload, pid, snum);
	/* update pid */
	dload->magic_struct.pid = PID_MAGIC_ID;
	dload->pid = pid;

	/* update serial number */
	dload->magic_struct.serial_num = 0;
	if (!snum) {
		memset(dload->serial_number, 0, SERIAL_NUMBER_LENGTH);
		goto out;
	}

	dload->magic_struct.serial_num = SERIAL_NUM_MAGIC_ID;
	strlcpy(dload->serial_number, snum, SERIAL_NUMBER_LENGTH);
out:
	iounmap(dload);
	return 0;
}

static struct android_usb_platform_data android_usb_pdata = {
	.update_pid_and_serial_num = usb_diag_update_pid_and_serial_num,
#ifdef CONFIG_MACH_MELIUS_VZW
	.cdrom = true,
#else
	.cdrom = false,
#endif
	//.nluns = 0,
};

static struct platform_device android_usb_device = {
	.name	= "android_usb",
	.id	= -1,
	.dev	= {
		.platform_data = &android_usb_pdata,
	},
};

static uint8_t spm_wfi_cmd_sequence[] __initdata = {
	0x03, 0x0f,
};

static uint8_t spm_retention_cmd_sequence[] __initdata = {
	0x00, 0x05, 0x03, 0x0D,
	0x0B, 0x00, 0x0f,
};

static uint8_t spm_retention_with_krait_v3_cmd_sequence[] __initdata = {
	0x42, 0x1B, 0x00,
	0x05, 0x03, 0x0D, 0x0B,
	0x00, 0x42, 0x1B,
	0x0f,
};

static uint8_t spm_power_collapse_without_rpm[] __initdata = {
	0x00, 0x24, 0x54, 0x10,
	0x09, 0x03, 0x01,
	0x10, 0x54, 0x30, 0x0C,
	0x24, 0x30, 0x0f,
};

static uint8_t spm_power_collapse_with_rpm[] __initdata = {
	0x00, 0x24, 0x54, 0x10,
	0x09, 0x07, 0x01, 0x0B,
	0x10, 0x54, 0x30, 0x0C,
	0x24, 0x30, 0x0f,
};

static struct msm_spm_seq_entry msm_spm_boot_cpu_seq_list[] __initdata = {
	[0] = {
		.mode = MSM_SPM_MODE_CLOCK_GATING,
		.notify_rpm = false,
		.cmd = spm_wfi_cmd_sequence,
	},
	[1] = {
		.mode = MSM_SPM_MODE_POWER_RETENTION,
		.notify_rpm = false,
		.cmd = spm_retention_cmd_sequence,
	},
	[2] = {
		.mode = MSM_SPM_MODE_POWER_COLLAPSE,
		.notify_rpm = false,
		.cmd = spm_power_collapse_without_rpm,
	},
	[3] = {
		.mode = MSM_SPM_MODE_POWER_COLLAPSE,
		.notify_rpm = true,
		.cmd = spm_power_collapse_with_rpm,
	},
};

static struct msm_spm_seq_entry msm_spm_nonboot_cpu_seq_list[] __initdata = {
	[0] = {
		.mode = MSM_SPM_MODE_CLOCK_GATING,
		.notify_rpm = false,
		.cmd = spm_wfi_cmd_sequence,
	},
	[1] = {
		.mode = MSM_SPM_MODE_POWER_RETENTION,
		.notify_rpm = false,
		.cmd = spm_retention_cmd_sequence,
	},
	[2] = {
		.mode = MSM_SPM_MODE_POWER_COLLAPSE,
		.notify_rpm = false,
		.cmd = spm_power_collapse_without_rpm,
	},
	[3] = {
		.mode = MSM_SPM_MODE_POWER_COLLAPSE,
		.notify_rpm = true,
		.cmd = spm_power_collapse_with_rpm,
	},
};

static struct msm_spm_platform_data msm_spm_data[] __initdata = {
	[0] = {
		.reg_base_addr = MSM_SAW0_BASE,
		.reg_init_values[MSM_SPM_REG_SAW2_CFG] = 0x1F,
#if defined(CONFIG_MSM_AVS_HW)
		.reg_init_values[MSM_SPM_REG_SAW2_AVS_CTL] = 0x00,
		.reg_init_values[MSM_SPM_REG_SAW2_AVS_HYSTERESIS] = 0x00,
#endif
		.reg_init_values[MSM_SPM_REG_SAW2_SPM_CTL] = 0x01,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DLY] = 0x03020004,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_0] = 0x0084009C,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_1] = 0x00A4001C,
		.vctl_timeout_us = 50,
		.num_modes = ARRAY_SIZE(msm_spm_boot_cpu_seq_list),
		.modes = msm_spm_boot_cpu_seq_list,
	},
	[1] = {
		.reg_base_addr = MSM_SAW1_BASE,
		.reg_init_values[MSM_SPM_REG_SAW2_CFG] = 0x1F,
#if defined(CONFIG_MSM_AVS_HW)
		.reg_init_values[MSM_SPM_REG_SAW2_AVS_CTL] = 0x00,
		.reg_init_values[MSM_SPM_REG_SAW2_AVS_HYSTERESIS] = 0x00,
#endif
		.reg_init_values[MSM_SPM_REG_SAW2_SPM_CTL] = 0x01,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DLY] = 0x03020004,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_0] = 0x0084009C,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_1] = 0x00A4001C,
		.vctl_timeout_us = 50,
		.num_modes = ARRAY_SIZE(msm_spm_nonboot_cpu_seq_list),
		.modes = msm_spm_nonboot_cpu_seq_list,
	},
};

static uint8_t l2_spm_wfi_cmd_sequence[] __initdata = {
	0x00, 0x20, 0x03, 0x20,
	0x00, 0x0f,
};

static uint8_t l2_spm_gdhs_cmd_sequence[] __initdata = {
	0x00, 0x20, 0x34, 0x64,
	0x48, 0x07, 0x48, 0x20,
	0x50, 0x64, 0x04, 0x34,
	0x50, 0x0f,
};
static uint8_t l2_spm_power_off_cmd_sequence[] __initdata = {
	0x00, 0x10, 0x34, 0x64,
	0x48, 0x07, 0x48, 0x10,
	0x50, 0x64, 0x04, 0x34,
	0x50, 0x0F,
};

static struct msm_spm_seq_entry msm_spm_l2_seq_list[] __initdata = {
	[0] = {
		.mode = MSM_SPM_L2_MODE_RETENTION,
		.notify_rpm = false,
		.cmd = l2_spm_wfi_cmd_sequence,
	},
	[1] = {
		.mode = MSM_SPM_L2_MODE_GDHS,
		.notify_rpm = true,
		.cmd = l2_spm_gdhs_cmd_sequence,
	},
	[2] = {
		.mode = MSM_SPM_L2_MODE_POWER_COLLAPSE,
		.notify_rpm = true,
		.cmd = l2_spm_power_off_cmd_sequence,
	},
};

static struct msm_spm_platform_data msm_spm_l2_data[] __initdata = {
	[0] = {
		.reg_base_addr = MSM_SAW_L2_BASE,
		.reg_init_values[MSM_SPM_REG_SAW2_SPM_CTL] = 0x00,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DLY] = 0x02020204,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_0] = 0x00A000AE,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_1] = 0x00A00020,
		.modes = msm_spm_l2_seq_list,
		.num_modes = ARRAY_SIZE(msm_spm_l2_seq_list),
	},
};

#ifdef CONFIG_FB_MSM_HDMI_MHL_8334
#define MHL_POWER_GPIO_PM8038	PM8038_GPIO_PM_TO_SYS(MHL_GPIO_PWR_EN)
#define MHL_POWER_GPIO_PM8917	PM8917_GPIO_PM_TO_SYS(25)
static struct msm_mhl_platform_data mhl_platform_data = {
	.irq = MSM_GPIO_TO_INT(MHL_GPIO_INT),
	.gpio_mhl_int = MHL_GPIO_INT,
	.gpio_mhl_reset = MHL_GPIO_RESET,
	.gpio_mhl_power = MHL_POWER_GPIO_PM8038,
	.gpio_hdmi_mhl_mux = HDMI_MHL_MUX_GPIO,
};

static struct i2c_board_info sii_device_info[] __initdata = {
	{
		/*
		 * keeps SI 8334 as the default
		 * MHL TX
		 */
		I2C_BOARD_INFO("sii8334", 0x39),
		.platform_data = &mhl_platform_data,
		.flags = I2C_CLIENT_WAKE,
	},
};
#endif /*CONFIG_FB_MSM_HDMI_MHL_8334*/

#ifdef CONFIG_KEYBOARD_GPIO
static struct gpio_keys_button gpio_keys_button[] = {
	{
		.code			= KEY_VOLUMEUP,
		.type			= EV_KEY,
		.gpio			= GPIO_VOL_UP,
		.active_low		= 1,
		.wakeup			= 0,
		.debounce_interval	= 5, /* ms */
		.desc			= "Vol Up",
#if defined(CONFIG_KEYBOARD_GPIO_EXTENDED_RESUME_EVENT)
		.support_evt		= NOT_SUPPORT_RESUME_KEY_EVENT,
#endif
	},
	{
		.code			= KEY_VOLUMEDOWN,
		.type			= EV_KEY,
		.gpio			= GPIO_VOL_DOWN,
		.active_low		= 1,
		.wakeup			= 0,
		.debounce_interval	= 5, /* ms */
		.desc			= "Vol Down",
#if defined(CONFIG_KEYBOARD_GPIO_EXTENDED_RESUME_EVENT)
		.support_evt		= NOT_SUPPORT_RESUME_KEY_EVENT,
#endif
	},
	{
		.code			= KEY_HOMEPAGE,
		.type			= EV_KEY,
		.gpio			= GPIO_HOME_KEY,
		.active_low		= 1,
		.wakeup			= 1,
		.debounce_interval	= 15, /* ms */
		.desc			= "Home",
#if defined(CONFIG_KEYBOARD_GPIO_EXTENDED_RESUME_EVENT)
		.support_evt		= SUPPORT_RESUME_KEY_EVENT,
#endif
	},
};
static struct gpio_keys_platform_data gpio_keys_platform_data = {
	.buttons	= gpio_keys_button,
	.nbuttons	= ARRAY_SIZE(gpio_keys_button),
	.rep		= 0,
#ifdef CONFIG_SENSORS_HALL
	.gpio_flip_cover = GPIO_HALL_IC_IRQ,
#endif
};

static struct platform_device msm8960_gpio_keys_device = {
	.name	= "gpio-keys",
	.id	= -1,
	.dev	= {
		.platform_data	= &gpio_keys_platform_data,
	}
};
#endif
#ifdef MSM8930_PHASE_2
#ifdef CONFIG_FM_SI4705
static void fmradio_gpio_init(void)
{
	pr_info("%s: init fm radio gpio \n",__func__);
	gpio_request(GPIO_FM_RST, "fmradio_reset");
	gpio_tlmm_config(GPIO_CFG(GPIO_FM_RST, 0, GPIO_CFG_OUTPUT,
	GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), 0);
	gpio_free(GPIO_FM_RST);
}
static void melius_si4705_ldo_setup(int on)
{
	static struct regulator *reg_l10;
	static struct regulator *reg_l4;
	int rc;

	printk(KERN_ERR"%s.\n", __func__);
	if (on) {
		reg_l10 = regulator_get(NULL, "8917_l10");
		if (IS_ERR(reg_l10))
			printk(KERN_ERR"could not get 8921_reg_l10, rc = %ld\n",
				PTR_ERR(reg_l10));
		rc = regulator_set_voltage(reg_l10, 2900000, 2900000);
		if (rc)
				printk(KERN_ERR"set_voltage l10 failed, rc=%d\n", rc);
		reg_l4 = regulator_get(NULL, "8917_l4");
		if (IS_ERR(reg_l4))
			printk(KERN_ERR"could not get reg_l4, rc = %ld\n",
				PTR_ERR(reg_l4));
		rc = regulator_set_voltage(reg_l4, 1800000, 1800000);
		if (rc)
			printk(KERN_ERR"set_voltage reg_l4 failed, rc=%d\n",
					rc);
		rc = regulator_set_optimum_mode(reg_l10, 100000);
		if (rc < 0)
			printk(KERN_ERR"set_optimum_mode reg_l10 failed, rc=%d\n",
					rc);
		rc = regulator_enable(reg_l10);
		if (rc)
			printk(KERN_ERR"enable reg_l10 failed, rc=%d\n", rc);
				rc = regulator_set_optimum_mode(reg_l4, 100000);
		if (rc < 0)
			printk(KERN_ERR"set_optimum_mode reg_l4 failed, rc=%d\n",
					rc);
		rc = regulator_enable(reg_l4);
		if (rc)
			printk(KERN_ERR"enable reg_l4 failed, rc=%d\n", rc);
	} else {
		rc = regulator_disable(reg_l10);
		if (rc)
			printk(KERN_ERR"disable reg_l10 failed, rc=%d\n", rc);
		rc = regulator_disable(reg_l4);
		if (rc)
			printk(KERN_ERR"disable reg_l4 failed, rc=%d\n", rc);
	}
}
static void fmradio_power(int on)
{
	int rev = msm8930_get_board_rev();
	if ( rev < 8)
	melius_si4705_ldo_setup(on);
	if (on) {
		printk(KERN_ERR"%s.\n", __func__);
		gpio_request(GPIO_FM_RST, "fmradio_reset");
		gpio_tlmm_config(GPIO_CFG(GPIO_FM_RST, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), 1);
		gpio_request(GPIO_FM_INT, "FMRAIDO INT");
		gpio_tlmm_config(GPIO_CFG(GPIO_FM_INT, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);

		gpio_set_value(GPIO_FM_RST, 0);
		gpio_set_value(GPIO_FM_INT, 0);
		usleep_range(5, 10);
		gpio_set_value(GPIO_FM_RST, 1);
		usleep_range(10, 15);
		gpio_set_value(GPIO_FM_INT, 1);
		gpio_tlmm_config(GPIO_CFG(GPIO_FM_INT, 0, GPIO_CFG_INPUT,
		GPIO_CFG_PULL_UP, GPIO_CFG_2MA), 1);
	} else {
		gpio_set_value(GPIO_FM_RST, 0);
		gpio_tlmm_config(GPIO_CFG(GPIO_FM_INT, 0, GPIO_CFG_INPUT,
		GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), 1);
		gpio_free(GPIO_FM_RST);
		gpio_free(GPIO_FM_INT);
	}
}

static struct si47xx_platform_data si47xx_pdata = {
	.rx_vol = {0x0, 0x13, 0x16, 0x19, 0x1C, 0x1F, 0x22, 0x25,
		0x28, 0x2B, 0x2E, 0x31, 0x34, 0x37, 0x3A, 0x3D},
	.power = fmradio_power,

};


#endif
#ifdef CONFIG_2MIC_ES305
static int a2220_hw_init(void)
{
	int rc = 0;
	struct pm_gpio a2220_reset = {
		.direction      = PM_GPIO_DIR_OUT,
		.output_buffer  = PM_GPIO_OUT_BUF_CMOS,
		.output_value   = 1,
		.pull	   = PM_GPIO_PULL_NO,
		.vin_sel	= PM_GPIO_VIN_S4,
		.out_strength   = PM_GPIO_STRENGTH_MED,
		.function       = PM_GPIO_FUNC_NORMAL,
	};

	pr_info("%s\n", __func__);

	rc = gpio_request(A2220_WAKEUP, "a2220_wakeup");
	if (rc < 0) {
		pr_err("%s: gpio request wakeup pin failed\n", __func__);
		goto err_alloc_data_failed;
	}

	rc = gpio_direction_output(A2220_WAKEUP, 1);
	if (rc < 0) {
		pr_err("%s: request wakeup gpio direction failed\n", __func__);
		goto err_free_gpio;
	}

	rc = gpio_request(A2220_RESET, "a2220_reset");
	if (rc < 0) {
		pr_err("%s: gpio request reset pin failed\n", __func__);
		goto err_free_gpio;
	}

	rc = pm8xxx_gpio_config(A2220_RESET, &a2220_reset);
	if (rc) {
		pr_err("%s: Failed to configure gpio %d\n", __func__,
			A2220_RESET);
		goto err_free_gpio;
	}

	rc = gpio_direction_output(A2220_RESET, 1);
	if (rc < 0) {
		pr_err("%s: request reset gpio direction failed\n", __func__);
		goto err_free_gpio_all;
	}
	gpio_set_value(A2220_WAKEUP, 1);
	gpio_set_value(A2220_RESET, 1);
	return rc;

err_free_gpio_all:
	gpio_free(A2220_RESET);
err_free_gpio:
	gpio_free(A2220_WAKEUP);
err_alloc_data_failed:
	pr_err("a2220_probe - failed\n");
	return rc;
}

static struct a2220_platform_data a2220_data = {
	.a2220_hw_init = a2220_hw_init,
	.gpio_reset = A2220_RESET,
	.gpio_wakeup = A2220_WAKEUP,
};

static struct i2c_board_info a2220_device[] __initdata = {
	{
		I2C_BOARD_INFO("audience_a2220", 0x3E),
		.platform_data = &a2220_data,
	},
#ifdef CONFIG_FM_SI4705
	{
		I2C_BOARD_INFO("si4705", 0x22>>1),
		.platform_data = &si47xx_pdata,
		.irq = MSM_GPIO_TO_INT(11),
	},
#endif
};
#ifndef CONFIG_2MIC_QUP_I2C
static struct i2c_gpio_platform_data  a2220_i2c_gpio_data = {
	.sda_pin		= GPIO_2MIC_I2C_SDA,
	.scl_pin		= GPIO_2MIC_I2C_SCL,
	.udelay			= 1,
};

static struct platform_device a2220_i2c_gpio_device = {
	.name			= "i2c-gpio",
	.id			= MSM_A2220_I2C_BUS_ID,
	.dev.platform_data	= &a2220_i2c_gpio_data,
};
#endif /* CONFIG_2MIC_QUP_I2C */
#endif /* CONFIG_2MIC_ES305 */

#endif /* MSM8930_PHASE_2 */

static struct msm_i2c_platform_data msm8960_i2c_qup_gsbi4_pdata = {
	.clk_freq = 100000,
	.src_clk_rate = 24000000,
};

static struct msm_i2c_platform_data msm8960_i2c_qup_gsbi3_pdata = {
	.clk_freq = 100000,
	.src_clk_rate = 24000000,
};
#if (defined CONFIG_2MIC_ES305) && (defined CONFIG_2MIC_QUP_I2C)
#ifdef CONFIG_2MIC_QUP_I2C_GSBI2
static struct msm_i2c_platform_data msm8960_i2c_qup_gsbi2_pdata = {
	.clk_freq = 400000,
	.src_clk_rate = 24000000,
	.use_gsbi_shared_mode = 1,
};
#else
static struct msm_i2c_platform_data msm8960_i2c_qup_gsbi5_pdata = {
	.clk_freq = 400000,
	.src_clk_rate = 24000000,
	.use_gsbi_shared_mode = 1,
};
#endif
#endif

#ifndef CONFIG_SLIMBUS_MSM_CTRL
static struct msm_i2c_platform_data msm8960_i2c_qup_gsbi8_pdata = {
	.clk_freq = 100000,
	.src_clk_rate = 24000000,
};
#endif
#ifdef CONFIG_SEC_FPGA
static struct regulator *barcode_vreg_2p85;
static struct regulator *barcode_vreg_l25;
static struct regulator *barcode_vreg_1p8;
static struct msm_xo_voter *fpga_xo;
static int ice4_clock_en(int onoff)
{
	if (onoff)
		msm_xo_mode_vote(fpga_xo, MSM_XO_MODE_ON);
	else
		msm_xo_mode_vote(fpga_xo, MSM_XO_MODE_OFF);
	return 0;
}
static void barcode_emul_poweron(int onoff)
{
	int ret;

	pr_info("%s onoff : %d\n", __func__, onoff);
	if (barcode_vreg_2p85 == NULL) {
		barcode_vreg_2p85 = regulator_get(NULL, "barcode_2p85");
		if (IS_ERR(barcode_vreg_2p85))
			return ;

		ret = regulator_set_voltage(barcode_vreg_2p85,
				2850000, 2850000);
		if (ret)
			pr_err("%s: error vreg_2p85 set voltage ret=%d\n",
				__func__, ret);
	}
	if (barcode_vreg_l25 == NULL) {
		barcode_vreg_l25 = regulator_get(NULL,
							"barcode_l25");
		if (IS_ERR(barcode_vreg_l25))
			return ;
		ret = regulator_set_voltage(barcode_vreg_l25,
						1250000, 1250000);
		if (ret)
			pr_err("%s: error vreg_l25 set voltage ret=%d\n",
							__func__, ret);
	}

	if (barcode_vreg_1p8 == NULL) {
		barcode_vreg_1p8 = regulator_get(NULL, "barcode_1p8");
		if (IS_ERR(barcode_vreg_1p8))
			return ;
	}

	if (onoff) {
		ret = regulator_enable(barcode_vreg_l25);
		if (ret)
			pr_err("%s: error enabling regulator\n",
							__func__);
		pr_info("%s gpio switch on\n", __func__);
		ret = regulator_enable(barcode_vreg_2p85);
		if (ret)
			pr_err("%s: error enabling regulator\n", __func__);
		ret = regulator_enable(barcode_vreg_1p8);
		if (ret)
			pr_err("%s: error enabling regulator\n", __func__);

		fpga_xo = msm_xo_get(MSM_XO_TCXO_D0, "ice4_fpga");
		if (IS_ERR(fpga_xo)) {
			printk(KERN_ERR "%s: Couldn't get TCXO_D0 vote for ice4_fpga\n",
					__func__);
		}
	} else {
		msm_xo_put(fpga_xo);
		if (regulator_is_enabled(barcode_vreg_2p85)) {
			ret = regulator_disable(barcode_vreg_2p85);
			if (ret)
				pr_err("%s: error disabling regulator\n",
				__func__);
		}
		if (regulator_is_enabled(barcode_vreg_1p8)) {
			ret = regulator_disable(barcode_vreg_1p8);
			if (ret)
				pr_err("%s: error disabling regulator\n",
				__func__);
		}
		if (regulator_is_enabled(barcode_vreg_l25)) {
			ret = regulator_disable(barcode_vreg_l25);
			if (ret)
				pr_err("%s: error disabling regulator\n",
						__func__);
		}
	}
}

static void barcode_gpio_config(void);

struct barcode_emul_platform_data barcode_emul_info = {
	.spi_en  = -1,
	.cresetb = GPIO_CRESET_B,
	.rst_n   = GPIO_FPGA_RST_N,
#ifndef CONFIG_NO_GPIO_EXPANDER_FPGA
	.cdone   = GPIO_FPGA_CDONE,
#endif
	.clock_en = ice4_clock_en,
	.poweron = barcode_emul_poweron,
	.gpioconfig = barcode_gpio_config,
	.spi_clk = GPIO_IRDA_SCL,
	.spi_si  = GPIO_IRDA_SDA,
#if defined(CONFIG_IR_REMOCON_FPGA)
	.ir_remote_init = irda_device_init,
	.irda_wake = GPIO_FPGA_RST_N,
	.irda_irq = GPIO_IRDA_IRQ,
	.ir_wake_en = irda_wake_en,
	.ir_vdd_onoff = irda_vdd_onoff,
#endif
};

static void barcode_gpio_config(void)
{
	pr_info("%s\n", __func__);

	gpio_tlmm_config(GPIO_CFG(GPIO_CRESET_B, 0, GPIO_CFG_OUTPUT,
		GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);

	barcode_emul_info.fw_type = ICE_IRDA;
}

static struct i2c_gpio_platform_data ice4_fpga_i2c_gpio_data = {
#if defined(CONFIG_IR_REMOCON_FPGA)
	.sda_pin		= GPIO_IRDA_SDA,
	.scl_pin		= GPIO_IRDA_SCL,
	.udelay = 2,
	.sda_is_open_drain = 0,
	.scl_is_open_drain = 0,
	.scl_is_output_only = 0,
#endif
};

struct platform_device ice4_fpga_i2c_gpio_device = {
	.name = "i2c-gpio",
	.id = MSM_SEC_FPGA_I2C_BUS_ID,
	.dev.platform_data = &ice4_fpga_i2c_gpio_data,
};

static struct i2c_board_info ice4_fpga_i2c_board_info[] = {
	{
		I2C_BOARD_INFO("ice4", (0x6c)),
		.platform_data = &barcode_emul_info,
	},
#if defined(CONFIG_LEDS_AN30259A)
    {
		I2C_BOARD_INFO("an30259a", 0x30),
	},
#endif
};
#endif

static struct msm_i2c_platform_data msm8960_i2c_qup_gsbi9_pdata = {
	.clk_freq = 100000,
	.src_clk_rate = 24000000,
};
#if !defined(CONFIG_GSM_MODEM_SPRD6500)
static struct msm_i2c_platform_data msm8960_i2c_qup_gsbi10_pdata = {
	.clk_freq = 100000,
	.src_clk_rate = 24000000,
};
#endif
#if !defined(CONFIG_GSM_MODEM_SPRD6500)
static struct msm_i2c_platform_data msm8960_i2c_qup_gsbi12_pdata = {
	.clk_freq = 100000,
	.src_clk_rate = 24000000,
};
#endif

#if defined(CONFIG_KS8851) || defined(CONFIG_KS8851_MODULE)
/*static struct ks8851_pdata spi_eth_pdata = {
	.irq_gpio = KS8851_IRQ_GPIO,
	.rst_gpio = KS8851_RST_GPIO,
};*/
#endif

#if defined(CONFIG_KS8851)
static struct spi_board_info spi_board_info[] __initdata = {
#if defined(CONFIG_KS8851) || defined(CONFIG_KS8851_MODULE)
	{
		.modalias               = "ks8851",
		.irq                    = MSM_GPIO_TO_INT(KS8851_IRQ_GPIO),
		.max_speed_hz           = 19200000,
		.bus_num                = 0,
		.chip_select            = 0,
		.mode                   = SPI_MODE_0,
		.platform_data		= &spi_eth_pdata
	},
#endif
#ifdef CONFIG_FB_MSM_MIPI_DSI_NOVATEK
	{
		.modalias               = "dsi_novatek_3d_panel_spi",
		.max_speed_hz           = 10800000,
		.bus_num                = 0,
		.chip_select            = 1,
		.mode                   = SPI_MODE_0,
	},
#endif
};
#endif

#ifdef CONFIG_IMX175_EEPROM	/* For Camera Actuator EEPROM By Teddy */
#if defined(CONFIG_MACH_MELIUS_SKT) || defined(CONFIG_MACH_MELIUS_KTT) || \
	defined(CONFIG_MACH_MELIUS_LGT)
static struct spi_board_info eeprom_spi_info_kor[] __initdata = {
	{
		.modalias			= "imx175_spi",
		.max_speed_hz			= 9963243,
		.bus_num			= 2,
		.chip_select			= 0,
		.mode				= SPI_MODE_0,
	},
};
#endif
static struct spi_board_info eeprom_spi_info[] __initdata = {
	{
		.modalias			= "imx175_spi",
		.max_speed_hz			= 9963243,
		.bus_num			= 11,
		.chip_select			= 0,
		.mode				= SPI_MODE_0,
	},
};

#endif

static struct platform_device msm_device_saw_core0 = {
	.name	= "saw-regulator",
	.id	= 0,
	.dev	= {
		.platform_data = &msm8930_pm8038_saw_regulator_core0_pdata,
	},
};

static struct platform_device msm_device_saw_core1 = {
	.name	= "saw-regulator",
	.id	= 1,
	.dev	= {
		.platform_data = &msm8930_pm8038_saw_regulator_core1_pdata,
	},
};

static struct tsens_platform_data msm_tsens_pdata  = {
	.tsens_factor		= 1000,
	.hw_type		= APQ_8064,
	.tsens_num_sensor	= 10,
	.slope = {1132, 1135, 1137, 1135, 1157,
			1142, 1124, 1153, 1175, 1166},
};

static struct platform_device msm_tsens_device = {
	.name   = "msm8930-tmu",
	.id = -1,
};

static struct msm_thermal_data msm_thermal_pdata = {
	.sensor_id = 9,
	.poll_ms = 250,
	.limit_temp_degC = 60,
	.temp_hysteresis_degC = 10,
	.freq_step = 2,
};

#ifdef CONFIG_MSM_FAKE_BATTERY
static struct platform_device fish_battery_device = {
	.name = "fish_battery",
};
#endif

#ifndef MSM8930_PHASE_2

/* 8930 Phase 1 */
static struct platform_device msm8930_device_ext_5v_vreg __devinitdata = {
	.name	= GPIO_REGULATOR_DEV_NAME,
	.id	= PM8921_MPP_PM_TO_SYS(7),
	.dev	= {
		.platform_data = &msm_gpio_regulator_pdata[GPIO_VREG_ID_EXT_5V],
	},
};

static struct platform_device msm8930_device_ext_l2_vreg __devinitdata = {
	.name	= GPIO_REGULATOR_DEV_NAME,
	.id	= 91,
	.dev	= {
		.platform_data = &msm_gpio_regulator_pdata[GPIO_VREG_ID_EXT_L2],
	},
};

#else

/* 8930 Phase 2 */
static struct platform_device msm8930_device_ext_5v_vreg __devinitdata = {
	.name	= GPIO_REGULATOR_DEV_NAME,
	.id	= 63,
	.dev	= {
		.platform_data = &msm8930_pm8038_gpio_regulator_pdata[
					MSM8930_GPIO_VREG_ID_EXT_5V],
	},
};

static struct platform_device msm8930_device_ext_otg_sw_vreg __devinitdata = {
	.name	= GPIO_REGULATOR_DEV_NAME,
	.id	= 97,
	.dev	= {
		.platform_data = &msm8930_pm8038_gpio_regulator_pdata[
					MSM8930_GPIO_VREG_ID_EXT_OTG_SW],
	},
};

#endif

static struct platform_device msm8930_device_rpm_regulator __devinitdata = {
	.name	= "rpm-regulator",
	.id	= -1,
	.dev	= {
#ifndef MSM8930_PHASE_2
		.platform_data = &msm_rpm_regulator_pdata,
#else
		.platform_data = &msm8930_pm8038_rpm_regulator_pdata,
#endif
	},
};

#ifdef CONFIG_SAMSUNG_JACK
#if defined (CONFIG_MACH_MELIUS_CHN_CTC)
static struct sec_jack_zone jack_zones[] = {
	[0] = {
		.adc_high	= 3,
		.delay_ms	= 10,
		.check_count	= 10,
		.jack_type	= SEC_HEADSET_3POLE,
	},
	[1] = {
		.adc_high	= 950,
		.delay_ms	= 10,
		.check_count	= 10,
		.jack_type	= SEC_HEADSET_3POLE,
	},
	[2] = {
		.adc_high	= 2700,
		.delay_ms	= 10,
		.check_count	= 10,
		.jack_type	= SEC_HEADSET_4POLE,
	},
	[3] = {
		.adc_high	= 9999,
		.delay_ms	= 10,
		.check_count	= 10,
		.jack_type	= SEC_HEADSET_4POLE,
	},
};

static struct sec_jack_zone jack_zones_rev08[] = {
	[0] = {
		.adc_high	= 3,
		.delay_ms	= 10,
		.check_count	= 10,
		.jack_type	= SEC_HEADSET_3POLE,
	},
	[1] = {
		.adc_high	= 710,
		.delay_ms	= 10,
		.check_count	= 10,
		.jack_type	= SEC_HEADSET_3POLE,
	},
	[2] = {
		.adc_high	= 2700,
		.delay_ms	= 10,
		.check_count	= 10,
		.jack_type	= SEC_HEADSET_4POLE,
	},
	[3] = {
		.adc_high	= 9999,
		.delay_ms	= 10,
		.check_count	= 10,
		.jack_type	= SEC_HEADSET_4POLE,
	},
};
#elif defined(CONFIG_MACH_MELIUS_USC) || defined(CONFIG_MACH_MELIUS_SPR) || defined(CONFIG_MACH_MELIUS_VZW)
static struct sec_jack_zone jack_zones[] = {
	[0] = {
		.adc_high	= 3,
		.delay_us	= 10,
		.check_count	= 10,
		.jack_type	= SEC_HEADSET_3POLE,
	},
	[1] = {
		.adc_high	= 620,
		.delay_us	= 10,
		.check_count	= 10,
		.jack_type	= SEC_HEADSET_3POLE,
	},
	[2] = {
		.adc_high	= 2700,
		.delay_us	= 10,
		.check_count	= 10,
		.jack_type	= SEC_HEADSET_4POLE,
	},
	[3] = {
		.adc_high	= 9999,
		.delay_us	= 10,
		.check_count	= 10,
		.jack_type	= SEC_HEADSET_4POLE,
	},
};
#else
static struct sec_jack_zone jack_zones[] = {
	[0] = {
		.adc_high	= 3,
		.delay_ms	= 10,
		.check_count	= 10,
		.jack_type	= SEC_HEADSET_3POLE,
	},
	[1] = {
		.adc_high	= 950,
		.delay_ms	= 10,
		.check_count	= 10,
		.jack_type	= SEC_HEADSET_3POLE,
	},
	[2] = {
		.adc_high	= 2700,
		.delay_ms	= 10,
		.check_count	= 10,
		.jack_type	= SEC_HEADSET_4POLE,
	},
	[3] = {
		.adc_high	= 9999,
		.delay_ms	= 10,
		.check_count	= 10,
		.jack_type	= SEC_HEADSET_4POLE,
	},
};
#endif

#if defined (CONFIG_MACH_MELIUS_CHN_CTC)
static struct sec_jack_buttons_zone jack_buttons_zones[] = {
	{
		.code		= KEY_MEDIA,
		.adc_low	= 0,
		.adc_high	= 175,
	},
	{
		.code		= KEY_VOLUMEUP,
		.adc_low	= 176,
		.adc_high	= 350,
	},
	{
		.code		= KEY_VOLUMEDOWN,
		.adc_low	= 351,
		.adc_high	= 680,
	},
};
static struct sec_jack_buttons_zone jack_buttons_zones_rev08[] = {
	{
		.code		= KEY_MEDIA,
		.adc_low	= 0,
		.adc_high	= 115,
	},
	{
		.code		= KEY_VOLUMEUP,
		.adc_low	= 116,
		.adc_high	= 240,
	},
	{
		.code		= KEY_VOLUMEDOWN,
		.adc_low	= 241,
		.adc_high	= 700,
	},
};
#elif defined (CONFIG_MACH_MELIUS_EUR_OPEN)
static struct sec_jack_buttons_zone jack_buttons_zones[] = {
	{
		.code		= KEY_MEDIA,
		.adc_low	= 0,
		.adc_high	= 175,
	},
	{
		.code		= KEY_VOLUMEUP,
		.adc_low	= 176,
		.adc_high	= 350,
	},
	{
		.code		= KEY_VOLUMEDOWN,
		.adc_low	= 351,
		.adc_high	= 680,
	},
};
static struct sec_jack_buttons_zone jack_buttons_zones_rev06[] = {
	{
		.code		= KEY_MEDIA,
		.adc_low	= 0,
		.adc_high	= 120,
	},
	{
		.code		= KEY_VOLUMEUP,
		.adc_low	= 121,
		.adc_high	= 325,
	},
	{
		.code		= KEY_VOLUMEDOWN,
		.adc_low	= 326,
		.adc_high	= 680,
	},
};
#elif defined(CONFIG_MACH_MELIUS_USC) || defined(CONFIG_MACH_MELIUS_SPR) || defined(CONFIG_MACH_MELIUS_VZW)
/* To support 3-buttons earjack */
static struct sec_jack_buttons_zone jack_buttons_zones[] = {
	{
		.code		= KEY_MEDIA,
		.adc_low	= 0,
		.adc_high	= 107,
	},
	{
		.code		= KEY_VOLUMEUP,
		.adc_low	= 108,
		.adc_high	= 210,
	},
	{
		.code		= KEY_VOLUMEDOWN,
		.adc_low	= 211,
		.adc_high	= 440,
	},
};
#else
/* To support 3-buttons earjack */
static struct sec_jack_buttons_zone jack_buttons_zones[] = {
	{
		.code		= KEY_MEDIA,
		.adc_low	= 0,
		.adc_high	= 175,
	},
	{
		.code		= KEY_VOLUMEUP,
		.adc_low	= 176,
		.adc_high	= 350,
	},
	{
		.code		= KEY_VOLUMEDOWN,
		.adc_low	= 351,
		.adc_high	= 680,
	},
};
#endif
#if defined(CONFIG_SAMSUNG_JACK_GNDLDET)
static int get_sec_gnd_jack_state(void)
{
	int status = 0;

	status = gpio_get_value_cansleep(GPIO_EAR_GND);
	pr_debug("%s : status %d\n", __func__, status);

	return status^1;
}

static int get_sec_det_jack_state(void)
{
	int status = 0;

	status = gpio_get_value_cansleep(GPIO_EAR_DET);
	pr_debug("%s : status %d\n", __func__, status);

	return status^1;
}

#endif
static int get_sec_send_key_state(void)
{
	int status = 0;

	status = gpio_get_value_cansleep(GPIO_EAR_SEND_END);
	pr_debug("%s : status %d\n", __func__, status);

	return status^1;
}

/* extern void msm8930_enable_codec_internal_micbias(bool state);*/
   extern void msm8930_enable_ear_micbias(bool state);

static void set_sec_micbias_state(bool state)
{

	pr_info("sec_jack: ear micbias %s\n", state ? "on" : "off");
/*TODO: will fix
	gpio_tlmm_config(GPIO_CFG(PMIC_GPIO_EAR_MICBIAS_EN, 0,
			GPIO_CFG_INPUT, GPIO_CFG_NO_PULL,
			GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	gpio_set_value(PMIC_GPIO_EAR_MICBIAS_EN, (state ? 1 : 0));
*/
	msm8930_enable_ear_micbias(state);
}

static int sec_jack_get_adc_value(void)
{
	int rc = 0;
	int retVal = 0;
	struct pm8xxx_adc_chan_result result;
	rc = pm8xxx_adc_mpp_config_read(
			PM8XXX_AMUX_MPP_7,
			ADC_MPP_1_AMUX6_SCALE_DEFAULT,
			&result);
	if (rc) {
		pr_err("%s : error reading mpp %d, rc = %d\n",
			__func__, PM8XXX_AMUX_MPP_7, rc);
		return rc;
	}
	retVal = ((int)result.physical)/1000;
	return retVal;
}

static struct sec_jack_platform_data sec_jack_data = {
#if defined(CONFIG_SAMSUNG_JACK_GNDLDET)
	.get_l_jack_state	= get_sec_det_jack_state,
#endif
	.get_send_key_state	= get_sec_send_key_state,
	.set_micbias_state	= set_sec_micbias_state,
	.get_adc_value		= sec_jack_get_adc_value,
	.zones			= jack_zones,
#ifdef CONFIG_MACH_MELIUS_CHN_CTC
	.zones_rev08		= jack_zones_rev08,
#endif
	.num_zones		= ARRAY_SIZE(jack_zones),
	.buttons_zones		= jack_buttons_zones,
#if defined (CONFIG_MACH_MELIUS_CHN_CTC)
	.buttons_zones_rev08	= jack_buttons_zones_rev08,
#elif defined (CONFIG_MACH_MELIUS_EUR_OPEN)
	.buttons_zones_rev06		= jack_buttons_zones_rev06,
#endif
	.num_buttons_zones	= ARRAY_SIZE(jack_buttons_zones),
	.det_gpio		= GPIO_EAR_DET,
	.send_end_gpio		= GPIO_EAR_SEND_END,
#if defined(CONFIG_SAMSUNG_JACK_GNDLDET)
	.get_gnd_jack_state	= get_sec_gnd_jack_state,
#endif
};

static struct platform_device sec_device_jack = {
	.name           = "sec_jack",
	.id             = -1,
	.dev            = {
		.platform_data  = &sec_jack_data,
	},
};
#endif /* SAMSUNG_JACK */
#ifdef CONFIG_VIBETONZ
static struct vibrator_platform_data msm_8930_vibrator_pdata = {
	.vib_model = HAPTIC_PWM,
	.vib_pwm_gpio = GPIO_VIB_PWM,
	.vib_en_gpio = GPIO_VIB_ON,
	.is_pmic_vib_en = 0,
	.is_pmic_haptic_pwr_en = 0,
	.is_no_haptic_pwr = 1, /* no need haptic_pwr_en_gpio */
#ifdef CONFIG_MOTOR_DRV_MAX77693
	.power_onoff = max77693_haptic_power_onoff,
#else
	.power_onoff = NULL,
#endif
};
static struct platform_device vibetonz_device = {
	.name = "tspdrv",
	.id = -1,
	.dev = {
		.platform_data = &msm_8930_vibrator_pdata ,
	},
};
#endif /* CONFIG_VIBETONZ */

static struct platform_device *early_common_devices[] __initdata = {
	&msm8960_device_dmov,
	&msm_device_smd,
#if defined (CONFIG_MACH_MELIUS_CHN_CTC) ||defined(CONFIG_MACH_CRATER_CHN_CTC)
	&msm8960_device_uart_gsbi2,
#else
	&msm8960_device_uart_gsbi5,
#endif
	&msm_device_uart_dm6,
	&msm_device_saw_core0,
	&msm_device_saw_core1,
};


/* ext_5v and ext_otg_sw are present when using PM8038 */
static struct platform_device *pmic_pm8038_devices[] __initdata = {
	&msm8930_device_ext_5v_vreg,
#ifndef MSM8930_PHASE_2
	&msm8930_device_ext_l2_vreg,
#endif
	&msm8960_device_ssbi_pmic,
#ifdef MSM8930_PHASE_2
	&msm8930_device_ext_otg_sw_vreg,
#endif
};

/* ext_5v and ext_otg_sw are not present when using PM8917 */
static struct platform_device *pmic_pm8917_devices[] __initdata = {
	&msm8960_device_ssbi_pmic,
};

static struct platform_device *common_devices[] __initdata = {
#ifndef CONFIG_SLIMBUS_MSM_CTRL
	&msm8960_device_qup_i2c_gsbi8,
#endif
#ifdef CONFIG_2MIC_ES305
#ifdef CONFIG_2MIC_QUP_I2C
#ifdef CONFIG_2MIC_QUP_I2C_GSBI2
	&msm8960_device_qup_i2c_gsbi2,
#else
	&msm8960_device_qup_i2c_gsbi5,
#endif
#else
	&a2220_i2c_gpio_device,
#endif
#endif
#ifdef CONFIG_GSM_MODEM_SPRD6500
	&msm8960_device_uart_gsbi12,
#endif
	&msm_8960_q6_lpass,
	&msm_8960_q6_mss_fw,
	&msm_8960_q6_mss_sw,
	&msm_8960_riva,
	&msm_pil_tzapps,
	&msm_pil_vidc,
#if !defined(CONFIG_GSM_MODEM_SPRD6500)
	&msm8960_device_qup_spi_gsbi1,
#endif
#if !defined(CONFIG_MACH_MELIUS_EUR_OPEN) && !defined(CONFIG_MACH_MELIUS_EUR_LTE)
//	&msm8960_device_qup_spi_gsbi2,
#endif
#if defined(CONFIG_GSM_MODEM_SPRD6500)
	&msm8960_device_qup_spi_gsbi5,
#endif
	&msm8960_device_qup_spi_gsbi11,
	&msm8960_device_qup_i2c_gsbi3,
	&msm8960_device_qup_i2c_gsbi4,
	&msm8960_device_qup_i2c_gsbi9,
#if !defined(CONFIG_GSM_MODEM_SPRD6500)
	&msm8960_device_qup_i2c_gsbi10,
	&msm8960_device_qup_i2c_gsbi12,
#endif
	&msm_slim_ctrl,
	&msm_device_wcnss_wlan,
#if defined(CONFIG_QSEECOM)
		&qseecom_device,
#endif
#ifdef CONFIG_SEC_FPGA
	&ice4_fpga_i2c_gpio_device,
#endif

#if defined(CONFIG_CRYPTO_DEV_QCRYPTO) || \
		defined(CONFIG_CRYPTO_DEV_QCRYPTO_MODULE)
	&qcrypto_device,
#endif

#if defined(CONFIG_CRYPTO_DEV_QCEDEV) || \
		defined(CONFIG_CRYPTO_DEV_QCEDEV_MODULE)
	&qcedev_device,
#endif
#ifdef CONFIG_MSM_ROTATOR
	&msm_rotator_device,
#endif
	&msm_device_sps,
#ifdef CONFIG_MSM_FAKE_BATTERY
	&fish_battery_device,
#endif
#ifdef CONFIG_ANDROID_PMEM
#ifndef CONFIG_MSM_MULTIMEDIA_USE_ION
	&msm8930_android_pmem_device,
	&msm8930_android_pmem_adsp_device,
	&msm8930_android_pmem_audio_device,
#endif /*CONFIG_MSM_MULTIMEDIA_USE_ION*/
#endif /*CONFIG_ANDROID_PMEM*/
	&msm8930_fmem_device,
	&msm_device_bam_dmux,
	&msm_fm_platform_init,

#ifdef CONFIG_HW_RANDOM_MSM
	&msm_device_rng,
#endif
	&msm8930_rpm_device,
	&msm8930_rpm_log_device,
	&msm8930_rpm_rbcpr_device,
	&msm8930_rpm_stat_device,
	&msm8930_rpm_master_stat_device,
#ifdef CONFIG_ION_MSM
	&msm8930_ion_dev,
#endif
	&msm_device_tz_log,
	&coresight_tpiu_device,
	&coresight_etb_device,
	&coresight_funnel_device,
	&coresight_etm0_device,
	&coresight_etm1_device,
	&msm_device_dspcrashd_8960,
	&msm8960_device_watchdog,
	&msm8930_rtb_device,
	&msm_bus_8930_apps_fabric,
	&msm_bus_8930_sys_fabric,
	&msm_bus_8930_mm_fabric,
	&msm_bus_8930_sys_fpb,
	&msm_bus_8930_cpss_fpb,
	&msm8960_device_cache_erp,
	&msm8930_iommu_domain_device,
	&msm_tsens_device,
	&msm8930_cache_dump_device,
	&msm8930_pc_cntr,
#if defined(CONFIG_MACH_MELIUS_CHN_CTC)|| defined(CONFIG_MACH_CRATER_CHN_CTC)
	&sns_i2c_gpio_device,
#endif
#if defined(CONFIG_SENSORS_GP2A030)
    &opt_gp2a,
#endif
#if defined(CONFIG_VIDEO_MHL_V2)
	&mhl_i2c_gpio_device,
#endif
#ifdef CONFIG_VIBETONZ
	&vibetonz_device,
#endif /* CONFIG_VIBETONZ */

#ifdef CONFIG_KEYBOARD_GPIO
	&msm8960_gpio_keys_device,
#endif
#ifdef CONFIG_BCM2079X_NFC_I2C
	&bcm2079x_i2c_gpio_device,
#endif
#ifdef CONFIG_MFD_MAX77693
	&max77693_i2c_gpio_device,
#endif
#ifdef CONFIG_MSM_ACTUATOR /* For Camera Actuator EEPROM By Teddy */
	&actuator_i2c_gpio_device,
#endif
#ifdef CONFIG_SEC_THERMISTOR
	&sec_device_thermistor,
#endif
#ifdef CONFIG_ANDROID_RAM_CONSOLE
	&ram_console_device,
#endif
};


static struct platform_device *express2_devices[] __initdata = {
	&msm8960_device_otg,
	&msm8960_device_gadget_peripheral,
	&msm_device_hsusb_host,
	&android_usb_device,
	&msm_pcm,
	&msm_pcm_routing,
#ifdef CONFIG_RADIO_USE_MI2S
	&msm_cpudai_mi2s,
#endif
#ifdef CONFIG_SLIMBUS_MSM_CTRL
	&msm_cpudai0,
	&msm_cpudai1,
#else
	&msm_i2s_cpudai0,
	&msm_i2s_cpudai1,
#endif
	&msm_cpudai_hdmi_rx,
	&msm_cpudai_bt_rx,
	&msm_cpudai_bt_tx,
	&msm_cpudai_fm_rx,
	&msm_cpudai_fm_tx,
	&msm_cpudai_auxpcm_rx,
	&msm_cpudai_auxpcm_tx,
	&msm_cpu_fe,
	&msm_stub_codec,
#ifdef CONFIG_MSM_GEMINI
	&msm8960_gemini_device,
#endif
	&msm_voice,
	&msm_voip,
	&msm_lpa_pcm,
	&msm_cpudai_afe_01_rx,
	&msm_cpudai_afe_01_tx,
	&msm_cpudai_afe_02_rx,
	&msm_cpudai_afe_02_tx,
	&msm_pcm_afe,
	&msm_compr_dsp,
#ifdef CONFIG_SAMSUNG_JACK
	&sec_device_jack,
#endif
	&msm_cpudai_incall_music_rx,
	&msm_cpudai_incall_record_rx,
	&msm_cpudai_incall_record_tx,
	&msm_pcm_hostless,
	&msm_multi_ch_pcm,
	&msm_lowlatency_pcm,
#ifdef CONFIG_USB_SWITCH_TSU6721
	&tsu_i2c_gpio_device,
#endif

};

static void __init msm8930_i2c_init(void)
{
#ifndef CONFIG_SLIMBUS_MSM_CTRL
	msm8960_device_qup_i2c_gsbi8.dev.platform_data =
					&msm8960_i2c_qup_gsbi8_pdata;
#endif
#if (defined CONFIG_2MIC_ES305) && (defined CONFIG_2MIC_QUP_I2C)
#ifdef CONFIG_2MIC_QUP_I2C_GSBI2
	msm8960_device_qup_i2c_gsbi2.dev.platform_data =
					&msm8960_i2c_qup_gsbi2_pdata;
#else
	msm8960_device_qup_i2c_gsbi5.dev.platform_data =
					&msm8960_i2c_qup_gsbi5_pdata;
#endif
#endif
	msm8960_device_qup_i2c_gsbi4.dev.platform_data =
					&msm8960_i2c_qup_gsbi4_pdata;

	msm8960_device_qup_i2c_gsbi3.dev.platform_data =
					&msm8960_i2c_qup_gsbi3_pdata;

	msm8960_device_qup_i2c_gsbi9.dev.platform_data =
					&msm8960_i2c_qup_gsbi9_pdata;

#if !defined(CONFIG_GSM_MODEM_SPRD6500)
	msm8960_device_qup_i2c_gsbi10.dev.platform_data =
					&msm8960_i2c_qup_gsbi10_pdata;
#endif
#if !defined(CONFIG_GSM_MODEM_SPRD6500)
	msm8960_device_qup_i2c_gsbi12.dev.platform_data =
					&msm8960_i2c_qup_gsbi12_pdata;
#endif
}

static struct msm_rpmrs_level msm_rpmrs_levels[] __initdata = {
	{
		MSM_PM_SLEEP_MODE_WAIT_FOR_INTERRUPT,
		MSM_RPMRS_LIMITS(ON, ACTIVE, MAX, ACTIVE),
		true,
		1, 784, 180000, 100,
	},

	{
		MSM_PM_SLEEP_MODE_RETENTION,
		MSM_RPMRS_LIMITS(ON, ACTIVE, MAX, ACTIVE),
		true,
		415, 715, 340827, 475,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE_STANDALONE,
		MSM_RPMRS_LIMITS(ON, ACTIVE, MAX, ACTIVE),
		true,
		1300, 228, 1200000, 2000,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(ON, GDHS, MAX, ACTIVE),
		false,
		2000, 138, 1208400, 3200,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(ON, HSFS_OPEN, ACTIVE, RET_HIGH),
		false,
		6000, 119, 1850300, 9000,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(OFF, GDHS, MAX, ACTIVE),
		false,
		9200, 68, 2839200, 16400,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(OFF, HSFS_OPEN, MAX, ACTIVE),
		false,
		10300, 63, 3128000, 18200,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(OFF, HSFS_OPEN, ACTIVE, RET_HIGH),
		false,
		18000, 10, 4602600, 27000,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(OFF, HSFS_OPEN, RET_HIGH, RET_LOW),
		false,
		20000, 2, 5752000, 32000,
	},
};

static struct msm_rpmrs_platform_data msm_rpmrs_data __initdata = {
	.levels = &msm_rpmrs_levels[0],
	.num_levels = ARRAY_SIZE(msm_rpmrs_levels),
	.vdd_mem_levels  = {
		[MSM_RPMRS_VDD_MEM_RET_LOW]	= 750000,
		[MSM_RPMRS_VDD_MEM_RET_HIGH]	= 750000,
		[MSM_RPMRS_VDD_MEM_ACTIVE]	= 1050000,
		[MSM_RPMRS_VDD_MEM_MAX]		= 1150000,
	},
	.vdd_dig_levels = {
		[MSM_RPMRS_VDD_DIG_RET_LOW]	= 0,
		[MSM_RPMRS_VDD_DIG_RET_HIGH]	= 0,
		[MSM_RPMRS_VDD_DIG_ACTIVE]	= 1,
		[MSM_RPMRS_VDD_DIG_MAX]		= 3,
	},
	.vdd_mask = 0x7FFFFF,
	.rpmrs_target_id = {
		[MSM_RPMRS_ID_PXO_CLK]		= MSM_RPM_ID_PXO_CLK,
		[MSM_RPMRS_ID_L2_CACHE_CTL]	= MSM_RPM_ID_LAST,
		[MSM_RPMRS_ID_VDD_DIG_0]	= MSM_RPM_ID_VOLTAGE_CORNER,
		[MSM_RPMRS_ID_VDD_DIG_1]	= MSM_RPM_ID_LAST,
		[MSM_RPMRS_ID_VDD_MEM_0]	= MSM_RPM_ID_PM8038_L24_0,
		[MSM_RPMRS_ID_VDD_MEM_1]	= MSM_RPM_ID_PM8038_L24_1,
		[MSM_RPMRS_ID_RPM_CTL]		= MSM_RPM_ID_RPM_CTL,
	},
};

static struct msm_rpmrs_platform_data msm_rpmrs_data_pm8917 __initdata = {
	.levels = &msm_rpmrs_levels[0],
	.num_levels = ARRAY_SIZE(msm_rpmrs_levels),
	.vdd_mem_levels  = {
		[MSM_RPMRS_VDD_MEM_RET_LOW]	= 750000,
		[MSM_RPMRS_VDD_MEM_RET_HIGH]	= 750000,
		[MSM_RPMRS_VDD_MEM_ACTIVE]	= 1050000,
		[MSM_RPMRS_VDD_MEM_MAX]		= 1150000,
	},
	.vdd_dig_levels = {
		[MSM_RPMRS_VDD_DIG_RET_LOW]	= 0,
		[MSM_RPMRS_VDD_DIG_RET_HIGH]	= 0,
		[MSM_RPMRS_VDD_DIG_ACTIVE]	= 1,
		[MSM_RPMRS_VDD_DIG_MAX]		= 3,
	},
	.vdd_mask = 0x7FFFFF,
	.rpmrs_target_id = {
		[MSM_RPMRS_ID_PXO_CLK]		= MSM_RPM_ID_PXO_CLK,
		[MSM_RPMRS_ID_L2_CACHE_CTL]	= MSM_RPM_ID_LAST,
		[MSM_RPMRS_ID_VDD_DIG_0]	= MSM_RPM_ID_VOLTAGE_CORNER,
		[MSM_RPMRS_ID_VDD_DIG_1]	= MSM_RPM_ID_LAST,
		[MSM_RPMRS_ID_VDD_MEM_0]	= MSM_RPM_ID_PM8917_L24_0,
		[MSM_RPMRS_ID_VDD_MEM_1]	= MSM_RPM_ID_PM8917_L24_1,
		[MSM_RPMRS_ID_RPM_CTL]		= MSM_RPM_ID_RPM_CTL,
	},
};

static struct msm_pm_boot_platform_data msm_pm_boot_pdata __initdata = {
	.mode = MSM_PM_BOOT_CONFIG_TZ,
};

#ifdef CONFIG_I2C

struct i2c_registry {
	int                    bus;
	struct i2c_board_info *info;
	int                    len;
};

#ifdef CONFIG_INPUT_MPU3050
#define MPU3050_INT_GPIO		69

static struct mpu3050_gyro_platform_data mpu3050_gyro = {
	.gpio_int = MPU3050_INT_GPIO,
};

static struct i2c_board_info __initdata mpu3050_i2c_boardinfo[] = {
	{
		I2C_BOARD_INFO("mpu3050", 0x68),
		.irq = MSM_GPIO_TO_INT(MPU3050_INT_GPIO),
		.platform_data = &mpu3050_gyro,
	},
};
#endif

#ifdef CONFIG_ISL9519_CHARGER
static struct isl_platform_data isl_data __initdata = {
	.valid_n_gpio		= 0,	/* Not required when notify-by-pmic */
	.chg_detection_config	= NULL,	/* Not required when notify-by-pmic */
	.max_system_voltage	= 4200,
	.min_system_voltage	= 3200,
	.chgcurrent		= 1000, /* 1900, */
	.term_current		= 400,	/* Need fine tuning */
	.input_current		= 2048,
};

static struct i2c_board_info isl_charger_i2c_info[] __initdata = {
	{
		I2C_BOARD_INFO("isl9519q", 0x9),
		.irq		= 0,	/* Not required when notify-by-pmic */
		.platform_data	= &isl_data,
	},
};
#endif /* CONFIG_ISL9519_CHARGER */

#ifdef CONFIG_STM_LIS3DH
static struct lis3dh_acc_platform_data lis3dh_accel = {
	.poll_interval = 200,
	.min_interval = 10,
	.g_range = LIS3DH_ACC_G_2G,
	.axis_map_x = 1,
	.axis_map_y = 0,
	.axis_map_z = 2,
	.negate_x = 0,
	.negate_y = 0,
	.negate_z = 1,
	.init = NULL,
	.exit = NULL,
	.gpio_int1 = -EINVAL,
	.gpio_int2 = -EINVAL,
};

static struct i2c_board_info __initdata lis3dh_i2c_boardinfo[] = {
	{
		I2C_BOARD_INFO(LIS3DH_ACC_DEV_NAME, 0x18),
		.platform_data = &lis3dh_accel,
	},
};
#endif /* CONFIG_STM_LIS3DH */

#ifdef CONFIG_SAMSUNG_CMC624
static struct i2c_board_info __initdata cmc624_i2c_borad_info[] = {
	{
		I2C_BOARD_INFO("cmc624", 0x38),
	},
};
#endif
static struct i2c_registry msm8930_i2c_devices[] __initdata = {
#ifdef CONFIG_ISL9519_CHARGER
	{
		MSM_8930_GSBI10_QUP_I2C_BUS_ID,
		isl_charger_i2c_info,
		ARRAY_SIZE(isl_charger_i2c_info),
	},
#endif /* CONFIG_ISL9519_CHARGER */
#if defined(CONFIG_VIDEO_MHL_V2)
	{
		MSM_MHL_I2C_BUS_ID,
		mhl_i2c_board_info,
		ARRAY_SIZE(mhl_i2c_board_info),
	},
#endif
#ifdef CONFIG_USB_SWITCH_TSU6721
	{
		MSM_TSU6721_I2C_BUS_ID,
		micro_usb_i2c_devices_info,
		ARRAY_SIZE(micro_usb_i2c_devices_info),
	},
#endif
#ifdef CONFIG_MFD_MAX77693
	{
		MSM_MUSB_I2C_BUS_ID,
		max77693_i2c_board_info,
		ARRAY_SIZE(max77693_i2c_board_info),
	},
#endif
#ifdef CONFIG_INPUT_MPU3050
	{
		MSM_8930_GSBI12_QUP_I2C_BUS_ID,
		mpu3050_i2c_boardinfo,
		ARRAY_SIZE(mpu3050_i2c_boardinfo),
	},
#endif
#ifdef CONFIG_BCM2079X_NFC_I2C
	{
		MSM_NFC_I2C_BUS_ID,
		nfc_bcm2079x_info,
		ARRAY_SIZE(nfc_bcm2079x_info),
	},
#endif
#ifdef CONFIG_OPTICAL_TAOS_TRITON
	{
		MSM_SNS_I2C_BUS_ID,
		opt_i2c_board_info,
		ARRAY_SIZE(opt_i2c_board_info),
	},
#endif
#ifndef CONFIG_SLIMBUS_MSM_CTRL
	{
		MSM_8960_GSBI8_QUP_I2C_BUS_ID,
		sitar_device_info,
		ARRAY_SIZE(sitar_device_info),
	},
#endif
#ifdef CONFIG_2MIC_ES305
	{
#ifdef CONFIG_2MIC_QUP_I2C
#ifdef CONFIG_2MIC_QUP_I2C_GSBI2
		MSM_8960_GSBI2_QUP_I2C_BUS_ID,
#else
		MSM_8960_GSBI5_QUP_I2C_BUS_ID,
#endif
#else
		MSM_A2220_I2C_BUS_ID,
#endif
		a2220_device,
		ARRAY_SIZE(a2220_device),
	},
#endif
#ifdef CONFIG_FB_MSM_HDMI_MHL_8334
	{
		MSM_8930_GSBI9_QUP_I2C_BUS_ID,
		sii_device_info,
		ARRAY_SIZE(sii_device_info),
	},
#endif
#ifdef CONFIG_STM_LIS3DH
	{
		MSM_8930_GSBI12_QUP_I2C_BUS_ID,
		lis3dh_i2c_boardinfo,
		ARRAY_SIZE(lis3dh_i2c_boardinfo),
	},
#endif
#ifdef CONFIG_SENSORS_GP2A030
	{
		MSM_SNS_I2C_BUS_ID,
		opt_i2c_board_info,
		ARRAY_SIZE(opt_i2c_board_info),
	},
#endif
#ifdef CONFIG_OPTICAL_TAOS_TMD3782
	{
		MSM_SNS_I2C_BUS_ID,
		opt_i2c_board_info,
		ARRAY_SIZE(opt_i2c_board_info),
	},
#endif
#ifdef CONFIG_SEC_FPGA
	{
		MSM_SEC_FPGA_I2C_BUS_ID,
		ice4_fpga_i2c_board_info,
		ARRAY_SIZE(ice4_fpga_i2c_board_info),
	},
#endif
#if defined(CONFIG_INPUT_MPU6050) || defined(CONFIG_INPUT_MPU6500) || \
	defined(CONFIG_INPUT_YAS_SENSORS)
	{
		MSM_SNS_I2C_BUS_ID,
		sns_i2c_board_info,
		ARRAY_SIZE(sns_i2c_board_info),
	},
#endif
};
#endif /* CONFIG_I2C */

static void __init register_i2c_devices(void)
{
#ifdef CONFIG_I2C
	int i;
#ifdef CONFIG_MSM_CAMERA
	struct i2c_registry msm8930_camera_i2c_devices = {
		MSM_8930_GSBI4_QUP_I2C_BUS_ID,
		msm8930_camera_board_info.board_info,
		msm8930_camera_board_info.num_i2c_board_info,
	};
#endif

#ifdef CONFIG_SAMSUNG_CMC624
	struct i2c_registry cmc624_i2c_devices = {
		MSM_CMC624_I2C_BUS_ID,
		cmc624_i2c_borad_info,
		ARRAY_SIZE(cmc624_i2c_borad_info),
	};
#endif
	/* Run the array and install devices as appropriate */
	for (i = 0; i < ARRAY_SIZE(msm8930_i2c_devices); ++i) {
			i2c_register_board_info(msm8930_i2c_devices[i].bus,
						msm8930_i2c_devices[i].info,
						msm8930_i2c_devices[i].len);
	}
#ifdef CONFIG_MSM_CAMERA
	i2c_register_board_info(msm8930_camera_i2c_devices.bus,
		msm8930_camera_i2c_devices.info,
		msm8930_camera_i2c_devices.len);
#endif

#ifdef CONFIG_SAMSUNG_CMC624
	i2c_register_board_info(cmc624_i2c_devices.bus,
		cmc624_i2c_devices.info,
		cmc624_i2c_devices.len);
#endif
#endif
}

#ifdef CONFIG_SAMSUNG_JACK
/*TODO: will fix
static struct pm_gpio ear_micbiase = {
	.direction		= PM_GPIO_DIR_OUT,
	.pull			= PM_GPIO_PULL_NO,
	.out_strength	= PM_GPIO_STRENGTH_HIGH,
	.function		= PM_GPIO_FUNC_NORMAL,
	.inv_int_pol	= 0,
	.vin_sel		= PM_GPIO_VIN_S4,
	.output_buffer	= PM_GPIO_OUT_BUF_CMOS,
	.output_value	= 0,
};
*/

static int secjack_gpio_init(void)
{
	int rc;

	rc = gpio_tlmm_config(GPIO_CFG(GPIO_EAR_DET, 0, GPIO_CFG_INPUT,
			GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	if (rc) {
		pr_err("%s GPIO_EAR_DET config failed\n", __func__);
		return rc;
	}
/*
	rc = gpio_tlmm_config(GPIO_CFG(GPIO_EAR_GND, 0, GPIO_CFG_INPUT,
			GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	if (rc) {
		pr_err("%s GPIO_EAR_GND config failed\n", __func__);
		return rc;
	}
*/
	rc = gpio_tlmm_config(GPIO_CFG(GPIO_EAR_SEND_END, 0, GPIO_CFG_INPUT,
			GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	if (rc) {
		pr_err("%s GPIO_EAR_SEND_END config failed\n", __func__);
		return rc;
	}
/*TODO: will fix
	rc = gpio_request(
		PM8038_GPIO_PM_TO_SYS(PMIC_GPIO_EAR_MICBIAS_EN),
			"EAR_MICBIAS");
	if (rc) {
		pr_err("failed to request ear micbias gpio\n");
		gpio_free(PM8038_GPIO_PM_TO_SYS(PMIC_GPIO_EAR_MICBIAS_EN));
		return rc;
	}
	rc = pm8xxx_gpio_config(
			PM8038_GPIO_PM_TO_SYS(PMIC_GPIO_EAR_MICBIAS_EN),
			&ear_micbiase);
	if (rc) {
		pr_err("%s PMIC_GPIO_EAR_MICBIAS_EN config failed\n", __func__);
		return rc;
	} else {
		gpio_direction_output(PM8038_GPIO_PM_TO_SYS(
			PMIC_GPIO_EAR_MICBIAS_EN), 0);
	}
*/
	return rc;
}
#endif
#ifdef CONFIG_SENSORS_HALL
void Hall_IC_init(void)
{
	int ret;
	printk(KERN_DEBUG "[Hall_IC]START %s\n", __func__);
	ret = gpio_request(GPIO_HALL_IC_IRQ, "GPIO_HALL_SENSOR_INT");
	if (ret != 0) {
		printk(KERN_ERR"Hall int request failed, ret=%d", ret);
		goto err_int_gpio_request;
	}
	gpio_tlmm_config(GPIO_CFG(GPIO_HALL_IC_IRQ, 0,
		GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);

	return;

err_int_gpio_request:
	gpio_free(GPIO_HALL_IC_IRQ);

}
#endif
#ifdef CONFIG_SLIMBUS_MSM_CTRL
static void __init msm8930_pm8917_wcd9xxx_pdata_fixup(
		struct wcd9xxx_pdata *cdc_pdata)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(cdc_pdata->regulator); i++) {

		if (cdc_pdata->regulator[i].name != NULL
			&& strncmp(cdc_pdata->regulator[i].name,
				"CDC_VDD_CP", 10) == 0) {
			cdc_pdata->regulator[i].min_uV =
				cdc_pdata->regulator[i].max_uV = 1800000;
			pr_info("%s: CDC_VDD_CP forced to 1.8 volts for PM8917\n",
				__func__);
			return;
		}
	}
}
#endif
/* Modify platform data values to match requirements for PM8917. */
static void __init msm8930_pm8917_pdata_fixup(void)
{
	struct acpuclk_platform_data *pdata;
#ifdef CONFIG_SLIMBUS_MSM_CTRL
	msm8930_pm8917_wcd9xxx_pdata_fixup(&sitar_platform_data);
	msm8930_pm8917_wcd9xxx_pdata_fixup(&sitar1p1_platform_data);
#endif
#ifdef CONFIG_FB_MSM_HDMI_MHL_8334
	mhl_platform_data.gpio_mhl_power = MHL_POWER_GPIO_PM8917;
#endif
	msm_device_saw_core0.dev.platform_data
		= &msm8930_pm8038_saw_regulator_core0_pdata;
	msm_device_saw_core1.dev.platform_data
		= &msm8930_pm8038_saw_regulator_core1_pdata;

	msm8930_device_rpm_regulator.dev.platform_data
		= &msm8930_pm8917_rpm_regulator_pdata;

	pdata = msm8930_device_acpuclk.dev.platform_data;
	pdata->uses_pm8917 = true;

	pdata = msm8930ab_device_acpuclk.dev.platform_data;
	pdata->uses_pm8917 = true;
}

static void __init msm8930ab_update_retention_spm(void)
{
	int i;

	/* Update the SPM sequences for krait retention on all cores */
	for (i = 0; i < ARRAY_SIZE(msm_spm_data); i++) {
		int j;
		struct msm_spm_platform_data *pdata = &msm_spm_data[i];
		for (j = 0; j < pdata->num_modes; j++) {
			if (pdata->modes[j].cmd ==
					spm_retention_cmd_sequence)
				pdata->modes[j].cmd =
				spm_retention_with_krait_v3_cmd_sequence;
		}
	}
}

static void __init gpio_rev_init(void)
{


}

#ifdef CONFIG_SAMSUNG_CMC624
static struct i2c_gpio_platform_data cmc624_i2c_gpio_data = {
	.sda_pin    = GPIO_IMA_I2C_SDA,
	.scl_pin    = GPIO_IMA_I2C_SCL,
	.udelay		= 5,
	.sda_is_open_drain	= 0,
	.scl_is_open_drain	= 0,
	.scl_is_output_only	= 0,
};

static struct platform_device cmc624_i2c_gpio_device = {
	.name       = "i2c-gpio",
	.id     = MSM_CMC624_I2C_BUS_ID,
	.dev.platform_data  = &cmc624_i2c_gpio_data,
};
#endif

#ifdef CONFIG_CHARGER_BQ24157

static sec_battery_platform_data_t sec_battery_pdata_bq24157 = {
		/* Charger */
	.chg_gpio_en = 0,
	.chg_polarity_en = 0,
	.chg_gpio_status = 0,
	.chg_polarity_status = 0,
	.chg_irq = 0,
	.chg_irq_attr = 0,
	.chg_float_voltage = 4350,
};



static struct i2c_gpio_platform_data gpio_i2c_data_fgchg = {
	.sda_pin = GPIO_FUELGAUGE_I2C_SDA,
	.scl_pin = GPIO_FUELGAUGE_I2C_SCL,
};


struct platform_device sec_device_fgchg_bq24157 = {
	.name = "i2c-gpio",
	.id = MSM_FUELGAUGE_I2C_BUS_ID,
	.dev.platform_data = &gpio_i2c_data_fgchg,
/*
	.num_resources=ARRAY_SIZE(msm8960_battery_devices),
	.resource=msm8960_battery_devices,
*/
};

/*
static struct platform_device *msm8960_battery_devices[] __initdata = {
	&sec_device_fgchg,
};
*/

static struct i2c_board_info sec_brdinfo_fgchg[] __initdata = {
	{
		I2C_BOARD_INFO("sec-charger",
			SEC_CHARGER_I2C_SLAVEADDR),
		.platform_data	= &sec_battery_pdata_bq24157,
	},
/*
	{
		I2C_BOARD_INFO("sec-fuelgauge",
			SEC_FUELGAUGE_I2C_SLAVEADDR),
		.platform_data	= &sec_battery_pdata,
	},
*/
};




void __init msm8930_init_battery_bq24157(void)
{
	gpio_tlmm_config(GPIO_CFG(GPIO_FUELGAUGE_I2C_SCL, 0, GPIO_CFG_OUTPUT,
		 GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);
	gpio_tlmm_config(GPIO_CFG(GPIO_FUELGAUGE_I2C_SDA,  0, GPIO_CFG_OUTPUT,
		 GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);
	gpio_set_value(GPIO_FUELGAUGE_I2C_SCL, 1);
	gpio_set_value(GPIO_FUELGAUGE_I2C_SDA, 1);

	/* board dependent changes in booting */
	platform_device_register(&sec_device_fgchg_bq24157);
	/* platform_add_devices(msm8960_battery_devices,
	 * ARRAY_SIZE(msm8960_battery_devices)); */
	i2c_register_board_info(MSM_FUELGAUGE_I2C_BUS_ID, sec_brdinfo_fgchg,
			ARRAY_SIZE(sec_brdinfo_fgchg));
}

#endif

extern void __init S5000_tsp_input_init(void);
#if defined(CONFIG_MIPI_SAMSUNG_ESD_REFRESH)
extern void set_esd_gpio_config(void);
#endif

void __init msm8930_melius_init(void)
{

#ifdef CONFIG_SEC_DEBUG
	sec_debug_init();
#endif

#ifdef CONFIG_PROC_AVC
	sec_avc_log_init();
#endif

	if (socinfo_get_pmic_model() == PMIC_MODEL_PM8917)
		msm8930_pm8917_pdata_fixup();
	if (meminfo_init(SYS_MEMORY, SZ_256M) < 0)
		pr_err("meminfo_init() failed!\n");

	platform_device_register(&msm_gpio_device);
	msm_tsens_early_init(&msm_tsens_pdata);
	msm_thermal_init(&msm_thermal_pdata);
	if (socinfo_get_pmic_model() != PMIC_MODEL_PM8917) {
		BUG_ON(msm_rpm_init(&msm8930_rpm_data));
		BUG_ON(msm_rpmrs_levels_init(&msm_rpmrs_data));
	} else {
		BUG_ON(msm_rpm_init(&msm8930_rpm_data_pm8917));
		BUG_ON(msm_rpmrs_levels_init(&msm_rpmrs_data_pm8917));
	}

	gpio_rev_init();
	regulator_suppress_info_printing();
	if (msm_xo_init())
		pr_err("Failed to initialize XO votes\n");
	platform_device_register(&msm8930_device_rpm_regulator);
	if (socinfo_get_pmic_model() == PMIC_MODEL_PM8917)
		msm_clock_init(&msm8930_pm8917_clock_init_data);
	else
		msm_clock_init(&msm8930_clock_init_data);
	msm_otg_pdata.phy_init_seq = hsusb_phy_init_seq;
	msm8960_device_otg.dev.platform_data = &msm_otg_pdata;
	android_usb_pdata.swfi_latency =
			msm_rpmrs_levels[0].latency_us;
	msm8930_init_gpiomux();

#if !defined(CONFIG_GSM_MODEM_SPRD6500)
	msm8960_device_qup_spi_gsbi1.dev.platform_data =
				&msm8960_qup_spi_gsbi1_pdata;
#endif
#if defined(CONFIG_GSM_MODEM_SPRD6500)
	msm8960_device_qup_spi_gsbi5.dev.platform_data =
				&msm8960_qup_spi_gsbi5_pdata;
#endif
//	msm8960_device_qup_spi_gsbi2.dev.platform_data =
//				&msm8960_qup_spi_gsbi2_pdata;
	msm8960_device_qup_spi_gsbi11.dev.platform_data =
				&msm8960_qup_spi_gsbi11_pdata;
#ifdef CONFIG_KS8851
	spi_register_board_info(spi_board_info, ARRAY_SIZE(spi_board_info));
#endif
#ifdef CONFIG_IMX175_EEPROM /* For Camera Actuator EEPROM By Teddy */
#if defined(CONFIG_MACH_MELIUS_SKT) || defined(CONFIG_MACH_MELIUS_KTT) || \
	defined(CONFIG_MACH_MELIUS_LGT)
	if(system_rev >= 0x9)
		spi_register_board_info(eeprom_spi_info_kor, ARRAY_SIZE(eeprom_spi_info_kor));
	else
#endif
	spi_register_board_info(eeprom_spi_info, ARRAY_SIZE(eeprom_spi_info));
#endif
	/*
	 * TODO: When physical 8930/PM8038 hardware becomes
	 * available, remove this block or add the config
	 * option.
	 */
#ifndef MSM8930_PHASE_2
	msm8960_init_pmic();
#else
	msm8930_init_pmic();
#endif
	msm8930_i2c_init();
	msm8930_init_gpu();
	if (cpu_is_krait_v3()) {
		msm_pm_set_tz_retention_flag(0);
		msm8930ab_update_retention_spm();
	} else {
		msm_pm_set_tz_retention_flag(1);
	}
	msm_spm_init(msm_spm_data, ARRAY_SIZE(msm_spm_data));
	msm_spm_l2_init(msm_spm_l2_data);
	msm8930_init_buses();
	if (cpu_is_msm8627()) {
		platform_add_devices(msm8627_footswitch,
				msm8627_num_footswitch);
	} else {
		if (socinfo_get_pmic_model() == PMIC_MODEL_PM8917)
			platform_add_devices(msm8930_pm8917_footswitch,
					msm8930_pm8917_num_footswitch);
		else
			platform_add_devices(msm8930_footswitch,
					msm8930_num_footswitch);
	}
	if (cpu_is_msm8627())
		platform_device_register(&msm8627_device_acpuclk);
	else if (cpu_is_msm8930())
		platform_device_register(&msm8930_device_acpuclk);
	else if (cpu_is_msm8930aa())
		platform_device_register(&msm8930aa_device_acpuclk);
	else if (cpu_is_msm8930ab())
		platform_device_register(&msm8930ab_device_acpuclk);

	platform_add_devices(early_common_devices,
				ARRAY_SIZE(early_common_devices));
	if (socinfo_get_pmic_model() != PMIC_MODEL_PM8917)
		platform_add_devices(pmic_pm8038_devices,
					ARRAY_SIZE(pmic_pm8038_devices));
	else
		platform_add_devices(pmic_pm8917_devices,
					ARRAY_SIZE(pmic_pm8917_devices));
	platform_add_devices(common_devices, ARRAY_SIZE(common_devices));
	msm8930_add_vidc_device();
	/*
	 * TODO: When physical 8930/PM8038 hardware becomes
	 * available, remove this block or add the config
	 * option.
	 */
#ifndef MSM8930_PHASE_2
	msm8960_pm8921_gpio_mpp_init();
#else
	if (socinfo_get_pmic_model() != PMIC_MODEL_PM8917)
		msm8930_pm8038_gpio_mpp_init();
	else
		msm8930_pm8917_gpio_mpp_init();
#endif
	platform_add_devices(express2_devices, ARRAY_SIZE(express2_devices));
#ifdef CONFIG_MSM_CAMERA
	msm8930_init_cam();
#endif
#ifdef CONFIG_SAMSUNG_CMC624
	platform_device_register(&cmc624_i2c_gpio_device);
#endif
	msm8930_init_mmc();
	register_i2c_devices();
	msm8930_init_fb();
#ifdef CONFIG_SAMSUNG_JACK
	secjack_gpio_init();
#endif
#ifdef CONFIG_FM_SI4705
	fmradio_gpio_init();
#endif
#if defined(CONFIG_MACH_CRATER_CHN_CTC)||defined(CONFIG_MACH_MELIUS_CHN_CTC)
	sensor_gpio_init();
#endif
#ifdef CONFIG_SLIMBUS_MSM_CTRL
	slim_register_board_info(msm_slim_devices,
		ARRAY_SIZE(msm_slim_devices));
#endif /* CONFIG_SLIMBUS_MSM_CTRL */
	BUG_ON(msm_pm_boot_init(&msm_pm_boot_pdata));
	samsung_sys_class_init();
#if defined(CONFIG_VIDEO_MHL_V2)
	msm8930_mhl_gpio_init();
#endif
#ifdef CONFIG_BCM2079X_NFC_I2C
	bcm2079x_init();
#endif
#ifdef CONFIG_INPUT_YAS_SENSORS
	sensor_device_init();
#endif
#if defined(CONFIG_OPTICAL_GP2AP020A00F) || \
	defined(CONFIG_OPTICAL_TAOS_TMD3782) || \
	defined(CONFIG_OPTICAL_TAOS_TRITON)	|| \
	defined(CONFIG_SENSORS_GP2A030)
	opt_init();
#endif
	if (PLATFORM_IS_CHARM25())
		platform_add_devices(mdm_devices, ARRAY_SIZE(mdm_devices));
#ifdef CONFIG_TOUCHSCREEN_MXT224S
	input_touchscreen_init();
#endif
#if defined(CONFIG_MIPI_SAMSUNG_ESD_REFRESH)
	set_esd_gpio_config();
#endif
#ifdef CONFIG_TOUCHSCREEN_SYNAPTICS_I2C_RMI
	S5000_tsp_input_init();
#endif
#ifdef CONFIG_TOUCHSCREEN_ZINITIX_BT532
	gpio_tlmm_config(GPIO_CFG(18, 0,
		GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);
	i2c_register_board_info(MSM_8930_GSBI3_QUP_I2C_BUS_ID, zinitix_i2c_info,
		ARRAY_SIZE(zinitix_i2c_info));
#endif	
#ifdef CONFIG_SENSORS_HALL
	Hall_IC_init();
#endif
#ifdef CONFIG_KEYBOARD_CYPRESS_TOUCH
	input_touchkey_init();
#endif

#ifdef CONFIG_MFD_MAX77693
	gpio_tlmm_config(GPIO_CFG(GPIO_IF_PMIC_IRQ,  0, GPIO_CFG_INPUT,
		GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_DISABLE);
	gpio_tlmm_config(GPIO_CFG(GPIO_IF_PMIC_SDA,  0, GPIO_CFG_INPUT,
		GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_DISABLE);
	gpio_tlmm_config(GPIO_CFG(GPIO_IF_PMIC_SCL,  0, GPIO_CFG_INPUT,
		GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_DISABLE);

	msm8960_init_battery();
	pr_info("%s msm8960_init_battery\n", __func__);

#endif

#ifdef CONFIG_CHARGER_BQ24157
	msm8930_init_battery_bq24157();
	pr_info("%s msm8930_init_battery_bq24157\n", __func__);
#endif
#if defined(CONFIG_TDMB) || defined(CONFIG_TDMB_MODULE)
	tdmb_dev_init();
#endif

#ifdef CONFIG_USB_HOST_NOTIFY
//	msm_otg_power_init(GPIO_OTG_TEST, 0);
#endif
}

MACHINE_START(MELIUS, "SAMSUNG MELIUS")
	.map_io = msm8930_map_io,
	.reserve = msm8930_reserve,
	.init_irq = msm8930_init_irq,
	.handle_irq = gic_handle_irq,
	.timer = &msm_timer,
	.init_machine = msm8930_melius_init,
	.init_early = msm8930_allocate_memory_regions,
	.init_very_early = msm8930_early_memory,
	.restart = msm_restart,
MACHINE_END
