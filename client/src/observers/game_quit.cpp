#include "components.hpp"
#include "observers.hpp"
#include <common/common.hpp>

void observers::game_quit(flecs::world& registry) {
    registry
        .observer<components::events::GameQuitEvent>("events::GameQuitEvent "
                                                     "on_add")
        .event(flecs::OnAdd)
        .run([](flecs::iter& iter) {
            debug::debug("quiting game...");
            flecs::world registry = iter.world();
            registry.quit();
        });
}
