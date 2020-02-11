#include "../../catch.hpp"
#include "../../../src/helpers/stringify.hpp"

using helpers::Stringify;
using main_program::FLDHeader;
using selectors::IntVector;
using classifiers::NearestNeighborScores;
using classifiers::NearestNeighborScore;
using classifiers::Centroids;


TEST_CASE("stringify_test", "[stringify]") {

    SECTION("FLDHeader -> OK") {
        FLDHeader fldHeader("h1", "h2");
        const std::string expectedResult = "(h1,h2)";

        const auto result = Stringify::toString(fldHeader);

        REQUIRE_THAT(result, Catch::Equals(expectedResult));
    }

    SECTION("IntVector -> OK") {
        const auto source = IntVector({1, 2, 3, 4, 5, 6, 7, 8, 9});
        const std::string expectedResult = "[1,2,3,4,5,6,7,8,9]";

        const auto result = Stringify::toString(source);

        REQUIRE_THAT(result, Catch::Equals(expectedResult));
    }

    SECTION("NearestNeighborScores -> OK") {
        const auto score1 = NearestNeighborScore("Acer", 1, 1);
        const auto score2 = NearestNeighborScore("Quercus", 2, 3);
        const auto score3 = NearestNeighborScore("Gilgamesz", 3, 5);
        const auto scores = NearestNeighborScores({score1, score2, score3});
        const std::string expectedStr = "[{Acer,1/1},{Quercus,2/3},{Gilgamesz,3/5}]";

        const auto result = Stringify::toString(scores);

        REQUIRE_THAT(result, Catch::Equals(expectedStr));
    }

    SECTION("Centroids -> OK") {
        const auto centroids = Centroids({
                                                 {"label4", {std::sqrt(2.0), 1.0 / 11.0}},
                                                 {"label3", {}},
                                                 {"label2", {0.3,            1.44, 2.77262}},
                                                 {"label1", {0,              1,    2}},
                                         });
        const std::string expectedResult = "[(label1,[0,1,2]),(label2,[0.3,1.44,2.77]),(label3,[]),(label4,[1.41,0.0909])]";

        const auto result = Stringify::toString(centroids);

        REQUIRE_THAT(result, Catch::Equals(expectedResult));
    }

}
