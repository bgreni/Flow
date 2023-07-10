#pragma once
#include "Ast/Node.h"
#include "Common/Types.h"

struct IntegerLiteral : Expression {
    IntegerLiteral(const FlowToken::Token & token, flow_int val)
        : Expression(token)
        , value(val)
    {}
    ACCEPT
    flow_int value;
};