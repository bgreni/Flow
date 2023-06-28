#pragma once
#include "Node.h"
#include <string>

struct PrefixEpxression : Expression {

    std::string toString() override {
        return std::string("(") + op + right->toString() + ")";
    }

    std::string op;
    std::unique_ptr<Expression> right;
};

struct IntegerLiteral : Expression {
    int64_t value;
};

struct Identifier : Expression {
    std::string toString() override {
        return value;
    }

    std::string value;
};