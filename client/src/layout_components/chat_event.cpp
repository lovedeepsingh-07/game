#include "layout.hpp"

void layout::components::chat_event(Document& doc, Context& ctx, const std::string& body) {
    // state configuration
    Clay_String body_cs = ctx.message_arena.alloc_clay_string(body);

    // color values
    app_utils::Color foreground_color = ctx.theme_e.get_color(Color_ID::MUTED_FOREGROUND);

    CLAY(Clay_ElementDeclaration{
        .layout = { .sizing = { .width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIT() },
                    .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER } } }) {
        CLAY_TEXT(
            body_cs,
            CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                .textColor = app_utils::to_clay_color(foreground_color),
                .fontId = 0,
                .fontSize = 24,
            })
        );
    }
}
