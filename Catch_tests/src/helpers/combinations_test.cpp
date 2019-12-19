#include "../../catch.hpp"
#include "../../../src/helpers/combinations.hpp"

using helpers::Combinations;

TEST_CASE("combinations.hpp", "[helpers]") {
    SECTION("getNext() -> [4] until hasNext()") {
        auto result = std::vector<std::vector<int>>();
        Combinations combinations(4, 3);
        std::vector<std::vector<int>> expectedResults = {
                {0, 1, 2},
                {0, 1, 3},
                {0, 2, 3},
                {1, 2, 3}
        };

        while (combinations.hasNext()) {
            result.push_back(combinations.getNext());
        }

        REQUIRE(result.size() == 4);
        REQUIRE_THAT(result, Catch::UnorderedEquals(expectedResults));
        bool hasNextCombination = combinations.hasNext();
        REQUIRE(!hasNextCombination);
    }

    SECTION("getNext() -> [10] until hasNext()") {
        auto result = std::vector<std::vector<int>>();
        Combinations combinations(5, 3);
        std::vector<std::vector<int>> expectedResults = {
                {0, 1, 2},
                {0, 1, 3},
                {0, 1, 4},
                {0, 2, 3},
                {0, 2, 4},
                {0, 3, 4},
                {1, 2, 3},
                {1, 2, 4},
                {1, 3, 4},
                {2, 3, 4}
        };

        while (combinations.hasNext()) {
            result.push_back(combinations.getNext());
        }

        REQUIRE(result.size() == 10);
        REQUIRE_THAT(result, Catch::UnorderedEquals(expectedResults));
        bool hasNextCombination = combinations.hasNext();
        REQUIRE(!hasNextCombination);
    }

    SECTION("getNext() -> [6] until hasNext()") {
        auto result = std::vector<std::vector<int>>();
        Combinations combinations(4, 2);
        std::vector<std::vector<int>> expectedResults = {
                {0, 1},
                {0, 2},
                {0, 3},
                {1, 2},
                {1, 3},
                {2, 3}
        };

        while (combinations.hasNext()) {
            result.push_back(combinations.getNext());
        }

        REQUIRE(result.size() == 6);
        REQUIRE_THAT(result, Catch::UnorderedEquals(expectedResults));
        bool hasNextCombination = combinations.hasNext();
        REQUIRE(!hasNextCombination);
    }

    SECTION("getNext() -> [35] until hasNext()") {
        auto result = std::vector<std::vector<int>>();
        Combinations combinations(7, 4);
        std::vector<std::vector<int>> expectedResults = {
                {0, 1, 2, 3},
                {0, 1, 2, 4},
                {0, 1, 2, 5},
                {0, 1, 2, 6},
                {0, 1, 3, 4},
                {0, 1, 3, 5},
                {0, 1, 3, 6},
                {0, 1, 4, 5},
                {0, 1, 4, 6},
                {0, 1, 5, 6},
                {0, 2, 3, 4},
                {0, 2, 3, 5},
                {0, 2, 3, 6},
                {0, 2, 4, 5},
                {0, 2, 4, 6},
                {0, 2, 5, 6},
                {0, 3, 4, 5},
                {0, 3, 4, 6},
                {0, 3, 5, 6},
                {0, 4, 5, 6},
                {1, 2, 3, 4},
                {1, 2, 3, 5},
                {1, 2, 3, 6},
                {1, 2, 4, 5},
                {1, 2, 4, 6},
                {1, 2, 5, 6},
                {1, 3, 4, 5},
                {1, 3, 4, 6},
                {1, 3, 5, 6},
                {1, 4, 5, 6},
                {2, 3, 4, 5},
                {2, 3, 4, 6},
                {2, 3, 5, 6},
                {2, 4, 5, 6},
                {3, 4, 5, 6}
        };

        while (combinations.hasNext()) {
            result.push_back(combinations.getNext());
        }

        REQUIRE(result.size() == 35);
        REQUIRE_THAT(result, Catch::UnorderedEquals(expectedResults));
        bool hasNextCombination = combinations.hasNext();
        REQUIRE(!hasNextCombination);
    }

}
