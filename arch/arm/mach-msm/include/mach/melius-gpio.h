/*
 * melius-gpio.h
 *
 * header file supporting gpio functions for Samsung device
 *
 * COPYRIGHT(C) Samsung Electronics Co., Ltd. 2006-2011 All Right Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <linux/mfd/pm8xxx/pm8038.h>
#include <linux/mfd/pm8xxx/pm8921.h>

#define PM8038_GPIO_BASE		NR_GPIO_IRQS
#define PM8038_GPIO_PM_TO_SYS(pm_gpio)	(pm_gpio - 1 + PM8038_GPIO_BASE)
#define PM8917_GPIO_PM_TO_SYS(pm_gpio)	PM8038_GPIO_PM_TO_SYS(pm_gpio)
 

/* MSM8930 GPIO */
#define GPIO_IRDA_EN			0
#define GPIO_MHL_RST			1
#define GPIO_LCD_EN			2
#define GPIO_FLASH_NOW_SW			3
#define GPIO_VT_CAM_MCLK			4
#define GPIO_CAM_MCLK			5
#if defined(CONFIG_MACH_MELIUS_CHN_CTC) || defined(CONFIG_MACH_CRATER_CHN_CTC)
#define GPIO_CAM_SENSOR_CORE_EN			47
#define GPIO_BATT_INT			78
#else
#define GPIO_CAM_SENSOR_CORE_EN			6
#define GPIO_BATT_INT			7
#endif
#define GPIO_MHL_SDA_1_8V			8
#define GPIO_MHL_SCL_1_8V			9
#define GPIO_LCD_BOOSTER_EN_EMUL			10
#define GPIO_MSM8930_10			10
#define GPIO_MSM8930_11			11
#if defined(CONFIG_MACH_MELIUS_CHN_CTC)
#define GPIO_2MIC_I2C_SDA		12
#define GPIO_2MIC_I2C_SCL		13
#else
#define GPIO_MSM8930_12			12
#define GPIO_MSM8930_13			13
#endif
#define GPIO_MUSB_INT			14
#if defined(CONFIG_GSM_MODEM_SPRD6500)
#define GPIO_IPC_SRDY			15
#else
#define GPIO_CHG_DET_N			15
#endif
#define GPIO_TSP_SDA_1_8V			16
#define GPIO_TSP_SCL_1_8V			17
#define GPIO_VT_CAM_ID			18
#if defined(CONFIG_GSM_MODEM_SPRD6500)
#define GPIO_PHONE_ACTIVE			19
#endif
#if defined(CONFIG_MACH_MELIUS_CHN_CTC)
#define GPIO_LCD_ESD_DET			102
#else
#define GPIO_LCD_ESD_DET			19
#endif
#define GPIO_CAM_SDA_1_8V			20
#define GPIO_CAM_SCL_1_8V			21
#if defined(CONFIG_MACH_CRATER_CHN_CTC)
#define GPIO_UART_TXD_MSM			10
#define GPIO_UART_RXD_MSM			11
#else
#define GPIO_UART_TXD_MSM			22
#define GPIO_UART_RXD_MSM			23
#endif
#if defined(CONFIG_MACH_MELIUS_CHN_CTC) || defined(CONFIG_MACH_CRATER_CHN_CTC)
#if defined(CONFIG_GSM_MODEM_SPRD6500)
#define GPIO_GSM_SPI_MOSI			22
#define GPIO_GSM_SPI_MISO			23
#define GPIO_GSM_SPI_CS_N			24
#define GPIO_GSM_SPI_CLK			25
#endif
#else
#define GPIO_2MIC_I2C_SDA			24
#define GPIO_2MIC_I2C_SCL			25
#endif
#define GPIO_MSM8930_26			26
#define GPIO_MSM8930_27			27
#define GPIO_BT_CTL			28
#define GPIO_BT_DATA			29
#define GPIO_SIM_IO_1			30
#define GPIO_SIM_CLK_1			31
#define GPIO_SIM_RST_1			32
#define GPIO_HALL_SENSOR_INT			33
#define GPIO_MUS_SDA			34
#define GPIO_MUS_SCL			35
#define GPIO_CODEC_SDA			36
#define GPIO_CODEC_SCL			37
#define GPIO_MSM8930_38			38
#define GPIO_MSM8930_39			39
#define GPIO_MSM8930_40			40
#define GPIO_MSM8930_41			41
#if defined(CONFIG_MACH_MELIUS_CHN_CTC) || defined(CONFIG_MACH_CRATER_CHN_CTC)
#define GPIO_IMA_I2C_SDA			6
#define GPIO_IMA_I2C_SCL			7
#define GPIO_S_LED_I2C_SDA		6   // Use same line(IrDA)
#define GPIO_S_LED_I2C_SCL			7   // Use same line(IrDA)
#else
#if defined(CONFIG_MACH_MELIUS_SKT) || defined(CONFIG_MACH_MELIUS_KTT) || \
	defined(CONFIG_MACH_MELIUS_LGT)
#define GPIO_S_LED_I2C_SDA			27   // Use same line(IrDA)
#define GPIO_S_LED_I2C_SCL			26   // Use same line(IrDA)
#else
#define GPIO_S_LED_I2C_SDA			42   // Use same line(IrDA)
#define GPIO_S_LED_I2C_SCL			43   // Use same line(IrDA)
#endif
#define GPIO_IMA_I2C_SDA			42
#define GPIO_IMA_I2C_SCL			43
#endif

#if defined(CONFIG_GSM_MODEM_SPRD6500)
#define GPIO_SDL_TXD	42
#define GPIO_SDL_RXD	43
#define GPIO_SDL_CTS	44
#define GPIO_SDL_RTS	45
#endif


#if defined(CONFIG_MACH_MELIUS_CHN_CTC) || defined(CONFIG_MACH_CRATER_CHN_CTC)
#define GPIO_SENSOR_SDA_1_8V			8
#define GPIO_SENSOR_SCL_1_8V			9
#else
#define GPIO_SENSOR_SDA_1_8V			44
#define GPIO_SENSOR_SCL_1_8V			45
#endif
#define GPIO_EAR_DET			46
#define GPIO_MSM8930_47			47
#define GPIO_MSM8930_48			48
#define GPIO_RGB_SENSOR_INT			49
#define GPIO_VOL_UP			50
#define GPIO_LCD_BOOSTER_EN			51
#define GPIO_MSM8930_52			52
#if defined(CONFIG_WCD9304_USE_MI2S_CLK_9600)
#define GPIO_AUDIO_MCLK			53
#elif defined (CONFIG_MACH_MELIUS_EUR_OPEN) || defined(CONFIG_MACH_MELIUS_EUR_LTE) \
	|| defined(CONFIG_MACH_MELIUS_SKT) || defined(CONFIG_MACH_MELIUS_KTT) \
	|| defined(CONFIG_MACH_MELIUS_LGT) \
	|| defined(CONFIG_MACH_MELIUS_ATT) || defined(CONFIG_MACH_MELIUS_TMO) \
	|| defined(CONFIG_MACH_MELIUS_VZW) || defined(CONFIG_MACH_MELIUS_SPR) \
	|| (defined(CONFIG_MACH_MELIUS_USC) && !defined(CONFIG_MACH_MELIUS_USC_00))
#define GPIO_AUDIO_MCLK			66
#else
#define GPIO_AUDIO_MCLK			53
#endif
#define GPIO_CRESET_B			54
#define GPIO_TX_I2S_SCK			55
#define GPIO_TX_I2S_WS			56
#define GPIO_TX_I2S_DI_CP			57
#define GPIO_MLCD_RST			58
#if defined(CONFIG_GSM_MODEM_SPRD6500)
#define GPIO_IPC_MRDY			59
#else
#define GPIO_UART_BOOT_ON			59
#endif
#define GPIO_RX_I2S_SCK			60
#define GPIO_RX_I2S_DO_CP			61
#define GPIO_RX_I2S_WS			62
#define GPIO_VIB_ON			63
#if defined(CONFIG_MACH_MELIUS_CHN_CTC)
#define GPIO_FLASH_CNTL_EN_SW		77 // system_rev < 0x07
#define GPIO_CAM_SENSOR_EN			1 // system_rev >= 0x07
#else
#define GPIO_FLASH_CNTL_EN_SW		64 // system_rev < 0x07
#define GPIO_CAM_SENSOR_EN			64 // system_rev >= 0x07
#endif
#if defined(CONFIG_MACH_CRATER_CHN_CTC) || defined(CONFIG_MACH_MELIUS_CHN_CTC)
#define GPIO_T_FLASH_DETECT			92
#else
#define GPIO_T_FLASH_DETECT			65
#endif
#define GPIO_TSP_INT			66
#define GPIO_GYRO_INT_N			67
#define GPIO_FPGA_RST_N			68
#define GPIO_CODEC_INT			69
#define GPIO_VIB_PWM			70
#define GPIO_AF_SDA_1_8V			71
#define GPIO_AF_SCL_1_8V			72

/* BATTERY */
#if !defined(CONFIG_MACH_MELIUS_CHN_CTC) && !defined(CONFIG_MACH_CRATER_CHN_CTC)
#define GPIO_BATT_INT                   7
#endif
#define GPIO_FUELGAUGE_I2C_SDA  73
#define GPIO_FUELGAUGE_I2C_SCL  74
//#define GPIO_CHARGER_CHG_EN             33
#define GPIO_CHARGER_CHG_EN             82
#define GPIO_FUEL_INT               98//69
#define GPIO_CHARGER_CHG_nSTAT  150

#ifdef CONFIG_MFD_MAX77693
#define GPIO_IF_PMIC_SDA                34
#define GPIO_IF_PMIC_SCL                35
#define GPIO_IF_PMIC_IRQ                14
#endif

#define GPIO_HW_GPIO75			75
#define GPIO_CAM_VT_nRST			76
#define GPIO_MHL_WAKE_UP			77
#define GPIO_MHL_INT			78
#define GPIO_2MIC_PW_DN			79
#define GPIO_NFC_EN			80
#define GPIO_VOL_DOWN			81
#define GPIO_TA_nCHG			82
#define GPIO_BT_SSBI			83
#define GPIO_WL_CMD_DATA2			84
#define GPIO_WL_CMD_DATA1			85
#define GPIO_WL_CMD_DATA0			86
#define GPIO_WL_CMD_SET			87
#define GPIO_WL_CMD_CLK			88
#define GPIO_IRDA_IRQ			89
#define GPIO_HOME_KEY			90
#define GPIO_HW_GPIO91			91
#if !defined(CONFIG_MACH_MELIUS_CHN_CTC) && !defined(CONFIG_MACH_CRATER_CHN_CTC)
#define GPIO_NFC_FIRMWARE			92
#endif
#define GPIO_HW_GPIO93			93
#define GPIO_EAR_SEND_END			94
#define GPIO_NFC_SDA_1_8V			95
#define GPIO_NFC_SCL_1_8V			96
#define GPIO_HW_GPIO97			97
#define GPIO_BATT_ALARM			98
#if defined(CONFIG_MACH_MELIUS_CHN_CTC) || defined(CONFIG_MACH_CRATER_CHN_CTC)
#define GPIO_GG_SEL			99
#else
#define GPIO_NC_99			99
#endif
#define GPIO_MHL_DSCL_1_8V			100
#define GPIO_MHL_DSDA_1_8V			101
#define GPIO_HDMI_HPD			102
#define GPIO_PM_APC_SEC_IRQ_N			103
#define GPIO_PM_APC_USR_IRQ_N			104
#define GPIO_PM_MDM_IRQ_N			105
#define GPIO_NFC_IRQ			106
#define GPIO_8M_RESET			107
#define GPIO_PS_HOLD			108
#if defined(CONFIG_GSM_MODEM_SPRD6500)
#define GPIO_GSM_PHONE_ON			109
#else
#define GPIO_APT_EN			109
#endif
#define GPIO_MDM_SUB_SEL1			110
#define GPIO_NC_111			111
#define GPIO_1_8V_PU			112
#if defined(CONFIG_GSM_MODEM_SPRD6500)
#define GPIO_PDA_ACTIVE			113
#else
#define GPIO_DRX_SW_SEL0			113
#endif
#if defined(CONFIG_GSM_MODEM_SPRD6500)
#define GPIO_AP_CP_INT1			114
#define GPIO_AP_CP_INT2			115
#else
#define GPIO_MDM_SUB_SEL2			114
#define GPIO_DRX_SW_SEL1			115
#endif
#if defined(CONFIG_GSM_MODEM_SPRD6500)
#define GPIO_CP_DUMP_INT			116
#else
#define GPIO_ANT_SW_SEL3			116
#endif
#define GPIO_ANT_SW_SEL2			117
#define GPIO_ANT_SW_SEL1			118
#if defined(CONFIG_MACH_MELIUS_CHN_CTC) || defined(CONFIG_MACH_CRATER_CHN_CTC)
#define GPIO_NFC_FIRMWARE			119
#else
#define GPIO_ANT_SW_SEL0			119
#endif
#define GPIO_NC_120			120
#define GPIO_NC_121			121
#define GPIO_PA0_R0			122
#define GPIO_PA0_R1			123
#define GPIO_NC_124			124
#define GPIO_WTR_RF_ON			125
#define GPIO_WTR_RX_ON			126
#define GPIO_APT_VCON			127
#define GPIO_RF_GRIP_INT1			128
#define GPIO_PA_ON7_B7			129
#define GPIO_HCMMPA_BAND_SEL0			130
#define GPIO_PA_ON5_LB20			131
#define GPIO_HCMMPA_BAND_SEL1			132
#define GPIO_MODE_SEL_GSM_BAR			133
#define GPIO_PA_ON2_B1B2B3B4_EN			134
#define GPIO_NC_135			135
#define GPIO_PA_ON0_B5B8_EN			136
#define GPIO_GPS_LNA_EN			137
#define GPIO_ADJ_DET			138
#define GPIO_DRX_SW_SEL2			139
#define GPIO_DRX_SW_SEL3			140
#define GPIO_NC_141			141
#define GPIO_WTR_SSBI2			142
#define GPIO_WTR_SSBI1			143
#define GPIO_NC_144			144
#define GPIO_NC_145			145
#define GPIO_WTR_GPDATA2			146
#define GPIO_WTR_GPDATA1			147
#define GPIO_WTR_GPDATA0			148
#define GPIO_RF_GRIP_EN			149
#define GPIO_OTG_TEST			150
#define GPIO_EAR_MICBIAS_EN			151


/* PM8917 GPIO */
#define GPIO_PM_NC1			PM8917_GPIO_PM_TO_SYS(1)
#define GPIO_PM_NC2			PM8917_GPIO_PM_TO_SYS(2)
#define GPIO_PM_NC3			PM8917_GPIO_PM_TO_SYS(3)
#define GPIO_PM_NC4			PM8917_GPIO_PM_TO_SYS(4)
#define GPIO_PM_TP501		PM8917_GPIO_PM_TO_SYS(5)
#define GPIO_PM_NC6			PM8917_GPIO_PM_TO_SYS(6)
#define GPIO_PM_UART_TXD_MSM		PM8917_GPIO_PM_TO_SYS(7)
#define GPIO_PM_UART_TXD		PM8917_GPIO_PM_TO_SYS(8)
#define GPIO_PM_UART_RXD		PM8917_GPIO_PM_TO_SYS(9)
#define GPIO_PM_UART_RXD_MSM		PM8917_GPIO_PM_TO_SYS(10)
#define PMIC_GPIO_2MIC_RST		PM8917_GPIO_PM_TO_SYS(11)
#define GPIO_PM_TP502		PM8917_GPIO_PM_TO_SYS(12)
#define GPIO_PM_IMA_PWR_EN		PM8917_GPIO_PM_TO_SYS(13)
#define GPIO_PM_SPK_EN		PM8917_GPIO_PM_TO_SYS(14)
#define GPIO_PM_IMA_CMC_EN		PM8917_GPIO_PM_TO_SYS(15)
#define GPIO_PM_IMA_nRST		PM8917_GPIO_PM_TO_SYS(16)
#define GPIO_PM_IMA_SLEEP		PM8917_GPIO_PM_TO_SYS(17)
#define GPIO_PM_IMA_INT		PM8917_GPIO_PM_TO_SYS(18)
#define GPIO_PM_CLK_MSM_FWD		PM8917_GPIO_PM_TO_SYS(19)
#define GPIO_PM_NC20			PM8917_GPIO_PM_TO_SYS(20)
#define GPIO_PM_NC21			PM8917_GPIO_PM_TO_SYS(21)
#define GPIO_PM_NC22			PM8917_GPIO_PM_TO_SYS(22)
#define GPIO_PM_NC23			PM8917_GPIO_PM_TO_SYS(23)
#define GPIO_PM_NC24			PM8917_GPIO_PM_TO_SYS(24)
#define GPIO_PM_NC25			PM8917_GPIO_PM_TO_SYS(25)
#define GPIO_PM_NC26			PM8917_GPIO_PM_TO_SYS(26)
#define GPIO_PM_NC27			PM8917_GPIO_PM_TO_SYS(27)
#define GPIO_PM_NC28			PM8917_GPIO_PM_TO_SYS(28)
#define GPIO_PM_WCD9304_RESET		PM8917_GPIO_PM_TO_SYS(29)
#define GPIO_PM_NC30			PM8917_GPIO_PM_TO_SYS(30)
#define GPIO_PM_NC31			PM8917_GPIO_PM_TO_SYS(31)
#define GPIO_PM_NC32			PM8917_GPIO_PM_TO_SYS(32)
#define GPIO_PM_NC33			PM8917_GPIO_PM_TO_SYS(33)
#define GPIO_PM_NC34			PM8917_GPIO_PM_TO_SYS(34)
#define GPIO_PM_NC35			PM8917_GPIO_PM_TO_SYS(35)
#define GPIO_PM_NC36			PM8917_GPIO_PM_TO_SYS(36)
#define GPIO_PM_MLCD_RST	PM8917_GPIO_PM_TO_SYS(37)
#define GPIO_PM_NC38			PM8917_GPIO_PM_TO_SYS(38)
/*----------------------------Don't change above here.------------------------------*/





/* Camera */
#define GPIO_FLASH_LED_UNLOCK		GPIO_FLASH_CNTL_EN_SW
#define	GPIO_MSM_FLASH_NOW			GPIO_FLASH_NOW_SW
#define	GPIO_MAIN_CAM_MCLK			GPIO_CAM_MCLK
#define	GPIO_SUB_CAM_MCLK			GPIO_VT_CAM_MCLK
#define	GPIO_CAM1_RST_N				GPIO_8M_RESET
#define	GPIO_CAM2_RST_N				GPIO_CAM_VT_nRST
#define GPIO_I2C_DATA_CAM			GPIO_CAM_SDA_1_8V
#define GPIO_I2C_CLK_CAM			GPIO_CAM_SCL_1_8V
#define	GPIO_CAM_IO_EN				GPIO_CAM_SENSOR_CORE_EN
#define GPIO_I2C_DATA_AF			GPIO_AF_SDA_1_8V
#define GPIO_I2C_CLK_AF				GPIO_AF_SCL_1_8V

/* MHL */
#define GPIO_MHL_SDA			GPIO_MHL_SDA_1_8V
#define GPIO_MHL_SCL			GPIO_MHL_SCL_1_8V


/* Audio */	
#define GPIO_VPS_AMP_EN				GPIO_NC_145		// need to change
#define GPIO_SPK_AMP_EN				GPIO_PM_SPK_EN
#define GPIO_AUDIO_MCLK_REV10		53		// need to change
#define GPIO_SPKR_I2S_TX_SCK		GPIO_TX_I2S_SCK
#define GPIO_SPKR_I2S_TX_WS			GPIO_TX_I2S_WS
#define GPIO_SPKR_I2S_TX_DIN		GPIO_TX_I2S_DI_CP
#define GPIO_SPKR_I2S_RX_SCK		GPIO_RX_I2S_SCK
#define GPIO_SPKR_I2S_RX_DOUT		GPIO_RX_I2S_DO_CP
#define GPIO_SPKR_I2S_RX_WS			GPIO_RX_I2S_WS


#define GPIO_VOLUME_UP				GPIO_VOL_UP
#define GPIO_VOLUME_DOWN			GPIO_VOL_DOWN
#define GPIO_HOME_KEY_REV01			GPIO_HOME_KEY
#define GPIO_SD_CARD_DET_N			GPIO_T_FLASH_DETECT

#define GPIO_EAR_MIC_BIAS_EN		GPIO_EAR_MICBIAS_EN

#ifdef CONFIG_SAMSUNG_CMC624
#define IMA_PWR_EN					GPIO_PM_IMA_PWR_EN
#define IMA_CMC_EN					GPIO_PM_IMA_CMC_EN
#define IMA_nRST					GPIO_PM_IMA_nRST
#define IMA_SLEEP					GPIO_PM_IMA_SLEEP
#define CMC_ESD						GPIO_PM_IMA_INT
#endif

#if defined(CONFIG_MACH_MELIUS_SKT) || defined(CONFIG_MACH_MELIUS_KTT) || \
	defined(CONFIG_MACH_MELIUS_LGT)
#define GPIO_TDMB_SPI_MOSI_REV00	GPIO_MSM8930_10
#define GPIO_TDMB_SPI_MISO_REV00	GPIO_MSM8930_11
#define GPIO_TDMB_SPI_CS_REV00		GPIO_MSM8930_12
#define GPIO_TDMB_SPI_CLK_REV00		GPIO_MSM8930_13
#define GPIO_CAM_SPI_MOSI		GPIO_MSM8930_10
#define GPIO_CAM_SPI_MISO		GPIO_MSM8930_11
#define GPIO_CAM_SPI_SSN		GPIO_MSM8930_12
#define GPIO_CAM_SPI_SCLK		GPIO_MSM8930_13
#define GPIO_IRDA_SCL			GPIO_MSM8930_26
#define GPIO_IRDA_SDA			GPIO_MSM8930_27
#define GPIO_TDMB_SPI_MOSI		GPIO_MSM8930_38
#define GPIO_TDMB_SPI_MISO		GPIO_MSM8930_39
#define GPIO_TDMB_SPI_CS		GPIO_MSM8930_40
#define GPIO_TDMB_SPI_CLK		GPIO_MSM8930_41
#define GPIO_CAM_SPI_MOSI_REV00		GPIO_MSM8930_38
#define GPIO_CAM_SPI_MISO_REV00		GPIO_MSM8930_39
#define GPIO_CAM_SPI_SSN_REV00		GPIO_MSM8930_40
#define GPIO_CAM_SPI_SCLK_REV00		GPIO_MSM8930_41
#define GPIO_TDMB_EN			GPIO_MSM8930_47
#define GPIO_TDMB_RST			GPIO_MSM8930_48
#define GPIO_TDMB_INT			GPIO_MSM8930_52
#define GPIO_PM_TDMB_ANT_DET		GPIO_PM_NC28
#else /* EUR_OPEN, EUR_LTE, ATT, TMO, USC...*/
#define GPIO_FM_RST			GPIO_MSM8930_10
#define GPIO_FM_INT			GPIO_MSM8930_11
#if defined(CONFIG_MACH_MELIUS_CHN_CTC) || defined(CONFIG_MACH_CRATER_CHN_CTC)
#define GPIO_IRDA_SDA			135
#define GPIO_IRDA_SCL			136
#else
#define GPIO_IRDA_SDA			GPIO_MSM8930_12
#define GPIO_IRDA_SCL			GPIO_MSM8930_13
#endif
#if defined(CONFIG_GSM_MODEM_SPRD6500)
#define GPIO_IPC_SUB_MRDY			GPIO_MSM8930_26
#define GPIO_IPC_SUB_SRDY			GPIO_MSM8930_27
#else
#define GPIO_FM_SSBI			GPIO_MSM8930_26
#define GPIO_FM_DATA			GPIO_MSM8930_27
#endif
#define GPIO_CAM_SPI_MOSI		GPIO_MSM8930_38
#define GPIO_CAM_SPI_MISO		GPIO_MSM8930_39
#define GPIO_CAM_SPI_SSN		GPIO_MSM8930_40
#define GPIO_CAM_SPI_SCLK		GPIO_MSM8930_41
#define GPIO_FM_DFS			GPIO_MSM8930_47
#if defined(CONFIG_GSM_MODEM_SPRD6500)
#define GPIO_UART_SEL			GPIO_MSM8930_48
#define GPIO_SIM_SEL			GPIO_MSM8930_52
#else
#define GPIO_FM_DCLK			GPIO_MSM8930_48
#define GPIO_FM_DOUT			GPIO_MSM8930_52
#endif
#endif

/*I2C BUS ID*/
#define I2C_LEDS_BUS_ID 		21

#if defined(CONFIG_WCD9304_CLK_9600)
#if defined(CONFIG_WCD9304_USE_MI2S_CLK_9600)
#define CLK_REVISION 0
#elif defined (CONFIG_MACH_MELIUS_EUR_OPEN) || defined(CONFIG_MACH_MELIUS_EUR_LTE) \
	|| defined(CONFIG_MACH_MELIUS_SKT) || defined(CONFIG_MACH_MELIUS_KTT) \
	|| defined(CONFIG_MACH_MELIUS_LGT) \
	|| defined(CONFIG_MACH_MELIUS_ATT) || defined(CONFIG_MACH_MELIUS_TMO)
#define CLK_REVISION 7
#elif defined(CONFIG_MACH_MELIUS_VZW) || defined(CONFIG_MACH_MELIUS_SPR) \
	|| (defined(CONFIG_MACH_MELIUS_USC) && !defined(CONFIG_MACH_MELIUS_USC_00))
#define CLK_REVISION 1
#else
#define CLK_REVISION 10
#endif
#endif

/* gpio for changed list */
enum {
	BOARD_REV00,
	BOARD_REV01,
	BOARD_REV02,
	BOARD_REV03,
	BOARD_REV04,
	BOARD_REV05,
	BOARD_REV06,
	BOARD_REV07,
	BOARD_REV08,
	BOARD_REV09,
	BOARD_REV10,
	BOARD_REV11,
	BOARD_REV12,
	BOARD_REV13,
	BOARD_REV14,
	BOARD_REV15,
	GPIO_REV_MAX,
};
