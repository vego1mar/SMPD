#ifndef COLLECTIONS_HPP
#define COLLECTIONS_HPP

#include <set>
#include <string>
#include <vector>


namespace helpers {

    /// This should be a utility class.
    class Collections {
    public:
        Collections() = delete;

        Collections(const Collections &rhs) = delete;

        Collections(Collections &&rvalue) noexcept = delete;

        Collections &operator=(const Collections &rhs) = delete;

        Collections &operator=(Collections &&rvalue) noexcept = delete;

        virtual ~Collections() = delete;

        static std::set<std::string> getDistinct(const std::vector<std::string> &source);

        static std::size_t count(const std::vector<std::string> &source, const std::string &label);

        static std::vector<int> getOrdinals(int begin, int end);

        static std::vector<int> convert(const std::vector<std::size_t> &source);

        static std::vector<std::size_t> convert(const std::vector<int> &source);

    };

}

#endif //COLLECTIONS_HPP
