#pragma once

#include <cstdint>
#include <string>

namespace cppbar {
namespace terminal {

enum class Platform { Windows, Linux, MacOS, Unknown };

struct TerminalInfo {
    int width;
    int height;
    bool supports_ansi;
    bool supports_unicode;
    bool is_tty;
};

Platform get_platform();

bool is_tty();

bool supports_ansi();

bool supports_unicode();

int get_width();

int get_height();

TerminalInfo get_terminal_info();

void set_cursor_position(int row, int col);

void move_cursor_up(int lines);

void move_cursor_down(int lines);

void clear_line();

void hide_cursor();

void show_cursor();

void enable_alternate_screen();

void disable_alternate_screen();

}  // namespace terminal
}  // namespace cppbar