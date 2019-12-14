#include "../../catch.hpp"
#include "../../../src/matrix/matrix.hpp"

using matrix::Matrix;
using matrix::TransformationType;

TEST_CASE("matrix.hpp") {
    SECTION("ctor(W,H) -> ones(W,H)") {
        std::size_t columns = 3;
        std::size_t rows = 4;
        std::vector<double> expectedRows = {1, 1, 1};
        std::vector<double> expectedColumns = {1, 1, 1, 1};

        Matrix matrix(columns, rows);

        REQUIRE(matrix.getColumns() == columns);
        REQUIRE(matrix.getRows() == rows);
        REQUIRE(matrix.getSize() == columns * rows);
        REQUIRE(matrix.getRow(0).size() == expectedRows.size());
        REQUIRE(matrix.getRow(1).size() == expectedRows.size());
        REQUIRE(matrix.getRow(2).size() == expectedRows.size());
        REQUIRE(matrix.getRow(3).size() == expectedRows.size());
        REQUIRE(matrix.getColumn(0).size() == expectedColumns.size());
        REQUIRE(matrix.getColumn(1).size() == expectedColumns.size());
        REQUIRE(matrix.getColumn(2).size() == expectedColumns.size());
        REQUIRE(matrix.getRow(0) == expectedRows);
        REQUIRE(matrix.getRow(1) == expectedRows);
        REQUIRE(matrix.getRow(2) == expectedRows);
        REQUIRE(matrix.getRow(3) == expectedRows);
        REQUIRE(matrix.getColumn(0) == expectedColumns);
        REQUIRE(matrix.getColumn(1) == expectedColumns);
        REQUIRE(matrix.getColumn(2) == expectedColumns);
    }

    SECTION("4x4 Identity -> row/column OK") {
        std::size_t columns = 4;
        std::size_t rows = 4;
        std::vector<double> expectedRow0 = {1, 0, 0, 0};
        std::vector<double> expectedRow1 = {0, 1, 0, 0};
        std::vector<double> expectedRow2 = {0, 0, 1, 0};
        std::vector<double> expectedRow3 = {0, 0, 0, 1};

        Matrix matrix(columns, rows);
        matrix.transform(TransformationType::Identity);

        REQUIRE(matrix.getColumns() == columns);
        REQUIRE(matrix.getRows() == rows);
        REQUIRE(matrix.getSize() == columns * rows);
        REQUIRE(matrix.getRow(0) == expectedRow0);
        REQUIRE(matrix.getRow(1) == expectedRow1);
        REQUIRE(matrix.getRow(2) == expectedRow2);
        REQUIRE(matrix.getRow(3) == expectedRow3);
        REQUIRE(matrix.getColumn(0) == expectedRow0);
        REQUIRE(matrix.getColumn(1) == expectedRow1);
        REQUIRE(matrix.getColumn(2) == expectedRow2);
        REQUIRE(matrix.getColumn(3) == expectedRow3);
    }

    SECTION("set(V) -> getRow/getColumn : V, OK") {
        std::size_t columns1 = 2;
        std::size_t rows1 = 3;
        Matrix matrix1(columns1, rows1);
        std::vector<double> values1 = {1, 2, 3, 4, 5, 6};
        std::size_t columns2 = 3;
        std::size_t rows2 = 2;
        Matrix matrix2(columns2, rows2);
        std::vector<std::vector<double>> values2 = {
                {1, 2, 3},
                {4, 5, 6}
        };

        matrix1.set(values1);
        matrix2.set(values2);

        REQUIRE(matrix1.getRows() == rows1);
        REQUIRE(matrix1.getColumns() == columns1);
        REQUIRE(matrix1.getRow(0) == std::vector<double>{1, 2});
        REQUIRE(matrix1.getRow(1) == std::vector<double>{3, 4});
        REQUIRE(matrix1.getRow(2) == std::vector<double>{5, 6});
        REQUIRE(matrix1.getColumn(0) == std::vector<double>{1, 3, 5});
        REQUIRE(matrix1.getColumn(1) == std::vector<double>{2, 4, 6});
        REQUIRE(matrix2.getRows() == rows2);
        REQUIRE(matrix2.getColumns() == columns2);
        REQUIRE(matrix2.getRow(0) == values2[0]);
        REQUIRE(matrix2.getRow(1) == values2[1]);
        REQUIRE(matrix2.getColumn(0) == std::vector<double>{1, 4});
        REQUIRE(matrix2.getColumn(1) == std::vector<double>{2, 5});
        REQUIRE(matrix2.getColumn(2) == std::vector<double>{3, 6});
    }
}
