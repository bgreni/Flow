#include "TokenStream.h"
#include "Token/Token.h"


char TokenStream::getChar(char & c) {
    if (mReadPosition >= size()) {
        c = 0;
        return 0;
    }
    c = at(mReadPosition);
    
    mPosition = mReadPosition;
    ++mReadPosition;
    return c;
}

char TokenStream::peek() {
    if (mReadPosition >= size()) {
        return 0;
    }
    return at(mReadPosition);
}