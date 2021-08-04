#include "ifctl.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/socket.h>

// return the name of founding interfaces e.g.: eth0
struct if_list* get_if_list(struct if_list *ifl, uint8_t _flags)
{
    return NULL;
}

void if_list_free(struct if_list *ifl)
{

}

// return information about the interface
struct if_info* get_if_info(struct if_info *ifi, uint8_t _flags)
{
    return NULL;
}