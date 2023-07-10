#pragma once
#include "Ast/Node.h"
#include "Common/Types.h"

struct PrefixExpression : Expression {
    PrefixExpression(
        const FlowToken::Token & token,
        ptr<Expression> && right) 
    : Expression(token)
    , right(std::move(right)) {}

    std::string toString() override {
        return "(" + tokenLiteral() + right->toString() + ")";
    }
    ACCEPT 

    ptr<Expression> right;
};