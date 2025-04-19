local on_command_executed  = function(event)
    if event.name == "CMD_OPTIONS" then
        game.push_interface("options-menu")
    end
    if event.name == "CMD_BACK" then
        game.pop_interface()
    end
    if event.name == "CMD_QUIT" then
        game.quit()
    end
end

script.on_event("on_command_executed", on_command_executed)
