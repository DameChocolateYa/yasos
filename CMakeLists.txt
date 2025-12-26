#
 # yasos - a compiler for yasos language
 # Copyright (c) 2025 DameChocolateYa
 # Licensed under the BSD 3-Clause License.
 # See LICENSE file in the project root for full license text.
#

cmake_minimum_required(VERSION 3.10)

project(yasos)

option(__DEBUG__ "" ON)
execute_process(
    COMMAND llvm-config --cmakedir
    OUTPUT_VARIABLE LLVM_CMAKE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Force CMake to use this path
set(LLVM_DIR "${LLVM_CMAKE_DIR}" CACHE PATH "Path to LLVMConfig.cmake")
find_package(LLVM REQUIRED CONFIG)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS "-no-pie -g -O3")

add_executable(yasos src/error.cc src/lexer.cc src/parser.cc src/generation.cc src/main.cc)
target_include_directories(yasos PRIVATE ${LLVM_INCLUDE_DIRS})
target_link_libraries(yasos PRIVATE LLVM)
