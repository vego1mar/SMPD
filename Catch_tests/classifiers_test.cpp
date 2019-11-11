#include "catch.hpp"
#include "../src/classifiers.hpp"

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
        classifiers::ClassificationResult results;
        results.setLabel("A");
        results.setFeatures(std::vector<double>({1, 2, 3}));
        results.setK(4);
        results.setAffiliation(12, 23);

        auto toString = results.toString();

        REQUIRE_THAT(toString, Catch::Equals("{A,[1.000000,2.000000,3.00000],4,{12/23,52.173913}}"));
    }
}
