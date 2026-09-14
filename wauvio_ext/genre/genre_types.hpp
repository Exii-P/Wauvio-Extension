#pragma once

// =============================================================================
//  Genre preset system - core data model
//
//  This header is intentionally MIDI-agnostic: it only knows about the
//  wauvio_ext primitives (audio::Note, audio::Articulation, audio::Instrument).
//  wauvio_midi/genre/genre_midi.hpp builds on top of this to add MIDI-only
//  concepts (GM program overrides, drum kit substitution, tick/tempo timing).
//
//  See README.md ("Genre Presets") for the full design rationale and the
//  override-precedence rules.
// =============================================================================

#include "../core/core.hpp"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

namespace wauvio {
namespace genre {

// -----------------------------------------------------------------------
//  Role
//
//  A coarse musical function used to pick which RoleProfile inside a
//  GenrePreset applies. This is NOT a replacement for instruments/channels;
//  it only selects *which style knobs* to use. For MIDI-imported music the
//  role is inferred automatically (percussion channel -> Percussion, GM
//  program 32-39 -> Bass, else -> Melodic). For hand-composed wauvio_ext
//  material the caller states the role explicitly, since only the caller
//  knows which Melody is the bass line.
// -----------------------------------------------------------------------
enum class Role { Melodic, Bass, Percussion };

// -----------------------------------------------------------------------
//  RoleProfile
//
//  The actual style knobs. Every field defaults to "no-op", so a
//  default-constructed RoleProfile applied to anything changes nothing.
//  Fields are grouped by the musical category they belong to; only
//  categories that map onto something the existing engine can already do
//  are represented here (see README.md for the categories that were
//  deliberately left out, and why).
// -----------------------------------------------------------------------
struct RoleProfile {
    // --- Dynamics -------------------------------------------------------
    // Multiplies the note's existing `expression` (0..1 continuous volume
    // multiplier that already exists on audio::Note / ResolvedNote).
    // Because expression is clamped to the dynamics-derived velocity as a
    // *ceiling* elsewhere in the engine, this can only ever attenuate,
    // never boost past the original dynamic marking. Accent patterns are
    // therefore implemented as relative attenuation of the non-accented
    // notes rather than a boost of the accented ones (see accent_pattern).
    double velocity_scale    = 1.0;
    double velocity_floor    = 0.0;   // 0..1, keeps quiet notes from disappearing entirely
    double velocity_variance = 0.0;   // 0..1, deterministic per-note humanization amount

    // Per-beat relative accent multipliers, cycled by (beat index within
    // bar) % accent_pattern.size(). Internally normalized so the loudest
    // entry becomes 1.0 (see velocity_scale note above for why).
    // Empty = no accent shaping.
    std::vector<double> accent_pattern;

    // --- Rhythm / timing (MIDI-only; see genre_midi.hpp) -----------------
    // These are meaningless for wauvio_ext Melody objects, whose timing is
    // implicit/sequential rather than absolute, so apply_to_melody() below
    // ignores them. They are only consumed by the MIDI note-transform.
    double swing_amount          = 0.0; // 0..1, delays the "and" 8th note
    double timing_humanize_sec   = 0.0; // deterministic jitter, seconds
    double quantize_grid_fraction = 0.0; // 0 = off; e.g. 0.25 snaps to 16th notes

    // --- Density ----------------------------------------------------------
    // Probability [0..1] that an eligible note is dropped. Notes at
    // Dynamics::f or louder are never thinned, so accents/melody peaks
    // survive. Increasing density (inventing new notes) is out of scope,
    // see README.md.
    double thin_probability = 0.0;

    // --- Melodic ----------------------------------------------------------
    int register_shift_semitones = 0;

    // --- Articulation / sustain-staccato tendency -------------------------
    // If the target instrument supports `articulation`, it is applied
    // directly (the idiomatic, existing mechanism: articulations already
    // reshape the envelope/timbre inside render_timbre()). If the
    // instrument does NOT support it, fallback_gate_ratio (MIDI-only, see
    // above) approximates the same feel by shortening note duration.
    bool               force_articulation = false;
    audio::Articulation articulation      = audio::Articulation::Sustain;
    double             fallback_gate_ratio = 1.0; // MIDI-only fallback, 1.0 = no-op

    // --- Synthesis styling --------------------------------------------------
    // Only affects instruments that are (or derive from) ModeledInstrument,
    // via its public `recipe` (TimbreRecipe) member. Silently ignored for
    // sample-based/unrecognized instruments.
    double filter_cutoff_mult    = 1.0;
    double detune_cents_delta    = 0.0;
    double stereo_width_override = -1.0; // -1 = leave instrument's own value
    double noise_mix_delta       = 0.0;

    // --- Default effects --------------------------------------------------
    // Applied via the instrument's own reverb()/chorus()/delay()/distortion()
    // builder methods (which already exist on audio::Instrument), and only
    // if that effect is not already explicitly enabled on the instrument.
    bool  want_reverb = false;
    float reverb_room = 0.5f, reverb_wet = 0.25f, reverb_damping = 0.5f;

    bool   want_chorus = false;
    double chorus_rate_hz = 0.5, chorus_depth_ms = 1.5;
    float  chorus_wet = 0.3f;

    bool   want_delay = false;
    double delay_time_ms = 250.0;
    float  delay_feedback = 0.3f, delay_wet = 0.25f;

    bool  want_distortion = false;
    float distortion_drive = 2.0f;
};

// -----------------------------------------------------------------------
//  GenrePreset
//
//  Pure data. Deliberately has no behavior beyond profile_for() dispatch,
//  so adding a new genre is just building one of these (see
//  genre_presets.hpp) - no engine code needs to change.
// -----------------------------------------------------------------------
class GenrePreset {
public:
    std::string name;
    std::string description;

    // Uniform playback-speed multiplier (1.0 = unchanged). This is a
    // deliberately simple, honest implementation of "tempo-related
    // behavior": a linear time scale, not a tempo curve edit. See
    // README.md for why per-section tempo changes are out of scope.
    double tempo_scale = 1.0;

    RoleProfile default_profile;

    RoleProfile bass_profile;
    bool        has_bass_profile = false;

    RoleProfile percussion_profile;
    bool        has_percussion_profile = false;

    const RoleProfile& profile_for(Role role) const noexcept {
        if (role == Role::Bass && has_bass_profile) return bass_profile;
        if (role == Role::Percussion && has_percussion_profile) return percussion_profile;
        return default_profile;
    }

    GenrePreset& set_bass(RoleProfile p) { bass_profile = std::move(p); has_bass_profile = true; return *this; }
    GenrePreset& set_percussion(RoleProfile p) { percussion_profile = std::move(p); has_percussion_profile = true; return *this; }
};

using GenrePresetPtr = std::shared_ptr<const GenrePreset>;

// -----------------------------------------------------------------------
//  Deterministic per-note pseudo-randomness.
//
//  Matches the existing library convention (NoiseGenerator is seeded from
//  the note itself, not global RNG state - see model_instruments.hpp) so
//  that genre application stays reproducible: same input + same seed =
//  same output, regardless of thread count or call order.
// -----------------------------------------------------------------------
namespace detail {

inline uint64_t splitmix64(uint64_t x) noexcept {
    x += 0x9E3779B97F4A7C15ULL;
    x = (x ^ (x >> 30)) * 0xBF58476D1CE4E5B9ULL;
    x = (x ^ (x >> 27)) * 0x94D049BB133111EBULL;
    return x ^ (x >> 31);
}

// Combine a genre-application seed with per-note identity (time + pitch +
// index) into a single deterministic stream. `salt` differentiates the
// several independent random decisions made per note (thinning, velocity
// humanize, timing humanize) so they don't all move in lockstep.
inline uint64_t note_hash(uint64_t seed, double t, int midi_note, size_t index, uint64_t salt) noexcept {
    uint64_t bits;
    static_assert(sizeof(bits) == sizeof(t), "expected 64-bit double");
    std::memcpy(&bits, &t, sizeof(bits));
    uint64_t h = seed;
    h = splitmix64(h ^ bits);
    h = splitmix64(h ^ (static_cast<uint64_t>(midi_note) * 0x100000001B3ULL));
    h = splitmix64(h ^ (static_cast<uint64_t>(index) * 0xD6E8FEB86659FD93ULL));
    h = splitmix64(h ^ salt);
    return h;
}

// Map a hash to [0, 1).
inline double unit(uint64_t h) noexcept {
    return static_cast<double>(h >> 11) * (1.0 / 9007199254740992.0); // 2^53
}

// Map a hash to [-1, 1).
inline double signed_unit(uint64_t h) noexcept { return unit(h) * 2.0 - 1.0; }

inline int clampi(int v, int lo, int hi) noexcept { return std::max(lo, std::min(hi, v)); }
inline double clampd(double v, double lo, double hi) noexcept { return std::max(lo, std::min(hi, v)); }

// Normalizes an accent pattern so its loudest entry is 1.0 (see the note
// on velocity_scale/accent_pattern above for why accents can only reduce,
// not boost).
inline double accent_multiplier(const std::vector<double>& pattern, int beat_index) {
    if (pattern.empty()) return 1.0;
    double peak = *std::max_element(pattern.begin(), pattern.end());
    if (peak <= 0.0) return 1.0;
    int idx = beat_index % static_cast<int>(pattern.size());
    if (idx < 0) idx += static_cast<int>(pattern.size());
    return pattern[static_cast<size_t>(idx)] / peak;
}

} // namespace detail
} // namespace genre
} // namespace wauvio
