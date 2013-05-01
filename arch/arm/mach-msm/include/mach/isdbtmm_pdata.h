/*
*
* isdbtmm driver
*
* Copyright (C) (2011, Samsung Electronics)
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation version 2.
*
* This program is distributed "as is" WITHOUT ANY WARRANTY of any
* kind, whether express or implied; without even the implied warranty
* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
*/

#ifndef _ISDBTMM_PDATA_H_
#define _ISDBTMM_PDATA_H_

#if defined(CONFIG_TMM_CHG_CTRL)
struct isdbtmm_platform_data {
	void (*tmm_reduce_chg_curr)(void);
	void (*tmm_recover_chg_curr)(void);
};
#endif
#endif
