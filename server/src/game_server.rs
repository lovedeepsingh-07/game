use crate::{command, error, state};
use boron_common::{constants, debug, utils, packet, flatbuffers};
use renet_netcode as netcode;
use std::sync::{Arc, RwLock};

pub fn run(state: Arc<RwLock<state::ServerState>>) -> Result<(), error::Error> {
    let server_state = state.read()?;
    let mut game_state = state::GameState::default();

    debug::debug(&format!(
        "started game server on {}",
        constants::GAME_SERVER_ADDRESS
    ));

    let mut server = renet::RenetServer::new(renet::ConnectionConfig::default());
    let server_address: std::net::SocketAddr = constants::GAME_SERVER_ADDRESS.parse()?;
    let socket: std::net::UdpSocket = std::net::UdpSocket::bind(server_address)?;

    let server_config = netcode::ServerConfig {
        current_time: std::time::SystemTime::now()
            .duration_since(std::time::SystemTime::UNIX_EPOCH)?,
        max_clients: constants::MAX_CLIENTS,
        protocol_id: constants::PROTOCOL_ID,
        public_addresses: vec![server_address],
        authentication: netcode::ServerAuthentication::Secure {
            private_key: utils::str_to_32_bytes(constants::PRIVATE_KEY),
        },
    };

    let mut transport = netcode::NetcodeServerTransport::new(server_config, socket)?;

    loop {
        match server_state.receiver.try_recv() {
            Ok(cmd) => command::handle(&mut game_state, cmd)?,
            Err(crossbeam::channel::TryRecvError::Empty) => {}
            Err(crossbeam::channel::TryRecvError::Disconnected) => {
                return Err(error::Error::ChannelReceiveError(String::from(
                    "Failed to receive commands, channel is closed",
                )));
            }
        }

        server.update(constants::DELTA_TIME_MS);
        transport
            .update(constants::DELTA_TIME_MS, &mut server)
            .unwrap();

        while let Some(event) = server.get_event() {
            match event {
                renet::ServerEvent::ClientConnected { client_id } => {
                    let client_data = match game_state.client_list.get(&client_id) {
                        Some(out) => out,
                        None => {
                            return Err(error::Error::StateError("client with that client_id does not exist, i.e is not logged-in".to_string()));
                        },
                    };
                    debug::info(
                        &format!("Client ({}, {}) joined the server", &client_data.username, client_data.id).as_str(),
                    );

                    let mut builder = flatbuffers::FlatBufferBuilder::with_capacity(1024);
                    let join_username = builder.create_string(&client_data.username);
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
                renet::ServerEvent::ClientDisconnected { client_id, reason } => {
                    let client_data = match game_state.client_list.get(&client_id) {
                        Some(out) => out,
                        None => {
                            return Err(error::Error::StateError("client with that client_id does not exist, i.e is not logged-in".to_string()));
                        },
                    };

                    server_state.sender.send(command::Command::Disconnect {
                        client_id: client_data.id,
                        username: client_data.username.clone()
                    })?;

                    debug::info(
                        &format!(
                            "Client ({}, {}) left the server, for reason: {:#?}",
                            &client_data.username, client_data.id, reason
                        )
                    );
                    let mut builder = flatbuffers::FlatBufferBuilder::with_capacity(1024);
                    let leave_username = builder.create_string(&client_data.username);
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
            };
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
                        return Err(error::Error::TransportError(format!("invalid flatbuffer packet data type")));
                    }
                }
                server.broadcast_message(renet::DefaultChannel::ReliableOrdered, message);
            }
        }

        transport.send_packets(&mut server);
        std::thread::sleep(constants::DELTA_TIME_MS);
    }
}
