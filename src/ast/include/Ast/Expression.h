#pragma once
#include "Ast/Node.h"
#include "Common/Types.h"
#include "Ast/Expressions/Identifier.h"
#include "Ast/Expressions/IdentifierWithType.h"
#include "Ast/Expressions/BinaryOp.h"
#include "Ast/Expressions/IntegerLiteral.h"
#include "Ast/Expressions/Lambda.h"
#include "Ast/Expressions/StreamOut.h"
#include "Ast/Expressions/PrefixExpression.h"
#include "Ast/Expressions/FunctionInvokation.h"
#include "Ast/Expressions/Array.h"
#include "Ast/Expressions/FlowType.h"

struct StreamIn : Expression {

    std::string toString() override {
        return fmt::format("({} >> {})", left->toString(), right->toString());
    }

    ptr<Expression> left;
    ptr<Expression> right;
};