#include "../../catch.hpp"
#include "../../../src/main_program/statistical_run.hpp"

using main_program::StatisticalRun;


TEST_CASE("statistical_run", "[statistical_run_test]") {

    SECTION("performRun[2] -> OK", "[stdout]") {
        // ./program_name --path ../../files/Maple_Oak.csv --features-no 2
        int argc = 5;
        char arg0[] = "program_name";
        char arg1[] = "--path";
        char arg2[] = "../../files/Maple_Oak.csv";
        char arg3[] = "--features-no";
        char arg4[] = "2";
        char *argv[] = {arg0, arg1, arg2, arg3, arg4, nullptr};

        StatisticalRun program;
        program.run(argc, argv);
        program.dispose();
    }

    SECTION("performRun[3] --sfs -> OK", "[stdout]") {
        // ./program_name --path ../../files/Maple_Oak.csv --features-no 3 --sfs
        int argc = 6;
        char arg0[] = "program_name";
        char arg1[] = "--path";
        char arg2[] = "../../files/Maple_Oak.csv";
        char arg3[] = "--features-no";
        char arg4[] = "3";
        char arg5[] = "--sfs";
        char *argv[] = {arg0, arg1, arg2, arg3, arg4, arg5, nullptr};

        StatisticalRun program;
        program.run(argc, argv);
        program.dispose();
    }

}
