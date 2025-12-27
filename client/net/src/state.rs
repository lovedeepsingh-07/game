use crate::error;
use renet_netcode as netcode;
use std::sync::{LazyLock, RwLock};

#[allow(dead_code)]
#[derive(Debug)]
pub struct ClientState {
    pub client_id: u64,
    pub username: String,
    pub authenticated: bool,
    pub connected: bool,
    pub connection_start_time: Option<std::time::Instant>,
    pub client: renet::RenetClient,
    pub transport: Option<netcode::NetcodeClientTransport>,
}
impl ClientState {
    fn new(username: String) -> ClientState {
        ClientState {
            client_id: rand::random::<u64>(),
            username,
            authenticated: false,
            connected: false,
            connection_start_time: None,
            client: renet::RenetClient::new(renet::ConnectionConfig::default()),
            transport: None,
        }
    }
}

pub(crate) static CLIENT_STATE: LazyLock<RwLock<Option<ClientState>>> =
    LazyLock::new(|| RwLock::new(None));

pub fn setup_client(username: String) -> Result<(), error::Error> {
    let mut client_state = CLIENT_STATE.write()?;

    match *client_state {
        Some(_) => {
            return Err(error::Error::StateAlreadyInitializedError);
        }
        None => {
            *client_state = Some(ClientState::new(username));
        }
    }

    Ok(())
}

pub fn reset_client() -> Result<(), error::Error> {
    let mut client_state = CLIENT_STATE.write()?;
    *client_state = None;
    Ok(())
}
