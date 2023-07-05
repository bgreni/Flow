#pragma once
#include "Node.h"
#include "Expression.h"
#include "fmt/core.h"


// struct ExpressionStatement : Statement {
//     std::string toString () {
//         return expression ? expression->toString() : "";
//     }

//     ptr<Expression> expression;
// };

struct ReturnStatement : Statement {
    ReturnStatement(
        const FlowToken::Token & token,
        ptr<Expression> && value
    )
    : Statement(token)
    , value(std::move(value))
    {}

    std::string toString() override {
        return "return " + value->toString() + ";";
    }

    ptr<Expression> value;
};

struct LetStatement : Statement {
    LetStatement(
        const FlowToken::Token & token,
        ptr<Identifier> && name,
        ptr<Identifier> && type,
        ptr<Expression> && value)
    : Statement(std::move(token))
    , name(std::move(name))
    , type(std::move(type))
    , value(std::move(value))
    {}

    std::string toString() override {
        return fmt::format(
            "{} {}: {} = {};",
            tokenLiteral(),
            name->toString(),
            type->toString(),
            value->toString());
    }

    ptr<Identifier> name;
    ptr<Identifier> type;
    ptr<Expression> value;
};