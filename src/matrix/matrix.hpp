#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <string>
#include <memory>
#include <vector>

namespace matrix {

    enum class TransformationType {
        Identity,
        Ones,
        Zeroes,
        Transposition,
        Negation,
        UpperTriangular
    };

    class Matrix {
    private:
        std::unique_ptr<std::size_t> columns;
        std::unique_ptr<std::size_t> rows;
        std::unique_ptr<double[]> features;

    public:
        Matrix() = delete;

        Matrix(std::size_t columns, std::size_t rows);

        Matrix(const Matrix &rhs);

        Matrix(Matrix &&rvalue) = default;

        Matrix &operator=(const Matrix &rhs) = delete;

        Matrix &operator=(Matrix &&rvalue) = delete;

        virtual ~Matrix() = default;

        bool operator==(const Matrix &rhs) const;

        bool operator!=(const Matrix &rhs) const;

        std::string toString() const;

        std::size_t getColumns() const;

        std::size_t getRows() const;

        std::size_t getSize() const;

        std::vector<double> getRow(std::size_t rowNo) const;

        std::vector<double> getColumn(std::size_t columnNo) const;

        double get(std::size_t column, std::size_t row) const;

        double get(std::size_t index) const;

        void set(const std::vector<double> &values);

        void set(const std::vector<std::vector<double>> &values);

        void set(std::size_t column, std::size_t row, double value);

        void transform(TransformationType type);

        void add(const Matrix &rhs);

        void add(const std::vector<double> &rhs);

        Matrix multiply(const Matrix &rhs);

        void multiply(double scalar);

        bool isSquare() const;

        double getDeterminant() const;

        double getDiagonalProduct() const;

    private:
        std::size_t getRowMajorOrderIndex(std::size_t rowNo, std::size_t columnNo) const;

        std::size_t getColumnMajorOrderIndex(std::size_t columnNo, std::size_t rowNo) const;

        void makeIdentityMatrix();

        void populateMatrix(double defaultValue);

        void transpose();

        void negate();

        static double fold(const std::vector<double> &row, const std::vector<double> &column);

        std::size_t makeUpperTriangular();

        static std::size_t getArgMax(std::size_t startRow, std::size_t endRow, std::size_t column, const Matrix &upper);

        void swapRows(std::size_t row1, std::size_t row2);

    };

}

#endif //MATRIX_HPP
