/*
 * msm8930-gpio.h
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

/* MSM8930 GPIO */
#if defined(CONFIG_MACH_MELIUS)
#include <mach/melius-gpio.h>
#elif defined(CONFIG_MACH_EXPRESS2)
#include <mach/express2-gpio.h>
#elif defined(CONFIG_MACH_HIGGS)
#include <mach/higgs-gpio.h>
#elif defined(CONFIG_MACH_CRATER)
#include <mach/crater-gpio.h>
#elif defined(CONFIG_MACH_BAFFIN)
#include <mach/baffin-gpio.h>
#elif defined(CONFIG_MACH_SERRANO)
#include <mach/serrano-gpio.h>
#elif defined(CONFIG_MACH_GOLDEN)
#include <mach/golden-gpio.h>
#elif defined(CONFIG_MACH_JASPER2)
#include <mach/jasper2-gpio.h>
#elif defined(CONFIG_MACH_EXPRESS)
#include <mach/express-gpio.h>
#elif defined(CONFIG_MACH_GODIVA)
#include <mach/godiva-gpio.h>
#elif defined(CONFIG_MACH_STUNNER)
#include <mach/stunner-gpio.h>
#elif defined(CONFIG_MACH_KS02)
#include <mach/ks02-gpio.h>
#elif defined(CONFIG_MACH_LT02_CHN_CTC)
#include <mach/lt02_ctc-gpio.h>
#elif defined(CONFIG_MACH_LT02)
#include <mach/lt02-gpio.h>
#elif defined(CONFIG_MACH_BISCOTTO)
#include <mach/biscotto-gpio.h>
#elif defined(CONFIG_MACH_CANE)
#include <mach/cane-gpio.h>
#endif
