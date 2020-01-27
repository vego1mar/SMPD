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

    };

}

#endif //COLLECTIONS_HPP
