use renet;
use renet_netcode as netcode;
use std::sync::{Mutex, OnceLock};

#[allow(non_camel_case_types)]
struct Server_State {
    server: renet::RenetServer,
    transport: netcode::NetcodeServerTransport,
}
static SERVER: OnceLock<Mutex<Server_State>> = OnceLock::new();

#[allow(non_camel_case_types)]
struct Client_State {
    client: renet::RenetClient,
    transport: netcode::NetcodeClientTransport,
}
static CLIENT: OnceLock<Mutex<Client_State>> = OnceLock::new();

#[cxx::bridge(namespace = "net")]
mod ffi {
    extern "Rust" {
        fn setup_server(port: u16);
        fn update_server(delta_time_ms: u64);
        fn setup_client();
        fn update_client(delta_time_ms: u64);
    }
}

pub fn setup_server(port: u16) {
    let server_addr: std::net::SocketAddr = std::net::SocketAddr::new(
        std::net::IpAddr::V4(std::net::Ipv4Addr::new(127, 0, 0, 1)),
        port,
    );
    let socket: std::net::UdpSocket = std::net::UdpSocket::bind(server_addr).unwrap();
    let server_config = netcode::ServerConfig {
        current_time: std::time::SystemTime::now()
            .duration_since(std::time::SystemTime::UNIX_EPOCH)
            .unwrap(),
        max_clients: 64,
        protocol_id: 0,
        public_addresses: vec![server_addr],
        authentication: netcode::ServerAuthentication::Unsecure,
    };

    SERVER
        .set(Mutex::new(Server_State {
            server: renet::RenetServer::new(renet::ConnectionConfig::default()),
            transport: netcode::NetcodeServerTransport::new(server_config, socket).unwrap(),
        }))
        .ok();
}

pub fn update_server(delta_time_ms: u64) {
    let delta_time = std::time::Duration::from_millis(delta_time_ms);
    let mut mutex_gaurd = SERVER.get().unwrap().lock().unwrap();
    let Server_State { server, transport } = &mut *mutex_gaurd;

    server.update(delta_time);
    let _ = transport.update(delta_time, server).unwrap();

    while let Some(event) = server.get_event() {
        match event {
            renet::ServerEvent::ClientConnected { client_id } => {
                println!("Client {client_id} connected");
            }
            renet::ServerEvent::ClientDisconnected { client_id, reason } => {
                println!("Client {client_id} disconnected: {reason}");
            }
        }
    }
    transport.send_packets(server);
}

pub fn setup_client() {
    let server_addr: std::net::SocketAddr = std::net::SocketAddr::new(
        std::net::IpAddr::V4(std::net::Ipv4Addr::new(127, 0, 0, 1)),
        9090,
    );
    let socket = std::net::UdpSocket::bind("127.0.0.1:0").unwrap();
    let current_time = std::time::SystemTime::now()
        .duration_since(std::time::SystemTime::UNIX_EPOCH)
        .unwrap();

    let authentication = netcode::ClientAuthentication::Unsecure {
        server_addr,
        client_id: 0,
        user_data: None,
        protocol_id: 0,
    };

    CLIENT
        .set(Mutex::new(Client_State {
            client: renet::RenetClient::new(renet::ConnectionConfig::default()),
            transport: netcode::NetcodeClientTransport::new(current_time, authentication, socket)
                .unwrap(),
        }))
        .ok();
}

pub fn update_client(delta_time_ms: u64) {
    let delta_time = std::time::Duration::from_millis(delta_time_ms);
    let mut mutex_gaurd = CLIENT.get().unwrap().lock().unwrap();
    let Client_State { client, transport } = &mut *mutex_gaurd;

    client.update(delta_time);
    transport.update(delta_time, client).unwrap();

    // if client.is_connected() {
    //     while let Some(message) = client.receive_message(DefaultChannel::ReliableOrdered) {
    //     }
    //
    //     client.send_message(DefaultChannel::ReliableOrdered, "client text");
    // }

    match transport.send_packets(client) {
        Ok(_) => {}
        Err(e) => {
            println!("Rust Error: {:#?}", e.to_string());
        }
    };
}
