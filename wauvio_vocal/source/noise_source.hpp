#pragma once

#include "../../wauvio.hpp"
#include <algorithm>

namespace wauvio::vocal {
    class NoiseSource {
    public:
        explicit NoiseSource(uint32_t seed = 1u) : noise_(seed) {}

        void  setAmplitude(float amp) noexcept { amp_ = std::max(amp, 0.0f); }
        float amplitude() const noexcept { return amp_; }

        float nextSample() noexcept { return noise_.tick_white() * amp_; }
        void  reset() noexcept { /* white noise carries no state worth resetting */ }

    private:
        wauvio::NoiseGenerator noise_;
        float amp_ = 0.0f;
    };
}
