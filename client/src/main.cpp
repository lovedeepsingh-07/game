#include "app_utils.hpp"
#include "components.hpp"
#include "constants.hpp"
#include "observers.hpp"
#include "phases.hpp"
#include "pipelines.hpp"
#include "systems.hpp"
#include <clay_raylib.hpp>

int main() {
    // ------ raylib setup ------
    Clay_Raylib_Initialize(constants::SCREEN_WIDTH, constants::SCREEN_HEIGHT, "flecs-raylib-scene-manager-clay", FLAG_MSAA_4X_HINT);
    SetTargetFPS(constants::TARGET_FPS);
    SetExitKey(0);

    // ------ registry setup ------
    flecs::world registry;
    registry.set_target_fps(constants::TARGET_FPS);
    registry.entity("scene_root").set_alias("scene_root").add<components::SceneRoot>();
    components::setup(registry);
    phases::setup(registry);
    pipelines::setup(registry);
    observers::setup(registry);
    systems::setup(registry);

    // ------ clay UI setup ------
    uint64_t clay_required_memory = Clay_MinMemorySize();
    auto clay_memory = Clay_Arena{
        .capacity = static_cast<size_t>(clay_required_memory),
        .memory = (char*)malloc(clay_required_memory),
    };
    Clay_Initialize(
        clay_memory,
        Clay_Dimensions{
            .width = (float)constants::SCREEN_WIDTH,
            .height = (float)constants::SCREEN_HEIGHT,
        },
        (Clay_ErrorHandler){ app_utils::clay_error_handler }
    );

    // ------ font setup ------
    std::array<Font, 1> font_list{
        LoadFontEx("assets/JetBrainsMonoNLNerdFontComplete-Regular.ttf", 48, NULL, 400)
    };
    SetTextureFilter(font_list[0].texture, TEXTURE_FILTER_BILINEAR);
    Clay_SetMeasureTextFunction(Raylib_MeasureText, font_list.data());
    registry.set<components::global_options::GameFonts>({ font_list });

    // switch active scene to MainMenu
    registry.add<components::ActiveScene, components::scenes::MainMenu>();

    // run the main loop with flecs studio enabled
    registry.app().enable_stats().enable_rest().run();

    Clay_Raylib_Close();
    return 0;
}
