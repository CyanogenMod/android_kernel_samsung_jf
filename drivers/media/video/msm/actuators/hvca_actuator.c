	/* Copyright (c) 2012, The Linux Foundation. All rights reserved.
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

#include <linux/delay.h>
#include <linux/types.h>
#include <media/msm_camera.h>
#include "hvca_actuator.h"

/* HVCA */

#define HVCA_SLAVE_ADDR 0xE4

#define STMV_SIZE		0x0180

#define STMCHTG_ON		0x08
#define STMSV_ON		0x04
#define STMVEN_ON		0x01

#define STMLFF_OFF		0x00

#define STMCHTG_SET		STMCHTG_ON
#define STMSV_SET		STMSV_ON
#define STMLFF_SET		STMLFF_OFF

/* Convergence Judgement */
#define INI_MSSET_211	0x00
/*Start : shchang@qti.qualcomm.com - 20130402 */
#define CHTGOKN_WAIT	5000
/*End : shchang@qti.qualcomm.com - 20130402 */
/* Register 80h ~  9F */
#define SWTCH_211		0x83
#define	MSSET_211		0x8F
#define	STMVEN_211		0x8A
#define	STMVENDH_211	0xA1
#define EQENBL          0x87
/* Delay RAM    00h ~ 3Fh */
#define	RZ_211H			0x04
#define	MS1Z22_211H		0x18
#define	MS1Z12_211H		0x16


/* EEPROM */
#define EEP_INIT_SECTION_START_ADDR   0x0030
#define EEP_INIT_SECTION_END_ADDR     0x01BF

#define AF_CAL_INF1_H	0x20
#define AF_CAL_INF1_L	0x21
#define AF_CAL_MAC1_H	0x22
#define AF_CAL_MAC1_L	0x23
#define AF_CAL_INF2_H	0x24
#define AF_CAL_INF2_L	0x25
#define AF_CAL_MAC2_H	0x26
#define AF_CAL_MAC2_L	0x27

#define DIRECT_MODE		0x00
#define	INDIRECT_EEPROM	0x10
#define	INDIRECT_HVCA	0x20
#define	MASK_AND		0x70
#define	MASK_OR			0x80

#define	DATA_1BYTE		0x01
#define	DATA_2BYTE		0x02

#define OFFSET_LIMIT	0x07

static struct msm_camera_i2c_client *af_client;
static int16_t position_inf1, position_mac1;
/*Start : shchang@qti.qualcomm.com - 20130308 */
static int16_t position_inf2, position_mac2, position_pan;
/*End : shchang@qti.qualcomm.com - 20130308 */

/* ========================================================================== */
static int32_t hvca_read(uint8_t addr, uint8_t *data)
{
	int32_t rc;
	uint16_t buf;

	af_client->client->addr = 0xE5;

	rc = msm_camera_i2c_read(af_client, addr, &buf, MSM_CAMERA_I2C_BYTE_DATA);
	if (rc < 0) {
		CDBG_HVCA("hvca_read failed, addr = 0x%x!\n", addr);
		return rc;
	}

	*data = (uint8_t)buf;

	return 0;
}
static int32_t hvca_read_w(uint8_t addr, int16_t *data)
{
	int32_t rc;
	uint8_t data_i2c;
	uint16_t buffer = 0;

	rc = hvca_read(addr, &data_i2c);
	if (rc != 0) {
		CDBG_HVCA("%s/%d: Error\n", __func__, __LINE__);
		return rc;
	}
	buffer = data_i2c << 8;
	rc = hvca_read(addr + 1, &data_i2c);
	if (rc != 0) {
		CDBG_HVCA("%s/%d: Error\n", __func__, __LINE__);
		return rc;
	}
	*data = buffer | data_i2c;

	return 0;
}

static int32_t eeprom_read(uint16_t addr, uint8_t *data) /* lsj */
{
	int32_t rc;
	uint16_t buf;

	if (addr >= 0x000 && addr <= 0x0FF)
		af_client->client->addr = 0xA1;
	else if (addr >= 0x100 && addr <= 0x1FF)
		af_client->client->addr = 0xA3;

	addr &= 0x00FF;
	rc = msm_camera_i2c_read(af_client, addr, &buf, MSM_CAMERA_I2C_BYTE_DATA);
	if (rc < 0) {
		CDBG_HVCA("eeprom_read failed, addr = 0x%x!\n", addr);
		return rc;
	}

	*data = (uint8_t)buf;

	return 0;
}

static int32_t hvca_write(uint8_t addr, uint8_t data)
{
	int32_t rc;

	af_client->client->addr = HVCA_SLAVE_ADDR;

	rc = msm_camera_i2c_write(af_client, addr, data, MSM_CAMERA_I2C_BYTE_DATA);
	if (rc < 0) {
		CDBG_HVCA("hvca_write_b failed, addr = 0x%x, val = 0x%x!\n",
			addr, data);
		return rc;
	}

	return 0;
}
static int32_t hvca_write_w(uint8_t addr, uint16_t data)
{
	int32_t rc;

	af_client->client->addr = HVCA_SLAVE_ADDR;

	//CDBG_HVCA("%s reg addr = 0x%x data = 0x%x\n", __func__, addr, data);
	rc = msm_camera_i2c_write(af_client, addr, data, MSM_CAMERA_I2C_WORD_DATA);
	if (rc < 0) {
		CDBG_HVCA("hvca_write_w failed, addr = 0x%x, val = 0x%x!\n",
			addr, data);
		return rc;
	}

	return 0;
}

static int8_t write_E2PDataToHVCA(void)
{
	uint8_t ucAdrIdx, ucData1, ucData2, ucError;
	uint16_t usEEPAddr, usTimeDelay, usData, usMask;

	uint8_t E2P_info_add = 0;
	uint8_t E2P_info_type = 0;
	uint8_t E2P_info_data1 = 0;
	uint8_t E2P_info_data2 = 0;

	uint8_t data_i2c = 0;
	int8_t rc = 0;

	ucAdrIdx = 0 ;
	usEEPAddr = EEP_INIT_SECTION_START_ADDR;

	//CDBG_HVCA("%s : E\n", __func__);

	while ((E2P_info_add != 0xFF) && 
		   (usEEPAddr < (EEP_INIT_SECTION_END_ADDR + 1))) {

		/* Set EEPROM Address */
		usEEPAddr = EEP_INIT_SECTION_START_ADDR + ucAdrIdx * 4;

		/* Get 4bytes EEPROM data */
		/* HVCA Address to write eeprom Data1,Data2 by the Data type */
		rc = eeprom_read(usEEPAddr, &E2P_info_add);
		if (rc != 0) {
			CDBG_HVCA("%s: Error Get 4bytes EEPROM data\n", __func__);
			return rc;
		}
		/* Data type */
		rc = eeprom_read(usEEPAddr + 1, &E2P_info_type);
		if (rc != 0) {
			CDBG_HVCA("%s: Error Data type\n", __func__);
			return rc;
		}
		/* Data1 */
		rc = eeprom_read(usEEPAddr + 2, &E2P_info_data1);
		if (rc != 0) {
			CDBG_HVCA("%s: Error Get Data1\n", __func__);
			return rc;
		}
		/* Data2 */
		rc = eeprom_read(usEEPAddr + 3, &E2P_info_data2);
		if (rc != 0) {
			CDBG_HVCA("%s: Error Get Data2\n", __func__);
			return rc;
		}

		//CDBG_HVCA("%s: add[0x%x] type[0x%x] data1[0x%x] data2[0x%x]\n", __func__, E2P_info_add, E2P_info_type, E2P_info_data1, E2P_info_data2);

		/* Address Value = 0xFF : End of Setting Values */
		if (E2P_info_add == 0xFF)
			break;

		/* Address Value = 0xDD : Time delay */
		else if (E2P_info_add == 0xDD) {
			usTimeDelay = (((uint16_t)E2P_info_data1 << 8) & 0xFF00) |
						  ((uint16_t)E2P_info_data2 & 0x00FF);
			usleep(usTimeDelay);

		/* Set Values from EEPROM to HVCA by DataType */
		} else {
			ucError = 0;
			/* Write 1 byte data to the HVCA Drive IC by data type */
			if ((E2P_info_type & 0x0F) == DATA_1BYTE) {
				switch (E2P_info_type & 0xF0) {
				case DIRECT_MODE:
					ucData1 = E2P_info_data1;
					break;
				case INDIRECT_EEPROM:
					rc = eeprom_read(E2P_info_data1, &ucData1);
					if (rc != 0) {
						CDBG_HVCA("%s: %d Error\n", __func__, __LINE__);
						return rc;
					}
					break;
				case INDIRECT_HVCA:
					rc = hvca_read(E2P_info_data1, &ucData1);
					if (rc != 0) {
						CDBG_HVCA("%s: %d Error\n", __func__, __LINE__);
						return rc;
					}
					break;
				case MASK_AND:
					rc = hvca_read(E2P_info_add, &ucData1);
					if (rc != 0) {
						CDBG_HVCA("%s: %d Error\n", __func__, __LINE__);
						return rc;
					}
					ucData1 = ucData1 & E2P_info_data1;
					break;
				case MASK_OR:
					rc = hvca_read(E2P_info_add, &ucData1);
					if (rc != 0) {
						CDBG_HVCA("%s: %d Error\n", __func__, __LINE__);
						return rc;
					}
					ucData1 = ucData1 | E2P_info_data1;
					break;
				default:
					ucError = 1;
					CDBG_HVCA("%s: error\n", __func__);
					break;
				}

				if (ucError != 1) {
					rc = hvca_write(E2P_info_add, ucData1);
					if (rc != 0) {
						CDBG_HVCA("%s: %d Error\n", __func__, __LINE__);
						return rc;
					}
				}
			} else {
				/*Write to the HVCA Drive IC by data type*/
				switch (E2P_info_type & 0xF0) {
				case DIRECT_MODE:
					usData = (((uint16_t)E2P_info_data1 << 8) & 0xFF00) |
							 ((uint16_t)E2P_info_data2 & 0x00FF);
					break;
				case INDIRECT_EEPROM:
					rc = eeprom_read(E2P_info_data1, &ucData1);
					if (rc != 0) {
						CDBG_HVCA("%s: %d Error\n", __func__, __LINE__);
						return rc;
					}
					rc = eeprom_read(E2P_info_data2, &ucData2);
					if (rc != 0) {
						CDBG_HVCA("%s: %d Error\n", __func__, __LINE__);
						return rc;
					}
					usData = (((uint16_t)ucData1 << 8) & 0xFF00) |
							 ((uint16_t)ucData2 & 0x00FF);
					break;
				case INDIRECT_HVCA:
					rc = hvca_read(E2P_info_data1, &ucData1);
					if (rc != 0) {
						CDBG_HVCA("%s: %d Error\n", __func__, __LINE__);
						return rc;
					}
					rc = hvca_read(E2P_info_data2, &ucData2);
					if (rc != 0) {
						CDBG_HVCA("%s: %d Error\n", __func__, __LINE__);
						return rc;
					}
					usData = (((uint16_t)ucData1 << 8) & 0xFF00) |
							 ((uint16_t)ucData2 & 0x00FF);
					break;
				case MASK_AND:
					usMask = (((uint16_t)E2P_info_data1 << 8) & 0xFF00) |
							 ((uint16_t)E2P_info_data2 & 0x00FF);

					rc = hvca_read(E2P_info_add, &ucData1);
					if (rc != 0) {
						CDBG_HVCA("%s: %d Error\n", __func__, __LINE__);
						return rc;
					}
					rc = hvca_read(E2P_info_add + 1, &ucData2);
					if (rc != 0) {
						CDBG_HVCA("%s: %d Error\n", __func__, __LINE__);
						return rc;
					}
					usData = (((uint16_t)ucData1 << 8) & 0xFF00) |
							 ((uint16_t)ucData2 & 0x00FF);

					usData = usData & usMask;
					break;
				case MASK_OR:
					usMask = (((uint16_t)E2P_info_data1 << 8) & 0xFF00)|
							 ((uint16_t)E2P_info_data2 & 0x00FF);

					rc = hvca_read(E2P_info_add, &ucData1);
					if (rc != 0) {
						CDBG_HVCA("%s: %d Error\n", __func__, __LINE__);
						return rc;
					}
					rc = hvca_read(E2P_info_add + 1, &ucData2);
					if (rc != 0) {
						CDBG_HVCA("%s: %d Error\n", __func__, __LINE__);
						return rc;
					}
					usData = (((uint16_t)ucData1 << 8) & 0xFF00) |
							 ((uint16_t)ucData2 & 0x00FF);

					usData = usData | usMask;
					break;
				default:
					ucError = 1;
					CDBG_HVCA("%s: error\n", __func__);
					break;
				}
				if (ucError != 1) {
					rc = hvca_write_w(E2P_info_add, usData);
					if (rc != 0) {
						CDBG_HVCA("%s: Error HVCA w\n", __func__);
						return rc;
					}
				}
			}
		}
		ucAdrIdx++;
	}

ucData1 = ucData1 & 0x7F;
hvca_write(0x83, ucData1);
// 0x5A
usData = (((uint16_t)0x08 << 8) & 0xFF00) | ((uint16_t)0x00 & 0x00FF);
hvca_write_w(0x5A, usData);
// 0x18
hvca_read(0x04, &ucData1);
hvca_read(0x05, &ucData2);
usData = (((uint16_t)ucData1 << 8) & 0xFF00) | ((uint16_t)ucData2 & 0x00FF);
hvca_write_w(0x18, usData);
// 0x16
usData = (((uint16_t)0x01 << 8) & 0xFF00) | ((uint16_t)0x80 & 0x00FF);
hvca_write_w(0x16, usData);

hvca_write(0xA0, 0x06);
// 0x83
hvca_read(0x83, &ucData1);
ucData1 = ucData1 | 0x80;
hvca_write(0x83, ucData1);


	/* read data(INF1) from EEPROM */
	rc = eeprom_read(AF_CAL_INF1_H, &data_i2c);
	if (rc != 0) {
		CDBG_HVCA("%s: Error Get EEPROM data for INF2\n", __func__);
		return rc;
	}
	CDBG_HVCA("%s: AF_CAL_INF1_H [0x%x]\n", __func__, data_i2c);
	position_inf1 = data_i2c << 8;
	rc = eeprom_read(AF_CAL_INF1_L, &data_i2c);
	if (rc != 0) {
		CDBG_HVCA("%s: Error Get EEPROM data for INF2\n", __func__);
		return rc;
	}
	CDBG_HVCA("%s: AF_CAL_INF1_L [0x%x]\n", __func__, data_i2c);
	position_inf1 |= data_i2c;
	/* read data(MAC1) from EEPROM */
	rc = eeprom_read(AF_CAL_MAC1_H, &data_i2c);
	if (rc != 0) {
		CDBG_HVCA("%s: Error Get EEPROM data for MAC2\n", __func__);
		return rc;
	}
	CDBG_HVCA("%s: AF_CAL_MAC1_H [0x%x]\n", __func__, data_i2c);
	position_mac1 = data_i2c << 8;
	rc = eeprom_read(AF_CAL_MAC1_L, &data_i2c);
	if (rc != 0) {
		CDBG_HVCA("%s: Error Get EEPROM data for MAC2\n", __func__);
		return rc;
	}
	CDBG_HVCA("%s: AF_CAL_MAC1_L [0x%x]\n", __func__, data_i2c);
	position_mac1 |= data_i2c;

	/* read data(INF2) from EEPROM */
	rc = eeprom_read(AF_CAL_INF2_H, &data_i2c);
	if (rc != 0) {
		CDBG_HVCA("%s: Error Get EEPROM data for INF2\n", __func__);
		return rc;
	}
	CDBG_HVCA("%s: AF_CAL_INF2_H [0x%x]\n", __func__, data_i2c);
	position_inf2 = data_i2c << 8;
	rc = eeprom_read(AF_CAL_INF2_L, &data_i2c);
	if (rc != 0) {
		CDBG_HVCA("%s: Error Get EEPROM data for INF2\n", __func__);
		return rc;
	}
	CDBG_HVCA("%s: AF_CAL_INF2_L [0x%x]\n", __func__, data_i2c);
	position_inf2 |= data_i2c;
	/* read data(MAC2) from EEPROM */
	rc = eeprom_read(AF_CAL_MAC2_H, &data_i2c);
	if (rc != 0) {
		CDBG_HVCA("%s: Error Get EEPROM data for MAC2\n", __func__);
		return rc;
	}
	CDBG_HVCA("%s: AF_CAL_MAC2_H [0x%x]\n", __func__, data_i2c);
	position_mac2 = data_i2c << 8;
	rc = eeprom_read(AF_CAL_MAC2_L, &data_i2c);
	if (rc != 0) {
		CDBG_HVCA("%s: Error Get EEPROM data for MAC2\n", __func__);
		return rc;
	}
	CDBG_HVCA("%s: AF_CAL_MAC2_L [0x%x]\n", __func__, data_i2c);
	position_mac2 |= data_i2c;

	return rc;
}

static uint16_t hvca_abs(int16_t val)
{
	if (val >= 0)
		return (uint16_t)val;

	return (uint16_t)(-val);
}

static uint8_t stmv211(uint8_t UcParMod, int16_t SsSmvEnd)
{
    int16_t sCurrentRZ = 0;
	int16_t sStmvSize;
	uint16_t UsSmvDpl;

    hvca_read_w(RZ_211H, &sCurrentRZ);

	UsSmvDpl = hvca_abs(sCurrentRZ - SsSmvEnd);

	//CDBG_HVCA("stmv211: displacement[0x%x] current[0x%x] want-to-go[0x%x]\n", UsSmvDpl, sCurrentRZ, SsSmvEnd);

	if ((UsSmvDpl <= STMV_SIZE) && ((UcParMod & 0x04) == STMSV_ON)) {
		if (UcParMod & STMCHTG_ON)
			hvca_write(MSSET_211, INI_MSSET_211 | 0x01);

		hvca_write_w(MS1Z22_211H, SsSmvEnd);

		/* Combine StepMove Enable Bit & StepMove Mode Bit */
		UcParMod |= STMVEN_ON;
	} else {
		if (sCurrentRZ < SsSmvEnd)
			sStmvSize = STMV_SIZE;
		else
			sStmvSize = -STMV_SIZE;
		hvca_write_w(MS1Z12_211H, sStmvSize);

		/* Set StepMove Target Positon */
		hvca_write_w(STMVENDH_211, SsSmvEnd);

		/* Combine StepMove Enable Bit & StepMove Mode Bit */
		UcParMod |= STMVEN_ON;
		/* Start StepMove */
		hvca_write(STMVEN_211, UcParMod);
	}

	return UcParMod;
}

static uint8_t stmv211Fin(uint8_t UcParMod)
{
	uint16_t UsSmvFin = 0;
	uint8_t data_i2c;
	uint8_t time_out_cnt = 0;

	do {
		/*Start : shchang@qti.qualcomm.com - 20130402 */
		usleep(1000);
		/*End : shchang@qti.qualcomm.com - 20130402 */
		
		hvca_read(STMVEN_211, &UcParMod);
		hvca_read_w(RZ_211H, &UsSmvFin);

		//CDBG_HVCA("stmv211Fin: RZ[0x%x] STMVEN_211[0x%x]\n", UsSmvFin, UcParMod);

		if ((UsSmvFin == 0x7FFF) || (UsSmvFin == 0x8001)) {
			/* Stop StepMove Operation */
			CDBG_HVCA("stmv211Fin: Stop StepMove Operation\n");
			hvca_write(STMVEN_211, UcParMod & 0xFE);
			UcParMod |= 0x10; /* STMVEN Failed */
		}

		time_out_cnt++;
	} while (UcParMod & STMVEN_ON);

	time_out_cnt = 0;
	if ((UcParMod & 0x08) == STMCHTG_ON) {
		usleep(CHTGOKN_WAIT);
		do {
			hvca_read(MSSET_211, &data_i2c);

			usleep(1000);
			time_out_cnt++;
		} while ((time_out_cnt < 15) && (data_i2c & 0x01));
	}

	return UcParMod;
}

static void hvca_lensMovePulse(uint16_t position)
{
	uint8_t UcParMod;

	/* Get StepMove Enable Mode */
	UcParMod = STMCHTG_SET | STMSV_SET | STMLFF_SET;
	UcParMod = stmv211(UcParMod, (short)position);

	if (UcParMod & STMVEN_ON)
		UcParMod = stmv211Fin(UcParMod);
}

/* ========================================================================== */
int32_t hvca_actuator_init_focus(
	struct msm_actuator_ctrl_t *a_ctrl,
	uint16_t size, enum msm_actuator_data_type type,
	struct reg_settings_t *settings)
{	
	int32_t rc = 0;

	af_client = &a_ctrl->i2c_client;

	rc = write_E2PDataToHVCA();
	if (rc != 0) {
		CDBG_HVCA("%s: Error Init HVCA", __func__);
		return rc;
	}

	return rc;
}


/*Start : shchang@qti.qualcomm.com - 20130312 */
extern uint16_t inf_dac1, inf_dac2, start_dac, macro_dac1, macro_dac2; 	
/*End : shchang@qti.qualcomm.com - 20130312 */
extern uint16_t cal_offset;//SEMCO Request by Lizk 05112013	
extern bool is_af_final; //SEMCO Request by Lizk 05112013

int32_t hvca_actuator_init_step_table(
	struct msm_actuator_ctrl_t *a_ctrl,
	struct msm_actuator_set_info_t *set_info)
{
	uint16_t i;
	uint16_t code_per_step;
    int16_t  table_entry;
/*Start : shchang@qti.qualcomm.com - 20130312 */
    int16_t start_p, end_p, pan, num_steps, inf_offset, mac_offset, step_size;
/*End : shchang@qti.qualcomm.com - 20130312 */


/*Start : shchang@qti.qualcomm.com - 20130312 */
	position_inf1 = inf_dac1;	/*Infinite Mechanical Stop*/
	position_inf2 = inf_dac2;	/*1.05M*/
	position_mac1 = macro_dac1;	/*Macro Mechanical Stop*/
	position_mac2 = macro_dac2;	/*10cm*/
	position_pan = start_dac;	/*1.2M*/
	inf_offset = 63; //44
	mac_offset = 230;
	step_size = 7;
/*End : shchang@qti.qualcomm.com - 20130312 */


	CDBG_HVCA("[shchang:AF] position_inf1 = %d, position_inf2 = %d, position_mac1 = %d, position_mac2 = %d, position_pan = %d\n", 
				position_inf1, position_inf2, position_mac1, position_mac2, position_pan);


    code_per_step = a_ctrl->region_params[0].code_per_step;

	CDBG_HVCA("%s: total_steps[%d] code_per_step[%d] ", __func__,
		set_info->af_tuning_params.total_steps, code_per_step);

	kfree(a_ctrl->step_position_table);
	a_ctrl->step_position_table = NULL;

	a_ctrl->step_position_table =
		kmalloc(sizeof(uint16_t) * (set_info->af_tuning_params.total_steps + 1),
			GFP_KERNEL);

	if (a_ctrl->step_position_table == NULL)
		return -EFAULT;

	CDBG_HVCA("[shchang:AF] position_inf1 = %d, position_mac1 = %d, code_per_step = %d\n", 
      position_inf1, position_mac1, a_ctrl->region_params[0].code_per_step);

	/*position_inf1 = 869, position_inf2 = 617, position_mac1 = 110, position_mac2 = 362, position_pan = 625*/

	/*Start : shchang@qti.qualcomm.com - 20130328 */
	if(position_pan + inf_offset > position_inf1)
		start_p = position_inf1;
	else
		start_p = position_pan + inf_offset;

	if(position_mac2 - mac_offset < position_mac1)
		end_p = position_mac1;
	else
		end_p = position_mac2 - mac_offset; //(25micron)
		
	pan = position_pan;
	num_steps = set_info->af_tuning_params.total_steps;
	/*End : shchang@qti.qualcomm.com - 20130311 */

	CDBG_HVCA("[shchang:AF] start_p = %d, end_p = %d, pan = %d, num_steps = %d\n", start_p, end_p, pan, num_steps);
	/*Start : shchang@qti.qualcomm.com - 20130311 */
	a_ctrl->step_position_table[0] = start_p;
	CDBG_HVCA("[shchang:AF] KAF_Table[ 0] = %d\n", a_ctrl->step_position_table[0]);
	for(i = 1; i < num_steps; i++)
	{
    if( i < 17 )
      step_size = 7;
    else if( i < 22 )
      step_size = 8;
    else
      step_size = 9;
    table_entry = start_p - (i*step_size);
    if( table_entry < 0 ) 
      table_entry = 0;
		a_ctrl->step_position_table[i] = table_entry;
		CDBG_HVCA("[shchang:AF] KAF_Table[%2d] = %d\n", i, a_ctrl->step_position_table[i]);
	}

	for(i=0; i< num_steps; i++)
	{
		a_ctrl->step_position_table[i] = ((a_ctrl->step_position_table[i] - 512)<<6) & 0xFFC0;	
	}
	/*End : shchang@qti.qualcomm.com - 20130311 */
	return 0;
}

int32_t hvca_actuator_set_default_focus(
	struct msm_actuator_ctrl_t *a_ctrl,
	struct msm_actuator_move_params_t *move_params)
{
	hvca_lensMovePulse(position_inf1);

	return 0;
}

int32_t hvca_actuator_move_focus(
	struct msm_actuator_ctrl_t *a_ctrl,
	struct msm_actuator_move_params_t *move_params)
{
	int32_t new_step_position_table, offset;
/*Start : shchang@qti.qualcomm.com - 20130312 */

	CDBG_HVCA("hvca_actuator_move_focus : HVCA (move_focus) direction[%2d], num_steps[%2d], index[%2d], Table_Value = %6d\n",
		move_params->sign_dir, move_params->num_steps, move_params->dest_step_pos, a_ctrl->step_position_table[move_params->dest_step_pos]);
/*End : shchang@qti.qualcomm.com - 20130312 */


	new_step_position_table = a_ctrl->step_position_table[move_params->dest_step_pos];

	if(is_af_final)
	{
		CDBG_HVCA("hvca_actuator_move_focus  : cal offset before limiter(%2x) = %2x", OFFSET_LIMIT, cal_offset);

		if((cal_offset & 0xFF) >= OFFSET_LIMIT)
		{
			cal_offset = OFFSET_LIMIT | (cal_offset & 0x100);
		}
		CDBG_HVCA("hvca_actuator_move_focus  : cal offset after limiter(%2x) = %2x", OFFSET_LIMIT, cal_offset);

		if(cal_offset >= 0x100)
		{
			offset = (cal_offset - 0x100) * (-1);
		}
		else
		{
			offset = cal_offset;
		}

		new_step_position_table = (new_step_position_table >> 6);
    CDBG_HVCA("hvca_actuator_move_focus  : final lens position(%d) DAC code before cal offset = %2x", 55 - move_params->dest_step_pos, new_step_position_table);
		new_step_position_table += offset;
		CDBG_HVCA("hvca_actuator_move_focus  : cal offset = %2d ", offset);
		CDBG_HVCA("hvca_actuator_move_focus  : final lens position(%d) DAC code after cal offset = %2x\n", 55 - move_params->dest_step_pos, new_step_position_table);

		if(new_step_position_table < 0x0000)
			new_step_position_table = 0x0000;
		else if(new_step_position_table > 0xFFFF)
			new_step_position_table = 0xFFFF;
		
		new_step_position_table = (new_step_position_table<<6) & 0xFFC0;	
		
		is_af_final = false;
	}
	hvca_lensMovePulse(new_step_position_table);
	//hvca_lensMovePulse(a_ctrl->step_position_table[move_params->dest_step_pos]);
/* End - Lizk: SEMCO Request */

	return 0;
}
