#include "interface.h"

#include <stdlib.h>

Interface* interface_create(const char* name, usize commands_count)
{
    Interface* inf = malloc(sizeof(Interface));

    inf->name = name;
    inf->commands_count = commands_count;
    inf->commands = malloc(sizeof(Command) * inf->commands_count);
    inf->command_index = 0;
    inf->cursor_index = 0;

    return inf;
}

Command* interface_get_command(const Interface* inf, usize index)
{
    return &inf->commands[index];
}
