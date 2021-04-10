cmake_minimum_required(VERSION 3.16)
project(3_lab C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_USE_PTHREADS_INIT true)
set(CMAKE_USE_PTHREADS_INIT ON)

#add_definitions(-DFAST_CUSTOM_RAND)

find_package(Threads REQUIRED)

add_executable(3_lab main.c)
target_link_libraries(3_lab Threads::Threads)
