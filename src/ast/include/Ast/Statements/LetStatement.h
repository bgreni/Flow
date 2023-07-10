#pragma once
#include "Ast/Node.h"
#include "Ast/Expression.h"

struct LetStatement : Statement {
    LetStatement(
        const FlowToken::Token & token,
        ptr<IdentifierWithType> && name,
        ptr<Expression> && value)
    : Statement(std::move(token))
    , name(std::move(name))
    , value(std::move(value))
    {}

    ACCEPT

    std::string toString() override {
        return fmt::format(
            "{} {} = {};",
            tokenLiteral(),
            name->toString(),
            value->toString());
    }

    ptr<IdentifierWithType> name;
    ptr<Expression> value;
};