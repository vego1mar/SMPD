#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <string>
#include <vector>
#include <set>
#include <stdexcept>
#include "casing.hpp"

using namespace casing;

namespace helpers {
    std::vector<ClassVector> getSubCluster(const std::string &label, std::vector<ClassVector> &vectors);

    std::set<std::string> getClustersLabels(std::vector<ClassVector> &vectors);

    std::vector<std::vector<ClassVector>> getSubClusters(std::vector<ClassVector> &vectors);

    void checkVectorSizes(std::vector<double> &input, std::vector<ClassVector> &cluster);

    std::vector<std::vector<double>> getMeanVectors(std::vector<std::vector<casing::ClassVector>> &subClusters);

    std::vector<double> getMeanVector(std::vector<ClassVector> &cluster);

    int getLowestValueIndex(std::vector<double> &features);

}

#endif //HELPERS_HPP
