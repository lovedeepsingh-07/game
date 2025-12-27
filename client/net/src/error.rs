use renet_netcode as netcode;

#[allow(non_camel_case_types)]
pub enum Error {
    StateAlreadyInitializedError,
    StateNotInitializedError,
    AuthError(String),
    PoisonError(String),
    IOError(String),
    UreqError(String),
    NetcodeError(String),
    TimeOutError,
    TransportError(String),
    SystemTimeError(String),
    ParseError(String),
}

impl std::fmt::Display for Error {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Error::StateAlreadyInitializedError => {
                write!(f, "1|Static state is ALREADY initialized")
            }
            Error::StateNotInitializedError => write!(f, "2|Static state is NOT initialized"),
            Error::AuthError(err_str) => write!(f, "3|{}", err_str),
            Error::PoisonError(err_str) => write!(f, "4|{}", err_str),
            Error::IOError(err_str) => write!(f, "5|{}", err_str),
            Error::UreqError(err_str) => write!(f, "6|{}", err_str),
            Error::NetcodeError(err_str) => write!(f, "7|{}", err_str),
            Error::TimeOutError => write!(f, "8|Timed out"),
            Error::TransportError(err_str) => write!(f, "9|{}", err_str),
            Error::SystemTimeError(err_str) => write!(f, "10|{}", err_str),
            Error::ParseError(err_str) => write!(f, "11|{}", err_str),
        }
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
impl From<ureq::Error> for Error {
    fn from(value: ureq::Error) -> Self {
        Error::UreqError(value.to_string())
    }
}
impl From<netcode::NetcodeError> for Error {
    fn from(value: netcode::NetcodeError) -> Self {
        Error::NetcodeError(value.to_string())
    }
}
impl From<netcode::NetcodeTransportError> for Error {
    fn from(value: netcode::NetcodeTransportError) -> Self {
        Error::TransportError(value.to_string())
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
