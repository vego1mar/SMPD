#include <sstream>
#include <limits>
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
        std::string sizeTuple = '(' + std::to_string(*rows) + ',' + std::to_string(*columns) + ')';
        std::string toString = '{' + sizeTuple + ",[";
        std::ostringstream stringify;
        stringify.precision(2);

        for (std::size_t i = 0; i < getRows(); i++) {
            stringify << '[';

            for (std::size_t j = 0; j < getColumns() - 1; j++) {
                stringify << get(j, i) << ',';
            }

            stringify << get(getColumns() - 1, i);
            stringify << "],";
        }

        std::string featuresStr = stringify.str();
        featuresStr = featuresStr.substr(0, featuresStr.size() - 1);
        toString += featuresStr + "]}";
        return toString;
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

    void Matrix::set(std::size_t column, std::size_t row, double value) {
        if (column >= getColumns() || row >= getRows()) {
            throw std::logic_error("column >= getColumns() || row >= getRows()");
        }

        auto index = getRowMajorOrderIndex(row, column);
        features[index] = value;
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
            case TransformationType::Negation:
                negate();
                break;
            case TransformationType::UpperTriangular:
                makeUpperTriangular();
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

    Matrix Matrix::multiply(const Matrix &rhs) {
        if (getColumns() != rhs.getRows()) {
            throw std::invalid_argument("getColumns() != rhs.getRows()");
        }

        std::size_t columnsNo = getRows();
        std::size_t rowsNo = rhs.getColumns();
        Matrix result(columnsNo, rowsNo);
        result.transform(TransformationType::Zeroes);
        auto values = std::vector<double>();

        for (std::size_t i = 0; i < getRows(); i++) {
            auto currentRow = getRow(i);

            for (std::size_t j = 0; j < rhs.getColumns(); j++) {
                auto currentColumn = rhs.getColumn(j);
                auto element = fold(currentRow, currentColumn);
                values.emplace_back(element);
            }
        }

        result.set(values);
        return result;
    }

    void Matrix::multiply(double scalar) {
        for (std::size_t i = 0; i < getSize(); i++) {
            features[i] *= scalar;
        }
    }

    bool Matrix::isSquare() const {
        return getColumns() == getRows();
    }

    double Matrix::getDeterminant() const {
        if (!isSquare()) {
            throw std::logic_error("!isSquare()");
        }

        if (getSize() == 1) {
            return get(0);
        }

        if (getSize() == 4) {
            // 2x2 --> ad - bc
            auto ad = get(0, 0) * get(1, 1);
            auto bc = get(1, 0) * get(0, 1);
            return ad - bc;
        }

        if (getSize() == 9) {
            // 3x3 --> a(ei-fh) - b(di-fg) + c(dh-eg)
            auto ei_fh = (get(1, 1) * get(2, 2)) - (get(2, 1) * get(1, 2));
            auto di_fg = (get(0, 1) * get(2, 2)) - (get(2, 1) * get(0, 2));
            auto dh_eg = (get(0, 1) * get(1, 2)) - (get(1, 1) * get(0, 2));
            auto a = get(0, 0) * ei_fh;
            auto b = get(1, 0) * di_fg;
            auto c = get(2, 0) * dh_eg;
            return a - b + c;
        }

        Matrix triangular(*this);
        auto swapsNo = triangular.makeUpperTriangular();
        bool isSwapsNumberEven = swapsNo % 2 == 0;
        double detSign = isSwapsNumberEven ? 1.0 : -1.0;
        return detSign * triangular.getDiagonalProduct();
    }

    double Matrix::getDiagonalProduct() const {
        double product = 1.0;

        for (std::size_t i = 0; i < getRows(); i++) {
            for (std::size_t j = 0; j < getColumns(); j++) {
                if (i == j) {
                    product *= get(j, i);
                }
            }
        }

        return product;
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

    void Matrix::negate() {
        for (std::size_t i = 0; i < getSize(); i++) {
            features[i] = -features[i];
        }
    }

    double Matrix::fold(const std::vector<double> &row, const std::vector<double> &column) {
        // Assumption: row.size() == column.size()
        double sum = 0.0;

        for (std::size_t i = 0; i < row.size(); i++) {
            sum += row[i] * column[i];
        }

        return sum;
    }

    std::size_t Matrix::makeUpperTriangular() {
        std::size_t pivotRow = 0;
        std::size_t pivotColumn = 0;
        std::size_t endRow = getRows();
        std::size_t endColumn = getColumns();
        std::size_t swapsNo = 0;
        auto epsilon = std::numeric_limits<double>::epsilon();

        while (pivotRow < endRow && pivotColumn < endColumn) {
            auto argMaxRow = getArgMax(pivotRow, endRow, pivotColumn, *this);
            auto pivotValue = get(pivotColumn, argMaxRow);
            bool isEqualZero = pivotValue < epsilon && pivotValue > -epsilon;

            if (isEqualZero) {
                pivotColumn++;
                continue;
            }

            swapRows(pivotRow, argMaxRow);

            if (pivotRow != argMaxRow) {
                swapsNo++;
            }

            for (std::size_t i = pivotRow + 1; i < endRow; i++) {
                auto scalar = get(pivotColumn, i) / get(pivotColumn, pivotRow);
                set(pivotColumn, i, 0.0);

                for (std::size_t j = pivotColumn + 1; j < endColumn; j++) {
                    auto newValue = get(j, i) - (get(j, pivotRow) * scalar);
                    set(j, i, newValue);
                }
            }

            pivotRow++;
            pivotColumn++;
        }

        return swapsNo;
    }

    std::size_t Matrix::getArgMax(std::size_t startRow, std::size_t endRow, std::size_t column, const Matrix &upper) {
        struct ArgMaxHelper {
            std::size_t row;
            double arg;
        };
        auto args = std::vector<ArgMaxHelper>();
        ArgMaxHelper currentSelect{};

        for (std::size_t i = startRow; i < endRow; i++) {
            currentSelect = ArgMaxHelper();
            currentSelect.row = i;
            currentSelect.arg = std::abs(upper.get(column, i));
            args.push_back(currentSelect);
        }

        std::size_t maxArg = 0;
        double maxValue = 0.0;

        for (std::size_t j = 0; j < args.size(); j++) {
            if (args[j].arg > maxValue) {
                maxValue = args[j].arg;
                maxArg = j;
            }
        }

        return args[maxArg].row;
    }

    void Matrix::swapRows(std::size_t row1, std::size_t row2) {
        if (row1 == row2) {
            return;
        }

        auto row1Set = getRow(row1);
        auto row2Set = getRow(row2);
        std::size_t row1Start = getRowMajorOrderIndex(row1, 0);
        std::size_t row2Start = getRowMajorOrderIndex(row2, 0);
        std::size_t row1End = getRowMajorOrderIndex(row1, getColumns());
        std::size_t row2End = getRowMajorOrderIndex(row2, getColumns());
        std::size_t j = 0;

        for (std::size_t i = row1Start; i < row1End; i++) {
            features[i] = row2Set[j];
            j++;
        }

        j = 0;

        for (std::size_t k = row2Start; k < row2End; k++) {
            features[k] = row1Set[j];
            j++;
        }
    }

}
