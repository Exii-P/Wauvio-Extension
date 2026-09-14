#pragma once

#include "phoneme_inventory.hpp"
#include "../phonemes/syllable.hpp"
#include <string>
#include <vector>

namespace wauvio::vocal {
    class Language {
    public:
        virtual ~Language() = default;
        virtual const char* name() const noexcept = 0;
        virtual const PhonemeInventory& inventory() const noexcept = 0;
        virtual std::vector<Syllable> tokenize(const std::string& phonemicText) const = 0;
    };
}
