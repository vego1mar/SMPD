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
        Transposition
        // TODO: upper triangular
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

        Matrix(Matrix &&rvalue) = delete;

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

        void transform(TransformationType type);

        void add(const Matrix &rhs);

        void add(const std::vector<double> &rhs);

        void subtract(const Matrix &rhs);

        //void subtract(const std::vector<double> &rhs);

        void multiply(const Matrix &rhs);

        //void multiply(double lhs);

        bool isSquare() const;

    private:
        std::size_t getRowMajorOrderIndex(std::size_t rowNo, std::size_t columnNo) const;

        std::size_t getColumnMajorOrderIndex(std::size_t columnNo, std::size_t rowNo) const;

        void makeIdentityMatrix();

        void populateMatrix(double defaultValue);

        void transpose();
    };

}

#endif //MATRIX_HPP
