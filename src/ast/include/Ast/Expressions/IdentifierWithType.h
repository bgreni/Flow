#pragma once
// #include "Ast/Node.h"
#include "Common/Types.h"
#include "Identifier.h"
#include "FlowType.h"

struct IdentifierWithType : Identifier {
    IdentifierWithType(
        const FlowToken::Token & token,
        ptr<FlowType> && type
    )
    : Identifier(token)
    , type(std::move(type))
    {}

    std::string toString() override {
        return fmt::format("{}: {}", value, type->toString());
    }

    ptr<FlowType> type;
};

using FunctionArgs = std::vector<ptr<IdentifierWithType>>;