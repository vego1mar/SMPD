#include "catch.hpp"
#include "../src/classifiers.hpp"
#include "../src/io_manager.hpp"

TEST_CASE("classifiers", "[classifiers]") {
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
    const std::string PATH_VECTORS1 = "../../files/classes_vectors_1";
    std::string PATH_INPUT1 = "../../files/classes_unknowns_1";

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

    SECTION("Cluster.read()  ->  OK") {
        classifiers::Cluster clusterMaster;
        bool isOK = true;

        clusterMaster.read(const_cast<std::string &>(PATH_VECTORS1));

        for (size_t i = 0; i < clusterMaster.getVectors().size(); i++) {
            if (clusterMaster.getVectors()[i] != cluster1[i]) {
                isOK = false;
            }
        }

        REQUIRE(isOK);
    }

    SECTION("Affiliation.getters  ->  OK") {
        classifiers::Affiliation affiliation(2, 3);

        REQUIRE_THAT(affiliation.getFraction(), Catch::Equals("2/3"));
        REQUIRE(affiliation.getPercent() == Approx(66.666).margin(3));
    }

    SECTION("ClassificationResult.toString()  ->  OK") {
        u_short k = 4;
        std::string label = "A";
        std::vector<double> input = {1, 2, 3};
        classifiers::Affiliation affiliation(12, 23);
        classifiers::ClassificationResult results(label, input, k, affiliation);

        auto toString = results.toString();

        REQUIRE_THAT(toString, Catch::Equals("{A,[1.000000,2.000000,3.00000],4,{12/23,52.173913}}"));
    }

    SECTION("x=(1,7,3), cluster1, k=1 -> 'B' [2]") {
        u_short k = 1;
        std::vector<double> input = {1, 7, 3};
        auto result = classifiers::nearest_neighbor_2(input, cluster1, k);
        std::string expectedResult = "{B,[1.000000,7.000000,3.00000],1,{1/1,100.000000}}";

        REQUIRE_THAT(result.getLabel(), Catch::Equals("B"));
        REQUIRE_THAT(result.toString(), Catch::Equals(expectedResult));
    }

    SECTION("x=(1,7,3), cluster1, k=3 -> 'A' [2]") {
        u_short k = 3;
        std::vector<double> input = {1, 7, 3};
        auto result = classifiers::nearest_neighbor_2(input, cluster1, k);
        std::string expectedResult = "{A,[1.000000,7.000000,3.00000],3,{2/3,66.666667}}";

        REQUIRE_THAT(result.getLabel(), Catch::Equals("A"));
        REQUIRE_THAT(result.toString(), Catch::Equals(expectedResult));
    }

    SECTION("x=(2,5,2), cluster1, k=3 -> 'A' [2]") {
        u_short k = 3;
        std::vector<double> input = {2, 5, 2};
        auto result = classifiers::nearest_neighbor_2(input, cluster1, k);
        std::string expectedString = "{A,[2.000000,5.000000,2.00000],3,{2/3,66.666667}}";

        REQUIRE_THAT(result.getLabel(), Catch::Equals("A"));
        REQUIRE_THAT(result.toString(), Catch::Equals(expectedString));
    }

    SECTION("x=(1,0,1), cluster1, k=0 -> throws [2]") {
        std::vector<double> x = {1, 0, 1};

        auto nn_func = [&]() {
            return classifiers::nearest_neighbor(x, cluster1, 0);
        };

        REQUIRE_THROWS_AS(nn_func(), std::invalid_argument);
    }

    SECTION("x=(1,0), cluster1, k=5 -> throws [2]") {
        std::vector<double> x = {1, 0};

        auto nn_func = [&]() {
            return classifiers::nearest_neighbor(x, cluster1, 5);
        };

        REQUIRE_THROWS_AS(nn_func(), std::invalid_argument);
    }

    SECTION("Cluster.classify() -> emplace_back(classified)") {
        std::vector<double> point = {3, 3, 3};
        u_short k = 5;
        const std::string EXPECTED_STRING = "{A,[3.000000,3.000000,3.00000],5,{4/5,80.000000}}";
        classifiers::Cluster bevy;
        bevy.read(const_cast<std::string &>(PATH_VECTORS1));

        bevy.classify(point, k);
        auto results = bevy.getClassified();

        REQUIRE(results.size() == 1);
        REQUIRE_THAT(results[0].toString(), Catch::Equals(EXPECTED_STRING));
    }

    SECTION("nearest_neighbor(inputCluster, cluster, k) -> vector(ClassificationResult)") {
        classifiers::Cluster cloud;
        cloud.read(const_cast<std::string &>(PATH_VECTORS1));
        std::vector<classifiers::ClassVector> inputGroup{};
        bool isOK = io_manager::readFileIntoCluster(PATH_INPUT1, inputGroup);
        u_short k = 3;

        cloud.classify(inputGroup, k);

        REQUIRE(isOK);
        REQUIRE(cloud.getClassified().size() == 2);
    }
}
