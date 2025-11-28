use renet;
use renet_netcode as netcode;
use std::sync::{Mutex, OnceLock};
use crate::{constants, debug};
use rand;

#[allow(non_camel_case_types)]
struct Client_State {
    client: renet::RenetClient,
    transport: netcode::NetcodeClientTransport,
    connected: bool,
    connection_start_time: Option<std::time::Instant>,
}
static CLIENT_STATE: OnceLock<Mutex<Client_State>> = OnceLock::new();

pub fn setup_client() {
    let client = renet::RenetClient::new(renet::ConnectionConfig::default());

    let current_time = std::time::SystemTime::now().duration_since(std::time::SystemTime::UNIX_EPOCH).unwrap();
    let server_address: std::net::SocketAddr = constants::SERVER_ADDRESS.parse().unwrap();

    let authentication = netcode::ClientAuthentication::Unsecure {
        server_addr: server_address,
        client_id: rand::random::<u64>(),
        user_data: None,
        protocol_id: constants::PROTOCOL_ID,
    };

    let socket = std::net::UdpSocket::bind("127.0.0.1:0").unwrap();
    let transport = netcode::NetcodeClientTransport::new(current_time, authentication, socket).unwrap();

    CLIENT_STATE.set(Mutex::new(Client_State {
        client,
        transport,
        connected: false,
        connection_start_time: None,
    })).ok();
}

pub fn is_client_connected() -> bool {
    let mut mutex_gaurd = CLIENT_STATE.get().unwrap().lock().unwrap();
    let Client_State { connected, .. } = &mut *mutex_gaurd;
    return *connected;
}

pub fn is_client_connecting() -> bool {
    let mut mutex_gaurd = CLIENT_STATE.get().unwrap().lock().unwrap();
    let Client_State { connected, connection_start_time, .. } = &mut *mutex_gaurd;
    if !*connected && !connection_start_time.is_none() {
        return true;
    }
    return false;
}

pub fn update_client(delta_time_ms: u64) {
    let delta_time = std::time::Duration::from_millis(delta_time_ms);
    let mut mutex_gaurd = CLIENT_STATE.get().unwrap().lock().unwrap();
    let Client_State { client, transport, connected, connection_start_time } = &mut *mutex_gaurd;

    let timeout = std::time::Duration::from_secs(5);

    if !*connected && connection_start_time.is_none() {
        *connection_start_time = Some(std::time::Instant::now());
    }

    if !*connected {
        if let Some(start_time) = connection_start_time {
            if start_time.elapsed() > timeout {
                *connection_start_time = None;
                debug::error("failed to connect to server, timed out");
                return;
            }
        }

        debug::debug("updating client");
        client.update(delta_time);
        debug::debug("updating transport");
        match transport.update(delta_time, client) {
            Ok(_) => {}
            Err(e) => {
                debug::error(format!("transport.update: {}", e.to_string()).as_str());
            }
        };
        debug::debug("sending packets via transport");
        match transport.send_packets(client) {
            Ok(_) => {}
            Err(e) => {
                debug::error(format!("transport.send_packets: {}", e.to_string()).as_str());
            }
        };
        debug::debug("connecting iteration done...");
        // std::thread::sleep(delta_time);

        *connected = client.is_connected();
        return;
    }

    debug::debug("updating client");
    client.update(delta_time);
    debug::debug("updating transport");
    transport.update(delta_time, client).unwrap();
    debug::debug("receiving messages");
    while let Some(message) = client.receive_message(renet::DefaultChannel::ReliableOrdered) {
        debug::info(format!("server_message: {:#?}", String::from_utf8(message.to_vec()).unwrap()).as_str());
    }
    debug::debug("sending messages");
    client.send_message(renet::DefaultChannel::ReliableOrdered, "client text");
    debug::debug("sending packets via transport");
    transport.send_packets(client).unwrap();
    debug::debug("done...");
}
