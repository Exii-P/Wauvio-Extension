#pragma once

#include "phoneme.hpp"
#include <vector>

namespace wauvio::vocal {
    struct Syllable {
        std::vector<Phoneme> onset;     // 0+ consonants before the nucleus
        Phoneme               nucleus;  // exactly one vowel (or nasal, for JP moraic ん)
        std::vector<Phoneme> coda;      // 0+ consonants after the nucleus
    };
}
