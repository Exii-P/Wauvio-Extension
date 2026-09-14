#pragma once

#include <algorithm>

namespace wauvio::vocal {
    class VoiceQuality {
    public:
        void  setRd(float rd) noexcept { rd_ = std::clamp(rd, 0.0f, 1.0f); }
        float rd() const noexcept { return rd_; }

        float openQuotient() const noexcept { return 0.40f + 0.30f * rd_; }
        float spectralTiltDbPerOctave() const noexcept { return -12.0f - 6.0f * rd_; }
        float openPhaseAsymmetry() const noexcept { return 0.20f + 0.30f * rd_; }
        float returnPhaseTimeConstantFraction() const noexcept { return 0.01f + 0.04f * rd_; }

    private:
        float rd_ = 0.5f;
    };
}
