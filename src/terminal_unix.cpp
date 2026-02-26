#if defined(__linux__) || defined(__APPLE__) || defined(__unix__)

#include "../include/cppbar/terminal/terminal_unix.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/ioctl.h>
#include <unistd.h>

namespace cppbar {
namespace terminal {
namespace unix_term {

std::string UnixTerminal::get_env(const char* name) {
    const char* value = std::getenv(name);
    return value ? value : "";
}

bool UnixTerminal::is_color_term() {
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

bool UnixTerminal::supports_ansi() {
    return is_color_term();
}

bool UnixTerminal::supports_unicode() {
    std::string lang = get_env("LANG");
    std::string lc_all = get_env("LC_ALL");
    std::string lc_ctype = get_env("LC_CTYPE");

    std::string locale = lc_all.empty() ? lc_ctype : lc_all;
    if (locale.empty()) {
        locale = lang;
    }

    return locale.find("UTF-8") != std::string::npos || locale.find("utf8") != std::string::npos;
}

int UnixTerminal::get_width() {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        return w.ws_col;
    }
    return 80;
}

int UnixTerminal::get_height() {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        return w.ws_row;
    }
    return 24;
}

void UnixTerminal::set_cursor_position(int row, int col) {
    printf("\033[%d;%dH", row + 1, col + 1);
}

void UnixTerminal::move_cursor_up(int lines) {
    printf("\033[%dA", lines);
}

void UnixTerminal::move_cursor_down(int lines) {
    printf("\033[%dB", lines);
}

void UnixTerminal::clear_line() {
    printf("\033[2K\r");
}

void UnixTerminal::hide_cursor() {
    printf("\033[?25l");
}

void UnixTerminal::show_cursor() {
    printf("\033[?25h");
}

void UnixTerminal::enable_alternate_screen() {
    printf("\033[?1049h");
}

void UnixTerminal::disable_alternate_screen() {
    printf("\033[?1049l");
}

}  // namespace unix_term
}  // namespace terminal
}  // namespace cppbar

#endif  // defined(__linux__) || defined(__APPLE__) || defined(__unix__)