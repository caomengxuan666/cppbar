#pragma once

#ifdef _WIN32

#include <cstdint>
#include <string>
#include <windows.h>

namespace cppbar {
namespace terminal {
namespace win {

class Win32Terminal {
public:
    static bool init();

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

    static void set_ansi_colors(bool enable);

private:
    static HANDLE get_stdout_handle();
    static bool is_vt100_supported();
    static void enable_vt100_mode();
};

}  // namespace win
}  // namespace terminal
}  // namespace cppbar

#endif  // _WIN32