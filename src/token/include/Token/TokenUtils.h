#pragma once

#include "TokenDefs.h"
#include <set>

namespace FlowToken {
    const auto cKeywords = std::unordered_map<std::string, Type>
    {
        {"let", Type::LET},
        {"return", Type::RETURN},
        {"fn", Type::FUNCTION},
        {"int", Type::INT},
        {"void", Type::VOID},
        {"lambda", Type::LAMBDA},
        {"if", Type::IF},
        {"elif", Type::ELIF},
        {"else", Type::ELSE},
    };

    static Token LiteralTypeToToken(Type t) {
        switch (t) {
            case Type::INT_LIT:
                return {Type::INT, "int", 0, 0};
            default:
                return {};
        }
    }

    static Type lookupIdent(std::string literal) {
        const auto it = cKeywords.find(literal);
        if (it != cKeywords.cend()) {
            return it->second;
        }
        return Type::IDENT;
    }

    inline bool isValueType(Type t) {
        static const auto types = std::set<Type>{
            Type::INT,
            Type::VOID,
            Type::LAMBDA,
        };
        return types.contains(t);
    }

    inline bool isLetter(char c) {
        return std::isalpha(c);
    }

    inline bool isDigit(char c) {
        return std::isdigit(c);
    }
}