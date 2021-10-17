cmake_minimum_required(VERSION 3.19)
project(lab5)

set(CMAKE_CXX_STANDARD 20)
set(TARGET_NAME ${PROJECT_NAME})


add_library(suffixtree STATIC suffixtree.cpp)
add_library(naive STATIC naive.cpp)

# main executable
add_executable(${TARGET_NAME} main.cpp)
target_link_libraries(${TARGET_NAME} suffixtree)

# benchmark executable
add_executable(${TARGET_NAME}_bench benchmark.cpp)
target_link_libraries(${TARGET_NAME}_bench suffixtree naive)
