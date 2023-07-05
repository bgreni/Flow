#pragma once

#include "TokenDefs.h"

namespace FlowToken {
    const auto cKeywords = std::unordered_map<std::string, Type>
    {
        {"let", Type::LET},
        {"return", Type::RETURN},
        {"fn", Type::FUNCTION}
    };

    Type lookupIdent(std::string literal) {
        const auto it = cKeywords.find(literal);
        if (it != cKeywords.cend()) {
            return it->second;
        }
        return Type::IDENT;
    }

    inline bool isLetter(char c) {
        return std::isalpha(c);
    }

    inline bool isDigit(char c) {
        return std::isdigit(c);
    }
}