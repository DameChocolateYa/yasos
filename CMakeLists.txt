cmake_minimum_required(VERSION 3.10)

project(yasos)

option(__DEBUG__ "" ON)
find_package(LLVM REQUIRED CONFIG)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-no-pie -g -O3")

add_executable(yasos src/error.cc src/lexer.cc src/parser.cc src/generation.cc src/main.cc)
target_include_directories(yasos PRIVATE ${LLVM_INCLUDE_DIRS})
target_link_libraries(yasos PRIVATE LLVM)
