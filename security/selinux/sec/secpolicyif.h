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


#ifndef SECPOLICYIF_H_
#define SECPOLICYIF_H_

/*=============================================================
 * folder & file declaration
  =============================================================*/

/* folder name */
#define SECFS_FOLDERNAME  "sec"

/* file name */
#define SECFS_FILENAME_CONTROL                   "control"
#define SECFS_FILENAME_POLICY_FILEACCESSGUARD    "fileaccess"
#define SECFS_FILENAME_POLICY_PROCESSACCESSGUARD "processaccess"


/*=============================================================
 * common const declaration
  =============================================================*/
#define PROCESS_NAME_MAX	17
			/* size of FIELD_SIZEOF(struct task_struct, comm) */

#define FLAG_TARGET		0x00010000
#define FLAG_CALLER		0x00020000
#define FLAG_SEPARATE		0x00040000

#define TARGET_TYPE(type)	 (FLAG_TARGET     | type)
#define CALLER_TYPE(type)	 (FLAG_CALLER     | type)
#define SEPATATE_TYPE(type)	 (FLAG_SEPARATE   | type)
#define NONDATA_TYPE		0x00000000 /* End data */

#define IS_TARGET(type)		((FLAG_TARGET     & type) != 0)
#define IS_CALLER(type)		((FLAG_CALLER     & type) != 0)
#define IS_SEPARATE(type)	((FLAG_SEPARATE   & type) != 0)
#define IS_NONDATA(type)	(type == NONDATA_TYPE)

/**
 * struct Header - Header parameters
 * @mType: header type
 *
 * Used to Header parametaers.
 */
struct Header {
	int mType;
};

/*=============================================================
 * type of separate
  =============================================================*/
#define SEPARATE_NEXT		SEPATATE_TYPE(0x0001)	/* next break */
#define SEPARATE_PARENT		SEPATATE_TYPE(0x0002)	/* parent break */

/*=============================================================
 * control file operatioin
  =============================================================*/
#define CONTROL_DELETE		TARGET_TYPE(0x0002)	/* file delete */
#define CONTROL_RESETDATA	TARGET_TYPE(0x0008)	/* Reset Data */


/* disable control file*/
#define CONTROL_DELETE_TARGET_CONTROL		0x00000001
/* disable process guard file */
#define CONTROL_DELETE_TARGET_PROCESSACCESS	0x00000002
/* disable file guard file*/
#define CONTROL_DELETE_TARGET_FILEACCESS	0x00000004
/* disable all file */
#define CONTROL_DELETE_TARGET_ALL				\
			(CONTROL_DELETE_TARGET_CONTROL |	\
			 CONTROL_DELETE_TARGET_PROCESSACCESS |	\
			 CONTROL_DELETE_TARGET_FILEACCESS)

/* reset file guard */
#define CONTROL_RESETDATA_TARGET_FILEACCESSGUARD	0x00000001
/* reset process guard */
#define CONTROL_RESETDATA_TARGET_PROCESSACCESSGUARD	0x00000002
/* reset all */
#define CONTROL_RESETDATA_TARGET_ALL					\
			(CONTROL_RESETDATA_TARGET_FILEACCESSGUARD |	\
			 CONTROL_RESETDATA_TARGET_PROCESSACCESSGUARD)

/**
 * struct Control - Control parameters
 * @mHeader: header
 * @mDump: command parameter type of CONTROL_DUMP
 * @mDelete: uid parameter type of CONTROL_DELETE
 * @mPrint: uid parameter type of CONTROL_PRINT
 * @mResetData: uid parameter type of CONTROL_RESETDATA
 * @mLogLevel: uid parameter type of CONTROL_LOGLV
 *
 * Used to Control parametaers.
 */
struct Control {
	struct Header		mHeader;
	union {
		
		/* CONTROL_DELETE */
		struct {
			int	mTarget;
		} mDelete;
		
		
		/* CONTROL_RESETDATA */
		struct {
			int	mTarget;
		} mResetData;
		
	} mData;
};

/*=============================================================
 * process accessguard file operatioin
  =============================================================*/

#define POLICY_PROC_TARGET_TYPE_COMMANDNAME		TARGET_TYPE(0x0001)
#define POLICY_PROC_TARGET_TYPE_UID			TARGET_TYPE(0x0002)

/**
 * struct PolicyProcGTarget - Process Policy parameters
 * @mHeader: header
 * @mCommandName: command parameter type of POLICY_PROC_TARGET_TYPE_COMMANDNAME
 * @mUid: uid parameter type of POLICY_FILEACCESSGUARGUARD_TARGET_UID
 *
 * Used to File Policy parametaers.
 */
struct PolicyProcGTarget {
	struct Header		mHeader;
	union {
		/* POLICY_PROC_TARGET_TYPE_COMMANDNAME */
		struct {
			int	mSize;			/* size of command */
			char	mName[1];		/* command name */
		} mCommandName;
		
		/* POLICY_FILEACCESSGUARGUARD_TARGET_UID */
		struct {
			uid_t	mUid;
		} mUid;
	} mData;
};


/*=============================================================
 * file access guard file operatioin
  =============================================================*/
#define POLICY_FILE_TARGET_TYPE_FULLPATH		TARGET_TYPE(0x0001)
#define POLICY_FILE_TARGET_TYPE_FOLDERPATH		TARGET_TYPE(0x0002)

/**
 * struct PolicyFileGTarget - File Policy parameters
 * @mHeader: header
 * @mFullPath: path parameter type of POLICY_FILE_TARGET_TYPE_FULLPATH
 * @mFolderPath: folder parameter type of POLICY_FILE_TARGET_TYPE_FOLDERPATH
 *
 * Used to File Policy parametaers.
 */
struct PolicyFileGTarget {
	struct Header		mHeader;
	union {
		/* POLICY_FILE_TARGET_TYPE_FULLPATH */
		struct {
			int	mSize;		/* size of path */
			char	mPath[1];	/* file path */
		} mFullPath;
		
		/* POLICY_FILE_TARGET_TYPE_FOLDERPATH */
		struct {
			int	mSize;		/* size of path */
			char	mPath[1];	/* folder path */
		} mFolderPath;
	} mData;
};

/*=============================================================
 * caller condition
  =============================================================*/
#define POLICY_CALLER_TYPE_COMMANDNAME	CALLER_TYPE(0x0001)

#define POLICY_CALLER_TYPE_UID_ONE		CALLER_TYPE(0x0002)
#define POLICY_CALLER_TYPE_UID_LESS		CALLER_TYPE(0x0004)
#define POLICY_CALLER_TYPE_UID_MORE		CALLER_TYPE(0x0008)

#define POLICY_CALLER_TYPE_GID_ONE		CALLER_TYPE(0x0010)
#define POLICY_CALLER_TYPE_GID_LESS		CALLER_TYPE(0x0020)
#define POLICY_CALLER_TYPE_GID_MORE		CALLER_TYPE(0x0040)

#define POLICY_CALLER_TYPE_PID_ONE		CALLER_TYPE(0x0080)
#define POLICY_CALLER_TYPE_PID_LESS		CALLER_TYPE(0x0100)
#define POLICY_CALLER_TYPE_PID_MORE		CALLER_TYPE(0x0200)

#define POLICY_CALLER_TYPE_ALL_DENIAL	CALLER_TYPE(0x8000)

/**
 * struct PolicyCaller - Caller Policy parameters
 * @mHeader: header
 * @mCommandName: command parameter type of POLICY_CALLER_TYPE_COMMANDNAME
 * @mUidOne: uid parameter type of POLICY_CALLER_TYPE_UID
 * @mUidLess: uid less parameter type of POLICY_CALLER_TYPE_UID_LESS
 * @mUidMore: uid more parameter type of POLICY_CALLER_TYPE_UID_MORE
 * @mGidOne: gid parameter type of POLICY_CALLER_TYPE_GID
 * @mGidLess: gid less parameter type of POLICY_CALLER_TYPE_GID_LESS
 * @mGidMore: gid more parameter type of POLICY_CALLER_TYPE_GID_MORE
 * @mPidOne: pid parameter type of POLICY_CALLER_TYPE_PID
 * @mPidLess: pid less parameter type of POLICY_CALLER_TYPE_PID_LESS
 * @mPidMore: pid more parameter type of POLICY_CALLER_TYPE_PID_MORE
 *
 * Used to Policy parametaers.
 * No struct type of POLICY_CALLER_TYPE_ALL_DENIAL
 */
struct PolicyCaller {
	struct Header		mHeader;
	union {
		/* POLICY_CALLER_TYPE_COMMANDNAME */
		struct {
			int	mSize;			/* size of command */
			char	mName[1];		/* command name */
		} mCommandName;
		
		/* POLICY_CALLER_TYPE_UID */
		struct {
			uid_t	mUid;
		} mUidOne;
		
		/* POLICY_CALLER_TYPE_UID_LESS */
		struct {
			uid_t	mUid;
		} mUidLess;
		
		/* POLICY_CALLER_TYPE_UID_MORE */
		struct {
			uid_t	mUid;
		} mUidMore;
		
		/* POLICY_CALLER_TYPE_GID */
		struct {
			gid_t	mGid;
		} mGidOne;
		
		/* POLICY_CALLER_TYPE_GID_LESS */
		struct {
			gid_t	mGid;
		} mGidLess;
		
		/* POLICY_CALLER_TYPE_GID_MORE */
		struct {
			gid_t	mGid;
		} mGidMore;
		
		/* POLICY_CALLER_TYPE_PID */
		struct {
			pid_t	mPid;
		} mPidOne;
		
		/* POLICY_CALLER_TYPE_PID_LESS */
		struct {
			pid_t	mPid;
		} mPidLess;
		
		/* POLICY_CALLER_TYPE_PID_MORE */
		struct {
			pid_t	mPid;
		} mPidMore;
		
		/* POLICY_CALLER_TYPE_ALL_DENIAL */
		/* No Struct */
		
	} mData;
};


#endif /* SECPOLICYIF_H_ */
