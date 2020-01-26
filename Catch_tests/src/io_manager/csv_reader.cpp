#include "../../catch.hpp"
#include "../../../src/io_manager/csv_reader.hpp"
#include "../../../src/test/constants.hpp"

using io_manager::CSVReader;
using io_manager::CSVFile;
using test::Constants;


TEST_CASE("csv_reader", "[csv_reader]") {
    const Constants vars;


    SECTION("read MapleOak.csv -> OK") {
        CSVReader reader(vars.MAPLE_OAK_PATH);

        reader.read();
        CSVFile content = reader.getContent();
        reader.dispose();

        REQUIRE(content.size() == vars.MAPLE_OAK_LINES);

        for (const auto &line : content) {
            REQUIRE(line.size() == vars.MAPLE_OAK_COLUMNS);
        }
    }

}
