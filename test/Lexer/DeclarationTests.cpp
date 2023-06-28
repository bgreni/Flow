#include <gtest/gtest.h>
#include "LexerTestUtils.h"

#define CLASS Lexer_DeclarationTest

TEST(CLASS, LetIntLit) {
    std::string input = "let a = 10;";
    SETUP(input)
    const auto expected = FlowToken::Tokens{
        TOK(LET, "let"),
        TOK(IDENT, "a"),
        TOK(ASSIGN, "="),
        TOK(INT_LIT, "10"),
        TOK(SEMICOLON, ';'),
        TOK(END, "")
    };
    CHECK
}

TEST(CLASS, FuncDecl) {
    std::string input = 
        "fn myFunc() {\n"
        "}";
    SETUP(input)
    const auto expected = FlowToken::Tokens {
        TOK(FUNCTION, "fn"),
        TOK(IDENT, "myFunc"),
        TOK(LPAREN, "("),
        TOK(RPAREN, ")"),
        TOK(LBRACK, "{"),
        TOK(RBRACK, "}"),
        TOK(END, "")
    };
    CHECK
}