cmake_minimum_required(VERSION 3.6)   # CMake version check
project(QHM)               # Create project "simple_example"

# added -fopenmp
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")
set(CMAKE_CXX_STANDARD 11)            # Enable c++11 standard

set(SOURCE_FILES main.cpp Graph.cpp Graph.h)            # Add main.cpp file of project root directory as source file
add_executable(qhm ${SOURCE_FILES})       # Add executable target with source files listed in SOURCE_FILES variable