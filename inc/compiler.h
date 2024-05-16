#ifndef __COMPILER_H__
#define __COMPILER_H__

#include <iosfwd>
#include <string>

namespace sgcc
{

struct CompilerOptions {
    bool dumpAssembly{false};
    bool prettyPrint{false};

    std::string outFile;
    std::string inputFile;
    std::string assemblyFile{"out.s"};
};

void compile(CompilerOptions&);

}
#endif // __COMPILER_H__