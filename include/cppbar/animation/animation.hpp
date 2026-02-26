#pragma once

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