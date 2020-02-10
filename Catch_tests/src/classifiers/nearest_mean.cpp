#include "../../catch.hpp"
#include "../../../src/classifiers/nearest_mean.hpp"

using classifiers::Labels;
using classifiers::NearestMean;


TEST_CASE("nearest_mean_test", "[nearest_mean]") {

    SECTION("classify -> OK") {
        typedef std::vector<std::vector<double>> Values;

        auto input = Matrix(3, 2);
        input.set(Values({
                                 {3, 9, 1},
                                 {0, 0, 0}
                         }));
        auto clusterA = Matrix(3, 4);
        clusterA.set(Values({
                                    {1, 2, 3},
                                    {1, 5, 3},
                                    {2, 5, 4},
                                    {4, 7, 5}
                            }));
        auto clusterB = Matrix(3, 4);
        clusterB.set(Values({
                                    {1, 7, 1},
                                    {3, 9, 1},
                                    {3, 8, 2},
                                    {5, 6, 1}
                            }));
        const auto labels = Labels({"A", "B"});

        NearestMean nm;
        const auto result = nm.classify(input, clusterA, clusterB, labels);

        const Labels expectedResult = {"B", "A"};
        REQUIRE_THAT(result, Catch::UnorderedEquals(expectedResult));
    }

}
