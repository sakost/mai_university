cmake_minimum_required(VERSION 3.17)
project(6_8_lab)

set(CMAKE_CXX_STANDARD 17)

find_package(Protobuf REQUIRED)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS lab.proto)

find_package(cppzmq)

add_executable(6_8_lab main.cpp ${PROTO_SRCS})
target_link_libraries(6_8_lab ${Protobuf_LIBRARIES} cppzmq)