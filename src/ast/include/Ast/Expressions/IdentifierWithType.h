#pragma once
// #include "Ast/Node.h"
#include "Common/Types.h"
#include "Identifier.h"

struct IdentifierWithType : Identifier {
    IdentifierWithType(
        const FlowToken::Token & token,
        std::string && type
    )
    : Identifier(token)
    , type(type)
    {}

    std::string toString() override {
        return fmt::format("{}: {}", value, type);
    }

    std::string type;
};

using FunctionArgs = std::vector<ptr<IdentifierWithType>>;