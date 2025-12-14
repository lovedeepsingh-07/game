#include <common/error.hpp>

std::string error::Error::to_string() const {
    if (this->kind == Kind::TimeError) {
        return fmt::format("TimeError: {}", this->message);
    }
    if (this->kind == Kind::AddressParseError) {
        return fmt::format("AddressParseError: {}", this->message);
    }
    if (this->kind == Kind::IOError) {
        return fmt::format("IOError: {}", this->message);
    }
    if (this->kind == Kind::StateError) {
        return fmt::format("StateError: {}", this->message);
    }
    if (this->kind == Kind::TransportError) {
        return fmt::format("TransportError: {}", this->message);
    }
    if (this->kind == Kind::FlatbufferError) {
        return fmt::format("FlatbufferError: {}", this->message);
    }
    if (this->kind == Kind::NetcodeError) {
        return fmt::format("NetcodeError: {}", this->message);
    }
    return "InvalidErrorConversion";
}

error::Error error::Error::from_rust(rust::Error e) {
    std::string err_str = std::string(e.what());

    std::vector<std::string> part_vec;

    size_t space_pos = err_str.find_first_of(" ");
    if (space_pos == std::string::npos) {
        part_vec.push_back(err_str);
        part_vec.push_back("");
    } else {
        part_vec.push_back(err_str.substr(0, space_pos));
        part_vec.push_back(err_str.substr(space_pos + 1));
    }

    if (part_vec.size() != 2) {
        return Error::InvalidErrorConversion();
    }

    if (part_vec[0] == "TimeError") {
        return Error::TimeError(part_vec[1]);
    }
    if (part_vec[0] == "AddressParseError") {
        return Error::AddressParseError(part_vec[1]);
    }
    if (part_vec[0] == "IOError") {
        return Error::IOError(part_vec[1]);
    }
    if (part_vec[0] == "StateError") {
        return Error::StateError(part_vec[1]);
    }
    if (part_vec[0] == "TransportError") {
        return Error::TransportError(part_vec[1]);
    }
    if (part_vec[0] == "FlatbufferError") {
        return Error::FlatbufferError(part_vec[1]);
    }
    if (part_vec[0] == "NetcodeError") {
        return Error::NetcodeError(part_vec[1]);
    }

    return Error::InvalidErrorConversion();
}
