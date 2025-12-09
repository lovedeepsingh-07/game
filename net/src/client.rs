use crate::{constants, debug};
use rand;
use renet;
use renet_netcode as netcode;
use std::sync::{Mutex, OnceLock};

#[allow(non_camel_case_types)]
struct Client_State {
    client: renet::RenetClient,
    transport: netcode::NetcodeClientTransport,
    connected: bool,
    connection_start_time: Option<std::time::Instant>,
}
static CLIENT_STATE: OnceLock<Mutex<Client_State>> = OnceLock::new();

pub fn client_setup(username: String, address: String) {
    let client = renet::RenetClient::new(renet::ConnectionConfig::default());

    let current_time = std::time::SystemTime::now()
        .duration_since(std::time::SystemTime::UNIX_EPOCH)
        .unwrap();
    let server_address: std::net::SocketAddr = address.parse().unwrap();

    let authentication = netcode::ClientAuthentication::Unsecure {
        server_addr: server_address,
        client_id: rand::random::<u64>(),
        user_data: Some(crate::to_netcode_user_data(username.as_str())),
        protocol_id: constants::PROTOCOL_ID,
    };

    let socket = std::net::UdpSocket::bind("127.0.0.1:0").unwrap();
    let transport =
        netcode::NetcodeClientTransport::new(current_time, authentication, socket).unwrap();

    CLIENT_STATE
        .set(Mutex::new(Client_State {
            client,
            transport,
            connected: false,
            connection_start_time: None,
        }))
        .ok();
}

pub fn client_is_connected() -> bool {
    let mut mutex_gaurd = CLIENT_STATE.get().unwrap().lock().unwrap();
    let Client_State { connected, .. } = &mut *mutex_gaurd;
    return *connected;
}

pub fn client_is_connecting() -> bool {
    let mut mutex_gaurd = match CLIENT_STATE.get(){
        Some(out) => {out},
        None => {
            return false;
        }
    }.lock().unwrap();
    let Client_State {
        connected,
        connection_start_time,
        ..
    } = &mut *mutex_gaurd;
    if !*connected && !connection_start_time.is_none() {
        return true;
    }
    return false;
}

pub fn client_connect(delta_time_ms: u64) {
    let delta_time = std::time::Duration::from_millis(delta_time_ms);
    let mut mutex_gaurd = CLIENT_STATE.get().unwrap().lock().unwrap();
    let Client_State {
        client,
        transport,
        connected,
        connection_start_time,
    } = &mut *mutex_gaurd;

    if !*connected && connection_start_time.is_none() {
        *connection_start_time = Some(std::time::Instant::now());
    }

    if !*connected {
        if let Some(start_time) = connection_start_time {
            if start_time.elapsed() > constants::CLIENT_CONNECT_TIMEOUT {
                *connection_start_time = None;
                debug::error("failed to connect to server, timed out");
                return;
            }
        }

        client.update(delta_time);
        match transport.update(delta_time, client) {
            Ok(_) => {}
            Err(e) => {
                debug::error(format!("transport.update: {}", e.to_string()).as_str());
            }
        };

        *connected = client.is_connected();
        return;
    }
}

pub fn client_update(delta_time_ms: u64) {
    let delta_time = std::time::Duration::from_millis(delta_time_ms);

    let mut mutex_gaurd = CLIENT_STATE.get().unwrap().lock().unwrap();
    let Client_State {
        client, transport, ..
    } = &mut *mutex_gaurd;

    client.update(delta_time);
    transport.update(delta_time, client).unwrap();
}

pub fn client_poll_messages() -> Vec<String> {
    let mut mutex_gaurd = CLIENT_STATE.get().unwrap().lock().unwrap();
    let Client_State { client, .. } = &mut *mutex_gaurd;

    let mut output: Vec<String> = Vec::new();
    while let Some(message) = client.receive_message(renet::DefaultChannel::ReliableOrdered) {
        output.push(String::from_utf8(message.to_vec()).unwrap());
    }
    return output;
}

pub fn client_send_message(input: String) {
    let mut mutex_gaurd = CLIENT_STATE.get().unwrap().lock().unwrap();
    let Client_State { client, .. } = &mut *mutex_gaurd;

    client.send_message(renet::DefaultChannel::ReliableOrdered, input);
}

pub fn client_send_packets() {
    let mut mutex_gaurd = CLIENT_STATE.get().unwrap().lock().unwrap();
    let Client_State {
        client, transport, ..
    } = &mut *mutex_gaurd;
    transport.send_packets(client).unwrap();
}
