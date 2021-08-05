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
#include <string.h>
#include <getopt.h>

# ifndef VERSION
#   define VERSION "0.1-beta"
# endif
#define version() printf("Version %s\n", VERSION);

static uint8_t _flags = IF_INFO;

// options definitions
static const struct option gloabal_options[] = {
    {"version", no_argument, NULL, 'v'},
    {"help", no_argument, NULL, 'h'},
    {"brief", no_argument, NULL, 'b'},
    {"iglb", no_argument, NULL, 'i'},
};

/*
#define OPTS_LEN 4

static struct options {
    char *opt; /; e.g.: add 
    const struct option *opts; // e.g.: --brief or --iglb
};

static const struct option add_options = {
    {}
};

static struct options app_options[OPTS_LEN];

static void add_opt(char *opt, struct option *opts, int *index)
{
    if (*index > (OPTS_LEN-1)) return;
    app_options[*index].opt = opt;
    app_options[*index++].opts = opts;
}
*/

static __attribute__((noreturn)) void usage(void)
{
    version()
    printf("--help / -h\t - Show this help menu\n");
    printf("--version / -v\t - Show the program version\n");
    exit(-EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    struct if_info *ifi = NULL;

    int index = 0;
    char opt = 0;
    
    while (1) 
    {
        opt = getopt_long(argc, argv, "vhib", gloabal_options, &index);
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

        case 'i':
            _flags |= IF_IGLOOPBACK;
            break;

        case 'b':
            _flags |= IF_BRIEF;
            break; 

        default:
            break;
        }
    }

    char *interface = NULL;
    for (int i=1; i < argc; i++) {
        if (!strstr(argv[i], "-")) {
           interface = argv[i];
           break;
        }

    }

    if((ifi = get_if_info(ifi, interface, _flags)) == NULL) {
        fprintf(stderr, "couldn't get information about the inferfaces\n");
        return -1;
    }
    
    show_info(ifi, _flags);

    return 0;
}