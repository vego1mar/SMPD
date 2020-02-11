#include "../../catch.hpp"
#include "../../../src/classifiers/nearest_mean.hpp"
#include "../../../src/helpers/stringify.hpp"

using classifiers::Labels;
using classifiers::NearestMean;
using helpers::Stringify;


TEST_CASE("nearest_mean_test", "[nearest_mean]") {

    typedef std::vector<std::vector<double>> Values;


    SECTION("classify -> OK") {
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

    SECTION("kMeans -> OK") {
        auto cluster = Matrix(11, 2);
        cluster.set(Values({
                                   {-7, -5, -6, -5, 2, 3, 8, 10, 0, 2,  5},
                                   {5,  3,  1,  -3, 2, 8, 5, 4,  1, -1, -2}
                           }));
        const auto labels = Labels({"A 1", "A 1", "A 1", "A 1", "B 2", "B 2", "B 2", "B 2", "C 3", "C 3", "A 2"});

        NearestMean nm;
        const auto result = nm.kMeans(cluster, labels);

        const auto toString = Stringify::toString(result);
        REQUIRE(toString.size() > 2);
        REQUIRE(result.size() == 3);

        for (const auto &inner : result) {
            REQUIRE(inner.second.size() == 2);
        }
    }

}
