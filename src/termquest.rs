use std::fs;
use std::path::Path;
use std::io;

use log::*;

use crate::command::*;
use crate::interface::*;

use std::io::{Stdout, stdout, Write};
use crossterm::{
    ExecutableCommand,
    cursor,
    event::*,
    terminal::*,
};

pub struct TermQuest {
    running: bool,
    terminal: Stdout,
    width: u16,
    height: u16,
    margin_x: u16,
    margin_y: u16,
    command_box_height: u16,
    inf_stack: Vec<Box<Interface>>,
}

impl TermQuest {
    pub fn new() -> Self {
        Self {
            running: false,
            terminal: stdout(),
            width: 0,
            height: 0,
            margin_x: 0,
            margin_y: 0,
            command_box_height: 8,
            inf_stack: Vec::new(),
        }
    }

    pub fn run(&mut self) -> io::Result<()> {
        self.running = true;

        let (width, height) = size()?;
        self.width = width;
        self.height = height;

        enable_raw_mode()?;

        self.terminal.execute(cursor::Hide)?;

        let mut inf = Interface::new("Main Menu");
        inf.add_command(Command::new("foo1"));
        inf.add_command(Command::new("foo2"));
        inf.add_command(Command::new("foo3"));
        inf.add_command(Command::new("foo4"));
        inf.add_command(Command::new("foo5"));
        inf.add_command(Command::new("foo6"));
        inf.add_command(Command::new("foo7"));
        inf.add_command(Command::new("foo8"));
        inf.add_command(Command::new("foo9"));

        self.push_interface(Box::new(inf));

        info!("TermQuest running...");
        self.load_all_mods()?;

        loop {
            if !self.running {
                break;
            }

            self.update();
            self.render()?;

            let event = read()?;

            let command_box_height = self.command_box_height;

            if let Event::Key(key_event) = event {
                match key_event.code {
                    KeyCode::Esc => self.close(),
                    KeyCode::Enter => {

                    },
                    KeyCode::Up => self.active_interface().move_up(),
                    KeyCode::Down => self.active_interface().move_down(command_box_height),
                    _ => {}
                }
            }
        }

        self.terminal.execute(cursor::Show)?;

        Ok(())
    }

    fn update(&mut self) {

    }

    fn render(&mut self) -> io::Result<()> {
        let terminal = &mut self.terminal;
        terminal.execute(Clear(ClearType::All))?;
        terminal.execute(cursor::MoveTo(0, 0))?;

        let inf = self.inf_stack.last().unwrap();

        let width = self.width - self.margin_x * 2;
        let mut y = self.margin_y;

        terminal.execute(cursor::MoveTo(self.margin_x, y))?;
        print!("┌");
        terminal.execute(cursor::MoveTo(self.margin_x + width - 1, y))?;
        print!("┐");

        for i in 2..width {
            terminal.execute(cursor::MoveTo(self.margin_x + i - 1, y))?;
            print!("─");
        }

        y += 1;

        terminal.execute(cursor::MoveTo(self.margin_x, y))?;
        print!("│");
        terminal.execute(cursor::MoveTo(self.margin_x + width - 1, y))?;
        print!("│");

        let name_offset = width / 2 - inf.name().len() as u16 / 2;

        terminal.execute(cursor::MoveTo(self.margin_x + name_offset, y))?;
        print!("{}", inf.name());

        y += 1;

        terminal.execute(cursor::MoveTo(self.margin_x, y))?;
        print!("├");
        terminal.execute(cursor::MoveTo(self.margin_x + width - 1, y))?;
        print!("┤");

        for i in 2..width {
            terminal.execute(cursor::MoveTo(self.margin_x + i - 1, y))?;
            print!("─");
        }

        while {
            y += 1;
            y < self.height - self.margin_y - 1 - self.command_box_height
        } {
            terminal.execute(cursor::MoveTo(self.margin_x, y))?;
            print!("│");
            terminal.execute(cursor::MoveTo(self.margin_x + width - 1, y))?;
            print!("│");
        }

        terminal.execute(cursor::MoveTo(self.margin_x, y))?;
        print!("├");
        terminal.execute(cursor::MoveTo(self.margin_x + width - 1, y))?;
        print!("┤");

        for i in 2..width {
            terminal.execute(cursor::MoveTo(self.margin_x + i - 1, y))?;
            print!("─");
        }

        y += 1;

        let offset = inf.command_index() as i32 - inf.cursor_index() as i32;

        for i in 0..self.command_box_height {
            terminal.execute(cursor::MoveTo(self.margin_x, y + i))?;
            print!("│");
            terminal.execute(cursor::MoveTo(self.margin_x + width - 1, y + i))?;
            print!("│");

            if offset + i as i32 >= 0 && ((offset + i as i32) as usize) < inf.commands_count() {
                terminal.execute(cursor::MoveTo(self.margin_x + 4, y + i))?;
                match inf.get_command((offset + i as i32) as usize) {
                    Some(cmd) => print!("{}", cmd.name()),
                    None => print!("")
                }
            }
        }

        terminal.execute(cursor::MoveTo(self.margin_x + 2, y + inf.cursor_index() as u16))?;
        print!("> ");

        y = self.height - self.margin_y;
        terminal.execute(cursor::MoveTo(self.margin_x, y))?;
        print!("└");
        terminal.execute(cursor::MoveTo(self.margin_x + width - 1, y))?;
        print!("┘");

        for i in 2..width {
            terminal.execute(cursor::MoveTo(self.margin_x + i - 1, y))?;
            print!("─");
        }

        terminal.flush()?;

        Ok(())
    }

    fn active_interface(&mut self) -> &mut Interface {
        &mut **self.inf_stack.last_mut().unwrap()
    }

    fn push_interface(&mut self, inf: Box<Interface>) {
        self.inf_stack.push(inf);
    }

    fn pop_interface(&mut self) -> Box<Interface> {
        self.inf_stack.pop().unwrap()
    }

    fn close(&mut self) {
        self.running = false;
        info!("TermQuest closing...");
    }

    fn load_all_mods(&self) -> io::Result<()> {
        let mods_path = Path::new("mods");

        if !mods_path.exists() {
            return Ok(());
        }

        let entries = fs::read_dir(mods_path)?;

        for entry in entries {
            let entry = entry?;
            let path = entry.path();

            if path.is_dir() {
                if let Some(mod_name) = path.file_name().and_then(|s| s.to_str()) {
                    self.load_mod(mod_name)?;
                }
            }
        }

        Ok(())
    }

    fn load_mod(&self, name: &str) -> io::Result<()> {
        println!("Loading mod {}", name);

        // TODO: Load mod

        Ok(())
    }
}
