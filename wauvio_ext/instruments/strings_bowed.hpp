#pragma once
#include "../model_instruments.hpp"

namespace wauvio {
namespace instruments {
using namespace wauvio::audio;

class SoloViolin : public BowedStringInstrument {
public:
    SoloViolin() : BowedStringInstrument("Solo Violin") {
        recipe.filter_cutoff = 7000.0; recipe.formant1_hz = 1200.0;
    }
};

class Viola : public BowedStringInstrument {
public:
    Viola() : BowedStringInstrument("Viola") {
        recipe.filter_cutoff = 5200.0; recipe.formant1_hz = 850.0; recipe.vibrato_depth_cents = 16.0;
    }
};

class Cello : public BowedStringInstrument {
public:
    Cello() : BowedStringInstrument("Cello") {
        recipe.filter_cutoff = 3400.0; recipe.formant1_hz = 400.0; recipe.vibrato_rate_hz = 4.8;
        recipe.vibrato_depth_cents = 20.0;
    }
};

class DoubleBass : public BowedStringInstrument {
public:
    DoubleBass() : BowedStringInstrument("Double Bass") {
        recipe.filter_cutoff = 1600.0; recipe.formant1_hz = 180.0; recipe.vibrato_rate_hz = 4.0;
        recipe.vibrato_depth_cents = 14.0; recipe.osc_mix = 0.5;
    }
};

class StringEnsemble : public BowedStringInstrument {
public:
    StringEnsemble() : BowedStringInstrument("String Ensemble") {
        recipe.detune_cents = 14.0; recipe.stereo_width = 0.4; recipe.filter_cutoff = 5000.0;
        recipe.envelope.attack = 0.15;
    }
};

class ChamberStrings : public BowedStringInstrument {
public:
    ChamberStrings() : BowedStringInstrument("Chamber Strings") {
        recipe.detune_cents = 9.0; recipe.stereo_width = 0.3; recipe.filter_cutoff = 5600.0;
        recipe.envelope.attack = 0.08;
    }
};

class FullStringSection : public BowedStringInstrument {
public:
    FullStringSection() : BowedStringInstrument("Full String Section") {
        recipe.detune_cents = 20.0; recipe.stereo_width = 0.5; recipe.filter_cutoff = 4800.0;
        recipe.envelope.attack = 0.2; recipe.gain = 1.0;
    }
};

}
}
