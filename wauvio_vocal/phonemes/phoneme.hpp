#pragma once

#include "phoneme_type.hpp"
#include "phoneme_features.hpp"
#include <array>
#include <string>

namespace wauvio::vocal {
    struct Phoneme {
        std::string symbol;
        PhonemeType type = PhonemeType::Silence;
        PhonemeFeatures features;

        std::array<float, 5> formantFreq{};
        std::array<float, 5> formantBW{};
        std::array<float, 5> formantAmpParallel{};

        float f2LocusSlope     = 1.0f;
        float f2LocusIntercept = 0.0f;
        float minDurationMs = 60.0f;
    };
}
