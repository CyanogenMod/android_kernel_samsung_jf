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


#ifndef SECFUNC_H_
#define SECFUNC_H_

#include "seccore.h"
#include "secpolicy.h"

/**
 * secfunc_init - porting initialize.
 * 
 * This function is porting initialize.
 *
 */
extern void secfunc_init(void);

/*=============================================================
 * caller process
  =============================================================*/
/* コール元判定マスクパターン */
#define CALLER_CHECKMASK_COMMANDNAME	0x00000001	/* command */
#define CALLER_CHECKMASK_UID		0x00000002	/* UID */
	#define CALLER_CHECKMASK_UID_ONE	0x00000001	/* UID one */
	#define CALLER_CHECKMASK_UID_LESS	0x00000002	/* UID less */
	#define CALLER_CHECKMASK_UID_MORE	0x00000004	/* UID more */
#define CALLER_CHECKMASK_GID		0x00000004		/* GID */
	#define CALLER_CHECKMASK_GID_ONE	0x00000001	/* GID one */
	#define CALLER_CHECKMASK_GID_LESS	0x00000002	/* GID less */
	#define CALLER_CHECKMASK_GID_MORE	0x00000004	/* GID more */
#define CALLER_CHECKMASK_PID		0x00000008	/* PID */
	#define CALLER_CHECKMASK_PID_ONE	0x00000001	/* PID one */
	#define CALLER_CHECKMASK_PID_LESS	0x00000002	/* PID less */
	#define CALLER_CHECKMASK_PID_MORE	0x00000004	/* PID more */
#define CALLER_CHECKMASK_PARENT		0x00000010	/* parent */
#define CALLER_CHECKMASK_ALL_DENIAL	0x00008000	/* all denial */

/* UID data */
struct CallerUid {
	int	mCheckMask;		/* UID mask */
	
	/* CALLER_CHECKMASK_UID_ONE */
	uid_t	mUid;			/* UID */
	
	/* CALLER_CHECKMASK_UID_LESS */
	uid_t	mUid_Less;		/* uid less */
	
	/* CALLER_CHECKMASK_UID_MORE */
	uid_t	mUid_More;		/* uid more */
};

/* GID data */
struct CallerGid {
	int	mCheckMask;		/* GID mask */
	
	/* CALLER_CHECKMASK_GID_ONE */
	gid_t	mGid;			/* GID */
	
	/* CALLER_CHECKMASK_GID_LESS */
	gid_t	mGid_Less;		/* gid less */
	
	/* CALLER_CHECKMASK_GID_MORE */
	gid_t	mGid_More;		/* gid more */
};

/* PID data */
struct CallerPid {
	int	mCheckMask;		/* PID mask */
	
	/* CALLER_CHECKMASK_PID_ONE */
	pid_t	mPid;			/* PID */
	
	/* CALLER_CHECKMASK_PID_LESS */
	pid_t	mPid_Less;		/* pid less */
	
	/* CALLER_CHECKMASK_PID_MORE */
	pid_t	mPid_More;		/* pid more */
};

/* caller data */
struct Caller {
	int	mCheckMask;			/* caller mask pattern */
	
	/* CALLER_CHECKMASK_COMMANDNAME */
	int	mCmdLength;			/* caller command name */
	char*	mpCmdLine;			/* caller command length */
	
	/* CALLER_CHECKMASK_UID */
	struct CallerUid	mCallerUid;	/* caller uid data */
	
	/* CALLER_CHECKMASK_GID */
	struct CallerGid	mCallerGid;	/* caller gid datan */
	
	/* CALLER_CHECKMASK_PID */
	struct CallerPid	mCallerPid;	/* caller pid data */
	
	/* CALLER_CHECKMASK_PARENT */
	struct Caller		*mParent;	/* parent caller pointer */
};

/* callers data */
struct Callers {
	int mMax;				/* caller max */
	struct Caller *mpCaller;		/* caller pointer */
};

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
extern bool secfunc_getcallprocinfo(struct ProcessInfo *pInfo, int count);

/**
 * secfunc_getprocinfo_locked - get caller infomation.
 * @pInfo: process infomation pointer
 * @task: target task struct
 * @locked: task locked
 * 
 * This function is to get caller infomation.
 *
 */
extern bool secfunc_getprocinfo_locked(struct ProcessInfo *pInfo, struct task_struct *task, bool locked);

/**
 * secfunc_getprocinfo - get caller infomation.
 * @pInfo: process infomation pointer
 * @task: target task struct
 * 
 * This function is to get caller infomation.
 *
 */
extern bool secfunc_getprocinfo(struct ProcessInfo *pInfo,
				struct task_struct *task);

/*=============================================================
 * process access permission data
  =============================================================*/
/* access target mask pattern */
#define PROCGDATA_TARGET_CHECKMASK_COMMANDNAME	0x00000001
#define PROCGDATA_TARGET_CHECKMASK_UID		0x00000002

struct ProcGData {
	/* access target condition */
	struct {
		int	mCheckMask;		/* mask */
		int	mCmdLength;		/* command name */
		char*	mpCmdLine;		/* command length */
		uid_t	mUid;			/* UID */
	} mTarget;
	
	/* callers data */
	struct Callers mCallers;
};

/*=============================================================
 * file access permission data
  =============================================================*/
/* access target mask pattern */
#define FILEGDATA_TARGET_CHECKMASK_FULLPATH	0x00000001
#define FILEGDATA_TARGET_CHECKMASK_FOLDERPATH	0x00000002

struct FileGData {
	/* access target condition */
	struct {
		int	mCheckMask;	/* mask */
		char	*mpPath;	/* path */
	} mTarget;
	
	/* callers data */
	struct Callers mCallers;
};

/*=============================================================
 * lock
  =============================================================*/
extern int secfunc_lock(void);
extern void secfunc_unlock(int idx);

/*=============================================================
 * C function library wrapper
  =============================================================*/
extern int secfunc_strcmp(const char *s1, const char *s2);
extern int secfunc_strncmp(const char *s1, const char *s2, size_t n);
extern size_t secfunc_strlen(const char *s);
extern char *secfunc_strcpy(char *s1, const char *s2);
extern char *secfunc_strcat(char *str1, const char *str2);
extern void *secfunc_memset(void *buf, int ch, size_t n);
extern void *secfunc_memcpy(void *buf1, const void *buf2, size_t n);
extern void *secfunc_malloc(size_t size);
extern void secfunc_free(const void *objp);


#endif
