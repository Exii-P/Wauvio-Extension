#pragma once

#include "../../wauvio.hpp"
#include <cmath>
#include <algorithm>

namespace wauvio::vocal {
    class FormantResonator {
    public:
        FormantResonator() { setSampleRate(wauvio::global_config().sample_rate); }

        FormantResonator(double freqHz, double bwHz, int sampleRate = 0) {
            setSampleRate(sampleRate > 0 ? sampleRate : wauvio::global_config().sample_rate);
            setBandwidth(bwHz);
            setFrequency(freqHz);
        }

        void setSampleRate(int sr) noexcept {
            sampleRate_ = sr > 0 ? sr : wauvio::global_config().sample_rate;
            updateCoeffs();
        }

        void setFrequency(double hz) noexcept {
            const double nyquistLimit = 0.45 * static_cast<double>(sampleRate_);
            freq_ = std::clamp(hz, 0.0, nyquistLimit);
            updateCoeffs();
        }

        void setBandwidth(double hz) noexcept {
            bw_ = std::max(hz, 1.0);
            updateCoeffs();
        }

        float process(float x) noexcept {
            const float y = static_cast<float>(A_) * x
                          + static_cast<float>(B_) * y1_
                          + static_cast<float>(C_) * y2_;
            y2_ = y1_;
            y1_ = y;
            return y;
        }

        void process(wauvio::Buffer& buf) noexcept {
            for (auto& s : buf) s = process(s);
        }

        void reset() noexcept { y1_ = y2_ = 0.0f; }

        double frequency() const noexcept { return freq_; }
        double bandwidth() const noexcept { return bw_; }
        int    sampleRate() const noexcept { return sampleRate_; }

        double rawA() const noexcept { return A_; }
        double rawB() const noexcept { return B_; }
        double rawC() const noexcept { return C_; }

    private:
        void updateCoeffs() noexcept {
            const double T = 1.0 / static_cast<double>(sampleRate_);
            C_ = -std::exp(-2.0 * PI_ * bw_ * T);
            B_ = 2.0 * std::exp(-PI_ * bw_ * T) * std::cos(2.0 * PI_ * freq_ * T);
            A_ = 1.0 - B_ - C_;
        }

        static constexpr double PI_ = 3.14159265358979323846;

        int    sampleRate_ = 44100;
        double freq_ = 500.0;
        double bw_   = 80.0;
        double A_ = 1.0, B_ = 0.0, C_ = 0.0;
        float  y1_ = 0.0f, y2_ = 0.0f;
    };
}

