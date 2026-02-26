#pragma once

#include "terminal/terminal.hpp"
#include "terminal/terminal_impl.hpp"
#include "style/style.hpp"
#include "style/style_impl.hpp"
#include "color/color.hpp"
#include "color/color_impl.hpp"
#include "animation/animation.hpp"
#include "animation/animation_impl.hpp"
#include "utils/clock.hpp"
#include "utils/format.hpp"
#include "utils/math.hpp"

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
      started_(false), finished_(false), hidden_(false), multi_progress_mode_(false), last_update_(0) {}

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