#pragma once

#include "Ast/Node.h"

struct IfStatement : Statement {
    struct Clause {
        Clause() = default;
        Clause(ptr<Expression> && cond, Body && body)
        : condition(std::move(cond))
        , body(std::move(body))
        {}

        Clause(Clause && other) noexcept
        : condition(std::move(other.condition))
        , body(std::move(other.body))
        {}

        ptr<Expression> condition;
        Body body;

        std::string toString() {
            std::string str =  condition ? " (" + condition->toString() + ")" : "";
            str += body.toString();
            return str;
        }
    };

    IfStatement(
        const FlowToken::Token & tok,
        Clause && mainBody,
        std::vector<Clause> && elifBlocks,
        Clause && elseBlock)
    : Statement(tok)
    , mainBody(std::move(mainBody))
    , elifBlocks(std::move(elifBlocks))
    , elseBlock(std::move(elseBlock))
    {}

    ACCEPT

    std::string toString() override {
        std::string str = "if";
        str += mainBody.toString();
        for (auto & bod : elifBlocks) {
            str += " elif" + bod.toString();
        }
        if (!elseBlock.body.empty()) {
            str += " else" + elseBlock.toString();
        }

        return str;
    }

    Clause mainBody;
    std::vector<Clause> elifBlocks;
    Clause elseBlock;
};
