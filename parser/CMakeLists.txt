cmake_minimum_required(VERSION 3.10)

add_library(parserlib parser.cpp parser.h)
target_include_directories(parserlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
