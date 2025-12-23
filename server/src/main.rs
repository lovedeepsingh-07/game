use boron_common::debug;
use std::sync::{Arc, RwLock};

mod command;
mod connect_token;
mod error;
mod game_server;
mod state;
mod web_server;

#[tokio::main]
async fn main() {
    let (sendr, recvr) = crossbeam::channel::unbounded::<command::Command>();
    let state = Arc::new(RwLock::new(state::ServerState {
        sender: sendr,
        receiver: recvr,
    }));

    let game_server_state = state.clone();
    std::thread::spawn(|| {
        match game_server::run(game_server_state) {
            Ok(_) => {}
            Err(e) => {
                debug::error(&format!("failed to run the game server, {}", e.to_string()));
            }
        };
    });

    let web_server_state = state.clone();
    match web_server::run(web_server_state).await {
        Ok(_) => {}
        Err(e) => {
            debug::error(&format!("failed to run the web server, {}", e.to_string()));
        }
    };
}
