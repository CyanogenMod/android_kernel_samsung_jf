/*
 * Integrity check code for crypto module.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */
#include <crypto/hash.h>
#include <crypto/sha.h>
#include <linux/err.h>
#include <linux/scatterlist.h>
#include <asm-generic/sections.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>

#include "internal.h"

static bool need_integrity_check = true;
extern long integrity_mem_reservoir;
//extern void free_bootmem(unsigned long addr, unsigned long size);

void do_integrity_check(void)
{
	u8 *rbuf = 0;
	u32 len;
	u8 hmac[SHA256_DIGEST_SIZE];
	struct hash_desc desc;
	struct scatterlist sg;
	u8 *key = "12345678";
    int i, step_len = PAGE_SIZE, err;
    u8 *pAllocBuf = 0;

    printk(KERN_INFO "FIPS: integrity start\n");
	
	if (unlikely(!need_integrity_check || in_fips_err())) {
        printk(KERN_INFO "FIPS: integrity check not needed\n");
		return;
	}
	rbuf = (u8*)phys_to_virt((unsigned long)CONFIG_CRYPTO_FIPS_INTEG_COPY_ADDRESS);
	
	if (*((u32 *) &rbuf[36]) != 0x016F2818) {
		printk(KERN_ERR "FIPS: invalid zImage magic number.");
		set_in_fips_err();
		goto err1;
	}

	if (*(u32 *) &rbuf[44] <= *(u32 *) &rbuf[40]) {
		printk(KERN_ERR "FIPS: invalid zImage calculated len");
		set_in_fips_err();
		goto err1;
	}

	len = *(u32 *) &rbuf[44] - *(u32 *) &rbuf[40];

    printk(KERN_INFO "FIPS: integrity actual zImageLen = %d\n", len);
    printk(KERN_INFO "FIPS: do kernel integrity check address: %lx \n", (unsigned long)rbuf);
	
	desc.tfm = crypto_alloc_hash("hmac(sha256)", 0, 0);

	if (IS_ERR(desc.tfm)) {
		printk(KERN_ERR "FIPS: integ failed to allocate tfm %ld\n",
		       PTR_ERR(desc.tfm));
		set_in_fips_err();
		goto err1;
	}
#if FIPS_FUNC_TEST == 2
    rbuf[1024] = rbuf[1024] + 1;
#endif
	crypto_hash_setkey(desc.tfm, key, strlen(key));

	pAllocBuf = kmalloc(step_len,GFP_KERNEL);
	if (!pAllocBuf) {
		printk(KERN_INFO "Fail to alloc memory, length %d\n", step_len);
		set_in_fips_err();
		goto err1;
	}

	err = crypto_hash_init(&desc);
	if (err) {
		printk(KERN_INFO "fail at crypto_hash_init\n");		
        set_in_fips_err();
		kfree(pAllocBuf);	
		goto err1;
	}

	for (i = 0; i < len; i += step_len) 	{

		//last is reached
		if (i + step_len >= len - 1)  {
			memcpy(pAllocBuf, &rbuf[i], len - i);
			sg_init_one(&sg, pAllocBuf, len - i);
			err = crypto_hash_update(&desc, &sg, len - i);
			if (err) {
				printk(KERN_INFO "Fail to crypto_hash_update1\n");
                set_in_fips_err();
				goto err;
			}
			err = crypto_hash_final(&desc, hmac);
			if (err) {
				printk(KERN_INFO "Fail to crypto_hash_final\n");
                set_in_fips_err();
				goto err;
			}
		} else {						
		    memcpy(pAllocBuf, &rbuf[i], step_len);
		    sg_init_one(&sg, pAllocBuf, step_len);
		    err = crypto_hash_update(&desc, &sg, step_len); 
	
		    if (err) {
			    printk(KERN_INFO "Fail to crypto_hash_update\n");
                set_in_fips_err();
			    goto err;
		    }
        }
	}
#if FIPS_FUNC_TEST == 2
    rbuf[1024] = rbuf[1024] - 1;
#endif
	if (!strncmp(hmac, &rbuf[len], SHA256_DIGEST_SIZE)) {
		printk(KERN_INFO "FIPS: integrity check passed\n");
	} else {
		printk(KERN_ERR "FIPS: integrity check failed. hmac:%lx, buf:%lx.\n",(long) hmac, (long)rbuf[len] );
		set_in_fips_err();
	}

 err:
	kfree(pAllocBuf);
	crypto_free_hash(desc.tfm);
 err1:
	need_integrity_check = false;
	
/*	if(integrity_mem_reservoir != 0) {
		printk(KERN_NOTICE "FIPS free integrity_mem_reservoir = %ld\n", integrity_mem_reservoir);
		free_bootmem((unsigned long)CONFIG_CRYPTO_FIPS_INTEG_COPY_ADDRESS, integrity_mem_reservoir);
	}
*/	
}

EXPORT_SYMBOL_GPL(do_integrity_check);
