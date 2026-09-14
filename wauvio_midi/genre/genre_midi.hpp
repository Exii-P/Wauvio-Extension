#pragma once

// =============================================================================
//  Genre preset system - MIDI integration
//
//  Builds on wauvio_ext/genre (GenrePreset, RoleProfile, style_instrument)
//  to add the MIDI-only pieces: automatic Role classification from GM
//  program/channel, absolute-time rhythm transforms (swing, quantize,
//  humanize - only meaningful once notes have real timestamps, which
//  wauvio_ext Melody objects don't), and opt-in instrumentation
//  substitution (drum kit / GM program swaps).
//
//  See README.md ("Genre Presets") for the override-precedence rules this
//  implements, and for what was deliberately left unsupported.
// =============================================================================

#include "../track/midi_music.hpp"
#include "../../wauvio_ext/genre/genre_apply.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <unordered_map>

namespace wauvio {
namespace midi {
namespace genre {

// -----------------------------------------------------------------------
//  GenreApplyOptions
//
//  Everything here defaults to "most conservative": instrumentation is
//  off, and even when turned on, a part whose instrument came from an
//  actual Program Change in the source file is left alone unless that is
//  *also* explicitly requested. This is the "genre suggestion vs genre
//  override, and the user has to allow it" distinction from the design
//  brief.
// -----------------------------------------------------------------------
struct GenreApplyOptions {
    bool allow_instrumentation      = false;
    bool override_explicit_programs = false;
    uint64_t seed                   = 1;
};

// -----------------------------------------------------------------------
//  MidiGenrePreset
//
//  Wraps a MIDI-agnostic wauvio::genre::GenrePreset ("base") with the
//  MIDI-only instrumentation data. Kept as a separate type (rather than
//  adding GM-program fields to GenrePreset itself) so wauvio_ext/genre
//  never has to know that GM programs or InstrumentFactory exist -
//  matches the rest of the library's layering.
// -----------------------------------------------------------------------
class MidiGenrePreset {
public:
    wauvio::genre::GenrePresetPtr base;

    // Keyed by original GM program number (0-127). Only consulted when
    // GenreApplyOptions::allow_instrumentation is true.
    std::unordered_map<int, InstrumentFactory> melodic_program_overrides;

    // Whole-kit substitution for percussion parts. Only consulted when
    // GenreApplyOptions::allow_instrumentation is true.
    std::function<std::shared_ptr<audio::DrumKit>()> percussion_kit;

    explicit MidiGenrePreset(wauvio::genre::GenrePresetPtr b) : base(std::move(b)) {}

    const std::string& name() const noexcept {
        static const std::string empty;
        return base ? base->name : empty;
    }
};

using MidiGenrePresetPtr = std::shared_ptr<const MidiGenrePreset>;

// -----------------------------------------------------------------------
//  Automatic role classification.
//  GM programs 32-39 are the bass family (General MIDI spec); channel 9
//  (already flagged via MidiPart::is_percussion by the loader) is
//  percussion; everything else is treated as melodic. This is
//  deliberately a plain range check, not a growing if/switch chain - see
//  README.md for why a per-GM-instrument-family table wasn't used
//  instead (it would need constant upkeep for no real behavioral gain
//  here, since only 3 roles exist).
// -----------------------------------------------------------------------
inline wauvio::genre::Role role_for_part(const track::MidiPart& part) noexcept {
    if (part.is_percussion) return wauvio::genre::Role::Percussion;
    if (part.gm_program >= 32 && part.gm_program <= 39) return wauvio::genre::Role::Bass;
    return wauvio::genre::Role::Melodic;
}

// -----------------------------------------------------------------------
//  Beat-grid estimate.
//
//  KNOWN LIMITATION: this assumes a constant tempo (music.initial_bpm)
//  across the whole piece and a beat == quarter note. MidiMusic no longer
//  retains the full tempo map after loading (only baked seconds remain on
//  each note), so swing/quantize/accent placement will drift after a
//  tempo change in the source file, and compound-meter time signatures
//  (6/8 etc.) are treated as if the beat were a quarter note. This is an
//  intentional, documented approximation (category 2) rather than an
//  invasive change to retain+expose the full TempoMap on MidiMusic.
// -----------------------------------------------------------------------
inline double estimate_beat_seconds(const track::MidiMusic& music) noexcept {
    double bpm = music.initial_bpm > 0.0 ? music.initial_bpm : 120.0;
    return 60.0 / bpm;
}

inline int estimate_beats_per_bar(const track::MidiMusic& music) noexcept {
    if (!music.time_signatures.empty() && music.time_signatures.front().numerator > 0)
        return music.time_signatures.front().numerator;
    return 4;
}

// -----------------------------------------------------------------------
//  Per-part note transform: the "genre defaults" that always apply once a
//  genre is set (as opposed to instrumentation, which is opt-in - see
//  apply_instrumentation below). Deterministic: every random-ish decision
//  is derived from (seed, note time, note pitch, note index), never from
//  wall-clock time or global RNG state, matching the rest of the
//  library's rendering-determinism guarantee.
// -----------------------------------------------------------------------
inline void apply_role_profile(track::MidiPart& part, const wauvio::genre::RoleProfile& prof,
                                double beat_seconds, int beats_per_bar, uint64_t seed)
{
    using namespace wauvio::genre::detail;

    for (size_t i = 0; i < part.notes.size(); ++i) {
        track::ResolvedNote& n = part.notes[i];
        if (n.dur <= 0.0) continue; // already dropped/invalid

        const double orig_t = n.t;
        const int orig_note = n.midi_note;

        // 1. Density: drop eligible notes outright. Notes at forte or
        //    louder are protected so melodic peaks/accents survive.
        if (prof.thin_probability > 0.0 && n.dyn < audio::Dynamics::f) {
            double r = unit(note_hash(seed, orig_t, orig_note, i, 1));
            if (r < prof.thin_probability) { n.dur = 0.0; continue; }
        }

        // 2. Register.
        if (prof.register_shift_semitones != 0)
            n.midi_note = clampi(n.midi_note + prof.register_shift_semitones, 0, 127);

        // 3. Articulation, with duration-based fallback for instruments
        //    that don't support the requested articulation.
        bool articulation_supported = false;
        if (prof.force_articulation) {
            n.articulation = prof.articulation;
            articulation_supported = part.instrument && part.instrument->supports(prof.articulation);
        }
        if (prof.fallback_gate_ratio < 1.0 && !articulation_supported)
            n.dur = std::max(0.01, n.dur * prof.fallback_gate_ratio);

        // 4. Timing: quantize, then swing, then humanize jitter - in that
        //    order so swing/humanize offset a clean grid position rather
        //    than the source file's own micro-timing.
        double t = orig_t;
        if (prof.quantize_grid_fraction > 0.0 && beat_seconds > 0.0) {
            double grid = beat_seconds * prof.quantize_grid_fraction;
            if (grid > 0.0) t = std::round(t / grid) * grid;
        }
        if (prof.swing_amount > 0.0 && beat_seconds > 0.0) {
            double eighth = beat_seconds / 2.0;
            double pos = std::fmod(t, beat_seconds);
            if (pos < 0.0) pos += beat_seconds;
            if (pos > eighth * 0.5 && pos < eighth * 1.5) // this note IS the off-beat 8th
                t += eighth * prof.swing_amount * 0.66;    // 0.66 ~= triplet-swing feel at amount=1
        }
        if (prof.timing_humanize_sec > 0.0) {
            double r = signed_unit(note_hash(seed, orig_t, orig_note, i, 2));
            t += r * prof.timing_humanize_sec;
        }
        n.t = std::max(0.0, t);

        // 5. Dynamics: accent pattern (relative attenuation, see
        //    RoleProfile::accent_pattern) + velocity humanize, both folded
        //    into `expression`, which is already how the renderer applies
        //    continuous volume shaping on top of the discrete Dynamics
        //    marking.
        double mult = prof.velocity_scale;
        if (!prof.accent_pattern.empty() && beat_seconds > 0.0) {
            int beat_index = static_cast<int>(std::floor(orig_t / beat_seconds));
            beat_index = ((beat_index % beats_per_bar) + beats_per_bar) % beats_per_bar;
            mult *= accent_multiplier(prof.accent_pattern, beat_index);
        }
        if (prof.velocity_variance > 0.0) {
            double r = signed_unit(note_hash(seed, orig_t, orig_note, i, 3));
            mult *= (1.0 + r * prof.velocity_variance);
        }
        n.expression = clampd(n.expression * mult, prof.velocity_floor, 1.0);
    }
}

// -----------------------------------------------------------------------
//  Instrumentation substitution ("genre suggestions").
//
//  Off unless GenreApplyOptions::allow_instrumentation is true. A part is
//  additionally skipped if it's pinned, or if its instrument came from an
//  actual Program Change in the source file and
//  override_explicit_programs wasn't also set - this is the
//  "must not force a specific instrument unless the genre explicitly
//  contains an override and the user has allowed it" rule from the design
//  brief, applied per-part.
// -----------------------------------------------------------------------
inline void apply_instrumentation(track::MidiMusic& music, const MidiGenrePreset& preset,
                                   const GenreApplyOptions& opts)
{
    if (!opts.allow_instrumentation) return;

    for (auto& part : music.parts) {
        if (part.instrument_pinned) continue;
        if (part.program_from_midi_file && !opts.override_explicit_programs) continue;

        if (part.is_percussion) {
            if (preset.percussion_kit) part.drum_kit = preset.percussion_kit();
        } else {
            auto it = preset.melodic_program_overrides.find(part.gm_program);
            if (it != preset.melodic_program_overrides.end() && it->second)
                part.instrument = it->second();
        }
    }
}

// -----------------------------------------------------------------------
//  Synthesis/effects styling ("genre defaults" - always applied, never
//  gated by allow_instrumentation, since it shapes the existing
//  instrument rather than replacing it). Percussion kits are styled
//  voice-by-voice via DrumKit's read accessors.
// -----------------------------------------------------------------------
inline void style_percussion_kit(audio::DrumKit& kit, const wauvio::genre::GenrePreset& preset) {
    for (int note : kit.mapped_notes()) {
        auto instr = kit.get(note);
        if (instr) wauvio::genre::style_instrument(*instr, preset, wauvio::genre::Role::Percussion);
    }
}

} // namespace genre
} // namespace midi

namespace track {

inline void MidiMusic::setGenre(std::shared_ptr<const midi::genre::MidiGenrePreset> preset) {
    setGenre(std::move(preset), midi::genre::GenreApplyOptions());
}

inline void MidiMusic::setGenre(std::shared_ptr<const midi::genre::MidiGenrePreset> preset,
                                 const midi::genre::GenreApplyOptions& options)
{
    // Snapshot pristine state exactly once, the first time any genre is
    // ever applied. Every call after that restores from this snapshot
    // first, so applying/removing/switching genres never compounds and
    // never leaks state from a previous genre.
    if (!has_pristine_) {
        pristine_parts_ = parts;
        pristine_duration_seconds_ = duration_seconds;
        has_pristine_ = true;
    }

    parts = pristine_parts_;
    duration_seconds = pristine_duration_seconds_;
    genre_preset_ = nullptr;

    if (!preset || !preset->base) return; // setGenre(nullptr) === clearGenre()

    const wauvio::genre::GenrePreset& base = *preset->base;
    const double beat_seconds = midi::genre::estimate_beat_seconds(*this);
    const int beats_per_bar = midi::genre::estimate_beats_per_bar(*this);

    for (auto& part : parts) {
        const auto& prof = base.profile_for(midi::genre::role_for_part(part));
        midi::genre::apply_role_profile(part, prof, beat_seconds, beats_per_bar, options.seed);
    }

    midi::genre::apply_instrumentation(*this, *preset, options);

    for (auto& part : parts) {
        if (part.is_percussion) {
            if (part.drum_kit) midi::genre::style_percussion_kit(*part.drum_kit, base);
        } else if (part.instrument) {
            wauvio::genre::style_instrument(*part.instrument, base, midi::genre::role_for_part(part));
        }
    }

    if (base.tempo_scale > 0.0 && base.tempo_scale != 1.0) {
        for (auto& part : parts)
            for (auto& n : part.notes) { n.t /= base.tempo_scale; n.dur /= base.tempo_scale; }
        duration_seconds /= base.tempo_scale;
    }

    genre_preset_ = preset;
}

inline void MidiMusic::clearGenre() {
    if (has_pristine_) {
        parts = pristine_parts_;
        duration_seconds = pristine_duration_seconds_;
    }
    genre_preset_ = nullptr;
}

} // namespace track
} // namespace wauvio
