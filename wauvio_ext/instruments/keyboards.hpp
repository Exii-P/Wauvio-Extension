#pragma once
#include "../model_instruments.hpp"

namespace wauvio {
namespace instruments {
using namespace wauvio::audio;

class AcousticGrandPiano : public KeyboardInstrument {
public:
    AcousticGrandPiano() : KeyboardInstrument("Acoustic Grand Piano") {
        recipe.filter_cutoff = 7200.0; recipe.osc_mix = 0.22; recipe.envelope.decay = 1.1;
        recipe.envelope.release = 0.4; recipe.stereo_width = 0.18;
    }
};

class UprightPiano : public KeyboardInstrument {
public:
    UprightPiano() : KeyboardInstrument("Upright Piano") {
        recipe.filter_cutoff = 5800.0; recipe.osc_mix = 0.3; recipe.envelope.decay = 0.85;
        recipe.noise_attack_burst = 0.08;
    }
};

class ElectricPiano : public KeyboardInstrument {
public:
    ElectricPiano() : KeyboardInstrument("Electric Piano") {
        recipe.use_fm = true; recipe.fm_ratio = 1.0; recipe.fm_index = 1.2; recipe.osc_mix = 0.55;
        recipe.filter_cutoff = 4200.0; recipe.envelope.decay = 1.3; recipe.stereo_width = 0.22;
    }
};

class Rhodes : public KeyboardInstrument {
public:
    Rhodes() : KeyboardInstrument("Rhodes") {
        recipe.use_fm = true; recipe.fm_ratio = 1.0; recipe.fm_index = 0.8; recipe.osc_mix = 0.6;
        recipe.filter_cutoff = 3600.0; recipe.envelope.decay = 1.6;
    }
};

class Wurlitzer : public KeyboardInstrument {
public:
    Wurlitzer() : KeyboardInstrument("Wurlitzer") {
        recipe.use_fm = true; recipe.fm_ratio = 2.0; recipe.fm_index = 1.6; recipe.osc_mix = 0.5;
        recipe.filter_cutoff = 4600.0; recipe.envelope.decay = 1.1;
    }
};

class Clavinet : public KeyboardInstrument {
public:
    Clavinet() : KeyboardInstrument("Clavinet") {
        recipe.percussive = true; recipe.osc1_shape = WaveShape::BL_Square;
        recipe.filter_cutoff = 3200.0; recipe.noise_attack_burst = 0.15;
        recipe.envelope = DAHDSR{0.0, 0.002, 0.0, 0.35, 0.0, 0.1};
    }
};

class Harpsichord : public KeyboardInstrument {
public:
    Harpsichord() : KeyboardInstrument("Harpsichord") {
        recipe.percussive = true; recipe.osc1_shape = WaveShape::BL_Sawtooth;
        recipe.filter_cutoff = 8500.0; recipe.noise_attack_burst = 0.2;
        recipe.envelope = DAHDSR{0.0, 0.001, 0.0, 0.7, 0.0, 0.12};
    }
};

class Celesta : public KeyboardInstrument {
public:
    Celesta() : KeyboardInstrument("Celesta") {
        recipe.use_fm = true; recipe.fm_ratio = 4.0; recipe.fm_index = 1.0; recipe.osc_mix = 0.7;
        recipe.filter_cutoff = 9500.0; recipe.envelope.decay = 1.4; recipe.stereo_width = 0.3;
    }
};

class ToyPiano : public KeyboardInstrument {
public:
    ToyPiano() : KeyboardInstrument("Toy Piano") {
        recipe.use_fm = true; recipe.fm_ratio = 3.5; recipe.fm_index = 1.3; recipe.osc_mix = 0.65;
        recipe.filter_cutoff = 6000.0; recipe.envelope.decay = 0.5;
    }
};

class PipeOrgan : public KeyboardInstrument {
public:
    PipeOrgan() : KeyboardInstrument("Pipe Organ") {
        recipe.percussive = false; recipe.osc1_shape = WaveShape::Sine; recipe.osc2_shape = WaveShape::Sine;
        recipe.osc2_ratio = 2.0; recipe.osc_mix = 0.5; recipe.noise_attack_burst = 0.0;
        recipe.envelope = DAHDSR{0.0, 0.05, 0.0, 0.02, 0.95, 0.3};
        recipe.filter_cutoff = 5000.0; recipe.stereo_width = 0.35;
    }
};

class ChurchOrgan : public KeyboardInstrument {
public:
    ChurchOrgan() : KeyboardInstrument("Church Organ") {
        recipe.osc1_shape = WaveShape::Sine; recipe.osc2_shape = WaveShape::Sine;
        recipe.osc2_ratio = 3.0; recipe.osc_mix = 0.45;
        recipe.envelope = DAHDSR{0.0, 0.08, 0.0, 0.02, 0.95, 0.6};
        recipe.filter_cutoff = 4200.0; recipe.stereo_width = 0.4;
    }
};

class HammondOrgan : public KeyboardInstrument {
public:
    HammondOrgan() : KeyboardInstrument("Hammond Organ") {
        recipe.osc1_shape = WaveShape::Sine; recipe.osc2_shape = WaveShape::Sine;
        recipe.osc2_ratio = 3.0; recipe.osc_mix = 0.4; recipe.detune_cents = 8.0;
        recipe.envelope = DAHDSR{0.0, 0.01, 0.0, 0.02, 0.95, 0.1};
        recipe.filter_cutoff = 6000.0; recipe.stereo_width = 0.25;
    }
};

}
}
