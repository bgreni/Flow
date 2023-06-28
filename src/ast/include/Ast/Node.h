#pragma once
#include "Token/Token.h"
#include <vector>

struct Node {
    Node(const FlowToken::Token & token) : token(token){}
    std::string tokenLiteral() { return token.literal; }

    virtual std::string toString() { return token.literal; }

    FlowToken::Token token;
};

struct Statement : Node{};
struct Expression : Node{};

struct Program : Node {
    std::string toString() override {
        std::string out;
        for (const auto & s : statements) {
            out += s->toString();
        }
        return out;
    }

    std::vector<std::unique_ptr<Statement>> statements;
};