#include "layout.hpp"

constexpr int max_bubble_width = 420;

void layout::components::chat_bubble(
    Document& doc, Context& ctx, const std::string& username, const std::string& body, bool outgoing_message
) {
    // state configuration
    Clay_String username_cs = ctx.message_arena.alloc_clay_string(username);
    Clay_String body_cs = ctx.message_arena.alloc_clay_string(body);

    // color values
    app_utils::Color background_color = ctx.theme_e.get_color(Color_ID::CARD);
    app_utils::Color border_color = ctx.theme_e.get_color(Color_ID::BORDER);
    app_utils::Color username_foreground_color =
        ctx.theme_e.get_color(Color_ID::MUTED_FOREGROUND);
    app_utils::Color body_foreground_color = ctx.theme_e.get_color(Color_ID::FOREGROUND);

    CLAY(Clay_ElementDeclaration{
        .layout = { .sizing = { .width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIT() } } }) {
        if (outgoing_message) {
            CLAY(Clay_ElementDeclaration{
                .layout = { .sizing = { .width = CLAY_SIZING_GROW() } } }) {}
        }
        CLAY(Clay_ElementDeclaration{
            .layout = { .sizing = { .width = CLAY_SIZING_FIT(0, max_bubble_width),
                                    .height = CLAY_SIZING_FIT() },
                        .padding = { .left = 12, .right = 12, .top = 6, .bottom = 6 },
                        .childGap = 2,
                        .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_TOP },
                        .layoutDirection = CLAY_TOP_TO_BOTTOM },
            .backgroundColor = app_utils::to_clay_color(background_color),
            .cornerRadius = CLAY_CORNER_RADIUS(ctx.theme_e.get_radius()),
            .border = { .color = app_utils::to_clay_color(border_color),
                        .width = ctx.theme_e.get_border_width() } }) {
            CLAY(Clay_ElementDeclaration{
                .layout = { .sizing = { .width = CLAY_SIZING_GROW(),
                                        .height = CLAY_SIZING_FIT() } } }) {
                if (outgoing_message) {
                    CLAY(Clay_ElementDeclaration{
                        .layout = { .sizing = { .width = CLAY_SIZING_GROW() } } }) {}
                }
                CLAY_TEXT(
                    username_cs,
                    CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                        .textColor = app_utils::to_clay_color(username_foreground_color),
                        .fontId = 0,
                        .fontSize = 24,
                    })
                );
            }
            CLAY(Clay_ElementDeclaration{
                .layout = { .sizing = { .width = CLAY_SIZING_GROW(),
                                        .height = CLAY_SIZING_FIT() } } }) {
                if (outgoing_message) {
                    CLAY(Clay_ElementDeclaration{
                        .layout = { .sizing = { .width = CLAY_SIZING_GROW() } } }) {}
                }
                CLAY_TEXT(
                    body_cs,
                    CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                        .textColor = app_utils::to_clay_color(body_foreground_color),
                        .fontId = 0,
                        .fontSize = 24,
                    })
                );
            }
        }
    }
}
