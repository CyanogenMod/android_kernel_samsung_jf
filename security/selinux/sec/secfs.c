/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 */
/*
 * Security Module.
 *
 * Author: 
 *
 * Copyright (C) 2012 FujiSoft incorporated
 */

#include <linux/security.h>
#include <linux/uaccess.h>
#include <linux/fs.h>

#include "secfunc.h"
#include "secpolicy.h"
#include "secpolicyif.h"
#include "seclog.h"

/* file key */
#define SECFS_KEY_CONTROL				0
#define SECFS_KEY_POLICY_FILEACCESSGUARD		1
#define SECFS_KEY_POLICY_PROCESSACCESSGUARD		2

/**
 * Security file directory
 */
static struct dentry *secfsdir = NULL;

/**
 * struct secfs_entry_file - security file create parameters
 * @mKey: key
 * @mName: file name
 * @mMode: file mode
 * @mOperations: file operations
 *
 * Used to create security file.
 */
struct secfs_entry_file {
	int	mKey;		/* unique key (not use) */
	bool	mExist;		/* file exists */
	char	*mName;		/* file name */
	mode_t	mMode;		/* file permission */
	const struct file_operations mOperations;
				/* fops */
};
static int secfs_count_using_files(void);
static struct secfs_entry_file g_entry_files[];


/**
 * secfs_ctl_write - control file write
 * @file: file pointer
 * @buf: write data pointer
 * @count: write data size
 * @ppos: loff_t pointer
 * 
 * This function is control file write.
 * SECFS_FILENAME_CONTROL
 *
 * return infomation
 * ssize_t:write success size
 */
static ssize_t secfs_ctl_write(
			struct file *file,
			const char __user *buf,
			size_t count,
			loff_t *ppos)
{
	bool bret = false;
	char *pData;
    int idx;
	
	idx = secfunc_lock();
	
	pData = (char *)secfunc_malloc(count);
	if (pData != NULL) {
		if (copy_from_user(pData, buf, count) == 0)
			bret = seccore_exectldata(pData, count);
		
		secfunc_free(pData);
	}
	
	secfunc_unlock(idx);
	
	return count;
}

/**
 * secfs_ctl_release - control file close
 * @inode: node pointer
 * @file: file pointer
 * 
 * This function is control file close.
 * SECFS_FILENAME_CONTROL
 *
 * return infomation
 * int:result
 */
static int secfs_ctl_release(
			struct inode *inode,
			struct file *file)
{
	int result = 0;
	int type;
    int idx;
	
	idx = secfunc_lock();
	
	type = secpolicy_getctlfsremove();
	if ((type & CONTROL_FILEREMOVE_TYPE_CONTROL) != 0) {
		g_entry_files[SECFS_KEY_CONTROL].mExist = false;
		securityfs_remove(file->f_dentry);
		type &= ~CONTROL_FILEREMOVE_TYPE_CONTROL;
		secpolicy_setctlfsremove(type);
		if (secfs_count_using_files() == 0) {
			securityfs_remove(secfsdir);
			secfsdir = NULL;
		}
	}
	
	secfunc_unlock(idx);
	
	return result;
}

/**
 * secfs_policy_fileguard_write - file access guard file write
 * @file: file pointer
 * @buf: write data pointer
 * @count: write data size
 * @ppos: loff_t pointer
 * 
 * This function is file access guard file write.
 * SECFS_FILENAME_POLICY_FILEACCESSGUARD
 *
 * return infomation
 * ssize_t:write success size
 */
static ssize_t secfs_policy_fileguard_write(
			struct file *file,
			const char __user *buf,
			size_t count,
			loff_t *ppos)
{
	bool bret = false;
	char *pData;
    int idx;
	
	idx = secfunc_lock();
	
	pData = (char *)secfunc_malloc(count);
	if (pData != NULL) {
		if (copy_from_user(pData, buf, count) == 0)
			bret = seccore_exefileguarddata(pData, count);
		
		secfunc_free(pData);
	}
	
	secfunc_unlock(idx);
	
	return count;
}

/**
 * secfs_ctl_release - file access guard file close
 * @inode: node pointer
 * @file: file pointer
 * 
 * This function is file access guard file close.
 * SECFS_FILENAME_POLICY_FILEACCESSGUARD
 *
 * return infomation
 * int:result
 */
static int secfs_policy_fileguard_release(
			struct inode *inode,
			struct file *file)
{
	int result = 0;
	int type;
    int idx;
	
	idx = secfunc_lock();
	
	type = secpolicy_getctlfsremove();
	if ((type & CONTROL_FILEREMOVE_TYPE_FILE) != 0) {
		g_entry_files[SECFS_KEY_POLICY_FILEACCESSGUARD].mExist
								= false;
		securityfs_remove(file->f_dentry);
		type &= ~CONTROL_FILEREMOVE_TYPE_FILE;
		secpolicy_setctlfsremove(type);
		if (secfs_count_using_files() == 0) {
			securityfs_remove(secfsdir);
			secfsdir = NULL;
		}
	}
	
	secfunc_unlock(idx);
	
	return result;
}

/**
 * secfs_policy_procguard_write - process access guard file write
 * @file: file pointer
 * @buf: write data pointer
 * @count: write data size
 * @ppos: loff_t pointer
 * 
 * This function is process access guard file write.
 * SECFS_FILENAME_POLICY_PROCESSACCESSGUARD
 *
 * return infomation
 * ssize_t:write success size
 */
static ssize_t secfs_policy_procguard_write(
			struct file *file,
			const char __user *buf,
			size_t count,
			loff_t *ppos)
{
	bool bret = false;
	char *pData;
    int idx;
	
	idx = secfunc_lock();
	
	pData = (char *)secfunc_malloc(count);
	if (pData != NULL) {
		if (copy_from_user(pData, buf, count) == 0)
			bret = seccore_exeprocguarddata(pData, count);
		
		secfunc_free(pData);
	}
	
	secfunc_unlock(idx);
	
	return count;
}

/**
 * secfs_policy_procguard_release - process access guard file close
 * @inode: node pointer
 * @file: file pointer
 * 
 * This function is process access guard file close.
 * SECFS_FILENAME_POLICY_PROCESSACCESSGUARD
 *
 * return infomation
 * int:result
 */
static int secfs_policy_procguard_release(
			struct inode *inode,
			struct file *file)
{
	int result = 0;
	int type;
    int idx;
	
	idx = secfunc_lock();
	
	type = secpolicy_getctlfsremove();
	if ((type & CONTROL_FILEREMOVE_TYPE_PROCESS) != 0) {
		g_entry_files[SECFS_KEY_POLICY_PROCESSACCESSGUARD].mExist
								= false;
		securityfs_remove(file->f_dentry);
		type &= ~CONTROL_FILEREMOVE_TYPE_PROCESS;
		secpolicy_setctlfsremove(type);
		if (secfs_count_using_files() == 0) {
			securityfs_remove(secfsdir);
			secfsdir = NULL;
		}
	}
	
	secfunc_unlock(idx);
	
	return result;
}

/**
 * secfs_create_entry - create security file
 * @parent: struct dentry pointer
 * @pEnrtyFile: entry file pointer
 * 
 * This function is to create security file.
 *
 */
static struct dentry* secfs_create_entry(
			struct dentry *parent,
			struct secfs_entry_file *pEnrtyFile)
{
	struct dentry *ret;
	
	/* create file */
	ret = securityfs_create_file(pEnrtyFile->mName,
				pEnrtyFile->mMode,
				parent,
				(void *)pEnrtyFile->mKey,
				&pEnrtyFile->mOperations);
	
	return ret;
}


/* file permission */
#define SECFS_PERMISSION_CONTROL			0200
#define SECFS_PERMISSION_POLICY_FILEACCESSGUARD		0200
#define SECFS_PERMISSION_POLICY_PROCESSACCESSGUARD	0200

/*==========================================================
 * Security File initialize
  ==========================================================*/
/**
 * seculityfs file list
 */
static struct secfs_entry_file g_entry_files[] =
{
	/* [0] : control */
	[SECFS_KEY_CONTROL] = {
		SECFS_KEY_CONTROL,
		false,
		SECFS_FILENAME_CONTROL,
		SECFS_PERMISSION_CONTROL,
		{
			/* close */
			.release = secfs_ctl_release,
			/* write */
			.write   = secfs_ctl_write,
		}
	} ,
	/* [1] : file access guard */
	[SECFS_KEY_POLICY_FILEACCESSGUARD] = {
		SECFS_KEY_POLICY_FILEACCESSGUARD,
		false,
		SECFS_FILENAME_POLICY_FILEACCESSGUARD,
		SECFS_PERMISSION_POLICY_FILEACCESSGUARD,
		{
			/* close */
			.release = secfs_policy_fileguard_release,
			/* write */
			.write   = secfs_policy_fileguard_write,
		}
	} ,
	/* [2] : process access guard */
	[SECFS_KEY_POLICY_PROCESSACCESSGUARD] = {
		SECFS_KEY_POLICY_PROCESSACCESSGUARD,
		false,
		SECFS_FILENAME_POLICY_PROCESSACCESSGUARD,
		SECFS_PERMISSION_POLICY_PROCESSACCESSGUARD,
		{
			/* close */
			.release = secfs_policy_procguard_release,
			/* write */
			.write   = secfs_policy_procguard_write,
		}
	}
};

/**
 * secfs_count_using_files - count exist file num
 * 
 * return exist file num
 */
static int secfs_count_using_files(void)
{
	int ret = 0;
	int i;
	struct secfs_entry_file *pef;

	pef = g_entry_files;
	for (i = 0; i < ARRAY_SIZE(g_entry_files); i++, pef++) {
		if (pef->mExist)
			ret++;
	}
	return ret;
}

/**
 * secfs_Init - security file initialize
 * 
 * This function is security file initialize.
 *
 * return infomation
 * int:result
 */
static int __init secfs_Init(void)
{
	int i;
	int result = 0;
	struct secfs_entry_file *pef;

	secfunc_init();
	
	/* create directory */
	secfsdir = securityfs_create_dir(SECFS_FOLDERNAME, NULL);
	if (IS_ERR(secfsdir)) {
		result = PTR_ERR(secfsdir);
	} else {
		struct dentry* pEntry[ARRAY_SIZE(g_entry_files)];
		pef = g_entry_files;
		for (i = 0; i < ARRAY_SIZE(g_entry_files); i++, pef++) {
			pEntry[i] = secfs_create_entry(secfsdir, pef);
			if (IS_ERR(pEntry[i])) {
				result = PTR_ERR(pEntry[i]);
				for (--i; i >= 0; i--) {
					securityfs_remove(pEntry[i]);
				}
				securityfs_remove(secfsdir);
				secfsdir = NULL;
				SECERROR("securityfs_remove");
				break;
			} else {
				pef->mExist = true;
			}
		}
	}
	
	return result;
}

fs_initcall(secfs_Init);
