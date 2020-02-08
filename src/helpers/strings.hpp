#ifndef STRINGS_HPP
#define STRINGS_HPP

#include <vector>
#include <string>


namespace helpers {

    class Strings {
    public:
        Strings() = delete;

        Strings(const Strings &rhs) = delete;

        Strings(Strings &&rvalue) noexcept = delete;

        Strings &operator=(const Strings &rhs) = delete;

        Strings &operator=(Strings &&rvalue) noexcept = delete;

        virtual ~Strings() = delete;

        static std::vector<std::string> split(const std::string &source, char delimiter = ' ');

        static std::string toLower(const std::string &source);

        static bool startsWith(const std::string &source, const std::string &prefix);

        static std::string between(const std::string &source, const std::string &lhs, const std::string &rhs);

        static std::string betweenFirsts(const std::string &source, const std::string &lhs, const std::string &rhs);

        static std::string flatten(const std::vector<char> &buffer);

        static std::string remove(const std::string &source, char match);

        static std::string replace(const std::string &source, char match, const std::string &replace);

        static std::string replace(const std::string &source, const std::string &what, const std::string &into);

        static bool isNumber(const std::string &source);

        static bool contains(const std::string &source, const std::string &matchStr);

        static std::string trimLeft(const std::string &str);

        static std::string trimRight(const std::string &str);

        static std::string trim(const std::string &str);

        static std::string getFirstWord(const std::string &source);

    };

}

#endif //STRINGS_HPP
