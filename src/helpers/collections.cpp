#include "collections.hpp"

using data_builders::Headers;


namespace helpers {

    std::set<std::string> Collections::getDistinct(const std::vector<std::string> &source) {
        std::set<std::string> result;

        for (const auto &str : source) {
            result.insert(str);
        }

        return result;
    }

    std::size_t Collections::count(const std::vector<std::string> &source, const std::string &label) {
        std::size_t counter = 0;

        for (const auto &str : source) {
            if (str == label) {
                counter++;
            }
        }

        return counter;
    }

    std::vector<int> Collections::getOrdinals(int begin, int end) {
        std::vector<int> ordinals;

        for (int i = begin; i < end; i++) {
            ordinals.emplace_back(i);
        }

        return ordinals;
    }

}
