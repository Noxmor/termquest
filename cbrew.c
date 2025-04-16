#define CBREW_IMPLEMENTATION
#define CBREW_ENABLE_CONSOLE_COLORS
#include <cbrew/cbrew.h>

int main(void)
{
    CbrewProject* project = CBREW_PRJ_NEW("termquest", CBREW_PROJECT_TYPE_APP);

    CBREW_PRJ_FILES(project, "./src/**");
    CBREW_PRJ_INCLUDE_DIR(project, "./include/");
    CBREW_PRJ_LINK(project, "termbox");
    CBREW_PRJ_LINK(project, "lua5.4");

    CbrewConfig* debug_config = CBREW_CFG_NEW(project, "Debug", "bin/termquest-debug", "bin-int/termquest-debug");

    CBREW_CFG_DEFINE(debug_config, "TQ_DEBUG");

    CbrewConfig* release_config = CBREW_CFG_NEW(project, "Release", "bin/termquest-release", "bin-int/termquest-release");

    CBREW_CFG_DEFINE(release_config, "TQ_RELEASE");

    cbrew_build();
}
