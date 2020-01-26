cmake_minimum_required(VERSION 3.15)
project(SMPD)

set(CMAKE_CXX_STANDARD 14)

add_executable(
        SMPD src/main.cpp

        src/casing.hpp src/casing.cpp
        src/classifiers.hpp src/classifiers.cpp
        src/helpers.hpp src/helpers.cpp
        src/io_manager.hpp src/io_manager.cpp
        src/selectors.hpp src/selectors.cpp
        src/statistical.hpp src/statistical.cpp

        src/helpers/combinations.hpp src/helpers/combinations.cpp
        src/helpers/converters.hpp src/helpers/converters.cpp
        src/helpers/strings.hpp src/helpers/strings.cpp

        src/io_manager/file_reader.hpp src/io_manager/file_reader.cpp

        src/matrix/matrix.hpp src/matrix/matrix.cpp

        src/selectors/fisher.hpp src/selectors/fisher.cpp

        src/io_manager/csv_reader.hpp src/io_manager/csv_reader.cpp
        src/statistics/statistics.hpp src/statistics/statistics.cpp
        src/test/constants.hpp)

add_subdirectory(Catch_tests)
