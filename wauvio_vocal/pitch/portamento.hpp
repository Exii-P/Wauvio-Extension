#pragma once

#include <algorithm>

namespace wauvio::vocal {
    class Portamento {
    public:
        void setSampleRate(int sr) noexcept { sampleRate_ = sr > 0 ? sr : 44100; }

        void start(float fromCents, float toCents, float durationMs) noexcept {
            fromCents_ = fromCents;
            toCents_ = toCents;
            durationSamples_ = std::max(0, static_cast<int>(durationMs * 0.001f * sampleRate_));
            elapsedSamples_ = 0;
        }

        bool active() const noexcept { return elapsedSamples_ < durationSamples_; }

        float next() noexcept {
            if (durationSamples_ <= 0 || elapsedSamples_ >= durationSamples_) return toCents_;
            const float t = static_cast<float>(elapsedSamples_) / static_cast<float>(durationSamples_);
            ++elapsedSamples_;
            return fromCents_ + t * (toCents_ - fromCents_);
        }

    private:
        int   sampleRate_ = 44100;
        float fromCents_ = 0.0f;
        float toCents_ = 0.0f;
        int   durationSamples_ = 0;
        int   elapsedSamples_ = 0;
    };
}
