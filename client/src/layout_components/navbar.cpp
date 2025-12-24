#include "layout.hpp"

void layout::components::navbar(Document& doc, Context& ctx) {
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
        // ------ theme button ------
        if (layout::components::navbar_button(doc, ctx, "theme_button", "theme")) {
            if (ctx.theme_e.get_curr_theme() == "dark") {
                ctx.theme_e.set_curr_theme("light");
            } else {
                ctx.theme_e.set_curr_theme("dark");
            }
        }
    }
}
