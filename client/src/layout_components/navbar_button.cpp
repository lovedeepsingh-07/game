#include "layout.hpp"
#include <raylib.h>

bool layout::components::navbar_button(Context& ctx) {
    CLAY(Clay_ElementDeclaration{
        .id = CLAY_ID("theme_button"),
        .layout = { .sizing = { .width = CLAY_SIZING_FIT(92), .height = CLAY_SIZING_FIT(20) },
                    .padding = { .left = 8, .right = 8, .top = 6, .bottom = 6 },
                    .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM },
        .backgroundColor = common::to_clay_color(ctx.theme_e.get_color(Color_ID::PRIMARY)),
        .cornerRadius = CLAY_CORNER_RADIUS(ctx.theme_e.get_radius()),
        .border = { .color = common::to_clay_color(ctx.theme_e.get_color(Color_ID::BORDER)),
                    .width = { 1, 1, 1, 1, 0 } } }) {
        if (Clay_Hovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (ctx.theme_e.get_curr_theme() == "dark") {
                ctx.theme_e.set_curr_theme("light");
            } else {
                ctx.theme_e.set_curr_theme("dark");
            }
        }
        CLAY_TEXT(
            CLAY_STRING("theme"),
            CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                .textColor = common::to_clay_color(ctx.theme_e.get_color(Color_ID::PRIMARY_FOREGROUND)),
                .fontId = 0,
                .fontSize = 20,
            })
        );
    }
    return false;
}
