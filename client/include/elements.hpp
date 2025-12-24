#pragma once

#include <string>

namespace elements {
    class Element {
      public:
        virtual ~Element() = default;
    };
    class Input : public Element {
      public:
        std::string value;
        bool focused{ false };
    };
};
