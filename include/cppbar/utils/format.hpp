#pragma once

#include <chrono>
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

namespace cppbar {
namespace utils {

class Format {
public:
    static std::string format_time(double seconds) {
        if (seconds < 0) {
            return "0s";
        }

        int64_t total_secs = static_cast<int64_t>(seconds);
        int64_t hours = total_secs / 3600;
        int64_t minutes = (total_secs % 3600) / 60;
        int64_t secs = total_secs % 60;

        std::ostringstream oss;

        if (hours > 0) {
            oss << hours << "h ";
            oss << minutes << "m ";
            oss << secs << "s";
        } else if (minutes > 0) {
            oss << minutes << "m ";
            oss << secs << "s";
        } else if (secs > 0 || seconds > 0.001) {
            oss << seconds << "s";
        } else {
            oss << "0s";
        }

        return oss.str();
    }

    static std::string format_speed(double items_per_second) {
        if (std::isinf(items_per_second) || std::isnan(items_per_second)) {
            return "0.00 it/s";
        }

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        oss << items_per_second << " it/s";
        return oss.str();
    }

    static std::string format_bytes(uint64_t bytes) {
        const char* units[] = {"B", "KB", "MB", "GB", "TB"};
        int unit_index = 0;
        double size = static_cast<double>(bytes);

        while (size >= 1024.0 && unit_index < 4) {
            size /= 1024.0;
            unit_index++;
        }

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        oss << size << " " << units[unit_index];
        return oss.str();
    }

    static std::string format_percentage(double percentage) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1);
        oss << percentage << "%";
        return oss.str();
    }

    static std::string pad_left(const std::string& str, size_t width, char fill = ' ') {
        if (str.length() >= width) {
            return str;
        }
        return std::string(width - str.length(), fill) + str;
    }

    static std::string pad_right(const std::string& str, size_t width, char fill = ' ') {
        if (str.length() >= width) {
            return str;
        }
        return str + std::string(width - str.length(), fill);
    }

    static std::string truncate(const std::string& str, size_t max_length,
                                const std::string& ellipsis = "...") {
        if (str.length() <= max_length) {
            return str;
        }

        if (max_length <= ellipsis.length()) {
            return ellipsis.substr(0, max_length);
        }

        return str.substr(0, max_length - ellipsis.length()) + ellipsis;
    }

    static std::string escape_ansi(const std::string& str) {
        std::string result;
        for (char c : str) {
            if (c == '\033') {
                result += "\\033";
            } else if (c == '\n') {
                result += "\\n";
            } else if (c == '\r') {
                result += "\\r";
            } else if (c == '\t') {
                result += "\\t";
            } else if (c < 32 || c > 126) {
                char buf[8];
                snprintf(buf, sizeof(buf), "\\x%02X", static_cast<unsigned char>(c));
                result += buf;
            } else {
                result += c;
            }
        }
        return result;
    }
};

}  // namespace utils
}  // namespace cppbar