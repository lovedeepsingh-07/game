use crate::{command, error, state};
use boron_common::{constants, debug, utils};
use renet_netcode as netcode;
use std::sync::{Arc, RwLock};

pub fn run(state: Arc<RwLock<state::ServerState>>) -> Result<(), error::Error> {
    let state = state.read()?;
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
        match state.receiver.try_recv() {
            Ok(cmd) => command::handle(&mut game_state, cmd)?,
            Err(crossbeam::channel::TryRecvError::Empty) => {}
            Err(crossbeam::channel::TryRecvError::Disconnected) => {
                return Err(error::Error::ChannelReceiveError(String::from(
                    "failed to receive commands, channel is closed",
                )));
            }
        }

        server.update(constants::DELTA_TIME_MS);
        transport
            .update(constants::DELTA_TIME_MS, &mut server)
            .unwrap();

        while let Some(event) = server.get_event() {
            debug::info(&format!("server event: {:#?}", event));
        }

        for client_id in server.clients_id() {
            while let Some(message) =
                server.receive_message(client_id, renet::DefaultChannel::ReliableOrdered)
            {
                debug::info(&format!("({}): {}", client_id, String::from_utf8(message.to_vec()).unwrap()));
            }
        }

        transport.send_packets(&mut server);
        std::thread::sleep(constants::DELTA_TIME_MS);
    }
}
