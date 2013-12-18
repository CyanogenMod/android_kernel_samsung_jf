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

#ifndef SECPOLICY_H_
#define SECPOLICY_H_

#include "seccore.h"

#define MAX_COMMAND_NAME FIELD_SIZEOF(struct task_struct, comm)

/*=============================================================
 * control file functions
  =============================================================*/
#define CONTROL_FILEREMOVE_TYPE_CONTROL 0x00000001
#define CONTROL_FILEREMOVE_TYPE_PROCESS 0x00000002
#define CONTROL_FILEREMOVE_TYPE_FILE    0x00000004
extern int secpolicy_getctlfsremove(void);
extern void secpolicy_setctlfsremove(int type);

/*=============================================================
 * process access policy functions
  =============================================================*/
extern struct ProcGData **secpolicy_startprocguarddata(void);
extern void secpolicy_stopprocguarddata(void);
extern void secpolicy_releaseprocguarddata(struct ProcGData **ppReleaseData);
extern void secpolicy_updateprocguarddata(struct ProcGData **ppUpdateData);
extern void secpolicy_resetprocguarddata(void);

/*=============================================================
 * file access policy functions
  =============================================================*/
extern struct FileGData **secpolicy_startfileguarddata(void);
extern void secpolicy_stopfileguarddata(void);
extern void secpolicy_releasefileguarddata(struct FileGData **ppReleaseData);
extern void secpolicy_updatefileguarddata(struct FileGData **ppUpdateData);
extern void secpolicy_resetfileguarddata(void);

#endif
