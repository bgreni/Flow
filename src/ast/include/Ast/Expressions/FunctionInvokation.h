#pragma once
#include "Common/Types.h"
#include "Ast/Expression.h"

struct FunctionInvokation : Expression {
    FunctionInvokation(
        const FlowToken::Token & token,
        std::vector<ptr<Expression>> && args
        )
    : Expression(token)
    , args(std::move(args))
    {}

    ACCEPT

    std::string toString() override {
        std::string str = tokenLiteral() + "(";
        for (auto & arg : args) {
            str += arg->toString();
            str += ", ";
        }
        if (args.size()) {
            str.pop_back();
            str.pop_back();
        }
        str += ")";
        return str;
    }

    std::vector<ptr<Expression>> args;
};