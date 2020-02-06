#include "../../catch.hpp"
#include "../../../src/classifiers/nearest_neighbors.hpp"

using classifiers::NearestNeighbors;
using classifiers::NearestNeighborScore;
using classifiers::Cluster;
using classifiers::Features;
using classifiers::Labels;
using classifiers::Input;


TEST_CASE("nearest_neighbors_test", "[nearest_neighbors]") {

    SECTION("classify -> OK") {
        const std::size_t NEIGHBORS_NO = 2;
        const Input input = {1, 2};
        auto cluster = std::make_unique<Cluster>();
        cluster->features = Features({
                                             {2, 5, 2},
                                             {1, 7, 3}
                                     });
        cluster->labels = Labels({"A1", "A2", "B1"});
        const std::string expectedResultStr = "{A2,1/2}";

        NearestNeighbors nn;
        const auto result = nn.classify(input, *cluster, NEIGHBORS_NO);

        REQUIRE_THAT(result.toString(), Catch::Equals(expectedResultStr));
    }

}
