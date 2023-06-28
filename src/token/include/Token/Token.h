#pragma once

#include "TokenDefs.h"
#include <string>
#include <fmt/core.h>

namespace FlowToken {
    struct Token {
        Token(Type t, char c) {
            type = t;
            literal = {c};
        }
        Token(Type t, std::string l) {
            type = t;
            literal = l;
        }

        operator bool() const {
            return type != Type::END;
        }

        bool operator==(const Token & other) const {
            return type == other.type && literal == other.literal;
        }

        Type type;
        std::string literal;
    };

    using Tokens = std::vector<Token>;
}

template <> struct fmt::formatter<FlowToken::Token> {
    constexpr auto parse(format_parse_context & ctx) -> format_parse_context::iterator {
        return ctx.begin() + 1;
    }

    auto format(const FlowToken::Token & token, format_context & ctx) const -> format_context::iterator {
        return fmt::format_to(ctx.out(), "{} - {}", toString(token.type), token.literal);
    }
};