#pragma once

#include "color.hpp"
#include <sstream>

namespace cppbar {
namespace color {

inline std::string RGB::to_ansi_foreground() const {
    std::ostringstream oss;
    oss << "\033[38;2;" << static_cast<int>(r) << ";" << static_cast<int>(g) << ";"
        << static_cast<int>(b) << "m";
    return oss.str();
}

inline std::string RGB::to_ansi_background() const {
    std::ostringstream oss;
    oss << "\033[48;2;" << static_cast<int>(r) << ";" << static_cast<int>(g) << ";"
        << static_cast<int>(b) << "m";
    return oss.str();
}

inline std::string Color::to_ansi_foreground() const {
    if (type_ == Type::None) {
        return "";
    }

    if (type_ == Type::RGB) {
        return rgb_.to_ansi_foreground();
    }

    const int ansi_colors[] = {30, 31, 32, 33, 34, 35, 36, 37, 90, 91, 92, 93, 94, 95, 96, 97};

    int index = static_cast<int>(named_);
    std::ostringstream oss;
    oss << "\033[" << ansi_colors[index] << "m";
    return oss.str();
}

inline std::string Color::to_ansi_background() const {
    if (type_ == Type::None) {
        return "";
    }

    if (type_ == Type::RGB) {
        return rgb_.to_ansi_background();
    }

    const int ansi_colors[] = {40,  41,  42,  43,  44,  45,  46,  47,
                               100, 101, 102, 103, 104, 105, 106, 107};

    int index = static_cast<int>(named_);
    std::ostringstream oss;
    oss << "\033[" << ansi_colors[index] << "m";
    return oss.str();
}

inline Color Color::from_256(uint8_t code) {
    if (code < 16) {
        int index = code;
        if (index < 8) {
            return static_cast<Named>(index);
        } else {
            return static_cast<Named>(index + 8);
        }
    } else if (code < 232) {
        code -= 16;
        uint8_t r = (code / 36) * 51;
        uint8_t g = ((code % 36) / 6) * 51;
        uint8_t b = (code % 6) * 51;
        return Color(r, g, b);
    } else {
        uint8_t gray = (code - 232) * 10 + 8;
        return Color(gray, gray, gray);
    }
}

inline Color Color::from_hex(const std::string& hex) {
    if (hex.empty() || hex[0] != '#') {
        return Color();
    }

    std::string h = hex.substr(1);

    try {
        if (h.length() == 3) {
            uint8_t r =
                static_cast<uint8_t>(std::stoi(h.substr(0, 1) + h.substr(0, 1), nullptr, 16));
            uint8_t g =
                static_cast<uint8_t>(std::stoi(h.substr(1, 1) + h.substr(1, 1), nullptr, 16));
            uint8_t b =
                static_cast<uint8_t>(std::stoi(h.substr(2, 1) + h.substr(2, 1), nullptr, 16));
            return Color(r, g, b);
        } else if (h.length() == 6) {
            uint8_t r = static_cast<uint8_t>(std::stoi(h.substr(0, 2), nullptr, 16));
            uint8_t g = static_cast<uint8_t>(std::stoi(h.substr(2, 2), nullptr, 16));
            uint8_t b = static_cast<uint8_t>(std::stoi(h.substr(4, 2), nullptr, 16));
            return Color(r, g, b);
        }
    } catch (...) {
        return Color();
    }

    return Color();
}

inline std::string apply_color(const std::string& text, const Color& fg, const Color& bg) {
    std::string result;
    result += fg.to_ansi_foreground();
    result += bg.to_ansi_background();
    result += text;
    result += reset_color();
    return result;
}

inline std::string reset_color() {
    return "\033[0m";
}

}  // namespace color
}  // namespace cppbar