#ifndef CLA_PARSER_HPP
#define CLA_PARSER_HPP

#include <vector>
#include <string>


namespace command_line {

    /// Command Line Arguments Parser
    class CLAParser {
    private:
        std::vector<std::string> tokens;
        std::string programName;
        static const std::string EMPTY_STRING;

    public:
        CLAParser() = delete;

        CLAParser(const int &argc, char **argv);

        CLAParser(const CLAParser &rhs) = delete;

        CLAParser(CLAParser &&rvalue) noexcept = delete;

        CLAParser &operator=(const CLAParser &rhs) = delete;

        CLAParser &operator=(CLAParser &&rvalue) noexcept = delete;

        virtual ~CLAParser() = default;

        bool isOptionExists(const std::string &option) const;

        const std::string &getOption(const std::string &option) const;

        const std::string &getProgramName() const;

        bool hasArguments() const;

    };

}

#endif //CLA_PARSER_HPP
