#pragma once

#include "resonator.hpp"
#include "formant.hpp"
#include <array>
#include <cstddef>

namespace wauvio::vocal {
    class CascadeFormantBank {
    public:
        static constexpr size_t kNumFormants = 5;

        void setSampleRate(int sr) noexcept {
            for (auto& r : stages_) r.setSampleRate(sr);
        }

        void setTargets(const std::array<Formant, kNumFormants>& formants) noexcept {
            for (size_t i = 0; i < kNumFormants; ++i) {
                stages_[i].setBandwidth(formants[i].bwHz);
                stages_[i].setFrequency(formants[i].freqHz);
            }
        }

        void setTarget(size_t idx, const Formant& f) noexcept {
            if (idx >= kNumFormants) return;
            stages_[idx].setBandwidth(f.bwHz);
            stages_[idx].setFrequency(f.freqHz);
        }

        float process(float x) noexcept {
            for (auto& r : stages_) x = r.process(x);
            return x;
        }

        void reset() noexcept { for (auto& r : stages_) r.reset(); }

    private:
        std::array<FormantResonator, kNumFormants> stages_{};
    };

    class ParallelFormantBank {
    public:
        static constexpr size_t kNumFormants = 5;

        void setSampleRate(int sr) noexcept {
            for (auto& r : stages_) r.setSampleRate(sr);
        }

        void setTargets(const std::array<Formant, kNumFormants>& formants) noexcept {
            formants_ = formants;
            for (size_t i = 0; i < kNumFormants; ++i) {
                stages_[i].setBandwidth(formants[i].bwHz);
                stages_[i].setFrequency(formants[i].freqHz);
            }
        }

        void setTarget(size_t idx, const Formant& f) noexcept {
            if (idx >= kNumFormants) return;
            formants_[idx] = f;
            stages_[idx].setBandwidth(f.bwHz);
            stages_[idx].setFrequency(f.freqHz);
        }

        float process(float x) noexcept {
            float sum = 0.0f;
            for (size_t i = 0; i < kNumFormants; ++i) {
                sum += formants_[i].ampLinear * stages_[i].process(x);
            }
            return sum;
        }

        void reset() noexcept { for (auto& r : stages_) r.reset(); }

    private:
        std::array<FormantResonator, kNumFormants> stages_{};
        std::array<Formant, kNumFormants> formants_{};
    };
}
