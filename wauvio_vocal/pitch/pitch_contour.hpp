#pragma once

#include "pitch_dynamics.hpp"
#include "vibrato.hpp"
#include "portamento.hpp"
#include "pitch_jitter.hpp"
#include <cmath>
#include <algorithm>

namespace wauvio::vocal {
    class PitchContour {
    public:
        void setSampleRate(int sr) noexcept {
            sampleRate_ = sr > 0 ? sr : 44100;
            core_.setSampleRate(sampleRate_);
            vibrato_.setSampleRate(sampleRate_);
            portamento_.setSampleRate(sampleRate_);
            jitter_.setSampleRate(sampleRate_);
        }

        void configure(const PitchDynamics& params) noexcept {
            params_ = params;
            core_.configure(params_);
            vibrato_.configure(params_.vibratoRateHz, params_.vibratoDepthCents, params_.vibratoOnsetMs);
            jitter_.configure(params_.fineFluctuationCents);
        }

        void reset(float f0Hz) noexcept {
            const float cents = hzToCents(f0Hz);
            core_.reset(cents);
            lastTargetCents_ = cents;
            portamento_.start(cents, cents, 0.0f);
        }

        void noteOn(float f0Hz) noexcept {
            const float targetCents = hzToCents(f0Hz);
            if (params_.portamentoMs > 0.0f) {
                portamento_.start(lastTargetCents_, targetCents, params_.portamentoMs);
                usingPortamento_ = true;
            } else {
                usingPortamento_ = false;
            }
            core_.noteOn(targetCents);
            vibrato_.noteOn();
            lastTargetCents_ = targetCents;
        }

        float next() noexcept {
            float cents;
            if (usingPortamento_) {
                const float rampedTarget = portamento_.next();
                cents = core_.process(&rampedTarget);
            } else {
                cents = core_.process();
            }
            cents += vibrato_.next();
            cents += jitter_.next();
            return centsToHz(cents);
        }

    private:
        float hzToCents(float hz) const noexcept {
            hz = std::max(hz, 1.0f);
            return 1200.0f * std::log2(hz / kReferenceHz);
        }
        float centsToHz(float cents) const noexcept {
            return kReferenceHz * std::exp2(cents / 1200.0f);
        }

        static constexpr float kReferenceHz = 130.0f;

        int sampleRate_ = 44100;
        PitchDynamics params_;
        PitchDynamicsCore core_;
        Vibrato vibrato_;
        Portamento portamento_;
        PitchJitter jitter_;
        float lastTargetCents_ = 0.0f;
        bool  usingPortamento_ = false;
    };
}
