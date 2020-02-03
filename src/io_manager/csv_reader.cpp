#include "csv_reader.hpp"
#include "../helpers/strings.hpp"

using helpers::Strings;


namespace io_manager {

    CSVReader::CSVReader(const std::string &path) {
        fileReader = std::make_unique<FileReader>();
        filePath = std::make_unique<std::string>(path);
        fileContent = std::make_unique<CSVFile>();
    }

    void CSVReader::dispose() {
        if (fileReader != nullptr) {
            fileReader.reset();
        }

        if (filePath != nullptr) {
            filePath.reset();
        }

        if (fileContent != nullptr) {
            fileContent.reset();
        }
    }

    void CSVReader::read() {
        fileReader->setType(ReadType::LINE_BY_LINE);
        fileReader->link(*filePath);

        if (!fileReader->isBind()) {
            throw std::invalid_argument("!fileReader->isBind()");
        }

        while (fileReader->hasNextLine()) {
            const auto line = fileReader->getNextLine();
            processLine(line);
        }

        fileReader->sever();
    }

    const CSVFile &CSVReader::getContent() const {
        return *fileContent;
    }

    void CSVReader::processLine(const std::string &line) {
        const auto verse = Strings::remove(line, '\n');
        const CSVLine record = Strings::split(verse, ',');
        fileContent->emplace_back(record);
    }

}
