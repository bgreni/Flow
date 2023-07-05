#include "Parser.h"
#include "Ast/Expression.h"
#include "Ast/Node.h"
#include "Ast/Statement.h"
#include <cstddef>
#include <memory>
#include <stdexcept>
#include "Exception.h"
#include "Common/Types.h"

using namespace FlowToken;

#define EXPECT_TOKEN(t) \
    if (!expectNext(t)) { \
        onUnexpectedToken(t, mPeekToken.type); \
    }

namespace {
    void onUnexpectedToken(Type exp, Type act) {
        throw ParserError(
            fmt::format("Expected {}. Got {}", toString(exp), toString(act))
        );
    }

    void onIdentifierParseError(const Token & tok) {
        throw ParserError(fmt::format("Failed to parse identifier: {}", tok.literal));
    }

    void onExpressionParseError(const Token & tok) {
        throw ParserError(fmt::format("Failed to parse expression near: {}", tok.literal));
    }
}

Parser::Parser(Lexer && lexer) 
    : mLexer(lexer)
    , mCurToken(mLexer.nextToken())
    , mPeekToken(mLexer.nextToken()) {
    
    mPrefixParseFns[Type::IDENT] = [this]() { return parseIdentifier(); };
    mPrefixParseFns[Type::INT_LIT] = [this]() { return parseIntegerLiteral(); };
    mPrefixParseFns[Type::BANG] = [this]() { return parsePrefixExpression(); };
}

ptr<Program> Parser::parseProgram() {
    auto program = std::make_unique<Program>();

    while (mCurToken.type != Type::END) {
        program->statements.emplace_back(parseStatement());
        nextToken();
    }
    return program;
}

ptr<Expression> Parser::parseExpression() {
    auto func = mPrefixParseFns.get(mCurToken.type);
    if (!func.has_value()) {
        return nullptr;
    }
    return func.value()();
}

ptr<Expression> Parser::parsePrefixExpression() {
    const auto tok = mCurToken;
    nextToken();
    auto right = parseExpression();
    return std::make_unique<PrefixExpression>(tok, std::move(right));
}

ptr<Identifier> Parser::parseIdentifier() {
    return std::make_unique<Identifier>(
        mCurToken
    );
}

ptr<IntegerLiteral> Parser::parseIntegerLiteral() {
    try {
        const auto val = std::stol(mCurToken.literal);
        return std::make_unique<IntegerLiteral>(mCurToken, val);
    } catch(std::invalid_argument & ia) {
        throw ParserError(fmt::format("Expected integer. Got {}", mCurToken.literal));
    } catch(std::range_error & re) {
        throw ParserError(fmt::format("Integer does not fit in 64 bits: {}", mCurToken.literal));
    }
}

ptr<Statement> Parser::parseStatement() {
    switch (mCurToken.type) {
        case Type::LET:
            return parseLetStatement();
        case Type::RETURN:
            return parseReturnStatement();
        default:
            throw ParserError(fmt::format("Cannot parse statement starting with: {}\nOf type: {}", mCurToken.literal, toString(mCurToken.type)));
    }
}

ptr<ReturnStatement> Parser::parseReturnStatement() {
    const auto tok = mCurToken;
    nextToken();
    auto value = parseExpression();
    if (!value) {
        onExpressionParseError(mCurToken);
    }
    EXPECT_TOKEN(Type::SEMICOLON);
    return std::make_unique<ReturnStatement>(tok, std::move(value));
}

ptr<LetStatement> Parser::parseLetStatement() {
    const auto tok = mCurToken;
    EXPECT_TOKEN(Type::IDENT)
    auto name = parseIdentifier();

    if (!name) {
        onIdentifierParseError(mCurToken);
    }

    EXPECT_TOKEN(Type::COLON)
    EXPECT_TOKEN(Type::IDENT)
    auto type = parseIdentifier();

    if (!type) {
        onIdentifierParseError(mCurToken);
    }

    EXPECT_TOKEN(Type::ASSIGN)
    nextToken();
    auto value = parseExpression();
    if (!value) {
        onExpressionParseError(mCurToken);
    }
    EXPECT_TOKEN(Type::SEMICOLON);
    return std::make_unique<LetStatement>(
        tok,
        std::move(name),
        std::move(type),
        std::move(value)
    );

}