#pragma once

#include <array>
#include <flecs.h>
#include <raylib.h>
#include <string>

namespace components {
    struct SceneRoot {};
    struct ActiveScene {};
    namespace scenes {
        struct Common {};
        struct MainMenu {};
        struct Game {};
    };
    namespace pipelines {
        struct MainMenu {
            flecs::entity pipeline;
        };
        struct Game {
            flecs::entity pipeline;
        };
    };
    namespace phases {
        struct Phase {};
        struct OnUpdate {};
        struct OnRender_Start {};
        struct OnRender_Game {};
        struct OnRender_UI {};
        struct OnRender_Debug {};
        struct OnRender_Finish {};
    }
    namespace global_options {
        struct Paused {};
        struct GameFonts {
            std::array<Font, 1> font_list;
        };
    };
    namespace events {
        struct GameQuitEvent {};
    };

    struct Controller {};
    struct Position {
        float x;
        float y;
    };
    struct Circle {
        float radius;
        Color color;
    };
    struct Rectangle {
        float width;
        float height;
        Color color;
    };
    struct Text {
        std::string text;
        int font_size;
        Color color;
    };

    void setup(flecs::world& registry);
}
