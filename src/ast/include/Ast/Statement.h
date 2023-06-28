#pragma once
#include "Node.h"
#include "Expression.h"
#include "fmt/core.h"


struct ExpressionStatement : Statement {
    std::string toString () {
        return expression ? expression->toString() : "";
    }

    std::unique_ptr<Expression> expression;
};

struct LetStatement : Statement {
    std::string toString() override {
        return fmt::format(
            "{} {} = {};",
            tokenLiteral(),
            name.toString(),
            value ? value->toString() : "");   
    }

    Identifier name;
    std::unique_ptr<Expression> value;
};