#include <gtest/gtest.h>
#include "LexerTestUtils.h"
#include "Token/Token.h"

#define CLASS Lexer_CompoundTests

TEST(CLASS, Declaration_and_Reassign) {
    std::string input = 
        "let a = 10;\n"
        "let b = 20;\n"
        "a = a + b;";
    SETUP(input)
    const auto expected = FlowToken::Tokens{
        TOK(LET, "let"),
        TOK(IDENT, "a"),
        TOK(ASSIGN, "="),
        TOK(INT_LIT, "10"),
        TOK(SEMICOLON, ";"),
        TOK(LET, "let"),
        TOK(IDENT, "b"),
        TOK(ASSIGN, "="),
        TOK(INT_LIT, "20"),
        TOK(SEMICOLON, ";"),
        TOK(IDENT, "a"),
        TOK(ASSIGN, "="),
        TOK(IDENT, "a"),
        TOK(PLUS, "+"),
        TOK(IDENT, "b"),
        TOK(SEMICOLON, ";"),
        TOK(END, "")
    };
    CHECK
}