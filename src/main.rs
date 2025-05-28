mod command;
mod interface;
mod termquest;

use termquest::*;

use log::*;
use simplelog::*;
use std::fs::File;

fn init_logger() -> Result<(), Box<dyn std::error::Error>> {
    let log_file = File::create("termquest.log")?;

    WriteLogger::init(LevelFilter::Info, Config::default(), log_file)?;

    Ok(())
}

fn main() {
    if let Err(e) = init_logger() {
        eprintln!("Failed to initialize logger: {}", e);
        return;
    }

    let mut termquest = TermQuest::new();
    if let Err(e) = termquest.run() {
        error!("TermQuest failed while running: {}", e);
    }
}
