cmake_minimum_required(VERSION 3.10)

project(beep++)

set(CMAKE_CXX_STANDARD 20)

set(OBJS src/functions/std.o)

set(CMAKE_CXX_FLAGS "-no-pie")

add_executable(beep++ src/main.cpp ${OBJS})
