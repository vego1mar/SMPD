#include "../../catch.hpp"
#include "../../../src/data_builders/csv_parser.hpp"
#include "../../../src/test/constants.hpp"

using data_builders::CSVParser;
using data_builders::CSVDataFile;
using data_builders::Headers;
using test::Constants;


TEST_CASE("csv_parser", "[csv_parser_test]") {
    const Constants vars;


    SECTION("build MapleOak -> OK") {
        CSVParser parser(vars.MAPLE_OAK_PATH);

        parser.build();
        const CSVDataFile &data = parser.getData();
        const Headers &headers = parser.getHeaders();

        REQUIRE(data.size() == vars.MAPLE_OAK_LINES);
        REQUIRE(headers.size() == vars.MAPLE_OAK_LINES);

        for (const auto &line : data) {
            REQUIRE(line.size() == vars.MAPLE_OAK_FEATURES);
        }

        parser.dispose();
    }

}
