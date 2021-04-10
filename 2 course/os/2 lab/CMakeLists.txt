cmake_minimum_required(VERSION 3.16)
project(2_lab C)

set(CMAKE_C_STANDARD 11)

add_executable(2_lab parent.c)
add_executable(2_lab_child child.c)