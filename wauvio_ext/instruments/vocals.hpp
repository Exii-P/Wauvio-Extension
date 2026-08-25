#pragma once
#include "../model_instruments.hpp"

namespace wauvio {
namespace instruments {
using namespace wauvio::audio;

class Choir : public VocalInstrument {
public:
    Choir() : VocalInstrument("Choir") {
        recipe.detune_cents = 16.0; recipe.stereo_width = 0.45; recipe.envelope.attack = 0.15;
    }
};

class MaleChoir : public VocalInstrument {
public:
    MaleChoir() : VocalInstrument("Male Choir") {
        recipe.formant1_hz = 550.0; recipe.detune_cents = 12.0; recipe.filter_cutoff = 3000.0;
    }
};

class FemaleChoir : public VocalInstrument {
public:
    FemaleChoir() : VocalInstrument("Female Choir") {
        recipe.formant1_hz = 900.0; recipe.detune_cents = 12.0; recipe.filter_cutoff = 4600.0;
    }
};

class ChildrensChoir : public VocalInstrument {
public:
    ChildrensChoir() : VocalInstrument("Children's Choir") {
        recipe.formant1_hz = 1200.0; recipe.detune_cents = 10.0; recipe.filter_cutoff = 5200.0;
    }
};

class VocalPad : public VocalInstrument {
public:
    VocalPad() : VocalInstrument("Vocal Pad") {
        recipe.detune_cents = 20.0; recipe.stereo_width = 0.55; recipe.envelope.attack = 0.6;
        recipe.envelope.release = 1.0;
    }
};

class SoloVocal : public VocalInstrument {
public:
    SoloVocal() : VocalInstrument("Solo Vocal") {
        recipe.formant1_hz = 750.0; recipe.vibrato_depth_cents = 18.0;
    }
};

class VocalAh : public VocalInstrument {
public:
    VocalAh() : VocalInstrument("Vocal Ah") {
        recipe.formant1_hz = 800.0; recipe.formant1_gain_db = 7.0;
        recipe.formant2_hz = 1150.0; recipe.formant2_gain_db = 4.0;
    }
};

class VocalOo : public VocalInstrument {
public:
    VocalOo() : VocalInstrument("Vocal Oo") {
        recipe.formant1_hz = 350.0; recipe.formant1_gain_db = 7.0;
        recipe.formant2_hz = 800.0; recipe.formant2_gain_db = 3.0; recipe.filter_cutoff = 2400.0;
    }
};

class VocalMm : public VocalInstrument {
public:
    VocalMm() : VocalInstrument("Vocal Mm") {
        recipe.formant1_hz = 250.0; recipe.formant1_gain_db = 6.0; recipe.filter_cutoff = 1600.0;
        recipe.noise_mix = 0.0;
    }
};

}
}
