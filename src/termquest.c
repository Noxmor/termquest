#include "termquest.h"
#include "mod_list.h"
#include "stack.h"

#include <string.h>
#include <termbox.h>

static int running = 0;

typedef struct GameState
{
    Stack inf_stack;
} GameState;

static GameState game_state;

static void termquest_init(void)
{
    tb_init();
    stack_init(&game_state.inf_stack);
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

void termquest_push_interface(Interface* inf)
{
    stack_push(&game_state.inf_stack, inf);
}

static Interface* termquest_peek_interface(void)
{
    return stack_peek(&game_state.inf_stack);
}

Interface* termquest_pop_interface(void)
{
    return stack_pop(&game_state.inf_stack);
}

void termquest_run(void)
{
    termquest_init();

    // TODO: Refactor
    ModList mod_list;

    mod_list_init(&mod_list);
    mod_list_scan(&mod_list);
    mod_list_load(&mod_list);

    struct tb_event event;
    running = 1;

    while (running)
    {
        Interface* active_inf = termquest_peek_interface();

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
