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

#include <linux/kernel.h>
#include <linux/security.h>
#include <linux/dcache.h>
#include <linux/string.h>
#include <linux/mount.h>
#include <linux/namei.h>
#include <linux/fs_struct.h>
#include <linux/sched.h>
#include <linux/ptrace.h>
#include <linux/limits.h>
#include <linux/fs.h>
#include <linux/binfmts.h>

#include "seccore.h"
#include "secfunc.h"
#include "secpolicy.h"
#include "seclog.h"
#include "seccore.h"

/*=============================================================
 * add_slash()
 *   if end of string is not "/" then add "/"
 *=============================================================*/
static void add_slash(char *pDir)
{
	int nDirLen;
	nDirLen = secfunc_strlen(pDir);
	if (pDir[nDirLen - 1] != '/') {
		pDir[nDirLen + 0] = '/';
		pDir[nDirLen + 1] = '\0';
	}
}

/*=============================================================
 * get_full_path()
 *   get full path string from struct path
 *=============================================================*/
static char *get_full_path(const struct path *pPath)
{
	static char realpath0[PATH_MAX + 1];
	char *pFullPathName;
	
	pFullPathName = d_path(pPath, realpath0, PATH_MAX);
	
	return pFullPathName;
}

/*=============================================================
 * get_full_path_from_dent()
 *   get full path string from struct dentry and struct vfsmount
 *=============================================================*/
static char *get_full_path_from_dent(
			struct dentry *pDentry,
			struct vfsmount *pMnt)
{
	const struct path tmppath = { pMnt, pDentry };
	return get_full_path(&tmppath);
}

/*=============================================================
 * search_path_prefix()
 *   
 *=============================================================*/
static bool search_path_prefix(
			char *path_name)
{
	bool ret = false;
	/* 1. prefix search */
	if (seccore_fileguard(path_name, true) == true) {
		/* accept : not match by prefix search */
		ret = true;
	} else {
		/* 2. exact matching */
		if (seccore_fileguard(path_name, false) == false) {
			ret = false;
		} else {
			/* add "/" */
			add_slash(path_name);
			
			/* 3. prefix search */
			ret = seccore_fileguard(path_name, true);
		}
	}
	return ret;
}


/*=============================================================
 * get_full_path_current()
 *   get full path string from relative path
 *=============================================================*/
static char *get_full_path_relative(const char *relative_pathname)
{
	char *pFullPathName = NULL;
	int err;
	struct path tmp_path;
	
	err = kern_path(relative_pathname, LOOKUP_FOLLOW, &tmp_path);
	if (!err) {
		pFullPathName = get_full_path(&tmp_path);
		path_put(&tmp_path);
	}
	return pFullPathName;
}


/*=============================================================
 *ptrace Hooks
  =============================================================*/
static int seclsm_ptrace_may_access(
			struct task_struct *child,
			unsigned int mode)
{
	int sec_result = 0;
	struct ProcessInfo target;
    int idx;
    
	idx = secfunc_lock();

#ifndef CONFIG_SECURITY_SELINUX
#if defined(CONFIG_ARCH_GOLDFISH)
	sec_result = cap_ptrace_may_access(child, mode);
#else    /* CONFIG_ARCH_GOLDFISH */
	sec_result = cap_ptrace_access_check(child, mode);
#endif   /* CONFIG_ARCH_GOLDFISH */
#endif
	if (sec_result == 0 && PTRACE_MODE_ATTACH == mode && child->mm != NULL) {
		if (secfunc_getprocinfo_locked(&target, child, true)) {
			if (seccore_procguard(&target) == false) {
				sec_result = -EPERM;
			}
			secfunc_free(target.mpCmdLine);
		} else {
			SECERROR("ptrace Error getprocinfo");
		}
	}
	
	secfunc_unlock(idx);
	
	
	return sec_result;
}

/*=============================================================
 * seclsm_dentry_open()
 *   check the file open
 *=============================================================*/
static int seclsm_dentry_open(struct file *file, const struct cred *cred)
{
	int sec_result = 0;
	char *pFilePathName;
    int idx;
	
	idx = secfunc_lock();
    
	pFilePathName = get_full_path(&file->f_path);
	if (!seccore_fileguard(pFilePathName, false)) {
		sec_result = -EPERM;
	}
    
	secfunc_unlock(idx);
	
	return sec_result;
}

/*=============================================================
 * seclsm_path_rename()
 *   check the file moving
 *=============================================================*/
static int seclsm_path_rename(
			struct path *old_path,
			struct dentry *old_dentry,
			struct path *new_path,
			struct dentry *new_dentry)
{
	int sec_result = 0;
	char *pFilePathName;
    int idx;
	
	idx = secfunc_lock();
    
	pFilePathName = get_full_path_from_dent(old_dentry, old_path->mnt);
	
	if (!search_path_prefix(pFilePathName)) {
		sec_result = -EPERM;
	} else {
		pFilePathName = get_full_path_from_dent(
					new_dentry, new_path->mnt);
		if (!search_path_prefix(pFilePathName)) {
			sec_result = -EPERM;
		}
	}
	
	secfunc_unlock(idx);
	
	return sec_result;
}

/*=============================================================
 * seclsm_path_link()
 *   check the file hard link
 *=============================================================*/
static int seclsm_path_link(
			struct dentry *old_dentry,
			struct path *new_dir,
			struct dentry *new_dentry)
{
	int sec_result = 0;
	char *pFilePathName;
    int idx;
    
	idx = secfunc_lock();
    
	pFilePathName = get_full_path_from_dent(old_dentry, new_dir->mnt);

	if (!search_path_prefix(pFilePathName)) {
		sec_result = -EPERM;
	} else {
		pFilePathName = get_full_path_from_dent(
					new_dentry, new_dir->mnt);
		if (!search_path_prefix(pFilePathName)) {
			sec_result = -EPERM;
		}
	}
	
	secfunc_unlock(idx);
	
	return sec_result;
}

/*=============================================================
 * seclsm_path_symlink()
 *   check the file symbolic link
 *=============================================================*/
static int seclsm_path_symlink(
			struct path *path,
			struct dentry *dentry,
			const char *old_name)
{
	int sec_result = 0;
	char *pNewPathName;
    int idx;
    
	idx = secfunc_lock();
    
	pNewPathName = get_full_path_from_dent(dentry, path->mnt);
	if (!search_path_prefix(pNewPathName)) {
		sec_result = -EPERM;
	}
	
	secfunc_unlock(idx);
	
	return sec_result;
}

/*=============================================================
 * seclsm_sb_mount()
 *   check the file mount
 *=============================================================*/
static int seclsm_sb_mount(
			char *dev_name,
			struct path *path,
			char *type,
			unsigned long flags,
			void *data)
{
	int sec_result = 0;
	char *pFilePathName;
    int idx;
    
	idx = secfunc_lock();
    
	pFilePathName = get_full_path(path);

	if (!search_path_prefix(pFilePathName)) {
		sec_result = -EPERM;
	} else {
		if (flags & (MS_BIND | MS_MOVE)) { /* MS_BIND, MS_MOVE */
			pFilePathName = get_full_path_relative(dev_name);
			if (pFilePathName) {
				if (!search_path_prefix(pFilePathName)) {
					sec_result = -EPERM;
				}
			}
		}
	}
    
	secfunc_unlock(idx);
	
	return sec_result;
}

#if !defined(CONFIG_ARCH_GOLDFISH)
/*=============================================================
 * seclsm_path_chroot()
 *   check the root directory change
 *=============================================================*/
static int seclsm_path_chroot(
			struct path *path)
{
	int sec_result = 0;
	char *pFilePathName;
    int idx;
    
	idx = secfunc_lock();
    
	pFilePathName = get_full_path(path);
	
	if (!search_path_prefix(pFilePathName)) {
		sec_result = -EPERM;
	}
	
	secfunc_unlock(idx);
	
	return sec_result;
}
#endif   /* CONFIG_ARCH_GOLDFISH */

/*=============================================================
 * seclsm_bprm_check_security()
 *   check execute process
 *=============================================================*/
static int seclsm_bprm_check_security(struct linux_binprm *bprm)
{
	int sec_result = 0;
	struct task_struct *curtask;
	struct ProcessInfo info;
    int idx;
    
	idx = secfunc_lock();
    
	curtask = current;
	
	if ((curtask->ptrace & PT_PTRACED) != 0) {
		/* traced process execute. */
		secfunc_memset(&info, 0, sizeof(info));
		info.mPid = curtask->pid;
		info.mUid = curtask->cred->uid;
		info.mGid = curtask->cred->gid;
		/* cmdline */
		info.mpCmdLine = get_full_path(&bprm->file->f_path);
		info.mCmdLength = secfunc_strlen(info.mpCmdLine);
		
		if (seccore_procguard(&info) == false) {
			sec_result = -EPERM;
		}
	}
    
	secfunc_unlock(idx);
	
	return sec_result;
}

/*=============================================================
 * LSM Initialize
  =============================================================*/
struct security_operations seclsm_Operations = {

#if defined(CONFIG_ARCH_GOLDFISH)
    .ptrace_may_access   = seclsm_ptrace_may_access,
#else    /* CONFIG_ARCH_GOLDFISH */
    .ptrace_access_check = seclsm_ptrace_may_access,
#endif   /* CONFIG_ARCH_GOLDFISH */

    .dentry_open         = seclsm_dentry_open,
    .path_rename         = seclsm_path_rename,
    .path_link           = seclsm_path_link,
    .path_symlink        = seclsm_path_symlink,
    .sb_mount            = seclsm_sb_mount,

#if !defined(CONFIG_ARCH_GOLDFISH)
    .path_chroot         = seclsm_path_chroot,
#endif   /* !CONFIG_ARCH_GOLDFISH */

    .bprm_check_security = seclsm_bprm_check_security,
};

void seclsm_init(void)
{
//    secfunc_init();
}
