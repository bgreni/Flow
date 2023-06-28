#include <gtest/gtest.h>
#include "LexerTestUtils.h"
#include "Token/Token.h"

#define CLASS Lexer_OperatorTests

TEST(CLASS, PlusInts) {
    std::string input = "10 + 34;";
    SETUP(input)
    const auto expected = FlowToken::Tokens{
        TOK(INT_LIT, "10"),
        TOK(PLUS, "+"),
        TOK(INT_LIT, "34"),
        TOK(SEMICOLON, ';'),
        TOK(END, "")
    };
    CHECK
}

TEST(CLASS, MinusInts) {
    std::string input = "443 - 2342;";
    SETUP(input)
    const auto expected = FlowToken::Tokens {
        TOK(INT_LIT, "443"),
        TOK(MINUS, "-"),
        TOK(INT_LIT, "2342"),
        TOK(SEMICOLON, ';'),
        TOK(END, "")
    };
    CHECK
}

TEST(CLASS, MultiplyInts) {
    std::string input = "898 * 9834;";
    SETUP(input)
    const auto expected = FlowToken::Tokens {
        TOK(INT_LIT, "898"),
        TOK(ASTERISK, "*"),
        TOK(INT_LIT, "9834"),
        TOK(SEMICOLON, ';'),
        TOK(END, "")
    };
    CHECK
}

TEST(CLASS, DivideInts) {
    std::string input = "32 / 23;";
    SETUP(input)
    const auto expected = FlowToken::Tokens {
        TOK(INT_LIT, "32"),
        TOK(SLASH, "/"),
        TOK(INT_LIT, "23"),
        TOK(SEMICOLON, ';'),
        TOK(END, "")
    };
    CHECK
}

TEST(CLASS, LtTest) {
    std::string input =
        "1 < 3;\n"
        "a << fTwo();";
    SETUP(input)
    const auto expected = FlowToken::Tokens {
        TOK(INT_LIT, "1"),
        TOK(LT, "<"),
        TOK(INT_LIT, "3"),
        TOK(SEMICOLON, ';'),
        TOK(IDENT, "a"),
        TOK(STREAMOUT, "<<"),
        TOK(IDENT, "fTwo"),
        TOK(LPAREN, "("),
        TOK(RPAREN, ")"),
        TOK(SEMICOLON, ';'),
        TOK(END, "")
    };
    CHECK
}   