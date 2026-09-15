#pragma once

#include "../utility/damped_follower.hpp"
#include <algorithm>
#include <cmath>

namespace wauvio::vocal {
    struct PitchDynamics {
        float vibratoRateHz = 5.5f;
        float vibratoDepthCents = 40.0f;
        float vibratoOnsetMs = 250.0f;
        float overshootDamping = 0.65f;
        float overshootNaturalHz = 8.0f;
        float preparationCents = -15.0f;
        float fineFluctuationCents = 3.0f;
        float portamentoMs = 0.0f;
    };

    class PitchDynamicsCore {
    public:
        void setSampleRate(int sr) noexcept {
            sampleRate_ = sr > 0 ? sr : 44100;
            follower_.setSampleRate(sampleRate_);
        }

        void configure(const PitchDynamics& params) noexcept {
            params_ = params;
            follower_.configure(std::max(params_.overshootNaturalHz, 0.1f),
                                 std::max(params_.overshootDamping, 0.0f));
        }

        void reset(float cents) noexcept {
            follower_.reset(cents);
            targetCents_ = cents;
            prepElapsedSamples_ = -1;
        }

        void noteOn(float targetCents) noexcept {
            targetCents_ = targetCents;
            prepElapsedSamples_ = 0;
        }

        float process(const float* externalTarget = nullptr) noexcept {
            const float target = externalTarget ? *externalTarget : targetCents_;
            float chased = follower_.process(target);

            if (prepElapsedSamples_ >= 0) {
                const float prepDurationMs = 60.0f;
                const float elapsedMs = 1000.0f * static_cast<float>(prepElapsedSamples_) / static_cast<float>(sampleRate_);
                if (elapsedMs < prepDurationMs) {

                    const float x = elapsedMs / prepDurationMs;
                    const float envelope = 0.5f - 0.5f * std::cos(2.0f * 3.14159265358979323846f * x);
                    chased += params_.preparationCents * envelope;
                    ++prepElapsedSamples_;
                } else {
                    prepElapsedSamples_ = -1;
                }
            }

            return chased;
        }

        float targetCents() const noexcept { return targetCents_; }

    private:
        int   sampleRate_ = 44100;
        PitchDynamics params_;
        DampedFollower follower_;
        float targetCents_ = 0.0f;
        int   prepElapsedSamples_ = -1;
    };
}
