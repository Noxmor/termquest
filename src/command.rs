pub struct Command {
    name: String,
    id: String,
}

impl Command {
    pub fn new(name: &str) -> Self {
        Command::with_id(name, name)
    }

    pub fn with_id(name: &str, id: &str) -> Self {
        Self {
            name: name.to_string(),
            id: id.to_string(),
        }
    }

    pub fn name(&self) -> &str {
        &self.name
    }
}
