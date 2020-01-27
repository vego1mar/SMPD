#include "../../catch.hpp"
#include "../../../src/helpers/collections.hpp"

using helpers::Collections;


TEST_CASE("collections_test", "[collections_test]") {

    SECTION("getDistinct -> OK") {
        const std::vector<std::string> source = {
                "name1", "name2", "name1", "name1", "name2", "name3", "", ".", "", ""
        };
        const std::set<std::string> expectedResult = {
                "name1", "name2", "name3", "", "."
        };

        const auto result = Collections::getDistinct(source);

        REQUIRE(result == expectedResult);
    }

    SECTION("count -> OK") {
        const std::vector<std::string> source = {
                "ab", "b", "ab", "ba", "a", "ab", "aa", "bb"
        };
        const std::size_t expectedResult1 = 3;
        const std::size_t expectedResult2 = 1;
        const std::size_t expectedResult3 = 0;

        const auto result1 = Collections::count(source, "ab");
        const auto result2 = Collections::count(source, "ba");
        const auto result3 = Collections::count(source, "c");

        REQUIRE(result1 == expectedResult1);
        REQUIRE(result2 == expectedResult2);
        REQUIRE(result3 == expectedResult3);
    }

}
