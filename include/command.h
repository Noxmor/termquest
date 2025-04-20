#ifndef TQ_COMMAND_H
#define TQ_COMMAND_H

typedef struct Command
{
    const char* name;
    const char* id;
} Command;

void command_init(Command* cmd, const char* name, const char* id);

#endif
