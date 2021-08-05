#define _GNU_SOURCE
#include "ifctl.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ether.h>

#define BUFF 1024

int idx = 0;

static inline unsigned get_bits(struct sockaddr_in *netmask)
{
    unsigned bits = 0; /* bits equals to 1 */
    for (int i=0; i < 32; i++) ((netmask->sin_addr.s_addr>>i)&0x1) ? bits++ : 0;
    return bits;
}

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
        perror("ioctl SIOCGIFCONF");
        return NULL;
    }

    ifl->if_len = ifc.ifc_len/sizeof(struct ifreq);
    if (_flags&IF_IGLOOPBACK) ifl->if_len--;
    ifl->if_name = malloc(ifl->if_len * sizeof(char*));

    for (int i=0; i < ifl->if_len;) {
        ifl->if_name[i] = malloc(IF_NAMESIZE);
        
        if (ioctl(sock, SIOCGIFFLAGS, ifc.ifc_req) < 0) {
            perror("ioctl SIOCGIFFLAGS");
            return NULL; 
        }

        if (_flags&IF_IGLOOPBACK) {
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

// return information about the interface(s)
struct if_info* get_if_info(struct if_info *ifi, char *if_name, uint8_t _flags)
{
    if (!if_name) {
        struct if_list ifl;
        if (!get_if_list(&ifl, _flags)) {
            fprintf(stderr, "couldn't found a interface\n");
            return NULL;
        }

        if (!ifi) ifi = malloc(ifl.if_len * sizeof(*ifi));

        for (int i=0; i < ifl.if_len; i++) {
            get_if_info(ifi, ifl.if_name[i], _flags);
        }

        return ifi;
    } else {
        if (!ifi) ifi = malloc(sizeof(*ifi));

        struct ifreq ifr;
        int sock = 0;
        if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            perror("socket()");
            return NULL;
        }

        strncpy(ifr.ifr_name, if_name, IF_NAMESIZE);
        strncpy(ifi[idx].if_name, if_name, IF_NAMESIZE);

        if (ioctl(sock, SIOCGIFADDR, &ifr) < 0) {
            perror("ioctl()");
            close(sock);
            return NULL;
        }

        ifi[idx].if_addr = *(struct sockaddr_in*)&ifr.ifr_addr;
        ifi[idx].if_addr6 =  *(struct sockaddr_in6*)&ifr.ifr_addr;

        if (ioctl(sock, SIOCGIFNETMASK, &ifr) < 0) {
            perror("ioctl()");
            close(sock);
            return NULL;
        }

        ifi[idx].if_netmask = ifr.ifr_addr;

         (*(struct sockaddr_in*)&ifi[idx].if_broadcast).sin_addr.s_addr = (ifi[idx].if_addr.sin_addr.s_addr) | ~((*(struct sockaddr_in*)&ifi[idx].if_netmask).sin_addr.s_addr);

        if (ioctl(sock, SIOCGIFHWADDR, &ifr) < 0) {
            perror("ioctl()");
            close(sock);
            return NULL;
        }

        ifi[idx].if_hwaddr = ifr.ifr_hwaddr;

        if (ioctl(sock, SIOCGIFFLAGS, &ifr) < 0) {
            perror("ioctl()");
            close(sock);
            return NULL;
        }

        ifi[idx].if_flags = ifr.ifr_flags;

        idx++;
        return ifi;
    }

    return NULL;
}

void show_info(struct if_info *ifi, uint8_t _flags)
{
    char buf[INET6_ADDRSTRLEN];
    char buf2[INET6_ADDRSTRLEN];
    struct in6_addr in6;
    if (_flags&IF_BRIEF) {
        for (int i=0; i < idx; i++) {
            printf("%s: inet %s ether %s\n", ifi[i].if_name, inet_ntoa(ifi[i].if_addr.sin_addr),
                                             ether_ntoa((struct ether_addr*)&ifi[i].if_hwaddr.sa_data));
        }
    } else {
        for (int i=0; i < idx; i++) {
            in6 = ifi[i].if_addr6.sin6_addr;
            printf("%s: flags=%u<>\n", ifi[i].if_name, ifi[i].if_flags);
            printf("\tinet %s ", inet_ntoa(ifi[i].if_addr.sin_addr));
            printf("netmask %s ", inet_ntoa((*(struct sockaddr_in*)&ifi[i].if_netmask).sin_addr));
            printf("broadcast %s\n", inet_ntoa((*(struct sockaddr_in*)&ifi[i].if_broadcast).sin_addr));
            printf("\tether %s\n", ether_ntoa((struct ether_addr*)&ifi[i].if_hwaddr.sa_data));
        }
    }
}