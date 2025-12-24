#pragma once

#include <clay/clay.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <raylib.h>
#include <raymath.h>

#define CLAY_RECTANGLE_TO_RAYLIB_RECTANGLE(rectangle)                          \
    (Rectangle) {                                                              \
        .x = rectangle.x, .y = rectangle.y, .width = rectangle.width,          \
        .height = rectangle.height                                             \
    }
#define CLAY_COLOR_TO_RAYLIB_COLOR(color)                                         \
    (Color) {                                                                     \
        .r = (unsigned char)roundf(color.r), .g = (unsigned char)roundf(color.g), \
        .b = (unsigned char)roundf(color.b), .a = (unsigned char)roundf(color.a)  \
    }

const Camera Raylib_camera{};

enum class CustomLayoutElementType : uint8_t {
    CUSTOM_LAYOUT_ELEMENT_TYPE_3D_MODEL
};

struct CustomLayoutElement_3DModel {
    Model model;
    float scale;
    Vector3 position;
    Matrix rotation;
};

struct CustomLayoutElement {
    CustomLayoutElementType type;
    union {
        CustomLayoutElement_3DModel model;
    } customData;
};

// Get a ray trace from the screen position (i.e mouse) within a specific section of the screen
Ray GetScreenToWorldPointWithZDistance(
    Vector2 position, Camera camera, int screenWidth, int screenHeight, float zDistance
);


Clay_Dimensions
Raylib_MeasureText(Clay_StringSlice text, Clay_TextElementConfig* config, void* userData);

void Clay_Raylib_Initialize(int width, int height, const char* title, unsigned int flags);

// A MALLOC'd buffer, that we keep modifying inorder to save from so many Malloc
// and Free Calls. Call Clay_Raylib_Close() to free
static char* temp_render_buffer = NULL;
static int temp_render_buffer_len = 0;

// Call after closing the window to clean up the render buffer
void Clay_Raylib_Close();

void Clay_Raylib_Render(Clay_RenderCommandArray renderCommands, Font* fonts);
