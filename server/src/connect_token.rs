use crate::error;
use boron_common::{constants, utils};
use renet_netcode as netcode;

pub fn generate(client_id: u64) -> Result<netcode::ConnectToken, error::Error> {
    let current_time =
        std::time::SystemTime::now().duration_since(std::time::SystemTime::UNIX_EPOCH)?;
    let server_address: std::net::SocketAddr = constants::GAME_SERVER_ADDRESS.parse()?;
    let user_data = None;
    let private_key = utils::str_to_32_bytes(constants::PRIVATE_KEY);

    let connect_token = netcode::ConnectToken::generate(
        current_time,
        constants::PROTOCOL_ID,
        constants::TOKEN_EXPIRE_SEC,
        client_id,
        constants::CLIENT_TIMEOUT_SEC,
        vec![server_address],
        user_data,
        &private_key,
    )?;

    Ok(connect_token)
}
