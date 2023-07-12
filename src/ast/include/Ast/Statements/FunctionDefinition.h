#pragma once
#include "Ast/Expressions/Identifier.h"
#include "Ast/Node.h"
#include "Ast/Expression.h"


struct FunctionDefinition : Statement {
    FunctionDefinition(
        const FlowToken::Token & token,
        ptr<Identifier> && name,
        ptr<Identifier> && returnType,
        FunctionArgs && args,
        Body && body
    )
    : Statement(token)
    , name(std::move(name))
    , returnType(std::move(returnType))
    , args(std::move(args))
    , body(std::move(body))
    {}

    ACCEPT

    bool isMain() {
        return name->tokenLiteral() == "main";
    }

    std::string toString() override {
        auto str = std::string("fn ") + name->toString() + "(";
        for (const auto & arg : args) {
            str += arg->toString();
            str += ", ";
        }
        if (args.size()) {
            str.pop_back();
            str.pop_back();
        }
        str += ")";
        if (returnType) {
            str += " -> " + returnType->toString();
        }
        str += body.toString();
        return str;
    }

    Body body;
    ptr<Identifier> name;
    ptr<Identifier> returnType;

    FunctionArgs args;
};