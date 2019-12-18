cmake_minimum_required(VERSION 3.15)
project(SMPD)

set(CMAKE_CXX_STANDARD 14)

add_executable(SMPD src/main.cpp
        src/statistical.hpp src/statistical.cpp
        src/casing.hpp src/casing.cpp
        src/io_manager.hpp src/io_manager.cpp
        src/classifiers.cpp src/classifiers.hpp
        src/helpers.cpp src/helpers.hpp src/selectors.cpp src/selectors.hpp src/matrix/matrix.hpp src/matrix/matrix.cpp src/selectors/fisher.cpp src/selectors/fisher.hpp src/statistics/statistics.cpp src/statistics/statistics.hpp src/helpers/converters.cpp src/helpers/converters.hpp src/helpers/combinations.cpp src/helpers/combinations.hpp)
add_subdirectory(Catch_tests)
