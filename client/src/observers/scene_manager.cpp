#include "components.hpp"
#include "observers.hpp"
#include "scenes.hpp"

void observers::scene_manager(flecs::world& registry) {
    registry.observer<components::ActiveScene>("scenes::MainMenu on_add")
        .event(flecs::OnAdd)
        .second<components::scenes::MainMenu>()
        .each(scenes::main_menu::on_enter);
    registry.observer<components::ActiveScene>("scenes::MainMenu on_remove")
        .event(flecs::OnRemove)
        .second<components::scenes::MainMenu>()
        .each(scenes::main_menu::on_exit);

    registry.observer<components::ActiveScene>("scenes::Game on_add")
        .event(flecs::OnAdd)
        .second<components::scenes::Game>()
        .each(scenes::game::on_enter);
    registry.observer<components::ActiveScene>("scenes::Game on_remove")
        .event(flecs::OnRemove)
        .second<components::scenes::Game>()
        .each(scenes::game::on_exit);
}
