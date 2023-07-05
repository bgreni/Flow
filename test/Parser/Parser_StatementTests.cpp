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

// TEST(CLASS, FuncDeclaration) {
//     RUN(TestCase::FUNC_DECL)
// }