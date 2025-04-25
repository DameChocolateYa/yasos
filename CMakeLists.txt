cmake_minimum_required(VERSION 3.10)

project(beeplang)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-no-pie -g -O2")

add_executable(beepc src/tokenization.cpp src/parser.cpp src/generation.cpp src/main.cpp)
