#pragma once

#include "Identifier.h"
#include "Common/Types.h"

struct FlowType : Identifier {
    FlowType(TOK_ARG) : Identifier(token){}
};

struct IntType : FlowType {
    IntType(TOK_ARG) : FlowType(token){}
};

struct VoidType : FlowType {
    VoidType() : FlowType(FlowToken::Token(FlowToken::Type::VOID, "void", 0, 0)){}
};

struct LambdaType : FlowType {
    LambdaType(TOK_ARG) : FlowType(token){}
};

