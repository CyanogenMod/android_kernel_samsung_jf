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
#include <linux/uaccess.h>
#include "seccore.h"
#include "secfunc.h"
#include "secpolicy.h"
#include "secpolicyif.h"
#include "seclog.h"

/*=============================================================
 * Control Policy functions
  =============================================================*/
static int gfile_remove_type=0;

/**
 * secpolicy_getctlfsremove - get file remove infomation.
 * 
 * This function is to get remove infomation.
 *
 * return infomation
 * int:remove file infomation
 */
int secpolicy_getctlfsremove(void)
{
	return gfile_remove_type;
}

/**
 * secpolicy_setctlfsremove - set file remove infomation.
 * @type: set remove file infomation
 * 
 * This function is to set control infoamtion.
 *
 */
void secpolicy_setctlfsremove(int type)
{
	gfile_remove_type = type;
}


/*=============================================================
 * process access guard data functions
  =============================================================*/
static struct ProcGData* gpprocg_data_empty[] = {
	NULL
};

static struct ProcGData **gppprocg_data = gpprocg_data_empty;

/**
 * secpolicy_startprocguarddata - call start process access guard data.
 * 
 * This function is to start caller infomation reference.
 *
 * return infomation
 * struct ProcGData**:process access data pointer
 */
struct ProcGData** secpolicy_startprocguarddata(void)
{
	return gppprocg_data;
}

/**
 * secpolicy_stopprocguarddata - call stop process access guard data.
 * 
 * This function is to start caller infomation reference.
 *
 */
void secpolicy_stopprocguarddata(void)
{
}

/**
 * secpolicy_releaseprocguarddata - release process access guard data.
 * @ppUpdateData: process access data pointer
 * 
 * This function is to release process access guard data.
 *
 */
void secpolicy_releaseprocguarddata(struct ProcGData **ppReleaseData)
{
	struct ProcGData **ppData = ppReleaseData;
	
	if ((ppReleaseData != NULL) &&
	   (ppReleaseData != gpprocg_data_empty)) {
		while (*ppData != NULL) {
			int i;
			struct ProcGData *pData = *ppData;
			/* free target data */
			if (pData->mTarget.mpCmdLine != NULL)
				secfunc_free(pData->mTarget.mpCmdLine);
			
			/* free caller data */
			for (i = 0; i < pData->mCallers.mMax; i++) {
				/* free parent data */
				struct Caller *pParentCaller;
				if (pData->mCallers.mpCaller[i].mpCmdLine != NULL)
					secfunc_free(pData->mCallers.mpCaller[i].mpCmdLine);
				pParentCaller = pData->mCallers.mpCaller[i].mParent;
				while (pParentCaller != NULL) {
					struct Caller *pWorkCaller = NULL;
					pWorkCaller = pParentCaller->mParent;
					secfunc_free(pParentCaller);
					pParentCaller = pWorkCaller;
				}
			}
			
			if (i != 0) {
				secfunc_free(pData->mCallers.mpCaller);
				pData->mCallers.mpCaller = NULL;
			}
			
			/* free data */
			secfunc_free(*ppData);
			*ppData = NULL;
			
			ppData++;
		}
		secfunc_free(ppReleaseData);
	}
}

/**
 * secpolicy_updateprocguarddata - update process access guard data.
 * @ppUpdateData: process access data pointer
 * 
 * This function is to update process access guard data.
 *
 */
void secpolicy_updateprocguarddata(struct ProcGData **ppUpdateData)
{
	if (ppUpdateData != NULL) {
		secpolicy_releaseprocguarddata(gppprocg_data);
		gppprocg_data = ppUpdateData;
	}
}

/**
 * secpolicy_resetprocguarddata - reaset process access guard data.
 * 
 * This function is to reaset process access guard data.
 *
 */
void secpolicy_resetprocguarddata(void)
{
	secpolicy_releaseprocguarddata(gppprocg_data);
	gppprocg_data = gpprocg_data_empty;
}


/*=============================================================
 * file access guard data functions
  =============================================================*/
static struct FileGData* gpfileg_data_empty[] =
{
	NULL
};
static struct FileGData **gppfileg_data = gpfileg_data_empty;

/**
 * secpolicy_startfileguarddata - call start file access guard data.
 * 
 * This function is to start caller infomation reference.
 *
 * return infomation
 * struct FileGData**:file access data pointer
 */
struct FileGData** secpolicy_startfileguarddata(void)
{
	return gppfileg_data;
}

/**
 * secpolicy_stopfileguarddata - call stop file access guard data.
 * 
 * This function is to start caller infomation reference.
 *
 */
void secpolicy_stopfileguarddata(void)
{
}

/**
 * secpolicy_releasefileguarddata - release file access guard data.
 * @ppUpdateData: file access data pointer
 * 
 * This function is to release process access guard data.
 *
 */
void secpolicy_releasefileguarddata(struct FileGData **ppReleaseData)
{
	struct FileGData **ppData = ppReleaseData;
	
	if ((ppReleaseData != NULL) && 
	   (ppReleaseData != gpfileg_data_empty)) {
		while(*ppData != NULL) {
			int i;
			struct FileGData *pData = *ppData;
			
			/* free target data */
			if (pData->mTarget.mpPath != NULL) {
				secfunc_free(pData->mTarget.mpPath);
				pData->mTarget.mpPath = NULL;
			}
			
			/* free caller data */
			for (i = 0; i < pData->mCallers.mMax; i++) {
				/* free parent data */
				struct Caller *pParentCaller;
				if (pData->mCallers.mpCaller[i].mpCmdLine != NULL)
					secfunc_free(pData->mCallers.mpCaller[i].mpCmdLine);
				pParentCaller = pData->mCallers.mpCaller[i].mParent;
				while(pParentCaller != NULL) {
					struct Caller *pWorkCaller = NULL;
					pWorkCaller = pParentCaller->mParent;
					secfunc_free(pParentCaller);
					pParentCaller = pWorkCaller;
				}
			}
			
			if (i != 0) {
				secfunc_free(pData->mCallers.mpCaller);
				pData->mCallers.mpCaller = NULL;
			}
			
			/* free data */
			secfunc_free(*ppData);
			*ppData = NULL;
			
			ppData++;
		}
		secfunc_free(ppReleaseData);
	}
}

/**
 * secpolicy_updatefileguarddata - update file access guard data.
 * @ppUpdateData: file access data pointer
 * 
 * This function is to update process access guard data.
 *
 */
void secpolicy_updatefileguarddata(struct FileGData **ppUpdateData)
{
	if (ppUpdateData != NULL) {
		secpolicy_releasefileguarddata(gppfileg_data);
		gppfileg_data = ppUpdateData;
	}
}

/**
 * secpolicy_resetfileguarddata - reaset file access guard data.
 * @ppUpdateData: file access data pointer
 * 
 * This function is to reaset process access guard data.
 *
 */
void secpolicy_resetfileguarddata(void)
{
	secpolicy_releasefileguarddata(gppfileg_data);
	gppfileg_data = gpfileg_data_empty;
}

