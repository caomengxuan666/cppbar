#pragma once

#include "animation/animation.hpp"
#include "animation/animation_impl.hpp"
#include "color/color.hpp"
#include "color/color_impl.hpp"
#include "terminal/terminal.hpp"
#include "terminal/terminal_impl.hpp"
#include "utils/clock.hpp"

#include <string>
#include <memory>
#include <iostream>

namespace cppbar {

class Spinner {
public:
    explicit Spinner(const std::string& text = "");
    ~Spinner();

    void set_text(const std::string& text);
    void set_color(const color::Color& fg, const color::Color& bg = color::Color());
    void set_animation(animation::Animation* anim);

    void start();
    void stop();
    void finish(const std::string& message = "");

    void tick();

    std::string render() const;

    static Spinner* make_spinner(const std::string& text = "");

private:
    void init();
    void ensure_started();
    void refresh_display();

    std::string text_;
    color::Color fg_color_;
    color::Color bg_color_;
    std::unique_ptr<animation::Animation> animation_;
    bool animation_owned_;

    bool started_;
    bool stopped_;

    utils::Clock clock_;
    size_t frame_count_;

    mutable std::string last_render_;
};

inline Spinner::Spinner(const std::string& text)
    : text_(text), fg_color_(), bg_color_(), animation_(nullptr), animation_owned_(false),
      started_(false), stopped_(false), frame_count_(0) {}

inline Spinner::~Spinner() {
    if (started_ && !stopped_) {
        stop();
    }
}

inline void Spinner::set_text(const std::string& text) {
    text_ = text;
}

inline void Spinner::set_color(const color::Color& fg, const color::Color& bg) {
    fg_color_ = fg;
    bg_color_ = bg;
}

inline void Spinner::set_animation(animation::Animation* anim) {
    if (animation_ && animation_owned_) {
        animation_.reset();
    }
    animation_ = std::unique_ptr<animation::Animation>(anim);
    animation_owned_ = false;
}

inline void Spinner::init() {
    if (terminal::is_tty()) {
        terminal::hide_cursor();
    }

    if (animation_ == nullptr) {
        std::vector<std::string> frames = {"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
        animation_ = std::unique_ptr<animation::Animation>(new animation::CycleAnimation(frames));
        animation_owned_ = true;
    }
}

inline void Spinner::ensure_started() {
    if (!started_) {
        start();
    }
}

inline void Spinner::start() {
    if (started_) {
        return;
    }

    init();
    clock_.start();
    started_ = true;
    stopped_ = false;
    frame_count_ = 0;

    refresh_display();
}

inline void Spinner::stop() {
    if (!started_) {
        return;
    }

    stopped_ = true;

    if (terminal::is_tty()) {
        terminal::clear_line();
        terminal::show_cursor();
    }

    started_ = false;
}

inline void Spinner::finish(const std::string& message) {
    if (!started_) {
        return;
    }

    std::string output = "✓ ";
    if (!text_.empty()) {
        output += text_;
    }
    if (!message.empty()) {
        output += " - " + message;
    }

    // Clear the entire line first to avoid overlap
    if (terminal::is_tty()) {
        std::cout << "\033[2K";  // Clear entire line
    }

    // Print completion message and move to next line
    std::cout << "\r" << output << std::endl << std::flush;

    if (terminal::is_tty()) {
        terminal::show_cursor();
    }

    started_ = false;
    stopped_ = true;
}

inline void Spinner::tick() {
    ensure_started();

    if (stopped_) {
        return;
    }

    refresh_display();

    if (animation_) {
        animation_->next_frame();
    }
    frame_count_++;
}

inline void Spinner::refresh_display() {
    if (!terminal::is_tty()) {
        return;
    }

    std::string output = render();
    if (output != last_render_) {
        std::cout << "\r" << output << std::flush;
        last_render_ = output;
    }
}

inline std::string Spinner::render() const {
    std::ostringstream oss;

    std::string spinner_char = animation_ ? animation_->get_frame() : " ";

    if (!fg_color_.is_none() || !bg_color_.is_none()) {
        spinner_char = color::apply_color(spinner_char, fg_color_, bg_color_);
    }

    oss << spinner_char << " " << text_;

    return oss.str();
}

inline Spinner* Spinner::make_spinner(const std::string& text) {
    return new Spinner(text);
}

}  // namespace cppbar