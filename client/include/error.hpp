#pragma once

#include <rust/cxx.h>
#include <string>

namespace error {
    class Error {
      public:
        enum class Kind {
            InvalidErrorConversion,
            StateAlreadyInitializedError,
            StateNotInitializedError,
            AuthError,
            PoisonError,
            IOError,
            UreqError,
            NetcodeError,
            TimeOutError,
            TransportError,
            SystemTimeError,
            ParseError,
        } kind;
        std::string message;

        Error(Kind _kind, std::string _message)
        : kind(_kind), message(_message) {};
        ~Error() = default;

        std::string to_string() const;
        static Error from_rust(rust::Error e);
    };
}
