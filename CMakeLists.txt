cmake_minimum_required(VERSION 3.10)

project(yasos)

option(__DEBUG__ "" ON)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-no-pie -g -O3")

add_executable(yasos src/error.cc src/tokenization.cc src/parser.cc src/generation.cc src/main.cc)
