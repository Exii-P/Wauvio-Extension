#pragma once

#include <algorithm>
#include <cmath>

namespace wauvio::vocal {
    class DampedFollower {
    public:
        void setSampleRate(int sr) noexcept { sampleRate_ = sr > 0 ? sr : 44100; }

        void configure(float naturalFreqHz, float dampingRatio, float initialResponse = 0.0f) noexcept {
            f_ = std::max(naturalFreqHz, 0.01f);
            zeta_ = std::max(dampingRatio, 0.0f);
            r_ = initialResponse;
            recompute();
        }

        void reset(float value) noexcept {
            y_ = value;
            yd_ = 0.0f;
            prevTarget_ = value;
        }

        float process(float target) noexcept {
            const float dt = 1.0f / static_cast<float>(sampleRate_);
            const float xd = (target - prevTarget_) / dt;
            prevTarget_ = target;

            y_  += dt * yd_;
            yd_ += dt * (target + k3_ * xd - y_ - k1_ * yd_) / k2_;
            return y_;
        }

        float value() const noexcept { return y_; }
        float velocity() const noexcept { return yd_; }

    private:
        void recompute() noexcept {
            constexpr float TWO_PI_ = 6.283185307179586f;
            constexpr float PI_ = 3.14159265358979323846f;
            k1_ = zeta_ / (PI_ * f_);
            k2_ = 1.0f / (TWO_PI_ * f_ * TWO_PI_ * f_);
            k3_ = r_ * zeta_ / (PI_ * f_);
        }

        int   sampleRate_ = 44100;
        float f_ = 5.0f, zeta_ = 0.7f, r_ = 0.0f;
        float k1_ = 0.0f, k2_ = 1.0f, k3_ = 0.0f;
        float y_ = 0.0f, yd_ = 0.0f, prevTarget_ = 0.0f;
    };
}
