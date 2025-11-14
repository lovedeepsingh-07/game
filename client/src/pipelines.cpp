#include "pipelines.hpp"
#include "components.hpp"

void pipelines::setup(flecs::world& registry) {
    registry.set(components::pipelines::MainMenu{
        .pipeline = registry.pipeline()
                        .with(flecs::System)
                        .with<components::phases::Phase>()
                        .cascade(flecs::DependsOn)
                        .with<components::scenes::MainMenu>()
                        .oper(flecs::Or)
                        .with<components::scenes::Common>()
                        .build() });
    registry.set(components::pipelines::Game{
        .pipeline = registry.pipeline()
                        .with(flecs::System)
                        .with<components::phases::Phase>()
                        .cascade(flecs::DependsOn)
                        .with<components::scenes::Game>()
                        .oper(flecs::Or)
                        .with<components::scenes::Common>()
                        .build() });
}
