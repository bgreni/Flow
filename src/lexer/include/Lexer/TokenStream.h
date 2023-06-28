
#pragma once

#include <string_view>
#include <string>

class TokenStream : public std::string_view {
public:
    TokenStream(std::string && input) : std::string_view(input){}

    char getChar(char & c);
    char peek();

    unsigned int mPosition = 0;
    unsigned int mReadPosition = 0;
};