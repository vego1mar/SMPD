#include "collections.hpp"


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

    std::vector<int> Collections::convert(const std::vector<std::size_t> &source) {
        if (source.empty()) {
            return std::vector<int>();
        }

        std::vector<int> target;
        target.reserve(source.size());

        for (const auto &value : source) {
            target.push_back(static_cast<int>(value));
        }

        return target;
    }

    std::vector<std::size_t> Collections::convert(const std::vector<int> &source) {
        if (source.empty()) {
            return std::vector<std::size_t>();
        }

        std::vector<std::size_t> target;
        target.reserve(source.size());

        for (const auto &value : source) {
            target.push_back(static_cast<std::size_t>(value));
        }

        return target;
    }

}
