#pragma once
#include "TokenStream.h"
#include <string>
#include "Token/Token.h"

class Lexer {
public:
    Lexer(std::string && stream)
        : mStream(std::move(stream))
    {}
    FlowToken::Token nextToken();
    bool readChar();
    FlowToken::Tokens tokenize();
private:
    void skipWhitespace();
    std::string readIdentifier();
    std::string readNumber();

    TokenStream mStream;
    char mCurChar;
};