#include <functional>
#include "csv_parser.hpp"

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
