/*
*
* drivers/media/tdmb/tdmb_ebi.c
*
* tdmb driver
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

#include <linux/io.h>
#include "tdmb.h"

void __iomem *v_addr_ebi_cs_base;

int tdmb_init_bus(unsigned long addr, int size)
{
	v_addr_ebi_cs_base = ioremap(addr, size);
	return 0;
}

void tdmb_exit_bus(void)
{
	v_addr_ebi_cs_base = NULL;
}
