use crate::{command, connect_token, error, state};

#[derive(Debug)]
pub enum Command {
    Login {
        client_id: u64,
        username: String,
        reply_sender: crossbeam::channel::Sender<CommandReply>,
    },
    Disconnect {
        client_id: u64,
        username: String,
    }
}

#[allow(non_camel_case_types)]
#[derive(Debug)]
pub enum CommandReply {
    Login_Valid(Vec<u8>),
    Login_InvalidID,
    Login_InvalidUsername,
}

pub fn handle(
    game_state: &mut state::GameState,
    cmd: command::Command,
) -> Result<(), error::Error> {
    match cmd {
        command::Command::Login {
            client_id,
            username,
            reply_sender,
        } => {
            if !game_state.usernames_in_use.contains(&username)
                && !game_state.client_list.contains_key(&client_id)
            {
                game_state.client_list.insert(
                    client_id,
                    state::ClientData {
                        id: client_id,
                        username: username.clone(),
                    },
                );
                game_state.usernames_in_use.insert(username);

                let mut connect_token_buffer: Vec<u8> = Vec::new();
                let connect_token = connect_token::generate(client_id)?;
                connect_token.write(&mut connect_token_buffer)?;
                reply_sender.send(command::CommandReply::Login_Valid(connect_token_buffer))?;
            } else if game_state.client_list.contains_key(&client_id) {
                reply_sender.send(command::CommandReply::Login_InvalidID)?;
            } else {
                reply_sender.send(command::CommandReply::Login_InvalidUsername)?;
            }
        },
        command::Command::Disconnect {
            client_id,
            username
        } => {
            let _ = game_state.client_list.remove(&client_id);
            let _ = game_state.usernames_in_use.remove(&username);
        }
    };
    Ok(())
}
