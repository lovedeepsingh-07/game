#include "phases.hpp"
#include "components.hpp"

void phases::setup(flecs::world& registry) {
    registry.component<components::phases::Phase>();
    registry.component<components::phases::OnUpdate>().add<components::phases::Phase>();
    registry.component<components::phases::OnRender_Start>()
        .add<components::phases::Phase>()
        .depends_on<components::phases::OnUpdate>();
    registry.component<components::phases::OnRender_Game>()
        .add<components::phases::Phase>()
        .depends_on<components::phases::OnRender_Start>();
    registry.component<components::phases::OnRender_UI>()
        .add<components::phases::Phase>()
        .depends_on<components::phases::OnRender_Game>();
    registry.entity<components::phases::OnRender_Debug>()
        .add<components::phases::Phase>()
        .depends_on<components::phases::OnRender_UI>();
    registry.entity<components::phases::OnRender_Finish>()
        .add<components::phases::Phase>()
        .depends_on<components::phases::OnRender_Debug>();
}
