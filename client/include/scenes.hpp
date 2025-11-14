#pragma once

#include "components.hpp"
#include <flecs.h>

namespace scenes {
    namespace main_menu {
        void on_enter(flecs::iter& iter, std::size_t, components::ActiveScene);
        void on_exit(flecs::iter& iter, std::size_t, components::ActiveScene);
    };
    namespace game {
        void on_enter(flecs::iter& iter, std::size_t, components::ActiveScene);
        void on_exit(flecs::iter& iter, std::size_t, components::ActiveScene);
    };
};
