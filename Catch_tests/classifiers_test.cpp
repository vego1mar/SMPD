#include "catch.hpp"
#include "../src/io_manager.hpp"
#include "../src/classifiers.hpp"

TEST_CASE("classifiers", "[classifiers]") {
    std::vector<casing::ClassVector> CLUSTER1 = {
            {"A", {1, 2, 3}},
            {"A", {2, 5, 3}},
            {"A", {2, 5, 4}},
            {"A", {4, 7, 5}},
            {"B", {1, 7, 1}},
            {"B", {3, 9, 1}},
            {"B", {3, 8, 2}},
            {"B", {5, 6, 1}},
    };
    std::vector<casing::ClassVector> CLUSTER2 = {
            {"A", {1, 2, 3}},
            {"A", {1, 5, 3}},
            {"A", {2, 5, 4}},
            {"A", {4, 7, 5}},
            {"B", {1, 7, 1}},
            {"B", {3, 9, 1}},
            {"B", {3, 8, 2}},
            {"B", {5, 6, 1}},
    };
    std::string PATH_VECTORS1 = "../../files/classes_vectors_1";
    std::string PATH_INPUT1 = "../../files/classes_unknowns_1";

//    SECTION("x=(1,7,3), cluster1, k=1 -> 'B'") {
//        std::vector<double> input = {1, 7, 3};
//        auto inputLabel = casing::nearest_neighbor(input, cluster1, 1);
//
//        REQUIRE_THAT(inputLabel, Catch::Equals("B"));
//    }
//
//    SECTION("x=(1,7,3), cluster1, k=3 -> 'A'") {
//        std::vector<double> input = {1, 7, 3};
//        auto inputLabel = casing::nearest_neighbor(input, cluster1, 3);
//
//        REQUIRE_THAT(inputLabel, Catch::Equals("A"));
//    }
//
//    SECTION("x=(2,5,2), cluster1, k=3 -> 'A'") {
//        std::vector<double> input = {2, 5, 2};
//        auto inputLabel = casing::nearest_neighbor(input, cluster1, 3);
//
//        REQUIRE_THAT(inputLabel, Catch::Equals("A"));
//    }
//
//    SECTION("x=(1,0,1), cluster1, k=0 -> throws") {
//        std::vector<double> x = {1, 0, 1};
//
//        auto nn_func = [&]() {
//            return casing::nearest_neighbor(x, cluster1, 0);
//        };
//
//        REQUIRE_THROWS_AS(nn_func(), std::invalid_argument);
//    }
//
//    SECTION("x=(1,0), cluster1, k=5 -> throws") {
//        std::vector<double> x = {1, 0};
//
//        auto nn_func = [&]() {
//            return casing::nearest_neighbor(x, cluster1, 5);
//        };
//
//        REQUIRE_THROWS_AS(nn_func(), std::invalid_argument);
//    }
//
//    SECTION("SuperCluster.read()  ->  OK") {
//        casing::SuperCluster clusterMaster;
//        bool isOK = true;
//
//        clusterMaster.read(const_cast<std::string &>(PATH_VECTORS1));
//
//        for (size_t i = 0; i < clusterMaster.getVectors().size(); i++) {
//            if (clusterMaster.getVectors()[i] != cluster1[i]) {
//                isOK = false;
//            }
//        }
//
//        REQUIRE(isOK);
//    }
//
//    SECTION("Affiliation.getters  ->  OK") {
//        casing::Affiliation affiliation(2, 3);
//
//        REQUIRE_THAT(affiliation.getFraction(), Catch::Equals("2/3"));
//        REQUIRE(affiliation.getPercent() == Approx(66.666).margin(3));
//    }
//
//    SECTION("NNResultSet.toString()  ->  OK") {
//        u_short k = 4;
//        std::string label = "A";
//        std::vector<double> input = {1, 2, 3};
//        casing::Affiliation affiliation(12, 23);
//        casing::NNResultSet results(label, input, k, affiliation);
//
//        auto toString = results.toString();
//
//        REQUIRE_THAT(toString, Catch::Equals("{A,[1.000000,2.000000,3.00000],4,{12/23,52.173913}}"));
//    }
//
//    SECTION("x=(1,7,3), cluster1, k=1 -> 'B' [2]") {
//        u_short k = 1;
//        std::vector<double> input = {1, 7, 3};
//        auto result = casing::nearest_neighbor_2(input, cluster1, k);
//        std::string expectedResult = "{B,[1.000000,7.000000,3.00000],1,{1/1,100.000000}}";
//
//        REQUIRE_THAT(result.getLabel(), Catch::Equals("B"));
//        REQUIRE_THAT(result.toString(), Catch::Equals(expectedResult));
//    }
//
//    SECTION("x=(1,7,3), cluster1, k=3 -> 'A' [2]") {
//        u_short k = 3;
//        std::vector<double> input = {1, 7, 3};
//        auto result = casing::nearest_neighbor_2(input, cluster1, k);
//        std::string expectedResult = "{A,[1.000000,7.000000,3.00000],3,{2/3,66.666667}}";
//
//        REQUIRE_THAT(result.getLabel(), Catch::Equals("A"));
//        REQUIRE_THAT(result.toString(), Catch::Equals(expectedResult));
//    }
//
//    SECTION("x=(2,5,2), cluster1, k=3 -> 'A' [2]") {
//        u_short k = 3;
//        std::vector<double> input = {2, 5, 2};
//        auto result = casing::nearest_neighbor_2(input, cluster1, k);
//        std::string expectedString = "{A,[2.000000,5.000000,2.00000],3,{2/3,66.666667}}";
//
//        REQUIRE_THAT(result.getLabel(), Catch::Equals("A"));
//        REQUIRE_THAT(result.toString(), Catch::Equals(expectedString));
//    }
//
//    SECTION("x=(1,0,1), cluster1, k=0 -> throws [2]") {
//        std::vector<double> x = {1, 0, 1};
//
//        auto nn_func = [&]() {
//            return casing::nearest_neighbor(x, cluster1, 0);
//        };
//
//        REQUIRE_THROWS_AS(nn_func(), std::invalid_argument);
//    }
//
//    SECTION("x=(1,0), cluster1, k=5 -> throws [2]") {
//        std::vector<double> x = {1, 0};
//
//        auto nn_func = [&]() {
//            return casing::nearest_neighbor(x, cluster1, 5);
//        };
//
//        REQUIRE_THROWS_AS(nn_func(), std::invalid_argument);
//    }
//
//    SECTION("SuperCluster.classify() -> emplace_back(classifiedByNN)") {
//        std::vector<double> point = {3, 3, 3};
//        u_short k = 5;
//        const std::string EXPECTED_STRING = "{A,[3.000000,3.000000,3.00000],5,{4/5,80.000000}}";
//        casing::SuperCluster bevy;
//        bevy.read(const_cast<std::string &>(PATH_VECTORS1));
//
//        bevy.classify(point, k);
//        auto results = bevy.getClassified();
//
//        REQUIRE(results.size() == 1);
//        REQUIRE_THAT(results[0].toString(), Catch::Equals(EXPECTED_STRING));
//    }
//
//    SECTION("nearest_neighbor(inputCluster, cluster, k) -> vector(NNResultSet)") {
//        casing::SuperCluster cloud;
//        cloud.read(const_cast<std::string &>(PATH_VECTORS1));
//        std::vector<casing::ClassVector> inputGroup{};
//        bool isOK = io_manager::readFileIntoCluster(PATH_INPUT1, inputGroup);
//        u_short k = 3;
//
//        cloud.classify(inputGroup, k);
//
//        REQUIRE(isOK);
//        REQUIRE(cloud.getClassified().size() == 2);
//    }
//
//    SECTION("SuperCluster.getSubCluster('A') -> outCluster('A')") {
//        casing::SuperCluster cloud;
//        cloud.read(const_cast<std::string &>(PATH_VECTORS1));
//        std::vector<casing::ClassVector> inputGroup{};
//        bool isOK = io_manager::readFileIntoCluster(PATH_INPUT1, inputGroup);
//        u_short k = 3;
//        std::vector<casing::ClassVector> expectedVectors;
//        expectedVectors = std::vector<casing::ClassVector>(cluster1.begin(), cluster1.begin() + 4);
//        expectedVectors.emplace_back("A", std::vector<double>({1, 7, 3}));
//        expectedVectors.emplace_back("A", std::vector<double>({2, 5, 2}));
//
//        cloud.classify(inputGroup, k);
//        std::vector<casing::ClassVector> outCluster = cloud.getSubCluster("A");
//
//        REQUIRE(isOK);
//        REQUIRE(cloud.getClassified().size() == 2);
//        REQUIRE_THAT(outCluster, Catch::UnorderedEquals(expectedVectors));
//    }
//
//    SECTION("SuperCluster.getSubClusters() -> outCluster[2][4]") {
//        casing::SuperCluster cloud;
//        cloud.read(const_cast<std::string &>(PATH_VECTORS1));
//        std::vector<casing::ClassVector> expectedGroupA;
//        std::vector<casing::ClassVector> expectedGroupB;
//        expectedGroupA = std::vector<casing::ClassVector>(cluster1.begin(), cluster1.begin() + 4);
//        expectedGroupB = std::vector<casing::ClassVector>(cluster1.begin() + 4, cluster1.end());
//
//        std::vector<std::vector<casing::ClassVector>> outCluster = cloud.getSubClusters();
//
//        REQUIRE(outCluster.size() == 2);
//        REQUIRE(outCluster[0].size() == 4);
//        REQUIRE(outCluster[1].size() == 4);
//        REQUIRE_THAT(outCluster[0], Catch::UnorderedEquals(expectedGroupA));
//        REQUIRE_THAT(outCluster[1], Catch::UnorderedEquals(expectedGroupB));
//    }

    SECTION("x=[2], CLUSTER1, k=5 -> [A,B]") {
        std::vector<casing::ClassVector> inputGroup = {
                casing::ClassVector("x", {2, 5, 2}),
                casing::ClassVector("x", {1, 7, 3})
        };
        std::string expectedString1 = "{A,[2.000000,5.000000,2.00000],5,{3/5,60.000000}}";
        std::string expectedString2 = "{B,[1.000000,7.000000,3.00000],5,{3/5,60.000000}}";

        auto resultSet = classifiers::nearest_neighbor(inputGroup, CLUSTER1, 5);

        REQUIRE(resultSet.size() == 2);
        REQUIRE_THAT(resultSet[0].toString(), Catch::Equals(expectedString1));
        REQUIRE_THAT(resultSet[1].toString(), Catch::Equals(expectedString2));
    }

    SECTION("nearest_mean([3,9,1], CLUSTER2) -> 'B'") {
        std::vector<double> input = {3, 9, 1};

        auto label = classifiers::nearest_mean(input, CLUSTER2);

        REQUIRE_THAT(label, Catch::Equals("B"));
    }
}
