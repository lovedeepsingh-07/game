#include "app_utils.hpp"
#include "components.hpp"
#include "user_interface.hpp"

void user_interface::main_menu(flecs::world& registry) {
    CLAY({ .id = CLAY_ID("main_menu_MainContainer"),
           .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
                       .padding = CLAY_PADDING_ALL(16) } }) {
        CLAY_TEXT(
            CLAY_STRING("Main Menu"),
            CLAY_TEXT_CONFIG({
                .textColor = app_utils::raylib_color_to_clay_color(WHITE),
                .fontId = 0,
                .fontSize = 25,
            })
        );
        CLAY({ .id = CLAY_ID("main_menu_quit_button"),
               .layout = { .sizing = { .width = { 120 }, .height = { 20 } },
                           .padding = CLAY_PADDING_ALL(2),
                           .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER } },
               .backgroundColor = Clay_Hovered()
                   ? app_utils::raylib_color_to_clay_color(GRAY)
                   : app_utils::raylib_color_to_clay_color(WHITE) }) {
            bool hovering = Clay_Hovered();
            if (hovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                registry.add<components::events::GameQuitEvent>();
            }
            CLAY_TEXT(
                CLAY_STRING("Quit"),
                CLAY_TEXT_CONFIG({
                    .textColor = hovering ? app_utils::raylib_color_to_clay_color(WHITE)
                                          : app_utils::raylib_color_to_clay_color(BLACK),
                    .fontId = 0,
                    .fontSize = 25,
                })
            );
        };
    }
}
