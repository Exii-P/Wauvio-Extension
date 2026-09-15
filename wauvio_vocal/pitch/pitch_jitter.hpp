#pragma once

#include "../../wauvio.hpp"
#include <algorithm>
#include <cmath>

namespace wauvio::vocal {
    class PitchJitter {
    public:
        explicit PitchJitter(uint32_t seed = 9001u) : noise_(seed) {}

        void setSampleRate(int sr) noexcept {
            sampleRate_ = sr > 0 ? sr : 44100;
            updateCoeff();
        }

        void configure(float amountCents, float cutoffHz = 4.0f) noexcept {
            amountCents_ = amountCents;
            cutoffHz_ = std::max(cutoffHz, 0.1f);
            updateCoeff();
        }

        float next() noexcept {
            const float x = noise_.tick_white();
            lp_ += coeff_ * (x - lp_);
            return lp_ * outputScale_;
        }

        void reset() noexcept { lp_ = 0.0f; }

    private:
        void updateCoeff() noexcept {

            constexpr float kTwoPi = 6.283185307179586f;
            const float x = std::exp(-kTwoPi * cutoffHz_ / static_cast<float>(sampleRate_));
            coeff_ = 1.0f - x;

            constexpr float kInputStd = 0.5774f;
            const float unscaledStd = std::sqrt(coeff_ / (2.0f - coeff_)) * kInputStd;
            outputScale_ = amountCents_ / std::max(unscaledStd, 1.0e-6f);
        }

        wauvio::NoiseGenerator noise_;
        int   sampleRate_ = 44100;
        float amountCents_ = 3.0f;
        float cutoffHz_ = 4.0f;
        float coeff_ = 0.01f;
        float outputScale_ = 1.0f;
        float lp_ = 0.0f;
    };
}
