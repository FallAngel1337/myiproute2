/**
 * Author: FallAngel
 * Copyright © 2021
 * 
 * ifconfig implementation
*/

#include "ifctl.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include <sys/socket.h>
#include <stdint.h>
#include <stdbool.h>

static const struct option long_options[] = {
    {"verbose", no_argument, NULL, 'v'},
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 's'}
};

static uint8_t _flags = 0;

static __attribute__((noreturn)) void usage(void)
{
    // show help here
    exit(-EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    if (argc == 1) usage();
    return 0;    
}