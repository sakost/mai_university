cmake_minimum_required(VERSION 3.17)
project(oop_excercise_08)

set(CMAKE_CXX_STANDARD 17)

find_package (Threads)

add_executable(oop_excercise_08 main.cpp figure.h concretefigures.h handler.h)
target_link_libraries (oop_excercise_08 Threads::Threads)
