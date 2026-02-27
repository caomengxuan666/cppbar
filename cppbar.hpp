// clang-format off

/*
 * MIT License
 *
 * Copyright (c) 2026 CppBar Contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// clang-format on

#pragma once

// #include "terminal/terminal.hpp"

#include <cstdint>
#include <string>

// Include platform-specific headers
// #include "terminal_unix.hpp"

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
// #include "terminal_win.hpp"

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

// Include platform-specific implementations (only if not already included)
#ifndef CPPBAR_TERMINAL_IMPL_INCLUDED
#define CPPBAR_TERMINAL_IMPL_INCLUDED
// #include "terminal_unix_impl.hpp"
#if defined(__linux__) || defined(__APPLE__) || defined(__unix__)

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/ioctl.h>
#include <unistd.h>

namespace cppbar {
namespace terminal {
namespace unix_term {

inline std::string UnixTerminal::get_env(const char* name) {
    const char* value = std::getenv(name);
    return value ? value : "";
}

inline bool UnixTerminal::is_color_term() {
    std::string term = get_env("TERM");
    std::string colorterm = get_env("COLORTERM");

    if (colorterm.find("truecolor") != std::string::npos ||
        colorterm.find("24bit") != std::string::npos) {
        return true;
    }

    if (term.find("color") != std::string::npos ||
        term.find("xterm-256color") != std::string::npos || term == "xterm" || term == "screen" ||
        term == "screen-256color" || term == "tmux" || term == "tmux-256color") {
        return true;
    }

    return false;
}

inline bool UnixTerminal::supports_ansi() {
    return is_color_term();
}

inline bool UnixTerminal::supports_unicode() {
    std::string lang = get_env("LANG");
    std::string lc_all = get_env("LC_ALL");
    std::string lc_ctype = get_env("LC_CTYPE");

    std::string locale = lc_all.empty() ? lc_ctype : lc_all;
    if (locale.empty()) {
        locale = lang;
    }

    return locale.find("UTF-8") != std::string::npos || locale.find("utf8") != std::string::npos;
}

inline int UnixTerminal::get_width() {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        return w.ws_col;
    }
    return 80;
}

inline int UnixTerminal::get_height() {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        return w.ws_row;
    }
    return 24;
}

inline void UnixTerminal::set_cursor_position(int row, int col) {
    printf("\033[%d;%dH", row + 1, col + 1);
}

inline void UnixTerminal::move_cursor_up(int lines) {
    printf("\033[%dA", lines);
}

inline void UnixTerminal::move_cursor_down(int lines) {
    printf("\033[%dB", lines);
}

inline void UnixTerminal::clear_line() {
    printf("\033[2K\r");
}

inline void UnixTerminal::hide_cursor() {
    printf("\033[?25l");
}

inline void UnixTerminal::show_cursor() {
    printf("\033[?25h");
}

inline void UnixTerminal::enable_alternate_screen() {
    printf("\033[?1049h");
}

inline void UnixTerminal::disable_alternate_screen() {
    printf("\033[?1049l");
}

}  // namespace unix_term
}  // namespace terminal
}  // namespace cppbar

#endif  // defined(__linux__) || defined(__APPLE__) || defined(__unix__)

// #include "terminal_win_impl.hpp"
#ifdef _WIN32

#include <io.h>
#include <fcntl.h>

namespace cppbar {
namespace terminal {
namespace win {

namespace {
inline HANDLE _internal_get_stdout_handle() {
    static HANDLE g_stdout_handle = INVALID_HANDLE_VALUE;
    if (g_stdout_handle == INVALID_HANDLE_VALUE) {
        g_stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    }
    return g_stdout_handle;
}

inline bool _internal_is_vt100_supported() {
    HANDLE h = _internal_get_stdout_handle();
    if (h == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD mode = 0;
    if (!GetConsoleMode(h, &mode)) {
        return false;
    }

    return (mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
}

inline void _internal_enable_vt100_mode() {
    HANDLE h = _internal_get_stdout_handle();
    if (h == INVALID_HANDLE_VALUE) {
        return;
    }

    DWORD mode = 0;
    if (GetConsoleMode(h, &mode)) {
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        mode |= DISABLE_NEWLINE_AUTO_RETURN;
        SetConsoleMode(h, mode);
    }
}
}  // namespace

inline HANDLE Win32Terminal::get_stdout_handle() {
    return _internal_get_stdout_handle();
}

inline bool Win32Terminal::is_vt100_supported() {
    return _internal_is_vt100_supported();
}

inline void Win32Terminal::enable_vt100_mode() {
    _internal_enable_vt100_mode();
}

inline bool Win32Terminal::init() {
    _internal_enable_vt100_mode();
    return _internal_is_vt100_supported();
}

inline bool Win32Terminal::supports_ansi() {
    return _internal_is_vt100_supported();
}

inline bool Win32Terminal::supports_unicode() {
    return true;
}

inline int Win32Terminal::get_width() {
    HANDLE h = _internal_get_stdout_handle();
    if (h == INVALID_HANDLE_VALUE) {
        return 80;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(h, &csbi)) {
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }

    return 80;
}

inline int Win32Terminal::get_height() {
    HANDLE h = _internal_get_stdout_handle();
    if (h == INVALID_HANDLE_VALUE) {
        return 24;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(h, &csbi)) {
        return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }

    return 24;
}

inline void Win32Terminal::set_cursor_position(int row, int col) {
    HANDLE h = _internal_get_stdout_handle();
    if (h != INVALID_HANDLE_VALUE) {
        COORD pos = {static_cast<SHORT>(col), static_cast<SHORT>(row)};
        SetConsoleCursorPosition(h, pos);
    }
}

inline void Win32Terminal::move_cursor_up(int lines) {
    HANDLE h = _internal_get_stdout_handle();
    if (h != INVALID_HANDLE_VALUE) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(h, &csbi)) {
            COORD pos = csbi.dwCursorPosition;
            pos.Y = (pos.Y >= lines) ? (pos.Y - lines) : 0;
            SetConsoleCursorPosition(h, pos);
        }
    }
}

inline void Win32Terminal::move_cursor_down(int lines) {
    HANDLE h = _internal_get_stdout_handle();
    if (h != INVALID_HANDLE_VALUE) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(h, &csbi)) {
            COORD pos = csbi.dwCursorPosition;
            pos.Y += lines;
            SetConsoleCursorPosition(h, pos);
        }
    }
}

inline void Win32Terminal::clear_line() {
    HANDLE h = _internal_get_stdout_handle();
    if (h != INVALID_HANDLE_VALUE) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(h, &csbi)) {
            DWORD written;
            COORD start = {0, csbi.dwCursorPosition.Y};
            FillConsoleOutputCharacterA(h, ' ', csbi.dwSize.X, start, &written);
            FillConsoleOutputAttribute(h, csbi.wAttributes, csbi.dwSize.X, start, &written);
            SetConsoleCursorPosition(h, start);
        }
    }
}

inline void Win32Terminal::hide_cursor() {
    HANDLE h = _internal_get_stdout_handle();
    if (h != INVALID_HANDLE_VALUE) {
        CONSOLE_CURSOR_INFO cci;
        if (GetConsoleCursorInfo(h, &cci)) {
            cci.bVisible = FALSE;
            SetConsoleCursorInfo(h, &cci);
        }
    }
}

inline void Win32Terminal::show_cursor() {
    HANDLE h = _internal_get_stdout_handle();
    if (h != INVALID_HANDLE_VALUE) {
        CONSOLE_CURSOR_INFO cci;
        if (GetConsoleCursorInfo(h, &cci)) {
            cci.bVisible = TRUE;
            SetConsoleCursorInfo(h, &cci);
        }
    }
}

inline void Win32Terminal::enable_alternate_screen() {
    // Not supported on Windows
}

inline void Win32Terminal::disable_alternate_screen() {
    // Not supported on Windows
}

inline void Win32Terminal::set_ansi_colors(bool /*enable*/) {
    // No-op - we use Windows API instead
}

}  // namespace win
}  // namespace terminal
}  // namespace cppbar

#endif  // _WIN32
#endif
// #include "terminal/terminal_impl.hpp"

// #include "terminal.hpp"

#include <cstdint>
#include <string>

// Include platform-specific headers
// #include "terminal_unix.hpp"

// #include "terminal_win.hpp"

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

// Include platform-specific implementations (only if not already included)
#ifndef CPPBAR_TERMINAL_IMPL_INCLUDED
#define CPPBAR_TERMINAL_IMPL_INCLUDED
// #include "terminal_unix_impl.hpp"

// #include "terminal_win_impl.hpp"

#endif
// #include "terminal_win.hpp"

// #include "terminal_unix.hpp"

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
// #include "style/style.hpp"

#include <string>

namespace cppbar {
namespace style {

struct StyleConfig {
    std::string left_bracket;
    std::string right_bracket;
    std::string filled_char;
    std::string empty_char;
    std::string tip_char;

    StyleConfig(const std::string& lb, const std::string& rb, const std::string& fc,
                const std::string& ec, const std::string& tc)
        : left_bracket(lb), right_bracket(rb), filled_char(fc), empty_char(ec), tip_char(tc) {}
};

class Style {
public:
    enum class Type { Unicode, Ascii, Braille };

    Style() : type_(Type::Unicode) {}

    explicit Style(Type type) : type_(type) {}

    StyleConfig get_config() const;

    void set_type(Type type) { type_ = type; }
    Type get_type() const { return type_; }

    static Style unicode() { return Style(Type::Unicode); }
    static Style ascii() { return Style(Type::Ascii); }
    static Style braille() { return Style(Type::Braille); }

private:
    Type type_;
};

struct PresetStyles {
    static StyleConfig classic();
    static StyleConfig modern();
    static StyleConfig minimal();
    static StyleConfig blocks();
    static StyleConfig dots();
    static StyleConfig squares();
};

}  // namespace style
}  // namespace cppbar
// #include "style/style_impl.hpp"

// #include "style.hpp"

#include <string>

namespace cppbar {
namespace style {

struct StyleConfig {
    std::string left_bracket;
    std::string right_bracket;
    std::string filled_char;
    std::string empty_char;
    std::string tip_char;

    StyleConfig(const std::string& lb, const std::string& rb, const std::string& fc,
                const std::string& ec, const std::string& tc)
        : left_bracket(lb), right_bracket(rb), filled_char(fc), empty_char(ec), tip_char(tc) {}
};

class Style {
public:
    enum class Type { Unicode, Ascii, Braille };

    Style() : type_(Type::Unicode) {}

    explicit Style(Type type) : type_(type) {}

    StyleConfig get_config() const;

    void set_type(Type type) { type_ = type; }
    Type get_type() const { return type_; }

    static Style unicode() { return Style(Type::Unicode); }
    static Style ascii() { return Style(Type::Ascii); }
    static Style braille() { return Style(Type::Braille); }

private:
    Type type_;
};

struct PresetStyles {
    static StyleConfig classic();
    static StyleConfig modern();
    static StyleConfig minimal();
    static StyleConfig blocks();
    static StyleConfig dots();
    static StyleConfig squares();
};

}  // namespace style
}  // namespace cppbar

namespace cppbar {
namespace style {

inline StyleConfig Style::get_config() const {
    switch (type_) {
    case Type::Ascii:
        return StyleConfig("[", "]", "=", " ", ">");
    case Type::Braille:
        return StyleConfig("│", "│", "⣿", "⣀", "⣶");
    case Type::Unicode:
    default:
        return StyleConfig("[", "]", "█", "░", ">");
    }
}

inline StyleConfig PresetStyles::classic() {
    return StyleConfig("[", "]", "=", " ", ">");
}

inline StyleConfig PresetStyles::modern() {
    return StyleConfig("[", "]", "█", "░", ">");
}

inline StyleConfig PresetStyles::minimal() {
    return StyleConfig("", "", "=", " ", ">");
}

inline StyleConfig PresetStyles::blocks() {
    return StyleConfig("|", "|", "▓", "░", "▒");
}

inline StyleConfig PresetStyles::dots() {
    return StyleConfig("◢", "◣", "●", "○", "◑");
}

inline StyleConfig PresetStyles::squares() {
    return StyleConfig("[", "]", "■", "□", "▣");
}

}  // namespace style
}  // namespace cppbar
// #include "color/color.hpp"
#undef RGB

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
// #include "color/color_impl.hpp"

// #include "color.hpp"
#undef RGB

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
// #include "animation/animation.hpp"

#include <string>
#include <vector>
#include <cmath>

namespace cppbar {
namespace animation {

class Animation {
public:
    Animation() : frame_count_(0), current_frame_(0), last_update_(0) {}

    virtual ~Animation() = default;

    virtual std::string get_frame() const = 0;
    virtual void next_frame() = 0;
    virtual void reset() = 0;
    virtual Animation* clone() const = 0;

    size_t get_frame_count() const { return frame_count_; }
    size_t get_current_frame() const { return current_frame_; }

protected:
    size_t frame_count_;
    size_t current_frame_;
    size_t last_update_;
};

class PulseAnimation : public Animation {
public:
    PulseAnimation();

    std::string get_frame() const override;
    void next_frame() override;
    void reset() override;
    Animation* clone() const override;

private:
    std::vector<std::string> frames_;
};

class WaveAnimation : public Animation {
public:
    WaveAnimation();

    std::string get_frame() const override;
    void next_frame() override;
    void reset() override;
    Animation* clone() const override;

private:
    std::vector<std::string> frames_;
};

class BounceAnimation : public Animation {
public:
    BounceAnimation();

    std::string get_frame() const override;
    void next_frame() override;
    void reset() override;
    Animation* clone() const override;

private:
    std::vector<std::string> frames_;
    bool moving_right_;
};

class GradientAnimation : public Animation {
public:
    GradientAnimation();

    std::string get_frame() const override;
    void next_frame() override;
    void reset() override;
    Animation* clone() const override;

private:
    std::vector<std::string> frames_;
};

class CycleAnimation : public Animation {
public:
    explicit CycleAnimation(const std::vector<std::string>& chars);

    std::string get_frame() const override;
    void next_frame() override;
    void reset() override;
    Animation* clone() const override;

private:
    std::vector<std::string> chars_;
};

class NoneAnimation : public Animation {
public:
    std::string get_frame() const override;
    void next_frame() override;
    void reset() override;
    Animation* clone() const override;
};

namespace presets {

Animation* pulse();
Animation* wave();
Animation* bounce();
Animation* gradient();
Animation* cycle(const std::vector<std::string>& chars);
Animation* none();

}  // namespace presets

}  // namespace animation
}  // namespace cppbar
// #include "animation/animation_impl.hpp"

// #include "animation.hpp"

#include <string>
#include <vector>
#include <cmath>

namespace cppbar {
namespace animation {

class Animation {
public:
    Animation() : frame_count_(0), current_frame_(0), last_update_(0) {}

    virtual ~Animation() = default;

    virtual std::string get_frame() const = 0;
    virtual void next_frame() = 0;
    virtual void reset() = 0;
    virtual Animation* clone() const = 0;

    size_t get_frame_count() const { return frame_count_; }
    size_t get_current_frame() const { return current_frame_; }

protected:
    size_t frame_count_;
    size_t current_frame_;
    size_t last_update_;
};

class PulseAnimation : public Animation {
public:
    PulseAnimation();

    std::string get_frame() const override;
    void next_frame() override;
    void reset() override;
    Animation* clone() const override;

private:
    std::vector<std::string> frames_;
};

class WaveAnimation : public Animation {
public:
    WaveAnimation();

    std::string get_frame() const override;
    void next_frame() override;
    void reset() override;
    Animation* clone() const override;

private:
    std::vector<std::string> frames_;
};

class BounceAnimation : public Animation {
public:
    BounceAnimation();

    std::string get_frame() const override;
    void next_frame() override;
    void reset() override;
    Animation* clone() const override;

private:
    std::vector<std::string> frames_;
    bool moving_right_;
};

class GradientAnimation : public Animation {
public:
    GradientAnimation();

    std::string get_frame() const override;
    void next_frame() override;
    void reset() override;
    Animation* clone() const override;

private:
    std::vector<std::string> frames_;
};

class CycleAnimation : public Animation {
public:
    explicit CycleAnimation(const std::vector<std::string>& chars);

    std::string get_frame() const override;
    void next_frame() override;
    void reset() override;
    Animation* clone() const override;

private:
    std::vector<std::string> chars_;
};

class NoneAnimation : public Animation {
public:
    std::string get_frame() const override;
    void next_frame() override;
    void reset() override;
    Animation* clone() const override;
};

namespace presets {

Animation* pulse();
Animation* wave();
Animation* bounce();
Animation* gradient();
Animation* cycle(const std::vector<std::string>& chars);
Animation* none();

}  // namespace presets

}  // namespace animation
}  // namespace cppbar
#include <algorithm>

namespace cppbar {
namespace animation {

inline PulseAnimation::PulseAnimation() {
    frames_ = {"░", "▒", "▓", "█", "▓", "▒", "░", " "};
    frame_count_ = frames_.size();
    current_frame_ = 0;
}

inline std::string PulseAnimation::get_frame() const {
    return frames_[current_frame_];
}

inline void PulseAnimation::next_frame() {
    current_frame_ = (current_frame_ + 1) % frame_count_;
}

inline void PulseAnimation::reset() {
    current_frame_ = 0;
}

inline Animation* PulseAnimation::clone() const {
    return new PulseAnimation(*this);
}

inline WaveAnimation::WaveAnimation() {
    frames_ = {"≋", "≈", "≈", "≋", "~", "≋", "≈", "≈"};
    frame_count_ = frames_.size();
    current_frame_ = 0;
}

inline std::string WaveAnimation::get_frame() const {
    return frames_[current_frame_];
}

inline void WaveAnimation::next_frame() {
    current_frame_ = (current_frame_ + 1) % frame_count_;
}

inline void WaveAnimation::reset() {
    current_frame_ = 0;
}

inline Animation* WaveAnimation::clone() const {
    return new WaveAnimation(*this);
}

inline BounceAnimation::BounceAnimation() : moving_right_(true) {
    frames_ = {".", "o", "O", "0", "O", "o", "."};
    frame_count_ = frames_.size();
    current_frame_ = 0;
}

inline std::string BounceAnimation::get_frame() const {
    return frames_[current_frame_];
}

inline void BounceAnimation::next_frame() {
    if (moving_right_) {
        if (current_frame_ < frame_count_ - 1) {
            current_frame_++;
        } else {
            moving_right_ = false;
            current_frame_--;
        }
    } else {
        if (current_frame_ > 0) {
            current_frame_--;
        } else {
            moving_right_ = true;
            current_frame_++;
        }
    }
}

inline void BounceAnimation::reset() {
    current_frame_ = 0;
    moving_right_ = true;
}

inline Animation* BounceAnimation::clone() const {
    return new BounceAnimation(*this);
}

inline GradientAnimation::GradientAnimation() {
    frames_ = {"░", "▒", "▓", "█"};
    frame_count_ = frames_.size();
    current_frame_ = 0;
}

inline std::string GradientAnimation::get_frame() const {
    return frames_[current_frame_];
}

inline void GradientAnimation::next_frame() {
    current_frame_ = (current_frame_ + 1) % frame_count_;
}

inline void GradientAnimation::reset() {
    current_frame_ = 0;
}

inline Animation* GradientAnimation::clone() const {
    return new GradientAnimation(*this);
}

inline CycleAnimation::CycleAnimation(const std::vector<std::string>& chars) : chars_(chars) {
    frame_count_ = chars_.size();
    current_frame_ = 0;
}

inline std::string CycleAnimation::get_frame() const {
    if (chars_.empty()) {
        return "";
    }
    return chars_[current_frame_];
}

inline void CycleAnimation::next_frame() {
    if (!chars_.empty()) {
        current_frame_ = (current_frame_ + 1) % chars_.size();
    }
}

inline void CycleAnimation::reset() {
    current_frame_ = 0;
}

inline Animation* CycleAnimation::clone() const {
    return new CycleAnimation(*this);
}

inline std::string NoneAnimation::get_frame() const {
    return "";
}

inline void NoneAnimation::next_frame() {}

inline void NoneAnimation::reset() {}

inline Animation* NoneAnimation::clone() const {
    return new NoneAnimation(*this);
}

namespace presets {

inline Animation* pulse() {
    return new PulseAnimation();
}

inline Animation* wave() {
    return new WaveAnimation();
}

inline Animation* bounce() {
    return new BounceAnimation();
}

inline Animation* gradient() {
    return new GradientAnimation();
}

inline Animation* cycle(const std::vector<std::string>& chars) {
    return new CycleAnimation(chars);
}

inline Animation* none() {
    return new NoneAnimation();
}

}  // namespace presets

}  // namespace animation
}  // namespace cppbar
// #include "utils/clock.hpp"

#include <chrono>

namespace cppbar {
namespace utils {

class Clock {
public:
    using Duration = std::chrono::steady_clock::duration;
    using TimePoint = std::chrono::steady_clock::time_point;

    Clock() : start_time_(TimePoint::min()) {}

    void start() { start_time_ = std::chrono::steady_clock::now(); }

    void reset() { start_time_ = std::chrono::steady_clock::now(); }

    bool is_running() const { return start_time_ != TimePoint::min(); }

    Duration elapsed() const {
        if (start_time_ == TimePoint::min()) {
            return Duration(0);
        }
        return std::chrono::steady_clock::now() - start_time_;
    }

    double elapsed_seconds() const {
        auto dur = elapsed();
        return std::chrono::duration<double>(dur).count();
    }

    template <typename T = std::chrono::milliseconds>
    typename T::rep elapsed_count() const {
        return std::chrono::duration_cast<T>(elapsed()).count();
    }

private:
    TimePoint start_time_;
};

class Timer {
public:
    Timer() : interval_(1000), last_tick_(Clock::TimePoint{}) {}

    explicit Timer(int interval_ms) : interval_(interval_ms), last_tick_(Clock::TimePoint{}) {}

    bool tick() {
        auto now = std::chrono::steady_clock::now();
        auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick_).count();

        if (elapsed >= interval_) {
            last_tick_ = now;
            return true;
        }
        return false;
    }

    void reset() { last_tick_ = std::chrono::steady_clock::now(); }

    void set_interval(int interval_ms) { interval_ = interval_ms; }

private:
    int interval_;
    std::chrono::steady_clock::time_point last_tick_;
};

}  // namespace utils
}  // namespace cppbar
// #include "utils/format.hpp"

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
// #include "utils/math.hpp"

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

#include <cstdint>
#include <string>
#include <iostream>
#include <memory>
#include <sstream>

namespace cppbar {

class ProgressBar {
public:
    ProgressBar(int64_t total);
    ~ProgressBar();

    void set_title(const std::string& title);
    void set_width(int width);
    void set_color(const color::Color& fg, const color::Color& bg = color::Color());
    void set_style(const style::Style& style);
    void set_animation(animation::Animation* anim);

    void show_percentage(bool show);
    void show_elapsed(bool show);
    void show_eta(bool show);
    void show_speed(bool show);

    void start();
    void tick(int64_t amount = 1);
    void update(int64_t current);
    void finish(const std::string& message = "");

    std::string render() const;

    static ProgressBar* make_bar(int64_t total);

    void set_multi_progress_mode(bool enabled) { multi_progress_mode_ = enabled; }
    bool is_multi_progress_mode() const { return multi_progress_mode_; }

private:
    void init();
    void ensure_started();
    void refresh_display();
    std::string render_bar() const;
    std::string render_info() const;

    int64_t total_;
    int64_t current_;
    std::string title_;
    int width_;
    color::Color fg_color_;
    color::Color bg_color_;
    style::Style style_;
    std::unique_ptr<animation::Animation> animation_;
    bool animation_owned_;

    bool show_percentage_;
    bool show_elapsed_;
    bool show_eta_;
    bool show_speed_;

    bool started_;
    bool finished_;
    bool hidden_;
    bool multi_progress_mode_;

    utils::Clock clock_;
    int64_t last_update_;

    mutable std::string last_render_;
};

inline ProgressBar::ProgressBar(int64_t total)
    : total_(total), current_(0), title_(""), width_(40), fg_color_(), bg_color_(),
      style_(style::Style::Type::Unicode), animation_(nullptr), animation_owned_(false),
      show_percentage_(true), show_elapsed_(false), show_eta_(false), show_speed_(false),
      started_(false), finished_(false), hidden_(false), multi_progress_mode_(false),
      last_update_(0) {}

inline ProgressBar::~ProgressBar() {
    if (started_ && !finished_) {
        finish();
    }
}

inline void ProgressBar::set_title(const std::string& title) {
    title_ = title;
}

inline void ProgressBar::set_width(int width) {
    width_ = width;
}

inline void ProgressBar::set_color(const color::Color& fg, const color::Color& bg) {
    fg_color_ = fg;
    bg_color_ = bg;
}

inline void ProgressBar::set_style(const style::Style& style) {
    style_ = style;
}

inline void ProgressBar::set_animation(animation::Animation* anim) {
    if (animation_ && animation_owned_) {
        animation_.reset();
    }
    animation_ = std::unique_ptr<animation::Animation>(anim);
    animation_owned_ = false;
}

inline void ProgressBar::show_percentage(bool show) {
    show_percentage_ = show;
}

inline void ProgressBar::show_elapsed(bool show) {
    show_elapsed_ = show;
}

inline void ProgressBar::show_eta(bool show) {
    show_eta_ = show;
}

inline void ProgressBar::show_speed(bool show) {
    show_speed_ = show;
}

inline void ProgressBar::init() {
    if (terminal::is_tty()) {
        terminal::hide_cursor();
    }

    if (animation_ == nullptr) {
        animation_ = std::unique_ptr<animation::Animation>(animation::presets::none());
        animation_owned_ = true;
    }
}

inline void ProgressBar::ensure_started() {
    if (!started_) {
        start();
    }
}

inline void ProgressBar::start() {
    if (started_) {
        return;
    }

    init();
    clock_.start();
    started_ = true;
    current_ = 0;
    last_update_ = 0;

    refresh_display();
}

inline void ProgressBar::tick(int64_t amount) {
    ensure_started();

    if (finished_) {
        return;
    }

    current_ += amount;
    if (current_ > total_) {
        current_ = total_;
    }

    refresh_display();

    if (current_ >= total_) {
        finish();
    }
}

inline void ProgressBar::update(int64_t current) {
    ensure_started();

    if (finished_) {
        return;
    }

    current_ = current;
    if (current_ > total_) {
        current_ = total_;
    }

    refresh_display();

    if (current_ >= total_) {
        finish();
    }
}

inline void ProgressBar::finish(const std::string& message) {
    if (finished_) {
        return;
    }

    ensure_started();
    current_ = total_;
    finished_ = true;

    std::string output = render();
    if (!message.empty()) {
        output += " " + message;
    }

    std::cout << "\r" << output << std::endl;

    if (terminal::is_tty()) {
        terminal::show_cursor();
    }
}

inline void ProgressBar::refresh_display() {
    if (!terminal::is_tty()) {
        return;
    }

    // In multi-progress mode, don't print here
    // Let MultiProgress handle all printing
    if (multi_progress_mode_) {
        if (animation_) {
            animation_->next_frame();
        }
        return;
    }

    std::string output = render();
    if (output != last_render_) {
        std::cout << "\r" << output << std::flush;
        last_render_ = output;
    }

    if (animation_) {
        animation_->next_frame();
    }
}

inline std::string ProgressBar::render() const {
    std::ostringstream oss;

    if (!title_.empty()) {
        oss << title_ << ": ";
    }

    oss << render_bar();

    if (show_percentage_ || show_elapsed_ || show_eta_ || show_speed_) {
        oss << " " << render_info();
    }

    return oss.str();
}

inline std::string ProgressBar::render_bar() const {
    double percentage = 0.0;
    if (total_ > 0) {
        percentage = static_cast<double>(current_) / static_cast<double>(total_);
    }

    auto config = style_.get_config();

    int available_width = width_ - config.left_bracket.length() - config.right_bracket.length() - 1;

    int filled_width = utils::Math::calculate_filled_width(available_width, percentage * 100.0);

    std::ostringstream oss;
    oss << config.left_bracket;

    for (int i = 0; i < available_width; ++i) {
        if (i < filled_width) {
            oss << config.filled_char;
        } else if (i == filled_width && filled_width < available_width && filled_width > 0) {
            oss << config.tip_char;
        } else {
            oss << config.empty_char;
        }
    }

    oss << config.right_bracket;

    std::string result = oss.str();

    if (!fg_color_.is_none() || !bg_color_.is_none()) {
        result = color::apply_color(result, fg_color_, bg_color_);
    }

    return result;
}

inline std::string ProgressBar::render_info() const {
    std::ostringstream oss;
    bool first = true;

    if (show_percentage_) {
        double percentage = 0.0;
        if (total_ > 0) {
            percentage = static_cast<double>(current_) / static_cast<double>(total_) * 100.0;
        }
        oss << utils::Format::format_percentage(percentage);
        first = false;
    }

    if (show_elapsed_) {
        if (!first)
            oss << " ";
        double elapsed = clock_.elapsed_seconds();
        oss << utils::Format::format_time(elapsed);
        first = false;
    }

    if (show_eta_) {
        if (!first)
            oss << " ";
        double elapsed = clock_.elapsed_seconds();
        int eta = utils::Math::calculate_eta(current_, total_, elapsed);
        oss << "ETA: " << utils::Format::format_time(eta);
        first = false;
    }

    if (show_speed_) {
        if (!first)
            oss << " ";
        double elapsed = clock_.elapsed_seconds();
        double speed = utils::Math::calculate_speed(current_, elapsed);
        oss << utils::Format::format_speed(speed);
    }

    return oss.str();
}

inline ProgressBar* ProgressBar::make_bar(int64_t total) {
    return new ProgressBar(total);
}

}  // namespace cppbar
