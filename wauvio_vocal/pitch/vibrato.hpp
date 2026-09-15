#pragma once

#include <cmath>
#include <algorithm>

namespace wauvio::vocal {
    class Vibrato {
    public:
        void setSampleRate(int sr) noexcept { sampleRate_ = sr > 0 ? sr : 44100; }

        void configure(float rateHz, float depthCents, float onsetMs) noexcept {
            rateHz_ = std::max(rateHz, 0.01f);
            depthCents_ = depthCents;
            onsetMs_ = std::max(onsetMs, 0.0f);
        }

        void noteOn() noexcept { onsetElapsedMs_ = 0.0f; }

        float next() noexcept {
            const float dt = 1000.0f / static_cast<float>(sampleRate_);
            phase_ += rateHz_ * (dt * 0.001f);
            if (phase_ >= 1.0f) phase_ -= std::floor(phase_);

            float depthMul = 1.0f;
            if (onsetMs_ > 0.0f) {
                depthMul = std::min(onsetElapsedMs_ / onsetMs_, 1.0f);
            }
            onsetElapsedMs_ += dt;

            constexpr float kTwoPi = 6.283185307179586f;
            return depthCents_ * depthMul * std::sin(kTwoPi * phase_);
        }

    private:
        int   sampleRate_ = 44100;
        float rateHz_ = 5.5f;
        float depthCents_ = 40.0f;
        float onsetMs_ = 250.0f;
        float phase_ = 0.0f;
        float onsetElapsedMs_ = 1.0e9f;
    };
}
