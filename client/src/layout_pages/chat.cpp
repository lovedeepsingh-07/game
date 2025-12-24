#include "debug.hpp"
#include "layout.hpp"
#include "theme_engine.hpp"
#include <raylib.h>

void layout::pages::chat(Document& doc, Context& ctx) {
    CLAY(Clay_ElementDeclaration{
        .layout = { .sizing = { .width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW() },
                    .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_TOP },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM } }) {
        // ------ message list ------
        CLAY(Clay_ElementDeclaration{
            .layout = { .sizing = { .width = CLAY_SIZING_GROW(160, 1000), .height = CLAY_SIZING_GROW() },
                        .padding = { .bottom = 24 },
                        .childGap = 16,
                        .layoutDirection = CLAY_TOP_TO_BOTTOM },
            .clip = { .vertical = true, .childOffset = Clay_GetScrollOffset() } }) {}
        CLAY(Clay_ElementDeclaration{
            .layout = { .sizing = { .width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIT() },
                        .padding = { .top = 20 },
                        .childGap = 12,
                        .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
                        .layoutDirection = CLAY_LEFT_TO_RIGHT } }) {
            layout::components::message_input(doc, ctx, "message_input", "Enter Message");
            if (layout::components::send_button(doc, ctx, "send_button", "Send")) {
                auto* message_element =
                    doc.get_element<elements::Input>("message_input");
                if (message_element != nullptr) {
                    std::string message_input =
                        app_utils::trim_whitespace(message_element->value);
                    if (message_input.size() != 0) {
                        debug::info(fmt::format("user said message: {}", message_input));
                        message_element->value = "";
                    }
                }
            };
        }
    }
}
