add_executable(Catch_tests_run statistical_test.cpp ../src/statistical.cpp ../src/classifiers.cpp classifiers_test.cpp)
target_link_libraries(Catch_tests_run)
