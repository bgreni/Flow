#include <gtest/gtest.h>
#include "LexerTestUtils.h"
#include "TestCommon/Common.h"

#define CLASS Lexer_ReturnTests

TEST(CLASS, ReturnIntLit) {
    std::string input = "return 100;";
    SETUP(input)
    const auto expected  = FlowToken::Tokens{
        TOK(RETURN, "return"),
        TOK(INT_LIT, "100"),
        TOK(SEMICOLON, ';'),
        TOK(END, "")
    };
    CHECK
}

TEST(CLASS, FuncInvoke) {
    std::string input = "return func();";
    SETUP(input)
    const auto expected  = FlowToken::Tokens{
        TOK(RETURN, "return"),
        TOK(IDENT, "func"),
        TOK(LPAREN, "("),
        TOK(RPAREN, ")"),
        TOK(SEMICOLON, ';'),
        TOK(END, "")
    };
    CHECK
}