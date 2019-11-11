#include <functional>
#include "statistical.hpp"

double statistical::arithmetic_mean(std::vector<double> &list) {
    return std::accumulate(list.begin(), list.end(), 0.0) / static_cast<double>(list.size());
}

double statistical::variance(std::vector<double> &list) {
    double square_distance = 0.0;
    double mean = statistical::arithmetic_mean(list);

    for (const double &value : list) {
        square_distance += statistical::mean_squared_error(value, mean);
    }

    return square_distance / static_cast<double>(list.size());
}

double statistical::standard_deviation(std::vector<double> &list) {
    return std::sqrt(statistical::variance(list));
}

double statistical::mean_norm_error(double left, double right, __u_short p_norm) {
    auto difference = left - right;
    auto result = 1.0;

    for (auto i = 0; i < p_norm; i++) {
        result *= difference;
    }

    return result;
}

double statistical::mean_squared_error(double left, double right) {
    return statistical::mean_norm_error(left, right, 2);
}

double statistical::norm_distance(std::vector<double> &point1, std::vector<double> &point2, __u_short p_norm) {
    if (point1.size() != point2.size()) {
        throw std::invalid_argument("Vector sizes differs.");
    }

    double summed_mse = 0.0;

    for (size_t i = 0; i < point1.size(); i++) {
        summed_mse += statistical::mean_norm_error(point1[i], point2[i], p_norm);
    }

    auto root_exponent = 1.0 / static_cast<double>(p_norm);
    return std::pow(summed_mse, root_exponent);
}

double statistical::geometric_distance(std::vector<double> &point1, std::vector<double> &point2) {
    if (point1.size() != point2.size()) {
        throw std::invalid_argument("Vector sizes differs.");
    }

    double summed_mse = 0.0;

    for (size_t i = 0; i < point1.size(); i++) {
        summed_mse += statistical::mean_squared_error(point1[i], point2[i]);
    }

    return std::sqrt(summed_mse);
}
