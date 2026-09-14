#pragma once

namespace wauvio::vocal {
    class GlottalSource {
    public:
        virtual ~GlottalSource() = default;

        virtual void setF0(float hz) noexcept = 0;
        virtual void setQuality(float rd) noexcept = 0;
        virtual void setOpenQuotient(float oq) noexcept = 0;
        virtual void setAspiration(float amount) noexcept = 0;
        virtual void setJitter(float amount) noexcept = 0;
        virtual void setShimmer(float amount) noexcept = 0;

        virtual float nextSample() noexcept = 0;
        virtual void  reset() noexcept = 0;
    };
}