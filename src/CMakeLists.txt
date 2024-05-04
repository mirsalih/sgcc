cmake_minimum_required(VERSION 3.10)

add_executable(sgcc main.cpp compiler.cpp)
target_link_libraries(sgcc PRIVATE lexerlib parserlib codegenlib)
