use crate::command::*;

pub struct Interface {
    name: String,
    commands: Vec<Command>,
    command_index: usize,
    cursor_index: usize,
}

impl Interface {
    pub fn new(name: &str) -> Self {
        Self {
            name: name.to_string(),
            commands: vec![],
            command_index: 0,
            cursor_index: 0,
        }
    }

    pub fn move_up(&mut self) {
        if self.command_index > 0 {
            self.command_index -= 1;
        }

        if self.cursor_index > 0 {
            self.cursor_index -= 1;
        }
    }

    pub fn move_down(&mut self, command_box_height: u16) {
        if self.command_index + 1 < self.commands_count() {
            self.command_index += 1;

            if self.cursor_index + 2 < command_box_height as usize {
                self.cursor_index += 1;
            }
        }
    }

    pub fn name(&self) -> &str {
        &self.name
    }

    pub fn selected_command(&self) -> &Command {
        self.commands.get(self.command_index).unwrap()
    }

    pub fn add_command(&mut self, cmd: Command) {
        self.commands.push(cmd);
    }

    pub fn get_command(&self, index: usize) -> Option<&Command> {
        self.commands.get(index)
    }

    pub fn commands_count(&self) -> usize {
        self.commands.len()
    }

    pub fn command_index(&self) -> usize {
        self.command_index
    }

    pub fn cursor_index(&self) -> usize {
        self.cursor_index
    }
}
