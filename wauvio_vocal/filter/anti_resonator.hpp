#pragma once

#include "resonator.hpp"
#include "../../wauvio.hpp"

namespace wauvio::vocal {
    class AntiResonator {
    public:
        AntiResonator() = default;

        AntiResonator(double freqHz, double bwHz, int sampleRate = 0)
            : pole_(freqHz, bwHz, sampleRate) {
            updateReciprocal();
        }

        void setSampleRate(int sr) noexcept { pole_.setSampleRate(sr); updateReciprocal(); }
        void setFrequency(double hz) noexcept { pole_.setFrequency(hz); updateReciprocal(); }
        void setBandwidth(double hz) noexcept { pole_.setBandwidth(hz); updateReciprocal(); }

        float process(float x) noexcept {
            const float y = static_cast<float>(Ap_) * x
                          + static_cast<float>(Bp_) * x1_
                          + static_cast<float>(Cp_) * x2_;
            x2_ = x1_;
            x1_ = x;
            return y;
        }

        void process(wauvio::Buffer& buf) noexcept {
            for (auto& s : buf) s = process(s);
        }

        void reset() noexcept { x1_ = x2_ = 0.0f; pole_.reset(); }

        double frequency() const noexcept { return pole_.frequency(); }
        double bandwidth() const noexcept { return pole_.bandwidth(); }

    private:
        void updateReciprocal() noexcept {
            const double A = pole_.rawA();
            Ap_ = 1.0 / A;
            Bp_ = -pole_.rawB() / A;
            Cp_ = -pole_.rawC() / A;
        }

        FormantResonator pole_;
        double Ap_ = 1.0, Bp_ = 0.0, Cp_ = 0.0;
        float  x1_ = 0.0f, x2_ = 0.0f;
    };
}
