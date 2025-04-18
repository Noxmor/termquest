#ifndef TQ_MOD_H
#define TQ_MOD_H

#include "types.h"

typedef struct Mod
{
    const char* name;
    const char* filepath;
    b8 active;
} Mod;

void mod_init(Mod* mod, const char* filepath);

void mod_load(Mod* mod);

void mod_execute_command(Mod* mod, const char* cmd_name);

#endif
