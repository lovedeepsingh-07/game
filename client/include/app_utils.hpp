#pragma once

#include <clay/clay.h>
#include <string>

namespace app_utils {
    struct Color {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };
    constexpr Color FAIL_PURPLE = app_utils::Color{ 127, 0, 237, 255 };
    struct Vector2 {
        float x;
        float y;
    };

    app_utils::Color set_color_alpha(app_utils::Color color, float ratio);
    Clay_Color to_clay_color(Color input_color);
    void clay_error_handler(Clay_ErrorData error_data);
    std::string trim_whitespace(const std::string& input);
}
