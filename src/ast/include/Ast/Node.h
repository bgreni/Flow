#pragma once
#include "Token/Token.h"
#include <vector>
#include "Common/Types.h"
#include "llvm/IR/Constants.h"
#include "../../../codegen/include/CodeGen/Llvm.h"

#define ACCEPT llvm::Value * accept(LLVMGenerator * v) override { \
    return v->visit(this); \
}

#define TOK_ARG const FlowToken::Token & token

struct Node {
    Node() = default;
    Node(TOK_ARG) : token(token){}
    virtual ~Node() = default;
    std::string tokenLiteral() { return token.literal; }

    virtual std::string toString() { return token.literal; }

    virtual llvm::Value * accept(LLVMGenerator * v) = 0;

    FlowToken::Token token;
};

struct Statement : Node {
    Statement(TOK_ARG) : Node(token){}

    bool isReturn() { return token.type == FlowToken::Type::RETURN; }
};
struct Expression : Node {
    Expression(TOK_ARG) : Node(token){}
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

struct Body : Node {
    Body() = default;
    Body(Body && other) noexcept
    : Node(other.token)
    , statements(std::move(other.statements))
    {}
    Body(std::vector<ptr<Statement>> && stmts) : Node(), statements(std::move(stmts)){}

    ACCEPT

    bool returns() {
        return statements.back()->isReturn();
    }

    std::string toString() override {
        std::string str = " {";

        for (const auto & stmt : statements) {
            str += "    " + stmt->toString();
        }
        str += "}";
        return str;
    }

    bool empty() const {
        return statements.empty();
    }

    ptr<Statement> & back() { return statements.back(); }

    std::vector<ptr<Statement>> statements{};
};