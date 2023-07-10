#pragma once
#include "Ast/Node.h"
#include "Ast/Expression.h"


struct ReturnStatement : Statement {
    ReturnStatement(
        const FlowToken::Token & token,
        ptr<Expression> && value
    )
    : Statement(token)
    , value(std::move(value))
    {}

    ACCEPT

    std::string toString() override {
        return "return " + value->toString() + ";";
    }

    ptr<Expression> value;
};