/*
 * apq8064-gpio.h
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

/* APQ8064 GPIO */
#if defined(CONFIG_MACH_JF_VZW)
#include <mach/jf_vzw-gpio.h>
#elif defined(CONFIG_MACH_JF_SPR)
#include <mach/jf_spr-gpio.h>
#elif defined(CONFIG_MACH_JF_ATT)
#include <mach/jf_att-gpio.h>
#elif defined(CONFIG_MACH_JF_TMO)
#include <mach/jf_tmo-gpio.h>
#elif defined(CONFIG_MACH_JF_USC)
#include <mach/jf_usc-gpio.h>
#elif defined(CONFIG_MACH_JF_EUR)
#include <mach/jf_eur-gpio.h>
#elif defined(CONFIG_MACH_JF_SKT)
#include <mach/jf_skt-gpio.h>
#elif defined(CONFIG_MACH_JF_KTT)
#include <mach/jf_ktt-gpio.h>
#elif defined(CONFIG_MACH_JF_LGT)
#include <mach/jf_lgt-gpio.h>
#elif defined(CONFIG_MACH_JF_DCM)
#include <mach/jf_dcm-gpio.h>
#elif defined(CONFIG_MACH_JACTIVE_EUR)
#include <mach/jactive_eur-gpio.h>
#elif defined(CONFIG_MACH_JACTIVE_ATT)
#include <mach/jactive_att-gpio.h>
#elif defined(CONFIG_MACH_JF_CRI)
#include <mach/jf_cri-gpio.h>
#elif defined(CONFIG_MACH_JFVE_EUR)
#include <mach/jfve_eur-gpio.h>
#endif
