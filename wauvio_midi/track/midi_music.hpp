#pragma once

#include "../resolution/instrument_resolver.hpp"
#include "../core/midi_types.hpp"

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace wauvio {

// Forward declarations only: the genre system is implemented in
// wauvio_midi/genre/genre_midi.hpp, which is included after this file (see
// wauvio_midi.hpp). Keeping wauvio_midi/track ignorant of the genre types'
// definitions -- only their names -- means this file doesn't need to
// depend on the genre headers, matching the rest of the library's
// "nothing lower depends on what's above it" layering. See README.md
// ("Genre Presets") for the full design.
namespace midi { namespace genre {
class MidiGenrePreset;
struct GenreApplyOptions;
} }

namespace track {

struct ResolvedNote {
    double t = 0.0;
    double dur = 0.0;
    int midi_note = 60;
    audio::Dynamics dyn = audio::Dynamics::mf;
    audio::Articulation articulation = audio::Articulation::Sustain;
    double expression = 1.0;
    double pitch_bend_semitones = 0.0;
    std::vector<std::pair<double,double>> pitch_bend_curve;
    int glide_from_midi = -1;
};

class MidiPart {
public:
    std::string name;
    int source_track_index = -1;
    int channel = 0;
    int gm_program = 0;
    int bank_msb = 0;
    int bank_lsb = 0;
    bool is_percussion = false;

    // True if the source MIDI file actually sent a Program Change for this
    // channel (as opposed to the loader defaulting to/guessing program 0).
    // Used by genre instrumentation substitution to avoid unexpectedly
    // replacing an instrument the file explicitly asked for - see
    // GenreApplyOptions::override_explicit_programs in genre_midi.hpp.
    bool program_from_midi_file = false;

    // When true, genre instrumentation substitution (drum kit / program
    // swaps) skips this part entirely, regardless of GenreApplyOptions.
    // Set this on any part you've manually customized and want protected.
    bool instrument_pinned = false;

    midi::InstrumentPtr instrument;
    std::shared_ptr<audio::DrumKit> drum_kit;

    std::vector<ResolvedNote> notes;
};

class MidiMusic {
public:
    std::vector<MidiPart> parts;
    double duration_seconds = 0.0;
    double initial_bpm = 120.0;
    uint16_t ticks_per_quarter = 480;
    bool smpte_timing = false;
    std::string source_path;

    std::vector<midi::TimeSignatureEvent> time_signatures;
    std::vector<midi::KeySignatureEvent> key_signatures;

    std::shared_ptr<midi::ParsedMidiFile> raw_midi;

    size_t part_count() const noexcept { return parts.size(); }
    const MidiPart& part(size_t index) const { return parts.at(index); }
    MidiPart& part(size_t index) { return parts.at(index); }

    template <typename T>
    void set_instrument(size_t part_index) {
        if (part_index >= parts.size()) return;
        parts[part_index].instrument = std::make_shared<T>();
        parts[part_index].drum_kit.reset();
        parts[part_index].is_percussion = false;
    }

    void set_instrument(size_t part_index, midi::InstrumentPtr instr) {
        if (part_index >= parts.size()) return;
        parts[part_index].instrument = std::move(instr);
        parts[part_index].drum_kit.reset();
        parts[part_index].is_percussion = false;
    }

    void set_drum_kit(size_t part_index, std::shared_ptr<audio::DrumKit> kit) {
        if (part_index >= parts.size()) return;
        parts[part_index].drum_kit = std::move(kit);
        parts[part_index].instrument.reset();
        parts[part_index].is_percussion = true;
    }

    // Protects a part from genre instrumentation substitution (see
    // MidiPart::instrument_pinned). Purely additive/optional bookkeeping;
    // has no effect unless a genre with instrumentation is later applied
    // with instrumentation substitution allowed.
    void pin_instrument(size_t part_index) {
        if (part_index < parts.size()) parts[part_index].instrument_pinned = true;
    }
    void unpin_instrument(size_t part_index) {
        if (part_index < parts.size()) parts[part_index].instrument_pinned = false;
    }

    // ---------------------------------------------------------------
    //  Genre presets (optional, opt-in - see wauvio_midi/genre/genre_midi.hpp
    //  for the full implementation and README.md for the design). These
    //  are declared here, next to the data they operate on, but defined
    //  out-of-line in the genre header so that wauvio_midi/track does not
    //  need to depend on wauvio_midi/genre (see the forward declarations
    //  above). Calling load_midi() and never touching these leaves
    //  behavior completely unchanged.
    // ---------------------------------------------------------------
    void setGenre(std::shared_ptr<const midi::genre::MidiGenrePreset> preset);
    void setGenre(std::shared_ptr<const midi::genre::MidiGenrePreset> preset,
                  const midi::genre::GenreApplyOptions& options);
    void clearGenre();
    bool has_genre() const noexcept { return genre_preset_ != nullptr; }
    std::shared_ptr<const midi::genre::MidiGenrePreset> current_genre() const noexcept { return genre_preset_; }

    size_t total_samples(int sample_rate) const {
        return static_cast<size_t>((duration_seconds + 2.0) * sample_rate) + 1;
    }

    StereoBuffer render_part(size_t part_index, int sample_rate, size_t total_len) const {
        StereoBuffer out(total_len, 0.0f);
        const MidiPart& p = parts.at(part_index);
        for (const auto& n : p.notes) {
            if (n.dur <= 0.0) continue;

            StereoBuffer note_audio;
            if (p.is_percussion) {
                if (!p.drum_kit) continue;
                audio::PlayedNote pn = p.drum_kit->play(n.midi_note, n.dyn,
                                                         std::min(0.5, std::max(0.08, n.dur)),
                                                         sample_rate);
                note_audio = std::move(pn.audio);
            } else {
                if (!p.instrument) continue;
                audio::Note note(n.midi_note, n.dur, n.dyn, n.articulation);
                note.expression = n.expression;
                note.pitch_bend_semitones = n.pitch_bend_semitones;
                note.pitch_bend_curve = n.pitch_bend_curve;
                note.glide_from_midi = n.glide_from_midi;
                audio::PlayedNote pn = p.instrument->play(note, sample_rate);
                note_audio = std::move(pn.audio);
            }

            const size_t off = static_cast<size_t>(n.t * sample_rate);
            for (size_t i = 0; i < note_audio.size() && off + i < out.size(); ++i) {
                out.L[off + i] += note_audio.L[i];
                out.R[off + i] += note_audio.R[i];
            }
        }
        return out;
    }

    StereoBuffer render(int sample_rate = 0) const {
        if (sample_rate <= 0) sample_rate = global_config().sample_rate;
        size_t total = total_samples(sample_rate);
        StereoBuffer mix(total, 0.0f);

        for (size_t pi = 0; pi < parts.size(); ++pi) {
            StereoBuffer part_audio = render_part(pi, sample_rate, total);
            for (size_t i = 0; i < part_audio.size(); ++i) {
                mix.L[i] += part_audio.L[i];
                mix.R[i] += part_audio.R[i];
            }
        }

        normalize(mix, 0.92f);
        clamp_buffer(mix);
        return mix;
    }

private:
    std::shared_ptr<const midi::genre::MidiGenrePreset> genre_preset_;

    // Snapshot of parts as they were immediately before the first ever
    // setGenre() call ("pristine"). Every setGenre()/clearGenre() call
    // restores from this snapshot before doing anything else, so genres
    // never compound and switching genres never leaks state from the
    // previous one. Populated lazily by wauvio_midi_genre_apply_impl().
    std::vector<MidiPart> pristine_parts_;
    double pristine_duration_seconds_ = 0.0;
    bool has_pristine_ = false;
};

}
}
