#include "layout.hpp"
#include <raylib.h>

void layout::pages::debug(Document& doc, Context& ctx) {
    CLAY(Clay_ElementDeclaration{
        .layout = { .sizing = { .width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW() },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM },
    }) {
        // ------ redirect button ------
        CLAY(Clay_ElementDeclaration{
            .id = CLAY_ID("redirect_button"),
            .layout = { .sizing = { .width = CLAY_SIZING_FIT(92), .height = CLAY_SIZING_FIT(20) },
                        .padding = { .left = 8, .right = 8, .top = 6, .bottom = 6 },
                        .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
                        .layoutDirection = CLAY_TOP_TO_BOTTOM },
            .backgroundColor = common::to_clay_color(ctx.theme_e.get_color(Color_ID::PRIMARY)),
            .cornerRadius = CLAY_CORNER_RADIUS(ctx.theme_e.get_radius()),
            .border = { .color = common::to_clay_color(ctx.theme_e.get_color(Color_ID::BORDER)),
                        .width = { 1, 1, 1, 1, 0 } } }) {
            if (Clay_Hovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                doc.set_curr_page("login");
            }
            CLAY_TEXT(
                CLAY_STRING("redirect"),
                CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                    .textColor = common::to_clay_color(ctx.theme_e.get_color(Color_ID::PRIMARY_FOREGROUND)),
                    .fontId = 0,
                    .fontSize = 20,
                })
            );
        };

        // ------ fg/bg color display ------
        CLAY(Clay_ElementDeclaration{
            .layout = { .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIT(0) },
                        .childGap = 20,
                        .layoutDirection = CLAY_LEFT_TO_RIGHT } }) {
            CLAY(Clay_ElementDeclaration{
                .layout = { .sizing = { .width = CLAY_SIZING_FIT(80),
                                        .height = CLAY_SIZING_FIT(30) } },
                .backgroundColor =
                    common::to_clay_color(ctx.theme_e.get_color(Color_ID::CARD)) }) {
                CLAY_TEXT(
                    CLAY_STRING("hello"),
                    CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                        .textColor = common::to_clay_color(ctx.theme_e.get_color(Color_ID::CARD_FOREGROUND)),
                        .fontId = 0,
                        .fontSize = 24,
                    })
                );
            }
            CLAY(Clay_ElementDeclaration{
                .layout = { .sizing = { .width = CLAY_SIZING_FIT(80),
                                        .height = CLAY_SIZING_FIT(30) } },
                .backgroundColor =
                    common::to_clay_color(ctx.theme_e.get_color(Color_ID::POPOVER)) }) {
                CLAY_TEXT(
                    CLAY_STRING("hello"),
                    CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                        .textColor = common::to_clay_color(ctx.theme_e.get_color(Color_ID::POPOVER_FOREGROUND)),
                        .fontId = 0,
                        .fontSize = 24,
                    })
                );
            }
            CLAY(Clay_ElementDeclaration{
                .layout = { .sizing = { .width = CLAY_SIZING_FIT(80),
                                        .height = CLAY_SIZING_FIT(30) } },
                .backgroundColor =
                    common::to_clay_color(ctx.theme_e.get_color(Color_ID::PRIMARY)) }) {
                CLAY_TEXT(
                    CLAY_STRING("hello"),
                    CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                        .textColor = common::to_clay_color(ctx.theme_e.get_color(Color_ID::PRIMARY_FOREGROUND)),
                        .fontId = 0,
                        .fontSize = 24,
                    })
                );
            }
            CLAY(Clay_ElementDeclaration{
                .layout = { .sizing = { .width = CLAY_SIZING_FIT(80),
                                        .height = CLAY_SIZING_FIT(30) } },
                .backgroundColor =
                    common::to_clay_color(ctx.theme_e.get_color(Color_ID::SECONDARY)) }) {
                CLAY_TEXT(
                    CLAY_STRING("hello"),
                    CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                        .textColor = common::to_clay_color(ctx.theme_e.get_color(Color_ID::SECONDARY_FOREGROUND)),
                        .fontId = 0,
                        .fontSize = 24,
                    })
                );
            }
            CLAY(Clay_ElementDeclaration{
                .layout = { .sizing = { .width = CLAY_SIZING_FIT(80),
                                        .height = CLAY_SIZING_FIT(30) } },
                .backgroundColor =
                    common::to_clay_color(ctx.theme_e.get_color(Color_ID::MUTED)) }) {
                CLAY_TEXT(
                    CLAY_STRING("hello"),
                    CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                        .textColor = common::to_clay_color(ctx.theme_e.get_color(Color_ID::MUTED_FOREGROUND)),
                        .fontId = 0,
                        .fontSize = 24,
                    })
                );
            }
            CLAY(Clay_ElementDeclaration{
                .layout = { .sizing = { .width = CLAY_SIZING_FIT(80),
                                        .height = CLAY_SIZING_FIT(30) } },
                .backgroundColor =
                    common::to_clay_color(ctx.theme_e.get_color(Color_ID::ACCENT)) }) {
                CLAY_TEXT(
                    CLAY_STRING("hello"),
                    CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                        .textColor = common::to_clay_color(ctx.theme_e.get_color(Color_ID::ACCENT_FOREGROUND)),
                        .fontId = 0,
                        .fontSize = 24,
                    })
                );
            }
            CLAY(Clay_ElementDeclaration{
                .layout = { .sizing = { .width = CLAY_SIZING_FIT(80),
                                        .height = CLAY_SIZING_FIT(30) } },
                .backgroundColor =
                    common::to_clay_color(ctx.theme_e.get_color(Color_ID::DESTRUCTIVE)) }) {
                CLAY_TEXT(
                    CLAY_STRING("hello"),
                    CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                        .textColor = common::to_clay_color(ctx.theme_e.get_color(Color_ID::DESTRUCTIVE_FOREGROUND)),
                        .fontId = 0,
                        .fontSize = 24,
                    })
                );
            }
            CLAY(Clay_ElementDeclaration{
                .layout = { .sizing = { .width = CLAY_SIZING_FIT(80),
                                        .height = CLAY_SIZING_FIT(30) } },
                .backgroundColor =
                    common::to_clay_color(ctx.theme_e.get_color(Color_ID::SIDEBAR)) }) {
                CLAY_TEXT(
                    CLAY_STRING("hello"),
                    CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                        .textColor = common::to_clay_color(ctx.theme_e.get_color(Color_ID::SIDEBAR_FOREGROUND)),
                        .fontId = 0,
                        .fontSize = 24,
                    })
                );
            }
            CLAY(Clay_ElementDeclaration{
                .layout = { .sizing = { .width = CLAY_SIZING_FIT(80),
                                        .height = CLAY_SIZING_FIT(30) } },
                .backgroundColor =
                    common::to_clay_color(ctx.theme_e.get_color(Color_ID::SIDEBAR_PRIMARY)) }) {
                CLAY_TEXT(
                    CLAY_STRING("hello"),
                    CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                        .textColor = common::to_clay_color(ctx.theme_e.get_color(Color_ID::SIDEBAR_PRIMARY_FOREGROUND)),
                        .fontId = 0,
                        .fontSize = 24,
                    })
                );
            }
            CLAY(Clay_ElementDeclaration{
                .layout = { .sizing = { .width = CLAY_SIZING_FIT(80),
                                        .height = CLAY_SIZING_FIT(30) } },
                .backgroundColor =
                    common::to_clay_color(ctx.theme_e.get_color(Color_ID::SIDEBAR_ACCENT)) }) {
                CLAY_TEXT(
                    CLAY_STRING("hello"),
                    CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                        .textColor = common::to_clay_color(ctx.theme_e.get_color(Color_ID::SIDEBAR_ACCENT_FOREGROUND)),
                        .fontId = 0,
                        .fontSize = 24,
                    })
                );
            }
        }
    }
}
