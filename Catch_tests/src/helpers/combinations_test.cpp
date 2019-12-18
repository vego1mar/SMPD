#include "../../catch.hpp"
#include "../../../src/helpers/combinations.hpp"

using helpers::Combinations;

TEST_CASE("combinations.hpp", "[helpers]") {
    SECTION("? -> ?") {
        auto result = std::vector<std::vector<int>>();
        Combinations combinations(4, 3);

        while (combinations.hasNext()) {
            result.push_back(combinations.getNext());
        }

        REQUIRE(result.size() == 4);
    }
}
