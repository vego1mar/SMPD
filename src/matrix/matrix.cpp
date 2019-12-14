#include "matrix.hpp"

namespace matrix {

    Matrix::Matrix(std::size_t columns, std::size_t rows)
            : columns(std::make_unique<std::size_t>(columns))
            , rows(std::make_unique<std::size_t>(rows))
            , features(std::make_unique<double[]>(columns * rows)) {
        transform(TransformationType::Ones);
    }

    Matrix::Matrix(const Matrix &rhs)
            : columns(std::make_unique<std::size_t>(*rhs.columns))
            , rows(std::make_unique<std::size_t>(*rhs.rows))
            , features(std::make_unique<double[]>((*rhs.columns) * (*rhs.rows))) {
        for (std::size_t i = 0; i < rhs.getSize(); i++) {
            features[i] = rhs.get(i);
        }
    }

    bool Matrix::operator==(const Matrix &rhs) const {
        if (getSize() != rhs.getSize()) {
            return false;
        }

        for (std::size_t i = 0; i < getSize(); i++) {
            if (get(i) != rhs.get(i)) {
                return false;
            }
        }

        return true;
    }

    bool Matrix::operator!=(const Matrix &rhs) const {
        return !(*this == rhs);
    }

    std::string Matrix::toString() const {
        throw std::runtime_error("Not implemented");
    }

    std::size_t Matrix::getColumns() const {
        return *columns;
    }

    std::size_t Matrix::getRows() const {
        return *rows;
    }

    std::size_t Matrix::getSize() const {
        return getColumns() * getRows();
    }

    std::vector<double> Matrix::getRow(std::size_t rowNo) const {
        if (rowNo > getRows()) {
            throw std::logic_error("rowNo > getRows()");
        }

        auto selectedRow = std::vector<double>();
        selectedRow.reserve(getColumns());

        for (std::size_t i = 0; i < getColumns(); i++) {
            auto index = getRowMajorOrderIndex(rowNo, i);
            auto element = features[index];
            selectedRow.push_back(element);
        }

        return selectedRow;
    }

    std::vector<double> Matrix::getColumn(std::size_t columnNo) const {
        if (columnNo > getColumns()) {
            throw std::logic_error("columnNo > getColumns()");
        }

        auto selectedColumn = std::vector<double>();
        selectedColumn.reserve(getRows());

        for (std::size_t i = 0; i < getRows(); i++) {
            auto index = getRowMajorOrderIndex(i, columnNo);
            auto element = features[index];
            selectedColumn.push_back(element);
        }

        return selectedColumn;
    }

    double Matrix::get(std::size_t column, std::size_t row) const {
        if (column >= getColumns() || row >= getRows()) {
            throw std::out_of_range("column >= getColumns() || row >= getRows()");
        }

        auto index = getRowMajorOrderIndex(row, column);
        return features[index];
    }

    double Matrix::get(std::size_t index) const {
        if (index >= getSize()) {
            throw std::out_of_range("index >= getSize()");
        }

        return features[index];
    }

    void Matrix::set(const std::vector<double> &values) {
        if (getSize() != values.size()) {
            throw std::invalid_argument("getSize() != values.size()");
        }

        std::size_t k = 0;

        for (std::size_t i = 0; i < getRows(); i++) {
            for (std::size_t j = 0; j < getColumns(); j++) {
                auto index = getRowMajorOrderIndex(i, j);
                features[index] = values[k];
                k++;
            }
        }
    }

    void Matrix::set(const std::vector<std::vector<double>> &values) {
        if (values.size() != getRows()) {
            throw std::logic_error("values.size() != getRows()");
        }

        for (std::size_t i = 0; i < getRows(); i++) {
            for (std::size_t j = 0; j < getColumns(); j++) {
                auto index = getRowMajorOrderIndex(i, j);
                features[index] = values[i][j];
            }
        }
    }

    void Matrix::transform(TransformationType type) {
        switch (type) {
            case TransformationType::Identity :
                makeIdentityMatrix();
                break;
            case TransformationType::Ones:
                populateMatrix(1.0);
                break;
            case TransformationType::Zeroes:
                populateMatrix(0.0);
                break;
            case TransformationType::Transposition:
                transpose();
                break;
            default:
                // Do not apply any transformation.
                break;
        }
    }

    void Matrix::add(const Matrix &rhs) {
        if (rhs.getRows() != getRows() || rhs.getColumns() != getColumns()) {
            throw std::logic_error("rhs.getRows() != getRows() || rhs.getColumns() != getColumns()");
        }

        for (std::size_t i = 0; i < getSize(); i++) {
            features[i] += rhs.get(i);
        }
    }

    void Matrix::add(const std::vector<double> &rhs) {
        if (rhs.size() != getRows()) {
            throw std::logic_error("rhs.size() != getRows()");
        }

        for (std::size_t i = 0; i < getRows(); i++) {
            for (std::size_t j = 0; j < getColumns(); j++) {
                auto index = getRowMajorOrderIndex(i, j);
                features[index] += rhs[i];
            }
        }
    }

    void Matrix::subtract(const Matrix &rhs) {
        throw std::runtime_error("Not implemented");
    }

    void Matrix::multiply(const Matrix &rhs) {
        throw std::runtime_error("Not implemented");
    }

    bool Matrix::isSquare() const {
        return getColumns() == getRows();
    }

    std::size_t Matrix::getRowMajorOrderIndex(std::size_t rowNo, std::size_t columnNo) const {
        return (getColumns() * rowNo) + columnNo;
    }

    std::size_t Matrix::getColumnMajorOrderIndex(std::size_t columnNo, std::size_t rowNo) const {
        return (getRows() * columnNo) + rowNo;
    }

    void Matrix::makeIdentityMatrix() {
        if (!isSquare()) {
            throw std::logic_error("!isSquare()");
        }

        for (std::size_t i = 0; i < getRows(); i++) {
            for (std::size_t j = 0; j < getColumns(); j++) {
                if (i == j) {
                    features[getRowMajorOrderIndex(i, j)] = 1.0;
                    continue;
                }

                features[getRowMajorOrderIndex(i, j)] = 0.0;
            }
        }
    }

    void Matrix::populateMatrix(double defaultValue) {
        for (std::size_t i = 0; i < getSize(); i++) {
            features[i] = defaultValue;
        }
    }

    void Matrix::transpose() {
        Matrix before(*this);

        for (std::size_t i = 0; i < getRows(); i++) {
            for (std::size_t j = 0; j < getColumns(); j++) {
                auto index = getColumnMajorOrderIndex(j, i);
                features[index] = before.get(j, i);
            }
        }

        *columns = before.getRows();
        *rows = before.getColumns();
    }

}
