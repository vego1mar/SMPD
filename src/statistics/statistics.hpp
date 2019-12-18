#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <vector>

namespace statistics {

    /// A utility class.
    class Statistics {
    public:
        Statistics() = delete;

        Statistics(const Statistics &rhs) = delete;

        Statistics(Statistics &&rvalue) noexcept = delete;

        Statistics &operator=(const Statistics &rhs) = delete;

        Statistics &operator=(Statistics &&rvalue) noexcept = delete;

        virtual ~Statistics() = delete;

    public:
        static double arithmeticMean(const std::vector<double> &list);

        static double meanSquaredError(double left, double right);

        static double variance(const std::vector<double> &list);

        static double standardDeviation(const std::vector<double> &list);

        static int argMax(const std::vector<double> &list);
    };

}

#endif //STATISTICS_HPP
