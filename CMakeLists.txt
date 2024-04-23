cmake_minimum_required(VERSION 3.10)
project(sgcc VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(BUILD_FOR_TESTS "Build the project for unit tests" OFF)
add_subdirectory(lexer)
add_subdirectory(parser)
if (BUILD_FOR_TESTS)
    add_subdirectory(tests)
else()
    add_subdirectory(src)
endif()
