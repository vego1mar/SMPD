#include "catch.hpp"
#include "../src/classifiers.hpp"

TEST_CASE("Nearest Neighbor", "[classifiers]") {
    std::vector<classifiers::ClassVector> cluster1 = {
            {"A", {1, 2, 3}},
            {"A", {2, 5, 3}},
            {"A", {2, 5, 4}},
            {"A", {4, 7, 5}},
            {"B", {1, 7, 1}},
            {"B", {3, 9, 1}},
            {"B", {3, 8, 2}},
            {"B", {5, 6, 1}},
    };

    SECTION("x=(1,7,3), cluster1, k=1 -> 'B'") {
        std::vector<double> input = {1, 7, 3};
        auto inputLabel = classifiers::nearest_neighbor(input, cluster1, 1);

        REQUIRE_THAT(inputLabel, Catch::Equals("B"));
    }

    SECTION("x=(1,7,3), cluster1, k=3 -> 'A'") {
        std::vector<double> input = {1, 7, 3};
        auto inputLabel = classifiers::nearest_neighbor(input, cluster1, 3);

        REQUIRE_THAT(inputLabel, Catch::Equals("A"));
    }

    SECTION("x=(2,5,2), cluster1, k=3 -> 'A'") {
        std::vector<double> input = {2, 5, 2};
        auto inputLabel = classifiers::nearest_neighbor(input, cluster1, 3);

        REQUIRE_THAT(inputLabel, Catch::Equals("A"));
    }

    SECTION("x=(1,0,1), cluster1, k=0 -> throws") {
        std::vector<double> x = {1, 0, 1};

        auto nn_func = [&]() {
            return classifiers::nearest_neighbor(x, cluster1, 0);
        };

        REQUIRE_THROWS_AS(nn_func(), std::invalid_argument);
    }

    SECTION("x=(1,0), cluster1, k=5 -> throws") {
        std::vector<double> x = {1, 0};

        auto nn_func = [&]() {
            return classifiers::nearest_neighbor(x, cluster1, 5);
        };

        REQUIRE_THROWS_AS(nn_func(), std::invalid_argument);
    }
}
