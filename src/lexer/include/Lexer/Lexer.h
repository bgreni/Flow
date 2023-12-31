#pragma once
#include "TokenStream.h"
#include <string>
#include "Token/Token.h"

class Lexer {
public:
    Lexer(std::string && stream)
        : mStream(std::move(stream))
    {readChar();}
    FlowToken::Token nextToken();
    bool readChar();
    FlowToken::Tokens tokenize();
private:
    void skipWhitespace();
    std::string readIdentifier();
    std::string readNumber();

    TokenStream mStream;
    char mCurChar;

    unsigned int mLine = 1;
    unsigned int mColumn = 0;
};