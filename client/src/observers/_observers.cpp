#include "observers.hpp"

void observers::setup(flecs::world& registry) {
    observers::game_quit(registry);
    observers::scene_manager(registry);
}
