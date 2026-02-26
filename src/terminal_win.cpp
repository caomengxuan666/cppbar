#ifdef _WIN32

#include "../include/cppbar/terminal/terminal_win.hpp"
#include <io.h>
#include <fcntl.h>

namespace cppbar {
namespace terminal {
namespace win {

namespace {
HANDLE g_stdout_handle = INVALID_HANDLE_VALUE;
bool g_ansi_enabled = false;
}  // namespace

HANDLE Win32Terminal::get_stdout_handle() {
    if (g_stdout_handle == INVALID_HANDLE_VALUE) {
        g_stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    }
    return g_stdout_handle;
}

bool Win32Terminal::is_vt100_supported() {
    HANDLE h = get_stdout_handle();
    if (h == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD mode = 0;
    if (!GetConsoleMode(h, &mode)) {
        return false;
    }

    return (mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
}

void Win32Terminal::enable_vt100_mode() {
    HANDLE h = get_stdout_handle();
    if (h == INVALID_HANDLE_VALUE) {
        return;
    }

    DWORD mode = 0;
    if (GetConsoleMode(h, &mode)) {
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        mode |= DISABLE_NEWLINE_AUTO_RETURN;
        SetConsoleMode(h, mode);
        g_ansi_enabled = true;
    }
}

bool Win32Terminal::init() {
    enable_vt100_mode();
    return g_ansi_enabled;
}

bool Win32Terminal::supports_ansi() {
    return g_ansi_enabled || is_vt100_supported();
}

bool Win32Terminal::supports_unicode() {
    return true;
}

int Win32Terminal::get_width() {
    HANDLE h = get_stdout_handle();
    if (h == INVALID_HANDLE_VALUE) {
        return 80;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(h, &csbi)) {
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }

    return 80;
}

int Win32Terminal::get_height() {
    HANDLE h = get_stdout_handle();
    if (h == INVALID_HANDLE_VALUE) {
        return 24;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(h, &csbi)) {
        return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }

    return 24;
}

void Win32Terminal::set_cursor_position(int row, int col) {
    if (supports_ansi()) {
        printf("\033[%d;%dH", row + 1, col + 1);
    } else {
        HANDLE h = get_stdout_handle();
        if (h != INVALID_HANDLE_VALUE) {
            COORD pos = {static_cast<SHORT>(col), static_cast<SHORT>(row)};
            SetConsoleCursorPosition(h, pos);
        }
    }
}

void Win32Terminal::move_cursor_up(int lines) {
    if (supports_ansi()) {
        printf("\033[%dA", lines);
    } else {
        HANDLE h = get_stdout_handle();
        if (h != INVALID_HANDLE_VALUE) {
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            if (GetConsoleScreenBufferInfo(h, &csbi)) {
                COORD pos = csbi.dwCursorPosition;
                pos.Y = (pos.Y >= lines) ? (pos.Y - lines) : 0;
                SetConsoleCursorPosition(h, pos);
            }
        }
    }
}

void Win32Terminal::move_cursor_down(int lines) {
    if (supports_ansi()) {
        printf("\033[%dB", lines);
    } else {
        HANDLE h = get_stdout_handle();
        if (h != INVALID_HANDLE_VALUE) {
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            if (GetConsoleScreenBufferInfo(h, &csbi)) {
                COORD pos = csbi.dwCursorPosition;
                pos.Y += lines;
                SetConsoleCursorPosition(h, pos);
            }
        }
    }
}

void Win32Terminal::clear_line() {
    if (supports_ansi()) {
        printf("\033[2K\r");
    } else {
        HANDLE h = get_stdout_handle();
        if (h != INVALID_HANDLE_VALUE) {
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            if (GetConsoleScreenBufferInfo(h, &csbi)) {
                DWORD written;
                COORD start = {0, csbi.dwCursorPosition.Y};
                FillConsoleOutputCharacter(h, ' ', csbi.dwSize.X, start, &written);
                SetConsoleCursorPosition(h, start);
            }
        }
    }
}

void Win32Terminal::hide_cursor() {
    if (supports_ansi()) {
        printf("\033[?25l");
    } else {
        HANDLE h = get_stdout_handle();
        if (h != INVALID_HANDLE_VALUE) {
            CONSOLE_CURSOR_INFO cci;
            if (GetConsoleCursorInfo(h, &cci)) {
                cci.bVisible = FALSE;
                SetConsoleCursorInfo(h, &cci);
            }
        }
    }
}

void Win32Terminal::show_cursor() {
    if (supports_ansi()) {
        printf("\033[?25h");
    } else {
        HANDLE h = get_stdout_handle();
        if (h != INVALID_HANDLE_VALUE) {
            CONSOLE_CURSOR_INFO cci;
            if (GetConsoleCursorInfo(h, &cci)) {
                cci.bVisible = TRUE;
                SetConsoleCursorInfo(h, &cci);
            }
        }
    }
}

void Win32Terminal::enable_alternate_screen() {
    if (supports_ansi()) {
        printf("\033[?1049h");
    }
}

void Win32Terminal::disable_alternate_screen() {
    if (supports_ansi()) {
        printf("\033[?1049l");
    }
}

void Win32Terminal::set_ansi_colors(bool enable) {
    if (enable) {
        enable_vt100_mode();
    }
}

}  // namespace win
}  // namespace terminal
}  // namespace cppbar

#endif  // _WIN32