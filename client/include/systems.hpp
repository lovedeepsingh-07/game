#pragma once

#include <flecs.h>

namespace systems {
    void global_events(flecs::world& registry);
    void controller(flecs::world& registry);
    namespace render {
        void setup(flecs::world& registry);
    };

    void setup(flecs::world& registry);
};
