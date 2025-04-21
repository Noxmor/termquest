#ifndef TQ_MOD_LIST_H
#define TQ_MOD_LIST_H

#include "mod.h"

typedef struct ModList
{
    Mod* mods;
    usize size;
    usize capacity;
} ModList;

void mod_list_init(ModList* mod_list);

void mod_list_scan(ModList* mod_list);

void mod_list_load(ModList* mod_list);

void mod_list_execute_command(ModList* mod_list, const Command* cmd);

void mod_list_render_interface(ModList* mod_list, const Interface* inf);

#endif
