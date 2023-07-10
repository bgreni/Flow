#include <exception>
#include <gtest/gtest.h>
#include "Ast/Expression.h"
#include "Ast/Statement.h"
#include "TestCommon/Common.h"
#include "TestCommon/Declarations.h"
#include "ParserTestUtils.h"

#define CLASS ParserExpressions

TEST(CLASS, Add) {
    ParserTestCase test {
        "return 10 + 20;",
        "return (10 + 20);"
    };
    RUN(test)
}

TEST(CLASS, AddNested) {
    ParserTestCase test {
        "return 10 + 20 + 30;",
        "return ((10 + 20) + 30);"
    };
    RUN(test)
}

TEST(CLASS, AddMinusTest) {
    ParserTestCase test {
        "let a: int = 10 - 5 + 23;",
        "let a: int = ((10 - 5) + 23);"
    };
    RUN(test)
}

TEST(CLASS, LessThanTest) {
    ParserTestCase test {
        "return a < 20;",
        "return (a < 20);"
    };
}

TEST(CLASS, GreaterThanTest) {
    ParserTestCase test {
        "return a > 20;",
        "return (a > 20);"
    };
}

TEST(CLASS, Divide) {
    ParserTestCase test {
        "return 10 / 20;",
        "return (10 / 20);"
    };
    RUN(test)
}

TEST(CLASS, Multiply) {
    ParserTestCase test {
        "return 10 * 20;",
        "return (10 * 20);"
    };
    RUN(test)
}

TEST(CLASS, PrecedenceTest) {
    ParserTestCase test {
        "return 1 + 2 * 3 - 5 / 10;",
        "return ((1 + (2 * 3)) - (5 / 10));"
    };
    RUN(test)
}

TEST(CLASS, Equals) {
    ParserTestCase test {
        "return a == b;",
        "return (a == b);"
    };
    RUN(test)
}

TEST(CLASS, FuncInvoke) {
    ParserTestCase test {
        "return print(10);",
        "return print(10);"
    };
    RUN(test)
}