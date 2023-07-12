#include <gtest/gtest.h>
#include "LexerTestUtils.h"
#include "Token/Token.h"
#include "TestCommon/Compound.h"


#define CLASS Lexer_CompoundTests

TEST(CLASS, Declaration_and_Reassign) {
    SETUP(TestCase::DECLARATION_AND_REASSIGN)
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

TEST(CLASS, IfStmt) {
    std::string input =
        "if (a < b) {"
        "    return 10;"
        "} elif (a < c) {"
        "    return 12;"
        "} else {"
        "    return 5;"
        "}";
    SETUP(input)
    const auto expected = FlowToken::Tokens{
        TOK(IF, "if"),
        TOK(LPAREN, "("),
        TOK(IDENT, "a"),
        TOK(LT, "<"),
        TOK(IDENT, "b"),
        TOK(RPAREN, ")"),
        TOK(LBRACK, "{"),
        TOK(RETURN, "return"),
        TOK(INT_LIT, "10"),
        TOK(SEMICOLON, ";"),
        TOK(RBRACK, "}"),
        TOK(ELIF, "elif"),
        TOK(LPAREN, "("),
        TOK(IDENT, "a"),
        TOK(LT, "<"),
        TOK(IDENT, "c"),
        TOK(RPAREN, ")"),
        TOK(LBRACK, "{"),
        TOK(RETURN, "return"),
        TOK(INT_LIT, "12"),
        TOK(SEMICOLON, ";"),
        TOK(RBRACK, "}"),
        TOK(ELSE, "else"),
        TOK(LBRACK, "{"),
        TOK(RETURN, "return"),
        TOK(INT_LIT, "5"),
        TOK(SEMICOLON, ";"),
        TOK(RBRACK, "}"),
    };
}