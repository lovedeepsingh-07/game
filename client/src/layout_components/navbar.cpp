#include "layout.hpp"

void layout::components::navbar(Context& ctx) {
    CLAY(Clay_ElementDeclaration{
        .layout = { .sizing = { .width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIT(40) },
                    .padding = { .left = 8, .right = 8, .top = 6, .bottom = 6 },
                    .layoutDirection = CLAY_LEFT_TO_RIGHT },
        .floating = { .attachPoints = { .element = CLAY_ATTACH_POINT_RIGHT_TOP,
                                        .parent = CLAY_ATTACH_POINT_RIGHT_TOP },
                      .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_CAPTURE,
                      .attachTo = CLAY_ATTACH_TO_ROOT } }) {
        // ------ filling ------
        CLAY(Clay_ElementDeclaration{
            .layout = { .sizing = { .width = CLAY_SIZING_GROW() } } }) {}
        layout::components::navbar_button(ctx);
    }
}
