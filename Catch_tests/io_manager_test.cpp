#include "catch.hpp"
#include "../src/io_manager.hpp"

TEST_CASE("readFileIntoCluster", "[io_manager]") {
    const std::string PATH_VECTORS1 = "../../files/classes_vectors_1";

    SECTION("readFileIntoCluster() -> 'classes_vectors_1' [8]") {
        // GIVEN
        std::vector<casing::ClassVector> cluster;
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

        // WHEN
        bool isOK = io_manager::readFileIntoCluster(const_cast<std::string &>(PATH_VECTORS1), cluster);

        // THEN
        REQUIRE(isOK);
        REQUIRE(cluster.size() == CLASSES_VECTORS_NO);

        for (const auto &classVector : cluster) {
            REQUIRE_THAT(classVector.getLabel(), Catch::Equals("A") || Catch::Equals("B"));
            REQUIRE_THAT(classVector.getFeatures(), Catch::UnorderedEquals(expectedFeatures[i]));
            i++;
        }
    }

    SECTION("readFileToBuffer(char) -> 'classes_vectors_1' [8]") {
        // GIVEN
        std::string path = const_cast<std::string &>(PATH_VECTORS1);
        std::vector<char> buffer;

        // WHEN
        bool isOK = io_manager::readFileToBuffer(path, buffer);

        // THEN
        REQUIRE(isOK);
        REQUIRE(!buffer.empty());
        REQUIRE(buffer.size() == 192);
        REQUIRE(buffer.capacity() == buffer.size());
    }

    SECTION("readFileToBuffer(u_char) -> 'classes_vectors_1' [8]") {
        // GIVEN
        std::string path = const_cast<std::string &>(PATH_VECTORS1);
        std::vector<u_char> buffer;

        // WHEN
        bool isOK = io_manager::readFileToBuffer(path, buffer);

        // THEN
        REQUIRE(isOK);
        REQUIRE(!buffer.empty());
        REQUIRE(buffer.size() == 192);
        REQUIRE(buffer.capacity() == buffer.size());
    }
}
