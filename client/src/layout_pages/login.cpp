#include "debug.hpp"
#include "layout.hpp"
#include "theme_engine.hpp"

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
            .backgroundColor = app_utils::to_clay_color(ctx.theme_e.get_color(Color_ID::CARD)),
            .cornerRadius = CLAY_CORNER_RADIUS(ctx.theme_e.get_radius()),
            .border = { .color = app_utils::to_clay_color(ctx.theme_e.get_color(Color_ID::BORDER)),
                        .width = ctx.theme_e.get_border_width() } }) {
            CLAY_TEXT(
                CLAY_STRING("Login"),
                CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                    .textColor = app_utils::to_clay_color(ctx.theme_e.get_color(Color_ID::FOREGROUND)),
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
            if (layout::components::login_button(doc, ctx, "login_button", "Login")) {
                std::string username_input = app_utils::trim_whitespace(
                    doc.get_element<elements::Input>("username_input")->value
                );
                std::string address_input = app_utils::trim_whitespace(
                    doc.get_element<elements::Input>("address_input")->value
                );
                if (username_input.size() == 0 || address_input.size() == 0) {
                    debug::error("username or address cannot be empty");
                } else {
                    debug::info(fmt::format("username, address: ({}, {})", username_input, address_input));
                }
            };
        }
    }
}
