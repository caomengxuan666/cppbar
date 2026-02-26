#pragma once

#include <cstdint>
#include <string>

namespace cppbar {
namespace color {

struct RGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    constexpr RGB(uint8_t red, uint8_t green, uint8_t blue) : r(red), g(green), b(blue) {}

    constexpr RGB() : r(0), g(0), b(0) {}

    std::string to_ansi_foreground() const;
    std::string to_ansi_background() const;
};

class Color {
public:
    enum class Named {
        Black,
        Red,
        Green,
        Yellow,
        Blue,
        Magenta,
        Cyan,
        White,
        BrightBlack,
        BrightRed,
        BrightGreen,
        BrightYellow,
        BrightBlue,
        BrightMagenta,
        BrightCyan,
        BrightWhite,
        None
    };

    Color() : type_(Type::None) {}

    Color(Named named) : type_(Type::Named), named_(named) {}

    Color(uint8_t r, uint8_t g, uint8_t b) : type_(Type::RGB), rgb_(r, g, b) {}

    Color(const RGB& rgb) : type_(Type::RGB), rgb_(rgb) {}

    bool is_none() const { return type_ == Type::None; }

    std::string to_ansi_foreground() const;
    std::string to_ansi_background() const;

    static Color from_256(uint8_t code);
    static Color from_hex(const std::string& hex);

    static constexpr RGB black() { return RGB(0, 0, 0); }
    static constexpr RGB red() { return RGB(255, 0, 0); }
    static constexpr RGB green() { return RGB(0, 255, 0); }
    static constexpr RGB yellow() { return RGB(255, 255, 0); }
    static constexpr RGB blue() { return RGB(0, 0, 255); }
    static constexpr RGB magenta() { return RGB(255, 0, 255); }
    static constexpr RGB cyan() { return RGB(0, 255, 255); }
    static constexpr RGB white() { return RGB(255, 255, 255); }

    static constexpr RGB bright_black() { return RGB(128, 128, 128); }
    static constexpr RGB bright_red() { return RGB(255, 128, 128); }
    static constexpr RGB bright_green() { return RGB(128, 255, 128); }
    static constexpr RGB bright_yellow() { return RGB(255, 255, 128); }
    static constexpr RGB bright_blue() { return RGB(128, 128, 255); }
    static constexpr RGB bright_magenta() { return RGB(255, 128, 255); }
    static constexpr RGB bright_cyan() { return RGB(128, 255, 255); }
    static constexpr RGB bright_white() { return RGB(255, 255, 255); }

private:
    enum class Type { None, Named, RGB };
    Type type_;
    Named named_;
    RGB rgb_;
};

std::string apply_color(const std::string& text, const Color& fg = Color(),
                        const Color& bg = Color());

std::string reset_color();

}  // namespace color
}  // namespace cppbar