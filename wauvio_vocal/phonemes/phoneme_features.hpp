#pragma once

namespace wauvio::vocal {
    enum class Place {
        None,
        Bilabial,
        Labiodental,
        Dental,
        Alveolar,
        Postalveolar,
        Palatal,
        Velar,
        Glottal,
    };

    enum class Manner {
        Vowel,
        Nasal,
        Fricative,
        Stop,
        Approximant,
        Silence,
    };

    struct PhonemeFeatures {
        Place  place  = Place::None;
        Manner manner = Manner::Vowel;
        bool   voiced = true;
    };
}
