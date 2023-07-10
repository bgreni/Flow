#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include "src/codegen/include/CodeGen/Llvm.h"
#include "Ast/Exceptions.h"

int main(int argc, char** argv) {
    const auto filename = std::string(argv[1]);
    
    auto file = std::ifstream(filename);

    std::string input;
    std::string line;

    while (getline(file, line)) {
        input += line;
    }

    file.close();


    auto lexer = Lexer(std::move(input));
    auto parser = Parser(std::move(lexer));

    auto program = parser.parseProgram();

    try {
        auto generator = LLVMGenerator();
        generator.process(program.get());
    } catch(const CodeGenError & e) {
        std::cout << "CodeGen failed: " << e.what() << std::endl;
    }

    return 0;
}
