use renet;
use renet_netcode as netcode;
use std::sync::{Mutex, OnceLock};
use crate::{constants, debug};

#[allow(non_camel_case_types)]
struct Server_State {
    server: renet::RenetServer,
    transport: netcode::NetcodeServerTransport,
}
static SERVER_STATE: OnceLock<Mutex<Server_State>> = OnceLock::new();

pub fn setup_server() {
    let server = renet::RenetServer::new(renet::ConnectionConfig::default());

    let current_time = std::time::SystemTime::now().duration_since(std::time::SystemTime::UNIX_EPOCH).unwrap();
    let server_address: std::net::SocketAddr = constants::SERVER_ADDRESS.parse().unwrap();

    let server_config = netcode::ServerConfig {
        current_time,
        max_clients: 64,
        protocol_id: constants::PROTOCOL_ID,
        public_addresses: vec![server_address],
        authentication: netcode::ServerAuthentication::Unsecure,
    };

    let socket: std::net::UdpSocket = std::net::UdpSocket::bind(server_address).unwrap();
    let transport = netcode::NetcodeServerTransport::new(server_config, socket).unwrap();

    SERVER_STATE.set(Mutex::new(Server_State { server, transport })).ok();
}

pub fn update_server(delta_time_ms: u64) {
    let delta_time = std::time::Duration::from_millis(delta_time_ms);
    let mut mutex_gaurd = SERVER_STATE.get().unwrap().lock().unwrap();
    let Server_State { server, transport } = &mut *mutex_gaurd;

    loop {
        debug::debug("updating server");
        server.update(delta_time);
        debug::debug("updating transport");
        transport.update(delta_time, server).unwrap();

        debug::debug("getting server events");
        while let Some(event) = server.get_event() {
            match event {
                renet::ServerEvent::ClientConnected { client_id } => {
                    debug::info(format!("Client {} connected", client_id).as_str());
                }
                renet::ServerEvent::ClientDisconnected { client_id, reason } => {
                    debug::info(format!("Client {} disconnected, for reason: {:#?}", client_id, reason).as_str());
                }
            }
        }

        for client_id in server.clients_id() {
            debug::debug("receiving messages");
            while let Some(message) =
                server.receive_message(client_id, renet::DefaultChannel::ReliableOrdered)
            {
                debug::info(format!("client_message: {:#?}", String::from_utf8(message.to_vec()).unwrap()).as_str());
            }
        }


        if server.clients_id().len() > 0 {
            debug::debug("broadcasting messages");
            server.broadcast_message(
                renet::DefaultChannel::ReliableOrdered,
                "a message from the server",
            );
        }

        // let client_id: renet::ClientId = 0;
        // server.broadcast_message_except(
        //     client_id,
        //     renet::DefaultChannel::ReliableOrdered,
        //     "server message",
        // );
        //
        // server.send_message(client_id, renet::DefaultChannel::ReliableOrdered, "server message");

        debug::debug("sleeping for 3 seconds");
        std::thread::sleep(std::time::Duration::from_millis(3000));

        debug::debug("sending packets via transport");
        transport.send_packets(server);

        std::thread::sleep(delta_time);
    }
}
