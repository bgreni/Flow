#pragma once
#include "Node.h"
#include "Common/Types.h"
#include <mach/processor_info.h>


struct IntegerLiteral : Expression {
    IntegerLiteral(const FlowToken::Token & token, flow_int val)
        : Expression(token)
        , value(val)
    {}
    flow_int value;
};

struct Identifier : Expression {
    Identifier(const FlowToken::Token & token)
        : Expression(token)
        , value(token.literal)
    {}

    std::string toString() override {
        return value;
    }

    std::string value;
};

struct PrefixExpression : Expression {
    PrefixExpression(
        const FlowToken::Token & token,
        ptr<Expression> && right) 
    : Expression(token)
    , right(std::move(right)) {}

    std::string toString() override {
        return "(" + tokenLiteral() + right->toString() + ")";
    }    

    ptr<Expression> right;
};

struct StreamOut : Expression {

    std::string toString() override {
        return fmt::format("({} << {})", left->toString(), right->toString());
    }

    ptr<Expression> left;
    ptr<Expression> right;
};

struct StreamIn : Expression {

    std::string toString() override {
        return fmt::format("({} >> {})", left->toString(), right->toString());
    }

    ptr<Expression> left;
    ptr<Expression> right;
};