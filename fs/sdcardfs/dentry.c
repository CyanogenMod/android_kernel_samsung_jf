/*
 * Copyright (c) 1998-2011 Erez Zadok
 * Copyright (c) 2009	   Shrikar Archak
 * Copyright (c) 2003-2011 Stony Brook University
 * Copyright (c) 2003-2011 The Research Foundation of SUNY
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include "sdcardfs.h"
#include "linux/ctype.h"

/*
 * returns: -ERRNO if error (returned to user)
 *          0: tell VFS to invalidate dentry
 *          1: dentry is valid
 */
static int sdcardfs_d_revalidate(struct dentry *dentry, struct nameidata *nd)
{
	struct path lower_path, saved_path;
	struct dentry *lower_dentry;
	int err = 1;

	if (nd && nd->flags & LOOKUP_RCU)
		return -ECHILD;

	sdcardfs_get_lower_path(dentry, &lower_path);
	lower_dentry = lower_path.dentry;
	if (!lower_dentry->d_op || !lower_dentry->d_op->d_revalidate)
		goto out;
	pathcpy(&saved_path, &nd->path);
	pathcpy(&nd->path, &lower_path);
	err = lower_dentry->d_op->d_revalidate(lower_dentry, nd);
	pathcpy(&nd->path, &saved_path);
out:
	sdcardfs_put_lower_path(dentry, &lower_path);
	return err;
}

static void sdcardfs_d_release(struct dentry *dentry)
{
	/* release and reset the lower paths */
	sdcardfs_put_reset_lower_path(dentry);
	free_dentry_private_data(dentry);
	return;
}

#ifdef SDCARDFS_CASE_INSENSITIVE_MATCH_SUPPORT
static int sdcardfs_hash_ci(const struct dentry *dentry, 
				const struct inode *inode, struct qstr *qstr)
{
	/* 
	 * This function is copy of vfat_hashi.
	 * FIXME Should we support national language?
	 *       Refer to vfat_hashi()
	 * struct nls_table *t = MSDOS_SB(dentry->d_sb)->nls_io;
	 */
	const unsigned char *name;
	unsigned int len;
	unsigned long hash;

	name = qstr->name;
	//len = vfat_striptail_len(qstr);
	len = qstr->len; 

	hash = init_name_hash();
	while (len--)
		//hash = partial_name_hash(nls_tolower(t, *name++), hash);
		hash = partial_name_hash(tolower(*name++), hash);
	qstr->hash = end_name_hash(hash);

	return 0;
}

/*
 * Case insensitive compare of two vfat names.
 */
static int sdcardfs_cmp_ci(const struct dentry *parent, 
		const struct inode *pinode,
		const struct dentry *dentry, const struct inode *inode,
		unsigned int len, const char *str, const struct qstr *name)
{
	/* This function is copy of vfat_cmpi */
	// FIXME Should we support national language? 
	//struct nls_table *t = MSDOS_SB(parent->d_sb)->nls_io;
	//unsigned int alen, blen;

	/* A filename cannot end in '.' or we treat it like it has none */
	/*
	alen = vfat_striptail_len(name);
	blen = __vfat_striptail_len(len, str);
	if (alen == blen) {
		if (nls_strnicmp(t, name->name, str, alen) == 0)
			return 0;
	}
	*/
	if (name->len == len) {
		if (strncasecmp(name->name, str, len) == 0)
			return 0; 
	}
	return 1;
}

const struct dentry_operations sdcardfs_dops = {
	.d_revalidate	= sdcardfs_d_revalidate,
	.d_release	= sdcardfs_d_release,
	.d_hash 	= sdcardfs_hash_ci, 
	.d_compare	= sdcardfs_cmp_ci,
};

#else /* SDCARDFS_CASE_INSENSITIVE_MATCH_SUPPORT */
const struct dentry_operations sdcardfs_dops = {
	.d_revalidate	= sdcardfs_d_revalidate,
	.d_release	= sdcardfs_d_release,
};

#endif /* SDCARDFS_CASE_INSENSITIVE_MATCH_SUPPORT */
