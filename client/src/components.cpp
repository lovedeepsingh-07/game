#include "components.hpp"

void components::setup(flecs::world& registry) {
    registry.component<components::SceneRoot>();
    registry.component<components::ActiveScene>().add(flecs::Relationship).add(flecs::Exclusive);
    registry.component<components::scenes::MainMenu>().add(flecs::Target);
    registry.component<components::scenes::Game>().add(flecs::Target);
    registry.component<components::pipelines::MainMenu>().add(flecs::Singleton);
    registry.component<components::pipelines::Game>().add(flecs::Singleton);

    registry.component<components::global_options::GameFonts>().add(flecs::Singleton);

    registry.component<components::events::GameQuitEvent>().add(flecs::Singleton);

    registry.component<components::Position>();
    registry.component<components::Rectangle>();
    registry.component<components::Text>();
}
