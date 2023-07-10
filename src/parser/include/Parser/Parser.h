#pragma once

#include "Ast/Llvm.h"
#include "Ast/Statements/ExpressionStatement.h"
#include "Lexer/Lexer.h"
#include "Ast/Expression.h"
#include "Ast/Statement.h"
#include "Ast/Node.h"
#include "Token/TokenDefs.h"
#include "Common/Map.h"
#include "Common/Types.h"

using ExprParseFn = std::function<ptr<Expression>()>;
using BinaryOpParseFn = std::function<ptr<Expression>(ptr<Expression> &&)>;

class Parser {
public:
    enum OpPrecedence : int {
        Lowest,
        Eq,
        LessGreat,
        Sum,
        Product
    };

    Parser(Lexer && lexer);

    ptr<Program> parseProgram();
private:
    bool expectNext(FlowToken::Type t) {
        if (mPeekToken.type == t) {
            nextToken();
            return true;
        }
        return false;
    }
    bool curTokenIs(FlowToken::Type t) { return mCurToken.type == t; }
    bool peekTokenIs(FlowToken::Type t) { return mPeekToken.type == t; }

    OpPrecedence getPrecedence(FlowToken::Type t) {
        return cPrecedences.get(t).value_or(OpPrecedence::Lowest);
    }

    OpPrecedence curPrecedence() {
        return getPrecedence(mCurToken.type);
    }

    OpPrecedence peekPrecedence() {
        return getPrecedence(mPeekToken.type);
    }

    void nextToken() {
        mCurToken = mPeekToken;
        mPeekToken = mLexer.nextToken();
    }

    ptr<Expression> parseExpression(OpPrecedence precedence = OpPrecedence::Lowest);
    ptr<Expression> parsePrefixExpression();
    ptr<Identifier> parseIdentifier();
    ptr<IdentifierWithType> parseIdentifierWithType();
    ptr<IntegerLiteral> parseIntegerLiteral();
    ptr<Lambda> parseLamda();
    ptr<BinaryOp> parseBinaryOp(ptr<Expression> && left);
    ptr<FunctionInvokation> parseFunctionInvokation();

    ptr<Statement> parseStatement();
    ptr<LetStatement> parseLetStatement();
    ptr<ReturnStatement> parseReturnStatement();
    ptr<FunctionDefinition> parseFunctionDefinition();
    ptr<ExpressionStatement> parseExpressionStatement();
    

    Lexer mLexer;

    FlowToken::Token mCurToken;
    FlowToken::Token mPeekToken;
    Flow::Map<FlowToken::Type, ExprParseFn> mPrefixParseFns;
    Flow::Map<FlowToken::Type, BinaryOpParseFn> mInfixParseFns;

    using Precedences = Flow::Map<FlowToken::Type, OpPrecedence>;

    const Precedences cPrecedences = {
        {{FlowToken::Type::EQUALS, OpPrecedence::Eq},
        {FlowToken::Type::LT, OpPrecedence::LessGreat},
        {FlowToken::Type::GT, OpPrecedence::LessGreat},
        {FlowToken::Type::PLUS, OpPrecedence::Sum},
        {FlowToken::Type::MINUS, OpPrecedence::Sum},
        {FlowToken::Type::SLASH, OpPrecedence::Product},
        {FlowToken::Type::ASTERISK, OpPrecedence::Product},
        },
    };
};