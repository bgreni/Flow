#include <exception>
#include <gtest/gtest.h>
#include "Ast/Expression.h"
#include "Ast/Statement.h"
#include "TestCommon/Common.h"
#include "TestCommon/Declarations.h"
#include "ParserTestUtils.h"

#define CLASS ParserCompound

TEST(CLASS, MainFunc) {
    ParserTestCase test {
        "fn main() {"
        "    let a: int = 10 + 2 * 7;"
        "    print(10);"
        "    return a / 15;"
        "}",
        "fn main() -> void {"
        "    let a: int = (10 + (2 * 7));"
        "    print(10);"
        "    return (a / 15);"
        "}"
    };
    RUN(test)
}