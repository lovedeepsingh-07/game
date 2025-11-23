#include "layout.hpp"
#include <common/theme_engine.hpp>

constexpr int min_card_width = 320;

void layout::pages::login(Document& doc, Context& ctx) {
    CLAY(Clay_ElementDeclaration{
        .layout = { .sizing = { .width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW() },
                    .padding = { .top = 160 },
                    .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_TOP },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM } }) {
        CLAY(Clay_ElementDeclaration{
            .layout = { .sizing = { .width = CLAY_SIZING_FIT(min_card_width),
                                    .height = CLAY_SIZING_FIT() },
                        .padding = { .left = 30, .right = 30, .top = 20, .bottom = 20 },
                        .childGap = 20,
                        .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_TOP },
                        .layoutDirection = CLAY_TOP_TO_BOTTOM },
            .backgroundColor = common::to_clay_color(ctx.theme_e.get_color(Color_ID::CARD)),
            .cornerRadius = CLAY_CORNER_RADIUS(ctx.theme_e.get_radius()),
            .border = { .color = common::to_clay_color(ctx.theme_e.get_color(Color_ID::BORDER)),
                        .width = ctx.theme_e.get_border_width() } }) {
            CLAY_TEXT(
                CLAY_STRING("Login"),
                CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                    .textColor = common::to_clay_color(ctx.theme_e.get_color(Color_ID::FOREGROUND)),
                    .fontId = 0,
                    .fontSize = 48,
                })
            );
            CLAY(Clay_ElementDeclaration{
                .layout = { .sizing = { .width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW() },
                            .childGap = 16,
                            .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_TOP },
                            .layoutDirection = CLAY_TOP_TO_BOTTOM } }) {
                layout::components::login_input(doc, ctx, "username_input", "Username");
                layout::components::login_input(doc, ctx, "address_input", "Address");
            }
            CLAY_TEXT(
                CLAY_STRING("Login Button"),
                CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                    .textColor = common::to_clay_color(ctx.theme_e.get_color(Color_ID::FOREGROUND)),
                    .fontId = 0,
                    .fontSize = 48,
                })
            );
        }
    }
}
