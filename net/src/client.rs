use crate::{constants, debug, packet, error};
use flatbuffers;
use rand;
use renet;
use renet_netcode as netcode;
use std::sync::{Mutex, OnceLock};

#[allow(non_camel_case_types)]
struct Client_State {
    username: String,
    client: renet::RenetClient,
    transport: netcode::NetcodeClientTransport,
    connected: bool,
    connection_start_time: Option<std::time::Instant>,
}
static CLIENT_STATE: OnceLock<Mutex<Client_State>> = OnceLock::new();

pub fn client_setup(username: String, address: String) -> Result<(), error::Error> {
    let client = renet::RenetClient::new(renet::ConnectionConfig::default());

    let current_time = std::time::SystemTime::now().duration_since(std::time::SystemTime::UNIX_EPOCH)?;
    let server_address: std::net::SocketAddr = address.parse()?;

    let authentication = netcode::ClientAuthentication::Unsecure {
        server_addr: server_address,
        client_id: rand::random::<u64>(),
        user_data: Some(crate::to_netcode_user_data(username.as_str())),
        protocol_id: constants::PROTOCOL_ID,
    };

    let socket = std::net::UdpSocket::bind("127.0.0.1:0")?;
    let transport =
        netcode::NetcodeClientTransport::new(current_time, authentication, socket)?;

    match CLIENT_STATE
        .set(Mutex::new(Client_State {
            username,
            client,
            transport,
            connected: false,
            connection_start_time: None,
        })) {
            Ok(_) => {}
            Err(_) => {
                return Err(error::Error::StateError(String::from(
                    "CLIENT_STATE is ALREADY initialized",
                )));
            }
        }
    Ok(())
}

pub fn client_get_username() -> Result<String, error::Error> {
    let cell_lock = match CLIENT_STATE.get() {
        Some(out) => out,
        None => {
            return Err(error::Error::StateError(String::from(
                "CLIENT_STATE is NOT initialized",
            )));
        },
    };
    let mut mutex_gaurd = cell_lock.lock()?;
    let Client_State { username, .. } = &mut *mutex_gaurd;

    Ok(username.to_string())
}

pub fn client_is_connected() -> bool {
    let cell_lock = match CLIENT_STATE.get() {
        Some(out) => out,
        None => {
            return false;
        },
    };
    let mut mutex_gaurd = match cell_lock.lock(){
        Ok(out) => out,
        Err(_) => {
            return false;
       }
    };
    let Client_State { connected, .. } = &mut *mutex_gaurd;
    return *connected;
}

pub fn client_is_connecting() -> bool {
    let cell_lock = match CLIENT_STATE.get() {
        Some(out) => out,
        None => {
            return false;
        },
    };
    let mut mutex_gaurd = match cell_lock.lock() {
        Ok(out) => out,
        Err(_) => {
            return false;
        }
    };
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

pub fn client_connect(delta_time_ms: u64) -> Result<(), error::Error> {
    let delta_time = std::time::Duration::from_millis(delta_time_ms);
    let cell_lock = match CLIENT_STATE.get() {
        Some(out) => out,
        None => {
            return Err(error::Error::StateError(String::from(
                "CLIENT_STATE is NOT initialized",
            )));
        },
    };
    let mut mutex_gaurd = cell_lock.lock()?;
    let Client_State {
        client,
        transport,
        connected,
        connection_start_time,
        ..
    } = &mut *mutex_gaurd;

    if !*connected && connection_start_time.is_none() {
        *connection_start_time = Some(std::time::Instant::now());
    }

    if !*connected {
        if let Some(start_time) = connection_start_time {
            if start_time.elapsed() > constants::CLIENT_CONNECT_TIMEOUT {
                *connection_start_time = None;
                debug::error("failed to connect to server, timed out");
                return Ok(());
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
        return Ok(());
    }
    Ok(())
}

pub fn client_update(delta_time_ms: u64) -> Result<(), error::Error> {
    let delta_time = std::time::Duration::from_millis(delta_time_ms);

    let cell_lock = match CLIENT_STATE.get() {
        Some(out) => out,
        None => {
            return Err(error::Error::StateError(String::from(
                "CLIENT_STATE is NOT initialized",
            )));
        },
    };
    let mut mutex_gaurd = cell_lock.lock()?;
    let Client_State {
        client, transport, ..
    } = &mut *mutex_gaurd;

    client.update(delta_time);
    transport.update(delta_time, client)?;

    Ok(())
}

pub fn client_poll_packets() -> Result<Vec<crate::ffi::Message_Bytes>, error::Error> {
    let cell_lock = match CLIENT_STATE.get() {
        Some(out) => out,
        None => {
            return Err(error::Error::StateError(String::from(
                "CLIENT_STATE is NOT initialized",
            )));
        },
    };
    let mut mutex_gaurd = cell_lock.lock()?;
    let Client_State { client, .. } = &mut *mutex_gaurd;

    let mut output: Vec<crate::ffi::Message_Bytes> = Vec::new();
    while let Some(message) = client.receive_message(renet::DefaultChannel::ReliableOrdered) {
        output.push(crate::ffi::Message_Bytes {
            data: message.to_vec(),
        });
    }
    return Ok(output);
}

pub fn client_send_message(input: String) -> Result<(), error::Error> {
    let cell_lock = match CLIENT_STATE.get() {
        Some(out) => out,
        None => {
            return Err(error::Error::StateError(String::from(
                "CLIENT_STATE is NOT initialized",
            )));
        },
    };
    let mut mutex_gaurd = cell_lock.lock()?;
    let Client_State {
        client, username, ..
    } = &mut *mutex_gaurd;

    {
        let mut builder = flatbuffers::FlatBufferBuilder::with_capacity(1024);
        let message_username = builder.create_string(username.as_str());
        let message_body = builder.create_string(input.as_str());
        let packet_data = packet::Message::create(
            &mut builder,
            &packet::MessageArgs {
                username: Some(message_username),
                body: Some(message_body),
            },
        );
        let packet = packet::Packet::create(
            &mut builder,
            &packet::PacketArgs {
                data_type: packet::Packet_Data::Message,
                data: Some(packet_data.as_union_value()),
            },
        );
        builder.finish(packet, None);
        let buf = builder.finished_data().to_vec();
        client.send_message(renet::DefaultChannel::ReliableOrdered, buf);
    }
    Ok(())
}

pub fn client_send_packets() -> Result<(), error::Error> {
    let cell_lock = match CLIENT_STATE.get() {
        Some(out) => out,
        None => {
            return Err(error::Error::StateError(String::from(
                "CLIENT_STATE is NOT initialized",
            )));
        },
    };
    let mut mutex_gaurd = cell_lock.lock()?;
    let Client_State {
        client, transport, ..
    } = &mut *mutex_gaurd;
    transport.send_packets(client)?;
    Ok(())
}
