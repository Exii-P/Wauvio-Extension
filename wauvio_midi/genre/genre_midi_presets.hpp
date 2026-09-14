#pragma once

// =============================================================================
//  Built-in genre presets - MIDI instrumentation layer.
//
//  Each factory here wraps the matching wauvio_ext preset (genre_presets.hpp)
//  with the MIDI-only, opt-in instrumentation choices: which drum kit a
//  genre suggests for percussion parts, and which GM programs it suggests
//  substituting on melodic parts. None of this ever applies unless the
//  caller passes GenreApplyOptions::allow_instrumentation = true - see
//  genre_midi.hpp and README.md.
//
//  Instrumentation choices here are intentionally conservative: they lean
//  on instruments/kits that already exist in wauvio_ext (kits.hpp,
//  synths.hpp) rather than inventing new classes, and they generally only
//  touch generic/synth GM program ranges (leads/pads/basses, 80-103) or
//  percussion, not e.g. "replace every piano with a guitar".
// =============================================================================

#include "genre_midi.hpp"
#include "../../wauvio_ext/genre/genre_presets.hpp"
#include "../../wauvio_ext/instruments/percussion/kits.hpp"
#include "../../wauvio_ext/instruments/synthesized/synths.hpp"
#include "../../wauvio_ext/instruments/guitars/guitars.hpp"

namespace wauvio {
namespace midi {
namespace genre {
namespace presets {

inline MidiGenrePresetPtr classical() {
    auto p = std::make_shared<MidiGenrePreset>(wauvio::genre::presets::classical());
    p->percussion_kit = [] { return std::make_shared<instruments::OrchestralPercussionKit>(); };
    return p;
}

inline MidiGenrePresetPtr jazz() {
    auto p = std::make_shared<MidiGenrePreset>(wauvio::genre::presets::jazz());
    p->percussion_kit = [] { return std::make_shared<instruments::JazzKit>(); };
    return p;
}

inline MidiGenrePresetPtr rock() {
    auto p = std::make_shared<MidiGenrePreset>(wauvio::genre::presets::rock());
    p->percussion_kit = [] { return std::make_shared<instruments::RockKit>(); };
    // GM 29/30 (overdriven/distorted electric guitar) already fit; nudge
    // the plain "clean" guitar programs and synth leads toward something
    // driven, since a genre preset shouldn't silently leave a clean guitar
    // clean and call it Rock.
    p->melodic_program_overrides[26] = [] { return std::make_shared<instruments::OverdrivenElectricGuitar>(); };
    p->melodic_program_overrides[27] = [] { return std::make_shared<instruments::OverdrivenElectricGuitar>(); };
    p->melodic_program_overrides[81] = [] { return std::make_shared<instruments::SynthLead>(); };
    return p;
}

inline MidiGenrePresetPtr metal() {
    auto p = std::make_shared<MidiGenrePreset>(wauvio::genre::presets::metal());
    p->percussion_kit = [] { return std::make_shared<instruments::IndustrialKit>(); };
    p->melodic_program_overrides[26] = [] { return std::make_shared<instruments::DistortedElectricGuitar>(); };
    p->melodic_program_overrides[27] = [] { return std::make_shared<instruments::DistortedElectricGuitar>(); };
    p->melodic_program_overrides[29] = [] { return std::make_shared<instruments::DistortedElectricGuitar>(); };
    return p;
}

inline MidiGenrePresetPtr house() {
    auto p = std::make_shared<MidiGenrePreset>(wauvio::genre::presets::house());
    p->percussion_kit = [] { return std::make_shared<instruments::Kit909>(); };
    p->melodic_program_overrides[38] = [] { return std::make_shared<instruments::SynthBass>(); };
    p->melodic_program_overrides[39] = [] { return std::make_shared<instruments::SynthBass>(); };
    p->melodic_program_overrides[80] = [] { return std::make_shared<instruments::SynthLead>(); };
    p->melodic_program_overrides[89] = [] { return std::make_shared<instruments::SynthPad>(); };
    return p;
}

inline MidiGenrePresetPtr techno() {
    auto p = std::make_shared<MidiGenrePreset>(wauvio::genre::presets::techno());
    p->percussion_kit = [] { return std::make_shared<instruments::Kit808>(); };
    p->melodic_program_overrides[38] = [] { return std::make_shared<instruments::SynthBass>(); };
    p->melodic_program_overrides[39] = [] { return std::make_shared<instruments::SynthBass>(); };
    p->melodic_program_overrides[80] = [] { return std::make_shared<instruments::SynthLead>(); };
    p->melodic_program_overrides[89] = [] { return std::make_shared<instruments::AtmosphericSynth>(); };
    return p;
}

inline MidiGenrePresetPtr drum_and_bass() {
    auto p = std::make_shared<MidiGenrePreset>(wauvio::genre::presets::drum_and_bass());
    p->percussion_kit = [] { return std::make_shared<instruments::ElectronicKit>(); };
    p->melodic_program_overrides[33] = [] { return std::make_shared<instruments::SynthBass>(); };
    p->melodic_program_overrides[38] = [] { return std::make_shared<instruments::SynthBass>(); };
    p->melodic_program_overrides[39] = [] { return std::make_shared<instruments::SynthBass>(); };
    return p;
}

inline MidiGenrePresetPtr chiptune() {
    auto p = std::make_shared<MidiGenrePreset>(wauvio::genre::presets::chiptune());
    p->percussion_kit = [] { return std::make_shared<instruments::LoFiKit>(); };
    // Lean heavily on the library's own square/pulse "chip" instruments
    // for the most common melodic/bass program ranges.
    for (int prog : {0, 1, 2, 40, 41, 56, 65, 73, 80, 81, 84, 86}) // pianos, strings, sax/flute, leads
        p->melodic_program_overrides[prog] = [] { return std::make_shared<instruments::ChiptuneLead>(); };
    for (int prog : {32, 33, 34, 35, 36, 37, 38, 39}) // full GM bass family
        p->melodic_program_overrides[prog] = [] { return std::make_shared<instruments::ChiptuneBass>(); };
    return p;
}

inline MidiGenrePresetPtr ambient() {
    auto p = std::make_shared<MidiGenrePreset>(wauvio::genre::presets::ambient());
    p->melodic_program_overrides[89] = [] { return std::make_shared<instruments::AtmosphericSynth>(); };
    p->melodic_program_overrides[88] = [] { return std::make_shared<instruments::AtmosphericSynth>(); };
    p->melodic_program_overrides[95] = [] { return std::make_shared<instruments::Drone>(); };
    return p;
}

inline MidiGenrePresetPtr touhou() {
    auto p = std::make_shared<MidiGenrePreset>(wauvio::genre::presets::touhou());
    p->percussion_kit = [] { return std::make_shared<instruments::RockKit>(); };
    p->melodic_program_overrides[26] = [] { return std::make_shared<instruments::OverdrivenElectricGuitar>(); };
    p->melodic_program_overrides[81] = [] { return std::make_shared<instruments::SynthLead>(); };
    return p;
}

} // namespace presets
} // namespace genre
} // namespace midi
} // namespace wauvio
