#include "catch.hpp"
#include "../src/helpers.hpp"

using helpers::Means;

TEST_CASE("helpers", "[helpers]") {
    SECTION("getOrdinalSequence(0,11) -> len(10)") {
        auto ordinals = helpers::getOrdinalSequence(0, 11);
        std::vector<int> expected = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        REQUIRE(ordinals.size() == 11);
        REQUIRE_THAT(ordinals, Catch::UnorderedEquals(expected));
    }

    SECTION("computeGeometricCloseness(A,B) -> (sqrt[21] + sqrt[10]) / 2") {
        Means setA = {
                {1, 2},
                {2, 3},
                {5, 4}
        };
        Means setB = {
                {3, 5},
                {3, 3},
                {1, 3}
        };
        double expectedResult = (1.0 + std::sqrt(13) + std::sqrt(17)) / 3;

        auto result = helpers::computeGeometricCloseness(setA, setB);

        REQUIRE(result == Approx(expectedResult));
    }
}
