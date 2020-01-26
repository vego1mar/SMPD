#ifndef CSV_READER_HPP
#define CSV_READER_HPP

#include <bits/unique_ptr.h>
#include "file_reader.hpp"


namespace io_manager {

    typedef std::vector<std::string> CSVLine;
    typedef std::vector<CSVLine> CSVFile;


    class CSVReader {
    private:
        std::unique_ptr<FileReader> fileReader;
        std::unique_ptr<std::string> filePath;
        std::unique_ptr<CSVFile> fileContent;

    public:
        CSVReader() = delete;

        explicit CSVReader(const std::string &path);

        CSVReader(const CSVReader &rhs) = delete;

        CSVReader(CSVReader &&rvalue) noexcept = delete;

        CSVReader &operator=(const CSVReader &rhs) = delete;

        CSVReader &operator=(CSVReader &&rvalue) noexcept = delete;

        virtual ~CSVReader();

        void read();

        const CSVFile &getContent() const;

        void dispose();

    private:
        void processLine(const std::string &line);

    };

}

#endif //CSV_READER_HPP
