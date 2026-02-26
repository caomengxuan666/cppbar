#pragma once

#include "animation.hpp"
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