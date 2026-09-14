#pragma once

#include "phoneme_timing.hpp"
#include "syllable.hpp"
#include <vector>

namespace wauvio::vocal {
    class PhonemeSequence {
    public:
        void addSyllable(const Syllable& syl, double noteDurationMs) {
            std::vector<PhonemeTiming> timings = PhonemeTimingRule::apportion(syl, noteDurationMs);
            for (auto& t : timings) {
                t.startMs += cursorMs_;
                events_.push_back(t);
            }
            cursorMs_ += noteDurationMs;
        }

        void addRest(double durationMs) {
            restPhoneme_.type = PhonemeType::Silence;
            events_.push_back({&restPhoneme_, cursorMs_, durationMs});
            cursorMs_ += durationMs;
        }

        const std::vector<PhonemeTiming>& events() const noexcept { return events_; }
        double totalDurationMs() const noexcept { return cursorMs_; }

    private:
        std::vector<PhonemeTiming> events_;
        double cursorMs_ = 0.0;
        Phoneme restPhoneme_;
    };
}
