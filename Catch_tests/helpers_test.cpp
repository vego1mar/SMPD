#include "catch.hpp"
#include "../src/helpers.hpp"
#include "../src/casing.hpp"

using helpers::Means;
using casing::ClassVector;

TEST_CASE("helpers", "[helpers]") {
    Cluster CLUSTER_A = {
            ClassVector("A", {1, 2, -1}),
            ClassVector("A", {1, 3, 2}),
            ClassVector("A", {2, 1, 1}),
            ClassVector("A", {1, 7, -3}),
    };
    Cluster CLUSTER_B = {
            ClassVector("B", {3, 4, 0}),
            ClassVector("B", {2, 4, 3}),
            ClassVector("B", {5, 5, -2})
    };

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

    SECTION("getTransposed(C) -> C^T") {
        Cluster expectedResult1 = {
                ClassVector("A", {1, 1, 2, 1}),
                ClassVector("A", {2, 3, 1, 7}),
                ClassVector("A", {-1, 2, 1, -3})
        };
        Cluster expectedResult2 = {
                ClassVector("B", {3, 2, 5}),
                ClassVector("B", {4, 4, 5}),
                ClassVector("B", {0, 3, -2})
        };

        auto result1 = helpers::getTransposed(CLUSTER_A);
        auto result2 = helpers::getTransposed(CLUSTER_B);

        REQUIRE_THAT(result1, Catch::UnorderedEquals(expectedResult1));
        REQUIRE_THAT(result2, Catch::UnorderedEquals(expectedResult2));
    }

    SECTION("getStandardDeviation(A;B) -> [3;3]") {
        std::vector<double> expectedResults1 = {
                std::sqrt(12.0) / 8.0,
                std::sqrt(83.0) / 4.0,
                std::sqrt(59.0) / 4.0,
        };
        std::vector<double> expectedResults2 = {
                std::sqrt(14.0) / 3.0,
                std::sqrt(2.0 / 9.0),
                std::sqrt(114.0 / 27.0)
        };

        auto result1 = helpers::getStandardDeviation(CLUSTER_A);
        auto result2 = helpers::getStandardDeviation(CLUSTER_B);

        REQUIRE_THAT(result1, Catch::UnorderedEquals(expectedResults1));
        REQUIRE_THAT(result2, Catch::UnorderedEquals(expectedResults2));
    }
}
