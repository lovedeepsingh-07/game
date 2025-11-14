#include "app_utils.hpp"
#include "user_interface.hpp"

void user_interface::game(flecs::world& registry) {
    CLAY({ .id = CLAY_ID("game_MainContainer"),
           .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
                       .padding = CLAY_PADDING_ALL(16) } }) {
        CLAY_TEXT(
            CLAY_STRING("Game"),
            CLAY_TEXT_CONFIG({
                .textColor = app_utils::raylib_color_to_clay_color(WHITE),
                .fontId = 0,
                .fontSize = 25,
            })
        );
    }
}
