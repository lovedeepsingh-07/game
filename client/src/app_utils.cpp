#include "app_utils.hpp"

Clay_Color app_utils::raylib_color_to_clay_color(Color rayColor) {
    Clay_Color clayColor;
    clayColor.r = rayColor.r;
    clayColor.g = rayColor.g;
    clayColor.b = rayColor.b;
    clayColor.a = rayColor.a;
    return clayColor;
}

void app_utils::clay_error_handler(Clay_ErrorData errorData) {
    debug::error(fmt::format("clay_error: {}", errorData.errorText.chars));
}
