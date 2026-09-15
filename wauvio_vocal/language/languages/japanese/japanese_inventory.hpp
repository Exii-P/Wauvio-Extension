#pragma once

#include "../../language.hpp"
#include "../../../phonemes/phoneme.hpp"

namespace wauvio::vocal::languages {
  class JapaneseLanguage : public Language {
  public:
    JapaneseLanguage() { buildInventory(); }

    const char* name() const noexcept override { return "Japanese (Tier 1)"; }
    const PhonemeInventory& inventory() const noexcept override { return inventory_; }

    std::vector<Syllable> tokenize(const std::string& phonemicText) const override;

  private:
    void buildInventory();
    PhonemeInventory inventory_;
  };

  inline void JapaneseLanguage::buildInventory() {
    using T = PhonemeType;

    { Phoneme p; p.symbol = "a"; p.type = T::Vowel; p.features = {Place::None, Manner::Vowel, true};
      p.formantFreq = {750.f, 1300.f, 2500.f, 3400.f, 4950.f}; p.formantBW = {80.f, 100.f, 120.f, 200.f, 200.f};
      p.minDurationMs = 110.f; inventory_.add(std::move(p)); }
    { Phoneme p; p.symbol = "i"; p.type = T::Vowel; p.features = {Place::None, Manner::Vowel, true};
      p.formantFreq = {280.f, 2300.f, 3000.f, 3400.f, 4950.f}; p.formantBW = {60.f, 90.f, 120.f, 200.f, 200.f};
      p.minDurationMs = 110.f; inventory_.add(std::move(p)); }

    { Phoneme p; p.symbol = "u"; p.type = T::Vowel; p.features = {Place::None, Manner::Vowel, true};
      p.formantFreq = {300.f, 1300.f, 2200.f, 3400.f, 4950.f}; p.formantBW = {60.f, 90.f, 120.f, 200.f, 200.f};
      p.minDurationMs = 110.f; inventory_.add(std::move(p)); }
    { Phoneme p; p.symbol = "e"; p.type = T::Vowel; p.features = {Place::None, Manner::Vowel, true};
      p.formantFreq = {500.f, 2000.f, 2500.f, 3400.f, 4950.f}; p.formantBW = {70.f, 100.f, 120.f, 200.f, 200.f};
      p.minDurationMs = 110.f; inventory_.add(std::move(p)); }
    { Phoneme p; p.symbol = "o"; p.type = T::Vowel; p.features = {Place::None, Manner::Vowel, true};
      p.formantFreq = {500.f, 900.f, 2500.f, 3400.f, 4950.f}; p.formantBW = {70.f, 90.f, 120.f, 200.f, 200.f};
      p.minDurationMs = 110.f; inventory_.add(std::move(p)); }

    { Phoneme p; p.symbol = "k"; p.type = T::Plosive; p.features = {Place::Velar, Manner::Stop, false};
      p.formantFreq = {350.f, 1900.f, 2400.f, 3400.f, 4950.f}; p.formantBW = {150.f, 200.f, 250.f, 250.f, 250.f};
      p.formantAmpParallel = {0.2f, 0.5f, 0.55f, 0.3f, 0.15f}; p.f2LocusSlope = 0.85f; p.f2LocusIntercept = 1900.f;
      p.minDurationMs = 70.f; inventory_.add(std::move(p)); }
    { Phoneme p; p.symbol = "g"; p.type = T::Plosive; p.features = {Place::Velar, Manner::Stop, true};
      p.formantFreq = {350.f, 1900.f, 2400.f, 3400.f, 4950.f}; p.formantBW = {150.f, 200.f, 250.f, 250.f, 250.f};
      p.formantAmpParallel = {0.15f, 0.4f, 0.4f, 0.2f, 0.1f}; p.f2LocusSlope = 0.85f; p.f2LocusIntercept = 1900.f;
      p.minDurationMs = 70.f; inventory_.add(std::move(p)); }
    { Phoneme p; p.symbol = "t"; p.type = T::Plosive; p.features = {Place::Alveolar, Manner::Stop, false};
      p.formantFreq = {300.f, 1700.f, 2600.f, 4000.f, 4950.f}; p.formantBW = {150.f, 200.f, 250.f, 250.f, 250.f};
      p.formantAmpParallel = {0.15f, 0.3f, 0.5f, 0.6f, 0.4f}; p.f2LocusSlope = 0.35f; p.f2LocusIntercept = 1700.f;
      p.minDurationMs = 60.f; inventory_.add(std::move(p)); }
    { Phoneme p; p.symbol = "d"; p.type = T::Plosive; p.features = {Place::Alveolar, Manner::Stop, true};
      p.formantFreq = {300.f, 1700.f, 2600.f, 4000.f, 4950.f}; p.formantBW = {150.f, 200.f, 250.f, 250.f, 250.f};
      p.formantAmpParallel = {0.15f, 0.3f, 0.45f, 0.4f, 0.25f}; p.f2LocusSlope = 0.35f; p.f2LocusIntercept = 1700.f;
      p.minDurationMs = 60.f; inventory_.add(std::move(p)); }
    { Phoneme p; p.symbol = "p"; p.type = T::Plosive; p.features = {Place::Bilabial, Manner::Stop, false};
      p.formantFreq = {200.f, 800.f, 2200.f, 3400.f, 4950.f}; p.formantBW = {150.f, 200.f, 250.f, 250.f, 250.f};
      p.formantAmpParallel = {0.3f, 0.5f, 0.4f, 0.2f, 0.1f}; p.f2LocusSlope = 0.75f; p.f2LocusIntercept = 350.f;
      p.minDurationMs = 60.f; inventory_.add(std::move(p)); }
    { Phoneme p; p.symbol = "b"; p.type = T::Plosive; p.features = {Place::Bilabial, Manner::Stop, true};
      p.formantFreq = {200.f, 800.f, 2200.f, 3400.f, 4950.f}; p.formantBW = {150.f, 200.f, 250.f, 250.f, 250.f};
      p.formantAmpParallel = {0.2f, 0.35f, 0.3f, 0.15f, 0.08f}; p.f2LocusSlope = 0.75f; p.f2LocusIntercept = 350.f;
      p.minDurationMs = 60.f; inventory_.add(std::move(p)); }

    { Phoneme p; p.symbol = "m"; p.type = T::Nasal; p.features = {Place::Bilabial, Manner::Nasal, true};
      p.formantFreq = {250.f, 1000.f, 2200.f, 3400.f, 4950.f}; p.formantBW = {100.f, 150.f, 200.f, 250.f, 250.f};
      p.f2LocusSlope = 0.75f; p.f2LocusIntercept = 350.f; p.minDurationMs = 80.f; inventory_.add(std::move(p)); }
    { Phoneme p; p.symbol = "n"; p.type = T::Nasal; p.features = {Place::Alveolar, Manner::Nasal, true};
      p.formantFreq = {280.f, 1600.f, 2600.f, 3400.f, 4950.f}; p.formantBW = {100.f, 150.f, 200.f, 250.f, 250.f};
      p.f2LocusSlope = 0.35f; p.f2LocusIntercept = 1700.f; p.minDurationMs = 80.f; inventory_.add(std::move(p)); }

    { Phoneme p; p.symbol = "N"; p.type = T::Nasal; p.features = {Place::Alveolar, Manner::Nasal, true};
      p.formantFreq = {280.f, 1600.f, 2600.f, 3400.f, 4950.f}; p.formantBW = {100.f, 150.f, 200.f, 250.f, 250.f};
      p.minDurationMs = 100.f; inventory_.add(std::move(p)); }

    { Phoneme p; p.symbol = "s"; p.type = T::Fricative; p.features = {Place::Alveolar, Manner::Fricative, false};
      p.formantFreq = {300.f, 1700.f, 2600.f, 4500.f, 6500.f}; p.formantBW = {200.f, 250.f, 250.f, 300.f, 400.f};
      p.formantAmpParallel = {0.05f, 0.1f, 0.3f, 0.9f, 0.7f}; p.minDurationMs = 100.f; inventory_.add(std::move(p)); }
    { Phoneme p; p.symbol = "z"; p.type = T::VoicedFricative; p.features = {Place::Alveolar, Manner::Fricative, true};
      p.formantFreq = {300.f, 1700.f, 2600.f, 4500.f, 6500.f}; p.formantBW = {200.f, 250.f, 250.f, 300.f, 400.f};
      p.formantAmpParallel = {0.05f, 0.1f, 0.25f, 0.6f, 0.45f}; p.minDurationMs = 90.f; inventory_.add(std::move(p)); }
    { Phoneme p; p.symbol = "h"; p.type = T::Aspirate; p.features = {Place::Glottal, Manner::Fricative, false};
      p.formantFreq = {500.f, 1500.f, 2500.f, 3500.f, 4950.f}; p.formantBW = {200.f, 250.f, 250.f, 300.f, 300.f};
      p.formantAmpParallel = {0.2f, 0.2f, 0.2f, 0.15f, 0.1f}; p.minDurationMs = 70.f; inventory_.add(std::move(p)); }

    { Phoneme p; p.symbol = "r"; p.type = T::Vowel; p.features = {Place::Alveolar, Manner::Approximant, true};
      p.formantFreq = {300.f, 1300.f, 1600.f, 3400.f, 4950.f}; p.formantBW = {100.f, 150.f, 150.f, 200.f, 200.f};
      p.f2LocusSlope = 0.4f; p.f2LocusIntercept = 1300.f; p.minDurationMs = 40.f; inventory_.add(std::move(p)); }
  }
}
