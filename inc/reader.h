#ifndef __READER_H__
#define __READER_H__

#include <string>
#include <fstream>
#include <algorithm>
#include <iterator>

struct SourceReader {
    std::string operator()(const std::string sourceFileName)
    {
        using namespace std;
        ifstream ifs{sourceFileName};

        if(!ifs.is_open()) {
            throw runtime_error("No file: "s + sourceFileName);
        }
        noskipws(ifs);
        return std::string(istream_iterator<char>{ifs}, {});
    }
};

#endif // __READER_H__