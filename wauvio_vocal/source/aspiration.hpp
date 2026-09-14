#pragma once

#include "../../wauvio.hpp"
#include <cmath>
#include <algorithm>

namespace wauvio::vocal {
    class Aspiration {
    public:
        explicit Aspiration(uint32_t seed = 1013u) : noise_(seed) {}

        void setAmount(float amount01) noexcept { amount_ = std::clamp(amount01, 0.0f, 1.0f); }
        float amount() const noexcept { return amount_; }

        float nextSample(float phase01, float openQuotient01) noexcept {
            if (amount_ <= 0.0f) return 0.0f;
            const float envelope = openPhaseEnvelope(phase01, openQuotient01);
            return noise_.tick_white() * envelope * amount_;
        }

        void reset() noexcept { /* white noise has no state worth resetting */ }

    private:
        static float openPhaseEnvelope(float phase01, float oq) noexcept {
            oq = std::clamp(oq, 0.05f, 0.98f);
            if (phase01 >= oq) return 0.0f;
            const float x = phase01 / oq;
            constexpr float kTwoPi = 6.283185307179586f;
            return 0.5f - 0.5f * std::cos(kTwoPi * x);
        }

        wauvio::NoiseGenerator noise_;
        float amount_ = 0.0f;
    };
}
