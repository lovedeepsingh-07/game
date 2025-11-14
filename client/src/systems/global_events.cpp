#include "components.hpp"
#include "systems.hpp"
#include <clay/clay.h>

void systems::global_events(flecs::world& registry) {
    registry.system()
        .kind<components::phases::OnUpdate>()
        .run([](flecs::iter& iter) {
            flecs::world registry = iter.world();
            if (IsKeyPressed(KEY_ONE)) {
                registry.add<components::ActiveScene, components::scenes::MainMenu>();
            }
            if (IsKeyPressed(KEY_TWO)) {
                registry.add<components::ActiveScene, components::scenes::Game>();
            }
            if (WindowShouldClose()) {
                registry.add<components::events::GameQuitEvent>();
            }
            if (IsKeyPressed(KEY_F9)) {
                Clay_SetDebugModeEnabled(!Clay_IsDebugModeEnabled());
            }
        })
        .add<components::scenes::Common>();
}
