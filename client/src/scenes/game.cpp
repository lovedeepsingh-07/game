#include "components.hpp"
#include "scenes.hpp"
#include <common/common.hpp>

void scenes::game::on_enter(flecs::iter& iter, size_t, components::ActiveScene) {
    debug::debug("game::on_enter");
    flecs::world registry = iter.world();

    registry.defer_begin();
    registry.delete_with(flecs::ChildOf, registry.lookup("scene_root"));
    registry.defer_end();

    flecs::entity scene_root = registry.lookup("scene_root");

    registry.entity("player")
        .set_alias("player")
        .add<components::Controller>()
        .set(components::Position{ 100, 100 })
        .set(components::Circle{ .radius = 20, .color = BLUE })
        .child_of(scene_root);

    registry.entity()
        .set(components::Position{ .x = 100, .y = 100 })
        .set(components::Rectangle{ .width = 120, .height = 24, .color = PURPLE })
        .child_of(scene_root);
    registry.entity()
        .set(components::Position{ .x = 100, .y = 100 })
        .set(components::Text{ .text = std::string("game"), .font_size = 24, .color = RAYWHITE })
        .child_of(scene_root);

    registry.set_pipeline(registry.get<components::pipelines::Game>().pipeline);
}

void scenes::game::on_exit(flecs::iter& iter, size_t, components::ActiveScene) {
    debug::debug("game::on_exit");
    flecs::world registry = iter.world();
    if (registry.has<components::global_options::Paused>()) {
        registry.remove<components::global_options::Paused>();
    }
}
