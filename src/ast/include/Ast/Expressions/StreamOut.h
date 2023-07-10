#pragma once
#include "Ast/Node.h"
#include "Common/Types.h"

struct StreamOut : Expression {

    std::string toString() override {
        return fmt::format("({} << {})", left->toString(), right->toString());
    }
    ACCEPT

    ptr<Expression> left;
    ptr<Expression> right;
};