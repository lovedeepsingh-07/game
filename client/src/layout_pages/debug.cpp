#include "layout.hpp"
#include <raylib.h>

void layout::pages::debug(Document& doc, Context& ctx) {
    CLAY(Clay_ElementDeclaration{
        .layout = { .sizing = { .width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW() },
                    .childGap = 20,
                    .layoutDirection = CLAY_TOP_TO_BOTTOM },
    }) {
        // ------ button display ------
        CLAY(Clay_ElementDeclaration{
            .layout = { .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIT(0) },
                        .childGap = 20,
                        .layoutDirection = CLAY_LEFT_TO_RIGHT } }) {
            if (layout::components::login_button(doc, ctx, "debug_redirect_button", "Redirect")) {
                doc.set_curr_page("login");
            }
        }

        // ------ fg/bg color display ------
        CLAY(Clay_ElementDeclaration{
            .layout = { .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIT(0) },
                        .childGap = 20,
                        .layoutDirection = CLAY_LEFT_TO_RIGHT } }) {
            CLAY(Clay_ElementDeclaration{
                .layout = { .sizing = { .width = CLAY_SIZING_FIT(80),
                                        .height = CLAY_SIZING_FIT(30) } },
                .backgroundColor =
                    app_utils::to_clay_color(ctx.theme_e.get_color(Color_ID::CARD)) }) {
                CLAY_TEXT(
                    CLAY_STRING("hello"),
                    CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                        .textColor = app_utils::to_clay_color(ctx.theme_e.get_color(Color_ID::CARD_FOREGROUND)),
                        .fontId = 0,
                        .fontSize = 24,
                    })
                );
            }
            CLAY(Clay_ElementDeclaration{
                .layout = { .sizing = { .width = CLAY_SIZING_FIT(80),
                                        .height = CLAY_SIZING_FIT(30) } },
                .backgroundColor =
                    app_utils::to_clay_color(ctx.theme_e.get_color(Color_ID::POPOVER)) }) {
                CLAY_TEXT(
                    CLAY_STRING("hello"),
                    CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                        .textColor = app_utils::to_clay_color(ctx.theme_e.get_color(Color_ID::POPOVER_FOREGROUND)),
                        .fontId = 0,
                        .fontSize = 24,
                    })
                );
            }
            CLAY(Clay_ElementDeclaration{
                .layout = { .sizing = { .width = CLAY_SIZING_FIT(80),
                                        .height = CLAY_SIZING_FIT(30) } },
                .backgroundColor =
                    app_utils::to_clay_color(ctx.theme_e.get_color(Color_ID::PRIMARY)) }) {
                CLAY_TEXT(
                    CLAY_STRING("hello"),
                    CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                        .textColor = app_utils::to_clay_color(ctx.theme_e.get_color(Color_ID::PRIMARY_FOREGROUND)),
                        .fontId = 0,
                        .fontSize = 24,
                    })
                );
            }
            CLAY(Clay_ElementDeclaration{
                .layout = { .sizing = { .width = CLAY_SIZING_FIT(80),
                                        .height = CLAY_SIZING_FIT(30) } },
                .backgroundColor =
                    app_utils::to_clay_color(ctx.theme_e.get_color(Color_ID::SECONDARY)) }) {
                CLAY_TEXT(
                    CLAY_STRING("hello"),
                    CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                        .textColor = app_utils::to_clay_color(ctx.theme_e.get_color(Color_ID::SECONDARY_FOREGROUND)),
                        .fontId = 0,
                        .fontSize = 24,
                    })
                );
            }
            CLAY(Clay_ElementDeclaration{
                .layout = { .sizing = { .width = CLAY_SIZING_FIT(80),
                                        .height = CLAY_SIZING_FIT(30) } },
                .backgroundColor =
                    app_utils::to_clay_color(ctx.theme_e.get_color(Color_ID::MUTED)) }) {
                CLAY_TEXT(
                    CLAY_STRING("hello"),
                    CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                        .textColor = app_utils::to_clay_color(ctx.theme_e.get_color(Color_ID::MUTED_FOREGROUND)),
                        .fontId = 0,
                        .fontSize = 24,
                    })
                );
            }
            CLAY(Clay_ElementDeclaration{
                .layout = { .sizing = { .width = CLAY_SIZING_FIT(80),
                                        .height = CLAY_SIZING_FIT(30) } },
                .backgroundColor =
                    app_utils::to_clay_color(ctx.theme_e.get_color(Color_ID::ACCENT)) }) {
                CLAY_TEXT(
                    CLAY_STRING("hello"),
                    CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                        .textColor = app_utils::to_clay_color(ctx.theme_e.get_color(Color_ID::ACCENT_FOREGROUND)),
                        .fontId = 0,
                        .fontSize = 24,
                    })
                );
            }
            CLAY(Clay_ElementDeclaration{
                .layout = { .sizing = { .width = CLAY_SIZING_FIT(80),
                                        .height = CLAY_SIZING_FIT(30) } },
                .backgroundColor =
                    app_utils::to_clay_color(ctx.theme_e.get_color(Color_ID::DESTRUCTIVE)) }) {
                CLAY_TEXT(
                    CLAY_STRING("hello"),
                    CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                        .textColor = app_utils::to_clay_color(ctx.theme_e.get_color(Color_ID::DESTRUCTIVE_FOREGROUND)),
                        .fontId = 0,
                        .fontSize = 24,
                    })
                );
            }
            CLAY(Clay_ElementDeclaration{
                .layout = { .sizing = { .width = CLAY_SIZING_FIT(80),
                                        .height = CLAY_SIZING_FIT(30) } },
                .backgroundColor =
                    app_utils::to_clay_color(ctx.theme_e.get_color(Color_ID::SIDEBAR)) }) {
                CLAY_TEXT(
                    CLAY_STRING("hello"),
                    CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                        .textColor = app_utils::to_clay_color(ctx.theme_e.get_color(Color_ID::SIDEBAR_FOREGROUND)),
                        .fontId = 0,
                        .fontSize = 24,
                    })
                );
            }
            CLAY(Clay_ElementDeclaration{
                .layout = { .sizing = { .width = CLAY_SIZING_FIT(80),
                                        .height = CLAY_SIZING_FIT(30) } },
                .backgroundColor =
                    app_utils::to_clay_color(ctx.theme_e.get_color(Color_ID::SIDEBAR_PRIMARY)) }) {
                CLAY_TEXT(
                    CLAY_STRING("hello"),
                    CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                        .textColor = app_utils::to_clay_color(ctx.theme_e.get_color(Color_ID::SIDEBAR_PRIMARY_FOREGROUND)),
                        .fontId = 0,
                        .fontSize = 24,
                    })
                );
            }
            CLAY(Clay_ElementDeclaration{
                .layout = { .sizing = { .width = CLAY_SIZING_FIT(80),
                                        .height = CLAY_SIZING_FIT(30) } },
                .backgroundColor =
                    app_utils::to_clay_color(ctx.theme_e.get_color(Color_ID::SIDEBAR_ACCENT)) }) {
                CLAY_TEXT(
                    CLAY_STRING("hello"),
                    CLAY_TEXT_CONFIG(Clay_TextElementConfig{
                        .textColor = app_utils::to_clay_color(ctx.theme_e.get_color(Color_ID::SIDEBAR_ACCENT_FOREGROUND)),
                        .fontId = 0,
                        .fontSize = 24,
                    })
                );
            }
        }
        // ------ input display ------
        CLAY(Clay_ElementDeclaration{
            .layout = { .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIT(0) },
                        .childGap = 20,
                        .layoutDirection = CLAY_LEFT_TO_RIGHT } }) {
            layout::components::login_input(doc, ctx, "debug_login_input", "login input");
        }
    }
}
