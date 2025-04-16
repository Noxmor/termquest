#ifndef TQ_INTERFACE_H
#define TQ_INTERFACE_H

#include "types.h"
#include "command.h"

typedef struct Interface
{
    Command* commands;
    usize commands_count;
} Interface;

#endif
