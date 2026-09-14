#pragma once

#include <algorithm>
#include <vector>

namespace wauvio::vocal {
    class SourceEnvelope {
    public:
        struct Segment {
            int   startSample;
            float startLevel;
            int   endSample;
            float endLevel;
        };

        void clear() noexcept { segments_.clear(); }

        void addSegment(int startSample, float startLevel, int endSample, float endLevel) {
            segments_.push_back({startSample, startLevel, std::max(endSample, startSample + 1), endLevel});
        }

        float levelAt(int sampleIndex) const noexcept {
            for (const auto& seg : segments_) {
                if (sampleIndex >= seg.startSample && sampleIndex < seg.endSample) {
                    const float t = static_cast<float>(sampleIndex - seg.startSample) /
                                     static_cast<float>(seg.endSample - seg.startSample);
                    return seg.startLevel + t * (seg.endLevel - seg.startLevel);
                }
            }
            if (segments_.empty()) return 0.0f;
            return sampleIndex < segments_.front().startSample ? segments_.front().startLevel
                                                                 : segments_.back().endLevel;
        }

    private:
        std::vector<Segment> segments_;
    };
}
