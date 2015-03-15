/*
 * Ported to Note 3 (n9005) and extended:
 * Jean-Pierre Rasquin <yank555.lu@gmail.com>
 *
 * Adapted for 9505 from Note 3:
 * Paul Reioux <reioux@gmail.com>
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


#ifndef _LINUX_FASTCHG_H
#define _LINUX_FASTCHG_H

#define FAST_CHARGE_VERSION "v1.0 by Paul Reioux"
#define AC_LEVELS  "1000 1300 1600"
#define USB_LEVELS "460 700 900"
#define ANY_LEVELS "0 to 1600"

extern int force_fast_charge;

#define FAST_CHARGE_DISABLED 0	/* default */
#define FAST_CHARGE_FORCE_AC 1
#define FAST_CHARGE_FORCE_CUSTOM_MA 2

extern int ac_charge_level;

#define AC_CHARGE_1000 1000
#define AC_CHARGE_1300 1300
#define AC_CHARGE_1600 1600

extern int usb_charge_level;

#define USB_CHARGE_460 460	/* default */
#define USB_CHARGE_700 700
#define USB_CHARGE_900 900

/* Whatever happens, this is the limit */
#define MAX_CHARGE_LEVEL 1700

extern int failsafe;
#define FAIL_SAFE_ENABLED 1	/* default */
#define FAIL_SAFE_DISABLED 0

#endif
