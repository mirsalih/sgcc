#include <iostream>
#include "../inc/reader.h"
#include "../inc/selectedExample.h"
#include "../lexer/lexer.h"
#include "../parser/parser.h"

int main() {
    try{
        sgcc::Lexer lexer(SourceReader{}(test::getSelectedExample()));
        sgcc::Parser parser(lexer.tokenize());
        auto program = parser.parseProgram();
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