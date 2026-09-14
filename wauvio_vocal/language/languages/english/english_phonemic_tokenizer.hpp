#pragma once

#include "english_inventory.hpp"
#include "../../phonemic_tokenizer.hpp"

namespace wauvio::vocal::languages {
    inline std::vector<Syllable> EnglishLanguage::tokenize(const std::string& phonemicText) const {
        const std::vector<Phoneme> flat = PhonemicTokenizer::tokenizeToPhonemes(phonemicText, inventory_);

        std::vector<Syllable> result;
        size_t i = 0;
        while (i < flat.size()) {
            Syllable syl;

            while (i < flat.size() && flat[i].type != PhonemeType::Vowel) {
                syl.onset.push_back(flat[i]);
                ++i;
            }

            if (i >= flat.size()) {
                if (!result.empty()) {
                    for (auto& c : syl.onset) result.back().coda.push_back(c);
                }
                break;
            }

            syl.nucleus = flat[i];
            ++i;

            while (i < flat.size() && flat[i].type != PhonemeType::Vowel) {
                size_t j = i;
                while (j < flat.size() && flat[j].type != PhonemeType::Vowel) ++j;
                if (j >= flat.size()) {
                    syl.coda.push_back(flat[i]);
                    ++i;
                } else {
                    break;
                }
            }

            result.push_back(std::move(syl));
        }
        return result;
    }
}
