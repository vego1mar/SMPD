#ifndef CONVERTERS_HPP
#define CONVERTERS_HPP

#include <string>

namespace helpers {

    /// A utility class.
    class Converters {
    public:
        Converters() = delete;

        Converters(const Converters &rhs) = delete;

        Converters(Converters &&rvalue) noexcept = delete;

        Converters &operator=(const Converters &rhs) = delete;

        Converters &operator=(Converters &&rvalue) noexcept = delete;

        virtual ~Converters() = delete;

        static std::string toHex(unsigned long number);

    };

}

#endif //CONVERTERS_HPP
