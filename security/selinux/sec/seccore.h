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


#ifndef SECCORE_H_
#define SECCORE_H_

#include <linux/sched.h>
#include "secpolicy.h"

struct ProcessInfo {
	int	mCmdLength;	/* task command */
	char	*mpCmdLine;	/* cmdline */
	pid_t	mPid;		/* PID */
	uid_t	mUid;		/* UID */
	gid_t	mGid;		/* GID */
};

/**
 * seccore_procguard - process access guard
 * @pProcInfo: process access infomation pointer
 * 
 * This function is process access guard.
 *
 * return infomation
 * true:access permision OK
 * false:access permision NG
 */
extern bool seccore_procguard(struct ProcessInfo *pProcInfo);

/**
 * seccore_fileguard - target check for file guard.
 * @pFilePathName: target file full path name pointer
 * @bPreFixChk: prefix check on/off
 * 
 * This function is target check for file access guard.
 *
 * return infomation
 * true:file access permision OK
 * false:file access permision NG
 */
extern bool seccore_fileguard(const char *pFilePathName,
			 bool bPreFixChk);


/**
 * seccore_exectldata - control data execute.
 * @pData: control binary data pointer
 * @size: control binary data size
 * 
 * This function is control data execute.
 *
 * return infomation
 * true: execute success
 * false: execute failed
 */
extern bool seccore_exectldata(const char *pData, size_t size);

/**
 * seccore_exeprocguarddata - process target execute.
 * @pData: process access guard policy data pointer
 * @size: process access guard policy data size
 * 
 * This function is process guard target execute.
 *
 * return infomation
 * true:execute success
 * false:execute failed
 */
extern bool seccore_exeprocguarddata(const char *pData, size_t size);

/**
 * seccore_exefileguarddata - file target execute.
 * @pData: file access guard policy data pointer
 * @size: file access guard policy data size
 * 
 * This function is file file target execute.
 *
 * return infomation
 * true:execute success
 * false:execute failed
 */
extern bool seccore_exefileguarddata(const char *pData, size_t size);

#endif /* SECCORE_H_ */
