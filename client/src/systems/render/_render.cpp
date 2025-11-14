#include "components.hpp"
#include "systems.hpp"
#include "user_interface.hpp"
#include <clay_raylib.hpp>

void systems::render::setup(flecs::world& registry) {
    registry.system()
        .kind<components::phases::OnRender_Start>()
        .run([](flecs::iter& iter) {
            BeginDrawing();
            ClearBackground(BLACK);
        })
        .add<components::scenes::Common>();
    registry.system<components::Rectangle, components::Position>()
        .kind<components::phases::OnRender_Game>()
        .each([](flecs::entity curr_entity, const components::Rectangle& rect,
                 const components::Position& pos) {
            DrawRectangle((int)pos.x, (int)pos.y, (int)rect.width, (int)rect.height, BLUE);
        })
        .add<components::scenes::MainMenu>();
    registry.system<components::Rectangle, components::Position>()
        .kind<components::phases::OnRender_Game>()
        .each([](flecs::entity curr_entity, const components::Rectangle& rect,
                 const components::Position& pos) {
            DrawRectangle((int)pos.x, (int)pos.y, (int)rect.width, (int)rect.height, RED);
        })
        .add<components::scenes::Game>();
    registry.system<components::Text, components::Position>()
        .kind<components::phases::OnRender_UI>()
        .each([](flecs::entity curr_entity, const components::Text& text,
                 const components::Position& pos) {
            DrawText(text.text.c_str(), (int)pos.x, (int)pos.y, text.font_size, text.color);
        })
        .add<components::scenes::Common>();
    registry.system()
        .kind<components::phases::OnRender_UI>()
        .run([](flecs::iter& iter) {
            flecs::world registry = iter.world();
            auto game_fonts = registry.get<components::global_options::GameFonts>();
            Vector2 mouse_pos = GetMousePosition();
            Vector2 scroll_delta = GetMouseWheelMoveV();
            float delta_time = registry.delta_time();

            Clay_SetLayoutDimensions((Clay_Dimensions
            ){ .width = (float)GetScreenWidth(), .height = (float)GetScreenHeight() });
            Clay_SetPointerState(Clay_Vector2{ mouse_pos.x, mouse_pos.y }, IsMouseButtonDown(0));
            Clay_UpdateScrollContainers(
                true, (Clay_Vector2){ scroll_delta.x, scroll_delta.y }, delta_time
            );
            Clay_BeginLayout();
            if (registry.has<components::ActiveScene, components::scenes::MainMenu>()) {
                user_interface::main_menu(registry);
            }
            if (registry.has<components::ActiveScene, components::scenes::Game>()) {
                user_interface::game(registry);
            }
            Clay_RenderCommandArray render_commands = Clay_EndLayout();
            Clay_Raylib_Render(render_commands, game_fonts.font_list.data());
        })
        .add<components::scenes::Common>();
    registry.system()
        .kind<components::phases::OnRender_Finish>()
        .run([](flecs::iter& iter) { EndDrawing(); })
        .add<components::scenes::Common>();
}
