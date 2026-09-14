#pragma once

#include "../phonemes/phoneme.hpp"
#include <string>
#include <unordered_map>
#include <optional>
#include <algorithm>

namespace wauvio::vocal {
    class PhonemeInventory {
    public:
        void add(Phoneme p) {
            maxSymbolLen_ = std::max(maxSymbolLen_, p.symbol.size());
            table_.emplace(p.symbol, std::move(p));
        }

        const Phoneme* find(const std::string& symbol) const noexcept {
            auto it = table_.find(symbol);
            return it != table_.end() ? &it->second : nullptr;
        }

        const Phoneme* matchAt(const std::string& text, size_t& pos) const noexcept {
            const size_t remaining = text.size() - pos;
            const size_t maxLen = std::min(maxSymbolLen_, remaining);
            for (size_t len = maxLen; len >= 1; --len) {
                const std::string candidate = text.substr(pos, len);
                if (const Phoneme* p = find(candidate)) {
                    pos += len;
                    return p;
                }
                if (len == 1) break;
            }
            return nullptr;
        }

        size_t size() const noexcept { return table_.size(); }

    private:
        std::unordered_map<std::string, Phoneme> table_;
        size_t maxSymbolLen_ = 1;
    };
}
