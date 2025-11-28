#include "layout.hpp"
#include <common/theme_engine.hpp>

void layout::pages::chat(Document& doc, Context& ctx) {
    CLAY(Clay_ElementDeclaration{
        .layout = { .sizing = { .width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW() },
                    .padding = { .top = 160 },
                    .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_TOP },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM } }) {
        CLAY_TEXT(
            CLAY_STRING("Chat Page"),
            CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                .textColor = common::to_clay_color(ctx.theme_e.get_color(Color_ID::FOREGROUND)),
                .fontId = 0,
                .fontSize = 48,
            })
        );
    }
}
