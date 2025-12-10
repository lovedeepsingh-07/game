#include "layout.hpp"
#include <array>
#include <clay_raylib.hpp>
#include <common/common.hpp>
#include <common/constants.hpp>
#include <common/context.hpp>

int main() {
    Clay_Raylib_Initialize(540, 750, "boron", FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(constants::TARGET_FPS);

    // clay setup
    uint64_t clay_required_memory = Clay_MinMemorySize();
    Clay_Arena clay_memory =
        Clay_CreateArenaWithCapacityAndMemory(clay_required_memory, malloc(clay_required_memory));
    Clay_Initialize(
        clay_memory,
        (Clay_Dimensions){ .width = (float)GetScreenWidth(),
                           .height = (float)GetScreenHeight() },
        (Clay_ErrorHandler){ common::clay_error_handler }
    );

    // font setup
    std::array<Font, 1> font_list;
    font_list[0] = LoadFontEx("./assets/Poppins-Regular.ttf", 48, NULL, 400);
    SetTextureFilter(font_list[0].texture, TEXTURE_FILTER_BILINEAR);
    Clay_SetMeasureTextFunction(Raylib_MeasureText, font_list.data());

    // context
    Context ctx;
    ctx.setup();
    ctx.theme_e.set_curr_theme("dark");

    // DOM
    Document doc;
    layout::pages::setup(doc);

    while (!WindowShouldClose()) {
        // clear the memory arena(s) at the starting of every frame
        ctx.frame_arena.clear();
        ctx.message_arena.clear();

        // TODO: some debugging specific code
        if (Clay_IsDebugModeEnabled()) {
            if (IsKeyPressed(KEY_ZERO)) {
                doc.set_curr_page("debug");
            } else if (IsKeyPressed(KEY_ONE)) {
                doc.set_curr_page("login");
            } else if (IsKeyPressed(KEY_TWO)) {
                doc.set_curr_page("chat");
            }
        }

        // debugging mode toggle
        if (IsKeyPressed(KEY_F9)) {
            Clay_SetDebugModeEnabled(!Clay_IsDebugModeEnabled());
        }

        // window state
        Vector2 mouse_pos = GetMousePosition();
        Vector2 scroll_delta =
            Vector2Scale(GetMouseWheelMoveV(), 5); // scroll delta scaled 2 times
        float delta_time = GetFrameTime();

        // update clay state
        Clay_SetLayoutDimensions((Clay_Dimensions){ (float)GetScreenWidth(),
                                                    (float)GetScreenHeight() });
        Clay_SetPointerState((Clay_Vector2){ mouse_pos.x, mouse_pos.y }, IsMouseButtonDown(0));
        Clay_UpdateScrollContainers(
            true, (Clay_Vector2){ scroll_delta.x, scroll_delta.y }, delta_time
        );

        Clay_BeginLayout();
        layout::components::navbar(doc, ctx);
        CLAY(Clay_ElementDeclaration{
            .id = CLAY_ID("main_container"),
            .layout = { .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) },
                        .padding = { .left = 16, .right = 16, .top = 60, .bottom = 16 },
                        .layoutDirection = CLAY_TOP_TO_BOTTOM },
            .backgroundColor =
                common::to_clay_color(ctx.theme_e.get_color(Color_ID::BACKGROUND)) }) {
            doc.render_curr_page(doc, ctx);
        }
        Clay_RenderCommandArray renderCommands = Clay_EndLayout();

        BeginDrawing();
        Clay_Raylib_Render(renderCommands, font_list.data());
        EndDrawing();
    }

    // unload the fonts once the main loop exits
    for (auto& curr_font : font_list) {
        UnloadFont(curr_font);
    }
    return 0;
}
