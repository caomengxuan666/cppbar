#ifdef _WIN32

#include <io.h>
#include <fcntl.h>

namespace cppbar {
namespace terminal {
namespace win {

namespace {
inline HANDLE get_stdout_handle() {
    static HANDLE g_stdout_handle = INVALID_HANDLE_VALUE;
    if (g_stdout_handle == INVALID_HANDLE_VALUE) {
        g_stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    }
    return g_stdout_handle;
}

inline bool is_vt100_supported() {
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

inline void enable_vt100_mode() {
    HANDLE h = get_stdout_handle();
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
    return get_stdout_handle();::get_stdout_handle();
}

inline bool Win32Terminal::is_vt100_supported() {
    return is_vt100_supported();100_supported();
}

inline void Win32Terminal::enable_vt100_mode() {
    enable_vt100_mode();t100_mode();
}

inline bool Win32Terminal::init() {
    enable_vt100_mode();
    return is_vt100_supported();
}

inline bool Win32Terminal::supports_ansi() {
    return is_vt100_supported();
}

inline bool Win32Terminal::supports_unicode() {
    return true;
}

inline int Win32Terminal::get_width() {
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

inline int Win32Terminal::get_height() {
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

inline void Win32Terminal::set_cursor_position(int row, int col) {
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

inline void Win32Terminal::move_cursor_up(int lines) {
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

inline void Win32Terminal::move_cursor_down(int lines) {
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

inline void Win32Terminal::clear_line() {
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

inline void Win32Terminal::hide_cursor() {
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

inline void Win32Terminal::show_cursor() {
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

inline void Win32Terminal::enable_alternate_screen() {
    if (supports_ansi()) {
        printf("\033[?1049h");
    }
}

inline void Win32Terminal::disable_alternate_screen() {
    if (supports_ansi()) {
        printf("\033[?1049l");
    }
}

inline void Win32Terminal::set_ansi_colors(bool enable) {
    if (enable) {
        enable_vt100_mode();
    }
}

}  // namespace win
}  // namespace terminal
}  // namespace cppbar

#endif  // _WIN32