#pragma once

#include <fmt/base.h>
#include <fmt/color.h>
#include <fmt/format.h>

namespace debug {
    void debug(const std::string& input);
    void info(const std::string& input);
    void warn(const std::string& input);
    void error(const std::string& input);
}
