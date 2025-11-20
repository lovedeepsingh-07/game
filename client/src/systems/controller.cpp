#include "components.hpp"
#include "constants.hpp"
#include "systems.hpp"

void systems::controller(flecs::world& registry) {
    registry
        .system<components::Controller, components::Position>("Player "
                                                              "Controller")
        .kind<components::phases::OnUpdate>()
        .each([](flecs::entity curr_entity, components::Controller, components::Position& pos) {
            if (IsKeyDown(KEY_A)) {
                pos.x -= constants::PLAYER_VEL;
            } else if (IsKeyDown(KEY_D)) {
                pos.x += constants::PLAYER_VEL;
            }
            if (IsKeyDown(KEY_W)) {
                pos.y -= constants::PLAYER_VEL;
            } else if (IsKeyDown(KEY_S)) {
                pos.y += constants::PLAYER_VEL;
            }
        })
        .add<components::scenes::Game>();
}
