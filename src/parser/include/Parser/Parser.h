#pragma once

#include "Lexer/Lexer.h"
#include "Ast/Expression.h"
#include "Ast/Statement.h"
#include "Ast/Node.h"
#include "Token/TokenDefs.h"
#include "Common/Map.h"
#include "Common/Types.h"

using ExprParseFn = std::function<ptr<Expression>()>;

class Parser {

public:
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

    void nextToken() {
        mCurToken = mPeekToken;
        mPeekToken = mLexer.nextToken();
    }

    ptr<Statement> parseStatement();
    ptr<LetStatement> parseLetStatement();
    ptr<ReturnStatement> parseReturnStatement();

    ptr<Expression> parseExpression();
    ptr<Expression> parsePrefixExpression();
    ptr<Identifier> parseIdentifier();
    ptr<IntegerLiteral> parseIntegerLiteral();
    

    Lexer mLexer;

    FlowToken::Token mCurToken;
    FlowToken::Token mPeekToken;
    Flow::Map<FlowToken::Type, ExprParseFn> mPrefixParseFns;
    Flow::Map<FlowToken::Type, ExprParseFn> mInfixParseFns;
};