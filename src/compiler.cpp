#include "../inc/compiler.h"

#include "../inc/reader.h"
#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../code-generator/cogen.h"
#include "../utility/ast_printer.h"

#include <filesystem>

namespace sgcc
{
void compile(CompilerOptions& comps)
{
    using namespace std;

    auto program = Parser{}.parseProgram(Lexer{}.tokenize(SourceReader{}(comps.inputFile)));
    if(!program) throw std::runtime_error("Parsing failed!");
    if(comps.prettyPrint) print(std::cout, *program);

    // code generation
    std::ofstream ofs(comps.assemblyFile);
    if(!ofs) throw runtime_error("failed to open output file: "s + comps.assemblyFile);
    Cogen(ofs, *program.get());
    ofs.close();

    // link the assembly
    auto command = "gcc -w "s + comps.assemblyFile + " -o " + comps.outFile;
    std::cout << command << '\n';
    if(system(command.c_str()) == -1) {
        throw runtime_error("Failed to link assembly: \n"s + command);
    }

    if(!comps.dumpAssembly) filesystem::remove(comps.assemblyFile);
}

}