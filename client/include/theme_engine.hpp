#pragma once

#include "app_utils.hpp"
#include <string>
#include <unordered_map>

enum class Color_ID : uint8_t {
    BACKGROUND,
    FOREGROUND,
    CARD,
    CARD_FOREGROUND,
    POPOVER,
    POPOVER_FOREGROUND,
    PRIMARY,
    PRIMARY_FOREGROUND,
    SECONDARY,
    SECONDARY_FOREGROUND,
    MUTED,
    MUTED_FOREGROUND,
    ACCENT,
    ACCENT_FOREGROUND,
    DESTRUCTIVE,
    DESTRUCTIVE_FOREGROUND,
    BORDER,
    INPUT,
    RING,
    CHART_1,
    CHART_2,
    CHART_3,
    CHART_4,
    CHART_5,
    SIDEBAR,
    SIDEBAR_FOREGROUND,
    SIDEBAR_PRIMARY,
    SIDEBAR_PRIMARY_FOREGROUND,
    SIDEBAR_ACCENT,
    SIDEBAR_ACCENT_FOREGROUND,
    SIDEBAR_BORDER,
    SIDEBAR_RING
};
struct Color_ID_Hash {
    std::size_t operator()(const Color_ID& color_id) const noexcept {
        return static_cast<size_t>(color_id);
    }
};
struct Theme {
    std::unordered_map<Color_ID, app_utils::Color, Color_ID_Hash> colors;
    float radius;
    Clay_BorderWidth border_width;
};

class ThemeEngine {
  private:
    std::unordered_map<std::string, Theme> m_themes;
    std::string m_curr_theme_id;

  public:
    ThemeEngine() = default;
    ~ThemeEngine() = default;

    // disable copying and moving
    ThemeEngine(const ThemeEngine&) = delete;
    ThemeEngine& operator=(const ThemeEngine&) = delete;
    ThemeEngine(ThemeEngine&&) = delete;
    ThemeEngine& operator=(ThemeEngine&&) = delete;

    void setup();

    void add_theme(const std::string& theme_id, Theme theme);
    void set_curr_theme(const std::string& theme_id);
    const std::string& get_curr_theme();
    app_utils::Color get_color(const Color_ID& color_id);
    float get_radius();
    Clay_BorderWidth get_border_width();
};
