use crate::{error, state};
use boron_common::constants;
use renet_netcode as netcode;

pub fn connect_client(delta_time_ms: u64) -> Result<(), error::Error> {
    let delta_time = std::time::Duration::from_millis(delta_time_ms);

    let mut write_guard = state::CLIENT_STATE.write()?;
    let client_state = write_guard
        .as_mut()
        .ok_or(error::Error::StateNotInitializedError)?;

    if !client_state.connected
        && client_state.connection_start_time.is_none()
        && !client_state.authenticated
    {
        client_state.connection_start_time = Some(std::time::Instant::now());

        // ------ authentication ------
        let config = ureq::Agent::config_builder()
            .http_status_as_error(false)
            .build();
        let agent: ureq::Agent = config.into();
        let res = agent
            .post(format!("http://{}/login", constants::WEB_SERVER_ADDRESS))
            .send_json(&serde_json::json!({"client_id": client_state.client_id, "username": client_state.username}));

        let connect_token: netcode::ConnectToken;

        match res {
            Ok(mut res) => {
                if res.status() == 200 {
                    // 200 - OK
                    connect_token = netcode::ConnectToken::read(&mut res.body_mut().as_reader())?;
                } else if res.status() == 409 {
                    // 409 - CONFLICT
                    let conflict_code = res.body_mut().read_to_string()?;
                    if conflict_code == "ID" {
                        return Err(error::Error::AuthError(format!(
                            "Client with this ID is ALREADY connected to the server"
                        )));
                    } else if conflict_code == "USERNAME" {
                        return Err(error::Error::AuthError(format!(
                            "Client with this username is ALREADY connected to the server"
                        )));
                    } else {
                        return Err(error::Error::AuthError(format!(
                            "Server returned an invalid conflict code, {}",
                            conflict_code
                        )));
                    }
                } else {
                    return Err(error::Error::AuthError(
                        "Server returned an invalid status code".to_string(),
                    ));
                }
            }
            Err(e) => {
                return Err(error::Error::AuthError(format!(
                    "Failed to authenticate client, {}",
                    e.to_string()
                )));
            }
        };
        client_state.authenticated = true;

        // ------ setup netcode transport ------
        let current_time = std::time::SystemTime::now()
            .duration_since(std::time::SystemTime::UNIX_EPOCH)?;
        let socket = std::net::UdpSocket::bind("127.0.0.1:0")?;
        let authentication = netcode::ClientAuthentication::Secure { connect_token };
        client_state.transport = Some(
            netcode::NetcodeClientTransport::new(current_time, authentication, socket)?,
        );
    }

    if !client_state.connected {
        if let Some(start_time) = client_state.connection_start_time {
            if start_time.elapsed()
                > std::time::Duration::from_secs(constants::CLIENT_TIMEOUT_SEC as u64)
            {
                client_state.connection_start_time = None;
                return Err(error::Error::TimeOutError);
            }
        }

        client_state.client.update(delta_time);
        if let Some(ref mut transport) = client_state.transport {
            transport.update(delta_time, &mut client_state.client)?;
        }

        client_state.connected = client_state.client.is_connected();
    }

    Ok(())
}
