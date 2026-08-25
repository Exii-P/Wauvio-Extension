#pragma once
#include "../model_instruments.hpp"

namespace wauvio {
namespace instruments {
using namespace wauvio::audio;

class SopranoSaxophone : public SaxophoneInstrument {
public:
    SopranoSaxophone() : SaxophoneInstrument("Soprano Saxophone") { recipe.filter_cutoff = 6200.0; }
};

class AltoSaxophone : public SaxophoneInstrument {
public:
    AltoSaxophone() : SaxophoneInstrument("Alto Saxophone") { recipe.filter_cutoff = 5000.0; }
};

class TenorSaxophone : public SaxophoneInstrument {
public:
    TenorSaxophone() : SaxophoneInstrument("Tenor Saxophone") { recipe.filter_cutoff = 3800.0; }
};

class BaritoneSaxophone : public SaxophoneInstrument {
public:
    BaritoneSaxophone() : SaxophoneInstrument("Baritone Saxophone") { recipe.filter_cutoff = 2600.0; }
};

}
}
