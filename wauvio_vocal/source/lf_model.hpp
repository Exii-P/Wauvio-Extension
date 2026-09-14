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
    class LFModel : public GlottalSource {
    public:
        explicit LFModel(int sampleRate = 0, uint32_t seed = 26261u)
            : aspiration_(seed ^ 0x5A5A5A5Au), rng_(seed) {
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

        double debugAreaBalanceResidual() const noexcept {
            if (dirty_) { const_cast<LFModel*>(this)->rebuildTable(); }
            const double T0 = 1.0 / static_cast<double>(f0_);
            const double dt = T0 / static_cast<double>(table_.size() - 1);
            double integral = 0.0;
            for (size_t i = 0; i + 1 < table_.size(); ++i) {
                integral += 0.5 * (static_cast<double>(table_[i]) + static_cast<double>(table_[i + 1])) * dt;
            }
            return integral;
        }

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
            constexpr size_t n = 256;
            table_.assign(n, 0.0f);

            const double T0 = 1.0 / static_cast<double>(f0_);
            const double oq = useManualOQ_ ? static_cast<double>(manualOQ_)
                                            : static_cast<double>(quality_.openQuotient());
            currentOpenQuotient_ = static_cast<float>(std::clamp(oq, 0.05, 0.98));

            const double Te = currentOpenQuotient_ * T0;
            const double Rk = static_cast<double>(quality_.openPhaseAsymmetry());
            const double Tp = Te / (1.0 + Rk);
            const double wg = PI_ / Tp;
            const double Tr = T0 - Te;

            const double Ta = static_cast<double>(quality_.returnPhaseTimeConstantFraction()) * T0;

            const double epsilon = solveEpsilon(Ta, Tr);
            const double alpha   = solveAlpha(wg, Te, epsilon, Ta, Tr);

            const double sinWgTe = std::sin(wg * Te);
            const double E0 = -1.0 / (std::exp(alpha * Te) * sinWgTe);

            double peak = 1e-9;
            for (size_t i = 0; i < n; ++i) {
                const double t = (static_cast<double>(i) / static_cast<double>(n - 1)) * T0;
                double v;
                if (t < Te) {
                    v = E0 * std::exp(alpha * t) * std::sin(wg * t);
                } else {
                    const double u = t - Te;
                    v = -(1.0 / (epsilon * Ta)) *
                        (std::exp(-epsilon * u) - std::exp(-epsilon * Tr));
                }
                table_[i] = static_cast<float>(v);
                peak = std::max(peak, std::abs(v));
            }

            const float invPeak = static_cast<float>(1.0 / peak);
            for (auto& s : table_) s *= invPeak;

            dirty_ = false;
        }

        static double solveEpsilon(double Ta, double Tr) noexcept {
            auto f = [&](double eps) { return eps * Ta - 1.0 + std::exp(-eps * Tr); };

            double lo = 1e-6;
            double hi = 200.0 / std::max(Ta, 1e-9);
            double flo = f(lo);
            double fhi = f(hi);
            int guard = 0;
            while (flo * fhi > 0.0 && guard < 20) {
                hi *= 2.0;
                fhi = f(hi);
                ++guard;
            }
            for (int i = 0; i < 60; ++i) {
                const double mid = 0.5 * (lo + hi);
                const double fmid = f(mid);
                if (fmid * flo <= 0.0) { hi = mid; fhi = fmid; }
                else { lo = mid; flo = fmid; }
            }
            return 0.5 * (lo + hi);
        }

        static double solveAlpha(double wg, double Te, double epsilon, double Ta, double Tr) noexcept {
            const double closedArea =
                -(1.0 / (epsilon * Ta)) *
                ((1.0 - std::exp(-epsilon * Tr)) / epsilon - Tr * std::exp(-epsilon * Tr));

            auto openArea = [&](double alpha) {
                const double sinWgTe = std::sin(wg * Te);
                const double cosWgTe = std::cos(wg * Te);
                const double denom = alpha * alpha + wg * wg;
                const double Iopen = (std::exp(alpha * Te) * (alpha * sinWgTe - wg * cosWgTe) + wg) / denom;
                const double E0 = -1.0 / (std::exp(alpha * Te) * sinWgTe);
                return E0 * Iopen;
            };

            auto F = [&](double alpha) { return openArea(alpha) + closedArea; };

            const double scanMax = 8.0 * wg;
            const int    steps = 400;
            double prevA = 1e-6;
            double prevF = F(prevA);
            double loA = prevA, hiA = prevA;
            bool found = false;
            for (int i = 1; i <= steps && !found; ++i) {
                const double a = (scanMax * static_cast<double>(i)) / static_cast<double>(steps);
                const double fa = F(a);
                if (prevF * fa <= 0.0) {
                    loA = prevA; hiA = a;
                    found = true;
                    break;
                }
                prevA = a; prevF = fa;
            }
            if (!found) {
                return 0.3 * wg;
            }

            double flo = F(loA);
            for (int i = 0; i < 60; ++i) {
                const double mid = 0.5 * (loA + hiA);
                const double fmid = F(mid);
                if (fmid * flo <= 0.0) { hiA = mid; }
                else { loA = mid; flo = fmid; }
            }
            return 0.5 * (loA + hiA);
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
