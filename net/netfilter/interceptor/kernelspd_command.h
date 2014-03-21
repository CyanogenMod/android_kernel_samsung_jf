/**
   @copyright
   Copyright (c) 2013, INSIDE Secure Oy. All rights reserved.
*/

#ifndef KERNEL_SPD_COMMAND_H
#define KERNEL_SPD_COMMAND_H

#include "public_defs.h"

#define LINUX_SPD_PROC_FILENAME "spd"

#define KERNEL_SPD_ID_VALID(__spd_id) \
  ((__spd_id) == KERNEL_SPD_IN || (__spd_id) == KERNEL_SPD_OUT)

enum
{
  KERNEL_SPD_IN,
  KERNEL_SPD_OUT,
  KERNEL_SPD_COUNT
};


enum
{
  /*
     Activate command is followed by a comma separated string of
     unprotected interfaces.
  */
  KERNEL_SPD_ACTIVATE,

  /*
    Insert command is followed by an encoded IP Selector Group.
   */
  KERNEL_SPD_INSERT_ENTRY,

  /*
    Remove command has no additional data.
   */
  KERNEL_SPD_REMOVE_ENTRY,

  /*
    Deactivate command has no additional data.
   */
  KERNEL_SPD_DEACTIVATE
};


enum
{
  KERNEL_SPD_DISCARD,
  KERNEL_SPD_BYPASS
};

struct KernelSpdCommand
{
  uint32_t command_id;
  uint32_t action_id;
  uint32_t priority;
  uint32_t precedence;
  uint32_t spd_id;
  uint32_t entry_id;
  uint32_t bytecount;
};

#endif /* KERNEL_SPD_COMMAND_H */

