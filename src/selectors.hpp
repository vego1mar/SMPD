#ifndef SELECTORS_HPP
#define SELECTORS_HPP

#include "casing.hpp"

using casing::Cluster;

namespace selectors {

    std::vector<double> fisher_coefficients(Cluster &cloud1, Cluster &cloud2);

    double fisher_coefficient(Cluster &cluster1, Cluster &cluster2);

}

#endif //SELECTORS_HPP
