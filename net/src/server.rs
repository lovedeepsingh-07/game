use crate::{constants, debug, packet};
use renet;
use renet_netcode as netcode;
use std::sync::{Mutex, OnceLock};

#[allow(non_camel_case_types)]
struct Server_State {
    server: renet::RenetServer,
    transport: netcode::NetcodeServerTransport,
}
static SERVER_STATE: OnceLock<Mutex<Server_State>> = OnceLock::new();

pub fn server_setup(port: u16) {
    let server = renet::RenetServer::new(renet::ConnectionConfig::default());

    let current_time = std::time::SystemTime::now()
        .duration_since(std::time::SystemTime::UNIX_EPOCH)
        .unwrap();
    let server_address: std::net::SocketAddr = format!("127.0.0.1:{}", port).parse().unwrap();

    let server_config = netcode::ServerConfig {
        current_time,
        max_clients: 64,
        protocol_id: constants::PROTOCOL_ID,
        public_addresses: vec![server_address],
        authentication: netcode::ServerAuthentication::Unsecure,
    };

    let socket: std::net::UdpSocket = std::net::UdpSocket::bind(server_address).unwrap();
    let transport = netcode::NetcodeServerTransport::new(server_config, socket).unwrap();

    SERVER_STATE
        .set(Mutex::new(Server_State { server, transport }))
        .ok();
}
pub fn server_update(delta_time_ms: u64) {
    let delta_time = std::time::Duration::from_millis(delta_time_ms);
    let mut mutex_gaurd = SERVER_STATE.get().unwrap().lock().unwrap();
    let Server_State { server, transport } = &mut *mutex_gaurd;

    server.update(delta_time);
    transport.update(delta_time, server).unwrap();

    while let Some(event) = server.get_event() {
        match event {
            renet::ServerEvent::ClientConnected { client_id } => {
                if let Some(user_data) = transport.user_data(client_id){
                    let username = crate::from_user_data(&user_data);
                    debug::info(format!("Client {} connected as {}", client_id, username).as_str());
                } else {
                    debug::info(format!("Client {} connected", client_id).as_str());
                }
            }
            renet::ServerEvent::ClientDisconnected { client_id, reason } => {
                debug::info(
                    format!(
                        "Client {} disconnected, for reason: {:#?}",
                        client_id, reason
                    )
                    .as_str(),
                );
            }
        }
    }

    for client_id in server.clients_id() {
        while let Some(message) =
            server.receive_message(client_id, renet::DefaultChannel::ReliableOrdered)
        {
            // decode message from flatbuffers's bytes
            let packet = packet::root_as_packet(&message).unwrap();
            match packet.data_type() {
                packet::Packet_Data::Message => {
                     if let Some(data) = packet.data_as_message(){
                         let username = match data.username() {
                             Some(out) => out,
                             None => {
                                 "invalid_username"
                             }
                         };
                         let message = match data.body() {
                             Some(out) => out,
                             None => {
                                 "invalid_message"
                             }
                         };
                        debug::info(format!("({}, {}): {:#?}", username, client_id, message).as_str());
                     }
                },
                _ => {
                    debug::error("invalid packet data type");
                },
            }
            server.broadcast_message(
                renet::DefaultChannel::ReliableOrdered,
                message,
            );
        }
    }
    // if server.clients_id().len() > 0 {
    //     server.broadcast_message(
    //         renet::DefaultChannel::ReliableOrdered,
    //         "a message from the server",
    //     );
    // }
}
pub fn server_send_packets() {
    let mut mutex_gaurd = SERVER_STATE.get().unwrap().lock().unwrap();
    let Server_State { server, transport } = &mut *mutex_gaurd;
    transport.send_packets(server);
}
