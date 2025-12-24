#include "layout.hpp"
#include <raylib.h>

void layout::components::message_input(
    Document& doc, Context& ctx, const std::string& id, const std::string& placeholder
) {
    Clay_String id_cs = ctx.frame_arena.alloc_clay_string(id);
    Clay_ElementId input_id = CLAY_SID(id_cs);
    Clay_String placeholder_cs = ctx.frame_arena.alloc_clay_string(placeholder);

    doc.add_element(id, std::make_unique<elements::Input>());
    auto* element_ctx = doc.get_element<elements::Input>(id);

    // input state configuration
    bool hovering = Clay_PointerOver(input_id);
    if (hovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !element_ctx->focused) {
        element_ctx->focused = true;
    }
    if (!hovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && element_ctx->focused) {
        element_ctx->focused = false;
    }

    // handlinng input value
    if (element_ctx->focused) {
        int key = GetCharPressed();
        while (key > 0) {
            if (key >= 32 && key <= 126) {
                element_ctx->value.push_back((char)key);
            }
            key = GetCharPressed();
        }
        if ((IsKeyPressed(KEY_BACKSPACE)) && !element_ctx->value.empty()) {
            element_ctx->value.pop_back();
        }
        if (IsKeyPressed(KEY_BACKSPACE)
            && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL))) {
            element_ctx->value.clear();
        }
    }
    auto input_value_cs = ctx.frame_arena.alloc_clay_string(element_ctx->value);

    // color values
    app_utils::Color input_placeholder_color =
        ctx.theme_e.get_color(Color_ID::MUTED_FOREGROUND);
    app_utils::Color input_foreground_color = ctx.theme_e.get_color(Color_ID::FOREGROUND);
    app_utils::Color input_border_color =
        ctx.theme_e.get_color(element_ctx->focused ? Color_ID::RING : Color_ID::BORDER);
    Clay_BorderWidth input_border_width = element_ctx->focused
        ? Clay_BorderWidth{ .left = 3, .right = 3, .top = 3, .bottom = 3 }
        : ctx.theme_e.get_border_width();

    CLAY(Clay_ElementDeclaration{
        .id = input_id,
        .layout = { .sizing = { .width = CLAY_SIZING_GROW(160, 800), .height = CLAY_SIZING_FIT(39) },
                    .padding = { .left = 12, .right = 12, .top = 7, .bottom = 7 },
                    .childAlignment = { .x = CLAY_ALIGN_X_LEFT, .y = CLAY_ALIGN_Y_CENTER },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM },
        .cornerRadius = CLAY_CORNER_RADIUS(ctx.theme_e.get_radius()),
        .clip = { .horizontal = true },
        .border = { .color = app_utils::to_clay_color(input_border_color),
                    .width = input_border_width } }) {
        if (element_ctx->value.empty()) {
            CLAY_TEXT(
                placeholder_cs,
                CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                    .textColor = app_utils::to_clay_color(input_placeholder_color),
                    .fontId = 0,
                    .fontSize = 24,
                })
            );
        } else {
            CLAY_TEXT(
                input_value_cs,
                CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                    .textColor = app_utils::to_clay_color(input_foreground_color),
                    .fontId = 0,
                    .fontSize = 24,
                })
            );
        }
    }
}
