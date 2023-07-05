#pragma once

#include "Parser/Parser.h"
#include <gtest/gtest.h>
#include "Ast/Statement.h"
#include "Ast/Expression.h"
#include "Ast/Node.h"

struct ParserTestCase {
    std::string input;
    std::string expected;
};

#define SETUP(test) \
    auto t = test.input; \
    auto lexer = Lexer(std::move(t)); \
    auto parser = Parser(std::move(lexer)); \
    auto actual = parser.parseProgram(); \
    EXPECT_NE(actual->statements.size(), 0) << "Failed to parse any statements";

#define CHECK \
    if (!actual) { \
        FAIL() << "Parse failed"; \
    } \
    EXPECT_EQ(actual->toString(), test.expected);

#define RUN(test) \
    try { \
        SETUP(test) \
        CHECK \
     } catch(const std::exception & e) { \
        FAIL() << fmt::format("something fucked up: {}", e.what()); \
    }
