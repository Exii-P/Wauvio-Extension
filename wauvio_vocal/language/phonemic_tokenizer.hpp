#pragma once

#include "phoneme_inventory.hpp"
#include "../phonemes/phoneme.hpp"
#include <string>
#include <vector>
#include <cctype>

namespace wauvio::vocal {
    class PhonemicTokenizer {
    public:
        static std::vector<Phoneme> tokenizeToPhonemes(const std::string& text, const PhonemeInventory& inv) {
            std::vector<Phoneme> out;
            size_t pos = 0;
            while (pos < text.size()) {
                if (std::isspace(static_cast<unsigned char>(text[pos])) || text[pos] == '-' || text[pos] == '.') {
                    ++pos;
                    continue;
                }
                if (const Phoneme* p = inv.matchAt(text, pos)) {
                    out.push_back(*p);
                } else {
                    ++pos;
                }
            }
            return out;
        }
    };
}
