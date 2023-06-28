#include <gtest/gtest.h>
#include "LexerTestUtils.h"

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