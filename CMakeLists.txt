cmake_minimum_required(VERSION 3.10)

project(yasos)

option(__DEBUG__ "" ON)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-no-pie -g -O3")

add_executable(yasos src/error.cpp src/tokenization.cpp src/parser.cpp src/functions/standard.cpp src/generation.cpp src/main.cpp)
