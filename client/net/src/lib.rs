mod connect;
mod error;
mod state;
mod update;

use connect::*;
use state::*;
use update::*;
use boron_common::debug;

#[cxx::bridge(namespace = "net")]
pub mod ffi {
    extern "Rust" {
        fn setup_client(username: String) -> Result<()>;
        fn reset_client() -> Result<()>;
        fn is_client_connected() -> bool;
        fn is_client_connecting() -> bool;
        fn connect_client(delta_time_ms: u64) -> Result<()>;
        fn update_client(elta_time_ms: u64) -> Result<()>;
        fn send_packets() -> Result<()>;
    }
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
        },
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
