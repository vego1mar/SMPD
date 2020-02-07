#include "../../catch.hpp"
#include "../../../src/classifiers/nearest_neighbors.hpp"

using classifiers::NearestNeighbors;
using classifiers::NearestNeighborScore;
using classifiers::Cluster;
using classifiers::NearestNeighborScores;
using classifiers::SuperCluster;
using classifiers::Features;
using classifiers::Labels;
using classifiers::Input;


TEST_CASE("nearest_neighbors_test", "[nearest_neighbors]") {

    SECTION("classify input -> OK") {
        const std::size_t NEIGHBORS_NO = 2;
        const Input input = {1, 2};
        auto cluster = std::make_unique<Cluster>();
        cluster->features = Features({
                                             {2, 5, 2},
                                             {1, 7, 3}
                                     });
        cluster->labels = Labels({"A1", "A2", "B1"});
        const std::string expectedResultStr = "{B1,1/2}";

        NearestNeighbors nn;
        const auto result = nn.classify(input, *cluster, NEIGHBORS_NO);

        REQUIRE_THAT(result.toString(), Catch::Equals(expectedResultStr));
    }

    SECTION("classify cluster -> OK") {
        const std::size_t NEIGHBORS_NO = 2;
        auto input = std::make_unique<Cluster>();
        input->features = Features(
                {{1, 2},
                 {4, 4}}
        );
        auto cluster = std::make_unique<Cluster>();
        cluster->features = Features({
                                             {2, 5, 2},
                                             {1, 7, 3}
                                     });
        cluster->labels = Labels({"A1", "A2", "B1"});
        const std::vector<std::string> expectedResultStrings = {"{B1,1/2}", "{A2,1/2}"};

        NearestNeighbors nn;
        const auto result = nn.classify(*input, *cluster, NEIGHBORS_NO);

        auto results = std::vector<std::string>();
        std::for_each(result.begin(), result.end(), [&results](const NearestNeighborScore &score) {
            results.emplace_back(score.toString());
        });

        REQUIRE_THAT(results, Catch::UnorderedEquals(expectedResultStrings));
    }

    SECTION("classify superCluster -> OK") {
        const std::vector<std::size_t> neighborsNo = {1, 3};
        auto input = Cluster();
        input.features = Features(
                {{1, 2},
                 {7, 5},
                 {3, 2}}
        );
        auto superCluster = SuperCluster();
        auto clusterA = Cluster();
        clusterA.features = Features({
                                             {1, 2, 2, 4},
                                             {2, 5, 5, 7},
                                             {3, 3, 4, 5}
                                     });
        clusterA.labels = Labels({"A 1", "A 2", "A 3", "A 4"});
        auto clusterB = Cluster();
        clusterB.features = Features({
                                             {1, 3, 3, 5},
                                             {7, 9, 8, 6},
                                             {1, 1, 2, 1}
                                     });
        clusterB.labels = Labels({"B 1", "B 2", "B 3", "B 4"});
        superCluster = {clusterA, clusterB};
        const std::vector<std::string> expectedResultStrings = {"{B}", "{A}"};


        NearestNeighbors nn;
        auto results = std::vector<std::string>();

        for (const auto &neighbors : neighborsNo) {
            const auto result = nn.classify(input, superCluster, neighbors);

            std::for_each(result.begin(), result.end(), [&results](const NearestNeighborScore &score) {
                results.emplace_back(score.toString());
            });
        }

        REQUIRE_THAT(results, Catch::UnorderedEquals(expectedResultStrings));
    }

}
