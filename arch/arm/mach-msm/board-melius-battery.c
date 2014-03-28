/*
 * Copyright (C) 2012 Samsung Electronics, Inc.
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
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/i2c.h>
#include <linux/i2c-gpio.h>
#include <linux/gpio.h>
#include <linux/mfd/pm8xxx/pm8921.h>
#include <linux/mfd/pm8xxx/pm8xxx-adc.h>
#include <linux/mfd/max77693-private.h>

#include <mach/board.h>
#include <mach/gpio.h>
#include <mach/msm8930-gpio.h>

#include "devices-msm8x60.h"
#include "board-8930.h"

#if defined(CONFIG_BATTERY_SAMSUNG)
#include <linux/battery/sec_battery.h>
#include <linux/battery/sec_fuelgauge.h>
#include <linux/battery/sec_charger.h>

#define SEC_BATTERY_PMIC_NAME ""

sec_battery_platform_data_t sec_battery_pdata;
static unsigned int sec_bat_recovery_mode;

#if defined(CONFIG_MACH_MELIUS)
#if defined(CONFIG_MACH_CRATER_CHN_CTC)
static sec_charging_current_t charging_current_table[] = {
	{1900,	1600,	200,	40*60},	/* Unknown */
	{0,	0,	0,	0},					/* Battery */
	{0,	0,	0,	0},					/* UPS */
	{1900,	1600,	200,	40*60},	/* MAINS */
	{460,	460,	200,	40*60},	/* USB */
	{460,	460,	200,	40*60},	/* USB_DCP */
	{1000,	1000,	200,	40*60},	/* USB_CDP */
	{460,	460,	200,	40*60},	/* USB_ACA */
	{1700,	1600,	200,	40*60},	/* MISC */
	{0,	0,	0,	0},					/* Cardock */
	{500,	500,	200,	40*60},	/* Wireless */
	{1900,	1600,	200,	40*60},	/* UartOff */
	{0,	0,	0,	0},					/* OTG */
	{0,	0,	0,	0},					/* BMS */
};
#else
static sec_charging_current_t charging_current_table[] = {
	{1900,	2100,	200,	40*60},	/* Unknown */
	{0,	0,	0,	0},					/* Battery */
	{0,	0,	0,	0},					/* UPS */
	{1900,	2100,	200,	40*60},	/* MAINS */
	{460,	460,	200,	40*60},	/* USB */
	{460,	460,	200,	40*60},	/* USB_DCP */
	{1000,	1000,	200,	40*60},	/* USB_CDP */
	{460,	460,	200,	40*60},	/* USB_ACA */
	{1700,	2100,	200,	40*60},	/* MISC */
	{0,	0,	0,	0},					/* Cardock */
	{500,	500,	200,	40*60},	/* Wireless */
	{1900,	2100,	200,	40*60},	/* UartOff */
	{0,	0,	0,	0},					/* OTG */
	{0,	0,	0,	0},					/* BMS */
};
#endif
#else
static sec_charging_current_t charging_current_table[] = {
	{1800,	2100,	200,	40*60},	/* Unknown */
	{0,	0,	0,	0},					/* Battery */
	{0,	0,	0,	0},					/* UPS */
	{1800,	2100,	200,	40*60},	/* MAINS */
	{460,	460,	200,	40*60},	/* USB */
	{460,	460,	200,	40*60},	/* USB_DCP */
	{1000,	1000,	200,	40*60},	/* USB_CDP */
	{460,	460,	200,	40*60},	/* USB_ACA */
	{1700,	2100,	200,	40*60},	/* MISC */
	{0,	0,	0,	0},					/* Cardock */
	{500,	500,	200,	40*60},	/* Wireless */
	{1800,	2100,	200,	40*60},	/* UartOff */
	{0,	0,	0,	0},					/* OTG */
	{0,	0,	0,	0},					/* BMS */
};
#endif

static bool sec_bat_adc_none_init(
		struct platform_device *pdev) {return true; }
static bool sec_bat_adc_none_exit(void) {return true; }
static int sec_bat_adc_none_read(unsigned int channel) {return 0; }

static bool sec_bat_adc_ap_init(
		struct platform_device *pdev) {return true; }
static bool sec_bat_adc_ap_exit(void) {return true; }

static int sec_bat_adc_ap_read(unsigned int channel)
{
	int rc = -1, data = -1;
	struct pm8xxx_adc_chan_result result;

	switch (channel) {
	case SEC_BAT_ADC_CHANNEL_TEMP:
	case SEC_BAT_ADC_CHANNEL_TEMP_AMBIENT:
		rc = pm8xxx_adc_mpp_config_read(
			PM8XXX_AMUX_MPP_3, ADC_MPP_1_AMUX6, &result);
		if (rc) {
			pr_err("error reading mpp %d, rc = %d\n",
				PM8XXX_AMUX_MPP_3, rc);
			return rc;
		}

		/* use measurement, no need to scale */
		data = (int)result.measurement;
		break;

	default:
		pr_err("Invalid adc channel: %d\n", channel);
		break;
	}

	return data;
}

static bool sec_bat_adc_ic_init(
		struct platform_device *pdev) {return true; }
static bool sec_bat_adc_ic_exit(void) {return true; }
static int sec_bat_adc_ic_read(unsigned int channel) {return 0; }

static bool sec_bat_gpio_init(void)
{
	return true;
}

static struct i2c_gpio_platform_data gpio_i2c_data_fgchg = {
	.sda_pin = GPIO_FUELGAUGE_I2C_SDA,
	.scl_pin = GPIO_FUELGAUGE_I2C_SCL,
};

static bool sec_fg_gpio_init(void)
{
	sec_battery_pdata.fg_irq = MSM_GPIO_TO_INT(GPIO_FUEL_INT);
#if defined(CONFIG_MACH_MELIUS_CHN_CTC) ||  defined(CONFIG_MACH_CRATER_CHN_CTC)
	gpio_tlmm_config(GPIO_CFG(GPIO_FUEL_INT, 0,
			GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);
#else
	gpio_tlmm_config(GPIO_CFG(gpio_i2c_data_fgchg.scl_pin, 0,
			GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);
#endif

	/* FUEL_ALERT Setting */
	pr_info("%s:system_rev (%d)\n",__func__, system_rev);

	gpio_tlmm_config(GPIO_CFG(gpio_i2c_data_fgchg.scl_pin, 0,
			GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);
	gpio_tlmm_config(GPIO_CFG(gpio_i2c_data_fgchg.sda_pin,  0,
			GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);
	gpio_set_value(gpio_i2c_data_fgchg.scl_pin, 1);
	gpio_set_value(gpio_i2c_data_fgchg.sda_pin, 1);

	return true;
}

static bool sec_chg_gpio_init(void)
{
	return true;
}

static bool sec_bat_is_lpm(void) {return (bool)0; }//poweroff_charging; }

int extended_cable_type;

static void sec_bat_initial_check(void)
{
	union power_supply_propval value;

	if (POWER_SUPPLY_TYPE_BATTERY < current_cable_type) {
		value.intval = current_cable_type<<ONLINE_TYPE_MAIN_SHIFT;
		psy_do_property("battery", set,
				POWER_SUPPLY_PROP_ONLINE, value);
	} else {
		psy_do_property("sec-charger", get,
				POWER_SUPPLY_PROP_ONLINE, value);
		if (value.intval == POWER_SUPPLY_TYPE_WIRELESS) {
			value.intval =
			POWER_SUPPLY_TYPE_WIRELESS<<ONLINE_TYPE_MAIN_SHIFT;
			psy_do_property("battery", set,
					POWER_SUPPLY_PROP_ONLINE, value);
		}
	}
}

static bool sec_bat_check_jig_status(void)
{
	return (current_cable_type == POWER_SUPPLY_TYPE_UARTOFF);
}
static bool sec_bat_switch_to_check(void) {return true; }
static bool sec_bat_switch_to_normal(void) {return true; }

static int sec_bat_check_cable_callback(void)
{
	return current_cable_type;
}

static int sec_bat_get_cable_from_extended_cable_type(
	int input_extended_cable_type)
{
	int cable_main, cable_sub, cable_power;
	int cable_type = POWER_SUPPLY_TYPE_UNKNOWN;
	union power_supply_propval value;
	int charge_current_max = 0, charge_current = 0;

	cable_main = GET_MAIN_CABLE_TYPE(input_extended_cable_type);
	if (cable_main != POWER_SUPPLY_TYPE_UNKNOWN)
		extended_cable_type = (extended_cable_type &
			~(int)ONLINE_TYPE_MAIN_MASK) |
			(cable_main << ONLINE_TYPE_MAIN_SHIFT);
	cable_sub = GET_SUB_CABLE_TYPE(input_extended_cable_type);
	if (cable_sub != ONLINE_SUB_TYPE_UNKNOWN)
		extended_cable_type = (extended_cable_type &
			~(int)ONLINE_TYPE_SUB_MASK) |
			(cable_sub << ONLINE_TYPE_SUB_SHIFT);
	cable_power = GET_POWER_CABLE_TYPE(input_extended_cable_type);
	if (cable_power != ONLINE_POWER_TYPE_UNKNOWN)
		extended_cable_type = (extended_cable_type &
			~(int)ONLINE_TYPE_PWR_MASK) |
			(cable_power << ONLINE_TYPE_PWR_SHIFT);

	switch (cable_main) {
	case POWER_SUPPLY_TYPE_CARDOCK:
		switch (cable_power) {
		case ONLINE_POWER_TYPE_BATTERY:
			cable_type = POWER_SUPPLY_TYPE_BATTERY;
			break;
		case ONLINE_POWER_TYPE_TA:
			switch (cable_sub) {
			case ONLINE_SUB_TYPE_MHL:
				cable_type = POWER_SUPPLY_TYPE_USB;
				break;
			case ONLINE_SUB_TYPE_AUDIO:
			case ONLINE_SUB_TYPE_DESK:
			case ONLINE_SUB_TYPE_SMART_NOTG:
			case ONLINE_SUB_TYPE_KBD:
				cable_type = POWER_SUPPLY_TYPE_MAINS;
				break;
			case ONLINE_SUB_TYPE_SMART_OTG:
				cable_type = POWER_SUPPLY_TYPE_CARDOCK;
				break;
			}
			break;
		case ONLINE_POWER_TYPE_USB:
			cable_type = POWER_SUPPLY_TYPE_USB;
			break;
		default:
			cable_type = current_cable_type;
			break;
		}
		break;
	case POWER_SUPPLY_TYPE_MISC:
		switch (cable_sub) {
		case ONLINE_SUB_TYPE_MHL:
			switch (cable_power) {
			case ONLINE_POWER_TYPE_BATTERY:
				cable_type = POWER_SUPPLY_TYPE_BATTERY;
				break;
			case ONLINE_POWER_TYPE_MHL_500:
				cable_type = POWER_SUPPLY_TYPE_MISC;
				charge_current_max = 400;
				charge_current = 400;
				break;
			case ONLINE_POWER_TYPE_MHL_900:
				cable_type = POWER_SUPPLY_TYPE_MISC;
				charge_current_max = 700;
				charge_current = 700;
				break;
			case ONLINE_POWER_TYPE_MHL_1500:
				cable_type = POWER_SUPPLY_TYPE_MISC;
				charge_current_max = 1300;
				charge_current = 1300;
				break;
			case ONLINE_POWER_TYPE_USB:
				cable_type = POWER_SUPPLY_TYPE_USB;
				charge_current_max = 300;
				charge_current = 300;
				break;
			default:
				cable_type = cable_main;
			}
			break;
		default:
			cable_type = cable_main;
			break;
		}
		break;
	default:
		cable_type = cable_main;
		break;
	}

	if (charge_current_max == 0) {
		charge_current_max =
			charging_current_table[cable_type].input_current_limit;
		charge_current =
			charging_current_table[cable_type].
			fast_charging_current;
	}
	value.intval = charge_current_max;
	psy_do_property(sec_battery_pdata.charger_name, set,
			POWER_SUPPLY_PROP_CURRENT_MAX, value);
	value.intval = charge_current;
	psy_do_property(sec_battery_pdata.charger_name, set,
			POWER_SUPPLY_PROP_CURRENT_AVG, value);
	return cable_type;
}

#if defined(CONFIG_MACH_MELIUS_CHN_CMCC) || defined(CONFIG_MACH_MELIUS_CHN_CTC) ||  defined(CONFIG_MACH_MELIUS_CHN_OPEN) ||  defined(CONFIG_MACH_MELIUS_ZH_LTE) ||defined(CONFIG_MACH_CRATER_CHN_CTC)
extern int msm8930_get_board_rev(void);
extern void sec_battery_detect_switch(int on);
#endif

static bool sec_bat_check_cable_result_callback(
				int cable_type)
{
	struct regulator *l14;
	current_cable_type = cable_type;

	if (current_cable_type == POWER_SUPPLY_TYPE_BATTERY)
	{
#if defined(CONFIG_MACH_MELIUS_CHN_CMCC) || defined(CONFIG_MACH_MELIUS_CHN_CTC) ||  defined(CONFIG_MACH_MELIUS_CHN_OPEN) ||  defined(CONFIG_MACH_MELIUS_ZH_LTE) ||defined(CONFIG_MACH_CRATER_CHN_CTC)
		if ( msm8930_get_board_rev()>=0xA )
			sec_battery_detect_switch(0);
#endif
		pr_info("%s set ldo off\n", __func__);
		l14 = regulator_get(NULL, "8917_l14");
		if(l14 > 0)
		{
			regulator_disable(l14);
		}
	}
	else
	{
		pr_info("%s set ldo on\n", __func__);
		l14 = regulator_get(NULL, "8917_l14");
		if(l14 > 0)
		{
			regulator_enable(l14);
		}
#if defined(CONFIG_MACH_MELIUS_CHN_CMCC) || defined(CONFIG_MACH_MELIUS_CHN_CTC) ||  defined(CONFIG_MACH_MELIUS_CHN_OPEN) ||  defined(CONFIG_MACH_MELIUS_ZH_LTE) ||defined(CONFIG_MACH_CRATER_CHN_CTC)
		if ( msm8930_get_board_rev()>=0xA )
			sec_battery_detect_switch(1);
#endif
	}
	return true;
}

/* callback for battery check
 * return : bool
 * true - battery detected, false battery NOT detected
 */
static bool sec_bat_check_callback(void)
{
	struct power_supply *psy;
	union power_supply_propval value;

	psy = get_power_supply_by_name(("sec-charger"));
	if (!psy) {
		pr_err("%s: Fail to get psy (%s)\n",
			__func__, "sec_charger");
		value.intval = 1;
	} else {
		int ret;
		ret = psy->get_property(psy, POWER_SUPPLY_PROP_PRESENT, &(value));
		if (ret < 0) {
			pr_err("%s: Fail to sec-charger get_property (%d=>%d)\n",
				__func__, POWER_SUPPLY_PROP_PRESENT, ret);
			value.intval = 1;
		}
	}

	return value.intval;
}
static bool sec_bat_check_result_callback(void) {return true; }

/* callback for OVP/UVLO check
 * return : int
 * battery health
 */
static int sec_bat_ovp_uvlo_callback(void)
{
	int health;
	health = POWER_SUPPLY_HEALTH_GOOD;

	return health;
}

static bool sec_bat_ovp_uvlo_result_callback(int health) {return true; }

/*
 * val.intval : temperature
 */
static bool sec_bat_get_temperature_callback(
		enum power_supply_property psp,
		union power_supply_propval *val) {return true; }
static bool sec_fg_fuelalert_process(bool is_fuel_alerted) {return true; }

#if defined(CONFIG_MACH_MELIUS)
#if (defined(CONFIG_MACH_MELIUS_ATT) || defined(CONFIG_MACH_MELIUS_SPR))
static const sec_bat_adc_table_data_t temp_table[] = {
        {27188, 700},
        {27605, 650},
        {28182, 600},
        {28748, 550},
        {29265, 500},
        {29900, 450},
        {30981, 400},
        {31896, 350},
        {32820, 300},
        {33956, 250},
        {35037, 200},
        {36083, 150},
        {37180, 100},
        {38267, 50},
        {39148, 0},
        {39916, -50},
        {40586, -100},
        {41222, -150},
        {41573, -200},
};
#elif (defined(CONFIG_MACH_MELIUS_TMO) || defined(CONFIG_MACH_MELIUS_MTR))
static const sec_bat_adc_table_data_t temp_table[] = {
	{235500,	700},
	{277000,	650},
	{310500, 620},
	{327000,	600},
	{355000, 580},
	{385500,	550},
	{445500,	500},
	{494800, 470},
	{521000,	450},
	{553000, 430},
	{605000,	400},
	{676000,	350},
	{775000,	300},
	{881500,	250},
	{983000,	200},
	{1092000,	150},
	{1196500,	100},
	{1305000,	50},
	{1404500,	0},
	{1448500,	-30},
	{1484000,	-50},
	{1518000,	-70},
	{1554000,	-100},
	{1610500,	-150},
	{1647000,	-200},
	{1665000,	-250},
	{1702500,	-300},
};
#elif defined(CONFIG_MACH_MELIUS_USC)
static const sec_bat_adc_table_data_t temp_table[] = {
	{234516,	700},
	{273272,	650},
	{333525, 620},
	{327000,	600},
	{456336,	550},
	{445500,	500},
	{529050,	450},
	{613994,	400},
	{699781,	350},
	{796731,	300},
	{903291,	250},
	{1007976,	200},
	{1113809,	150},
	{1219643,	100},
	{1310659,	50},
	{1401675,	0},
	{1481018,	-5},
	{1547873,	-10},
	{1609876,	-150},
	{1652458,	-200},
};
#elif defined(CONFIG_MACH_MELIUS_SKT) || defined(CONFIG_MACH_MELIUS_KTT) || \
	defined(CONFIG_MACH_MELIUS_LGT)
static const sec_bat_adc_table_data_t temp_table[] = {
	{253974,	700},
	{293492,	650},
	{342399,	600},
	{399614,	550},
	{462542,	500},
	{536017,	450},
	{618773,	400},
	{708133,	350},
	{805038,	300},
	{902842,	250},
	{1001907,	200},
	{1104199,	150},
	{1205077,	100},
	{1301381,	50},
	{1404180,	0},
	{1481127,	-50},
	{1548483,	-100},
	{1606024,	-150},
	{1653639,	-200},
};
#elif defined(CONFIG_MACH_MELIUS_CHN_CTC)
static const sec_bat_adc_table_data_t temp_table[] = {
	{239402,	700},
	{279324,	650},
	{333753,	600}, 
	{386019,	550},
	{450200,	500},
	{523595,	450},
	{606583,	400},
	{698033,	350},
	{791984,	300},
	{896551,	250},
	{1000621,	200},
	{1106255,	150},
	{1209171,	100},
	{1306680,	50},
	{1398931,	0},
	{1476448,	-50},
	{1545412,	-100},
	{1603156,	-150},
	{1652206,	-200},
	{1690884,	-250},
	{1720716,	-300},
};
#else
static const sec_bat_adc_table_data_t temp_table[] = {
	{240910,	700},
	{278128,	650},
	{322194,	600},
	{379784,	550},
	{439390,	500},
	{520574,	450},
	{599342,	400},
	{679372,	350},
	{772749,	300},
	{881036,	250},
	{982737,	200},
	{1091602,	150},
	{1196111,	100},
	{1291930,	50},
	{1402210,	0},
	{1466921,	-50},
	{1551839,	-100},
	{1609876,	-150},
	{1652458,	-200},
};
#endif
#else
static const sec_bat_adc_table_data_t temp_table[] = {
	{27188,	700},
	{27605,	650},
	{28182,	600},
	{28748,	550},
	{29265,	500},
	{29900,	450},
	{30981,	400},
	{31896,	350},
	{32820,	300},
	{33956,	250},
	{35037,	200},
	{36083,	150},
	{37180,	100},
	{38267,	50},
	{39148,	0},
	{39916,	-50},
	{40586,	-100},
	{41222,	-150},
	{41573,	-200},
};
#endif

/* ADC region should be exclusive */
static sec_bat_adc_region_t cable_adc_value_table[] = {
	{0,	0},
	{0,	0},
	{0,	0},
	{0,	0},
	{0,	0},
	{0,	0},
	{0,	0},
	{0,	0},
	{0,	0},
	{0,	0},
	{0,	0},
};

static int polling_time_table[] = {
	10,	/* BASIC */
	30,	/* CHARGING */
	30,	/* DISCHARGING */
	30,	/* NOT_CHARGING */
	5 * 60,	/* SLEEP */
};

static struct battery_data_t melius_battery_data[] = {
	/* SDI battery data (High voltage 4.35V) */
	{
#if defined(CONFIG_MACH_MELIUS_SKT) || defined(CONFIG_MACH_MELIUS_KTT) || \
	defined(CONFIG_MACH_MELIUS_LGT)
		.RCOMP0 = 0x90,
		.RCOMP_charging = 0x88,
		.temp_cohot = -925,
		.temp_cocold = -8175,
#elif defined(CONFIG_MACH_MELIUS_ATT) || defined(CONFIG_MACH_MELIUS_TMO) || \
	defined(CONFIG_MACH_MELIUS_VZW) || defined(CONFIG_MACH_MELIUS_SPR) || \
	defined(CONFIG_MACH_MELIUS_USC)
		.RCOMP0 = 0x88,
		.RCOMP_charging = 0x88,
		.temp_cohot = -925,
		.temp_cocold = -8175,
#elif defined(CONFIG_MACH_CRATER_CHN_CTC)
		.RCOMP0 = 0x7F,
		.RCOMP_charging = 0x7F,
		.temp_cohot = -1025,
		.temp_cocold = -6050,
#else
		.RCOMP0 = 0x88,
		.RCOMP_charging = 0x88,
		.temp_cohot = -85,
		.temp_cocold = -4200,
#endif
		.is_using_model_data = true,
		.type_str = "SDI",
	}
};

sec_battery_platform_data_t sec_battery_pdata = {
	/* NO NEED TO BE CHANGED */
	.initial_check = sec_bat_initial_check,
	.bat_gpio_init = sec_bat_gpio_init,
	.fg_gpio_init = sec_fg_gpio_init,
	.chg_gpio_init = sec_chg_gpio_init,

	.is_lpm = sec_bat_is_lpm,
	.check_jig_status = sec_bat_check_jig_status,
	.check_cable_callback =
		sec_bat_check_cable_callback,
	.get_cable_from_extended_cable_type =
		sec_bat_get_cable_from_extended_cable_type,
	.cable_switch_check = sec_bat_switch_to_check,
	.cable_switch_normal = sec_bat_switch_to_normal,
	.check_cable_result_callback =
		sec_bat_check_cable_result_callback,
	.check_battery_callback =
		sec_bat_check_callback,
	.check_battery_result_callback =
		sec_bat_check_result_callback,
	.ovp_uvlo_callback = sec_bat_ovp_uvlo_callback,
	.ovp_uvlo_result_callback =
		sec_bat_ovp_uvlo_result_callback,
	.fuelalert_process = sec_fg_fuelalert_process,
	.get_temperature_callback =
		sec_bat_get_temperature_callback,
	.adc_api[SEC_BATTERY_ADC_TYPE_NONE] = {
		.init = sec_bat_adc_none_init,
		.exit = sec_bat_adc_none_exit,
		.read = sec_bat_adc_none_read
		},
	.adc_api[SEC_BATTERY_ADC_TYPE_AP] = {
		.init = sec_bat_adc_ap_init,
		.exit = sec_bat_adc_ap_exit,
		.read = sec_bat_adc_ap_read
		},
	.adc_api[SEC_BATTERY_ADC_TYPE_IC] = {
		.init = sec_bat_adc_ic_init,
		.exit = sec_bat_adc_ic_exit,
		.read = sec_bat_adc_ic_read
		},
	.cable_adc_value = cable_adc_value_table,
	.charging_current = charging_current_table,
	.polling_time = polling_time_table,
	/* NO NEED TO BE CHANGED */

	.pmic_name = SEC_BATTERY_PMIC_NAME,

	.adc_check_count = 6,
	.adc_type = {
		SEC_BATTERY_ADC_TYPE_NONE,	/* CABLE_CHECK */
		SEC_BATTERY_ADC_TYPE_AP,	/* BAT_CHECK */
		SEC_BATTERY_ADC_TYPE_AP,	/* TEMP */
		SEC_BATTERY_ADC_TYPE_AP,	/* TEMP_AMB */
		SEC_BATTERY_ADC_TYPE_AP,	/* FULL_CHECK */
	},

	/* Battery */
	.vendor = "SDI SDI",
	.technology = POWER_SUPPLY_TECHNOLOGY_LION,
	.battery_data = (void *)melius_battery_data,
	.bat_gpio_ta_nconnected = 0,
	.bat_polarity_ta_nconnected = 0,
#if defined(CONFIG_CHARGER_MAX77803)
	.bat_irq = IF_PMIC_IRQ_BASE + MAX77693_CHG_IRQ_BATP_I,
	.bat_irq_attr = IRQF_TRIGGER_FALLING,
#endif
#if defined(CONFIG_MACH_JF_VZW) || defined(CONFIG_MACH_JF_LGT)
	.cable_check_type =
		SEC_BATTERY_CABLE_CHECK_PSY |
		SEC_BATTERY_CABLE_CHECK_NOINCOMPATIBLECHARGE,
#else
	.cable_check_type =
		SEC_BATTERY_CABLE_CHECK_PSY,
#endif
	.cable_source_type =
		SEC_BATTERY_CABLE_SOURCE_EXTERNAL |
		SEC_BATTERY_CABLE_SOURCE_EXTENDED,

	.event_check = true,
	.event_waiting_time = 600,

	/* Monitor setting */
	.polling_type = SEC_BATTERY_MONITOR_ALARM,
	.monitor_initial_count = 3,

	/* Battery check */
	.battery_check_type = SEC_BATTERY_CHECK_INT,
	.check_count = 0,
	/* Battery check by ADC */
	.check_adc_max = 1440,
	.check_adc_min = 0,

	/* OVP/UVLO check */
	.ovp_uvlo_check_type = SEC_BATTERY_OVP_UVLO_CHGPOLLING,

	/* Temperature check */
	.thermal_source = SEC_BATTERY_THERMAL_SOURCE_ADC,
	.temp_adc_table = temp_table,
	.temp_adc_table_size =
		sizeof(temp_table)/sizeof(sec_bat_adc_table_data_t),
	.temp_amb_adc_table = temp_table,
	.temp_amb_adc_table_size =
		sizeof(temp_table)/sizeof(sec_bat_adc_table_data_t),

	.temp_check_type = SEC_BATTERY_TEMP_CHECK_TEMP,
	.temp_check_count = 1,

#if defined(CONFIG_MACH_MELIUS_SKT) || defined(CONFIG_MACH_MELIUS_KTT) || \
	defined(CONFIG_MACH_MELIUS_LGT)
	.temp_high_threshold_event = 700,
	.temp_high_recovery_event = 445,
	.temp_low_threshold_event = -45,
	.temp_low_recovery_event = 0,

	.temp_high_threshold_normal = 700,
	.temp_high_recovery_normal = 445,
	.temp_low_threshold_normal = -45,
	.temp_low_recovery_normal = 0,

	.temp_high_threshold_lpm = 700,
	.temp_high_recovery_lpm = 445,
	.temp_low_threshold_lpm = -45,
	.temp_low_recovery_lpm = 0,
#elif defined(CONFIG_MACH_MELIUS_ATT) || defined(CONFIG_MACH_MELIUS_TMO)
	.temp_high_threshold_event = 620,
	.temp_high_recovery_event = 430,
	.temp_low_threshold_event = -30,
	.temp_low_recovery_event = 0,

	.temp_high_threshold_normal = 480,
	.temp_high_recovery_normal = 430,
	.temp_low_threshold_normal = -30,
	.temp_low_recovery_normal = 0,

	.temp_high_threshold_lpm = 480,
	.temp_high_recovery_lpm = 430,
	.temp_low_threshold_lpm = -30,
	.temp_low_recovery_lpm = 0,
#elif defined(CONFIG_MACH_MELIUS_SPR)
	.temp_high_threshold_event = 613,
	.temp_high_recovery_event = 437,
	.temp_low_threshold_event = -31,
	.temp_low_recovery_event = 18,

	.temp_high_threshold_normal = 547,
	.temp_high_recovery_normal = 437,
	.temp_low_threshold_normal = -31,
	.temp_low_recovery_normal = 18,

	.temp_high_threshold_lpm = 547,
	.temp_high_recovery_lpm = 437,
	.temp_low_threshold_lpm = -31,
	.temp_low_recovery_lpm = 18,
#elif defined(CONFIG_MACH_MELIUS_CHN_CTC)
	.temp_high_threshold_event = 678,
	.temp_high_recovery_event = 462,
	.temp_low_threshold_event = -50,
	.temp_low_recovery_event = 35,

	.temp_high_threshold_normal = 678,
	.temp_high_recovery_normal = 462,
	.temp_low_threshold_normal = -50,
	.temp_low_recovery_normal = 35,

	.temp_high_threshold_lpm = 678,
	.temp_high_recovery_lpm = 462,
	.temp_low_threshold_lpm = -50,
	.temp_low_recovery_lpm = 35,
#else
	.temp_high_threshold_event = 600,
	.temp_high_recovery_event = 400,
	.temp_low_threshold_event = -50,
	.temp_low_recovery_event = 0,

	.temp_high_threshold_normal = 600,
	.temp_high_recovery_normal = 400,
	.temp_low_threshold_normal = -50,
	.temp_low_recovery_normal = 0,

	.temp_high_threshold_lpm = 600,
	.temp_high_recovery_lpm = 400,
	.temp_low_threshold_lpm = -50,
	.temp_low_recovery_lpm = 0,
#endif

	.full_check_type = SEC_BATTERY_FULLCHARGED_CHGPSY,
	.full_check_type_2nd = SEC_BATTERY_FULLCHARGED_TIME,
	.full_check_count = 1,
	.chg_gpio_full_check = 0,
	.chg_polarity_full_check = 1,
	.full_condition_type = SEC_BATTERY_FULL_CONDITION_SOC |
		SEC_BATTERY_FULL_CONDITION_NOTIMEFULL |
		SEC_BATTERY_RECHARGE_CONDITION_VCELL,
	.full_condition_soc = 97,
	.full_condition_vcell = 4300,

	.recharge_check_count = 2,
	.recharge_condition_type =
		SEC_BATTERY_RECHARGE_CONDITION_VCELL,
	.recharge_condition_soc = 98,
	.recharge_condition_vcell = 4300,

	.charging_total_time = 6 * 60 * 60,
	.recharging_total_time = 90 * 60,
	.charging_reset_time = 0,

	/* Fuel Gauge */
	.fg_irq = MSM_GPIO_TO_INT(GPIO_FUEL_INT),
	.fg_irq_attr =
		IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING,
	.fuel_alert_soc = 4,
	.repeated_fuelalert = false,
	.capacity_calculation_type =
		SEC_FUELGAUGE_CAPACITY_TYPE_RAW |
		SEC_FUELGAUGE_CAPACITY_TYPE_SCALE |
		SEC_FUELGAUGE_CAPACITY_TYPE_DYNAMIC_SCALE,
		/* SEC_FUELGAUGE_CAPACITY_TYPE_ATOMIC, */
#if defined(CONFIG_MACH_MELIUS_SKT) || defined(CONFIG_MACH_MELIUS_KTT) || \
	defined(CONFIG_MACH_MELIUS_LGT)
	.capacity_max = 970,
#else
	.capacity_max = 1000,
#endif
	.capacity_max_margin = 50,
	.capacity_min = 0,

	/* Charger */
	.charger_name = "sec-charger",
	.chg_gpio_en = 0,
	.chg_polarity_en = 0,
	.chg_gpio_status = 0,
	.chg_polarity_status = 0,
	.chg_irq = 0,
	.chg_irq_attr = IRQF_TRIGGER_FALLING,
	.chg_float_voltage = 4350,
};

static struct platform_device sec_device_battery = {
	.name = "sec-battery",
	.id = -1,
	.dev.platform_data = &sec_battery_pdata,
};

struct platform_device sec_device_fgchg = {
	.name = "i2c-gpio",
	.id = MSM_FUELGAUGE_I2C_BUS_ID,
	.dev.platform_data = &gpio_i2c_data_fgchg,
};

static struct i2c_board_info sec_brdinfo_fgchg[] __initdata = {
#if !defined(CONFIG_MFD_MAX77693)
	{
		I2C_BOARD_INFO("sec-charger",
			SEC_CHARGER_I2C_SLAVEADDR),
		.platform_data	= &sec_battery_pdata,
	},
#endif
	{
		I2C_BOARD_INFO("sec-fuelgauge",
			SEC_FUELGAUGE_I2C_SLAVEADDR),
		.platform_data	= &sec_battery_pdata,
	},
};

static struct platform_device *msm8960_battery_devices[] __initdata = {
	&sec_device_fgchg,
	&sec_device_battery,
};

static int __init sec_bat_current_boot_mode(char *mode)
{
	/*
	*	1 is recovery booting
	*	0 is normal booting
	*/

	if (strncmp(mode, "1", 1) == 0)
		sec_bat_recovery_mode = 1;
	else
		sec_bat_recovery_mode = 0;

	pr_info("%s : %s", __func__, sec_bat_recovery_mode == 1 ?
				"recovery" : "normal");

	return 1;
}
__setup("androidboot.boot_recovery=", sec_bat_current_boot_mode);

void __init msm8960_init_battery(void)
{


	gpio_tlmm_config(GPIO_CFG(GPIO_FUELGAUGE_I2C_SCL, 0, GPIO_CFG_OUTPUT,
		 GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);
	gpio_tlmm_config(GPIO_CFG(GPIO_FUELGAUGE_I2C_SDA,  0, GPIO_CFG_OUTPUT,
		 GPIO_CFG_NO_PULL, GPIO_CFG_2MA), 1);
	gpio_set_value(GPIO_FUELGAUGE_I2C_SCL, 1);
	gpio_set_value(GPIO_FUELGAUGE_I2C_SDA, 1);

	gpio_i2c_data_fgchg.sda_pin = GPIO_FUELGAUGE_I2C_SDA;
	gpio_i2c_data_fgchg.scl_pin = GPIO_FUELGAUGE_I2C_SCL;

	platform_add_devices(
		msm8960_battery_devices,
		ARRAY_SIZE(msm8960_battery_devices));

	i2c_register_board_info(
		MSM_FUELGAUGE_I2C_BUS_ID,
		sec_brdinfo_fgchg,
		ARRAY_SIZE(sec_brdinfo_fgchg));
}

#endif

