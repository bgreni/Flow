#pragma once
#include "Ast/Node.h"
#include "Common/Types.h"
#include "IdentifierWithType.h"

struct Lambda : Expression {
    Lambda(
        const FlowToken::Token & token,
        FunctionArgs && args,
        Body && body,
        ptr<FlowType> && returnType
    )
    : Expression(token)
    , args(std::move(args))
    , body(std::move(body))
    , returnType(std::move(returnType))
    {}
    ACCEPT

    std::string toString() override {
        auto str = std::string("fn") + "(";
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

    FunctionArgs args;
    Body body;
    ptr<FlowType> returnType;
};