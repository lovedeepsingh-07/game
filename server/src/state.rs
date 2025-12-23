use crate::command;
use std::collections::{HashMap, HashSet};

#[derive(Debug)]
pub struct ServerState {
    pub sender: crossbeam::channel::Sender<command::Command>,
    pub receiver: crossbeam::channel::Receiver<command::Command>,
}

#[derive(Debug, Default)]
pub struct ClientData {
    pub id: u64,
    pub username: String,
}
#[derive(Debug, Default)]
pub struct GameState {
    pub client_list: HashMap<u64, ClientData>,
    pub usernames_in_use: HashSet<String>,
}
