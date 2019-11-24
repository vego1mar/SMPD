#include "catch.hpp"
#include "../src/io_manager.hpp"
#include "../src/classifiers.hpp"

TEST_CASE("casing", "[casing]") {
    Cluster CLUSTER1 = {
            {"A", {1, 2, 3}},
            {"A", {2, 5, 3}},
            {"A", {2, 5, 4}},
            {"A", {4, 7, 5}},
            {"B", {1, 7, 1}},
            {"B", {3, 9, 1}},
            {"B", {3, 8, 2}},
            {"B", {5, 6, 1}},
    };
    std::string PATH_VECTORS1 = "../../files/classes_vectors_1";
    std::string PATH_INPUT1 = "../../files/classes_unknowns_1";

    SECTION("NNCluster.read()  ->  OK") {
        NNCluster nnCluster;
        bool isOK = true;

        nnCluster.read(PATH_VECTORS1);

        for (size_t i = 0; i < nnCluster.getVectors().size(); i++) {
            if (nnCluster.getVectors()[i] != CLUSTER1[i]) {
                isOK = false;
            }
        }

        REQUIRE(isOK);
    }

    SECTION("Affiliation.getters  ->  OK") {
        Affiliation affiliation(2, 3);

        REQUIRE_THAT(affiliation.getFraction(), Catch::Equals("2/3"));
        REQUIRE(affiliation.getPercent() == Approx(66.666).margin(3));
    }

    SECTION("NNResultSet.toString()  ->  OK") {
        u_short k = 4;
        std::string label = "A";
        std::vector<double> input = {1, 2, 3};
        Affiliation affiliation(12, 23);
        NNResultSet results(label, input, k, affiliation);

        auto toString = results.toString();

        REQUIRE_THAT(toString, Catch::Equals("{A,[1.000000,2.000000,3.00000],4,{12/23,52.173913}}"));
    }

    SECTION("NNCluster.classify() -> emplace_back(classifiedByNN)") {
        std::vector<double> point = {3, 3, 3};
        u_short k = 5;
        const std::string EXPECTED_STRING = "{A,[3.000000,3.000000,3.00000],5,{4/5,80.000000}}";
        NNCluster bevy;
        bevy.read(PATH_VECTORS1);

        bevy.classify(point, k);
        auto results = bevy.getClassified();

        REQUIRE(results.size() == 1);
        REQUIRE_THAT(results[0].toString(), Catch::Equals(EXPECTED_STRING));
    }
    SECTION("NNCluster.getSubCluster('A') -> outCluster('A')") {
        NNCluster cloud;
        cloud.read(PATH_VECTORS1);
        Cluster inputGroup;
        bool isOK = io_manager::readFileIntoCluster(PATH_INPUT1, inputGroup);
        u_short k = 3;
        Cluster expectedVectors;
        expectedVectors = Cluster(CLUSTER1.begin(), CLUSTER1.begin() + 4);
        expectedVectors.emplace_back("A", std::vector<double>({1, 7, 3}));
        expectedVectors.emplace_back("A", std::vector<double>({2, 5, 2}));

        cloud.classify(inputGroup, k);
        auto outCluster = cloud.getSubCluster("A");

        REQUIRE(isOK);
        REQUIRE(cloud.getClassified().size() == 2);
        REQUIRE_THAT(outCluster, Catch::UnorderedEquals(expectedVectors));
    }

    SECTION("NNCluster.getSubClusters() -> outCluster[2][4]") {
        NNCluster cloud;
        cloud.read(PATH_VECTORS1);
        Cluster expectedGroupA;
        Cluster expectedGroupB;
        expectedGroupA = Cluster(CLUSTER1.begin(), CLUSTER1.begin() + 4);
        expectedGroupB = Cluster(CLUSTER1.begin() + 4, CLUSTER1.end());

        auto outCluster = cloud.getSubClusters();

        REQUIRE(outCluster.size() == 2);
        REQUIRE(outCluster[0].size() == 4);
        REQUIRE(outCluster[1].size() == 4);
        REQUIRE_THAT(outCluster[0], Catch::UnorderedEquals(expectedGroupA));
        REQUIRE_THAT(outCluster[1], Catch::UnorderedEquals(expectedGroupB));
    }
}
