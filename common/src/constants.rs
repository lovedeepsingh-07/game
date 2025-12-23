pub const MAX_CLIENTS: usize = 128;
pub const TOKEN_EXPIRE_SEC: u64 = 30;
pub const CLIENT_TIMEOUT_SEC: i32 = 30;
pub const GAME_SERVER_ADDRESS: &'static str = "127.0.0.1:9090";
pub const WEB_SERVER_ADDRESS: &'static str = "0.0.0.0:3000";

pub const PROTOCOL_ID: u64 = 0;
pub const PRIVATE_KEY: &'static str = "BORON_CHAT_BORON_CHAT";

pub const TARGET_FPS: u64 = 90;
pub const DELTA_TIME_MS: std::time::Duration = std::time::Duration::from_millis(1000 / TARGET_FPS);
