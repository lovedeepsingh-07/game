use crate::{constants, debug, error, packet};
use renet;
use renet_netcode as netcode;
use std::{
    collections::HashMap,
    sync::{Mutex, OnceLock},
};

#[allow(non_camel_case_types)]
struct Server_State {
    server: renet::RenetServer,
    transport: netcode::NetcodeServerTransport,
    connected_clients: HashMap<u64, String>,
}
static SERVER_STATE: OnceLock<Mutex<Server_State>> = OnceLock::new();

pub fn server_setup(port: u16) -> Result<(), error::Error> {
    let server = renet::RenetServer::new(renet::ConnectionConfig::default());

    let current_time =
        std::time::SystemTime::now().duration_since(std::time::SystemTime::UNIX_EPOCH)?;
    let server_address: std::net::SocketAddr = format!("127.0.0.1:{}", port).parse()?;

    let server_config = netcode::ServerConfig {
        current_time,
        max_clients: 64,
        protocol_id: constants::PROTOCOL_ID,
        public_addresses: vec![server_address],
        authentication: netcode::ServerAuthentication::Unsecure,
    };

    let socket: std::net::UdpSocket = std::net::UdpSocket::bind(server_address)?;
    let transport = netcode::NetcodeServerTransport::new(server_config, socket)?;

    match SERVER_STATE.set(Mutex::new(Server_State {
        server,
        transport,
        connected_clients: HashMap::new(),
    })) {
        Ok(_) => {}
        Err(_) => {
            return Err(error::Error::StateError(String::from(
                "SERVER_STATE is ALREADY initialized",
            )));
        }
    };

    Ok(())
}
pub fn server_update(delta_time_ms: u64) -> Result<(), error::Error> {
    let delta_time = std::time::Duration::from_millis(delta_time_ms);
    let cell_lock = match SERVER_STATE.get() {
        Some(out) => out,
        None => {
            return Err(error::Error::StateError(String::from(
                "SERVER_STATE is NOT initialized",
            )));
        },
    };
    let mut mutex_gaurd = cell_lock.lock()?;
    let Server_State {
        server,
        transport,
        connected_clients,
    } = &mut *mutex_gaurd;

    server.update(delta_time);
    transport.update(delta_time, server)?;

    while let Some(event) = server.get_event() {
        match event {
            renet::ServerEvent::ClientConnected { client_id } => {
                let mut username = client_id.to_string();

                if let Some(user_data) = transport.user_data(client_id) {
                    username = crate::from_user_data(&user_data);
                }
                debug::info(
                    format!("Client ({}, {}) joined the server", &username, client_id).as_str(),
                );

                {
                    let mut builder = flatbuffers::FlatBufferBuilder::with_capacity(1024);
                    let join_username = builder.create_string(&username);
                    let packet_data = packet::JoinEvent::create(
                        &mut builder,
                        &packet::JoinEventArgs {
                            username: Some(join_username),
                        },
                    );
                    let packet = packet::Packet::create(
                        &mut builder,
                        &packet::PacketArgs {
                            data_type: packet::Packet_Data::JoinEvent,
                            data: Some(packet_data.as_union_value()),
                        },
                    );
                    builder.finish(packet, None);
                    let buf = builder.finished_data().to_vec();
                    server.broadcast_message(renet::DefaultChannel::ReliableOrdered, buf);
                }
                connected_clients.insert(client_id, username);
            }
            renet::ServerEvent::ClientDisconnected { client_id, reason } => {
                let username = match connected_clients.remove(&client_id) {
                    Some(out) => out,
                    None => String::from("invalid_client"),
                };

                debug::info(
                    format!(
                        "Client ({}, {}) left the server, for reason: {:#?}",
                        &username, client_id, reason
                    )
                    .as_str(),
                );

                {
                    let mut builder = flatbuffers::FlatBufferBuilder::with_capacity(1024);
                    let leave_username = builder.create_string(&username);
                    let packet_data = packet::LeaveEvent::create(
                        &mut builder,
                        &packet::LeaveEventArgs {
                            username: Some(leave_username),
                        },
                    );
                    let packet = packet::Packet::create(
                        &mut builder,
                        &packet::PacketArgs {
                            data_type: packet::Packet_Data::LeaveEvent,
                            data: Some(packet_data.as_union_value()),
                        },
                    );
                    builder.finish(packet, None);
                    let buf = builder.finished_data().to_vec();
                    server.broadcast_message(renet::DefaultChannel::ReliableOrdered, buf);
                }
            }
        }
    }

    for client_id in server.clients_id() {
        while let Some(message) =
            server.receive_message(client_id, renet::DefaultChannel::ReliableOrdered)
        {
            // decode message from flatbuffers's bytes
            let packet = packet::root_as_packet(&message)?;
            match packet.data_type() {
                packet::Packet_Data::Message => {
                    if let Some(data) = packet.data_as_message() {
                        let username = match data.username() {
                            Some(out) => out,
                            None => "invalid_username",
                        };
                        let message = match data.body() {
                            Some(out) => out,
                            None => "invalid_message",
                        };
                        debug::info(
                            format!("({}, {}): {:#?}", username, client_id, message).as_str(),
                        );
                    }
                }
                _ => {
                    debug::error("invalid packet data type");
                }
            }
            server.broadcast_message(renet::DefaultChannel::ReliableOrdered, message);
        }
    }
    // if server.clients_id().len() > 0 {
    //     server.broadcast_message(
    //         renet::DefaultChannel::ReliableOrdered,
    //         "a message from the server",
    //     );
    // }
    Ok(())
}
pub fn server_send_packets() -> Result<(), error::Error> {
    let cell_lock = match SERVER_STATE.get() {
        Some(out) => out,
        None => {
            return Err(error::Error::StateError(String::from(
                "SERVER_STATE is NOT initialized",
            )));
        },
    };
    let mut mutex_gaurd = cell_lock.lock()?;
    let Server_State {
        server,
        transport,
        ..
    } = &mut *mutex_gaurd;
    transport.send_packets(server);
    Ok(())
}
