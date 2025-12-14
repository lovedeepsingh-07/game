#pragma once

#include <fmt/format.h>
#include <rust/cxx.h>
#include <string>
#include <vector>

namespace error {
    class Error {
      public:
        enum class Kind {
            InvalidErrorConversion,
            TimeError,
            AddressParseError,
            IOError,
            StateError,
            TransportError,
            FlatbufferError,
            NetcodeError,
        } kind;
        std::string message;

        Error(Kind _kind, std::string _message)
        : kind(_kind), message(_message) {};
        ~Error() = default;

        std::string to_string() const;

        static Error InvalidErrorConversion() {
            return Error(Kind::InvalidErrorConversion, "");
        }
        static Error TimeError(std::string message) {
            return Error(Kind::TimeError, std::move(message));
        }
        static Error AddressParseError(std::string message) {
            return Error(Kind::AddressParseError, std::move(message));
        }
        static Error IOError(std::string message) {
            return Error(Kind::IOError, std::move(message));
        }
        static Error StateError(std::string message) {
            return Error(Kind::StateError, std::move(message));
        }
        static Error TransportError(std::string message) {
            return Error(Kind::TransportError, std::move(message));
        }
        static Error FlatbufferError(std::string message) {
            return Error(Kind::FlatbufferError, std::move(message));
        }
        static Error NetcodeError(std::string message) {
            return Error(Kind::NetcodeError, std::move(message));
        }

        static Error from_rust(rust::Error e);
    };
}
