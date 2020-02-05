#ifndef HELPERS_DEFINITIONS_HPP
#define HELPERS_DEFINITIONS_HPP

#include <vector>
#include <set>


namespace helpers {

    typedef std::size_t ULong;
    typedef std::vector<ULong> Values;


    struct Sequence {
        Values values;

        bool hasDistinctValues() const {
            auto distinct = std::make_unique<std::set<ULong>>();

            for (const auto &value : values) {
                distinct->insert(value);
            }

            bool hasDistinctValues = distinct->size() == values.size();
            distinct.reset();
            return hasDistinctValues;
        }
    };


    struct Range {
        ULong begin;
        ULong end;

        Range(ULong left, ULong right) : begin(left), end(right) {
        }

        bool isMonotonic() const {
            return begin <= end;
        }
    };


}

#endif //HELPERS_DEFINITIONS_HPP
