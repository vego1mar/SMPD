#include <iomanip>
#include "converters.hpp"

namespace helpers {

    std::string Converters::toHex(unsigned long number) {
        std::stringstream stream;
        stream << std::hex << number;
        return stream.str();
    }

}
