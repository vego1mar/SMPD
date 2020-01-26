#include "../../catch.hpp"
#include "../../../src/helpers/strings.hpp"

using helpers::Strings;


TEST_CASE("strings.hpp", "[strings]") {

    SECTION("split(case1,case2) -> OK") {
        std::string caseStr1 = "token1   2 three  4th ";
        std::string caseStr2 = "a,b,,c,d,efi,,,";
        std::vector<std::string> expectedResult1 = {"token1", "2", "three", "4th"};
        std::vector<std::string> expectedResult2 = {"a", "b", "c", "d", "efi"};

        auto result1 = Strings::split(caseStr1);
        auto result2 = Strings::split(caseStr2, ',');

        REQUIRE_THAT(result1, Catch::Equals(expectedResult1));
        REQUIRE_THAT(result2, Catch::Equals(expectedResult2));
    }

    SECTION("toLower(case1) -> OK") {
        std::string caseStr = "MAKE mE lOwEr";
        std::string expectedResult = "make me lower";

        auto result = Strings::toLower(caseStr);

        REQUIRE_THAT(result, Catch::Equals(expectedResult));
    }

    SECTION("startsWith(case1) -> OK") {
        std::string caseStr1 = "beta_012";

        auto result1 = Strings::startsWith(caseStr1, "beta");
        auto result2 = Strings::startsWith(caseStr1, "eta");

        REQUIRE(result1);
        REQUIRE(!result2);
    }

    SECTION("between(case1,case2) -> OK") {
        std::string caseStr1 = "aaaBB tok1 CDCC ddd";
        std::string caseStr2 = "<tag>tag_content</tag>";
        std::string expectedStr1 = "B tok1 CDC";
        std::string expectedStr2 = "tag_content";

        auto result1 = Strings::between(caseStr1, "B", "C");
        auto result2 = Strings::between(caseStr2, "<tag>", "</tag>");

        REQUIRE_THAT(result1, Catch::Equals(expectedStr1));
        REQUIRE_THAT(result2, Catch::Equals(expectedStr2));
    }

    SECTION("between(case3) -> OK") {
        std::string caseStr3 = "\nstr1_something_str2\n";
        std::string expectedStr3 = "_something_";

        auto result3 = Strings::between(caseStr3, "str1", "str2");

        REQUIRE_THAT(result3, Catch::Equals(expectedStr3));
    }

    SECTION("between(case4,case5) -> OK") {
        std::string caseStr4 = "alphabet 125abrz\\nstates: 0";
        std::string caseStr5 = "alphabet 125abrz\nstates: 0";
        std::string expectedStr4 = " 125abrz\\n";
        std::string expectedStr5 = " 125abrz\n";

        auto result4 = Strings::between(caseStr4, "alphabet", "states");
        auto result5 = Strings::between(caseStr5, "alphabet", "states");

        REQUIRE_THAT(result4, Catch::Equals(expectedStr4));
        REQUIRE_THAT(result5, Catch::Equals(expectedStr5));
    }

    SECTION("isNumber(case1-case7) -> OK") {
        std::string caseStr1 = "-12.70000";
        std::string caseStr2 = "11.23";
        std::string caseStr3 = "5.";
        std::string caseStr4 = "2";
        std::string caseStr5 = "1-";
        std::string caseStr6 = "1,3";
        std::string caseStr7 = "-1.30o0";

        auto result1 = Strings::isNumber(caseStr1);
        auto result2 = Strings::isNumber(caseStr2);
        auto result3 = Strings::isNumber(caseStr3);
        auto result4 = Strings::isNumber(caseStr4);
        auto result5 = Strings::isNumber(caseStr5);
        auto result6 = Strings::isNumber(caseStr6);
        auto result7 = Strings::isNumber(caseStr7);

        REQUIRE(result1);
        REQUIRE(result2);
        REQUIRE(result3);
        REQUIRE(result4);
        REQUIRE(!result5);
        REQUIRE(!result6);
        REQUIRE(!result7);
    }

    SECTION("betweenFirsts(case1-case4) -> OK") {
        std::string source1 = "alpha\tbet some strings then\nbeta";
        std::string source2 = "alpha\tbet a some strings then\nbeta";
        std::string source3 = "gamma 0123 delt abcd\ndelta";
        std::string source4 = "\rtheta 0 \nksi 1 \tpsi 2 \fni\r";

        auto result1 = Strings::betweenFirsts(source1, "alpha", "beta");
        auto result2 = Strings::betweenFirsts(source2, "beta", "alpha");
        auto result3 = Strings::betweenFirsts(source3, "gamma", "delta");
        auto result4 = Strings::betweenFirsts(source4, "ksi", "psi");

        REQUIRE_THAT(result1, Catch::Equals(source1.substr(5, 23)));
        REQUIRE_THAT(result2, Catch::Equals(""));
        REQUIRE_THAT(result3, Catch::Equals(source3.substr(5, 16)));
        REQUIRE_THAT(result4, Catch::Equals(source4.substr(13, 4)));
    }

    SECTION("replace(str into str) -> OK") {
        const std::string source1 = " 4  5  ^  $ @  ! ";
        const std::string what1 = "  ";
        const std::string into1 = " ";
        const std::string expectedResult1 = " 4 5 ^ $ @ ! ";

        const auto result1 = Strings::replace(source1, what1, into1);

        REQUIRE_THAT(result1, Catch::Equals(expectedResult1));
    }

}
