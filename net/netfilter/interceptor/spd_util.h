/**
   @copyright
   Copyright (c) 2013, INSIDE Secure Oy. All rights reserved.
*/

#ifndef SPD_UTIL_H
#define SPD_UTIL_H

extern char *unprotected_ifaces;

int
spd_is_protected_interface(
        const char *name);

#endif /* SPD_UTIL_H */
