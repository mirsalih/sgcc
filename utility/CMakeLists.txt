cmake_minimum_required(VERSION 3.10)

add_library(utilitylib ast_printer.cpp ast_printer.h)
target_include_directories(utilitylib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
