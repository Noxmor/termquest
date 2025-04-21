#include "mod.h"
#include "termquest.h"
#include "prototype_registry.h"
#include "event_listener.h"

#include <stdlib.h>
#include <string.h>

#include <lua5.4/lualib.h>
#include <lua5.4/lua.h>
#include <lua5.4/lauxlib.h>

static void mod_register_interface(Mod* mod, lua_State* L)
{
    lua_getfield(L, -2, "name");
    const char* internal_name = luaL_checkstring(L, -1);
    char* name = malloc(sizeof(char) * (strlen(mod->name) + 1 + strlen(internal_name) + 1));
    sprintf(name, "%s:%s", mod->name, internal_name);
    lua_pop(L, 1);

    lua_getfield(L, -2, "commands");
    lua_len(L, -1);
    usize commands_count = lua_tointeger(L, -1);
    lua_pop(L, 2);

    Interface* inf = interface_create(name, commands_count);
    interface_registry_insert(inf);

    lua_getfield(L, -2, "commands");
    lua_pushnil(L);
    usize i = 0;
    while (lua_next(L, -2) != 0)
    {
        Command* cmd = interface_get_command(inf, i++);

        lua_getfield(L, -1, "name");
        const char* name = strdup(luaL_checkstring(L, -1));
        lua_pop(L, 1);

        lua_getfield(L, -1, "id");
        const char* id = NULL;
        if (!lua_isnil(L, -1))
        {
            id = strdup(luaL_checkstring(L, -1));
        }
        else
        {
            id = strdup(name);
        }

        command_init(cmd, name, id);

        lua_pop(L, 1);

        lua_pop(L, 1);
    }
    lua_pop(L, 1);

    lua_getfield(L, -2, "active");

    if (lua_isboolean(L, -1) && lua_toboolean(L, -1))
    {
        termquest_push_interface(inf);
    }

    lua_pop(L, 1);
}

static int data_extend(lua_State* L)
{
    lua_getfield(L, LUA_REGISTRYINDEX, "mod");
    Mod* mod = (Mod*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    if (!lua_istable(L, 2))
    {
        return luaL_error(L, "data:extend() requires a table");
    }

    lua_pushnil(L);
    while (lua_next(L, -2) != 0)
    {
        lua_getfield(L, -1, "type");
        const char* type = luaL_checkstring(L, -1);

        if (strcmp(type, "interface") == 0)
        {
            mod_register_interface(mod, L);
        }

        lua_pop(L, 1);

        lua_pop(L, 1);
    }

    return 0;
}

static void mod_register_event_listener(Mod* mod, const char* event_name, int callback)
{
    EventListener* listener = event_listener_create(event_name, callback);
    list_add(&mod->event_listeners, listener);
}

static int script_on_event(lua_State* L)
{
    lua_getfield(L, LUA_REGISTRYINDEX, "mod");
    Mod* mod = (Mod*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    int args_count = lua_gettop(L);
    if (args_count != 2)
    {
        return luaL_error(L, "Expected 2 arguments (string eventName, function callback), got %d", args_count);
    }

    const char *event_name = luaL_checkstring(L, 1);
    luaL_checktype(L, 2, LUA_TFUNCTION);

    int lua_func_ref = luaL_ref(L, LUA_REGISTRYINDEX);

    if (lua_func_ref == LUA_REFNIL)
    {
         return luaL_error(L, "Could not store nil function reference for event '%s'", event_name);
    }
    else if (lua_func_ref == LUA_NOREF)
    {
          return luaL_error(L, "Could not create registry reference for event '%s' (no reference returned)", event_name);
    }

    mod_register_event_listener(mod, event_name, lua_func_ref);

    return 0;
}

static int game_quit(lua_State* L)
{
    termquest_quit();

    return 0;
}

static int game_push_interface(lua_State* L)
{
    const char* name = luaL_checkstring(L, -1);
    if (strchr(name, ':') == NULL)
    {
        lua_getfield(L, LUA_REGISTRYINDEX, "mod");
        Mod* mod = (Mod*)lua_touserdata(L, -1);
        lua_pop(L, 1);

        char* canonical_name = malloc(sizeof(char) * (strlen(mod->name) + 1 + strlen(name) + 1));
        sprintf(canonical_name, "%s:%s", mod->name, name);
        Interface* inf = interface_registry_find(canonical_name);
        free(canonical_name);
        termquest_push_interface(inf);
    }
    else
    {
        Interface* inf = interface_registry_find(name);
        termquest_push_interface(inf);
    }

    return 0;
}

static int game_pop_interface(lua_State* L)
{
    termquest_pop_interface();

    return 0;
}

static int game_create_save_file(lua_State* L)
{
    termquest_create_save_file(luaL_checkstring(L, 1));

    return 0;
}

static int game_render(lua_State* L)
{
    i32 x = lua_tointeger(L, 1);
    i32 y = lua_tointeger(L, 2);
    const char* str = luaL_checkstring(L, 3);

    termquest_render(x, y, str);

    return 0;
}

static int game_width(lua_State* L)
{
    u32 width = termquest_width();
    lua_pushinteger(L, width);

    return 1;
}

static int game_height(lua_State* L)
{
    u32 height = termquest_height();
    lua_pushinteger(L, height);

    return 1;
}

void mod_init(Mod* mod, const char* filepath)
{
    mod->filepath = filepath;
    mod->name = strrchr(mod->filepath, '/');
    if (mod->name != NULL)
    {
        ++mod->name;
    }
    else
    {
        mod->name = mod->filepath;
    }

    mod->data = luaL_newstate();
    mod->script = luaL_newstate();

    list_init(&mod->event_listeners);

    mod->active = TQ_FALSE;
}

void mod_load(Mod* mod)
{
    lua_State* L = mod->data;
    luaL_openlibs(L);

    lua_newtable(L);
    lua_pushcfunction(L, &data_extend);
    lua_setfield(L, -2, "extend");
    lua_setglobal(L, "data");

    lua_pushlightuserdata(L, mod);
    lua_setfield(L, LUA_REGISTRYINDEX, "mod");

    char buffer[PATH_MAX];
    sprintf(buffer, "%s/data.lua", mod->filepath);

    if (luaL_dofile(L, buffer) != LUA_OK)
    {
        fprintf(stderr, "Error loading mod: %s\n", lua_tostring(L, -1));
        lua_close(L);
    }

    L = mod->script;
    luaL_openlibs(L);
    lua_newtable(L);
    lua_pushcfunction(L, &script_on_event);
    lua_setfield(L, -2, "on_event");
    lua_setglobal(L, "script");

    lua_newtable(L);
    lua_pushcfunction(L, &game_quit);
    lua_setfield(L, -2, "quit");
    lua_pushcfunction(L, &game_push_interface);
    lua_setfield(L, -2, "push_interface");
    lua_pushcfunction(L, &game_pop_interface);
    lua_setfield(L, -2, "pop_interface");
    lua_pushcfunction(L, &game_render);
    lua_setfield(L, -2, "render");
    lua_pushcfunction(L, &game_width);
    lua_setfield(L, -2, "width");
    lua_pushcfunction(L, &game_height);
    lua_setfield(L, -2, "height");
    lua_setglobal(L, "game");

    lua_pushlightuserdata(L, mod);
    lua_setfield(L, LUA_REGISTRYINDEX, "mod");

    sprintf(buffer, "%s/script.lua", mod->filepath);

    if (luaL_dofile(L, buffer) != LUA_OK)
    {
        fprintf(stderr, "Error loading mod: %s\n", lua_tostring(L, -1));
        lua_close(L);
    }
}

void mod_execute_command(Mod* mod, const Command* cmd)
{
    for (usize i = 0; i < list_size(&mod->event_listeners); ++i)
    {
        EventListener* listener = list_get(&mod->event_listeners, i);
        if (strcmp(listener->event_name, "on_command_executed") == 0)
        {
            lua_State* L = mod->script;
            lua_rawgeti(L, LUA_REGISTRYINDEX, listener->callback);
            lua_newtable(L);
            lua_pushstring(L, cmd->name);
            lua_setfield(L, -2, "name");
            lua_pushstring(L, cmd->id);
            lua_setfield(L, -2, "id");
            lua_pcall(L, 1, 0, 0);
        }
    }
}

void mod_render_interface(Mod* mod, const Interface* inf)
{
    for (usize i = 0; i < list_size(&mod->event_listeners); ++i)
    {
        EventListener* listener = list_get(&mod->event_listeners, i);
        if (strcmp(listener->event_name, "on_interface_rendered") == 0)
        {
            lua_State* L = mod->script;
            lua_rawgeti(L, LUA_REGISTRYINDEX, listener->callback);
            lua_newtable(L);
            lua_pushstring(L, inf->name);
            lua_setfield(L, -2, "name");
            lua_pcall(L, 1, 0, 0);
        }
    }
}
