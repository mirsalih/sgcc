#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <unistd.h>

#include "../inc/compiler.h"

void printUsage(const char* progName) {
    std::cout << "Usage: " << progName << R"([options] source file
Options:
    -h                       Display this information.
    -p                       Prett-print the AST nodes
    -a [assembly file]       Place the output assembly file (default name out.s)
    -o <file>                Place the output into <file>
    )";
}
/**
 * @brief 
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char** argv) {
    using namespace std;
    using namespace sgcc;

    CompilerOptions comps;
    int opt;
    while((opt = getopt(argc, argv, "hpa::o:")) != -1) {
        switch(opt) {
            case 'h':
                printUsage(argv[0]);
                return EXIT_SUCCESS;
            
            case 'p':
                comps.prettyPrint = true;
                break;

            case 'o':
                comps.outFile = optarg;
                break;

            case 'a':
                if(optarg != nullptr) {
                    comps.assemblyFile = optarg;
                }
                else if (argc - optind > 1 && argv[optind] != nullptr && argv[optind][0] != '-') {
                    comps.assemblyFile = argv[optind];
                    optind++;
                }
                comps.dumpAssembly = true;
                break;
        }
    }

    // find the source code 
    if(optind < argc) {
        comps.inputFile = argv[optind++];
    }
    else {
        cerr << "Error: no input file\n";
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    if(optind < argc) {
        std::cerr << "Error: too many arguments. " << optind << "  " << argc << '\n';
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    if(comps.outFile.empty()) {
        comps.outFile = comps.inputFile.substr(0, comps.inputFile.find_last_of("."));
    }

    try {
        compile(comps);
    }
    catch(const std::exception& e) {
        std::cout<< "Error catched: " << e.what();
        return EXIT_FAILURE;
    }
    
    return 0;
}