/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
/*
 * Security Module.
 *
 * Author: 
 *
 * Copyright (C) 2012 FujiSoft incorporated
 */


#ifndef _SEC_SECLOG_H_
#define _SEC_SECLOG_H_


#define SECERROR(fmt, args...)					\
		printk(KERN_INFO "[f]" fmt "\n", ## args)

#endif /* _SEC_SECLOG_H_ */
