#pragma once

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