#pragma once

#include "terminal.hpp"
#include "terminal_win.hpp"
#include "terminal_unix.hpp"

#if defined(__linux__) || defined(__APPLE__) || defined(__unix__)
#include <unistd.h>
#endif

namespace cppbar {
namespace terminal {

inline Platform get_platform() {
#if defined(_WIN32)
    return Platform::Windows;
#elif defined(__linux__)
    return Platform::Linux;
#elif defined(__APPLE__)
    return Platform::MacOS;
#else
    return Platform::Unknown;
#endif
}

inline bool is_tty() {
#if defined(_WIN32)
    return _isatty(_fileno(stdout));
#else
    return isatty(STDOUT_FILENO);
#endif
}

inline bool supports_ansi() {
#if defined(_WIN32)
    return win::Win32Terminal::supports_ansi();
#elif defined(__linux__) || defined(__APPLE__) || defined(__unix__)
    return unix_term::UnixTerminal::supports_ansi();
#else
    return false;
#endif
}

inline bool supports_unicode() {
#if defined(_WIN32)
    return win::Win32Terminal::supports_unicode();
#elif defined(__linux__) || defined(__APPLE__) || defined(__unix__)
    return unix_term::UnixTerminal::supports_unicode();
#else
    return false;
#endif
}

inline int get_width() {
#if defined(_WIN32)
    return win::Win32Terminal::get_width();
#elif defined(__linux__) || defined(__APPLE__) || defined(__unix__)
    return unix_term::UnixTerminal::get_width();
#else
    return 80;
#endif
}

inline int get_height() {
#if defined(_WIN32)
    return win::Win32Terminal::get_height();
#elif defined(__linux__) || defined(__APPLE__) || defined(__unix__)
    return unix_term::UnixTerminal::get_height();
#else
    return 24;
#endif
}

inline TerminalInfo get_terminal_info() {
    TerminalInfo info;
    info.width = get_width();
    info.height = get_height();
    info.supports_ansi = supports_ansi();
    info.supports_unicode = supports_unicode();
    info.is_tty = is_tty();
    return info;
}

inline void set_cursor_position(int row, int col) {
#if defined(_WIN32)
    win::Win32Terminal::set_cursor_position(row, col);
#elif defined(__linux__) || defined(__APPLE__) || defined(__unix__)
    unix_term::UnixTerminal::set_cursor_position(row, col);
#endif
}

inline void move_cursor_up(int lines) {
#if defined(_WIN32)
    win::Win32Terminal::move_cursor_up(lines);
#elif defined(__linux__) || defined(__APPLE__) || defined(__unix__)
    unix_term::UnixTerminal::move_cursor_up(lines);
#endif
}

inline void move_cursor_down(int lines) {
#if defined(_WIN32)
    win::Win32Terminal::move_cursor_down(lines);
#elif defined(__linux__) || defined(__APPLE__) || defined(__unix__)
    unix_term::UnixTerminal::move_cursor_down(lines);
#endif
}

inline void clear_line() {
#if defined(_WIN32)
    win::Win32Terminal::clear_line();
#elif defined(__linux__) || defined(__APPLE__) || defined(__unix__)
    unix_term::UnixTerminal::clear_line();
#endif
}

inline void hide_cursor() {
#if defined(_WIN32)
    win::Win32Terminal::hide_cursor();
#elif defined(__linux__) || defined(__APPLE__) || defined(__unix__)
    unix_term::UnixTerminal::hide_cursor();
#endif
}

inline void show_cursor() {
#if defined(_WIN32)
    win::Win32Terminal::show_cursor();
#elif defined(__linux__) || defined(__APPLE__) || defined(__unix__)
    unix_term::UnixTerminal::show_cursor();
#endif
}

inline void enable_alternate_screen() {
#if defined(_WIN32)
    win::Win32Terminal::enable_alternate_screen();
#elif defined(__linux__) || defined(__APPLE__) || defined(__unix__)
    unix_term::UnixTerminal::enable_alternate_screen();
#endif
}

inline void disable_alternate_screen() {
#if defined(_WIN32)
    win::Win32Terminal::disable_alternate_screen();
#elif defined(__linux__) || defined(__APPLE__) || defined(__unix__)
    unix_term::UnixTerminal::disable_alternate_screen();
#endif
}

}  // namespace terminal
}  // namespace cppbar