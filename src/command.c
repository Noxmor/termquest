#include "command.h"

void command_init(Command* cmd, const char* name, const char* id)
{
    cmd->name = name;
    cmd->id = id;
}
