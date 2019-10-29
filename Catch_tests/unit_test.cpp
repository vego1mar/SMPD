// #define CATCH_CONFIG_MAIN - don't need to repeat this include in more than one cpp file
#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "../src/statistical.hpp"

TEST_CASE("Arithmetic mean", "[statistical]") {
    SECTION("{1,2,3,4,5} -> 3") {
        std::vector<double> list = {1, 2, 3, 4, 5};
        auto mean = statistical::arithmetic_mean(list);
        CHECK(mean == Approx(3.0));
    }
}
