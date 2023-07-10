#include <gtest/gtest.h>
#include "LexerTestUtils.h"
#include "Token/Token.h"
#include "TestCommon/Operators.h"

#define CLASS Lexer_OperatorTests

TEST(CLASS, PlusInts) {
    SETUP(TestCase::ADD_INTS)
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
    SETUP(TestCase::SUB_INTS)
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
    SETUP(TestCase::MULT_INTS)
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
    SETUP(TestCase::DIV_INTS)
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
    SETUP(TestCase::LT_TEST)
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

TEST(CLASS, GtTest) {
    SETUP(TestCase::GT_TEST);
    const auto expected = FlowToken::Tokens {
        TOK(INT_LIT, "3"),
        TOK(GT, ">"),
        TOK(INT_LIT, "2"),
        TOK(SEMICOLON, ';'),
        TOK(INT_LIT, "10"),
        TOK(STREAMIN, ">>"),
        TOK(IDENT, "fOne"),
        TOK(LPAREN, "("),
        TOK(RPAREN, ")"),
        TOK(SEMICOLON, ';'),
        TOK(END, "")
    };
    CHECK
}

TEST(CLASS, BangTest) {
    SETUP(TestCase::BANG_TEST)
    const auto expected = FlowToken::Tokens {
        TOK(BANG, "!"),
        TOK(IDENT, "condition"),
        TOK(SEMICOLON, ";"),
        TOK(END, "")
    };
    CHECK
}

TEST(CLASS, Eq) {
    std::string input = "a == b;";
    SETUP(input)
    const auto expected = FlowToken::Tokens {
        TOK(IDENT, "a"),
        TOK(EQUALS, "=="),
        TOK(IDENT, "b"),
        TOK(SEMICOLON, ";"),
        TOK(END, "")
    };
    CHECK
}

TEST(CLASS, NotEq) {
    std::string input = "a != b;";
    SETUP(input)
    const auto expected = FlowToken::Tokens {
        TOK(IDENT, "a"),
        TOK(NOT_EQUALS, "!="),
        TOK(IDENT, "b"),
        TOK(SEMICOLON, ";"),
        TOK(END, "")
    };
    CHECK
}