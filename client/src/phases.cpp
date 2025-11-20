#include "phases.hpp"
#include "components.hpp"

/* OnUpdate
 * OnRender_Start
 * OnRender_Game
 * OnRender_UI
 * OnRender_Debug
 * OnRender_Finish
 */

void phases::setup(flecs::world& registry) {
    registry.component<components::phases::Phase>();

    // OnUpdate
    registry.component<components::phases::OnUpdate>().add<components::phases::Phase>();

    // OnRender_Start
    registry.component<components::phases::OnRender_Start>()
        .add<components::phases::Phase>()
        .depends_on<components::phases::OnUpdate>();

    // OnRender_Game
    registry.component<components::phases::OnRender_Game>()
        .add<components::phases::Phase>()
        .depends_on<components::phases::OnRender_Start>();

    // OnRender_UI
    registry.component<components::phases::OnRender_UI>()
        .add<components::phases::Phase>()
        .depends_on<components::phases::OnRender_Game>();

    // OnRender_Debug
    registry.entity<components::phases::OnRender_Debug>()
        .add<components::phases::Phase>()
        .depends_on<components::phases::OnRender_UI>();

    // OnRender_Finish
    registry.entity<components::phases::OnRender_Finish>()
        .add<components::phases::Phase>()
        .depends_on<components::phases::OnRender_Debug>();
}
