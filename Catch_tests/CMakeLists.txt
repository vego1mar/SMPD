add_executable(Catch_tests_run
        statistical_test.cpp ../src/statistical.cpp
        classifiers_test.cpp ../src/classifiers.cpp
        io_manager_test.cpp ../src/io_manager.cpp
        ../src/casing.cpp
        ../src/helpers.cpp
        )
target_link_libraries(Catch_tests_run)
