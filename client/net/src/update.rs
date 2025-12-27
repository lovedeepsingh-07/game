use boron_common::debug;
use crate::{error, state};

pub fn update_client(delta_time_ms: u64) -> Result<(), error::Error> {
    let delta_time = std::time::Duration::from_millis(delta_time_ms);
    let mut write_guard = state::CLIENT_STATE.write()?;
    let client_state = write_guard
        .as_mut()
        .ok_or(error::Error::StateNotInitializedError)?;

    client_state.client.update(delta_time);
    if let Some(ref mut transport) = client_state.transport {
        transport.update(delta_time, &mut client_state.client)?;
    }

    Ok(())
}

pub fn send_message(input: String) -> Result<(), error::Error> {
    let mut write_guard = state::CLIENT_STATE.write()?;
    let client_state = write_guard
        .as_mut()
        .ok_or(error::Error::StateNotInitializedError)?;

    client_state.client.send_message(renet::DefaultChannel::ReliableOrdered, input);
    Ok(())
}

pub fn send_packets() -> Result<(), error::Error> {
    let mut write_guard = state::CLIENT_STATE.write()?;
    let client_state = write_guard
        .as_mut()
        .ok_or(error::Error::StateNotInitializedError)?;
    if let Some(ref mut transport) = client_state.transport {
        transport.send_packets(&mut client_state.client)?;
    }
    Ok(())
}
