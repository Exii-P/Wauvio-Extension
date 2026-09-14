#pragma once

namespace wauvio::vocal {
    enum class PhonemeType {
        Silence,          // closure, pause
        Vowel,            // cascade branch, periodic
        Nasal,            // cascade branch + nasal pole/zero, periodic
        Fricative,        // parallel branch, noise only (voiceless: s, f, sh, h...)
        VoicedFricative,  // parallel branch noise + cascade voicing mixed (z, v, zh...)
        Plosive,          // closure (silence) + brief burst, +/- aspiration/voicing
        Aspirate,         // noise-excited, glottal-modulated (h, breathy release)
    };
}
