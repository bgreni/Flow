#pragma once

#include "Ast/Node.h"
#include "Common/Types.h"

struct BinaryOp : Expression {
    BinaryOp(
        const FlowToken::Token & token,
        ptr<Expression> && left,
        ptr<Expression> && right)
    : Expression(token)
    , left(std::move(left))
    , right(std::move(right))
    {}

    ACCEPT

    std::string toString() override {
        return fmt::format("({} {} {})", left->toString(), tokenLiteral(), right->toString());
    }

    ptr<Expression> left;
    ptr<Expression> right;
};