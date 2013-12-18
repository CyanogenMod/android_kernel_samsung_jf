/*  drivers/misc/sec_torch.h
 *
 *  Copyright (C) 2013 Samsung Electronics Co.Ltd
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#ifndef __SEC_TORCH_H__
#define __SEC_TORCH_H__

#define TORCH_FLASH_DEBUG	1

#if	TORCH_FLASH_DEBUG
#define	torch_debug_msg(fmt, args...)	printk(KERN_INFO "[TORCH: %-18s:%5d]" fmt, __FUNCTION__, __LINE__, ## args)
#else
#define	torch_debug_msg(fmt, args...)	do{}while(0)
#endif

extern int Torch_Flash_mode;

extern void (*Torch_Flash_OnOff) (unsigned long);

#endif /* __SEC_TORCH_H__ */
