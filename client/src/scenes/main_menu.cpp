#include "scenes.hpp"
#include <common/common.hpp>

void scenes::main_menu::on_enter(flecs::iter& iter, size_t, components::ActiveScene) {
    debug::debug("main_menu::on_enter");
    flecs::world registry = iter.world();

    registry.defer_begin();
    registry.delete_with(flecs::ChildOf, registry.lookup("scene_root"));
    registry.defer_end();

    flecs::entity scene_root = registry.lookup("scene_root");

    registry.entity()
        .set(components::Position{ .x = 100, .y = 100 })
        .set(components::Rectangle{ .width = 120, .height = 24, .color = PURPLE })
        .child_of(scene_root);
    registry.entity()
        .set(components::Position{ .x = 100, .y = 100 })
        .set(components::Text{ .text = std::string("main_menu"), .font_size = 24, .color = RAYWHITE })
        .child_of(scene_root);

    registry.set_pipeline(registry.get<components::pipelines::MainMenu>().pipeline);
}

void scenes::main_menu::on_exit(flecs::iter& iter, size_t, components::ActiveScene) {
    debug::debug("main_menu::on_exit");
}
