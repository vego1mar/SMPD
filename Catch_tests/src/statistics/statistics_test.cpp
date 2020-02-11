#include "../../catch.hpp"
#include "../../../src/statistics/statistics.hpp"

using statistics::Statistics;
using statistics::Means;


TEST_CASE("statistics_test", "[statistics]") {

    SECTION("geometricCloseness1 -> OK") {
        const auto means1 = Means({
                                          {1,         -2,         3.0 + (1.0 / 11.0)},
                                          {1.0 / 3.0, 13.0 / 2.0, std::sqrt(2.0)}
                                  });
        const auto means2 = Means({
                                          {0,              0,              0},
                                          {std::sqrt(3.0), std::sqrt(3.0), std::sqrt(3.0)}
                                  });

        const auto result = Statistics::geometricCloseness(means1, means2);

        const auto expectedResult = 4.3969833905579101;
        REQUIRE_THAT(result, Catch::WithinAbs(expectedResult, 0.01));
    }

    SECTION("geometricCloseness2 -> OK") {
        const auto means1 = Means({
                                          {1.0 / 2.0, 1.0 / 3.0, 1.0 / 5.0},
                                  });
        const auto means2 = Means({
                                          {0, 0, 0},
                                  });

        const auto result = Statistics::geometricCloseness(means1, means2);

        const auto expectedResult = 0.6333333333333333;
        REQUIRE_THAT(result, Catch::WithinAbs(expectedResult, 0.01));
    }

}
