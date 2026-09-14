#pragma once

#include "japanese_inventory.hpp"
#include "../../phonemic_tokenizer.hpp"

namespace wauvio::vocal::languages {
    inline std::vector<Syllable> JapaneseLanguage::tokenize(const std::string& phonemicText) const {
        const std::vector<Phoneme> flat = PhonemicTokenizer::tokenizeToPhonemes(phonemicText, inventory_);

        std::vector<Syllable> result;
        size_t i = 0;
        while (i < flat.size()) {
            Syllable syl;

            if (flat[i].symbol == "N") {
                syl.nucleus = flat[i];
                ++i;
                result.push_back(std::move(syl));
                continue;
            }

            if (flat[i].type != PhonemeType::Vowel) {
                syl.onset.push_back(flat[i]);
                ++i;
            }

            if (i < flat.size() && flat[i].type == PhonemeType::Vowel) {
                syl.nucleus = flat[i];
                ++i;
            } else {
                Phoneme silence; silence.symbol = "_"; silence.type = PhonemeType::Silence;
                syl.nucleus = silence;
            }

            result.push_back(std::move(syl));
        }
        return result;
    }
}
