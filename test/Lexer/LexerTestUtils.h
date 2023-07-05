#pragma once
#include "Token/Token.h"
#include <gtest/gtest.h>
#include <fmt/core.h>
#include "Lexer/Lexer.h"
#include <exception>


#define SETUP(str) \
    auto t = str; \
    auto lexer = Lexer(std::move(t)); \
    const auto actual = lexer.tokenize();

#define TRY try {
#define CATCH \
} catch(const std::exception & e) { \
    FAIL() << fmt::format("Error occured during test: {}", e.what()); \
}

#define CHECK \
    EXPECT_EQ(expected.size(), actual.size()); \
    \
    for (size_t i = 0; i < expected.size(); ++i) { \
        EXPECT_TRUE(expected[i] == actual[i]) \
            << "\nToken doesn't match\n" \
            << fmt::format("expected: {}\nactual: {}\n", expected[i], actual[i]); \
    }