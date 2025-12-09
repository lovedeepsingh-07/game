pub mod client;
pub mod constants;
pub mod debug;
pub mod server;

use client::*;
use server::*;
use renet_netcode as netcode;

// NOTE: import the generated flatbuffer code by flatc
#[allow(dead_code, unused_imports, unsafe_code, unsafe_op_in_unsafe_fn, non_camel_case_types)]
#[path = "./generated/packet_.rs"]
mod packet_generated;
pub use packet_generated::packet;

#[cxx::bridge(namespace = "net")]
mod ffi {
    #[namespace = "net::server"]
    extern "Rust" {
        #[rust_name = "server_setup"]
        fn setup(port: u16);
        #[rust_name = "server_update"]
        fn update(delta_time_ms: u64);
        #[rust_name = "server_send_packets"]
        fn send_packets();
    }

    #[namespace = "net::client"]
    extern "Rust" {
        #[rust_name = "client_setup"]
        fn setup(username: String, address: String);
        #[rust_name = "client_is_connected"]
        fn is_connected() -> bool;
        #[rust_name = "client_is_connecting"]
        fn is_connecting() -> bool;
        #[rust_name = "client_connect"]
        fn connect(delta_time_ms: u64);
        #[rust_name = "client_update"]
        fn update(delta_time_ms: u64);
        #[rust_name = "client_poll_messages"]
        fn poll_messages() -> Vec<String>;
        #[rust_name = "client_send_message"]
        fn send_message(input: String);
        #[rust_name = "client_send_packets"]
        fn send_packets();
    }
}

pub fn to_netcode_user_data(input: &str) -> [u8; netcode::NETCODE_USER_DATA_BYTES] {
    let mut user_data = [0u8; netcode::NETCODE_USER_DATA_BYTES];
    if input.len() > netcode::NETCODE_USER_DATA_BYTES - 8 {
        panic!("Username is too big");
    }
    user_data[0] = input.len() as u8;
    user_data[1..input.len() + 1].copy_from_slice(input.as_bytes());

    user_data
}

pub fn from_user_data(user_data: &[u8; netcode::NETCODE_USER_DATA_BYTES]) -> String {
    let mut len = user_data[0] as usize;
    len = len.min(netcode::NETCODE_USER_DATA_BYTES - 1);
    let data = user_data[1..len + 1].to_vec();
    let output = String::from_utf8(data).unwrap_or("unknown".to_string());
    output
}
