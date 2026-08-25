#pragma once
#include "../model_instruments.hpp"

namespace wauvio {
namespace instruments {
using namespace wauvio::audio;

class Trumpet : public BrassInstrument {
public:
    Trumpet() : BrassInstrument("Trumpet") {
        recipe.filter_cutoff = 6800.0; recipe.formant1_hz = 1400.0; recipe.pitch_attack_cents = 45.0;
    }
};

class Flugelhorn : public BrassInstrument {
public:
    Flugelhorn() : BrassInstrument("Flugelhorn") {
        recipe.filter_cutoff = 4200.0; recipe.formant1_hz = 900.0; recipe.osc_mix = 0.2;
    }
};

class Trombone : public BrassInstrument {
public:
    Trombone() : BrassInstrument("Trombone") {
        recipe.filter_cutoff = 4600.0; recipe.formant1_hz = 700.0; recipe.pitch_attack_time = 0.06;
    }
};

class BassTrombone : public BrassInstrument {
public:
    BassTrombone() : BrassInstrument("Bass Trombone") {
        recipe.filter_cutoff = 3200.0; recipe.formant1_hz = 500.0; recipe.osc_mix = 0.4;
    }
};

class FrenchHorn : public BrassInstrument {
public:
    FrenchHorn() : BrassInstrument("French Horn") {
        recipe.filter_cutoff = 3600.0; recipe.formant1_hz = 600.0; recipe.osc1_shape = WaveShape::BL_Triangle;
        recipe.pitch_attack_cents = 20.0;
    }
};

class Tuba : public BrassInstrument {
public:
    Tuba() : BrassInstrument("Tuba") {
        recipe.filter_cutoff = 1800.0; recipe.formant1_hz = 250.0; recipe.osc_mix = 0.5;
        recipe.pitch_attack_time = 0.08;
    }
};

class Euphonium : public BrassInstrument {
public:
    Euphonium() : BrassInstrument("Euphonium") {
        recipe.filter_cutoff = 2800.0; recipe.formant1_hz = 450.0; recipe.osc_mix = 0.35;
    }
};

class Cornet : public BrassInstrument {
public:
    Cornet() : BrassInstrument("Cornet") {
        recipe.filter_cutoff = 5800.0; recipe.formant1_hz = 1200.0; recipe.pitch_attack_cents = 35.0;
    }
};

class BaritoneHorn : public BrassInstrument {
public:
    BaritoneHorn() : BrassInstrument("Baritone Horn") {
        recipe.filter_cutoff = 2600.0; recipe.formant1_hz = 400.0; recipe.osc_mix = 0.4;
    }
};

}
}
