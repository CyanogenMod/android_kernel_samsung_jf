/* Copyright (c) 2010-2013, 2015, The Linux Foundation. All rights reserved.
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

#ifndef _VCD_DDL_UTILS_H_
#define _VCD_DDL_UTILS_H_

#include <linux/delay.h>
#include <media/msm/vidc_type.h>

extern u32 vidc_msg_pmem;
extern u32 vidc_msg_timing;

extern u32 vidc_debug_level;
extern u32 vidc_debug_out;

enum timing_data {
	DEC_OP_TIME,
	DEC_IP_TIME,
	ENC_OP_TIME,
	ENC_SLICE_OP_TIME,
	MAX_TIME_DATA
};

#define DBG_PMEM(x...) \
do { \
	if (vidc_msg_pmem) \
		printk(KERN_DEBUG x); \
} while (0)

/* Enable dynamic debug logging method */
enum vidc_msg_level {
	PRIO_FATAL = 0x1,
	PRIO_ERROR = PRIO_FATAL,
	PRIO_HIGH = 0x2,
	PRIO_MED = 0x4,
	PRIO_LOW = 0x8
};

enum vidc_msg_out {
	VIDC_OUT_PRINTK = 1,
	VIDC_OUT_FTRACE
};

#define debug_msg_printk(__msg...) \
do { \
	if (vidc_debug_out == VIDC_OUT_PRINTK) \
		printk(KERN_INFO __msg); \
	else if (vidc_debug_out == VIDC_OUT_FTRACE) \
		trace_printk(KERN_DEBUG __msg); \
} while (0)

#define DDL_MSG_LOW(x...) \
do { \
	if (vidc_debug_level & PRIO_LOW) \
		debug_msg_printk(x); \
} while (0)

#define DDL_MSG_MED(x...) \
do { \
	if (vidc_debug_level & PRIO_MED) \
		debug_msg_printk(x); \
} while (0)

#define DDL_MSG_HIGH(x...) \
do { \
	if (vidc_debug_level & PRIO_HIGH) \
		debug_msg_printk(x); \
} while (0)

#define DDL_MSG_ERROR(x...) \
do { \
	if (vidc_debug_level & PRIO_ERROR) \
		debug_msg_printk("\n <ERROR>: " x); \
} while (0)

#define DDL_MSG_FATAL(x...) \
do { \
	if (vidc_debug_level & PRIO_FATAL) \
		debug_msg_printk("\n <FATAL>: " x); \
} while (0)

#define DDL_ALIGN_SIZE(sz, guard_bytes, align_mask) \
	(((u32)(sz) + guard_bytes) & align_mask)
#define DDL_ADDR_IS_ALIGNED(addr, align_bytes) \
	(!((u32)(addr) & ((align_bytes) - 1)))
#define  DDL_ALIGN(val, grid) ((!(grid)) ? (val) : \
		((((val) + (grid) - 1) / (grid)) * (grid)))
#define  DDL_ALIGN_FLOOR(val, grid) ((!(grid)) ? (val) : \
		(((val) / (grid)) * (grid)))
#define DDL_OFFSET(base, addr) ((!(addr)) ? 0 : (u32)((u8 *) \
		(addr) - (u8 *) (base)))
#define DDL_ADDR_OFFSET(base, addr) DDL_OFFSET((base).align_physical_addr, \
		(addr).align_physical_addr)
#define DDL_GET_ALIGNED_VITUAL(x)   ((x).align_virtual_addr)
#define DDL_KILO_BYTE(x)   ((x)*1024)
#define DDL_MEGA_BYTE(x)   ((x)*1024*1024)
#define DDL_FRAMERATE_SCALE_FACTOR      (1000)
#define DDL_FRAMERATE_SCALE(x)          ((x) * DDL_FRAMERATE_SCALE_FACTOR)

#define DDL_MIN(x, y)  ((x < y) ? x : y)
#define DDL_MAX(x, y)  ((x > y) ? x : y)
#define DDL_MEMCPY(dest, src, len)  memcpy((dest), (src), (len))
#define DDL_MEMSET(src, value, len) memset((src), (value), (len))

void ddl_set_core_start_time(const char *func_name, u32 index);
void ddl_reset_core_time_variables(u32 index);
void ddl_calc_core_proc_time_cnt(const char *func_name, u32 index, u32 count);
void ddl_update_core_start_time(const char *func_name, u32 index);
int ddl_get_core_decode_proc_time(u32 *ddl_handle);
void ddl_reset_avg_dec_time(u32 *ddl_handle);
#endif
