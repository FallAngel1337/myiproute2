/**
 * ifctl.h - Interface control library
 * Author: FallAngel
 * 
 * Copyright © 2021
 *
 * Here contain some useful definitions/macros
 * that will be used to control/interact with the
 * linux network interfaces.
*/

#ifndef _IFCTL_H
#define _IFCTL_H

#ifdef __linux__

#include <net/if.h>
#include <sys/ioctl.h>
#include <linux/netlink.h>

#else
#error ** Seems your not at a GNU/Linux environment ... **

#endif  /* __linux__ */

#endif /* _IFCTL_H */