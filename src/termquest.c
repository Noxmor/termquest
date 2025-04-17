#include "termquest.h"
#include "interface.h"
#include "mod_list.h"

#include <string.h>
#include <stdlib.h>
#include <termbox.h>

static int running = 0;

static void termquest_init(void)
{
    tb_init();
}

static void termquest_shutdown(void)
{
    tb_shutdown();
}

static void render_string(i32 x, i32 y, const char* str)
{
    for (usize i = 0; i < strlen(str); ++i)
    {
        tb_change_cell(x + i, y, str[i], TB_WHITE, TB_BLACK);
    }
}

static void interface_render(const Interface* inf)
{
    for (usize i = 0; i < inf->commands_count; ++i)
    {
        Command* cmd = &inf->commands[i];
        render_string(0, i, cmd->display_key);
    }
}

void termquest_run(void)
{
    // TODO: Refactor
    ModList mod_list;

    mod_list_init(&mod_list);
    mod_list_scan(&mod_list);
    mod_list_load(&mod_list);

    termquest_init();

    Interface* main_menu = interface_create("main-menu");
    main_menu->commands_count = 1;
    main_menu->commands = malloc(sizeof(Command) * main_menu->commands_count);
    main_menu->commands[0].display_key = "CMD_QUIT";

    Interface* active_inf = main_menu;
    struct tb_event event;
    running = 1;

    while (running)
    {
        tb_clear();
        interface_render(active_inf);
        tb_present();

        switch (tb_poll_event(&event))
        {
            case TB_EVENT_KEY:
            {
                if (event.key == TB_KEY_ESC)
                {
                    termquest_quit();
                }

                break;
            }

            default: break;
        }
    }

    termquest_shutdown();
}

void termquest_quit(void)
{
    running = 0;
}
