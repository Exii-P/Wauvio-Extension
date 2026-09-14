#pragma once

#include "glottal_source.hpp"
#include "voice_quality.hpp"
#include "aspiration.hpp"
#include "../../wauvio.hpp"
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

namespace wauvio::vocal {
    class RosenbergModel : public GlottalSource {
    public:
        explicit RosenbergModel(int sampleRate = 0, uint32_t seed = 7919u)
            : aspiration_(seed ^ 0xA5A5A5A5u), rng_(seed) {
            sampleRate_ = sampleRate > 0 ? sampleRate : wauvio::global_config().sample_rate;
            currentCycleF0_ = f0_;
            rebuildTable();
        }

        void setF0(float hz) noexcept override {
            hz = std::max(hz, 20.0f);
            if (std::abs(hz - f0_) > 0.01f) {
                f0_ = hz;
                dirty_ = true;
            }
        }

        void setQuality(float rd) noexcept override {
            if (std::abs(rd - quality_.rd()) > 1e-4f) {
                quality_.setRd(rd);
                dirty_ = true;
            }
        }

        void setOpenQuotient(float oq) noexcept override {
            oq = std::clamp(oq, 0.1f, 0.95f);
            if (!useManualOQ_ || std::abs(oq - manualOQ_) > 1e-4f) {
                manualOQ_ = oq;
                useManualOQ_ = true;
                dirty_ = true;
            }
        }

        void setAspiration(float amount) noexcept override { aspiration_.setAmount(amount); }
        void setJitter(float amount01) noexcept override { jitterAmount_ = std::clamp(amount01, 0.0f, 1.0f); }
        void setShimmer(float amount01) noexcept override { shimmerAmount_ = std::clamp(amount01, 0.0f, 1.0f); }

        float nextSample() noexcept override {
            if (dirty_) rebuildTable();

            const float periodSamples = static_cast<float>(sampleRate_) / currentCycleF0_;
            const float tablePos = phase_ * (static_cast<float>(table_.size()) - 1.0f);
            const int   i0 = static_cast<int>(tablePos);
            const int   i1 = std::min(i0 + 1, static_cast<int>(table_.size()) - 1);
            const float frac = tablePos - static_cast<float>(i0);
            const float glottal = (table_[i0] + frac * (table_[i1] - table_[i0])) * currentCycleGain_;

            const float asp = aspiration_.nextSample(phase_, currentOpenQuotient_);

            phase_ += 1.0f / periodSamples;
            if (phase_ >= 1.0f) {
                phase_ -= 1.0f;
                beginNewCycle();
            }

            return glottal + asp;
        }

        void reset() noexcept override {
            phase_ = 0.0f;
            currentCycleF0_ = f0_;
            currentCycleGain_ = 1.0f;
            aspiration_.reset();
        }

        float currentRd() const noexcept { return quality_.rd(); }
        float currentF0() const noexcept { return f0_; }

    private:
        void beginNewCycle() noexcept {
            const float jitterRange = 0.03f * jitterAmount_;
            currentCycleF0_ = f0_ * (1.0f + jitterRange * uniform_());
            currentCycleF0_ = std::max(currentCycleF0_, 20.0f);

            const float shimmerRangeDb = 3.0f * shimmerAmount_;
            const float shimmerDb = shimmerRangeDb * uniform_();
            currentCycleGain_ = std::pow(10.0f, shimmerDb / 20.0f);
        }

        float uniform_() noexcept { return dist_(rng_); }

        void rebuildTable() noexcept {
            constexpr size_t n = 128;
            table_.assign(n, 0.0f);

            const float oq = useManualOQ_ ? manualOQ_ : quality_.openQuotient();
            const float openFrac = std::clamp(oq, 0.05f, 0.98f);
            currentOpenQuotient_ = openFrac;
            const float k1 = 1.0f - quality_.openPhaseAsymmetry();

            const size_t openSamples = static_cast<size_t>(openFrac * static_cast<float>(n));
            const size_t n1 = static_cast<size_t>(k1 * static_cast<float>(openSamples));
            const size_t n2 = openSamples - n1;

            for (size_t i = 0; i < n; ++i) {
                float g;
                if (n1 > 0 && i < n1) {
                    g = 0.5f - 0.5f * std::cos(static_cast<float>(PI_) * static_cast<float>(i) / static_cast<float>(n1));
                } else if (n2 > 0 && i < n1 + n2) {
                    const float t = static_cast<float>(i - n1);
                    g = std::cos(static_cast<float>(PI_) * t / (2.0f * static_cast<float>(n2)));
                } else {
                    g = 0.0f;
                }
                table_[i] = g;
            }

            dirty_ = false;
        }

        static constexpr double PI_ = 3.14159265358979323846;

        int    sampleRate_ = 44100;
        float  f0_ = 220.0f;
        VoiceQuality quality_;
        bool   useManualOQ_ = false;
        float  manualOQ_ = 0.5f;

        Aspiration aspiration_;
        float  jitterAmount_ = 0.0f;
        float  shimmerAmount_ = 0.0f;

        float  phase_ = 0.0f;
        float  currentCycleF0_ = 220.0f;
        float  currentCycleGain_ = 1.0f;
        float  currentOpenQuotient_ = 0.5f;
        bool   dirty_ = true;
        std::vector<float> table_;

        std::mt19937 rng_;
        std::uniform_real_distribution<float> dist_{-1.0f, 1.0f};
    };
}
