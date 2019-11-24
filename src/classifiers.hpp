#ifndef CLASSIFIERS_HPP
#define CLASSIFIERS_HPP

#include <string>
#include <vector>
#include "casing.hpp"

using casing::Cluster;
using casing::NNResultSet;

namespace classifiers {

    std::string nearest_neighbor(std::vector<double> &input, Cluster &cluster, u_short k = 1);

    NNResultSet nearest_neighbor_2(std::vector<double> input, Cluster &cluster, u_short k);

    std::vector<NNResultSet> nearest_neighbor(Cluster &inputGroup, Cluster &cluster, u_short k);

    std::string nearest_mean(std::vector<double> &input, Cluster &cluster);

    std::vector<std::string> nearest_mean(Cluster &inputGroup, Cluster &cluster);

}

#endif //CLASSIFIERS_HPP
