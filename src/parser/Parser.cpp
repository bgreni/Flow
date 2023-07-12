#include "Parser.h"
#include "Ast/Expression.h"
#include "Ast/Expressions/Identifier.h"
#include "../codegen/include/CodeGen/Llvm.h"
#include "Ast/Node.h"
#include "Ast/Statement.h"
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include "Ast/Statements/ExpressionStatement.h"
#include "Exception.h"
#include "Common/Types.h"
#include "Token/Token.h"
#include "Token/TokenDefs.h"
#include "Token/TokenUtils.h"

using namespace FlowToken;



#define EXPECT_TOKEN(t) \
    if (!expectNext(t)) { \
        onUnexpectedToken(t, mPeekToken.type, mPeekToken.line, mPeekToken.column); \
    }

#define EXPECT_TYPE \
    if (!FlowToken::isValueType(mPeekToken.type)) { \
        throw ParserError(fmt::format( \
            "Expected type value, got {}\nline {}, column {}", \
            toString(mPeekToken.type), \
            mPeekToken.line, \
            mPeekToken.column)); \
    } \
    nextToken();

namespace {
    void onUnexpectedToken(Type exp, Type act, unsigned int line, unsigned int column) {
        throw ParserError(
            fmt::format(
                "Expected {}. Got {}\nAt line {}, column {}",
                toString(exp),
                toString(act),
                line,
                column)
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
    
    mPrefixParseFns[Type::IDENT] = [this]() -> ptr<Expression> {
        if (peekTokenIs(Type::LPAREN)) {
            return parseFunctionInvokation();
        }
        return parseIdentifier();
    };
    mPrefixParseFns[Type::INT_LIT] = [this]() { return parseIntegerLiteral(); };
    mPrefixParseFns[Type::BANG] = [this]() { return parsePrefixExpression(); };
    mPrefixParseFns[Type::FUNCTION] = [this]() { return parseLamda(); };
    mPrefixParseFns[Type::LSQR] = [this]() { return parseArray(); };

    auto parseBinOp = [this](ptr<Expression> && left) { return parseBinaryOp(std::move(left)); };
    mInfixParseFns[Type::PLUS] = parseBinOp;
    mInfixParseFns[Type::MINUS] = parseBinOp;
    mInfixParseFns[Type::SLASH] = parseBinOp;
    mInfixParseFns[Type::ASTERISK] = parseBinOp;
    mInfixParseFns[Type::EQUALS] = parseBinOp;
    mInfixParseFns[Type::LT] = parseBinOp;
    mInfixParseFns[Type::GT] = parseBinOp;
}

ptr<Program> Parser::parseProgram() {
    auto program = std::make_unique<Program>();

    while (mCurToken.type != Type::END) {
        program->statements.emplace_back(std::move(parseStatement()));
        nextToken();
    }
    return program;
}

ptr<Expression> Parser::parseExpression(OpPrecedence precedence) {
    auto func = mPrefixParseFns.get(mCurToken.type);
    if (!func.has_value()) {
        throw ParserError(
            fmt::format(
                "Could not parse expression starting with: {} of type {}\n Line {}, column {}",
                toString(mCurToken.type),
                mCurToken.literal,
                mCurToken.line,
                mCurToken.column));
    }
    auto left = func.value()();

    while (!peekTokenIs(Type::SEMICOLON) && precedence < peekPrecedence()) {
        auto infixFunc = mInfixParseFns.get(mPeekToken.type);
        if (!infixFunc.has_value()) {
            throw ParserError(
                fmt::format(
                    "Unknown operation: {}\n Line {}, column {}",
                    mPeekToken.literal,
                    mPeekToken.line,
                    mPeekToken.column));
        }
        nextToken();

        left = infixFunc.value()(std::move(left));
    }
    return left;
}

ptr<BinaryOp> Parser::parseBinaryOp(ptr<Expression> && left) {
    const auto precedence = curPrecedence();
    const auto tok = mCurToken;
    nextToken();
    auto right = parseExpression(precedence);

    return std::make_unique<BinaryOp>(
        tok,
        std::move(left),
        std::move(right)
    );
}

ptr<Expression> Parser::parsePrefixExpression() {
    const auto tok = mCurToken;
    nextToken();
    auto right = parseExpression(getPrecedence(tok.type));
    return std::make_unique<PrefixExpression>(tok, std::move(right));
}

ptr<FunctionInvokation> Parser::parseFunctionInvokation() {
    const auto tok = mCurToken;
    EXPECT_TOKEN(LPAREN)

    auto args = std::vector<ptr<Expression>>();
    if (mPeekToken.type != Type::RPAREN) {
        while(true) {
            nextToken();
            args.emplace_back(std::move(parseExpression()));
            if (mPeekToken.type == Type::RPAREN) break;
            EXPECT_TOKEN(COMMA)
        }
    }
    EXPECT_TOKEN(RPAREN)
    return std::make_unique<FunctionInvokation>(tok, std::move(args));
}

ptr<Identifier> Parser::parseIdentifier() {
    return std::make_unique<Identifier>(
        mCurToken
    );
}

ptr<FlowType> Parser::parseType() {
    if (peekTokenIs(Type::LSQR)) {
        EXPECT_TOKEN(LSQR)
        EXPECT_TYPE
        const auto tok = mCurToken;
        EXPECT_TOKEN(COMMA)
        EXPECT_TOKEN(INT_LIT)
        const auto count = parseIntegerLiteral()->value;
        EXPECT_TOKEN(RSQR)
        return std::make_unique<ArrType>(tok, count);
    } else {
        EXPECT_TYPE
        switch (mCurToken.type) {
            case Type::INT:
                return std::make_unique<IntType>(mCurToken);
            case Type::LAMBDA:
                return std::make_unique<LambdaType>(mCurToken);
        }
    }
    return nullptr;
}

ptr<IdentifierWithType> Parser::parseIdentifierWithType() {
    const auto nameToken = mCurToken;
    EXPECT_TOKEN(COLON)
    std::unique_ptr<FlowType> type{};
    type = std::move(parseType());

    return std::make_unique<IdentifierWithType>(
        nameToken,
        std::move(type)
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

ptr<Lambda> Parser::parseLamda() {
    const auto tok = mCurToken;
    EXPECT_TOKEN(LPAREN)

    auto args = FunctionArgs();
    if (mPeekToken.type != Type::RPAREN) {
        while(true) {
            EXPECT_TOKEN(IDENT)
            args.emplace_back(std::move(parseIdentifierWithType()));
            if (mPeekToken.type == Type::RPAREN) break;
            EXPECT_TOKEN(COMMA)
        }
    }
    EXPECT_TOKEN(RPAREN)
    auto type = [this]() -> ptr<FlowType> {
        if (peekTokenIs(Type::ARROW)) {
            EXPECT_TOKEN(ARROW)
            return parseType();
        }
        return std::make_unique<VoidType>();
    }();

    auto body = parseBody();

    return std::make_unique<Lambda>(tok, std::move(args), std::move(body), std::move(type));
}

ptr<Array> Parser::parseArray() {
    const auto token = mCurToken;
    auto exprs = std::vector<ptr<Expression>>();
    while (!peekTokenIs(Type::RSQR)) {
        nextToken();
        exprs.emplace_back(std::move(parseExpression()));
        if (peekTokenIs(RSQR)) break;
        EXPECT_TOKEN(COMMA)
    }
    EXPECT_TOKEN(RSQR);

    // assert all items are of the same type
    if (!std::all_of(
        exprs.cbegin(),
        exprs.cend(),
        [&exprs](const ptr<Expression> & expr) {
            return expr->token.type == exprs.front()->token.type;
        })) {
        throw ParserError("Elements of array are not all of the same type");
    }

    auto type = std::make_unique<ArrType>(LiteralTypeToToken(exprs.front()->token.type), exprs.size());
    return std::make_unique<Array>(
        token,
        std::move(type),
        std::move(exprs));
}

Body Parser::parseBody() {
    EXPECT_TOKEN(LBRACK)

    auto body = Body();
    while (mPeekToken.type != Type::RBRACK) {
        nextToken();
        body.statements.emplace_back(std::move(parseStatement()));
    }
    EXPECT_TOKEN(RBRACK)
    return body;
}

ptr<Statement> Parser::parseStatement() {
    switch (mCurToken.type) {
        case Type::LET:
            return parseLetStatement();
        case Type::RETURN:
            return parseReturnStatement();
        case Type::FUNCTION:
            return parseFunctionDefinition();
        case Type::IDENT:
            if (peekTokenIs(Type::LPAREN)) {
                return parseExpressionStatement();
            }
        case Type::IF:
            return parseIfStatement();
        default:
            throw ParserError(fmt::format("Cannot parse statement starting with: {}\nOf type: {}", mCurToken.literal, toString(mCurToken.type)));
    }
}

ptr<ExpressionStatement> Parser::parseExpressionStatement() {
    const auto tok = mCurToken;
    auto expr = parseExpression();
    EXPECT_TOKEN(SEMICOLON)
    return std::make_unique<ExpressionStatement>(tok, std::move(expr));
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
    auto name = parseIdentifierWithType();

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
        std::move(value)
    );
}

ptr<FunctionDefinition> Parser::parseFunctionDefinition() {
    const auto tok = mCurToken;

    EXPECT_TOKEN(IDENT)
    auto name = parseIdentifier();

    EXPECT_TOKEN(LPAREN);
    
    auto args = FunctionArgs();
    if (mPeekToken.type != Type::RPAREN) {
        while(true) {
            EXPECT_TOKEN(IDENT)
            args.emplace_back(std::move(parseIdentifierWithType()));
            if (mPeekToken.type == Type::RPAREN) break;
            EXPECT_TOKEN(COMMA)
        }
    }
    EXPECT_TOKEN(RPAREN)

    auto type = [this]() {
        if (peekTokenIs(Type::ARROW)) {
            EXPECT_TOKEN(ARROW)
            EXPECT_TYPE
            return parseIdentifier();
        }
        return std::make_unique<Identifier>(
            Token(Type::IDENT, "void", mCurToken.line, mCurToken.column)
        );
    }();

    auto body = parseBody();

    return std::make_unique<FunctionDefinition>(
        tok,
        std::move(name),
        std::move(type),
        std::move(args),
        std::move(body)
    );
}

ptr<IfStatement> Parser::parseIfStatement() {
    const auto tok = mCurToken;
    const auto parseClause = [&]() {
        EXPECT_TOKEN(LPAREN)
        nextToken();
        auto cond = parseExpression();
        EXPECT_TOKEN(RPAREN)
        auto body = parseBody();
        return IfStatement::Clause(
            std::move(cond),
            std::move(body)
        );
    };

    auto ifClause = parseClause();

    auto elifs = std::vector<IfStatement::Clause>();
    while (peekTokenIs(Type::ELIF)) {
        nextToken();
        elifs.emplace_back(std::move(parseClause()));
    }

    auto elseBody = [&]() -> IfStatement::Clause {
        if (peekTokenIs(Type::ELSE)) {
            nextToken();
            return {nullptr, parseBody()};
        }
        return IfStatement::Clause();
    }();

    return std::make_unique<IfStatement>(
        tok,
        std::move(ifClause),
        std::move(elifs),
        std::move(elseBody));
}