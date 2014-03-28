/**
   @copyright
   Copyright (c) 2013, INSIDE Secure Oy. All rights reserved.
*/


#include "implementation_defs.h"
#include "spd_util.h"
#include "ip_selector_db.h"
#include "kernelspd_command.h"

char *unprotected_ifaces;

int
spd_is_protected_interface(
        const char *name)
{
  if (name == NULL)
    {
      return 1;
    }

  if (unprotected_ifaces != NULL)
    {
      const char *p = unprotected_ifaces;

      while (*p)
        {
          const char *end = p;

          while (*end != '\0' && *end != ',')
            end++;

          if (strncmp(name, p, end - p) == 0)
            {
              return 0;
            }

          p = end;
          if (*p) p++;
        }
    }

  return 1;
}
