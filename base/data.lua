data:extend({
    {
        type = "interface",
        name = "main-menu",
        commands = {
            { name = "CMD_NEW_GAME" },
            { name = "CMD_LOAD_GAME" },
            { name = "CMD_OPTIONS" },
            { name = "CMD_QUIT" },
        },
        active = true
    },
    {
        type = "interface",
        name = "options-menu",
        commands = {
            { name = "CMD_SELECT_LANGUAGE" },
            { name = "CMD_BACK" },
        },
    },
    {
        type = "interface",
        name = "language-menu",
        commands = {
            { name = "CMD_BACK" },
        },
    },
    {
        type = "interface",
        name = "new-game",
        commands = {
            { name = "CMD_BACK" },
        },
    },
    {
        type = "interface",
        name = "load-game",
        commands = {
            { name = "CMD_BACK" },
        },
    },
})
