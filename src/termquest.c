#include "termquest.h"

#include <termbox.h>

static int running = 0;

static void init(void)
{
    tb_init();
}

static void shutdown(void)
{
    tb_shutdown();
}

void termquest_run(void)
{
    init();

    struct tb_event event;
    running = 1;

    while (running)
    {
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

    shutdown();
}

void termquest_quit(void)
{
    running = 0;
}
