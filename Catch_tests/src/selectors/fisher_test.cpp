#include "../../catch.hpp"
#include "../../../src/matrix/matrix.hpp"
#include "../../../src/selectors/fisher.hpp"

using fisher::FLD;
using matrix::Matrix;

TEST_CASE("fisher.hpp") {
    SECTION("selectOne(A,B) -> 1") {
        auto clusterA = Matrix(4, 3);
        clusterA.set(std::vector<std::vector<double>>{
                {2,  3, 1, 7},
                {1,  1, 2, 1},
                {-1, 2, 1, -3}
        });
        auto clusterB = Matrix(3, 3);
        clusterB.set(std::vector<std::vector<double>>{
                {4, 4, 5},
                {3, 2, 5},
                {0, 3, -2}
        });
        FLD fld;

        fld.selectOne(clusterA, clusterB);

        REQUIRE_THAT(fld.toString(), Catch::StartsWith("{FLD@"));
        REQUIRE(fld.getFeatureIndex() == 1);
    }

    SECTION("toString() -> as('FLD@XXX')") {
        auto fld1 = std::unique_ptr<FLD>();
        FLD fld2;

        auto str1 = fld1->toString();
        auto str2 = fld2.toString();

        REQUIRE(str1 != str2);
        REQUIRE_THAT(str1, Catch::StartsWith("{FLD@"));
        REQUIRE_THAT(str2, Catch::StartsWith("{FLD@"));
        REQUIRE_THAT(str1, Catch::EndsWith("}"));
        REQUIRE_THAT(str2, Catch::EndsWith("}"));
    }
}
