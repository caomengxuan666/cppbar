#pragma once

#include "progress_bar.hpp"
#include "terminal/terminal.hpp"
#include "terminal/terminal_impl.hpp"

#include <vector>
#include <memory>
#include <iostream>
#include <mutex>
#include <atomic>

namespace cppbar {

class MultiProgress {
public:
    MultiProgress();
    ~MultiProgress();

    ProgressBar* add_bar(int64_t total, const std::string& title = "");

    void start();
    void finish();

    void refresh();

    void tick(size_t index, int64_t amount = 1);

    ProgressBar& operator[](size_t index);

private:
    void move_up(int lines);
    void move_down(int lines);
    void erase_line();

    std::vector<std::unique_ptr<ProgressBar>> bars_;
    std::atomic<bool> started_;
    std::mutex refresh_mutex_;
    std::atomic<bool> first_refresh_;
};

inline MultiProgress::MultiProgress() : started_(false), first_refresh_(true) {}

inline MultiProgress::~MultiProgress() {
    if (started_) {
        finish();
    }
}

inline ProgressBar* MultiProgress::add_bar(int64_t total, const std::string& title) {
    auto bar = std::make_unique<ProgressBar>(total);
    if (!title.empty()) {
        bar->set_title(title);
    }
    bar->show_percentage(true);
    bar->show_eta(true);
    bar->show_speed(true);

    // Enable multi-progress mode to prevent individual bar printing
    bar->set_multi_progress_mode(true);

    ProgressBar* ptr = bar.get();
    bars_.push_back(std::move(bar));
    return ptr;
}

inline void MultiProgress::start() {
    if (started_) {
        return;
    }

    if (terminal::is_tty()) {
        terminal::hide_cursor();
    }

    started_ = true;

    for (auto& bar : bars_) {
        bar->start();
    }

    refresh();
}

inline void MultiProgress::finish() {
    if (!started_) {
        return;
    }

    started_ = false;

    if (terminal::is_tty()) {
        terminal::show_cursor();
        std::cout << std::endl;
    }
}

inline ProgressBar& MultiProgress::operator[](size_t index) {
    if (index >= bars_.size()) {
        throw std::out_of_range("MultiProgress index out of range");
    }
    std::lock_guard<std::mutex> lock(refresh_mutex_);
    return *bars_[index];
}

inline void MultiProgress::refresh() {
    if (!started_) {
        return;
    }

    if (!terminal::is_tty()) {
        for (auto& bar : bars_) {
            std::cout << bar->render() << std::endl;
        }
        return;
    }

    std::lock_guard<std::mutex> lock(refresh_mutex_);

    if (first_refresh_.load()) {
        // First refresh: print all lines
        for (size_t i = 0; i < bars_.size(); ++i) {
            std::cout << bars_[i]->render();
            if (i < bars_.size() - 1) {
                std::cout << "\n";
            }
        }
        std::cout << std::flush;
        first_refresh_.store(false);
        return;
    }

    // Subsequent refreshes: move up to first line, then erase and reprint
    int num_bars = static_cast<int>(bars_.size());
    
    // First, move up to the first bar
    if (num_bars > 1) {
        move_up(num_bars - 1);
    }
    
    // Then clear current line and all lines below
    for (int i = 0; i < num_bars; ++i) {
        erase_line();
        if (i < num_bars - 1) {
            move_down(1);
        }
    }
    
    // Move back to the first line
    if (num_bars > 1) {
        move_up(num_bars - 1);
    }

    // Print all progress bars
    for (size_t i = 0; i < bars_.size(); ++i) {
        std::cout << bars_[i]->render();
        if (i < bars_.size() - 1) {
            std::cout << "\n";
        }
    }

    std::cout << std::flush;
}

inline void MultiProgress::tick(size_t index, int64_t amount) {
    if (index >= bars_.size()) {
        return;
    }

    bars_[index]->tick(amount);
    refresh();
}

inline void MultiProgress::move_up(int lines) {
    if (lines <= 0) {
        return;
    }
    terminal::move_cursor_up(lines);
}

inline void MultiProgress::move_down(int lines) {
    if (lines <= 0) {
        return;
    }
    terminal::move_cursor_down(lines);
}

inline void MultiProgress::erase_line() {
    terminal::clear_line();
}

}  // namespace cppbar
