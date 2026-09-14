#pragma once

#include "../phonemes/phoneme.hpp"
#include "../filter/formant.hpp"
#include <array>

namespace wauvio::vocal {
    inline constexpr size_t kF2Index = 1;

    inline float locusF2(const Phoneme& consonant, float vowelF2Hz) noexcept {
        return consonant.f2LocusIntercept + consonant.f2LocusSlope * vowelF2Hz;
    }

    inline std::array<Formant, 5> coarticulatedTargets(const Phoneme& consonant, const Phoneme& adjacentVowel) noexcept {
        std::array<Formant, 5> out;
        for (size_t i = 0; i < 5; ++i) {
            out[i] = Formant{consonant.formantFreq[i], consonant.formantBW[i], 1.0f};
        }
        out[kF2Index].freqHz = locusF2(consonant, adjacentVowel.formantFreq[kF2Index]);
        return out;
    }
}
