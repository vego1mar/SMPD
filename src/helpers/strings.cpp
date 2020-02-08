#include <algorithm>
#include <regex>
#include "strings.hpp"


namespace helpers {

    std::vector<std::string> Strings::split(const std::string &source, char delimiter) {
        std::stringstream stringify(source);
        std::string buffer;
        std::vector<std::string> words;

        while (std::getline(stringify, buffer, delimiter)) {
            if (!buffer.empty()) {
                words.push_back(buffer);
            }
        }

        return words;
    }

    std::string Strings::toLower(const std::string &source) {
        std::string target(source);
        std::transform(target.begin(), target.end(), target.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        return target;
    }

    bool Strings::startsWith(const std::string &source, const std::string &prefix) {
        return source.size() >= prefix.size() && source.compare(0, prefix.size(), prefix) == 0;
    }

    std::string Strings::between(const std::string &source, const std::string &lhs, const std::string &rhs) {
        // "[\s\S]" => any whitespace or non-whitespace
        std::string regexStr = lhs + "[\\s\\S]*(.*)" + rhs;
        std::regex contentBetween(regexStr);
        std::smatch baseMatch;
        std::regex_search(source, baseMatch, contentBetween);

        if (baseMatch.empty()) {
            return std::string();
        }

        std::string matchStr = baseMatch[0].str();
        auto lowerBound = lhs.size();
        auto upperBound = matchStr.size() - rhs.size();
        auto result = matchStr.substr(0, upperBound).substr(lowerBound);
        return result;
    }

    std::string Strings::betweenFirsts(const std::string &source, const std::string &lhs, const std::string &rhs) {
        auto left = source.find(lhs);
        auto right = source.find(rhs, left + lhs.size());
        bool isFirstStringFound = left != std::string::npos;
        bool isLastStringFound = right != std::string::npos;

        if (isFirstStringFound && isLastStringFound) {
            auto subString = source.substr(left + lhs.size(), right - left - lhs.size());
            return subString;
        }

        return std::string();
    }

    std::string Strings::flatten(const std::vector<char> &buffer) {
        std::string flatten;

        for (const auto &symbol : buffer) {
            flatten += symbol;
        }

        return flatten;
    }

    std::string Strings::remove(const std::string &source, char match) {
        std::string removed;

        for (const auto &character : source) {
            if (character != match) {
                removed += character;
            }
        }

        return removed;
    }

    std::string Strings::replace(const std::string &source, char match, const std::string &replace) {
        std::string replaced;

        for (const auto &character : source) {
            if (character == match) {
                replaced += replace;
                continue;
            }

            replaced += character;
        }

        return replaced;
    }

    std::string Strings::replace(const std::string &source, const std::string &what, const std::string &into) {
        if (source.empty()) {
            return "";
        }

        auto str = std::string(source);
        std::size_t position = 0;

        while ((position = str.find(what, position)) != std::string::npos) {
            str.replace(position, what.length(), into);
            position += into.length();
        }

        return str;
    }

    bool Strings::isNumber(const std::string &source) {
        std::regex number("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?");
        return std::regex_match(source, number);
    }

    bool Strings::contains(const std::string &source, const std::string &matchStr) {
        return source.find(matchStr) != std::string::npos;
    }

    std::string Strings::trimLeft(const std::string &str) {
        auto s = std::string(str);

        auto findIfNotSpace = std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
        });

        s.erase(s.begin(), findIfNotSpace);
        return s;
    }

    std::string Strings::trimRight(const std::string &str) {
        auto s = std::string(str);

        auto findIfNotSpace = std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !std::isspace(ch);
        });

        s.erase(findIfNotSpace.base(), s.end());
        return s;
    }

    std::string Strings::trim(const std::string &str) {
        auto s = trimLeft(str);
        s = trimRight(s);
        return s;
    }

    std::string Strings::getFirstWord(const std::string &source) {
        const auto trimmedSource = trim(source);
        const auto spaceIndex = trimmedSource.find_first_of(' ');

        if (spaceIndex == std::string::npos) {
            return trimmedSource;
        }

        const auto word = trimmedSource.substr(0, spaceIndex);
        return word;
    }

}
