local on_command_executed  = function(event)
    if event.name == "CMD_NEW_GAME" then
        game.push_interface("new-game")
    end
    if event.name == "CMD_LOAD_GAME" then
        game.push_interface("load-game")
    end
    if event.name == "CMD_OPTIONS" then
        game.push_interface("options-menu")
    end
    if event.name == "CMD_SELECT_LANGUAGE" then
        game.push_interface("language-menu")
    end
    if event.name == "CMD_BACK" then
        game.pop_interface()
    end
    if event.name == "CMD_QUIT" then
        game.quit()
    end
end

local on_interface_rendered = function(event)
    if event.name == "main-menu" then
        game.render(game.width() // 2 - 13, game.height() // 2, "This is an example string.")
    end
end

script.on_event("on_command_executed", on_command_executed)
script.on_event("on_interface_rendered", on_interface_rendered)
