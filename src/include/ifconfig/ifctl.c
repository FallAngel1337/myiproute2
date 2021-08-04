#include "ifctl.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>

#define BUFF 1024

// return the name of founding interfaces e.g.: eth0
struct if_list* get_if_list(struct if_list *ifl, uint8_t _flags)
{
    struct ifconf ifc;
    char buf[BUFF];
    int sock = 0;

    ifc.ifc_buf = buf;
    ifc.ifc_len = BUFF;

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0
    || ioctl(sock, SIOCGIFCONF, &ifc))
    {
        perror("something gone wrong");
        return NULL;
    }

    ifl->if_len = ifc.ifc_len/sizeof(struct ifreq);
    if (_flags&IGNORE_LOOPBACK) ifl->if_len--;
    ifl->if_name = malloc(ifl->if_len * sizeof(char*));

    for (int i=0; i < ifl->if_len;) {
        ifl->if_name[i] = malloc(IF_NAMESIZE);
        
        if (ioctl(sock, SIOCGIFFLAGS, ifc.ifc_req) < 0) {
            perror("something gone wrong");
            return NULL; 
        }

        if (_flags&IGNORE_LOOPBACK) {
            if (!(ifc.ifc_req->ifr_flags&IFF_LOOPBACK)) {
                strncpy(ifl->if_name[i], ifc.ifc_req->ifr_name, IF_NAMESIZE);
                i++;
            }
        } else {
            strncpy(ifl->if_name[i], ifc.ifc_req->ifr_name, IF_NAMESIZE);
            i++;
        }

        ifc.ifc_req++;
    }

    close(sock);
    return ifl;
}

// free the if_list structure members
void if_list_free(struct if_list *ifl)
{
    for (int i=0; i < ifl->if_len; i++) {
        free(ifl->if_name[i]);
    }
    free(ifl->if_name);
}

// return information about the interface
struct if_info* get_if_info(struct if_info *ifi, uint8_t _flags)
{
    return NULL;
}
