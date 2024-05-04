#include <iostream>
#include "../inc/reader.h"
#include "../inc/selectedExample.h"
#include "../parser/parser.h"
#include "../lexer/lexer.h"

int main() {
    const auto tokens = sgcc::Lexer{}.tokenize(SourceReader{}(test::getSelectedExample()));
    try{
        auto program = sgcc::Parser{}.parseProgram(tokens);
        if(program) {
            program->print(std::cout);
        }
        else {
            std::cout<< "Erro accured";
        }
    }
    catch(const std::exception& e) {
        std::cout<< "Error Chached!!!!! " << e.what();
    }
    
    return 0;
}