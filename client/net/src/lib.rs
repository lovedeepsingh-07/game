mod connect;
mod error;
mod state;
mod update;
mod message;

use boron_common::debug;
use connect::*;
use state::*;
use update::*;
use message::*;

#[cxx::bridge(namespace = "net")]
pub mod ffi {
    struct MessageBytes {
        pub data: Vec<u8>,
    }
    extern "Rust" {
        fn setup_client(username: String) -> Result<()>;
        fn reset_client() -> Result<()>;
        fn disconnect_client() -> Result<()>;
        fn get_client_username() -> String;
        fn is_client_connected() -> bool;
        fn is_client_connecting() -> bool;
        fn connect_client(delta_time_ms: u64) -> Result<()>;
        fn update_client(delta_time_ms: u64) -> Result<()>;
        fn poll_packets() -> Result<Vec<MessageBytes>>;
        fn send_message(input: String) -> Result<()>;
        fn send_packets() -> Result<()>;
    }
}

pub fn disconnect_client() -> Result<(), error::Error> {
    let mut write_guard = state::CLIENT_STATE.write()?;
    let client_state = write_guard
        .as_mut()
        .ok_or(error::Error::StateNotInitializedError)?;

    client_state.client.disconnect();
    Ok(())
}

pub fn get_client_username() -> String {
    let client_state = match state::CLIENT_STATE.read() {
        Ok(out) => out,
        Err(e) => {
            debug::error(&e.to_string());
            return String::from("invalid_username");
        }
    };
    match *client_state {
        Some(ref client_state) => {
            return client_state.username.clone();
        }
        None => {
            return String::from("invalid_username");
        }
    };
}

pub fn is_client_connected() -> bool {
    let client_state = match state::CLIENT_STATE.read() {
        Ok(out) => out,
        Err(e) => {
            debug::error(&e.to_string());
            return false;
        }
    };
    match *client_state {
        Some(ref client_state) => {
            return client_state.connected;
        }
        None => {
            return false;
        }
    };
}

pub fn is_client_connecting() -> bool {
    let client_state = match state::CLIENT_STATE.read() {
        Ok(out) => out,
        Err(e) => {
            debug::error(&e.to_string());
            return false;
        }
    };
    match *client_state {
        Some(ref client_state) => {
            return !client_state.connected && !client_state.connection_start_time.is_none();
        }
        None => {
            return false;
        }
    };
}
