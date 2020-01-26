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
    }

    const CSVDataFile &CSVParser::getData() const {
        return *data;
    }

    const Headers &CSVParser::getHeaders() const {
        return *headers;
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

}
