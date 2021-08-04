/**
 * Author: FallAngel
 * Copyright © 2021
 * 
 * ifconfig implementation
*/

#include "ifctl.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>

# ifndef VERSION
#   define VERSION "0.1-beta"
# endif
#define version() printf("Version %s\n", VERSION);

static uint8_t _flags = 0;

static const struct option long_options[] = {
    {"version", no_argument, NULL, 'v'},
    {"help", no_argument, NULL, 'h'},
    {"list", no_argument, &_flags, IF_LIST},
    {"info", required_argument, &_flags, IF_INFO},
    {"all", no_argument, &_flags, IF_ALL},
    {"iglb", no_argument, NULL, 'b'},
};

static __attribute__((noreturn)) void usage(void)
{
    version()
    printf("--help / -h\t - Show this help menu\n");
    printf("--version / -v\t - Show the program version\n");
    printf("--list / -l\t - List all the avaliable interfaces\n");
    printf("--info <iface>\t - Display information about one interface\n");
    printf("--all\t - Display information about all found interfaces\n");
    printf("--iglb\t - Ignore the loopback interface\n");
    exit(-EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    if (argc == 1) usage();

    struct if_list ifl;
    struct if_info ifi;

    int index = 0;
    char opt = 0;
    
    while (1) 
    {
        opt = getopt_long(argc, argv, "vhli:ab", long_options, &index);
        if (opt < 0) break;

        switch (opt)
        {
        case 0:
            break;

        case 'v':
            version()
            exit(-EXIT_FAILURE);
        
        case 'h':
            usage();
        
        case 'l':
            break;
        
        case 'i':
            break;

        case 'b':
            _flags |= IGNORE_LOOPBACK;
            break;

        default:
            break;
        }
    }
 
    if (!(_flags&IF_ALL)) {
        if (_flags&IF_LIST) {
            if (get_if_list(&ifl, _flags)) {
                for (int i=0; i < ifl.if_len; i++) {
                    printf("%s\n", ifl.if_name[i]);
                }
            }
        }
    } else {
        get_if_info(&ifi, _flags);
    }

    return 0;    
}