#pragma once

#include <unordered_map>
#include <optional>

namespace Flow {
    template<class T, class U>
    class Map : public std::unordered_map<T, U> {
    public:
        bool contains(T k) const;
        std::optional<U> get(T k) const {
            const auto it = this->find(k);
            if (it == this->cend()) {
                return std::nullopt;
            }
            return it->second;
        }
    };
}