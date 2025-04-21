#ifndef TQ_MOD_H
#define TQ_MOD_H

#include "types.h"
#include "list.h"
#include "command.h"
#include "interface.h"

typedef struct lua_State lua_State;

typedef struct Mod
{
    const char* name;
    const char* filepath;
    lua_State* data;
    lua_State* script;
    List event_listeners;
    b8 active;
} Mod;

void mod_init(Mod* mod, const char* filepath);

void mod_load(Mod* mod);

void mod_execute_command(Mod* mod, const Command* cmd);

void mod_render_interface(Mod* mod, const Interface* inf);

#endif
