#pragma once

#include "TokenDefs.h"
#include <string>
#include <fmt/core.h>

namespace FlowToken {
    struct Token {
        Token(
            Type t,
            char c,
            unsigned int line,
            unsigned int column)
            : type(t)
            , literal({c})
            , line(line)
            , column(column)
            {}
        Token(
            Type t, 
            std::string l,
            unsigned int line,
            unsigned int column)
            : type(t)
            , literal(l)
            , line(line)
            , column(column)
            {}

        operator bool() const {
            return type != Type::END;
        }

        bool operator==(const Token & other) const {
            return type == other.type && literal == other.literal;
        }

        Type type;
        std::string literal;
        unsigned int line;
        unsigned int column;
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