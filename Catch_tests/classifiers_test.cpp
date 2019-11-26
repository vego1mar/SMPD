#include "catch.hpp"
#include "../src/io_manager.hpp"
#include "../src/classifiers.hpp"

using casing::Cluster;
using casing::NNResultSet;
using casing::ClassVector;
using casing::NNCluster;

TEST_CASE("classifiers", "[classifiers]") {
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
    Cluster CLUSTER2 = {
            {"A", {1, 2, 3}},
            {"A", {1, 5, 3}},
            {"A", {2, 5, 4}},
            {"A", {4, 7, 5}},
            {"B", {1, 7, 1}},
            {"B", {3, 9, 1}},
            {"B", {3, 8, 2}},
            {"B", {5, 6, 1}},
    };
    std::string PATH_VECTORS_1 = "../../files/classes_vectors_1";
    std::string PATH_VECTORS_2 = "../../files/classes_vectors_2";
    std::string PATH_INPUT_1 = "../../files/classes_unknowns_1";

    SECTION("x=(1,7,3), CLUSTER1, k=1 -> 'B'") {
        std::vector<double> input = {1, 7, 3};
        auto inputLabel = classifiers::nearest_neighbor(input, CLUSTER1, 1);

        REQUIRE_THAT(inputLabel, Catch::Equals("B"));
    }

    SECTION("x=(1,7,3), CLUSTER1, k=3 -> 'A'") {
        std::vector<double> input = {1, 7, 3};
        auto inputLabel = classifiers::nearest_neighbor(input, CLUSTER1, 3);

        REQUIRE_THAT(inputLabel, Catch::Equals("A"));
    }

    SECTION("x=(2,5,2), CLUSTER1, k=3 -> 'A'") {
        std::vector<double> input = {2, 5, 2};
        auto inputLabel = classifiers::nearest_neighbor(input, CLUSTER1, 3);

        REQUIRE_THAT(inputLabel, Catch::Equals("A"));
    }

    SECTION("x=(1,0,1), CLUSTER1, k=0 -> throws") {
        std::vector<double> x = {1, 0, 1};

        auto nn_func = [&]() {
            return classifiers::nearest_neighbor(x, CLUSTER1, 0);
        };

        REQUIRE_THROWS_AS(nn_func(), std::invalid_argument);
    }

    SECTION("x=(1,0), CLUSTER1, k=5 -> throws") {
        std::vector<double> x = {1, 0};

        auto nn_func = [&]() {
            return classifiers::nearest_neighbor(x, CLUSTER1, 5);
        };

        REQUIRE_THROWS_AS(nn_func(), std::invalid_argument);
    }

    SECTION("x=(1,7,3), CLUSTER1, k=1 -> 'B' [2]") {
        u_short k = 1;
        std::vector<double> input = {1, 7, 3};
        auto result = classifiers::nearest_neighbor_2(input, CLUSTER1, k);
        std::string expectedResult = "{B,[1.000000,7.000000,3.00000],1,{1/1,100.000000}}";

        REQUIRE_THAT(result.getLabel(), Catch::Equals("B"));
        REQUIRE_THAT(result.toString(), Catch::Equals(expectedResult));
    }

    SECTION("x=(1,7,3), CLUSTER1, k=3 -> 'A' [2]") {
        u_short k = 3;
        std::vector<double> input = {1, 7, 3};
        auto result = classifiers::nearest_neighbor_2(input, CLUSTER1, k);
        std::string expectedResult = "{A,[1.000000,7.000000,3.00000],3,{2/3,66.666667}}";

        REQUIRE_THAT(result.getLabel(), Catch::Equals("A"));
        REQUIRE_THAT(result.toString(), Catch::Equals(expectedResult));
    }

    SECTION("x=(2,5,2), CLUSTER1, k=3 -> 'A' [2]") {
        u_short k = 3;
        std::vector<double> input = {2, 5, 2};
        auto result = classifiers::nearest_neighbor_2(input, CLUSTER1, k);
        std::string expectedString = "{A,[2.000000,5.000000,2.00000],3,{2/3,66.666667}}";

        REQUIRE_THAT(result.getLabel(), Catch::Equals("A"));
        REQUIRE_THAT(result.toString(), Catch::Equals(expectedString));
    }

    SECTION("x=(1,0,1), CLUSTER1, k=0 -> throws [2]") {
        std::vector<double> x = {1, 0, 1};

        auto nn_func = [&]() {
            return classifiers::nearest_neighbor(x, CLUSTER1, 0);
        };

        REQUIRE_THROWS_AS(nn_func(), std::invalid_argument);
    }

    SECTION("x=(1,0), CLUSTER1, k=5 -> throws [2]") {
        std::vector<double> x = {1, 0};

        auto nn_func = [&]() {
            return classifiers::nearest_neighbor(x, CLUSTER1, 5);
        };

        REQUIRE_THROWS_AS(nn_func(), std::invalid_argument);
    }

    SECTION("nearest_neighbor(CLUSTER_FILE, k) -> vector(NNResultSet)") {
        NNCluster cloud;
        cloud.read(PATH_VECTORS_1);
        Cluster inputGroup{};
        bool isOK = io_manager::readFileIntoCluster(PATH_INPUT_1, inputGroup);
        u_short k = 3;

        cloud.classify(inputGroup, k);

        REQUIRE(isOK);
        REQUIRE(cloud.getClassified().size() == 2);
    }

    SECTION("x=[2], CLUSTER1, k=5 -> [A,B]") {
        Cluster inputGroup = {
                ClassVector("x", {2, 5, 2}),
                ClassVector("x", {1, 7, 3})
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

    SECTION("nearest_neighbor(INPUT_FILE, CLUSTER_FILE, k=3) -> [A,A]") {
        NNCluster cloud;
        cloud.read(PATH_VECTORS_1);
        Cluster inputGroup{};
        bool isOK = io_manager::readFileIntoCluster(PATH_INPUT_1, inputGroup);
        u_short k = 3;
        std::vector<std::string> expected = {
                "{A,[1.000000,7.000000,3.00000],3,{2/3,66.666667}}",
                "{A,[2.000000,5.000000,2.00000],3,{2/3,66.666667}}"
        };
        int i = 0;

        cloud.classify(inputGroup, k);

        REQUIRE(isOK);
        REQUIRE(cloud.getClassified().size() == 2);

        for (const auto &item : cloud.getClassified()) {
            REQUIRE_THAT(item.getLabel(), Catch::Equals("A"));
            REQUIRE_THAT(item.toString(), Catch::Equals(expected[i]));
            i++;
        }
    }

    SECTION("nearest_mean(INPUT_FILE, CLUSTER_FILE) -> [A,A]") {
        Cluster inputGroup;
        Cluster baseCluster;
        bool isInputGroupOK = io_manager::readFileIntoCluster(PATH_INPUT_1, inputGroup);
        bool isBaseClusterOK = io_manager::readFileIntoCluster(PATH_VECTORS_1, baseCluster);
        std::vector<std::string> expected = {"A", "A"};

        auto labels = classifiers::nearest_mean(inputGroup, baseCluster);

        REQUIRE(isInputGroupOK);
        REQUIRE(isBaseClusterOK);
        REQUIRE(labels.size() == 2);
        REQUIRE_THAT(labels, Catch::UnorderedEquals(expected));
    }

    SECTION("k_means(CLUSTER_FILE_2, k=3) -> [k][split]") {
        Cluster base;
        io_manager::readFileIntoCluster(PATH_VECTORS_2, base);
        u_short k = 3;
        const u_long MAX_ITER = 10'000;
        int pointsNo = 0;
        auto pointsNoFunc = [&pointsNo](Cluster &c) { pointsNo += c.size(); };

        auto result = classifiers::k_means(base, k, MAX_ITER);

        REQUIRE(!result.empty());
        REQUIRE(result.size() == k);
        std::for_each(result.begin(), result.end(), pointsNoFunc);
        REQUIRE(pointsNo == base.size());
    }
}
