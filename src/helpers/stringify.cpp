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

}
