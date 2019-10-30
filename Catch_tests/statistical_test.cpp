// #define CATCH_CONFIG_MAIN - don't need to repeat this include in more than one cpp file
#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "../src/statistical.hpp"

TEST_CASE("Arithmetic Mean", "[statistical]") {
    SECTION("{1,2,3,4,5} -> 3") {
        std::vector<double> list = {1, 2, 3, 4, 5};
        auto mean = statistical::arithmetic_mean(list);

        REQUIRE(mean == Approx(3.0));
    }

    SECTION("{2*sqrt(2), PI} -> 2.9850", "[statistical]") {
        std::vector<double> list = {2.0 * std::sqrt(2.0), M_PI};
        auto mean = statistical::arithmetic_mean(list);

        REQUIRE(mean == Approx(2.9850).epsilon(0.01));
    }
}

TEST_CASE("Mean Norm Error", "[statistical]") {
    SECTION("(5,3); p=3 -> 8") {
        auto mne = statistical::mean_norm_error(5, 3, 3);

        REQUIRE(mne == Approx(8));
    }

    SECTION("(1,PI); p=2 -> 4.586") {
        auto mne = statistical::mean_norm_error(1, M_PI, 2);

        REQUIRE(mne == Approx(4.586).margin(3));
    }

    SECTION("(sqrt(3),sqrt(2)); p=1 -> 0.317") {
        auto mne = statistical::mean_norm_error(std::sqrt(3), std::sqrt(2), 1);

        REQUIRE(mne == Approx(0.317).margin(3));
    }

    SECTION("(3,23.32); p=0 -> 1") {
        auto mne = statistical::mean_norm_error(3, 23.32, 0);

        REQUIRE(mne == Approx(1.0));
    }
}

TEST_CASE("Mean Squared Error", "[statistical]") {
    SECTION("(4/6, 10/25) -> 16/225") {
        auto mse = statistical::mean_squared_error(4.0 / 6.0, 10.0 / 25.0);

        REQUIRE(mse == Approx(16.0 / 225.0));
    }
}

TEST_CASE("Variance", "[statistical]") {
    SECTION("{1,7,4,sqrt(2),-3.1} -> 11.2666") {
        std::vector<double> values = {1, 7, 4, std::sqrt(2), -3.1};
        auto variance = statistical::variance(values);

        REQUIRE(variance == Approx(11.2666).margin(4));
    }
}

TEST_CASE("Standard Deviation", "[statistical]") {
    SECTION("{1,7,4,sqrt(2),-3.1} -> 3.356") {
        std::vector<double> values = {1, 7, 4, std::sqrt(2), -3.1};
        auto variance = statistical::standard_deviation(values);

        REQUIRE(variance == Approx(3.356).margin(3));
    }
}

TEST_CASE("Norm Distance", "[statistical]") {
    SECTION("(1,3,PI); (1/3,0,5) -> 2.7535") {
        std::vector<double> point1 = {1, 3, M_PI};
        std::vector<double> point2 = {1.0 / 3.0, 0, 5};
        auto distance = statistical::norm_distance(point1, point2, 3);

        REQUIRE(distance == Approx(2.7535).margin(4));
    }

    SECTION("(1, 1, 1); (1, 1) -> throw", "[statistical]") {
        std::vector<double> point1 = {1, 1, 1};
        std::vector<double> point2 = {1, 1};

        auto distance_func = [&]() {
            return statistical::norm_distance(point1, point2, 0);
        };

        REQUIRE_THROWS_AS(distance_func(), std::invalid_argument);
    }
}

TEST_CASE("Geometric Distance", "[statistical]") {
    SECTION("(1,7,3); (4,7,5) -> sqrt(13)") {
        std::vector<double> point1 = {1, 7, 3};
        std::vector<double> point2 = {4, 7, 5};
        auto distance = statistical::geometric_distance(point1, point2);

        REQUIRE(distance == Approx(std::sqrt(13)));
    }

    SECTION("(0); () -> throw") {
        std::vector<double> point1 = {0};
        std::vector<double> point2 = {};

        auto distance_func = [&]() {
            return statistical::geometric_distance(point1, point2);
        };

        REQUIRE_THROWS_AS(distance_func(), std::invalid_argument);
    }
}
