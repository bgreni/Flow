#pragma once

#include <exception>
#include <string>

class ParserError : public std::exception {
public:
    ParserError(const std::string & error) 
    : mError(error){}
    const char * what() const noexcept override {
        return mError.c_str();
    }
private:
    std::string mError;
};