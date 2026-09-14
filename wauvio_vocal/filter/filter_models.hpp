#pragma once

#include "formant.hpp"
#include "../phonemes/phoneme.hpp"
#include <array>
#include <cstddef>

namespace wauvio::vocal {
    struct VocalTractTargets {
        std::array<Formant, 5> cascade;
        std::array<Formant, 5> parallel;
        bool nasalEnabled = false;
    };

    inline VocalTractTargets targetsFromPhoneme(const Phoneme& ph) noexcept {
        VocalTractTargets t;
        for (size_t i = 0; i < 5; ++i) {
            t.cascade[i]  = Formant{ph.formantFreq[i], ph.formantBW[i], 1.0f};
            t.parallel[i] = Formant{ph.formantFreq[i], ph.formantBW[i], ph.formantAmpParallel[i]};
        }
        t.nasalEnabled = (ph.type == PhonemeType::Nasal);
        return t;
    }
}
