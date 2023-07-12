#pragma once
#include "Ast/Node.h"
#include "Common/Types.h"
#include "FlowType.h"

struct ArrType : FlowType {
    ArrType(TOK_ARG, unsigned int count)
    : FlowType(token)
    , count(count)
    {}

    std::string toString() override {
        return fmt::format("[{}, {}]", tokenLiteral(), count);
    }
    unsigned int count;
};

struct Array : Expression {
    Array(
        TOK_ARG,
        ptr<ArrType> && type,
        std::vector<ptr<Expression>> && values)
    : Expression(token)
    , type(std::move(type))
    , values(std::move(values))
    {}

    ACCEPT

    std::string toString() override {
        std::string str = "[";
        for (const auto & val : values) {
            str += val->toString() + ", ";
        }
        str.pop_back();
        str.pop_back();
        str += "]";
        return str;
    }

//    bool isLiteralValues() {
//        for (const auto & val : values) {
//            if (type)
//        }
//    }

    ptr<ArrType> type;
    std::vector<ptr<Expression>> values;
};
