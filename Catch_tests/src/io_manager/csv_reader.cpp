#include "../../catch.hpp"
#include "../../../src/io_manager/csv_reader.hpp"
#include "../../../src/test/constants.hpp"

using io_manager::CSVReader;
using io_manager::CSVFile;
using test::Constants;


TEST_CASE("csv_reader", "[csv_reader]") {
    Constants consts;

    SECTION("read MapleOak.csv -> OK") {
        CSVReader reader(consts.PATH_MAPLE_OAK);

        reader.read();
        CSVFile content = reader.getContent();
        reader.dispose();

        REQUIRE(content.size() == 123);
    }
}
