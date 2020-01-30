#include <functional>
#include <set>
#include "csv_parser.hpp"
#include "../helpers/strings.hpp"

using helpers::Strings;


namespace data_builders {

    CSVParser::CSVParser(const std::string &filePath) {
        headers = std::make_unique<Headers>();
        reader = std::make_unique<CSVReader>(filePath);
        data = std::make_unique<CSVDataFile>();
    }

    void CSVParser::build() {
        if (reader == nullptr) {
            throw std::bad_function_call();
        }

        reader->read();
        processCSVRecords(reader->getContent());
        reader->dispose();
        buildMatrix();
    }

    void CSVParser::dispose() {
        if (headers != nullptr) {
            headers.reset();
        }

        if (reader != nullptr) {
            reader.reset();
        }

        if (data != nullptr) {
            data.reset();
        }

        if (dataset != nullptr) {
            dataset.reset();
        }
    }

    const Matrix &CSVParser::getDataset() const {
        return *dataset;
    }

    const Headers &CSVParser::getHeaders() const {
        return *headers;
    }

    void CSVParser::getData(const std::string &header, Matrix &out) const {
        if (headers->empty() || dataset == nullptr) {
            throw std::bad_function_call();
        }

        if (headers->size() != dataset->getRows()) {
            throw std::invalid_argument("headers->size() != dataset->getRows()");
        }

        auto selection = std::make_unique<CSVDataFile>();

        for (std::size_t i = 0; i < headers->size(); i++) {
            const auto &name = (*headers)[i];

            if (name == header) {
                selection->push_back(dataset->getRow(i));
            }
        }

        out.set(*selection);
        selection.reset();
    }

    void CSVParser::regroupAsSuperCluster(std::vector<Matrix> &outMatrix, Headers &outHeaders) const {
        typedef std::vector<double> Row;
        typedef std::vector<Row> Rows;

        std::set<std::string> firstWords;
        std::size_t outIndex = 0;

        for (std::size_t i = 0; i < headers->size(); i++) {
            const auto &header = (*headers)[i];
            const auto firstWord = Strings::split(header, ' ')[0];
            bool isAlreadyExists = firstWords.find(firstWord) != firstWords.end();

            if (isAlreadyExists) {
                continue;
            }

            firstWords.insert(firstWord);
            auto rows = std::make_unique<Rows>();
            rows->push_back(dataset->getRow(i));
            outHeaders.push_back(firstWord);

            for (std::size_t j = i + 1; j < headers->size(); j++) {
                const auto &nextHeader = (*headers)[j];
                const auto nextFirstWord = Strings::split(nextHeader, ' ')[0];

                if (firstWord == nextFirstWord) {
                    rows->push_back(dataset->getRow(j));
                }
            }

            const auto columnsNo = (*rows)[0].size();
            const auto rowsNo = rows->size();
            outMatrix.emplace_back(Matrix(columnsNo, rowsNo));
            outMatrix[outIndex].set(*rows);
            outIndex++;
            rows.reset();
        }
    }

    void CSVParser::processCSVRecords(const CSVFile &csvFile) {
        for (std::size_t i = 0; i < csvFile.size(); i++) {
            const auto &record = csvFile[i];
            const auto &header = record[0];
            headers->push_back(header);
            data->push_back(CSVDataLine());

            for (std::size_t j = 1; j < record.size(); j++) {
                const auto &valueStr = record[j];
                double value = std::stod(valueStr);
                (*data)[i].push_back(value);
            }
        }
    }

    void CSVParser::buildMatrix() {
        const std::size_t columns = (*data)[0].size();
        const std::size_t rows = data->size();
        dataset = std::make_unique<Matrix>(columns, rows);
        dataset->set(*data);
        data.reset();
    }

}
