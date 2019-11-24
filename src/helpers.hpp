#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <string>
#include <vector>
#include <set>
#include <stdexcept>
#include <unordered_map>
#include "casing.hpp"

using casing::Cluster;
using casing::Affiliation;

namespace helpers {
    typedef std::tuple<std::string, double, size_t> DistRecord;
    typedef std::vector<DistRecord> DistRecordList;
    typedef std::unordered_map<std::string, u_int> LabelsMap;


    Cluster getSubCluster(const std::string &label, Cluster &vectors);

    std::set<std::string> getClustersLabels(Cluster &vectors);

    std::vector<Cluster> getSubClusters(Cluster &vectors);

    void checkVectorSizes(std::vector<double> &input, Cluster &cluster);

    std::vector<std::vector<double>> getMeanVectors(std::vector<Cluster> &subClusters);

    std::vector<double> getMeanVector(Cluster &cluster);

    int getLowestValueIndex(std::vector<double> &features);

    DistRecordList getDistanceRecords(std::vector<double> &input, Cluster &cluster);

    void sortDistanceRecords(DistRecordList &records);

    LabelsMap getDistinctLabelsCounts(DistRecordList &records, u_short k);

    std::tuple<std::string, Affiliation> getNearestNeighborAndAffiliation(LabelsMap &counts);

}

#endif //HELPERS_HPP
