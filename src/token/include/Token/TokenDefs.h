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
        COMMA,
        ARROW,

        // literals
        INT_LIT,

        // operators
        ASSIGN,
        EQUALS,
        NOT_EQUALS,
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
        // types
        INT,
        VOID,
        LAMBDA,
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
        STRINGIFY(COMMA),
        STRINGIFY(ARROW),

        STRINGIFY(INT_LIT),

        STRINGIFY(ASSIGN),
        STRINGIFY(EQUALS),
        STRINGIFY(NOT_EQUALS),
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
        STRINGIFY(INT),
        STRINGIFY(VOID),
        STRINGIFY(LAMBDA),
    };

    static const char* toString(Type t) {
        return strs[t];
    }
}