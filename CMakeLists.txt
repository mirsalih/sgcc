cmake_minimum_required(VERSION 3.10)
project(sgcc VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
add_subdirectory(lexer)
add_subdirectory(parser)
add_subdirectory(code-generator)
add_subdirectory(utility)
add_subdirectory(src)
add_subdirectory(tests)
