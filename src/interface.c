#include "interface.h"

#include <stdlib.h>

Interface* interface_create(const char* name)
{
    Interface* inf = malloc(sizeof(Interface));

    inf->name = name;
    inf->commands = NULL;
    inf->commands_count = 0;

    return inf;
}
