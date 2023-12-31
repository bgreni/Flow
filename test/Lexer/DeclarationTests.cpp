#include <gtest/gtest.h>
#include "LexerTestUtils.h"
#include "TestCommon/Declarations.h"


#define CLASS Lexer_DeclarationTest

// TEST(CLASS, LetIntLit) {
//     SETUP(TestCase::LET_INT_LIT)
//     const auto expected = FlowToken::Tokens{
//         TOK(LET, "let"),
//         TOK(IDENT, "a"),
//         TOK(ASSIGN, "="),
//         TOK(INT_LIT, "10"),
//         TOK(SEMICOLON, ';'),
//         TOK(END, "")
//     };
//     CHECK
// }

TEST(CLASS, LetIntLitWithType) {
    SETUP(TestCase::LET_INT)
    const auto expected = FlowToken::Tokens{
        TOK(LET, "let"),
        TOK(IDENT, "a"),
        TOK(COLON, ":"),
        TOK(INT, "int"),
        TOK(ASSIGN, "="),
        TOK(INT_LIT, "10"),
        TOK(SEMICOLON, ';'),
        TOK(END, "")
    };
    CHECK
}

TEST(CLASS, FuncDecl) {
    SETUP(TestCase::FUNC_DECL)
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

TEST(CLASS, FuncWithType) {
    std::string input = "fn func() -> int { return 10; }";
    SETUP(input)
    const auto expected = FlowToken::Tokens {
        TOK(FUNCTION, "fn"),
        TOK(IDENT, "func"),
        TOK(LPAREN, "("),
        TOK(RPAREN, ")"),
        TOK(ARROW, "->"),
        TOK(INT, "int"),
        TOK(LBRACK, "{"),
        TOK(RETURN, "return"),
        TOK(INT_LIT, "10"),
        TOK(SEMICOLON, ";"),
        TOK(RBRACK, "}"),
        TOK(END, "")
    };
    CHECK
}

TEST(CLASS, LambdaFuncDecl) {
    std::string input = "let fun = fn() -> int { return 10; };";
    SETUP(input)
    const auto expected = FlowToken::Tokens {
        TOK(LET, "let"),
        TOK(IDENT, "fun"),
        TOK(ASSIGN, "="),
        TOK(FUNCTION, "fn"),
        TOK(LPAREN, "("),
        TOK(RPAREN, ")"),
        TOK(ARROW, "->"),
        TOK(INT, "int"),
        TOK(LBRACK, "{"),
        TOK(RETURN, "return"),
        TOK(INT_LIT, "10"),
        TOK(SEMICOLON, ";"),
        TOK(RBRACK, "}"),
        TOK(SEMICOLON, ";"),
        TOK(END, "")
    };
    CHECK
}