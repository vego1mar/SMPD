#ifndef STATISTICAL_HPP
#define STATISTICAL_HPP

#include <numeric>
#include <cmath>
#include <vector>

namespace statistical {
    double arithmetic_mean(std::vector<double> &list);

    double variance(std::vector<double> &list);

    double standard_deviation(std::vector<double> &list);

    double mean_norm_error(double left, double right, __u_short p_norm);

    double mean_squared_error(double left, double right);

    double norm_distance(std::vector<double> &point1, std::vector<double> &point2, __u_short p_norm);

    double geometric_distance(std::vector<double> &point1, std::vector<double> &point2);
}

#endif //STATISTICAL_HPP
