#include "../../catch.hpp"
#include "../../../src/helpers/stringify.hpp"

using helpers::Stringify;
using main_program::FLDHeader;
using selectors::IntVector;


TEST_CASE("stringify_test", "[stringify]") {

    SECTION("toString FLDHeader -> OK") {
        FLDHeader fldHeader("h1", "h2");
        const std::string expectedResult = "(h1,h2)";

        const auto result = Stringify::toString(fldHeader);

        REQUIRE_THAT(result, Catch::Equals(expectedResult));
    }

    SECTION("toString IntVector -> OK") {
        const auto source = IntVector({1, 2, 3, 4, 5, 6, 7, 8, 9});
        const std::string expectedResult = "[1,2,3,4,5,6,7,8,9]";

        const auto result = Stringify::toString(source);

        REQUIRE_THAT(result, Catch::Equals(expectedResult));
    }

}
