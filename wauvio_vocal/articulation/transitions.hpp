#pragma once

#include "../utility/damped_follower.hpp"
#include "../filter/formant.hpp"
#include "../phonemes/phoneme_type.hpp"
#include <array>
#include <cstddef>

namespace wauvio::vocal {
    enum class TransitionSpeed { Stop, Normal, Legato };

    inline TransitionSpeed speedForPhoneme(PhonemeType type) noexcept {
        return type == PhonemeType::Plosive ? TransitionSpeed::Stop : TransitionSpeed::Normal;
    }

    class FormantTransition {
    public:
        void setSampleRate(int sr) noexcept {
            for (auto& f : freqFollowers_) f.setSampleRate(sr);
            for (auto& f : bwFollowers_) f.setSampleRate(sr);
            configureDefaults();
        }

        void setSpeed(TransitionSpeed speed) noexcept {
            speed_ = speed;
            configureDefaults();
        }

        void reset(const std::array<Formant, 5>& formants) noexcept {
            for (size_t i = 0; i < 5; ++i) {
                freqFollowers_[i].reset(formants[i].freqHz);
                bwFollowers_[i].reset(formants[i].bwHz);
            }
        }

        std::array<Formant, 5> next(const std::array<Formant, 5>& target) noexcept {
            std::array<Formant, 5> out;
            for (size_t i = 0; i < 5; ++i) {
                out[i].freqHz = freqFollowers_[i].process(target[i].freqHz);
                out[i].bwHz   = bwFollowers_[i].process(target[i].bwHz);
                out[i].ampLinear = target[i].ampLinear;
            }
            return out;
        }

    private:
        void configureDefaults() noexcept {
            static constexpr std::array<float, 5> kBaseFreqHz = {20.0f, 14.0f, 12.0f, 10.0f, 10.0f};
            constexpr float kZeta = 0.9f;

            float speedMul = 1.0f;
            switch (speed_) {
                case TransitionSpeed::Stop:   speedMul = 2.5f; break;
                case TransitionSpeed::Normal: speedMul = 1.0f; break;
                case TransitionSpeed::Legato: speedMul = 0.4f; break;
            }
            for (size_t i = 0; i < 5; ++i) {
                freqFollowers_[i].configure(kBaseFreqHz[i] * speedMul, kZeta);
                bwFollowers_[i].configure(kBaseFreqHz[i] * speedMul, kZeta);
            }
        }

        std::array<DampedFollower, 5> freqFollowers_{};
        std::array<DampedFollower, 5> bwFollowers_{};
        TransitionSpeed speed_ = TransitionSpeed::Normal;
    };
}
