#include "Lexer.h"
#include "Token/TokenUtils.h"
#include <iostream>

using namespace FlowToken;

#define MAKE_TOKEN Token(t, literal, mLine, mColumn)

bool Lexer::readChar() {
    ++mColumn;
    return bool(mStream.getChar(mCurChar));
}

Tokens Lexer::tokenize() {
    Tokens out;
    while (auto t = nextToken()) {
        out.emplace_back(t);
    }
    out.emplace_back(Token(Type::END, "", mLine, mColumn));
    return out;
}


Token Lexer::nextToken() {

    if (mCurChar == 0) {
        return Token(Type::END, "", mLine, mColumn);
    }

    skipWhitespace();
    
    Type t;
    std::string literal = {mCurChar};

    switch (mCurChar) {
        case '=':
            
            switch(mStream.peek()) {
                case '=':
                    t = Type::EQUALS;
                    literal = "==";
                    readChar();
                    break;
                default:
                    t = Type::ASSIGN;
            }
            break;
        case ':':
            t = Type::COLON;
            break;
        case ';':
            t = Type::SEMICOLON;
            break;
        case '+':
            t = Type::PLUS;
            break;
        case '-':
            switch (mStream.peek()) {
                case '>':
                    t = Type::ARROW;
                    literal = "->";
                    readChar();
                    break;
                default:
                    t = Type::MINUS;
            }
            break;
        case '*':
            t = Type::ASTERISK;
            break;
        case '/':
            t = Type::SLASH;
            break;
        case '{':
            t = Type::LBRACK;
            break;
        case '}':
            t = Type::RBRACK;
            break;
        case '(':
            t = Type::LPAREN;
            break;
        case ')':
            t = Type::RPAREN;
            break;
        case '[':
            t = Type::LSQR;
            break;
        case ']':
            t = Type::RSQR;
            break;
        case '!':
            switch (mStream.peek()) {
                case '=':
                    t = Type::NOT_EQUALS;
                    literal = "!=";
                    readChar();
                    break;
                default:
                    t = Type::BANG;
            }
            break;
        case ',':
            t = Type::COMMA;
            break;
        case '<':
            switch (mStream.peek()) {
                case '<':
                    t = Type::STREAMOUT;
                    literal = "<<";
                    readChar();
                    break;
                default:
                    t = Type::LT;
            }
            break;
        case '>':
            switch (mStream.peek()) {
                case '>':
                    t = Type::STREAMIN;
                    literal = ">>";
                    readChar();
                    break;
                default:
                    t = Type::GT;
            }
            break;
        default:
            if (isLetter(mCurChar)) {
                literal = readIdentifier();
                t = lookupIdent(literal);
                return MAKE_TOKEN;
            }
            else if (isDigit(mCurChar)) {
                t = Type::INT_LIT;
                literal = readNumber();
                return MAKE_TOKEN;
            }
            else {
                t = Type::ILLEGAL;
            }
    }

    readChar();
    return MAKE_TOKEN;
}

void Lexer::skipWhitespace() {
    while (std::isspace(mCurChar)) {
        if (mCurChar == '\n' || mCurChar == '\r') { ++mLine; mColumn = 0; }
        if (!readChar()) break;
    }
}

std::string Lexer::readIdentifier() {
    std::string out;
    while (isLetter(mCurChar)) {
        out += mCurChar;
        if (!readChar()) break;
    }
    return out;
}

std::string Lexer::readNumber() {
    std::string out;
    while (isDigit(mCurChar)) {
        out += mCurChar;
        if (!readChar()) break;
    }
    return out;
}