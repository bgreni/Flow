#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include <iostream>

int main() {
    std::string input = "return !a;";
    auto lexer = Lexer(std::move(input));
    auto parser = Parser(std::move(lexer));

    auto program = parser.parseProgram();
    std::cout << program->toString() << std::endl;

    return 0;
}
