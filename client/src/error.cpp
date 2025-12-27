#include "error.hpp"
#include <fmt/format.h>

std::string error::Error::to_string() const {
    if (this->kind == Kind::StateAlreadyInitializedError) {
        return fmt::format("StateAlreadyInitializedError: {}", this->message);
    }
    if (this->kind == Kind::StateNotInitializedError) {
        return fmt::format("StateNotInitializedError: {}", this->message);
    }
    if (this->kind == Kind::AuthError) {
        return fmt::format("AuthError: {}", this->message);
    }
    if (this->kind == Kind::PoisonError) {
        return fmt::format("PoisonError: {}", this->message);
    }
    if (this->kind == Kind::IOError) {
        return fmt::format("IOError: {}", this->message);
    }
    if (this->kind == Kind::UreqError) {
        return fmt::format("UreqError: {}", this->message);
    }
    if (this->kind == Kind::NetcodeError) {
        return fmt::format("NetcodeError: {}", this->message);
    }
    if (this->kind == Kind::TimeOutError) {
        return fmt::format("TimeOutError: {}", this->message);
    }
    if (this->kind == Kind::TransportError) {
        return fmt::format("TransportError: {}", this->message);
    }
    if (this->kind == Kind::SystemTimeError) {
        return fmt::format("SystemTimeError: {}", this->message);
    }
    if (this->kind == Kind::ParseError) {
        return fmt::format("ParseError: {}", this->message);
    }
    return "InvalidErrorConversion";
}

error::Error error::Error::from_rust(rust::Error e) {
    std::string err_str = std::string(e.what());
    std::vector<std::string> part_vec;

    size_t sep_pos = err_str.find_first_of("|");
    if (sep_pos == std::string::npos) {
        part_vec.push_back(err_str);
        part_vec.push_back("");
    } else {
        part_vec.push_back(err_str.substr(0, sep_pos));
        part_vec.push_back(err_str.substr(sep_pos + 1));
    }

    if (part_vec.size() != 2) {
        return Error(Kind::InvalidErrorConversion, "");
    }
    int enum_int = std::stoi(part_vec[0]);
    Kind err_kind = static_cast<Kind>(enum_int);
    return Error(err_kind, std::move(part_vec[1]));

    return Error(Kind::InvalidErrorConversion, "");
}
