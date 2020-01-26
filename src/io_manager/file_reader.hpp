#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include <string>
#include <vector>
#include <fstream>

namespace io_manager {

    enum class ReadType {
        WHOLE_FILE,
        LINE_BY_LINE
    };

    class FileReader {
    private:
        ReadType type = ReadType::LINE_BY_LINE;
        std::string fileName;
        std::ifstream file;
        bool isTied = false;
        std::vector<char> contentBuffer;

    public:
        FileReader() = default;

        FileReader(const FileReader &rhs) = delete;

        FileReader(FileReader &&rvalue) noexcept = delete;

        FileReader &operator=(const FileReader &rhs) = delete;

        FileReader &operator=(FileReader &&rvalue) noexcept = delete;

        virtual ~FileReader() = default;

        const ReadType &getType() const;

        void setType(const ReadType &readType);

        long getFileSize() const;

        const std::string &getFileName() const;

        bool link(const std::string &path, std::_Ios_Openmode openModes = std::ios::out);

        bool sever();

        bool isBind() const;

        std::string getNextLine();

        bool hasNextLine();

        std::vector<char> &getContentBuffer();

        void readIntoBuffer();

        bool isEndOfFile();

    };

}

#endif //FILE_READER_HPP
