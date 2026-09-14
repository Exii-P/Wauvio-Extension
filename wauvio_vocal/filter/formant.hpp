#pragma once

namespace wauvio::vocal {
    struct Formant {
        float freqHz   = 500.0f;
        float bwHz     = 80.0f;
        float ampLinear = 1.0f;
    };
}
