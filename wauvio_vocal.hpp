#pragma once

#include "wauvio_vocal/filter/resonator.hpp"
#include "wauvio_vocal/filter/anti_resonator.hpp"

#include "wauvio_vocal/filter/formant.hpp"
#include "wauvio_vocal/filter/formant_bank.hpp"

#include "wauvio_vocal/filter/filter_models.hpp"
#include "wauvio_vocal/filter/vocal_tract.hpp"

#include "wauvio_vocal/source/glottal_source.hpp"
#include "wauvio_vocal/source/voice_quality.hpp"
#include "wauvio_vocal/source/aspiration.hpp"
#include "wauvio_vocal/source/noise_source.hpp"
#include "wauvio_vocal/source/frication.hpp"
#include "wauvio_vocal/source/source_envelope.hpp"
#include "wauvio_vocal/source/rosenberg_model.hpp"
#include "wauvio_vocal/source/lf_model.hpp"

#include "wauvio_vocal/phonemes/phoneme_type.hpp"
#include "wauvio_vocal/phonemes/phoneme_features.hpp"
#include "wauvio_vocal/phonemes/phoneme.hpp"
#include "wauvio_vocal/phonemes/syllable.hpp"
#include "wauvio_vocal/phonemes/phoneme_timing.hpp"
#include "wauvio_vocal/phonemes/phoneme_sequence.hpp"

#include "wauvio_vocal/language/phoneme_inventory.hpp"
#include "wauvio_vocal/language/phonemic_tokenizer.hpp"
#include "wauvio_vocal/language/language.hpp"

#include "wauvio_vocal/language/languages/english/english_inventory.hpp"
#include "wauvio_vocal/language/languages/english/english_phonemic_tokenizer.hpp"

#include "wauvio_vocal/language/languages/japanese/japanese_inventory.hpp"
#include "wauvio_vocal/language/languages/japanese/japanese_phonemic_tokenizer.hpp"

#include "wauvio_vocal/utility/damped_follower.hpp"
#include "wauvio_vocal/articulation/articulation.hpp"