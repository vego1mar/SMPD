#include "../../catch.hpp"
#include "../../../src/command_line/cla_parser.hpp"

using command_line::CLAParser;


TEST_CASE("cla_parser_test", "[cla_parser]") {

    SECTION("getOption('--fsm') -> 'dfa'") {
        // program_name --fsm dfa --def dfa_1.txt --input input_1 NULL
        int argc = 7;
        char arg0[] = "program_name";
        char arg1[] = "--fsm";
        char arg2[] = "dfa";
        char arg3[] = "--def";
        char arg4[] = "../../src/files/dfa_1.txt";
        char arg5[] = "--input";
        char arg6[] = "../../src/files/input_1.txt";
        char *argv[] = {arg0, arg1, arg2, arg3, arg4, arg5, arg6, nullptr};
        CLAParser parser(argc, argv);

        bool isFsmOptionPresent = parser.isOptionExists("--fsm");
        bool isDefOptionPresent = parser.isOptionExists("--def");
        bool isInputOptionPresent = parser.isOptionExists("--input");
        bool areOptionsPresent = isFsmOptionPresent && isDefOptionPresent && isInputOptionPresent;
        assert(areOptionsPresent);
        auto fsmValue = parser.getOption("--fsm");
        auto defValue = parser.getOption("--def");
        auto inputValue = parser.getOption("--input");

        REQUIRE_THAT(parser.getProgramName(), Catch::Equals("program_name"));
        REQUIRE_THAT(fsmValue, Catch::Equals("dfa"));
        REQUIRE_THAT(defValue, Catch::Equals("../../src/files/dfa_1.txt"));
        REQUIRE_THAT(inputValue, Catch::Equals("../../src/files/input_1.txt"));
    }

}
