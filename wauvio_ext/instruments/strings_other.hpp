#pragma once
#include "../model_instruments.hpp"

namespace wauvio {
namespace instruments {
using namespace wauvio::audio;

class BassGuitar : public PluckedStringInstrument {
public:
    BassGuitar() : PluckedStringInstrument("Bass Guitar") {
        recipe.filter_cutoff = 1800.0; recipe.osc_mix = 0.15; recipe.envelope.decay = 0.7;
    }
};

class FretlessBass : public PluckedStringInstrument {
public:
    FretlessBass() : PluckedStringInstrument("Fretless Bass") {
        recipe.filter_cutoff = 1500.0; recipe.vibrato_depth_cents = 8.0; recipe.envelope.decay = 0.9;
    }
};

class UprightBass : public BowedStringInstrument {
public:
    UprightBass() : BowedStringInstrument("Upright Bass") {
        recipe.filter_cutoff = 1400.0; recipe.formant1_hz = 150.0; recipe.osc_mix = 0.55;
        recipe.art_decay_scale[wauvio::audio::Articulation::Pizzicato] = 0.3;
    }
};

class Dulcimer : public PluckedStringInstrument {
public:
    Dulcimer() : PluckedStringInstrument("Dulcimer") {
        recipe.filter_cutoff = 5500.0; recipe.envelope.decay = 1.0; recipe.noise_attack_burst = 0.15;
    }
};

class HammeredDulcimer : public MalletInstrument {
public:
    HammeredDulcimer() : MalletInstrument("Hammered Dulcimer") {
        recipe.fm_ratio = 2.3; recipe.fm_index = 1.0; recipe.filter_cutoff = 6200.0;
        recipe.envelope.decay = 1.1;
    }
};

}
}
