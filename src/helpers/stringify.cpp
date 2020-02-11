#include <sstream>
#include "stringify.hpp"


namespace helpers {

    std::string Stringify::toString(const FLDHeader &fldHeader) {
        const auto &first = std::get<0>(fldHeader);
        const auto &second = std::get<1>(fldHeader);
        std::string result = '(' + first + ',' + second + ')';
        return result;
    }

    std::string Stringify::toString(const IntVector &source) {
        if (source.empty()) {
            return "[]";
        }

        std::string result = "[";

        for (std::size_t i = 0; i < source.size() - 1; i++) {
            const auto &number = source[i];
            result += std::to_string(number) + ',';
        }

        result += std::to_string(source[source.size() - 1]);
        result += ']';
        return result;
    }

    std::string Stringify::toString(const std::vector<std::size_t> &source) {
        if (source.empty()) {
            return "[]";
        }

        std::string result = "[";

        for (std::size_t i = 0; i < source.size() - 1; i++) {
            const auto &number = source[i];
            result += std::to_string(number) + ',';
        }

        const auto &lastSource = source[source.size() - 1];
        result += std::to_string(lastSource) + ']';
        return result;
    }

    std::string Stringify::toString(const NearestNeighborScores &scores) {
        if (scores.empty()) {
            return "[]";
        }

        std::string str = "[";

        for (std::size_t i = 0; i < scores.size() - 1; i++) {
            const auto &score = scores[i];
            str += score.toString() + ',';
        }

        const auto &lastScore = scores[scores.size() - 1];
        str += lastScore.toString() + ']';
        return str;
    }

    std::string Stringify::toString(const LabelsCountMap &countMap) {
        if (countMap.empty()) {
            return "[]";
        }

        std::string str = "[";

        for (const auto &entry : countMap) {
            str += '(' + entry.first + ',' + std::to_string(entry.second) + "),";
        }

        if (str.size() > 1) {
            str = str.substr(0, str.size() - 1);
        }

        str += ']';
        return str;
    }

    std::string Stringify::toString(const Centroids &centroids) {
        if (centroids.empty()) {
            return "[]";
        }

        std::string str = "[";

        for (const auto &entry : centroids) {
            str += '(' + entry.first + ',' + toString(entry.second) + "),";
        }

        if (str.size() > 1) {
            str = str.substr(0, str.size() - 1);
        }

        str += ']';
        return str;
    }

    std::string Stringify::toString(const std::vector<double> &source) {
        if (source.empty()) {
            return "[]";
        }

        std::ostringstream stringify;
        stringify.precision(3);
        stringify << '[';

        for (std::size_t i = 0; i < source.size() - 1; i++) {
            stringify << source[i] << ',';
        }

        stringify << source[source.size() - 1] << ']';
        return stringify.str();
    }

}
