#pragma once

#include "noise_source.hpp"
#include <algorithm>

namespace wauvio::vocal {
    class Frication {
    public:
        explicit Frication(uint32_t seed = 4021u) : source_(seed) {}

        void  setAmount(float amount01) noexcept { source_.setAmplitude(std::clamp(amount01, 0.0f, 1.0f)); }
        float amount() const noexcept { return source_.amplitude(); }

        float nextSample() noexcept { return source_.nextSample(); }
        void  reset() noexcept { source_.reset(); }

    private:
        NoiseSource source_;
    };
}
