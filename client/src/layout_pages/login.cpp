#include "debug.hpp"
#include "error.hpp"
#include "layout.hpp"
#include "theme_engine.hpp"
#include <net.h>
#include <raylib.h>

constexpr int min_card_width = 320;
bool trying_to_connect = false;

void layout::pages::login(Document& doc, Context& ctx) {
    if (trying_to_connect) {
        // ------ net::connect_client() ------
        try {
            net::connect_client((uint64_t)GetFrameTime() * 1000.0F);
        } catch (rust::Error e) {
            auto err = error::Error::from_rust(e);
            if (err.kind == error::Error::Kind::AuthError) {
                debug::error(fmt::format("Failed to connect to the server, {}", err.to_string()));
                debug::info("Try using a different username");
                // ------ net::reset_client() ------
                try {
                    net::reset_client();
                } catch (rust::Error e) {
                    auto err = error::Error::from_rust(e);
                    debug::error(fmt::format("Failed to reset the client state, {}", err.to_string()));
                }
                // ------ net::reset_client() ------
            }
        }
        // ------ net::connect_client() ------
        if (net::is_client_connected()) {
            trying_to_connect = false;
            doc.set_curr_page("chat");
        }
        // ------ net::send_packets() ------
        try {
            net::send_packets();
        } catch (rust::Error e) {
            auto err = error::Error::from_rust(e);
            if (err.kind != error::Error::Kind::StateNotInitializedError) {
                debug::error(fmt::format("Failed to send packets to the server, {}", err.to_string()));
                std::exit(1);
            }
        }
        // ------ net::send_packets() ------
    }
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
            }
            if (layout::components::login_button(doc, ctx, "login_button", net::is_client_connecting() ? "Connecting..." : "Login")
                && !trying_to_connect) {
                std::string username_input = app_utils::trim_whitespace(
                    doc.get_element<elements::Input>("username_input")->value
                );
                if (username_input.size() < 8) {
                    debug::error("Username should be atleast 8 characters long");
                } else {
                    // ------ net::setup_client() ------
                    try {
                        net::setup_client(username_input);
                    } catch (rust::Error e) {
                        auto err = error::Error::from_rust(e);
                        if (err.kind != error::Error::Kind::StateAlreadyInitializedError) {
                            debug::error(fmt::format("Failed to setup the client, {}", err.to_string()));
                            std::exit(1);
                        }
                    }
                    // ------ net::setup_client() ------
                    trying_to_connect = true;
                }
            };
        }
    }
}
