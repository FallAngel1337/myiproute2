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
#include <sys/types.h>
#include <netinet/in.h>

// some basic flags to use
enum {
    CHECK_UP = 0x1, /* if interface is up */
    CHECK_DOWN = 0x2, /* if interface is down */
    SET_UP = 0x4, /* set interface up */
    SET_DOWN = 0x8, /* set interace down */

    IF_IGLOOPBACK = 0x10, /* ignore the loopback interface */

    IF_LIST = 0x20, /* list all found interfaces */
    IF_INFO = 0x40, /* information about one single interface */
    IF_BRIEF = 0x80, /* brief output */
};

struct if_info {
    char if_name[IF_NAMESIZE];
    struct sockaddr_in if_addr;
    struct sockaddr_in6 if_addr6;
    
    union
    {
        struct sockaddr if_netmask; /* netmask  */
        struct sockaddr if_wildcard;
    } if_mask;

	struct sockaddr if_hwaddr; /* MAC address */

    union 
    {
        struct sockaddr if_broadcast; /* broadcast address */
        struct sockaddr if_network; /* network address (first (not usable) address) */
    };

    unsigned short if_flags;
};
#define if_netmask if_mask.if_netmask
#define if_wilcard if_mask.if_wildcard

struct if_list {
    char **if_name;
    size_t if_len; /* number of interfaces */
};

// return the name of founding interfaces e.g.: eth0
struct if_list* get_if_list(struct if_list *ifl, uint8_t _flags);

// free the if_list structure memebers
void if_list_free(struct if_list *ifl);

// return information about the interface
struct if_info* get_if_info(struct if_info *ifi, char *if_name, uint8_t _flags);

// display all retrived information
void show_info(struct if_info *ifi, uint8_t _flags);

#else
#error ** Seems your not at a GNU/Linux environment ... **

#endif  /* __linux__ */
#endif /* _IFCTL_H */