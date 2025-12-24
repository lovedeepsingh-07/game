#include "layout.hpp"
#include <raylib.h>

bool layout::components::navbar_button(
    Document& doc, Context& ctx, const std::string& id, const std::string& button_text
) {
    Clay_String button_text_cs = ctx.frame_arena.alloc_clay_string(button_text);
    Clay_String id_cs = ctx.frame_arena.alloc_clay_string(id);
    Clay_ElementId button_id = CLAY_SID(id_cs);

    // button state configuration
    bool button_clicked = false;
    bool hovering = Clay_PointerOver(button_id);
    if (hovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        button_clicked = true;
    }

    // color values
    app_utils::Color button_background_color =
        ctx.theme_e.get_color(hovering ? Color_ID::ACCENT : Color_ID::BACKGROUND);
    app_utils::Color button_foreground_color =
        ctx.theme_e.get_color(hovering ? Color_ID::ACCENT_FOREGROUND : Color_ID::FOREGROUND);
    app_utils::Color button_border_color = ctx.theme_e.get_color(Color_ID::BORDER);
    Clay_BorderWidth button_border_width = hovering
        ? Clay_BorderWidth{ .left = 3, .right = 3, .top = 3, .bottom = 3 }
        : ctx.theme_e.get_border_width();

    CLAY(Clay_ElementDeclaration{
        .id = button_id,
        .layout = { .sizing = { .width = CLAY_SIZING_FIT(120), .height = CLAY_SIZING_FIT(39) },
                    .padding = { .left = 12, .right = 12, .top = 7, .bottom = 7 },
                    .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM },
        .backgroundColor = app_utils::to_clay_color(button_background_color),
        .cornerRadius = CLAY_CORNER_RADIUS(ctx.theme_e.get_radius()),
        .border = { .color = app_utils::to_clay_color(button_border_color),
                    .width = button_border_width } }) {
        CLAY_TEXT(
            button_text_cs,
            CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                .textColor = app_utils::to_clay_color(button_foreground_color),
                .fontId = 0,
                .fontSize = 24,
            })
        );
    };
    return button_clicked;
}
