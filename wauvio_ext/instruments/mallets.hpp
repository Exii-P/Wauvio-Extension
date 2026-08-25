#pragma once
#include "../model_instruments.hpp"

namespace wauvio {
namespace instruments {
using namespace wauvio::audio;

class Glockenspiel : public MalletInstrument {
public:
    Glockenspiel() : MalletInstrument("Glockenspiel") {
        recipe.fm_ratio = 3.99; recipe.fm_index = 1.6; recipe.filter_cutoff = 11000.0;
        recipe.envelope.decay = 1.6;
    }
};

class Vibraphone : public MalletInstrument {
public:
    Vibraphone() : MalletInstrument("Vibraphone") {
        recipe.fm_ratio = 4.0; recipe.fm_index = 0.9; recipe.filter_cutoff = 6500.0;
        recipe.vibrato_rate_hz = 4.5; recipe.vibrato_depth_cents = 10.0; recipe.vibrato_delay_sec = 0.0;
        recipe.envelope.decay = 2.2;
    }
};

class Marimba : public MalletInstrument {
public:
    Marimba() : MalletInstrument("Marimba") {
        recipe.fm_ratio = 3.4; recipe.fm_index = 1.1; recipe.filter_cutoff = 3800.0;
        recipe.envelope.decay = 0.9; recipe.noise_attack_burst = 0.05;
    }
};

class Xylophone : public MalletInstrument {
public:
    Xylophone() : MalletInstrument("Xylophone") {
        recipe.fm_ratio = 3.9; recipe.fm_index = 1.8; recipe.filter_cutoff = 9000.0;
        recipe.envelope.decay = 0.55; recipe.noise_attack_burst = 0.08;
    }
};

class TubularBells : public MalletInstrument {
public:
    TubularBells() : MalletInstrument("Tubular Bells") {
        recipe.fm_ratio = 2.4; recipe.fm_index = 2.2; recipe.filter_cutoff = 7000.0;
        recipe.envelope.decay = 3.2; recipe.stereo_width = 0.35;
    }
};

class Kalimba : public MalletInstrument {
public:
    Kalimba() : MalletInstrument("Kalimba") {
        recipe.fm_ratio = 2.0; recipe.fm_index = 0.7; recipe.filter_cutoff = 5200.0;
        recipe.envelope.decay = 1.4; recipe.noise_attack_burst = 0.12;
    }
};

class MusicBox : public MalletInstrument {
public:
    MusicBox() : MalletInstrument("Music Box") {
        recipe.fm_ratio = 5.0; recipe.fm_index = 1.0; recipe.filter_cutoff = 10000.0;
        recipe.envelope.decay = 1.1; recipe.noise_attack_burst = 0.18;
    }
};

class SteelTongueDrum : public MalletInstrument {
public:
    SteelTongueDrum() : MalletInstrument("Steel Tongue Drum") {
        recipe.fm_ratio = 2.7; recipe.fm_index = 1.0; recipe.filter_cutoff = 3200.0;
        recipe.envelope.decay = 1.8; recipe.stereo_width = 0.2;
    }
};

}
}
