#pragma once

#include <algorithm>
#include <string>

namespace cppbar {
namespace utils {

class Math {
public:
    static int clamp(int value, int min_val, int max_val) {
        return std::max(min_val, std::min(value, max_val));
    }

    static double clamp(double value, double min_val, double max_val) {
        return std::max(min_val, std::min(value, max_val));
    }

    static double lerp(double a, double b, double t) { return a + (b - a) * t; }

    static double ease_in_out(double t) { return t * t * (3.0 - 2.0 * t); }

    static double ease_in(double t) { return t * t; }

    static double ease_out(double t) { return t * (2.0 - t); }

    static int calculate_eta(int64_t current, int64_t total, double elapsed_seconds) {
        if (total <= 0 || current <= 0 || elapsed_seconds <= 0) {
            return 0;
        }

        double rate = static_cast<double>(current) / elapsed_seconds;
        if (rate <= 0) {
            return 0;
        }

        int64_t remaining = total - current;
        return static_cast<int>(remaining / rate);
    }

    static double calculate_speed(int64_t current, double elapsed_seconds) {
        if (elapsed_seconds <= 0) {
            return 0.0;
        }
        return static_cast<double>(current) / elapsed_seconds;
    }

    static int calculate_filled_width(int total_width, double percentage) {
        return static_cast<int>(total_width * percentage / 100.0);
    }

    static int calculate_display_width(const std::string& str) {
        int width = 0;
        for (size_t i = 0; i < str.length();) {
            unsigned char c = static_cast<unsigned char>(str[i]);

            if (c < 0x80) {
                width += 1;
                i += 1;
            } else if ((c & 0xE0) == 0xC0) {
                width += 1;
                i += 2;
            } else if ((c & 0xF0) == 0xE0) {
                width += is_wide_char(str, i) ? 2 : 1;
                i += 3;
            } else if ((c & 0xF8) == 0xF0) {
                width += is_wide_char(str, i) ? 2 : 1;
                i += 4;
            } else {
                width += 1;
                i += 1;
            }
        }
        return width;
    }

    static bool is_wide_char(const std::string& str, size_t pos) {
        if (pos >= str.length()) {
            return false;
        }

        unsigned char c1 = static_cast<unsigned char>(str[pos]);
        if ((c1 & 0xF0) != 0xE0 && (c1 & 0xF8) != 0xF0) {
            return false;
        }

        uint32_t codepoint = 0;

        if ((c1 & 0xF0) == 0xE0) {
            if (pos + 2 >= str.length())
                return false;
            unsigned char c2 = static_cast<unsigned char>(str[pos + 1]);
            unsigned char c3 = static_cast<unsigned char>(str[pos + 2]);
            codepoint = ((c1 & 0x0F) << 12) | ((c2 & 0x3F) << 6) | (c3 & 0x3F);
        } else if ((c1 & 0xF8) == 0xF0) {
            if (pos + 3 >= str.length())
                return false;
            unsigned char c2 = static_cast<unsigned char>(str[pos + 1]);
            unsigned char c3 = static_cast<unsigned char>(str[pos + 2]);
            unsigned char c4 = static_cast<unsigned char>(str[pos + 3]);
            codepoint =
                ((c1 & 0x07) << 18) | ((c2 & 0x3F) << 12) | ((c3 & 0x3F) << 6) | (c4 & 0x3F);
        }

        return is_east_asian_wide(codepoint);
    }

private:
    static bool is_east_asian_wide(uint32_t codepoint) {
        if (codepoint >= 0x1100 && codepoint <= 0x115F)
            return true;
        if (codepoint >= 0x2329 && codepoint <= 0x232A)
            return true;
        if (codepoint >= 0x2E80 && codepoint <= 0x303E)
            return true;
        if (codepoint >= 0x3040 && codepoint <= 0xA4CF)
            return true;
        if (codepoint >= 0xAC00 && codepoint <= 0xD7A3)
            return true;
        if (codepoint >= 0xF900 && codepoint <= 0xFAFF)
            return true;
        if (codepoint >= 0xFE10 && codepoint <= 0xFE19)
            return true;
        if (codepoint >= 0xFE30 && codepoint <= 0xFE6F)
            return true;
        if (codepoint >= 0xFF00 && codepoint <= 0xFF60)
            return true;
        if (codepoint >= 0xFFE0 && codepoint <= 0xFFE6)
            return true;
        if (codepoint >= 0x20000 && codepoint <= 0x2FFFD)
            return true;
        if (codepoint >= 0x30000 && codepoint <= 0x3FFFD)
            return true;
        return false;
    }
};

}  // namespace utils
}  // namespace cppbar