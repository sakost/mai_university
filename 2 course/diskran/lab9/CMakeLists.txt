cmake_minimum_required(VERSION 3.17)
project(lab9)

set(CMAKE_CXX_STANDARD 17)

add_library(lab9_graph STATIC graph.cpp)

add_library(lab9_johnson STATIC johnson.cpp)
target_link_libraries(lab9_johnson lab9_graph)

add_library(lab9_floyd_warshall STATIC floyd_warshall.cpp)
target_link_libraries(lab9_floyd_warshall lab9_graph)

# benchmark executable
add_executable(lab9_benchmark benchmark.cpp)
target_link_libraries(lab9_benchmark lab9_floyd_warshall lab9_johnson)

# main executable
add_executable(lab9 main.cpp)
target_link_libraries(lab9 lab9_johnson)
