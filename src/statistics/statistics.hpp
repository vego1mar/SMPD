#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <vector>
#include "../matrix/matrix.hpp"

using matrix::Matrix;


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

        static double arithmeticMean(const std::vector<double> &list);

        static std::vector<double> arithmeticMean(const Matrix &matrix);

        static std::vector<double> arithmeticMean2(const Matrix &matrix);

        static double meanSquaredError(double left, double right);

        static double variance(const std::vector<double> &list);

        static double standardDeviation(const std::vector<double> &list);

        static int argMax(const std::vector<double> &list);

        static std::size_t argMin(const std::vector<double> &list);

        static double normL2(const std::vector<double> &list);

        static double geometricDistance(const std::vector<double> &point1, const std::vector<double> &point2);

    };

}

#endif //STATISTICS_HPP
