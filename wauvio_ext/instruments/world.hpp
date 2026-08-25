#pragma once
#include "../model_instruments.hpp"

namespace wauvio {
namespace instruments {
using namespace wauvio::audio;

class Erhu : public BowedStringInstrument {
public:
    Erhu() : BowedStringInstrument("Erhu") {
        recipe.filter_cutoff = 5200.0; recipe.formant1_hz = 1100.0; recipe.vibrato_depth_cents = 28.0;
        recipe.osc_mix = 0.5;
    }
};

class Guzheng : public PluckedStringInstrument {
public:
    Guzheng() : PluckedStringInstrument("Guzheng") {
        recipe.filter_cutoff = 6000.0; recipe.envelope.decay = 1.3; recipe.noise_attack_burst = 0.18;
    }
};

class Bansuri : public WoodwindInstrument {
public:
    Bansuri() : WoodwindInstrument("Bansuri") {
        recipe.filter_cutoff = 4400.0; recipe.noise_mix = 0.13; recipe.vibrato_depth_cents = 20.0;
    }
};

class Duduk : public WoodwindInstrument {
public:
    Duduk() : WoodwindInstrument("Duduk") {
        recipe.osc1_shape = WaveShape::BL_Sawtooth; recipe.osc_mix = 0.4; recipe.noise_mix = 0.05;
        recipe.formant1_hz = 800.0; recipe.formant1_gain_db = 6.0; recipe.filter_cutoff = 3000.0;
        recipe.vibrato_depth_cents = 22.0;
    }
};

class Mbira : public MalletInstrument {
public:
    Mbira() : MalletInstrument("Mbira") {
        recipe.fm_ratio = 2.5; recipe.fm_index = 0.6; recipe.filter_cutoff = 4400.0;
        recipe.envelope.decay = 1.0;
    }
};

class Balafon : public MalletInstrument {
public:
    Balafon() : MalletInstrument("Balafon") {
        recipe.fm_ratio = 3.2; recipe.fm_index = 1.0; recipe.filter_cutoff = 4000.0;
        recipe.envelope.decay = 0.8;
    }
};

class SteelPan : public MalletInstrument {
public:
    SteelPan() : MalletInstrument("Steel Pan") {
        recipe.fm_ratio = 2.9; recipe.fm_index = 1.4; recipe.filter_cutoff = 5000.0;
        recipe.envelope.decay = 1.3; recipe.stereo_width = 0.3;
    }
};

class Didgeridoo : public WorldInstrument {
public:
    Didgeridoo() : WorldInstrument("Didgeridoo") {
        recipe.osc1_shape = WaveShape::BL_Square; recipe.osc_mix = 0.5; recipe.noise_mix = 0.15;
        recipe.filter_cutoff = 900.0; recipe.envelope = DAHDSR{0.0, 0.05, 0.0, 0.05, 0.9, 0.2};
        recipe.fixed_pitch_hz = 73.4;
    }
};

class Bagpipes : public WorldInstrument {
public:
    Bagpipes() : WorldInstrument("Bagpipes") {
        recipe.osc1_shape = WaveShape::BL_Square; recipe.osc2_shape = WaveShape::BL_Sawtooth;
        recipe.osc_mix = 0.4; recipe.detune_cents = 3.0; recipe.noise_mix = 0.08;
        recipe.filter_cutoff = 4600.0; recipe.envelope = DAHDSR{0.0, 0.03, 0.0, 0.02, 0.95, 0.15};
    }
};

class Accordion : public KeyboardInstrument {
public:
    Accordion() : KeyboardInstrument("Accordion") {
        recipe.osc1_shape = WaveShape::BL_Sawtooth; recipe.osc2_shape = WaveShape::BL_Sawtooth;
        recipe.osc_mix = 0.4; recipe.detune_cents = 9.0; recipe.noise_attack_burst = 0.02;
        recipe.envelope = DAHDSR{0.0, 0.04, 0.0, 0.02, 0.9, 0.2}; recipe.filter_cutoff = 4200.0;
    }
};

class Concertina : public KeyboardInstrument {
public:
    Concertina() : KeyboardInstrument("Concertina") {
        recipe.osc1_shape = WaveShape::BL_Square; recipe.osc_mix = 0.3;
        recipe.envelope = DAHDSR{0.0, 0.03, 0.0, 0.02, 0.9, 0.15}; recipe.filter_cutoff = 4800.0;
    }
};

class Bandoneon : public KeyboardInstrument {
public:
    Bandoneon() : KeyboardInstrument("Bandoneon") {
        recipe.osc1_shape = WaveShape::BL_Sawtooth; recipe.osc_mix = 0.45; recipe.detune_cents = 11.0;
        recipe.envelope = DAHDSR{0.0, 0.05, 0.0, 0.02, 0.88, 0.25}; recipe.filter_cutoff = 3400.0;
    }
};

}
}
