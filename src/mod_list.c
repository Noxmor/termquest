#include "mod_list.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>

#define TQ_MODS_DIR "mods"
#define TQ_BASE_MOD "base"

static void mod_list_add(ModList* mod_list, const char* filepath)
{
    if (mod_list->size >= mod_list->capacity)
    {
        mod_list->capacity = mod_list->capacity ? mod_list->capacity * 2 : 1;
        mod_list->mods = realloc(mod_list->mods, sizeof(Mod) * mod_list->capacity);
    }

    Mod* mod = &mod_list->mods[mod_list->size++];
    mod_init(mod, filepath);
}

void mod_list_init(ModList* mod_list)
{
    mod_list->mods = NULL;
    mod_list->size = 0;
    mod_list->capacity = 0;

    mod_list_add(mod_list, TQ_BASE_MOD);

    Mod* base_mod = &mod_list->mods[0];
    base_mod->active = TQ_TRUE;
}

void mod_list_scan(ModList* mod_list)
{
    DIR* dir = opendir(TQ_MODS_DIR);

    if (dir == NULL)
    {
        return;
    }

    char path[PATH_MAX];
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        sprintf(path, "%s/%s", TQ_MODS_DIR, entry->d_name);

        struct stat path_stat;
        if (stat(path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
        {
            mod_list_add(mod_list, path);
        }
    }

    closedir(dir);
}

void mod_list_load(ModList* mod_list)
{
    for (usize i = 0; i < mod_list->size; ++i)
    {
        Mod* mod = &mod_list->mods[i];
        if (mod->active)
        {
            mod_load(mod);
        }
    }
}

void mod_list_execute_command(ModList* mod_list, const char* cmd_name)
{
    for (usize i = 0; i < mod_list->size; ++i)
    {
        Mod* mod = &mod_list->mods[i];
        mod_execute_command(mod, cmd_name);
    }
}
