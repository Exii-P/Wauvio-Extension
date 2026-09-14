#pragma once

#include "resonator.hpp"
#include "anti_resonator.hpp"
#include "formant_bank.hpp"
#include "filter_models.hpp"
#include <array>

namespace wauvio::vocal {
    class VocalTract {
    public:
        void setSampleRate(int sr) noexcept {
            cascade_.setSampleRate(sr);
            parallel_.setSampleRate(sr);
            nasalPole_.setSampleRate(sr);
            nasalZero_.setSampleRate(sr);
        }

        void setCascadeTargets(const std::array<Formant, 5>& formants) noexcept {
            cascade_.setTargets(formants);
        }

        void setParallelTargets(const std::array<Formant, 5>& formants) noexcept {
            parallel_.setTargets(formants);
        }

        void setTargets(const VocalTractTargets& t) noexcept {
            setCascadeTargets(t.cascade);
            setParallelTargets(t.parallel);
            setNasalEnabled(t.nasalEnabled);
        }

        void setNasalEnabled(bool on) noexcept { nasalEnabled_ = on; }

        void setNasalPole(float freqHz, float bwHz) noexcept { nasalPole_.setFrequency(freqHz); nasalPole_.setBandwidth(bwHz); }
        void setNasalZero(float freqHz, float bwHz) noexcept { nasalZero_.setFrequency(freqHz); nasalZero_.setBandwidth(bwHz); }

        float process(float voicedAndAspiration, float frication) noexcept {
            float cascadeOut = cascade_.process(voicedAndAspiration);
            if (nasalEnabled_) {
                cascadeOut = nasalZero_.process(cascadeOut);
                cascadeOut = nasalPole_.process(cascadeOut);
            }
            const float parallelOut = parallel_.process(frication);
            const float summed = cascadeOut + parallelOut;

            const float radiated = summed - prevRadiated_;
            prevRadiated_ = summed;
            return radiated;
        }

        void reset() noexcept {
            cascade_.reset();
            parallel_.reset();
            nasalPole_.reset();
            nasalZero_.reset();
            prevRadiated_ = 0.0f;
        }

    private:
        CascadeFormantBank cascade_;
        ParallelFormantBank parallel_;
        FormantResonator nasalPole_{250.0, 100.0};
        AntiResonator    nasalZero_{250.0, 100.0};
        bool  nasalEnabled_ = false;
        float prevRadiated_ = 0.0f;
    };
}
