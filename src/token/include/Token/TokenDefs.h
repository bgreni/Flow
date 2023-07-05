#pragma once

#include <unordered_map>
#include <string>
#include <vector>

namespace FlowToken {

    enum Type {
        ILLEGAL,
        END,
        IDENT,
        LET,
        SEMICOLON,
        COLON,
        LBRACK,
        RBRACK,
        LPAREN,
        RPAREN,

        // literals
        INT_LIT,

        // operators
        ASSIGN,
        EQUALS,
        PLUS,
        MINUS,
        ASTERISK,
        SLASH,
        STREAMIN,
        STREAMOUT,
        GT,
        LT,
        BANG,

        // keywords
        RETURN,
        FUNCTION,
    };

    #define STRINGIFY(name) #name

    static const char * strs[] = {
        STRINGIFY(ILLEGAL),
        STRINGIFY(END),
        STRINGIFY(IDENT),
        STRINGIFY(LET),
        STRINGIFY(SEMICOLON),
        STRINGIFY(COLON),
        STRINGIFY(LBRACK),
        STRINGIFY(RBRACK),
        STRINGIFY(LPAREN),
        STRINGIFY(RPAREN),

        STRINGIFY(INT_LIT),

        STRINGIFY(ASSIGN),
        STRINGIFY(EQUALS),
        STRINGIFY(PLUS),
        STRINGIFY(MINUS),
        STRINGIFY(ASTERISK),
        STRINGIFY(SLASH),
        STRINGIFY(STREAMIN),
        STRINGIFY(STREAMOUT),
        STRINGIFY(GT),
        STRINGIFY(LT),
        STRINGIFY(BANG),

        STRINGIFY(RETURN),
        STRINGIFY(FUNCTION),
    };

    static const char* toString(Type t) {
        return strs[t];
    }
}