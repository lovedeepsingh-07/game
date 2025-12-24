// use renet_netcode as netcode;
// use boron_common::{constants, debug};
// use std::io::Write;

#[cxx::bridge(namespace = "net")]
pub mod ffi {
    extern "Rust" {
        fn connect();
        fn is_connecting();
        fn is_connected();
    }
}

pub fn connect() {
}
pub fn is_connecting() {
}
pub fn is_connected() {
}

// pub fn hello_client() {
//     debug::debug("Enter your username: ");
//     std::io::stdout().flush().unwrap();
//     let mut username = String::new();
//     std::io::stdin().read_line(&mut username).unwrap();
//
//     let username = username.trim().to_string();
//     let client_id = rand::random::<u64>();
//
//     debug::info(&format!(
//         "logging into {}/login...",
//         constants::WEB_SERVER_ADDRESS
//     ));
//     let config = ureq::Agent::config_builder()
//         .http_status_as_error(false)
//         .build();
//     let agent: ureq::Agent = config.into();
//     let res = agent
//         .post(format!("http://{}/login", constants::WEB_SERVER_ADDRESS))
//         .send_json(&serde_json::json!({"client_id": client_id, "username": username}));
//
//     let connect_token: netcode::ConnectToken;
//
//     match res {
//         Ok(mut res) => {
//             if res.status() == 200 { // 200 - OK
//                 connect_token = netcode::ConnectToken::read(&mut res.body_mut().as_reader()).unwrap()
//             } else if res.status() == 409 {
//                 // 409 - CONFLICT
//                 let conflict_code = res.body_mut().read_to_string().unwrap();
//                 if conflict_code == "ID" {
//                     debug::info("client with this ID is already connected to the server!");
//                 } else if conflict_code == "USERNAME" {
//                     debug::info("client with this username is already connected to the server!");
//                 } else {
//                     debug::error(&format!("invalid conflict code, {}", conflict_code));
//                 }
//                 return;
//             } else {
//                 debug::error(&format!("{:#?}", res));
//                 return;
//             }
//         }
//         Err(e) => {
//             debug::error(e.to_string().as_str());
//             return;
//         }
//     };
//
//     debug::info(&format!(
//         "you will be playing as ({}, {})",
//         username, client_id
//     ));
//     debug::info(&format!("{:?}", connect_token));
// }
