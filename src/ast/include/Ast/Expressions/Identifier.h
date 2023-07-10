#pragma once
#include "Ast/Node.h"
#include "Common/Types.h"

struct Identifier : Expression {
    Identifier(const FlowToken::Token & token)
        : Expression(token)
        , value(token.literal)
    {}

    bool isVoid() { return token.type == FlowToken::Type::VOID; }

    std::string toString() override {
        return value;
    }

    ACCEPT

    std::string value;
};