#ifndef CSV_PARSER_HPP
#define CSV_PARSER_HPP

#include <memory>
#include <vector>
#include "../io_manager/csv_reader.hpp"

using io_manager::CSVReader;
using io_manager::CSVFile;


namespace data_builders {

    typedef std::vector<std::string> Headers;
    typedef std::vector<double> CSVDataLine;
    typedef std::vector<CSVDataLine> CSVDataFile;


    class CSVParser {
    private:
        std::unique_ptr<Headers> headers;
        std::unique_ptr<CSVReader> reader;
        std::unique_ptr<CSVDataFile> data;

    public:
        CSVParser() = delete;

        explicit CSVParser(const std::string &filePath);

        CSVParser(const CSVParser &rhs) = delete;

        CSVParser(CSVParser &&rvalue) noexcept = delete;

        CSVParser &operator=(const CSVParser &rhs) = delete;

        CSVParser &operator=(CSVParser &&rvalue) noexcept = delete;

        virtual ~CSVParser() = default;

        void build();

        void dispose();

        const CSVDataFile &getData() const;

        const Headers &getHeaders() const;

    private:
        void processCSVRecords(const CSVFile &csvFile);

    };

}

#endif //CSV_PARSER_HPP
