#pragma once

#include "genre_types.hpp"
#include "../core/model_instruments.hpp"

#include <cstdint>

namespace wauvio {
namespace genre {

// -----------------------------------------------------------------------
//  apply_to_melody
//
//  Mutates `melody` in place according to `preset`'s profile for `role`.
//  This is meant to be called BEFORE the notes are handed to a Track
//  (Track only exposes append-only construction, so mutating already-added
//  entries isn't possible without changing that class - seeexplanation in
//  README.md for why this call-before-add shape was chosen instead).
//
//  Only fields that make sense for a sequential, relative-timed Melody are
//  applied here: dynamics, articulation, register, and thinning (which
//  replaces a note with a rest of the same duration rather than removing
//  it, so the overall length of the Melody is preserved). Swing,
//  quantization, and per-second timing humanization require absolute
//  timestamps and are only implemented for MIDI-imported music -- see
//  wauvio_midi/genre/genre_midi.hpp.
//
//  `instrument`, if provided, is used to check supports() before forcing
//  an articulation, exactly like the MIDI-side transform does.
//  `seed` makes thinning/humanization deterministic and reproducible.
// -----------------------------------------------------------------------
inline void apply_to_melody(audio::Melody& melody, const GenrePreset& preset, Role role,
                             const audio::Instrument* instrument = nullptr, uint64_t seed = 1)
{
    const RoleProfile& prof = preset.profile_for(role);

    for (size_t i = 0; i < melody.size(); ++i) {
        audio::Note& n = melody[i];
        if (n.is_rest()) continue;

        // Density: replace (not erase) so the Melody's total duration is unchanged.
        if (prof.thin_probability > 0.0 && n.dynamics < audio::Dynamics::f) {
            double r = detail::unit(detail::note_hash(seed, static_cast<double>(i), n.midi_note, i, 1));
            if (r < prof.thin_probability) {
                n = audio::rest(n.duration);
                continue;
            }
        }

        if (prof.register_shift_semitones != 0)
            n.midi_note = detail::clampi(n.midi_note + prof.register_shift_semitones, 0, 127);

        if (prof.force_articulation && (!instrument || instrument->supports(prof.articulation)))
            n.articulation = prof.articulation;

        double mult = prof.velocity_scale;
        if (!prof.accent_pattern.empty()) {
            // No absolute bar position is available for a plain Melody, so
            // the note's position within the Melody itself is used as a
            // best-effort proxy for "beat index".
            mult *= detail::accent_multiplier(prof.accent_pattern, static_cast<int>(i));
        }
        if (prof.velocity_variance > 0.0) {
            double r = detail::signed_unit(detail::note_hash(seed, static_cast<double>(i), n.midi_note, i, 2));
            mult *= (1.0 + r * prof.velocity_variance);
        }
        double new_expr = n.expression * mult;
        n.expression = detail::clampd(new_expr, prof.velocity_floor, 1.0);
    }
}

// -----------------------------------------------------------------------
//  style_instrument
//
//  Applies a RoleProfile's synthesis/effects knobs to an Instrument.
//  Effects are only turned on if not already explicitly enabled (so a
//  caller's own reverb()/chorus()/etc. calls always win - "preserve
//  explicit settings" applies here too, not just to MIDI channels).
//  Synthesis knobs (filter/detune/noise/stereo-width) only apply to
//  ModeledInstrument-derived instruments (checked via dynamic_cast), since
//  those are the only ones with an inspectable TimbreRecipe; sample-backed
//  instruments are left untouched.
// -----------------------------------------------------------------------
inline void style_instrument(audio::Instrument& instrument, const GenrePreset& preset, Role role) {
    const RoleProfile& prof = preset.profile_for(role);

    if (auto* modeled = dynamic_cast<audio::ModeledInstrument*>(&instrument)) {
        auto& recipe = modeled->recipe;
        recipe.filter_cutoff = std::max(60.0, recipe.filter_cutoff * prof.filter_cutoff_mult);
        recipe.detune_cents += prof.detune_cents_delta;
        recipe.noise_mix = detail::clampd(recipe.noise_mix + prof.noise_mix_delta, 0.0, 1.0);
        if (prof.stereo_width_override >= 0.0) recipe.stereo_width = prof.stereo_width_override;
    }

    if (prof.want_reverb && !instrument.reverb_on)
        instrument.reverb(prof.reverb_room, prof.reverb_wet, prof.reverb_damping);
    if (prof.want_chorus && !instrument.chorus_on)
        instrument.chorus(prof.chorus_rate_hz, prof.chorus_depth_ms, prof.chorus_wet);
    if (prof.want_delay && !instrument.delay_on)
        instrument.delay(prof.delay_time_ms, prof.delay_feedback, prof.delay_wet);
    if (prof.want_distortion && !instrument.distortion_on)
        instrument.distortion(prof.distortion_drive);
}

} // namespace genre
} // namespace wauvio
