#include "app_utils.hpp"
#include "debug.hpp"
#include <algorithm>

app_utils::Color app_utils::set_color_alpha(app_utils::Color color, float ratio) {
    ratio = std::clamp<float>(ratio, 0, 1);
    color.a = (uint8_t)(255 * ratio);
    return color;
}
Clay_Color app_utils::to_clay_color(app_utils::Color input_color) {
    Clay_Color clay_color;
    clay_color.r = input_color.r;
    clay_color.g = input_color.g;
    clay_color.b = input_color.b;
    clay_color.a = input_color.a;
    return clay_color;
}
void app_utils::clay_error_handler(Clay_ErrorData error_data) {
    debug::error(fmt::format("clay_error: {}", error_data.errorText.chars));
}
std::string app_utils::trim_whitespace(const std::string& input) {
    size_t start = 0;
    while (start < input.size()
           && (std::isspace(static_cast<unsigned char>(input[start])) != 0)) {
        ++start;
    }
    size_t end = input.size();
    while (end > start && (std::isspace(static_cast<unsigned char>(input[end - 1])) != 0)) {
        --end;
    }
    return input.substr(start, end - start);
}
