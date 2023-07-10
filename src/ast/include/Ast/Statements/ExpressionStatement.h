#pragma once
#include "Ast/Node.h"

struct ExpressionStatement : Statement {
    ExpressionStatement(const FlowToken::Token & tok, ptr<Expression> && expr)
    : Statement(tok)
    , expr(std::move(expr))
    {}

    ACCEPT

    std::string toString() override {
        return expr->toString() + ";";
    }

    ptr<Expression> expr;
};