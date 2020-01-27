#include <algorithm>
#include "cla_parser.hpp"


namespace command_line {

    const std::string CLAParser::EMPTY_STRING = std::string();


    CLAParser::CLAParser(const int &argc, char **argv) : programName(argv[0]) {
        for (int i = 1; i < argc; i++) {
            tokens.emplace_back(std::string(argv[i]));
        }
    }

    bool CLAParser::isOptionExists(const std::string &option) const {
        return std::find(tokens.begin(), tokens.end(), option) != tokens.end();
    }

    const std::string &CLAParser::getOption(const std::string &option) const {
        std::vector<std::string>::const_iterator it;
        it = std::find(tokens.begin(), tokens.end(), option);

        if (it != tokens.end() && ++it != tokens.end()) {
            return *it;
        }

        return EMPTY_STRING;
    }

    const std::string &CLAParser::getProgramName() const {
        return programName;
    }

    bool CLAParser::hasArguments() const {
        return !tokens.empty();
    }

}
