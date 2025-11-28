pub mod constants;
pub mod client;
pub mod server;
pub mod debug;

use client::*;
use server::*;

#[cxx::bridge(namespace = "net")]
mod ffi {
    extern "Rust" {
        fn setup_server();
        fn update_server(delta_time_ms: u64);

        fn setup_client();
        fn update_client(delta_time_ms: u64);
        fn is_client_connected() -> bool;
        fn is_client_connecting() -> bool;
    }
}
