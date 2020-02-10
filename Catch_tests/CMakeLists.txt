add_executable(
        Catch_tests_run

        src/classifiers/nearest_mean.cpp ../src/classifiers/nearest_mean.cpp
        src/classifiers/nearest_neighbors_test.cpp ../src/classifiers/nearest_neighbors.cpp

        src/command_line/cla_parser_test.cpp ../src/command_line/cla_parser.cpp

        ../src/data_builders/classifiers_grouper.cpp
        src/data_builders/csv_parser_test.cpp ../src/data_builders/csv_parser.cpp

        src/helpers/collections_test.cpp ../src/helpers/collections.cpp
        src/helpers/combinations_test.cpp ../src/helpers/combinations.cpp
        src/helpers/continuation_sequence_test.cpp ../src/helpers/continuation_sequence.cpp
        ../src/helpers/converters.cpp
        src/helpers/stringify_test.cpp ../src/helpers/stringify.cpp
        src/helpers/strings_test.cpp ../src/helpers/strings.cpp

        src/io_manager/csv_reader.cpp ../src/io_manager/csv_reader.cpp
        ../src/io_manager/file_reader.cpp

        src/main_program/statistical_run_test.cpp ../src/main_program/statistical_run.cpp

        src/matrix/matrix_test.cpp ../src/matrix/matrix.cpp

        src/selectors/fisher_test.cpp ../src/selectors/fisher.cpp

        ../src/statistics/statistics.cpp

        casing_test.cpp ../src/casing.cpp
        classifiers_test.cpp ../src/classifiers.cpp
        helpers_test.cpp ../src/helpers.cpp
        io_manager_test.cpp ../src/io_manager.cpp
        selectors_test.cpp ../src/selectors.cpp
        statistical_test.cpp ../src/statistical.cpp

)

target_link_libraries(Catch_tests_run)
