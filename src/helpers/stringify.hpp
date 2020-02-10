#ifndef STRINGIFY_HPP
#define STRINGIFY_HPP

#include <string>
#include "../main_program/statistical_run.hpp"
#include "../selectors/definitions.hpp"
#include "../classifiers/definitions.hpp"

using main_program::FLDHeader;
using selectors::IntVector;
using classifiers::NearestNeighborScores;
using classifiers::LabelsCountMap;


namespace helpers {

    class Stringify {
    public:
        Stringify() = delete;

        Stringify(const Stringify &rhs) = delete;

        Stringify(Stringify &&rvalue) noexcept = delete;

        Stringify &operator=(const Stringify &rhs) = delete;

        Stringify &operator=(Stringify &&rvalue) noexcept = delete;

        virtual ~Stringify() = delete;

        static std::string toString(const FLDHeader &fldHeader);

        static std::string toString(const IntVector &source);

        static std::string toString(const std::vector<std::size_t> &source);

        static std::string toString(const NearestNeighborScores &scores);

        static std::string toString(const LabelsCountMap &countMap);

    };

}

#endif //STRINGIFY_HPP
