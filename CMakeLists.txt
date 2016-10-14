cmake_minimum_required(VERSION 3.6)
project(c_minus_compiler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp compiler/lexer/lex.h compiler/lexer/lex.cpp utils/DFA/DFA.h utils/DFA/DFA.cpp compiler/parser/parser.h compiler/parser/parser.cpp)
add_executable(c_minus_compiler ${SOURCE_FILES})