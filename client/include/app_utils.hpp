#include <clay/clay.h>
#include <common/common.hpp>
#include <raylib.h>

namespace app_utils {
    Clay_Color raylib_color_to_clay_color(Color rayColor);
    void clay_error_handler(Clay_ErrorData errorData);
}
