add_executable(Catch_tests_run
        statistical_test.cpp ../src/statistical.cpp
        classifiers_test.cpp ../src/classifiers.cpp
        io_manager_test.cpp ../src/io_manager.cpp
        casing_test.cpp ../src/casing.cpp
        helpers_test.cpp ../src/helpers.cpp
        selectors_test.cpp ../src/selectors.cpp
        src/matrix/matrix_test.cpp ../src/matrix/matrix.cpp
        )
target_link_libraries(Catch_tests_run)
