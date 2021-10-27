cmake_minimum_required(VERSION 3.17)
project(da_lab_6)

set(CMAKE_CXX_STANDARD 17)

add_executable(da_lab_6_slow main.cpp longarithmetic.h)

add_executable(da_lab_6_fast main.cpp longarithmetic.h)
target_compile_definitions(da_lab_6_fast PUBLIC FAST_MUL)
