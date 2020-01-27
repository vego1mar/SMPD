#ifndef STRINGIFY_HPP
#define STRINGIFY_HPP

#include <string>
#include "../main_program/statistical_run.hpp"
#include "../selectors/fisher.hpp"

using main_program::FLDHeader;
using selectors::IntVector;


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

    };

}

#endif //STRINGIFY_HPP
