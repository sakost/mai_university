cmake_minimum_required(VERSION 3.19)
project(lab5)

set(CMAKE_CXX_STANDARD 20)
set(TARGET_NAME ${PROJECT_NAME})

add_executable(${TARGET_NAME} main.cpp suffixtree.cpp suffixtree.h)