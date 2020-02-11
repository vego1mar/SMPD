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

    std::vector<double> Statistics::arithmeticMean2(const Matrix &matrix) {
        std::vector<double> mean;
        mean.reserve(matrix.getColumns());

        for (std::size_t i = 0; i < matrix.getColumns(); i++) {
            auto currentMean = arithmeticMean(matrix.getColumn(i));
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

    std::size_t Statistics::argMin(const std::vector<double> &list) {
        if (list.empty()) {
            throw std::invalid_argument("list.empty()");
        }

        int minIndex = 0;
        double minValue = list[minIndex];

        for (std::size_t i = 0; i < list.size(); i++) {
            const auto &feature = list[i];

            if (feature < minValue) {
                minValue = feature;
                minIndex = i;
            }
        }

        return minIndex;
    }

    double Statistics::normL2(const std::vector<double> &list) {
        double sum = 0.0;

        for (const auto &value : list) {
            sum += value * value;
        }

        return std::sqrt(sum);
    }

    double Statistics::geometricDistance(const std::vector<double> &point1, const std::vector<double> &point2) {
        if (point1.size() != point2.size()) {
            throw std::invalid_argument("point1.size() != point2.size()");
        }

        double summed_mse = 0.0;

        for (size_t i = 0; i < point1.size(); i++) {
            summed_mse += meanSquaredError(point1[i], point2[i]);
        }

        return std::sqrt(summed_mse);
    }

    double Statistics::geometricCloseness(const Means &mean1, const Means &mean2) {
        if (mean1.empty() && mean2.empty()) {
            return 0.0;
        }

        if (mean1.empty() || mean2.empty()) {
            throw std::length_error("mean1.empty() || mean2.empty()");
        }

        if (mean1.size() != mean2.size()) {
            throw std::length_error("mean1.size() != mean2.size()");
        }

        auto distances = std::make_unique<std::vector<double>>();

        for (std::size_t i = 0; i < mean1.size(); i++) {
            const auto &point1 = mean1[i];
            const auto &point2 = mean2[i];
            const auto distance = geometricDistance(point1, point2);
            distances->emplace_back(distance);
        }

        const auto sum = std::accumulate(distances->begin(), distances->end(), 0.0);
        const auto closeness = sum / distances->size();
        distances.reset();
        return closeness;
    }

}
