/* Copyright (c) 2012, Code Aurora Forum. All rights reserved.
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

#include <linux/module.h>
#include <msm.h>
#include "msm_camera_eeprom.h"
#include <media/v4l2-subdev.h>

#define SPI_PROTOCOL_OFFSET 4
#define SPI_PROTOCOL_READ_CODE 0x03
#define SPI_PROTOCOL_WRITE_ENABLE_CODE 0x06
#define SPI_PROTOCOL_SECTOR_ERASE_CODE 0x20
#define SPI_PROTOCOL_READ_STATUS0_CODE 0x05
#define SPI_PROTOCOL_PAGE_PROGRAM_CODE 0x02

#if defined(CONFIG_MACH_KS02)
#define TEST_TEMP	1
#define TEST_NTEMP	0
#endif
/*Start : shchang@qualcomm.com : 1122*/
#define ftov_BEn(A) ((((uint32_t)(A) & 0xff000000) >> 24) | \
	(((uint32_t)(A) & 0x00ff0000) >> 8) | \
	(((uint32_t)(A) & 0x0000ff00) << 8) | \
	(((uint32_t)(A) & 0x000000ff) << 24))

#define ftov_LEn(A) (uint32_t)(A)
/*End : shchang@qualcomm.com : 1122*/

#define sizeof_member(type, member) sizeof(((type *)0)->member)
#define QC_TEST 1

DEFINE_MUTEX(imx175_eeprom_mutex);
static struct msm_eeprom_ctrl_t imx175_eeprom_t;

int32_t imx175_spi_txfr(struct spi_device *spi,
	char *tx_buf, char *rx_buf, uint32_t num_byte)
{
	struct spi_transfer t;
	struct spi_message m;
	int ret;

	spi->bits_per_word = 8;
	spi->mode = SPI_MODE_0;
	memset(&t, 0, sizeof(t));

	t.tx_buf = tx_buf;
	t.rx_buf = rx_buf;
	t.len = num_byte;
	t.speed_hz = 10800000;
	/*supported rates can be obtained from clock-8960.c*/
	spi_setup(spi);
	spi_message_init(&m);
	spi_message_add_tail(&t, &m);
	ret = spi_sync(spi, &m);
	if (ret)
		pr_err("%s: Error spi transfer: %d\n", __func__, ret);
	return ret;
}

int32_t imx175_spi_write_enable(struct msm_eeprom_ctrl_t *eeprom_ctrl)
{
	struct spi_device *spi = eeprom_ctrl->spi;
	char tx_buf;
	char rx_buf;

	tx_buf = SPI_PROTOCOL_WRITE_ENABLE_CODE;

	return imx175_spi_txfr(spi, &tx_buf, &rx_buf, 1);
}

int32_t imx175_spi_read_status0(struct msm_eeprom_ctrl_t *eeprom_ctrl,
	uint8_t *read_status)
{
	char tx_buf[2] = {0};
	char rx_buf[2] = {0};
	int ret;

	tx_buf[0] = SPI_PROTOCOL_READ_STATUS0_CODE;
	ret = imx175_spi_txfr(eeprom_ctrl->spi, &tx_buf[0], &rx_buf[0], 2);
	*read_status = rx_buf[1];
	return ret;
}
/* patch#183359 : s*/
int32_t imx175_spi_read_id(struct msm_eeprom_ctrl_t *eeprom_ctrl)
{
	char tx_buf[6] = {0x90};
	char rx_buf[6] = {0};
	int ret;

	ret = imx175_spi_txfr(eeprom_ctrl->spi, &tx_buf[0], &rx_buf[0], 6);
	pr_err("%s: ReadID:0x%x, 0x%x\n", __func__, rx_buf[4], rx_buf[5]);
	return ret;
}
/* patch#183359 : e*/

int32_t imx175_spi_sector_erase(struct msm_eeprom_ctrl_t *eeprom_ctrl,
	uint32_t sector_addr)
{
	char tx_buf[4] = {0};
	char rx_buf[4] = {0};
	char read_status = 0xFF;
	int ret, i = 0;

	ret = imx175_spi_write_enable(eeprom_ctrl);
	if (ret) {
		pr_err("%s: Error write enable: %d\n", __func__, ret);
		return ret;
	}

	tx_buf[0] = SPI_PROTOCOL_SECTOR_ERASE_CODE;
	tx_buf[1] = (char)((sector_addr >> 16) & 0xFF);
	tx_buf[2] = (char)((sector_addr >> 8) & 0xFF);
	tx_buf[3] = (char)(sector_addr & 0xFF);
	ret = imx175_spi_txfr(eeprom_ctrl->spi, &tx_buf[0], &rx_buf[0], 4);
	if (ret) {
		pr_err("%s: Error sector erase: %d\n", __func__, ret);
		return ret;
	}

	do {
		if (i > 10) {
			pr_err("%s: Sector erase timeout\n", __func__);
			return -ETIMEDOUT;
		}
		ret = imx175_spi_read_status0(eeprom_ctrl, &read_status);
		if (ret) {
			pr_err("%s: Error read status0: %d\n", __func__, ret);
			return ret;
		}
		i++;
		msleep(40);
	} while (read_status != 0);
	return ret;
}

int32_t imx175_spi_page_program(struct msm_eeprom_ctrl_t *eeprom_ctrl,
	uint32_t reg_addr, char *tx_buf, char *rx_buf, uint32_t num_byte)
{
	char read_status = 0xFF;
	int ret, i = 0;

	ret = imx175_spi_write_enable(eeprom_ctrl);
	if (ret) {
		pr_err("%s: Error write enable: %d\n", __func__, ret);
		return ret;
	}

	tx_buf[0] = SPI_PROTOCOL_PAGE_PROGRAM_CODE;
	tx_buf[1] = (char)((reg_addr >> 16) & 0xFF);
	tx_buf[2] = (char)((reg_addr >> 8) & 0xFF);
	tx_buf[3] = (char)(reg_addr & 0xFF);
	ret = imx175_spi_txfr(eeprom_ctrl->spi, &tx_buf[0], &rx_buf[0],
		num_byte + SPI_PROTOCOL_OFFSET);
	if (ret) {
		pr_err("%s: Error page program: %d\n", __func__, ret);
		return ret;
	}

	do {
		if (i > 10) {
			pr_err("%s: Page program timeout\n", __func__);
			return -ETIMEDOUT;
		}
		ret = imx175_spi_read_status0(eeprom_ctrl, &read_status);
		if (ret) {
			pr_err("%s: Error read status0: %d\n", __func__, ret);
			return ret;
		}
		i++;
		msleep(40);
	} while (read_status != 0);
	return ret;
}

int32_t imx175_spi_read(struct msm_eeprom_ctrl_t *eeprom_ctrl,
	uint32_t reg_addr, void *inbuf, uint32_t num_byte)
{
	char *tx_buf;
	int ret;
	char *data = (char *)inbuf;

	pr_err("%s : Enter - reg_addr : 0x%x\n", __func__, reg_addr);

	tx_buf = kmalloc(num_byte + SPI_PROTOCOL_OFFSET, GFP_KERNEL);
	if (NULL == tx_buf) {
		pr_err("%s: Error allocating tx_buf.\n", __func__);
		return -ENOMEM;
	}

	tx_buf[0] = SPI_PROTOCOL_READ_CODE;
	tx_buf[1] = (char)((reg_addr >> 16) & 0xFF);
	tx_buf[2] = (char)((reg_addr >> 8) & 0xFF);
	tx_buf[3] = (char)(reg_addr & 0xFF);
	ret = imx175_spi_txfr(eeprom_ctrl->spi, &tx_buf[0], data,
		num_byte + SPI_PROTOCOL_OFFSET);
	kfree(tx_buf);
	return ret;
}

int32_t imx175_direct_read(struct msm_eeprom_ctrl_t *ectrl,
	struct eeprom_data_access_t *data_access)
{
	char *rx_buf;
	int ret;

	if (NULL == data_access->data) {
		pr_err("%s: Invalid read buffer.\n", __func__);
		return -EINVAL;
	}

	rx_buf = kmalloc(data_access->num_bytes + SPI_PROTOCOL_OFFSET,
		GFP_KERNEL);
	if (NULL == rx_buf) {
		pr_err("%s: Error allocating rx_buf.\n", __func__);
		return -ENOMEM;
	}

	ret = imx175_spi_read(ectrl, data_access->addr, rx_buf,
		data_access->num_bytes);
	if (!ret) {
		pr_err("%s: spi_read is success\n", __func__);
		if (copy_to_user(data_access->data,
			&rx_buf[SPI_PROTOCOL_OFFSET],
			data_access->num_bytes)) {
			pr_err("%s: Error copy to userspace\n", __func__);
			ret = -EFAULT;
		}
	}
	kfree(rx_buf);
	return ret;
}

int32_t imx175_direct_write(struct msm_eeprom_ctrl_t *ectrl,
	struct eeprom_data_access_t *data_access)
{
	char *rx_buf;
	char *tx_buf;
	int ret;

	if (NULL == data_access->data) {
		pr_err("%s: Invalid read buffer.\n", __func__);
		return -EINVAL;
	}

	tx_buf = kmalloc(data_access->num_bytes + SPI_PROTOCOL_OFFSET,
		GFP_KERNEL);
	if (NULL == tx_buf) {
		pr_err("%s: Error allocating tx_buf.\n", __func__);
		return -ENOMEM;
	}

	rx_buf = kmalloc(data_access->num_bytes + SPI_PROTOCOL_OFFSET,
		GFP_KERNEL);
	if (NULL == rx_buf) {
		pr_err("%s: Error allocating rx_buf.\n", __func__);
		kfree(tx_buf);
		return -ENOMEM;
	}

	if (copy_from_user(&tx_buf[SPI_PROTOCOL_OFFSET], data_access->data,
		data_access->num_bytes)) {
		pr_err("%s: Error copy from userspace\n", __func__);
		kfree(tx_buf);
		kfree(rx_buf);
		return -EFAULT;
	}

	ret = imx175_spi_page_program(ectrl, data_access->addr, tx_buf,
		rx_buf, data_access->num_bytes);
	kfree(tx_buf);
	kfree(rx_buf);
	return ret;
}

int32_t imx175_direct_erase(struct msm_eeprom_ctrl_t *ectrl,
	struct eeprom_data_access_t *data_access)
{
	int ret;
	ret = imx175_spi_sector_erase(ectrl, data_access->addr);
	if (ret)
		pr_err("%s: Error sector erase: %d\n", __func__, ret);
	return ret;
}

static struct v4l2_subdev_core_ops imx175_eeprom_subdev_core_ops = {
	.ioctl = msm_eeprom_subdev_ioctl,
};

static struct v4l2_subdev_ops imx175_eeprom_subdev_ops = {
	.core = &imx175_eeprom_subdev_core_ops,
};

/* data after formatting*/
static struct msm_calib_af imx175_af_data;
static struct msm_calib_wb imx175_wb_data;
static struct msm_calib_lsc imx175_lsc_data;
static struct msm_calib_wb imx175_gld_wb_data;
static struct msm_calib_lsc imx175_gld_lsc_data;

//uint16_t gTest;

/*Start : shchang@qti.qualcomm.com - 20130308 */
uint16_t inf_dac1; 		/*Infinity Mechnical Stop*/
uint16_t inf_dac2; 		/*Infinity Mechnical Stop*/ 		
uint16_t start_dac;		/*Pan 1.2M*/
uint16_t macro_dac1; 	/*Macro Mechnical Stop*/
uint16_t macro_dac2; 	/*Macro Mechnical Stop*/		
/*End : shchang@qti.qualcomm.com - 20130308 */

uint16_t cal_offset; 	/*SEMCO request by Lizk 05112013*/	

/* based on F-Rom definition*/
struct sFRom_af {
	uint32_t inf1;
	uint32_t inf2;
	uint32_t macro1;
	uint32_t macro2;
	uint32_t position;
#if defined(CONFIG_MACH_KS02)
	uint32_t pid;/* Randy PID*/
#else /* for Melius and Serrano */
	uint32_t panoffest;/* SEMCO request by Lizk 05112013*/
	uint32_t caloffset;/* SEMCO request by Lizk 05112013*/
#endif
};
struct sFRom_wb {
	uint32_t rg;
	uint32_t bg;
	uint32_t gr_gb;
};
struct sFRom_lsc {
	uint16_t r_gain[221];
	uint16_t gr_gain[221];
	uint16_t gb_gain[221];
	uint16_t b_gain[221];
};
struct sFRom_af FRom_af;
struct sFRom_wb FRom_wb;
struct sFRom_lsc FRom_lsc;

/* buffer for receiving data from F-Rom*/
static uint8_t imx175_afcalib_data[SPI_PROTOCOL_OFFSET + sizeof(FRom_af)];
static uint8_t imx175_wbcalib_data[SPI_PROTOCOL_OFFSET + sizeof(FRom_wb)];
static uint8_t imx175_lsccalib_data[SPI_PROTOCOL_OFFSET + sizeof(FRom_lsc)];
static uint8_t imx175_gld_wbcalib_data[SPI_PROTOCOL_OFFSET + sizeof(FRom_wb)];
static uint8_t imx175_gld_lsccalib_data[SPI_PROTOCOL_OFFSET + sizeof(FRom_lsc)];
/* patch#183359 : s*/
static uint8_t imx175_mod_info_data[SPI_PROTOCOL_OFFSET + 4];
/* patch#183359 : e*/
static struct msm_camera_eeprom_info_t imx175_calib_supp_info = {	//Check by teddy
	{TRUE, sizeof(imx175_af_data), 0, 1},
	{TRUE, sizeof(imx175_wb_data), 1, 1},
	{TRUE, sizeof(imx175_lsc_data), 2, 1},
	{FALSE, 0, 0, 1},
	{FALSE, 0, 0, 1},
	{TRUE, sizeof(imx175_gld_wb_data), 3, 1},
	{FALSE, sizeof(imx175_gld_lsc_data), 4, 1},
};

static struct msm_camera_eeprom_read_t imx175_eeprom_read_tbl[] = {
	{0x000040, &imx175_mod_info_data[0], sizeof(imx175_mod_info_data), 0},
	{0x001000, &imx175_afcalib_data[0], sizeof(FRom_af), 0},
	{0x002000, &imx175_wbcalib_data[0], sizeof(FRom_wb), 0},
	{0x00300C, &imx175_lsccalib_data[0], sizeof(FRom_lsc), 0},/*origi*/
	{0x001200, &imx175_gld_wbcalib_data[0], sizeof(FRom_wb), 0},
	{0x001218, &imx175_gld_lsccalib_data[0], sizeof(FRom_lsc), 0},
};

static struct msm_camera_eeprom_data_t imx175_eeprom_data_tbl[] = {
	{&imx175_af_data, sizeof(imx175_af_data)},
	{&imx175_wb_data, sizeof(imx175_wb_data)},
	{&imx175_lsc_data, sizeof(imx175_lsc_data)},
	{&imx175_gld_wb_data, sizeof(imx175_wb_data)},
	{&imx175_gld_lsc_data, sizeof(imx175_lsc_data)},
};


static void imx175_format_afdata(void)
{
	struct sFRom_af *af =
		(struct sFRom_af *)&imx175_afcalib_data[SPI_PROTOCOL_OFFSET];

/*Start : shchang@qti.qualcomm.com - 20130312 */
	//imx175_af_data.inf_dac = (uint16_t)af->inf1;
	//imx175_af_data.macro_dac = (uint16_t)af->macro1;
/*End : shchang@qti.qualcomm.com - 20130312 */


/*Start : shchang@qti.qualcomm.com - 20130311 */
/*
	printk("[shchang:AF] inf1 = %d, inf2 = %d, macro1 = %d, macro2 = %d, position = %d\n", 
		((uint16_t)ftov_LEn(af->inf1)), ((uint16_t)ftov_LEn(af->inf2)), ((uint16_t)ftov_LEn(af->macro1)), ((uint16_t)ftov_LEn(af->macro2)),((uint16_t)ftov_LEn(af->position)));
*/		
/*End : shchang@qti.qualcomm.com - 20130311 */

	/*Start : shchang@qualcomm.com : 1122*/
	if (((uint16_t)ftov_BEn(af->position)) == 0x0000 || ((uint16_t)ftov_BEn(af->position)) == 0xFFFF) {
		if (((uint16_t)ftov_LEn(af->position)) == 0x0000 || ((uint16_t)ftov_LEn(af->position)) == 0xFFFF) {
			imx175_calib_supp_info.af.is_supported = FALSE;
			pr_err("[shchang : %s] No data!!\n", __func__);
		} else {
#if defined(CONFIG_MACH_KS02)
			imx175_af_data.start_dac = start_dac = (uint16_t)ftov_LEn(af->position);
#else
			/*Start : shchang@qti.qualcomm.com - 20130312 */
			imx175_af_data.inf_dac1 = inf_dac1 = (uint16_t)ftov_LEn(af->inf1);			/*Infinity Mechnical Stop*/
			imx175_af_data.inf_dac2 = inf_dac2 = (uint16_t)ftov_LEn(af->inf2);			/*Infinity Mechnical Stop*/			
			imx175_af_data.start_dac = start_dac = (uint16_t)ftov_LEn(af->position);	/*Pan 1.2M*/
			imx175_af_data.macro_dac1 = macro_dac1 = (uint16_t)ftov_LEn(af->macro1);		/*Macro Mechnical Stop*/
			imx175_af_data.macro_dac2 = macro_dac2 = (uint16_t)ftov_LEn(af->macro2);		/*Macro Mechnical Stop*/			
			/*End : shchang@qti.qualcomm.com - 20130312 */
#endif
			pr_err("[shchang : %s] Little Endian!!\n", __func__);
		}
	} else {
		imx175_af_data.start_dac = start_dac = (uint16_t)ftov_BEn(af->position);
		pr_err("[shchang : %s] Big Endian!!\n", __func__);
	}
/*End : shchang@qualcomm.com : 1122*/
#if defined(CONFIG_MACH_KS02)
#if TEST_TEMP /* Randy PID*/
	CDBG("%s: af->pid (0x%x)\n", __func__, af->pid);
	if (((uint16_t)ftov_BEn(af->pid)) == 0x00) {
		if (((uint16_t)ftov_LEn(af->pid)) == 0x00) {
			imx175_calib_supp_info.af.is_supported = FALSE;
			pr_err("[shchang : %s] No data!!\n", __func__);
		} else {
			imx175_af_data.pid_dac
				= (uint16_t)ftov_LEn(af->pid);
			pr_err("[shchang : %s] Little Endian!!\n", __func__);
		}
	} else {
		imx175_af_data.pid_dac = (uint16_t)ftov_BEn(af->pid);
		pr_err("[shchang : %s] Big Endian!!\n", __func__);
	}
	CDBG("%s: PID (0x%x)\n", __func__, imx175_af_data.pid_dac);
#endif /* Randy PID */
#else /* for Melius and Serrano */

	if((uint16_t)ftov_BEn(af->caloffset) == 0x0000 || (uint16_t)ftov_BEn(af->caloffset) == 0xFFFF)
	{
		imx175_af_data.cal_offset_dac  = cal_offset = 0;		/* SEMCO request by Lizk*/			
	}
	else
	{
		imx175_af_data.cal_offset_dac  = cal_offset = (uint16_t)ftov_BEn(af->caloffset);		/* SEMCO request by Lizk*/			
	}
#endif
	/*pr_err("[shchang:CAL4-1] af->Pan_position = 0x%x,
	imx175_af_data.start_dac = %d\n",
	af->position, imx175_af_data.start_dac);*/
}
static void imx175_format_wbdata(void)
{
	struct sFRom_wb *wb =
		(struct sFRom_wb *)&imx175_wbcalib_data[SPI_PROTOCOL_OFFSET];
	struct sFRom_wb *gld_wb =
	(struct sFRom_wb *)&imx175_gld_wbcalib_data[SPI_PROTOCOL_OFFSET];

/*
	printk("[shchang] [Little]gld_wb->rg = %d, gld_wb->bg = %d, wb->rg = %d, wb->bg = %d\n", 
		(uint16_t)ftov_LEn(gld_wb->rg),(uint16_t)ftov_LEn(gld_wb->bg), (uint16_t)ftov_LEn(wb->rg), (uint16_t)ftov_LEn(wb->bg)); 
	printk("[shchang] [Golden]gld_wb->rg = %d, gld_wb->bg = %d, wb->rg = %d, wb->bg = %d\n", 
		(uint16_t)ftov_BEn(gld_wb->rg),(uint16_t)ftov_BEn(gld_wb->bg), (uint16_t)ftov_BEn(wb->rg), (uint16_t)ftov_BEn(wb->bg)); 	
*/

	if (( ((uint16_t)ftov_BEn(wb->rg) == 0x0000) &&
		   ((uint16_t)ftov_BEn(wb->bg) == 0x0000) &&
		   ((uint16_t)ftov_BEn(gld_wb->rg) == 0x0000) &&
		   ((uint16_t)ftov_BEn(gld_wb->bg) == 0x0000)  ) ||
		( ((uint16_t)ftov_BEn(wb->rg) == 0x0000) &&
		  ((uint16_t)ftov_BEn(wb->bg) == 0x0000) &&
		  ((uint16_t)ftov_BEn(gld_wb->rg) == 0xFFFF) &&
		  ((uint16_t)ftov_BEn(gld_wb->bg) == 0xFFFF) ) ){
		if (( ((uint16_t)ftov_LEn(wb->rg) == 0x0000) &&
		   	   ((uint16_t)ftov_LEn(wb->bg) == 0x0000) &&
		       ((uint16_t)ftov_LEn(gld_wb->rg) == 0x0000) &&
		       ((uint16_t)ftov_LEn(gld_wb->bg) == 0x0000)  ) ||
		    ( ((uint16_t)ftov_LEn(wb->rg) == 0x0000) &&
		      ((uint16_t)ftov_LEn(wb->bg) == 0x0000) &&
		      ((uint16_t)ftov_LEn(gld_wb->rg) == 0xFFFF) &&
		      ((uint16_t)ftov_LEn(gld_wb->bg) == 0xFFFF) ) ) {
			imx175_calib_supp_info.wb.is_supported = FALSE;
			pr_err("[shchang : %s] No data!!\n", __func__);
		} else {

			/*Start : shchang@qti.qualcomm.com - 20130308 */
			if((((uint16_t)ftov_LEn(gld_wb->rg)) == 0x0000 && ((uint16_t)ftov_LEn(gld_wb->bg) == 0x0000)) ||
				(((uint16_t)ftov_LEn(gld_wb->rg)) == 0xFFFF && ((uint16_t)ftov_LEn(gld_wb->bg) == 0xFFFF)))
			{
				imx175_wb_data.r_over_g = (uint16_t)ftov_LEn(wb->rg);
				imx175_wb_data.b_over_g = (uint16_t)ftov_LEn(wb->bg);
				imx175_wb_data.gr_over_gb = 1;/*(uint16_t)wb->gr_gb;*/

				/*Start : shchang@qti.qualcomm.com - 20130311 (Hardcoding the golden values)*/
				imx175_gld_wb_data.r_over_g = 1;//(uint16_t)ftov_LEn(wb->rg); //1783;
				imx175_gld_wb_data.b_over_g = 1;//(uint16_t)ftov_LEn(wb->bg); //2255;
				imx175_gld_wb_data.gr_over_gb = 1;			
				/*End : shchang@qti.qualcomm.com - 20130311 (Hardcoding the golden values)*/
			}
			else
			{
				imx175_wb_data.r_over_g = (uint16_t)ftov_LEn(wb->rg);
				imx175_wb_data.b_over_g = (uint16_t)ftov_LEn(wb->bg);
				imx175_wb_data.gr_over_gb = 1;/*(uint16_t)wb->gr_gb;*/

				imx175_gld_wb_data.r_over_g = (uint16_t)ftov_LEn(gld_wb->rg);
				imx175_gld_wb_data.b_over_g = (uint16_t)ftov_LEn(gld_wb->bg);
				imx175_gld_wb_data.gr_over_gb = 1;
			}
			/*End : shchang@qti.qualcomm.com - 20130308 */
			pr_err("[shchang : %s] Little Endian!!\n", __func__);
		}
	} else {
		/*Start : shchang@qti.qualcomm.com - 20130308 */
		if((((uint16_t)ftov_BEn(gld_wb->rg)) == 0x0000 && ((uint16_t)ftov_BEn(gld_wb->bg) == 0x0000)) ||
			(((uint16_t)ftov_BEn(gld_wb->rg)) == 0xFFFF && ((uint16_t)ftov_BEn(gld_wb->bg) == 0xFFFF)))
		{
			imx175_wb_data.r_over_g = (uint16_t)ftov_BEn(wb->rg);
			imx175_wb_data.b_over_g = (uint16_t)ftov_BEn(wb->bg);
			imx175_wb_data.gr_over_gb = 1;/*(uint16_t)wb->gr_gb;*/

			/*Start : shchang@qti.qualcomm.com - 20130311 (Hardcoding the golden values)*/
			imx175_gld_wb_data.r_over_g = 1; //(uint16_t)ftov_BEn(wb->rg); //1783; //LSI : 2331
			imx175_gld_wb_data.b_over_g = 1; //(uint16_t)ftov_BEn(wb->bg); //2255; //LSI : 2430
			imx175_gld_wb_data.gr_over_gb = 1;/*(uint16_t)gld_wb->gr_gb;*/
			/*End : shchang@qti.qualcomm.com - 20130311 (Hardcoding the golden values) */
		}
		else
		{
			imx175_wb_data.r_over_g = (uint16_t)ftov_BEn(wb->rg);
			imx175_wb_data.b_over_g = (uint16_t)ftov_BEn(wb->bg);
			imx175_wb_data.gr_over_gb = 1;/*(uint16_t)wb->gr_gb;*/

			imx175_gld_wb_data.r_over_g = 1; //imx175_wb_data.r_over_g;
			imx175_gld_wb_data.b_over_g = 1; //imx175_wb_data.b_over_g;
			imx175_gld_wb_data.gr_over_gb = 1;/*(uint16_t)gld_wb->gr_gb;*/		
		}
		/*End : shchang@qti.qualcomm.com - 20130308 */
		pr_err("[shchang : %s] Big Endian!!\n", __func__);
	}

	/*End : shchang@qualcomm.com : 1122*/

	/*pr_err("[shchang:CAL2-1] wb->rg = 0x%x, wb->bg = 0x%x\n",
		imx175_wb_data.r_over_g, imx175_wb_data.b_over_g);
	pr_err("[shchang:CAL3-1] GD_wb->rg = 0x%x, GD_wb->bg = 0x%x\n",
		imx175_gld_wb_data.r_over_g, imx175_gld_wb_data.b_over_g);*/
}
static void imx175_format_lscdata(void)
{
	int i;
	uint16_t *r = (uint16_t *)&imx175_lsccalib_data[SPI_PROTOCOL_OFFSET];
	uint16_t *gr = (uint16_t *)((uint8_t *)r +
		sizeof_member(struct sFRom_lsc, r_gain));
	uint16_t *gb = (uint16_t *)((uint8_t *)gr +
		sizeof_member(struct sFRom_lsc, gr_gain));
	uint16_t *b = (uint16_t *)((uint8_t *)gb +
		sizeof_member(struct sFRom_lsc, gb_gain));

	uint16_t *gld_r =
		(uint16_t *)&imx175_gld_lsccalib_data[SPI_PROTOCOL_OFFSET];
	uint16_t *gld_gr = (uint16_t *)((uint8_t *)gld_r +
		sizeof_member(struct sFRom_lsc, r_gain));
	uint16_t *gld_gb = (uint16_t *)((uint8_t *)gld_gr +
		sizeof_member(struct sFRom_lsc, gr_gain));
	uint16_t *gld_b = (uint16_t *)((uint8_t *)gld_gb +
		sizeof_member(struct sFRom_lsc, gb_gain));

	for (i = 0; i < 221; i++) {
		imx175_lsc_data.r_gain[i] = r[i];
		/*pr_err("[shchang : CAL] imx175_lsc_data.r_gain[&d] = %d\n",
			i, imx175_lsc_data.r_gain[i]);*/
	}
	/*pr_err("[shchang : CAL] imx175_lsc_data.r_gain[0]
	= 0x%x\n", imx175_lsc_data.r_gain[0]);
	pr_err("[shchang : CAL] imx175_lsc_data.r_gain[2]
	= -x%x\n", imx175_lsc_data.r_gain[2]);*/

	for (i = 0; i < 221; i++)
		imx175_lsc_data.b_gain[i] = b[i];

	/*pr_err("[shchang : CAL] imx175_lsc_data.b_gain[0] =
	%x\n",	imx175_lsc_data.b_gain[0]);
	pr_err("[shchang : CAL] imx175_lsc_data.b_gain[1] =
	%x\n",	imx175_lsc_data.b_gain[1]);*/
	for (i = 0; i < 221; i++)
		imx175_lsc_data.gr_gain[i] = gr[i];

	/*pr_err("[shchang : CAL] imx175_lsc_data.gr_gain[0] =
	%x\n",	imx175_lsc_data.gr_gain[0]);
	pr_err("[shchang : CAL] imx175_lsc_data.gr_gain[1] =
	%x\n",	imx175_lsc_data.gr_gain[1]);*/

	for (i = 0; i < 221; i++)
		imx175_lsc_data.gb_gain[i] = gb[i];

	/*pr_err("[shchang : CAL] imx175_lsc_data.gb_gain[0] =
	%x\n",	imx175_lsc_data.gb_gain[0]);
	pr_err("[shchang : CAL] imx175_lsc_data.gb_gain[1] =
	%x\n",	imx175_lsc_data.gb_gain[1]);*/

	for (i = 0; i < 221; i++)
		imx175_gld_lsc_data.r_gain[i] = gld_r[i];

	for (i = 0; i < 221; i++)
		imx175_gld_lsc_data.b_gain[i] = gld_b[i];

	for (i = 0; i < 221; i++)
		imx175_gld_lsc_data.gr_gain[i] = gld_gr[i];

	for (i = 0; i < 221; i++)
		imx175_gld_lsc_data.gb_gain[i] = gld_gb[i];
}

/* patch#183359 : s*/
uint8_t imx175_check_module_info(void)
{
	uint8_t *ptr = &imx175_mod_info_data[SPI_PROTOCOL_OFFSET];

	CDBG("[shchang:CAL mod_info] %x:%x:%x:%x\n",
		ptr[0], ptr[1], ptr[2], ptr[3]);
	/*log : [shchang:CAL mod_info] (4F/53/43):30:38:51*/
	
	/***************************************************
	  Caution!!! This is for Melius/Serrano/KS02/Crater model. 
	  *Please Do not seperate below condition based on deconfig!!
	  ***************************************************/
	  
	if (ptr[0] != 0x4F && ptr[0] != 0x53 && ptr[0] != 0x43)
		return 0;
	if ((ptr[1] != 0x30) && (ptr[1] != 0x38))
		return 0;
	if ((ptr[2] != 0x30) && (ptr[2] != 0x38))
		return 0;
	if (ptr[3] != 0x51)
		return 0;
	return 1;
}
/* patch#183359 : e*/
void imx175_format_calibrationdata(void)
{
	/* patch#183359 : s*/
	uint8_t is_all_supported = 0;
	is_all_supported = imx175_check_module_info();
	/*Start : shchang@qti.qualcomm.com - 20130312 */
	pr_err("%s: is_all_supported= %d\n", __func__, is_all_supported);
	/*End : shchang@qti.qualcomm.com - 20130312 */
	
	if (is_all_supported) {
		/* patch#183359 : e*/
		imx175_format_afdata();
		imx175_format_wbdata();
		imx175_format_lscdata();
		/* patch#183359 : s*/
	} else {
		imx175_calib_supp_info.af.is_supported = FALSE;
		imx175_calib_supp_info.wb.is_supported = FALSE;
		imx175_calib_supp_info.lsc.is_supported = FALSE;
		imx175_calib_supp_info.gld_wb.is_supported = FALSE;
		/*imx175_calib_supp_info.gld_lsc.is_supported = FALSE;*/
	}
		/* patch#183359 : e*/
}

static int __devinit imx175_spi_probe(struct spi_device *spi)
{
	int rc = 0;
	struct msm_cam_subdev_info sd_info;
	struct msm_eeprom_ctrl_t *eeprom_ctrl =
		(struct msm_eeprom_ctrl_t *)spi_get_device_id(spi)->driver_data;

	eeprom_ctrl->spi = spi;
	pr_err("imx175 spi driver probe\n");


	if (eeprom_ctrl->func_tbl.eeprom_init != NULL) {
		rc = eeprom_ctrl->func_tbl.eeprom_init(eeprom_ctrl,
			eeprom_ctrl->i2c_client.client->adapter);
	}
	msm_camera_eeprom_read_tbl(eeprom_ctrl,
		eeprom_ctrl->read_tbl,
		eeprom_ctrl->read_tbl_size);

	if (eeprom_ctrl->func_tbl.eeprom_format_data != NULL)
		eeprom_ctrl->func_tbl.eeprom_format_data();

	if (eeprom_ctrl->func_tbl.eeprom_release != NULL)
		rc = eeprom_ctrl->func_tbl.eeprom_release(eeprom_ctrl);

	/* Initialize sub device */
	v4l2_spi_subdev_init(&eeprom_ctrl->sdev,
		spi, eeprom_ctrl->eeprom_v4l2_subdev_ops);

	sd_info.sdev_type = EEPROM_DEV;
	sd_info.sd_index = 0;
	sd_info.irq_num = 0;
	msm_cam_register_subdev_node(&eeprom_ctrl->sdev, &sd_info);

	return 0;
}

static const struct spi_device_id imx175_spi_id[] = {
	{"imx175_spi", (kernel_ulong_t)&imx175_eeprom_t},
	{ }
};

static struct spi_driver imx175_spi_driver = {
	.id_table = imx175_spi_id,
	.driver = {
		.name	= "imx175_spi",
		.bus	= &spi_bus_type,
		.owner	= THIS_MODULE,
	},
	.probe		= imx175_spi_probe,
};

void imx175_eeprom_init(void)
{
	spi_register_driver(&imx175_spi_driver);
}

static struct msm_eeprom_ctrl_t imx175_eeprom_t = {
	.eeprom_v4l2_subdev_ops = &imx175_eeprom_subdev_ops,
	.eeprom_mutex = &imx175_eeprom_mutex,

	.func_tbl = {
		.eeprom_init = NULL,
		.eeprom_release = NULL,
		.eeprom_get_info = msm_camera_eeprom_get_info,
		.eeprom_get_data = msm_camera_eeprom_get_data,
		.eeprom_set_dev_addr = NULL,
		.eeprom_format_data = imx175_format_calibrationdata,
		.eeprom_read = imx175_spi_read,
		.eeprom_direct_data_read = imx175_direct_read,
		.eeprom_direct_data_write = imx175_direct_write,
		.eeprom_direct_data_erase = imx175_direct_erase,
	},
	.info = &imx175_calib_supp_info,
	.info_size = sizeof(struct msm_camera_eeprom_info_t),
	.read_tbl = imx175_eeprom_read_tbl,
	.read_tbl_size = ARRAY_SIZE(imx175_eeprom_read_tbl),
	.data_tbl = imx175_eeprom_data_tbl,
	.data_tbl_size = ARRAY_SIZE(imx175_eeprom_data_tbl),
};
