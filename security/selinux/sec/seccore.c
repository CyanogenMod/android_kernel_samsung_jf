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

#include "seccore.h"
#include "secfunc.h"
#include "secpolicy.h"
#include "secpolicyif.h"
#include "seclog.h"

/**
 * seccore_callerchk_command - caller cmd check
 * @pCaller: caller infomation pointer
 * @pCallProcInfo: caller process infomation pointer
 * 
 * This function is caller command check.
 *
 * return infomation
 * true:check result OK
 * false:check result NG
 */
static bool seccore_callerchk_command(struct Caller *pCaller,
				      struct ProcessInfo *pCallProcInfo)
{
	bool result = false;		/* default check is false */
	if ((pCaller->mCmdLength == pCallProcInfo->mCmdLength) &&
		(secfunc_strcmp(pCaller->mpCmdLine,
					pCallProcInfo->mpCmdLine)  == 0))
		result = true;
	
	return result;
}

/**
 * seccore_callerchk_uid - caller uid check
 * @pCaller: caller infomation pointer
 * @pCallProcInfo: caller process infomation pointer
 * 
 * This function is caller uid check.
 *
 * return infomation
 * true:check result OK
 * false:check result NG
 */
static bool seccore_callerchk_uid(struct CallerUid *pCallerUid,
				  struct ProcessInfo *pCallProcInfo)
{
	bool result = false;		/* default check is false */
	
	/* UID check */
	if (pCallerUid->mCheckMask & CALLER_CHECKMASK_UID_ONE) {
		if (pCallerUid->mUid == pCallProcInfo->mUid)
			result = true;
	}
	
	if (result == false) {
		int lessmoreChk = 0;
		
		/* UID less check */
		if (pCallerUid->mCheckMask & CALLER_CHECKMASK_UID_LESS) {
			if (pCallerUid->mUid_Less < pCallProcInfo->mUid)
				lessmoreChk |= CALLER_CHECKMASK_UID_LESS;
		}
		/* UID more check */
		if (pCallerUid->mCheckMask & CALLER_CHECKMASK_UID_MORE) {
			if (pCallerUid->mUid_More > pCallProcInfo->mUid)
				lessmoreChk |= CALLER_CHECKMASK_UID_MORE;
		}
		
		if ((lessmoreChk != 0) &&
		    (lessmoreChk == (pCallerUid->mCheckMask &
		    (CALLER_CHECKMASK_UID_LESS | CALLER_CHECKMASK_UID_MORE))))
			result = true;
	}
	return result;
}

/**
 * seccore_callerchk_gid - caller gid check
 * @pCaller: caller infomation pointer
 * @pCallProcInfo: caller process infomation pointer
 * 
 * This function is caller gid check.
 *
 * return infomation
 * true:check result OK
 * false:check result NG
 */
static bool seccore_callerchk_gid(struct CallerGid *pCallerGid,
				  struct ProcessInfo *pCallProcInfo)
{
	bool result = false;		/* default check is false */
	
	/* GID check */
	if (pCallerGid->mCheckMask & CALLER_CHECKMASK_GID_ONE) {
		if (pCallerGid->mGid == pCallProcInfo->mGid)
			result = true;
	}
	
	if (result == false) {
		int lessmoreChk = 0;
		
		/* GID less check */
		if (pCallerGid->mCheckMask & CALLER_CHECKMASK_GID_LESS) {
			if (pCallerGid->mGid_Less < pCallProcInfo->mGid)
				lessmoreChk |= CALLER_CHECKMASK_GID_LESS;
		}
		/* GID more check */
		if (pCallerGid->mCheckMask & CALLER_CHECKMASK_GID_MORE) {
			if (pCallerGid->mGid_More > pCallProcInfo->mGid)
				lessmoreChk |= CALLER_CHECKMASK_GID_MORE;
		}
		
		if ((lessmoreChk != 0) &&
		    (lessmoreChk == (pCallerGid->mCheckMask &
		    (CALLER_CHECKMASK_GID_LESS | CALLER_CHECKMASK_GID_MORE))))
			result = true;
	}
	return result;
}

/**
 * seccore_callerchk_pid - caller pid check
 * @pCaller: caller infomation pointer
 * @pCallProcInfo: caller process infomation pointer
 * 
 * This function is caller pid check.
 *
 * return infomation
 * true:check result OK
 * false:check result NG
 */
static bool seccore_callerchk_pid(struct CallerPid *pCallerPid,
				  struct ProcessInfo *pCallProcInfo)
{
	bool result = false;		/* default check is false */
	
	/* PID check */
	if (pCallerPid->mCheckMask & CALLER_CHECKMASK_PID_ONE) {
		if (pCallerPid->mPid == pCallProcInfo->mPid)
			result = true;
	}
	
	if (result == false) {
		int lessmoreChk = 0;
		
		/* PID less check */
		if (pCallerPid->mCheckMask & CALLER_CHECKMASK_PID_LESS) {
			if (pCallerPid->mPid_Less < pCallProcInfo->mPid)
				lessmoreChk |= CALLER_CHECKMASK_PID_LESS;
		}
		/* PID more check */
		if (pCallerPid->mCheckMask & CALLER_CHECKMASK_PID_MORE) {
			if (pCallerPid->mPid_More > pCallProcInfo->mPid)
				lessmoreChk |= CALLER_CHECKMASK_PID_MORE;
		}
		
		if ((lessmoreChk != 0) &&
		    (lessmoreChk == (pCallerPid->mCheckMask &
		    (CALLER_CHECKMASK_PID_LESS | CALLER_CHECKMASK_PID_MORE))))
			result = true;
	}
	return result;
}

/**
 * seccore_callerchk - caller check
 * @pCaller: caller infomation pointer
 * @pCallProcInfo: caller process infomation pointer
 * 
 * This function is caller check.
 *
 * return infomation
 * true:check result OK
 * false:check result NG
 */
static bool seccore_callerchk(struct Caller *pCaller,
			      struct ProcessInfo *pCallProcInfo,
			      int parentPos)
{
	bool result = false;		/* default check result is false */
	
	if (!(pCaller->mCheckMask & CALLER_CHECKMASK_ALL_DENIAL)) {
		bool cmdChk = true;
		bool uidChk = true;
		bool gidChk = true;
		bool pidChk = true;
		bool parentChk = true;
	
		/* command check */
		if (pCaller->mCheckMask & CALLER_CHECKMASK_COMMANDNAME)
			cmdChk = seccore_callerchk_command(pCaller,
							pCallProcInfo);
		
		/* UID check */
		if ((pCaller->mCheckMask & CALLER_CHECKMASK_UID) && cmdChk)
			uidChk = seccore_callerchk_uid(&pCaller->mCallerUid,
							pCallProcInfo);
		
		/* GID check */
		if ((pCaller->mCheckMask & CALLER_CHECKMASK_GID) &&
		    cmdChk && uidChk)
			gidChk = seccore_callerchk_gid(&pCaller->mCallerGid,
							pCallProcInfo);
		
		/* PID check */
		if ((pCaller->mCheckMask & CALLER_CHECKMASK_PID) &&
		    cmdChk && uidChk && gidChk)
			pidChk = seccore_callerchk_pid(&pCaller->mCallerPid,
						       pCallProcInfo);
		
		/* parent cheack */
		if ((pCaller->mCheckMask & CALLER_CHECKMASK_PARENT) &&
		    cmdChk && uidChk && gidChk && pidChk) {
			struct ProcessInfo procInfo;
			parentPos++;
			if (secfunc_getcallprocinfo(&procInfo, parentPos)) {
				/* parent caller check */
				parentChk = seccore_callerchk(pCaller->mParent,
							&procInfo, parentPos);
				secfunc_free(procInfo.mpCmdLine);
			} else {
				/* Error. cant get parent caller infomation */
				parentChk = false;
			}
		}
		
		if (cmdChk && uidChk && gidChk && pidChk && parentChk)
			result = true;
	}
	
	return result;
}

/**
 * seccore_callerschk - caller condition check
 * @pCallers: callers infomation pointer
 * 
 * This function is caller conditioin check.
 *
 * return infomation
 * true:check result OK
 * false:check result NG
 */
static bool seccore_callerschk(struct Callers *pCallers)
{
	bool result = false;
	struct ProcessInfo procInfo;
	int i;
	if (secfunc_getcallprocinfo(&procInfo, 0)) {
		for (i = 0; i < pCallers->mMax; i++) {
			result = seccore_callerchk(&pCallers->mpCaller[i],
						   &procInfo, 0);
			if (result == true) {
				result = true;
				break;
			}
		}
		secfunc_free(procInfo.mpCmdLine);
	}
	
	return result;
}

/**
 * seccore_procguard_targetchk - target check for process guard.
 * @pProcInfo: access process infomation pointer
 * @pData: process guard data pointer
 * 
 * This function is target check for process access guard.
 *
 * return infomation
 * true:check target hit
 * false:check target no hit
 */
static bool seccore_procguard_targetchk(struct ProcessInfo *pProcInfo,
					struct ProcGData *pData)
{
	bool result = false;
	int targetCheck = 0;
	
	if (pData->mTarget.mCheckMask & PROCGDATA_TARGET_CHECKMASK_COMMANDNAME) {
		if ((pData->mTarget.mCmdLength == pProcInfo->mCmdLength) &&
		   (secfunc_strcmp(pData->mTarget.mpCmdLine,
				  pProcInfo->mpCmdLine) == 0))
			targetCheck |= PROCGDATA_TARGET_CHECKMASK_COMMANDNAME;
	}
	
	if (pData->mTarget.mCheckMask & PROCGDATA_TARGET_CHECKMASK_UID) {
		if (pData->mTarget.mUid == pProcInfo->mUid)
			targetCheck |= PROCGDATA_TARGET_CHECKMASK_UID;
	}
	
	if ((targetCheck != 0) &&
	   (targetCheck == pData->mTarget.mCheckMask))
		result = true;
	
	return result;
}

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
bool seccore_procguard(struct ProcessInfo *pProcInfo)
{
	bool result = true;
	struct ProcGData **ppData;
	
	ppData = secpolicy_startprocguarddata();
	if (ppData != NULL) {
		while (*ppData != NULL) {
			struct ProcGData *pData = *ppData;
			bool targetChk = seccore_procguard_targetchk(pProcInfo,
								     pData);
			if (targetChk) {
				result = seccore_callerschk(&pData->mCallers);
				break;
			}
			
			ppData++;
		}
	}
	secpolicy_stopprocguarddata();
	
	return result;
}

/**
 * seccore_fileguard_targetChk - target check for file guard.
 * @pFilePathName: target file full path name pointer
 * @bPreFixChk: prefix check on/off
 * @pData: file guard data pointer
 * 
 * This function is target check for file access guard.
 *
 * return infomation
 * true:check target hit
 * false:check target no hit
 */
static bool seccore_fileguard_targetChk(const char *pFilePathName,
					bool bPreFixChk,
					struct FileGData *pData)
{
	bool result = false;
	int targetCheck = 0;
	
	if (pData->mTarget.mCheckMask & FILEGDATA_TARGET_CHECKMASK_FULLPATH) {
		if (bPreFixChk) {
			/* prefix check */
			if (secfunc_strncmp(pData->mTarget.mpPath,
					   pFilePathName,
					   secfunc_strlen(pFilePathName)) == 0)
				targetCheck |= FILEGDATA_TARGET_CHECKMASK_FULLPATH;
		} else {
			/* exact check */
			if (secfunc_strcmp(pData->mTarget.mpPath,
					  pFilePathName) == 0)
				targetCheck |= FILEGDATA_TARGET_CHECKMASK_FULLPATH;
		}
	}
	
	if (pData->mTarget.mCheckMask & FILEGDATA_TARGET_CHECKMASK_FOLDERPATH) {
		if (bPreFixChk) {
			/* prefix check */
			if (secfunc_strncmp(pData->mTarget.mpPath,
					   pFilePathName,
					   secfunc_strlen(pFilePathName)) == 0)
				targetCheck |= FILEGDATA_TARGET_CHECKMASK_FOLDERPATH;
		} else {
			/* exact check */
			if (secfunc_strcmp(pData->mTarget.mpPath,
					  pFilePathName) == 0)
				targetCheck |= FILEGDATA_TARGET_CHECKMASK_FOLDERPATH;
		}
	}
	
	if ((targetCheck != 0) &&
	   (targetCheck == pData->mTarget.mCheckMask))
		result = true;
	
	return result;
}

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
bool seccore_fileguard(const char *pFilePathName,
			 bool bPreFixChk)
{
	bool result = true;
	struct FileGData **ppData;
	
	ppData = secpolicy_startfileguarddata();
	if (ppData != NULL) {
		while (*ppData != NULL) {
			struct FileGData* pData = *ppData;
			bool targetChk;
			targetChk = seccore_fileguard_targetChk(pFilePathName,
							   bPreFixChk, pData);
			if (targetChk) {
				result = seccore_callerschk(&pData->mCallers);
				break;
			}
			ppData++;
		}
	}
	secpolicy_stopfileguarddata();
	
	return result;
}


/**
 * seccore_nextheader - move next header.
 * @pHeader: header pointer
 * @size: move size
 * 
 * This function is header pointer move.
 *
 * return infomation
 * struct Header:moved header pointer
 */
static struct Header *seccore_nextheader(struct Header *pHeader, int size)
{
	char *p = (char *)pHeader;
	p += size;
	pHeader = (struct Header *)p;
	return pHeader;
}

/**
 * seccore_extenddata - extend data.
 * @pArray: data pointer
 * @srcSize: data size
 * @addSize: additional data size
 * 
 * This function is data extened.
 *
 * return infomation
 * char*: extend pointer
 */
static char* seccore_extenddata(char *pArray, int srcSize, int addSize)
{
	char *p = secfunc_malloc(srcSize + addSize);
	if (p != NULL) {
		secfunc_memset(p, 0, srcSize+addSize);
		if (pArray != NULL && srcSize != 0)
			secfunc_memcpy(p, pArray, srcSize);
	}
	return p;
}

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
bool seccore_exectldata(const char *pData, size_t size)
{
	struct Header *pHeader = (struct Header*)pData;
	while (!IS_NONDATA(pHeader->mType)) {
		int type = 0;
		int size = 0;
		struct Control *pControl;
		pControl = (struct Control*)pHeader;
		
		switch (pControl->mHeader.mType) {
		case CONTROL_DELETE:
			type = 0;
			if (pControl->mData.mDelete.mTarget &
			    CONTROL_DELETE_TARGET_CONTROL)
				/* delete control file */
				type |= CONTROL_FILEREMOVE_TYPE_CONTROL;
			
			if (pControl->mData.mDelete.mTarget &
			    CONTROL_DELETE_TARGET_PROCESSACCESS)
				/* delete process guard file */
				type |= CONTROL_FILEREMOVE_TYPE_PROCESS;
			
			if (pControl->mData.mDelete.mTarget &
			    CONTROL_DELETE_TARGET_FILEACCESS)
				/* delete file guard file */
				type |= CONTROL_FILEREMOVE_TYPE_FILE;
			
			secpolicy_setctlfsremove(type);
			size = sizeof(struct Header)
			     + sizeof(pControl->mData.mDelete.mTarget);
			break;
		case CONTROL_RESETDATA:
			if (pControl->mData.mResetData.mTarget &
			    CONTROL_RESETDATA_TARGET_FILEACCESSGUARD)
				/* reset file guard */
				secpolicy_resetfileguarddata();
			
			if (pControl->mData.mResetData.mTarget &
			    CONTROL_RESETDATA_TARGET_PROCESSACCESSGUARD)
				/* reset process guard */
				secpolicy_resetprocguarddata();
			
			size = sizeof(struct Header)
			     + sizeof(pControl->mData.mResetData.mTarget);
			break;
		default:
			SECERROR("Unknown Type=%08X",pControl->mHeader.mType);
			return false;
		}
		
		/* header shift */
		pHeader = seccore_nextheader(pHeader, size);
		while (IS_SEPARATE(pHeader->mType)) {
			pHeader = seccore_nextheader(pHeader,
						sizeof(struct Header));
		}
		
	}
	return true;
}

/**
 * seccore_parsecaller - caller data prase.
 * @pAccessData: access data pointer
 * @pCaller: caller policy data pointer
 * 
 * This function is caller policy prase.
 *
 * return infomation
 * int: update caller policy data size
 */
static int seccore_parsecaller(struct Caller *pAccessData,
				struct PolicyCaller *pCaller)
{
	int size = 0;
	switch (pCaller->mHeader.mType) {
	case POLICY_CALLER_TYPE_COMMANDNAME:
		pAccessData->mCheckMask |= CALLER_CHECKMASK_COMMANDNAME;
		size = pCaller->mData.mCommandName.mSize;
		pAccessData->mCmdLength = pCaller->mData.mCommandName.mSize;
		pAccessData->mpCmdLine = secfunc_malloc(pAccessData->mCmdLength + 1);
		if (pAccessData->mpCmdLine != NULL) {
			secfunc_memset(pAccessData->mpCmdLine,
				       0,
				       pAccessData->mCmdLength + 1);
			secfunc_memcpy(pAccessData->mpCmdLine,
				       pCaller->mData.mCommandName.mName,
				       pAccessData->mCmdLength);
			
			size = sizeof(struct Header)
			     + sizeof(pCaller->mData.mCommandName.mSize)
			     + pCaller->mData.mCommandName.mSize;
		}
		
		break;
	case POLICY_CALLER_TYPE_UID_ONE:
		pAccessData->mCheckMask |= CALLER_CHECKMASK_UID;
		pAccessData->mCallerUid.mCheckMask |= CALLER_CHECKMASK_UID_ONE;
		pAccessData->mCallerUid.mUid = pCaller->mData.mUidOne.mUid;
		
		size = sizeof(struct Header)
		     + sizeof(pCaller->mData.mUidOne.mUid);
		
		break;
	case POLICY_CALLER_TYPE_UID_LESS:
		pAccessData->mCheckMask |= CALLER_CHECKMASK_UID;
		pAccessData->mCallerUid.mCheckMask |= CALLER_CHECKMASK_UID_LESS;
		pAccessData->mCallerUid.mUid_Less = pCaller->mData.mUidLess.mUid;
		
		size = sizeof(struct Header)
		     + sizeof(pCaller->mData.mUidLess.mUid);
		
		break;
	case POLICY_CALLER_TYPE_UID_MORE:
		pAccessData->mCheckMask |= CALLER_CHECKMASK_UID;
		pAccessData->mCallerUid.mCheckMask |= CALLER_CHECKMASK_UID_MORE;
		pAccessData->mCallerUid.mUid_More = pCaller->mData.mUidMore.mUid;
		
		size = sizeof(struct Header)
		     + sizeof(pCaller->mData.mUidMore.mUid);
		
		break;

	case POLICY_CALLER_TYPE_GID_ONE:
		pAccessData->mCheckMask |= CALLER_CHECKMASK_GID;
		pAccessData->mCallerGid.mCheckMask |= CALLER_CHECKMASK_GID_ONE;
		pAccessData->mCallerGid.mGid = pCaller->mData.mGidOne.mGid;
		
		size = sizeof(struct Header)
		     + sizeof(pCaller->mData.mGidOne.mGid);
		
		break;
	case POLICY_CALLER_TYPE_GID_LESS:
		pAccessData->mCheckMask |= CALLER_CHECKMASK_GID;
		pAccessData->mCallerGid.mCheckMask |= CALLER_CHECKMASK_GID_LESS;
		pAccessData->mCallerGid.mGid_Less = pCaller->mData.mGidLess.mGid;
		
		size = sizeof(struct Header)
		     + sizeof(pCaller->mData.mGidLess.mGid);
		
		break;
	case POLICY_CALLER_TYPE_GID_MORE:
		pAccessData->mCheckMask |= CALLER_CHECKMASK_GID;
		pAccessData->mCallerGid.mCheckMask |= CALLER_CHECKMASK_GID_MORE;
		pAccessData->mCallerGid.mGid_More = pCaller->mData.mGidMore.mGid;
		
		size = sizeof(struct Header)
		     + sizeof(pCaller->mData.mGidMore.mGid);
		
		break;
	case POLICY_CALLER_TYPE_PID_ONE:
		pAccessData->mCheckMask |= CALLER_CHECKMASK_PID;
		pAccessData->mCallerPid.mCheckMask |= CALLER_CHECKMASK_PID_ONE;
		pAccessData->mCallerPid.mPid = pCaller->mData.mPidOne.mPid;
		
		size = sizeof(struct Header)
		     + sizeof(pCaller->mData.mPidOne.mPid);
		
		break;
	case POLICY_CALLER_TYPE_PID_LESS:
		pAccessData->mCheckMask |= CALLER_CHECKMASK_PID;
		pAccessData->mCallerPid.mCheckMask |= CALLER_CHECKMASK_PID_LESS;
		pAccessData->mCallerPid.mPid_Less = pCaller->mData.mPidLess.mPid;
		
		size = sizeof(struct Header)
		     + sizeof(pCaller->mData.mPidLess.mPid);
		
		break;
	case POLICY_CALLER_TYPE_PID_MORE:
		pAccessData->mCheckMask |= CALLER_CHECKMASK_PID;
		pAccessData->mCallerPid.mCheckMask |= CALLER_CHECKMASK_PID_MORE;
		pAccessData->mCallerPid.mPid_More = pCaller->mData.mPidMore.mPid;
		
		size = sizeof(struct Header)
		     + sizeof(pCaller->mData.mPidMore.mPid);
		
		break;
	case POLICY_CALLER_TYPE_ALL_DENIAL:
		pAccessData->mCheckMask |= CALLER_CHECKMASK_ALL_DENIAL;
		
		size = sizeof(struct Header);
		
		break;
	default:
		SECERROR("Unknown Type=%08X", pCaller->mHeader.mType);
		break;
	}
	return size;
}

/**
 * seccore_execaller - caller data execute.
 * @pHeader: header data pointer
 * @pCallers: callers data pointer
 * @ppNextHeader: next caller header pointer
 * 
 * This function is caller execute.
 *
 * return infomation
 * true:execute success
 * false:execute failed
 */
static bool seccore_execaller(struct Header *pHeader,
			      struct Callers *pCallers,
			      struct Header **ppNextHeader)
{
	bool result = true;
	struct Caller *pSetCaller = NULL;
	while (true) {
		if (pHeader->mType == SEPARATE_NEXT) {
			/* header shift */
			pHeader = seccore_nextheader(pHeader, sizeof(struct Header));
			
			if (IS_CALLER(pHeader->mType)) {
				/* memory extended */
				struct Caller* pCaller;
				pCallers->mMax++;
				pCaller = (struct Caller *)seccore_extenddata(
					(char *)pCallers->mpCaller,
					sizeof(struct Caller) * pCallers->mMax,
					sizeof(struct Caller));
				if (pCaller == NULL) {
					result = false;
					break;
				}

				if (pCallers->mpCaller != NULL)
					secfunc_free(pCallers->mpCaller);
				
				pCallers->mpCaller = pCaller;
				pSetCaller = &pCallers->mpCaller[pCallers->mMax - 1];
			} else if (IS_TARGET(pHeader->mType)) {
				break;
			}
		} else if (pHeader->mType == SEPARATE_PARENT) {
			struct Caller* pWorkCaller = pSetCaller;
			pSetCaller->mCheckMask |= CALLER_CHECKMASK_PARENT;
			
			/* header shift */
			pHeader = seccore_nextheader(pHeader,
						     sizeof(struct Header));
			
			/* parent data set */
			pSetCaller = secfunc_malloc(sizeof(struct Caller));
			if (pSetCaller == NULL) {
				result = false;
				break;
			}
			secfunc_memset(pSetCaller, 0, sizeof(struct Caller));
			pWorkCaller->mParent = pSetCaller;
			
		} else if (IS_NONDATA(pHeader->mType)) {
			/* end */
			break;
		}
		
		/* read caller condifions */
		while (IS_CALLER(pHeader->mType)) {
			int size;
			struct PolicyCaller* pCaller;
			pCaller = (struct PolicyCaller *)pHeader;
			size = seccore_parsecaller(pSetCaller, pCaller);
			if (size == 0)
				break;
			/* header shift */
			pHeader = seccore_nextheader(pHeader, size);
		}
		
		if (IS_CALLER(pHeader->mType)) {
			result = false;
			break;
		}
	}
	
	*ppNextHeader = pHeader;
	return result;
}

/**
 * seccore_exeprocg_target - process target execute.
 * @pHeader: header data pointer
 * @pProcGData: process access data pointer
 * 
 * This function is process guard target execute.
 *
 * return infomation
 * struct Header*:update header pointer
 */
static struct Header* seccore_exeprocg_target(struct Header *pHeader,
					      struct ProcGData *pProcGData)
{
	while (IS_TARGET(pHeader->mType)) {
		int size = 0;
		struct PolicyProcGTarget *pTarget;
		pTarget = (struct PolicyProcGTarget *)pHeader;
		
		switch (pTarget->mHeader.mType) {
		case POLICY_PROC_TARGET_TYPE_COMMANDNAME:
			pProcGData->mTarget.mCheckMask |= PROCGDATA_TARGET_CHECKMASK_COMMANDNAME;
			size = pTarget->mData.mCommandName.mSize;
			
			pProcGData->mTarget.mCmdLength = pTarget->mData.mCommandName.mSize;
			pProcGData->mTarget.mpCmdLine = secfunc_malloc(pProcGData->mTarget.mCmdLength + 1);
			if (pProcGData->mTarget.mpCmdLine != NULL) {
				secfunc_memset(pProcGData->mTarget.mpCmdLine,
					       0,
					       pProcGData->mTarget.mCmdLength + 1);
				secfunc_memcpy(pProcGData->mTarget.mpCmdLine,
					       &pTarget->mData.mCommandName.mName[0],
					       pProcGData->mTarget.mCmdLength);
				size = sizeof(struct Header)
				     + sizeof(pTarget->mData.mCommandName.mSize)
				     + pTarget->mData.mCommandName.mSize;
			} else {
				pHeader = NULL;
			}
			
			break;
		case POLICY_PROC_TARGET_TYPE_UID:
			pProcGData->mTarget.mCheckMask |= PROCGDATA_TARGET_CHECKMASK_UID;
			pProcGData->mTarget.mUid = pTarget->mData.mUid.mUid;
			
			size = sizeof(struct Header)
			     + sizeof(pTarget->mData.mUid.mUid);
			
			break;
		}
		
		if (pHeader == NULL)
			break;
		
		/* header shift */
		pHeader = seccore_nextheader(pHeader, size);
	}
	
	return pHeader;
}

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
bool seccore_exeprocguarddata(const char *pData, size_t size)
{
	bool result = true;
	int targetSetIndex = -1;
	struct Header* pHeader = (struct Header *)(pData);
	struct ProcGData **ppUpdateData = NULL;
	struct ProcGData *pProcGData = NULL;
	struct ProcGData **ppWork = NULL;
	
	while (!IS_NONDATA(pHeader->mType)) {
		targetSetIndex++;
		/* memory extend */
		ppWork=(struct ProcGData **)seccore_extenddata(
			(char *)ppUpdateData,
			sizeof(struct ProcGData *) * (targetSetIndex + 1),
			sizeof(struct ProcGData *));
		if (ppWork == NULL) {
			result = false;
			break;
		}
		if (ppUpdateData != NULL)
			secfunc_free(ppUpdateData);
		
		ppUpdateData = ppWork;
		
		/* memory extend add target data */
		pProcGData = secfunc_malloc(sizeof(struct ProcGData));
		if (pProcGData == NULL) {
			result = false;
			break;
		}
		secfunc_memset(pProcGData, 0, sizeof(struct ProcGData));
		ppUpdateData[targetSetIndex] = pProcGData;
		
		/* read target conditions */
		pHeader = seccore_exeprocg_target(pHeader, pProcGData);
		
		/* read caller conditions */
		result = seccore_execaller(pHeader,
					   &pProcGData->mCallers,
					   &pHeader);
		if (result == false)
			break;
		
	}
	
	if (result)
	{
		/* update data */
		secpolicy_updateprocguarddata(ppUpdateData);
	} else {
		/* release data */
		secpolicy_releaseprocguarddata(ppUpdateData);
	}
	return result;
}

/**
 * seccore_exefileg_target - file target execute.
 * @pHeader: header pointer
 * @pFileGData: file access data pointer
 * 
 * This function is file guard target execute.
 *
 * return infomation
 * struct Header*:update header pointer
 */
static struct Header* seccore_exefileg_target(struct Header *pHeader,
					      struct FileGData *pFileGData)
{
	while (IS_TARGET(pHeader->mType)) {
		int size = 0;
		struct PolicyFileGTarget *pTarget;
		pTarget = (struct PolicyFileGTarget *)pHeader;
		
		switch (pTarget->mHeader.mType) {
		case POLICY_FILE_TARGET_TYPE_FULLPATH:
			pFileGData->mTarget.mCheckMask |= FILEGDATA_TARGET_CHECKMASK_FULLPATH;
			size = pTarget->mData.mFullPath.mSize;
			pFileGData->mTarget.mpPath = secfunc_malloc(size + 1);
			if (pFileGData->mTarget.mpPath != NULL) {
				secfunc_memset(pFileGData->mTarget.mpPath, 0, size + 1);
				secfunc_memcpy(pFileGData->mTarget.mpPath,
					       pTarget->mData.mFullPath.mPath,
					       size);
				size = sizeof(struct Header)
				     + sizeof(pTarget->mData.mFullPath.mSize)
				     + pTarget->mData.mFullPath.mSize;
			} else {
				pHeader = NULL;
			}
			break;
		case POLICY_FILE_TARGET_TYPE_FOLDERPATH:
			pFileGData->mTarget.mCheckMask |= FILEGDATA_TARGET_CHECKMASK_FOLDERPATH;
			size = pTarget->mData.mFolderPath.mSize;
			pFileGData->mTarget.mpPath = secfunc_malloc(size + 1);
			if (pFileGData->mTarget.mpPath != NULL) {
				secfunc_memset(pFileGData->mTarget.mpPath, 0, size + 1);
				secfunc_memcpy(pFileGData->mTarget.mpPath,
					       pTarget->mData.mFolderPath.mPath,
					       size);
				
				size = sizeof(struct Header)
				     + sizeof(pTarget->mData.mFolderPath.mSize)
				     + pTarget->mData.mFolderPath.mSize;
			} else {
				pHeader = NULL;
			}
			break;
		}
		
		if (pHeader == NULL)
			break;
		
		/* header shift */
		pHeader = seccore_nextheader(pHeader, size);
	}
	
	return pHeader;
}

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
bool seccore_exefileguarddata(const char *pData, size_t size)
{
	bool result = true;
	int targetSetIndex=-1;
	struct Header* pHeader = (struct Header*)(pData);
	struct FileGData **ppUpdateData = NULL;
	struct FileGData *pFileGData = NULL;
	struct FileGData **ppWork = NULL;
	
	while (!IS_NONDATA(pHeader->mType)) {
		targetSetIndex++;
		
		/* memory extend */
		ppWork = (struct FileGData**)seccore_extenddata(
			(char *)ppUpdateData,
			sizeof(struct FileGData *) * (targetSetIndex + 1),
			sizeof(struct FileGData *));
		if (ppWork == NULL) {
			result = false;
			break;
		}
		if (ppUpdateData != NULL)
			secfunc_free(ppUpdateData);
		
		ppUpdateData = ppWork;
		
		/* memory extend add target data */
		pFileGData = secfunc_malloc(sizeof(struct FileGData));
		if (pFileGData == NULL) {
			result = false;
			break;
		}
		secfunc_memset(pFileGData, 0, sizeof(struct FileGData));
		ppUpdateData[targetSetIndex] = pFileGData;
		
		/* read target conditions */
		pHeader = seccore_exefileg_target(pHeader, pFileGData);
		if (pHeader == NULL) {
			result = false;
			break;
		}
		
		/* read caller conditions */
		result = seccore_execaller(pHeader,
					   &pFileGData->mCallers,
					   &pHeader);
		if (result == false)
			break;
		
	}
	
	if (result) {
		/* update data */
		secpolicy_updatefileguarddata(ppUpdateData);
	} else {
		/* release data */
		secpolicy_releasefileguarddata(ppUpdateData);
	}
	return result;
}

