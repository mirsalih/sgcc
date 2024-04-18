cmake_minimum_required(VERSION 3.10)

add_library(lexerlib lexer.cpp lexer.h)
target_include_directories(lexerlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
