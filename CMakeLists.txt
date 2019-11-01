cmake_minimum_required(VERSION 3.15)
project(SMPD)

set(CMAKE_CXX_STANDARD 14)

add_executable(SMPD src/main.cpp src/statistical.hpp src/statistical.cpp src/classifiers.hpp src/classifiers.cpp)
add_subdirectory(Catch_tests)
