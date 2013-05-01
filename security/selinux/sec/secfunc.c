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
#include <linux/path.h>
#include <linux/mount.h>
#include <linux/cred.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <linux/file.h>
#include "seccore.h"
#include "secfunc.h"
#include "secpolicy.h"
#include "seclog.h"

static struct srcu_struct seclsm_ss;

/**
 * secfunc_init - porting initialize.
 * 
 * This function is porting initialize.
 *
 */
void secfunc_init(void)
{
    init_srcu_struct(&seclsm_ss);
}

/**
 * secfunc_getcallprocinfo - get caller parent infomation.
 * @pInfo: process infomation pointer
 * @count: current parent data back count
 *         0:current caller infomation
 *         1:current caller parent call iinfomation.
 * 
 * This function is to get caller infomation.
 *
 * return infomation
 * true:get infomation success
 * false:get infomation failed
 */
bool secfunc_getcallprocinfo(struct ProcessInfo *pInfo, int count)
{
	bool result = false;
	int i = 0;
	struct task_struct *task = current;
	
	for (i = 0; i < count; i++) {
		if (task->parent == NULL)
			break;
		if (task == task->parent)
			break;
		task = task->parent;
	}
	
	if (i == count) {
		if (secfunc_getprocinfo(pInfo, task)) {
			result = true;
		}
	} else {
		result = false;
		SECERROR("count=%d Error", count);
	}
	
	return result;
	
}

/**
 * secfunc_get_full_path - get full path.
 * @ppath: Path struct pointer.
 * 
 * This function is to get full path.
 *
 */
static char *secfunc_get_full_path(const struct path *ppath)
{
	static char realpath0[PATH_MAX + 1];
	char *pfullpathname;
	
	pfullpathname = d_path(ppath, realpath0, PATH_MAX);
	
	return pfullpathname;
}

/**
 * secfunc_get_cmdline - get cmdline.
 * @cmdline: command line pointer.max size PAGE_SIZE
 * @task: target task struct
 * 
 * This function is to get command line data.
 *
 */
static char *secfunc_get_cmdline(struct task_struct *task, bool locked)
{
	char *pcmdline = NULL;
	struct mm_struct *mm;
	mm = task->mm;
    
	if (mm != NULL) {
		struct file *exe_file;
		struct path exe_path;
	    
        exe_file = mm->exe_file;
		if (exe_file) {
			int len;
			char* p;
			exe_path = exe_file->f_path;
			p = secfunc_get_full_path(&exe_path);
			len = secfunc_strlen(p);
			pcmdline = secfunc_malloc(len + 1);
			if (pcmdline != NULL) {
				secfunc_memset(pcmdline, 0, len + 1);
				secfunc_strcpy(pcmdline, p);
			} else {
				SECERROR("Error:get cmdline");
			}
		} else {
			SECERROR("Error:get cmdline");
		}
	}
	return pcmdline;
}

/**
 * secfunc_getprocinfo_locked - get caller infomation.
 * @pInfo: process infomation pointer
 * @task: target task struct
 * @locked: task locked
 * 
 * This function is to get caller infomation.
 *
 */
bool secfunc_getprocinfo_locked(struct ProcessInfo *pInfo,
				struct task_struct *task,
				bool locked)
{
	bool result = false;
	char *pcmdline = secfunc_get_cmdline(task, locked);
	if (pcmdline != NULL) {
		pInfo->mCmdLength = secfunc_strlen(pcmdline);
		pInfo->mpCmdLine = pcmdline;
		pInfo->mPid = task->pid;
		pInfo->mUid = task->cred->uid;
		pInfo->mGid = task->cred->gid;
		result = true;
	}
	
	return result;
}

/**
 * secfunc_getprocinfo - get caller infomation.
 * @pInfo: process infomation pointer
 * @task: target task struct
 * 
 * This function is to get caller infomation.
 *
 */
bool secfunc_getprocinfo(struct ProcessInfo *pInfo, struct task_struct *task)
{
	bool chk = spin_is_locked(&task->alloc_lock);
	if (chk) {
		SECERROR("spin locked pid=%d", task->pid);
	}
	
	return secfunc_getprocinfo_locked(pInfo, task, chk);
}

/*=============================================================
 * lock
  =============================================================*/
/**
 * secfunc_lock - lock.
 * 
 * This function is lock.
 *
 */

int secfunc_lock(void)
{
    int idx;
    
    idx = srcu_read_lock(&seclsm_ss);

    return idx;
}

/**
 * secfunc_unlock - unlock.
 * 
 * This function is unlock.
 *
 */
void secfunc_unlock(int idx)
{
    srcu_read_unlock(&seclsm_ss, idx);
}

/*=============================================================
 * C function library wrapper
  =============================================================*/
int secfunc_strcmp(const char *s1, const char *s2)
{
	return strcmp(s1, s2);
}

int secfunc_strncmp(const char *s1, const char *s2, size_t n)
{
	return strncmp(s1, s2, n);
}

size_t secfunc_strlen(const char *s)
{
	return strlen(s);
}

char *secfunc_strcpy(char *s1, const char *s2)
{
	return strcpy(s1, s2);
}

char *secfunc_strcat(char *str1, const char *str2)
{
	return strcat(str1, str2);
}

void *secfunc_memset(void *buf, int ch, size_t n)
{
	return memset(buf, ch, n);
}

void *secfunc_memcpy(void *buf1, const void *buf2, size_t n)
{
	return memcpy(buf1, buf2, n);
}

void *secfunc_malloc(size_t size)
{
	return kzalloc(size, GFP_ATOMIC);
}

void secfunc_free(const void *objp)
{
	kfree(objp);
}

