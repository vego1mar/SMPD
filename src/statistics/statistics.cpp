#include <numeric>
#include <cmath>
#include <stdexcept>
#include "statistics.hpp"

namespace statistics {

    double Statistics::arithmeticMean(const std::vector<double> &list) {
        return std::accumulate(list.begin(), list.end(), 0.0) / static_cast<double>(list.size());
    }

    std::vector<double> Statistics::arithmeticMean(const Matrix &matrix) {
        std::vector<double> mean;
        mean.reserve(matrix.getRows());

        for (std::size_t i = 0; i < matrix.getRows(); i++) {
            auto currentMean = arithmeticMean(matrix.getRow(i));
            mean.emplace_back(currentMean);
        }

        return mean;
    }

    double Statistics::meanSquaredError(double left, double right) {
        const unsigned short P_NORM = 2;
        auto difference = left - right;
        auto result = 1.0;

        for (auto i = 0; i < P_NORM; i++) {
            result *= difference;
        }

        return result;
    }

    double Statistics::variance(const std::vector<double> &list) {
        double square_distance = 0.0;
        double mean = arithmeticMean(list);

        for (const double &value : list) {
            square_distance += meanSquaredError(value, mean);
        }

        return square_distance / static_cast<double>(list.size());
    }

    double Statistics::standardDeviation(const std::vector<double> &list) {
        return std::sqrt(variance(list));
    }

    int Statistics::argMax(const std::vector<double> &list) {
        if (list.empty()) {
            throw std::invalid_argument("list.empty()");
        }

        int maxIndex = 0;
        double maxValue = list[maxIndex];
        int i = 0;

        for (const auto &feature : list) {
            if (feature > maxValue) {
                maxValue = feature;
                maxIndex = i;
            }

            i++;
        }

        return maxIndex;
    }

    double Statistics::normL2(const std::vector<double> &list) {
        double sum = 0.0;

        for (const auto &value : list) {
            sum += value * value;
        }

        return std::sqrt(sum);
    }

}
