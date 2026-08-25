#pragma once
#include "../model_instruments.hpp"

namespace wauvio {
namespace instruments {
using namespace wauvio::audio;

class AcousticGuitar : public PluckedStringInstrument {
public:
    AcousticGuitar() : PluckedStringInstrument("Acoustic Guitar") {
        recipe.filter_cutoff = 4500.0; recipe.envelope.decay = 0.9;
    }
};

class NylonGuitar : public PluckedStringInstrument {
public:
    NylonGuitar() : PluckedStringInstrument("Nylon Guitar") {
        recipe.filter_cutoff = 3400.0; recipe.osc1_shape = WaveShape::Triangle;
        recipe.noise_attack_burst = 0.14; recipe.envelope.decay = 1.0;
    }
};

class SteelGuitar : public PluckedStringInstrument {
public:
    SteelGuitar() : PluckedStringInstrument("Steel Guitar") {
        recipe.filter_cutoff = 6200.0; recipe.noise_attack_burst = 0.28; recipe.envelope.decay = 0.8;
    }
};

class ElectricGuitar : public PluckedStringInstrument {
public:
    ElectricGuitar() : PluckedStringInstrument("Electric Guitar") {
        recipe.filter_cutoff = 5200.0; recipe.osc1_shape = WaveShape::BL_Square; recipe.osc_mix = 0.35;
        recipe.envelope.decay = 0.6;
    }
};

class CleanElectricGuitar : public PluckedStringInstrument {
public:
    CleanElectricGuitar() : PluckedStringInstrument("Clean Electric Guitar") {
        recipe.filter_cutoff = 4000.0; recipe.envelope.decay = 0.85; recipe.noise_attack_burst = 0.1;
    }
};

class MutedElectricGuitar : public PluckedStringInstrument {
public:
    MutedElectricGuitar() : PluckedStringInstrument("Muted Electric Guitar") {
        recipe.filter_cutoff = 1800.0; recipe.envelope.decay = 0.18; recipe.noise_attack_burst = 0.3;
    }
};

class TwelveStringGuitar : public PluckedStringInstrument {
public:
    TwelveStringGuitar() : PluckedStringInstrument("12-String Guitar") {
        recipe.filter_cutoff = 5800.0; recipe.detune_cents = 12.0; recipe.stereo_width = 0.35;
        recipe.envelope.decay = 1.0;
    }
};

class ClassicalGuitar : public PluckedStringInstrument {
public:
    ClassicalGuitar() : PluckedStringInstrument("Classical Guitar") {
        recipe.filter_cutoff = 3200.0; recipe.osc1_shape = WaveShape::Triangle;
        recipe.envelope.decay = 1.1; recipe.noise_attack_burst = 0.1;
    }
};

class Harp : public PluckedStringInstrument {
public:
    Harp() : PluckedStringInstrument("Harp") {
        recipe.filter_cutoff = 8000.0; recipe.osc1_shape = WaveShape::Sine; recipe.osc_mix = 0.15;
        recipe.envelope.decay = 1.8; recipe.stereo_width = 0.3;
    }
};

class Mandolin : public PluckedStringInstrument {
public:
    Mandolin() : PluckedStringInstrument("Mandolin") {
        recipe.filter_cutoff = 7200.0; recipe.detune_cents = 10.0; recipe.envelope.decay = 0.35;
        recipe.noise_attack_burst = 0.2;
    }
};

class Ukulele : public PluckedStringInstrument {
public:
    Ukulele() : PluckedStringInstrument("Ukulele") {
        recipe.filter_cutoff = 5000.0; recipe.osc1_shape = WaveShape::Triangle;
        recipe.envelope.decay = 0.5;
    }
};

class Banjo : public PluckedStringInstrument {
public:
    Banjo() : PluckedStringInstrument("Banjo") {
        recipe.filter_cutoff = 7800.0; recipe.noise_attack_burst = 0.3; recipe.envelope.decay = 0.4;
    }
};

class Sitar : public PluckedStringInstrument {
public:
    Sitar() : PluckedStringInstrument("Sitar") {
        recipe.filter_cutoff = 6500.0; recipe.detune_cents = 16.0; recipe.osc_mix = 0.4;
        recipe.envelope.decay = 1.3; recipe.formant1_hz = 1500.0; recipe.formant1_gain_db = 4.0;
    }
};

class Koto : public PluckedStringInstrument {
public:
    Koto() : PluckedStringInstrument("Koto") {
        recipe.filter_cutoff = 5600.0; recipe.osc1_shape = WaveShape::Triangle;
        recipe.envelope.decay = 1.2; recipe.noise_attack_burst = 0.15;
    }
};

class Shamisen : public PluckedStringInstrument {
public:
    Shamisen() : PluckedStringInstrument("Shamisen") {
        recipe.filter_cutoff = 4400.0; recipe.noise_attack_burst = 0.32; recipe.envelope.decay = 0.3;
    }
};

}
}
