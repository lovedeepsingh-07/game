#pragma once

#include <flecs.h>

namespace observers {
    void scene_manager(flecs::world& registry);
    void game_quit(flecs::world& registry);

    void setup(flecs::world& registry);
};
