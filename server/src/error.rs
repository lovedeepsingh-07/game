use renet_netcode as netcode;

pub enum Error {
    ChannelSendError(String),
    ChannelReceiveError(String),
    PoisonError(String),
    IOError(String),
    SystemTimeError(String),
    ParseError(String),
    ConnectTokenError(String),
}

impl std::string::ToString for Error {
    fn to_string(&self) -> String {
        match self {
            Error::ChannelSendError(err_str) => format!("ChannelSendError {}", err_str),
            Error::ChannelReceiveError(err_str) => format!("ChannelReceiveError {}", err_str),
            Error::PoisonError(err_str) => format!("PoisonError {}", err_str),
            Error::IOError(err_str) => format!("IOError {}", err_str),
            Error::SystemTimeError(err_str) => format!("SystemTimeError {}", err_str),
            Error::ParseError(err_str) => format!("ParseError {}", err_str),
            Error::ConnectTokenError(err_str) => format!("ConnectTokenError {}", err_str),
        }
    }
}

impl<T> From<crossbeam::channel::SendError<T>> for Error {
    fn from(value: crossbeam::channel::SendError<T>) -> Self {
        Error::ChannelSendError(value.to_string())
    }
}
impl From<crossbeam::channel::RecvError> for Error {
    fn from(value: crossbeam::channel::RecvError) -> Self {
        Error::ChannelReceiveError(value.to_string())
    }
}
impl<T> From<std::sync::PoisonError<T>> for Error {
    fn from(value: std::sync::PoisonError<T>) -> Self {
        Error::PoisonError(value.to_string())
    }
}
impl From<std::io::Error> for Error {
    fn from(value: std::io::Error) -> Self {
        Error::IOError(value.to_string())
    }
}
impl From<std::time::SystemTimeError> for Error {
    fn from(value: std::time::SystemTimeError) -> Self {
        Error::SystemTimeError(value.to_string())
    }
}
impl From<std::net::AddrParseError> for Error {
    fn from(value: std::net::AddrParseError) -> Self {
        Error::ParseError(value.to_string())
    }
}
impl From<netcode::TokenGenerationError> for Error {
    fn from(value: netcode::TokenGenerationError) -> Self {
        Error::ConnectTokenError(value.to_string())
    }
}
