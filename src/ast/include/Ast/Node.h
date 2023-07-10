#pragma once
#include "Token/Token.h"
#include <vector>
#include "Common/Types.h"
#include "llvm/IR/Constants.h"
#include "../../../codegen/include/CodeGen/Llvm.h"

#define ACCEPT llvm::Value * accept(LLVMGenerator * v) override { \
    return v->visit(this); \
}

struct Node {
    Node(const FlowToken::Token & token) : token(token){}
    virtual ~Node() = default;
    std::string tokenLiteral() { return token.literal; }

    virtual std::string toString() { return token.literal; }

    virtual llvm::Value * accept(LLVMGenerator * v) = 0;

    FlowToken::Token token;
};

struct Statement : Node {
    Statement(const FlowToken::Token & token) : Node(token){}

    bool isReturn() { return token.type == FlowToken::Type::RETURN; }
};
struct Expression : Node {
    Expression(const FlowToken::Token & token) : Node(token){}
};

struct Program {
    std::string toString() {
        std::string out;
        for (const auto & s : statements) {
            out += s->toString();
        }
        return out;
    }

    std::vector<ptr<Statement>> statements;
};