#include <exception>
#include <gtest/gtest.h>
#include "Ast/Expression.h"
#include "Ast/Statement.h"
#include "TestCommon/Common.h"
#include "TestCommon/Declarations.h"
#include "ParserTestUtils.h"

#define CLASS ParserDeclaration

TEST(CLASS, LetInt) {
    ParserTestCase test = {
        "let a: int = 10;",
        "let a: int = 10;"
    };
    RUN(test)
}

TEST(CLASS, ReturnIdent) {
    ParserTestCase test {
        "return thing;",
        "return thing;"
    };
    RUN(test)
}

TEST(CLASS, ReturnWithBang) {
    ParserTestCase test {
        "return !a;",
        "return (!a);"
    };
    RUN(test)
}

TEST(CLASS, FuncDeclaration) {
    ParserTestCase test {
        "fn add() -> int {"
        "    let a: int = 10;"
        "    return a;"
        "}",
        "fn add() -> int {"
        "    let a: int = 10;"
        "    return a;"
        "}"
    };
    RUN(test)
}

TEST(CLASS, VoidFunc) {
    ParserTestCase test {
        "fn add() {"
        "    let a: int = 10;"
        "}",
        "fn add() -> void {"
        "    let a: int = 10;"
        "}"
    };
    RUN(test)
}

TEST(CLASS, FuncDeclarationWithArgs) {
    ParserTestCase test {
        "fn add(a: int) -> int {"
        "    return a;"
        "}",
        "fn add(a: int) -> int {"
        "    return a;"
        "}"
    };
    RUN(test)
}

TEST(CLASS, LamdaDecl) {
    ParserTestCase test {
        "let func: lambda = fn(a: int) -> int {\n"
        "    return a;\n"
        "};",
        "let func: lambda = fn(a: int) -> int {"
        "    return a;"
        "};"
    };
    RUN(test)
}

TEST(CLASS, IfStmt) {
    ParserTestCase test {
        "if (a < b) {"
        "    return 10;"
        "} elif (a < c) {"
        "    return 12;"
        "} else {"
        "    return 5;"
        "}",
        "if ((a < b)) {"
        "    return 10;"
        "} elif ((a < c)) {"
        "    return 12;"
        "} else {"
        "    return 5;"
        "}"
    };
    RUN(test)
}

TEST(CLASS, ArrayDecl) {
    ParserTestCase test {
        "let a: [int, 2] = [1, 2];",
        "let a: [int, 2] = [1, 2];"
    };
    RUN(test)
}