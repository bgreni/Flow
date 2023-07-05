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