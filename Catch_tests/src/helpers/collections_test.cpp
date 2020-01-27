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

}
