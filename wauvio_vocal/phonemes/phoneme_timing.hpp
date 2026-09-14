#pragma once

#include "syllable.hpp"
#include <vector>
#include <algorithm>

namespace wauvio::vocal {
    struct PhonemeTiming {
        const Phoneme* phoneme;
        double startMs;
        double durationMs;
        const Phoneme* adjacentVowel = nullptr;
    };

    class PhonemeTimingRule {
    public:
        static constexpr float kConsonantDurationMs = 80.0f;
        static constexpr float kMinVowelDurationMs = 20.0f;

        static std::vector<PhonemeTiming> apportion(const Syllable& syl, double noteDurationMs) {
            std::vector<PhonemeTiming> out;
            const size_t consonantCount = syl.onset.size() + syl.coda.size();

            float consonantDurEach = kConsonantDurationMs;
            double consonantTotal = consonantCount * static_cast<double>(consonantDurEach);
            double vowelDur = noteDurationMs - consonantTotal;

            if (vowelDur < kMinVowelDurationMs && consonantCount > 0) {
                const double available = std::max(0.0, noteDurationMs - kMinVowelDurationMs);
                consonantDurEach = static_cast<float>(available / consonantCount);
                consonantTotal = consonantCount * static_cast<double>(consonantDurEach);
                vowelDur = noteDurationMs - consonantTotal;
            }
            vowelDur = std::max(vowelDur, 0.0);

            double cursor = 0.0;
            for (const auto& c : syl.onset) {
                out.push_back({&c, cursor, consonantDurEach, &syl.nucleus});
                cursor += consonantDurEach;
            }
            out.push_back({&syl.nucleus, cursor, vowelDur, nullptr});
            cursor += vowelDur;
            for (const auto& c : syl.coda) {
                out.push_back({&c, cursor, consonantDurEach, &syl.nucleus});
                cursor += consonantDurEach;
            }
            return out;
        }
    };
}
