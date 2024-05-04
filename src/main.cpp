#include <iostream>
#include <fstream>
#include <string>

#include "../inc/compiler.h"
#include "../inc/reader.h"

/**
 * @brief 
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char** argv) {
    using namespace std;
    if(argc < 2) {
        std::cerr << "no input file ";
        return 0;
    }

    try {
        const std::string sourCode = SourceReader{}(argv[1]);
        const std::string outFName{argc > 2 ? argv[2] : "out.s"};
        std::ofstream ofs(outFName);
        if(!ofs) throw std::runtime_error("failed to open output file: "s + outFName);

        sgcc::compile(sourCode, ofs);
    }
    catch(const std::exception& e) {
        std::cout<< "Error catched: " << e.what();
    }
    
    return 0;
}