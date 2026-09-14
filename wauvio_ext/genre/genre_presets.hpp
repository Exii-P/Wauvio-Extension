#pragma once

// =============================================================================
//  Built-in genre presets (ext-level parameters only).
//
//  Each preset documents, in comments right next to the numbers, what it
//  changes and why that's characteristic of the genre. The MIDI-level
//  instrumentation add-ons (drum kit swaps, GM program substitutions) for
//  the same genres live in wauvio_midi/genre/genre_midi_presets.hpp, built
//  on top of these.
//
//  These are intentionally a small set of genuinely differentiated presets
//  rather than a long list of shallow ones, per the design brief.
// =============================================================================

#include "genre_types.hpp"

namespace wauvio {
namespace genre {
namespace presets {

// ---------------------------------------------------------------------------
//  Classical
//  Subtle, expressive, mostly-unquantized human timing; downbeat-weighted
//  4/4 accents; legato phrasing where the instrument supports it; a
//  concert-hall reverb default. No genre is "more default" than this one -
//  it nudges rather than transforms.
// ---------------------------------------------------------------------------
inline GenrePresetPtr classical() {
    auto p = std::make_shared<GenrePreset>();
    p->name = "Classical";
    p->description = "Expressive human timing, legato phrasing, hall reverb.";

    RoleProfile d;
    d.velocity_variance   = 0.05;         // subtle human dynamic variation
    d.timing_humanize_sec = 0.006;        // subtle human timing (MIDI only)
    d.accent_pattern       = {1.0, 0.72, 0.86, 0.72}; // 4/4 downbeat weighting
    d.force_articulation  = true;
    d.articulation         = audio::Articulation::Legato;
    d.fallback_gate_ratio  = 0.98;        // barely-there fallback if Legato unsupported
    d.want_reverb          = true;
    d.reverb_room = 0.62f; d.reverb_wet = 0.28f; d.reverb_damping = 0.4f;
    p->default_profile = d;
    return p;
}

// ---------------------------------------------------------------------------
//  Jazz
//  Swing eighths, backbeat (2 & 4) accenting, and noticeably more
//  humanization than Classical - swing is the single most identifying
//  rhythmic trait a MIDI-level transform can add.
// ---------------------------------------------------------------------------
inline GenrePresetPtr jazz() {
    auto p = std::make_shared<GenrePreset>();
    p->name = "Jazz";
    p->description = "Swung eighths, backbeat accents, loose human timing.";

    RoleProfile d;
    d.swing_amount         = 0.58;
    d.timing_humanize_sec  = 0.012;
    d.velocity_variance    = 0.10;
    d.accent_pattern        = {0.88, 1.0, 0.82, 1.0}; // emphasize beats 2 & 4
    d.want_reverb           = true;
    d.reverb_room = 0.4f; d.reverb_wet = 0.16f;
    p->default_profile = d;

    RoleProfile perc = d;
    perc.thin_probability = 0.10; // sparser, looser ride/hihat pattern
    p->set_percussion(perc);

    return p;
}

// ---------------------------------------------------------------------------
//  Rock
//  Straight (no swing), punchy accent-forward feel, forced Marcato/Accent
//  articulation where supported, and a default overdrive on melodic/lead
//  material. Tight but not mechanical (small humanize).
// ---------------------------------------------------------------------------
inline GenrePresetPtr rock() {
    auto p = std::make_shared<GenrePreset>();
    p->name = "Rock";
    p->description = "Straight rhythm, punchy backbeat accents, driven guitars.";

    RoleProfile d;
    d.timing_humanize_sec = 0.004;
    d.accent_pattern        = {0.88, 1.0, 0.9, 1.0}; // backbeat feel
    d.force_articulation   = true;
    d.articulation          = audio::Articulation::Accent;
    d.fallback_gate_ratio   = 0.9;
    d.want_distortion       = true;
    d.distortion_drive      = 2.2f;
    p->default_profile = d;

    RoleProfile bass = d;
    bass.articulation = audio::Articulation::Marcato;
    bass.want_distortion = false; // keep low end clean under a driven guitar
    p->set_bass(bass);

    return p;
}

// ---------------------------------------------------------------------------
//  Metal
//  Rock's tighter, more extreme sibling: near-zero humanize (mechanical
//  precision), heavier distortion, a darker/tighter bass filter, and a
//  small tempo push. Deliberately does NOT attempt to fabricate
//  double-kick/blast-beat percussion patterns - that would require
//  inventing new notes, which this system does not do (see README.md).
// ---------------------------------------------------------------------------
inline GenrePresetPtr metal() {
    auto p = std::make_shared<GenrePreset>();
    p->name = "Metal";
    p->description = "Tight/mechanical timing, heavy distortion, tightened low end.";
    p->tempo_scale = 1.05;

    RoleProfile d;
    d.timing_humanize_sec  = 0.0015;      // much tighter than Rock
    d.quantize_grid_fraction = 0.25;      // mechanical precision
    d.accent_pattern         = {0.82, 1.0, 0.85, 1.0};
    d.force_articulation    = true;
    d.articulation           = audio::Articulation::Accent;
    d.fallback_gate_ratio    = 0.85;
    d.want_distortion        = true;
    d.distortion_drive       = 3.4f;
    p->default_profile = d;

    RoleProfile bass = d;
    bass.filter_cutoff_mult = 0.82; // tighter, less boomy low end under heavy guitars
    bass.want_distortion    = false;
    p->set_bass(bass);

    RoleProfile perc = d;
    perc.want_distortion = false;
    p->set_percussion(perc);

    return p;
}

// ---------------------------------------------------------------------------
//  House
//  Straight, quantized four-on-the-floor feel, brighter filter on
//  percussion/bass, and a modest tempo nudge toward house tempos. Kept
//  distinct from Techno by having a touch of shuffle instead of being
//  perfectly mechanical.
// ---------------------------------------------------------------------------
inline GenrePresetPtr house() {
    auto p = std::make_shared<GenrePreset>();
    p->name = "House";
    p->description = "Quantized four-on-the-floor groove, bright low end, light shuffle.";
    p->tempo_scale = 1.04;

    RoleProfile d;
    d.quantize_grid_fraction = 0.25;
    d.swing_amount           = 0.08; // light shuffle, not full swing
    d.filter_cutoff_mult     = 1.15;
    d.want_delay             = true;
    d.delay_time_ms = 220.0; d.delay_feedback = 0.22f; d.delay_wet = 0.16f;
    p->default_profile = d;

    RoleProfile bass = d;
    bass.filter_cutoff_mult = 1.3;
    bass.want_delay = false;
    p->set_bass(bass);

    RoleProfile perc = d;
    perc.swing_amount = 0.0; // kick/hats stay perfectly on-grid
    perc.want_delay = false;
    p->set_percussion(perc);

    return p;
}

// ---------------------------------------------------------------------------
//  Techno
//  House's harder, perfectly mechanical relative: zero swing/humanize,
//  hard quantization, darker/harder filter, and a stronger tempo push.
// ---------------------------------------------------------------------------
inline GenrePresetPtr techno() {
    auto p = std::make_shared<GenrePreset>();
    p->name = "Techno";
    p->description = "Perfectly quantized, mechanical groove, driving and dark.";
    p->tempo_scale = 1.08;

    RoleProfile d;
    d.quantize_grid_fraction = 0.25;
    d.swing_amount           = 0.0;
    d.timing_humanize_sec    = 0.0;
    d.filter_cutoff_mult     = 0.9;   // darker than House
    d.accent_pattern          = {1.0, 0.8, 1.0, 0.8};
    p->default_profile = d;

    RoleProfile bass = d;
    bass.filter_cutoff_mult = 0.75;
    p->set_bass(bass);

    RoleProfile perc = d;
    perc.thin_probability = 0.05; // occasional dropped hat for movement, still on-grid
    p->set_percussion(perc);

    return p;
}

// ---------------------------------------------------------------------------
//  Drum & Bass
//  Fast tempo push, sub-heavy bass register shift, a touch of breakbeat
//  shuffle on percussion, and darker/leaner bass filtering. Does not
//  fabricate an actual breakbeat pattern (would require inventing notes).
// ---------------------------------------------------------------------------
inline GenrePresetPtr drum_and_bass() {
    auto p = std::make_shared<GenrePreset>();
    p->name = "Drum & Bass";
    p->description = "Fast tempo, sub-register bass, shuffled breakbeat feel.";
    p->tempo_scale = 1.35;

    RoleProfile d;
    d.timing_humanize_sec = 0.003;
    p->default_profile = d;

    RoleProfile bass = d;
    bass.register_shift_semitones = -12; // one octave down: sub-bass character
    bass.filter_cutoff_mult       = 0.7;
    bass.detune_cents_delta       = -2.0; // cleaner/tighter low end
    p->set_bass(bass);

    RoleProfile perc = d;
    perc.swing_amount     = 0.18; // breakbeat shuffle
    perc.thin_probability = 0.08;
    p->set_percussion(perc);

    return p;
}

// ---------------------------------------------------------------------------
//  Chiptune
//  Bright register, hard quantization, minimal reverb/space (dry, "in the
//  chip"), and (at the MIDI level) substitution toward the library's own
//  ChiptuneLead/ChiptuneBass square/pulse-wave instruments.
// ---------------------------------------------------------------------------
inline GenrePresetPtr chiptune() {
    auto p = std::make_shared<GenrePreset>();
    p->name = "Chiptune";
    p->description = "Bright, quantized, dry 8-bit-style character.";

    RoleProfile d;
    d.register_shift_semitones = 12;    // bright, characteristic high octave
    d.quantize_grid_fraction   = 0.25;
    d.timing_humanize_sec      = 0.0;
    d.filter_cutoff_mult       = 1.2;
    p->default_profile = d;

    RoleProfile bass = d;
    bass.register_shift_semitones = 0; // keep the bass in its own register
    bass.filter_cutoff_mult       = 1.0;
    p->set_bass(bass);

    RoleProfile perc = d;
    perc.register_shift_semitones = 0;
    p->set_percussion(perc);

    return p;
}

// ---------------------------------------------------------------------------
//  Ambient
//  Slow, sparse, wide, and reverberant: thinned note density, long
//  legato/sustain, a slightly slower tempo, wide stereo image, and a long
//  reverb/delay default.
// ---------------------------------------------------------------------------
inline GenrePresetPtr ambient() {
    auto p = std::make_shared<GenrePreset>();
    p->name = "Ambient";
    p->description = "Sparse, slow, wide, and reverberant.";
    p->tempo_scale = 0.85;

    RoleProfile d;
    d.thin_probability      = 0.22;
    d.timing_humanize_sec   = 0.02;
    d.force_articulation    = true;
    d.articulation           = audio::Articulation::Legato;
    d.fallback_gate_ratio    = 1.0;
    d.stereo_width_override = 0.6;
    d.want_reverb            = true;
    d.reverb_room = 0.88f; d.reverb_wet = 0.5f; d.reverb_damping = 0.3f;
    d.want_delay              = true;
    d.delay_time_ms = 560.0; d.delay_feedback = 0.4f; d.delay_wet = 0.32f;
    p->default_profile = d;
    return p;
}

// ---------------------------------------------------------------------------
//  Touhou
//  Fan-arrange "Touhou style" is genuinely a fusion (bright melodic synths
//  and rock rhythm section, fast and driving, occasionally orchestral).
//  This preset captures what's expressible with the existing engine: a
//  brighter, faster, harder-hitting take with tight quantization and a
//  touch of drive. It deliberately does NOT attempt to fabricate the dense
//  ornamental note runs the style is famous for - that requires actual
//  composition/arranging logic, which is out of scope (see README.md).
// ---------------------------------------------------------------------------
inline GenrePresetPtr touhou() {
    auto p = std::make_shared<GenrePreset>();
    p->name = "Touhou";
    p->description = "Bright, fast, driving fusion feel (melodic+rock elements).";
    p->tempo_scale = 1.12;

    RoleProfile d;
    d.quantize_grid_fraction = 0.25;
    d.timing_humanize_sec    = 0.002;
    d.filter_cutoff_mult     = 1.25;
    d.velocity_scale         = 1.0;
    d.accent_pattern          = {1.0, 0.85, 0.95, 0.85};
    d.want_distortion         = true;
    d.distortion_drive        = 1.6f; // present but not metal-heavy
    p->default_profile = d;

    RoleProfile bass = d;
    bass.filter_cutoff_mult = 1.0;
    bass.want_distortion = true;
    bass.distortion_drive = 1.4f;
    p->set_bass(bass);

    return p;
}

} // namespace presets
} // namespace genre
} // namespace wauvio
