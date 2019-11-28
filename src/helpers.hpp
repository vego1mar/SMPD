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
    typedef std::unordered_map<std::string, std::vector<int>> SubClassesIndicesMap;
    typedef std::vector<std::vector<double>> Means;


    Cluster getSubCluster(const std::string &label, Cluster &vectors);

    std::set<std::string> getClustersLabels(Cluster &vectors);

    std::vector<Cluster> getSubClusters(Cluster &vectors);

    void checkVectorSizes(std::vector<double> &input, Cluster &cluster);

    void checkVectorSizes(Cluster &cluster);

    Means getMeanVectors(std::vector<Cluster> &subClusters);

    std::vector<double> getMeanVector(Cluster &cluster);

    int getLowestValueIndex(std::vector<double> &features);

    int getHighestValueIndex(std::vector<double> &features);

    DistRecordList getDistanceRecords(std::vector<double> &input, Cluster &cluster);

    void sortDistanceRecords(DistRecordList &records);

    LabelsMap getDistinctLabelsCounts(DistRecordList &records, u_short k);

    std::tuple<std::string, Affiliation> getNearestNeighborAndAffiliation(LabelsMap &counts);

    std::vector<int> getOrdinalSequence(u_short start, u_short end);

    std::vector<int> getSubSequence(std::vector<int> &sequence, u_short start, u_short end);

    void setCentroidLabelsInOrder(Cluster &cluster, std::vector<int> &ordinals);

    void determineSubLabelsInOrder(Cluster &cluster, std::vector<int> &ordinals);

    void determineSubLabels(Cluster &cluster, Means &centroids, std::vector<std::string> &labels);

    SubClassesIndicesMap getSubClassesIndices(Cluster &cluster);

    Means getMeans(Cluster &cluster, SubClassesIndicesMap &indicesMap, std::vector<int> &ordinals);

    Means getMeans(Cluster &cluster, SubClassesIndicesMap &indicesMap, std::vector<std::string> &centroidLabels);

    std::vector<std::string> retrieveLabels(SubClassesIndicesMap &indicesMap);

    double computeGeometricCloseness(Means &previous, Means &next);

    void checkInnerVectorSizes(Cluster &cloud1, Cluster &cloud2);

    Cluster getTransposed(Cluster &cloud);

    std::vector<double> getStandardDeviation(Cluster &cloud);

}

#endif //HELPERS_HPP
