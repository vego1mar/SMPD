#include <fstream>
#include "catch.hpp"
#include "../src/io_manager.hpp"
#include "../src/classifiers.hpp"

using casing::Cluster;
using casing::NNCluster;

TEST_CASE("io_manager", "[io_manager]") {
    std::string PATH_VECTORS1 = "../../files/classes_vectors_1";
    std::string PATH_RESULTS1 = "../../files/classes_results_1";
    std::string PATH_INPUT1 = "../../files/classes_unknowns_1";

    SECTION("readFileIntoCluster() -> 'classes_vectors_1' [8]") {
        Cluster cluster;
        std::vector<std::vector<double>> expectedFeatures = {
                {1, 2, 3},
                {2, 5, 3},
                {2, 5, 4},
                {4, 7, 5},
                {1, 7, 1},
                {3, 9, 1},
                {3, 8, 2},
                {5, 6, 1}
        };
        const size_t CLASSES_VECTORS_NO = 8;
        size_t i = 0;

        bool isOK = io_manager::readFileIntoCluster(PATH_VECTORS1, cluster);

        REQUIRE(isOK);
        REQUIRE(cluster.size() == CLASSES_VECTORS_NO);

        for (const auto &classVector : cluster) {
            REQUIRE_THAT(classVector.getLabel(), Catch::Equals("A") || Catch::Equals("B"));
            REQUIRE_THAT(classVector.getFeatures(), Catch::UnorderedEquals(expectedFeatures[i]));
            i++;
        }
    }

    SECTION("readFileToBuffer(char) -> 'classes_vectors_1' [8]") {
        std::string path = PATH_VECTORS1;
        std::vector<char> buffer;

        bool isOK = io_manager::readFileToBuffer(path, buffer);

        REQUIRE(isOK);
        REQUIRE(!buffer.empty());
        REQUIRE(buffer.size() == 192);
        REQUIRE(buffer.capacity() == buffer.size());
    }

    SECTION("readFileToBuffer(u_char) -> 'classes_vectors_1' [8]") {
        std::string path = PATH_VECTORS1;
        std::vector<u_char> buffer;

        bool isOK = io_manager::readFileToBuffer(path, buffer);

        REQUIRE(isOK);
        REQUIRE(!buffer.empty());
        REQUIRE(buffer.size() == 192);
        REQUIRE(buffer.capacity() == buffer.size());
    }

    SECTION("writeResultsIntoFile(nnResultSet) -> 100 B") {
        NNCluster nnCluster;
        nnCluster.read(PATH_VECTORS1);
        Cluster inputGroup;
        io_manager::readFileIntoCluster(PATH_INPUT1, inputGroup);
        nnCluster.classify(inputGroup, 3);
        auto nnResultSet = nnCluster.getClassified();

        bool isOK = io_manager::writeResultsIntoFile(PATH_RESULTS1, nnResultSet);
        long fileSize = io_manager::getFileSize(PATH_RESULTS1);

        REQUIRE(isOK);
        REQUIRE(fileSize == 100);
    }

    SECTION("writeResultsIntoFile(labels) -> 4 B") {
        Cluster inputGroup;
        Cluster baseCluster;
        io_manager::readFileIntoCluster(PATH_INPUT1, inputGroup);
        io_manager::readFileIntoCluster(PATH_VECTORS1, baseCluster);
        auto labels = classifiers::nearest_mean(inputGroup, baseCluster);

        bool isOK = io_manager::writeResultsIntoFile(PATH_RESULTS1, labels);
        long fileSize = io_manager::getFileSize(PATH_RESULTS1);

        REQUIRE(isOK);
        REQUIRE(fileSize == 4);
    }
}
