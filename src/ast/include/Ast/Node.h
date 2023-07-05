#pragma once
#include "Token/Token.h"
#include <vector>
#include "Common/Types.h"

struct Node {
    Node(const FlowToken::Token & token) : token(token){}
    std::string tokenLiteral() { return token.literal; }

    virtual std::string toString() { return token.literal; }

    FlowToken::Token token;
};

struct Statement : Node {
    Statement(const FlowToken::Token & token) : Node(token){}
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