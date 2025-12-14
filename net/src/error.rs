use renet_netcode as netcode;
use flatbuffers;

pub enum Error {
    TimeError(String),
    AddressParseError(String),
    IOError(String),
    StateError(String),
    TransportError(String),
    FlatbufferError(String),
    NetcodeError(String),
}

impl From<std::time::SystemTimeError> for Error {
    fn from(error: std::time::SystemTimeError) -> Self {
        Error::TimeError(error.to_string())
    }
}
impl From<std::net::AddrParseError> for Error {
    fn from(error: std::net::AddrParseError) -> Self {
        Error::AddressParseError(error.to_string())
    }
}
impl From<std::io::Error> for Error {
    fn from(error: std::io::Error) -> Self {
        Error::IOError(error.to_string())
    }
}
impl<T> From<std::sync::PoisonError<T>> for Error {
    fn from(error: std::sync::PoisonError<T>) -> Self {
        Error::StateError(error.to_string())
    }
}
impl From<netcode::NetcodeTransportError> for Error {
    fn from(error: netcode::NetcodeTransportError) -> Self {
        Error::TransportError(error.to_string())
    }
}
impl From<flatbuffers::InvalidFlatbuffer> for Error {
    fn from(error: flatbuffers::InvalidFlatbuffer) -> Self {
        Error::FlatbufferError(error.to_string())
    }
}
impl From<netcode::NetcodeError> for Error {
    fn from(error: netcode::NetcodeError) -> Self {
        Error::NetcodeError(error.to_string())
    }
}

impl std::fmt::Display for Error {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Error::TimeError(err_str) => {
                write!(f, "TimeError {}", err_str)
            }
            Error::AddressParseError(err_str) => {
                write!(f, "AddressParseError {}", err_str)
            }
            Error::IOError(err_str) => {
                write!(f, "IOError {}", err_str)
            }
            Error::StateError(err_str) => {
                write!(f, "StateError {}", err_str)
            }
            Error::TransportError(err_str) => {
                write!(f, "TransportError {}", err_str)
            }
            Error::FlatbufferError(err_str) => {
                write!(f, "FlatbufferError {}", err_str)
            }
            Error::NetcodeError(err_str) => {
                write!(f, "NetcodeError {}", err_str)
            }
        }
    }
}
