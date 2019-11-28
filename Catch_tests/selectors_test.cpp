#include "catch.hpp"
#include "../src/casing.hpp"
#include "../src/selectors.hpp"
#include "../src/io_manager.hpp"

using casing::Cluster;
using casing::ClassVector;

TEST_CASE("selectors.hpp", "[selectors]") {
    Cluster CLUSTER_A = {
            ClassVector("A", {1, 2, -1}),
            ClassVector("A", {1, 3, 2}),
            ClassVector("A", {2, 1, 1}),
            ClassVector("A", {1, 7, -3}),
    };
    Cluster CLUSTER_B = {
            ClassVector("B", {3, 4, 0}),
            ClassVector("B", {2, 4, 3}),
            ClassVector("B", {5, 5, -2})
    };
    std::string PATH_VECTORS_3 = "../../files/classes_vectors_3";
    std::string PATH_RESULTS_1 = "../../files/classes_results_1";

    SECTION("fisher_coefficients(A,B) -> [-2.56,0.60,-4.34],[1]") {
        std::vector<double> expectedResult1 = {
                std::abs((5.0 / 4.0) - (10.0 / 3.0)) / ((std::sqrt(12.0) / 8.0) - (std::sqrt(14.0) / 3.0)),
                std::abs((13.0 / 4.0) - (13.0 / 3.0)) / ((std::sqrt(83.0) / 4.0) - (std::sqrt(2.0) / 3.0)),
                std::abs((-1.0 / 4.0) - (1.0 / 3.0)) / ((std::sqrt(59.0) / 4.0) - (std::sqrt(114.0 / 27.0)))
        };
        double expectedResult2 = expectedResult1[1];

        auto result1 = selectors::fisher_coefficients(CLUSTER_A, CLUSTER_B);
        auto result2 = selectors::fisher_coefficient(CLUSTER_A, CLUSTER_B);

        REQUIRE_THAT(result1, Catch::UnorderedEquals(expectedResult1));
        REQUIRE(result2 == Approx(expectedResult2));
    }

    SECTION("fisher_coefficient(A,B) -> 0.60") {
        double expectedResult = std::abs((13.0 / 4.0) - (13.0 / 3.0)) /
                                ((std::sqrt(83.0) / 4.0) - (std::sqrt(2.0) / 3.0));
        Cluster cluster;
        bool isOK1 = io_manager::readFileIntoCluster(PATH_VECTORS_3, cluster);
        auto result = selectors::fisher_coefficient(CLUSTER_A, CLUSTER_B);
        std::vector<std::string> output = {std::to_string(result)};
        bool isOK2 = io_manager::writeResultsIntoFile(PATH_RESULTS_1, output);

        REQUIRE(isOK1);
        REQUIRE(result == Approx(expectedResult));
        REQUIRE(isOK2);
        REQUIRE(io_manager::getFileSize(PATH_RESULTS_1) == 9);
    }
}
