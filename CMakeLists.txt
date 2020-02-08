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

        src/classifiers/definitions.hpp
        src/classifiers/nearest_neighbors.hpp src/classifiers/nearest_neighbors.cpp

        src/command_line/cla_parser.hpp src/command_line/cla_parser.cpp

        src/data_builders/classifiers_grouper.hpp src/data_builders/classifiers_grouper.cpp
        src/data_builders/csv_parser.hpp src/data_builders/csv_parser.cpp

        src/helpers/collections.hpp src/helpers/collections.cpp
        src/helpers/combinations.hpp src/helpers/combinations.cpp
        src/helpers/continuation_sequence.hpp src/helpers/continuation_sequence.cpp
        src/helpers/converters.hpp src/helpers/converters.cpp
        src/helpers/definitions.hpp
        src/helpers/stringify.hpp src/helpers/stringify.cpp
        src/helpers/strings.hpp src/helpers/strings.cpp

        src/io_manager/csv_reader.hpp src/io_manager/csv_reader.cpp
        src/io_manager/file_reader.hpp src/io_manager/file_reader.cpp

        src/main_program/statistical_run.hpp src/main_program/statistical_run.cpp

        src/matrix/matrix.hpp src/matrix/matrix.cpp

        src/selectors/definitions.hpp
        src/selectors/fisher.hpp src/selectors/fisher.cpp

        src/statistics/statistics.hpp src/statistics/statistics.cpp

        src/test/constants.hpp

)

add_subdirectory(Catch_tests)
