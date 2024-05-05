#include "../inc/compiler.h"

#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../code-generator/cogen.h"
#include "../utility/ast_printer.h"

namespace sgcc
{
void compile(const std::string& src, std::ostream& os)
{   
    auto program = Parser{}.parseProgram(Lexer{}.tokenize(src));
    if(!program) throw std::runtime_error("Parsing failed!");
    print(std::cout, *program);
    sgcc::Cogen(os, *program.get());
}

}