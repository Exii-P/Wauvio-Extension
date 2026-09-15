#pragma once

#include <vector>

namespace wauvio::vocal::languages {
    enum class AccentTone { High, Low };

    struct PitchAccentPattern {
        std::vector<AccentTone> moraTones;
    };

    inline float accentBiasCents(AccentTone tone) noexcept {
        return tone == AccentTone::High ? 0.0f : -60.0f;
    }
}
