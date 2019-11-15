#ifndef CLASSIFIERS_HPP
#define CLASSIFIERS_HPP

#include <string>
#include <vector>
#include "casing.hpp"

namespace classifiers {

    std::string nearest_neighbor(std::vector<double> &input, std::vector<casing::ClassVector> &cluster, u_short k = 1);

    casing::NNResultSet
    nearest_neighbor_2(std::vector<double> input, std::vector<casing::ClassVector> &cluster, u_short k);

    std::vector<casing::NNResultSet>
    nearest_neighbor(std::vector<casing::ClassVector> &inputGroup, std::vector<casing::ClassVector> &cluster,
                     u_short k);

    std::string nearest_mean(std::vector<double> &input, std::vector<casing::ClassVector> &cluster);

    std::vector<std::string>
    nearest_mean(std::vector<casing::ClassVector> &inputGroup, std::vector<casing::ClassVector> &cluster);

}

#endif //CLASSIFIERS_HPP
