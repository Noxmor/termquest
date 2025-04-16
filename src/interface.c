#include "interface.h"

void interface_init(Interface* inf, const char* name)
{
    inf->name = name;
    inf->commands = NULL;
    inf->commands_count = 0;
}
