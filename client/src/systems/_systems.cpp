#include "systems.hpp"

void systems::setup(flecs::world& registry) {
    systems::global_events(registry);
    systems::render::setup(registry);
}
