#pragma once
#include "../model_instruments.hpp"

namespace wauvio {
namespace instruments {
using namespace wauvio::audio;

class Kick : public PercussionInstrument {
public:
    Kick() : PercussionInstrument("Kick") {
        recipe.fixed_pitch_hz = 58.0; recipe.noise_mix = 0.08; recipe.filter_cutoff = 250.0;
        recipe.envelope = DAHDSR{0.0, 0.001, 0.0, 0.28, 0.0, 0.05};
        recipe.pitch_attack_cents = -1200.0; recipe.pitch_attack_time = 0.06;
    }
};

class Snare : public PercussionInstrument {
public:
    Snare() : PercussionInstrument("Snare") {
        recipe.fixed_pitch_hz = 180.0; recipe.noise_mix = 0.65; recipe.pink_noise = false;
        recipe.filter_cutoff = 3500.0; recipe.envelope = DAHDSR{0.0, 0.001, 0.0, 0.12, 0.0, 0.05};
    }
};

class Rimshot : public PercussionInstrument {
public:
    Rimshot() : PercussionInstrument("Rimshot") {
        recipe.fixed_pitch_hz = 900.0; recipe.noise_mix = 0.4; recipe.filter_cutoff = 5500.0;
        recipe.envelope = DAHDSR{0.0, 0.0005, 0.0, 0.05, 0.0, 0.02};
    }
};

class ClosedHiHat : public PercussionInstrument {
public:
    ClosedHiHat() : PercussionInstrument("Closed Hi-Hat") {
        recipe.fixed_pitch_hz = 3000.0; recipe.noise_mix = 0.85; recipe.pink_noise = false;
        recipe.filter_cutoff = 9000.0; recipe.envelope = DAHDSR{0.0, 0.0005, 0.0, 0.04, 0.0, 0.02};
    }
};

class OpenHiHat : public PercussionInstrument {
public:
    OpenHiHat() : PercussionInstrument("Open Hi-Hat") {
        recipe.fixed_pitch_hz = 3200.0; recipe.noise_mix = 0.85; recipe.pink_noise = false;
        recipe.filter_cutoff = 9000.0; recipe.envelope = DAHDSR{0.0, 0.001, 0.0, 0.35, 0.0, 0.1};
    }
};

class CrashCymbal : public PercussionInstrument {
public:
    CrashCymbal() : PercussionInstrument("Crash Cymbal") {
        recipe.fixed_pitch_hz = 2400.0; recipe.noise_mix = 0.8; recipe.pink_noise = false;
        recipe.filter_cutoff = 8000.0; recipe.envelope = DAHDSR{0.0, 0.002, 0.0, 1.6, 0.0, 0.5};
        recipe.stereo_width = 0.3;
    }
};

class RideCymbal : public PercussionInstrument {
public:
    RideCymbal() : PercussionInstrument("Ride Cymbal") {
        recipe.fixed_pitch_hz = 2000.0; recipe.noise_mix = 0.55; recipe.pink_noise = false;
        recipe.filter_cutoff = 7000.0; recipe.envelope = DAHDSR{0.0, 0.002, 0.0, 1.1, 0.0, 0.3};
    }
};

class RideBell : public PercussionInstrument {
public:
    RideBell() : PercussionInstrument("Ride Bell") {
        recipe.fixed_pitch_hz = 2600.0; recipe.noise_mix = 0.25; recipe.filter_cutoff = 6500.0;
        recipe.envelope = DAHDSR{0.0, 0.001, 0.0, 0.6, 0.0, 0.15};
    }
};

class Tom : public PercussionInstrument {
public:
    Tom() : PercussionInstrument("Tom") {
        recipe.fixed_pitch_hz = 150.0; recipe.noise_mix = 0.15; recipe.filter_cutoff = 1600.0;
        recipe.envelope = DAHDSR{0.0, 0.001, 0.0, 0.35, 0.0, 0.08};
    }
};

class FloorTom : public PercussionInstrument {
public:
    FloorTom() : PercussionInstrument("Floor Tom") {
        recipe.fixed_pitch_hz = 90.0; recipe.noise_mix = 0.15; recipe.filter_cutoff = 900.0;
        recipe.envelope = DAHDSR{0.0, 0.001, 0.0, 0.45, 0.0, 0.1};
    }
};

class ChinaCymbal : public PercussionInstrument {
public:
    ChinaCymbal() : PercussionInstrument("China Cymbal") {
        recipe.fixed_pitch_hz = 2800.0; recipe.noise_mix = 0.85; recipe.pink_noise = false;
        recipe.filter_cutoff = 9500.0; recipe.envelope = DAHDSR{0.0, 0.002, 0.0, 1.0, 0.0, 0.4};
    }
};

class SplashCymbal : public PercussionInstrument {
public:
    SplashCymbal() : PercussionInstrument("Splash Cymbal") {
        recipe.fixed_pitch_hz = 3400.0; recipe.noise_mix = 0.8; recipe.pink_noise = false;
        recipe.filter_cutoff = 9500.0; recipe.envelope = DAHDSR{0.0, 0.002, 0.0, 0.5, 0.0, 0.15};
    }
};

class Tambourine : public PercussionInstrument {
public:
    Tambourine() : PercussionInstrument("Tambourine") {
        recipe.fixed_pitch_hz = 4500.0; recipe.noise_mix = 0.75; recipe.pink_noise = false;
        recipe.filter_cutoff = 8000.0; recipe.envelope = DAHDSR{0.0, 0.001, 0.0, 0.2, 0.0, 0.06};
    }
};

class Shaker : public PercussionInstrument {
public:
    Shaker() : PercussionInstrument("Shaker") {
        recipe.noise_mix = 1.0; recipe.pink_noise = false; recipe.filter_cutoff = 7000.0;
        recipe.envelope = DAHDSR{0.0, 0.002, 0.0, 0.1, 0.0, 0.04};
    }
};

class Maracas : public PercussionInstrument {
public:
    Maracas() : PercussionInstrument("Maracas") {
        recipe.noise_mix = 1.0; recipe.pink_noise = false; recipe.filter_cutoff = 6500.0;
        recipe.envelope = DAHDSR{0.0, 0.001, 0.0, 0.08, 0.0, 0.03};
    }
};

class Claves : public PercussionInstrument {
public:
    Claves() : PercussionInstrument("Claves") {
        recipe.fixed_pitch_hz = 2500.0; recipe.noise_mix = 0.1; recipe.filter_cutoff = 4500.0;
        recipe.envelope = DAHDSR{0.0, 0.0005, 0.0, 0.06, 0.0, 0.02};
    }
};

class Cowbell : public PercussionInstrument {
public:
    Cowbell() : PercussionInstrument("Cowbell") {
        recipe.fixed_pitch_hz = 560.0; recipe.noise_mix = 0.05; recipe.filter_cutoff = 3000.0;
        recipe.envelope = DAHDSR{0.0, 0.001, 0.0, 0.2, 0.0, 0.05};
    }
};

class Woodblock : public PercussionInstrument {
public:
    Woodblock() : PercussionInstrument("Woodblock") {
        recipe.fixed_pitch_hz = 1400.0; recipe.noise_mix = 0.1; recipe.filter_cutoff = 4000.0;
        recipe.envelope = DAHDSR{0.0, 0.0005, 0.0, 0.05, 0.0, 0.02};
    }
};

class Castanets : public PercussionInstrument {
public:
    Castanets() : PercussionInstrument("Castanets") {
        recipe.fixed_pitch_hz = 2200.0; recipe.noise_mix = 0.3; recipe.filter_cutoff = 5500.0;
        recipe.envelope = DAHDSR{0.0, 0.0003, 0.0, 0.04, 0.0, 0.02};
    }
};

class Triangle : public PercussionInstrument {
public:
    Triangle() : PercussionInstrument("Triangle") {
        recipe.fixed_pitch_hz = 5200.0; recipe.noise_mix = 0.05; recipe.filter_cutoff = 10000.0;
        recipe.envelope = DAHDSR{0.0, 0.001, 0.0, 1.2, 0.0, 0.3};
    }
};

class Cabasa : public PercussionInstrument {
public:
    Cabasa() : PercussionInstrument("Cabasa") {
        recipe.noise_mix = 1.0; recipe.pink_noise = false; recipe.filter_cutoff = 6000.0;
        recipe.envelope = DAHDSR{0.0, 0.002, 0.0, 0.15, 0.0, 0.05};
    }
};

class Guiro : public PercussionInstrument {
public:
    Guiro() : PercussionInstrument("Guiro") {
        recipe.noise_mix = 0.9; recipe.pink_noise = false; recipe.filter_cutoff = 4500.0;
        recipe.envelope = DAHDSR{0.0, 0.002, 0.0, 0.3, 0.0, 0.06};
    }
};

class Conga : public PercussionInstrument {
public:
    Conga() : PercussionInstrument("Conga") {
        recipe.fixed_pitch_hz = 220.0; recipe.noise_mix = 0.1; recipe.filter_cutoff = 1800.0;
        recipe.envelope = DAHDSR{0.0, 0.001, 0.0, 0.25, 0.0, 0.06};
    }
};

class Bongo : public PercussionInstrument {
public:
    Bongo() : PercussionInstrument("Bongo") {
        recipe.fixed_pitch_hz = 340.0; recipe.noise_mix = 0.1; recipe.filter_cutoff = 2400.0;
        recipe.envelope = DAHDSR{0.0, 0.001, 0.0, 0.18, 0.0, 0.05};
    }
};

class Djembe : public PercussionInstrument {
public:
    Djembe() : PercussionInstrument("Djembe") {
        recipe.fixed_pitch_hz = 180.0; recipe.noise_mix = 0.2; recipe.filter_cutoff = 2200.0;
        recipe.envelope = DAHDSR{0.0, 0.001, 0.0, 0.3, 0.0, 0.08};
    }
};

class Cajon : public PercussionInstrument {
public:
    Cajon() : PercussionInstrument("Cajon") {
        recipe.fixed_pitch_hz = 100.0; recipe.noise_mix = 0.35; recipe.filter_cutoff = 3000.0;
        recipe.envelope = DAHDSR{0.0, 0.001, 0.0, 0.2, 0.0, 0.06};
    }
};

class Taiko : public PercussionInstrument {
public:
    Taiko() : PercussionInstrument("Taiko") {
        recipe.fixed_pitch_hz = 65.0; recipe.noise_mix = 0.1; recipe.filter_cutoff = 800.0;
        recipe.envelope = DAHDSR{0.0, 0.002, 0.0, 0.6, 0.0, 0.2}; recipe.stereo_width = 0.25;
    }
};

class Timpani : public MalletInstrument {
public:
    Timpani() : MalletInstrument("Timpani") {
        recipe.fm_ratio = 1.5; recipe.fm_index = 0.6; recipe.filter_cutoff = 1200.0;
        recipe.envelope.decay = 1.4; recipe.noise_attack_burst = 0.08; recipe.stereo_width = 0.2;
    }
};

}
}
