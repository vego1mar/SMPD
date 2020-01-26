#include "../../catch.hpp"
#include "../../../src/data_builders/csv_parser.hpp"
#include "../../../src/test/constants.hpp"
#include "../../../src/matrix/matrix.hpp"

using data_builders::CSVParser;
using data_builders::CSVDataFile;
using data_builders::Headers;
using test::Constants;
using matrix::Matrix;


TEST_CASE("csv_parser", "[csv_parser_test]") {
    const Constants vars;


    SECTION("build MapleOak -> OK") {
        CSVParser parser(vars.MAPLE_OAK_PATH);

        parser.build();
        const Matrix &dataset = parser.getDataset();
        const Headers &headers = parser.getHeaders();

        REQUIRE(dataset.getRows() == vars.MAPLE_OAK_LINES);
        REQUIRE(headers.size() == vars.MAPLE_OAK_LINES);
        REQUIRE(dataset.getColumns() == vars.MAPLE_OAK_FEATURES);

        parser.dispose();
    }

}
