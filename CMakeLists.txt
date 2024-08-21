cmake_minimum_required(VERSION 3.29)
project(mc8emu)

set(CMAKE_CXX_STANDARD 20)

add_executable(mc8emu chip8_main.cpp
        CPUStructures.h
        Chip8.cpp)
