cmake_minimum_required(VERSION 3.19)
project(lab8)

set(CMAKE_CXX_STANDARD 20)
set(TARGET_NAME ${PROJECT_NAME})

add_executable(${TARGET_NAME} main.cpp)

target_compile_options(${TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
)
