#pragma once

#if defined(__linux__) || defined(__APPLE__) || defined(__unix__)

#include <cstdint>
#include <string>

namespace cppbar {
namespace terminal {
namespace unix_term {

class UnixTerminal {
public:
    static bool supports_ansi();

    static bool supports_unicode();

    static int get_width();

    static int get_height();

    static void set_cursor_position(int row, int col);

    static void move_cursor_up(int lines);

    static void move_cursor_down(int lines);

    static void clear_line();

    static void hide_cursor();

    static void show_cursor();

    static void enable_alternate_screen();

    static void disable_alternate_screen();

private:
    static std::string get_env(const char* name);
    static bool is_color_term();
};

}  // namespace unix_term
}  // namespace terminal
}  // namespace cppbar

#endif  // defined(__linux__) || defined(__APPLE__) || defined(__unix__)