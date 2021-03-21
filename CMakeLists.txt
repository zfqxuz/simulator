cmake_minimum_required(VERSION 3.10)
project(assembler)

set(CMAKE_CXX_STANDARD 14)

add_executable(assembler main.cpp func.h main.h dta.h func.cpp dta.cpp  oplist.cpp oplist.h simulartor.cpp simulartor.h)
