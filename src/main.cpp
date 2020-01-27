#include "main_program/statistical_run.hpp"

namespace programs {

    void runProgram1(int argc, char **argv) {
        using main_program::StatisticalRun;
        StatisticalRun program;
        program.run(argc, argv);
        program.dispose();
    }

}


int main(int argc, char **argv) {
    programs::runProgram1(argc, argv);
    return 0;
}
