#include <tuple>
#include <algorithm>
#include "helpers.hpp"
#include "casing.hpp"
#include "statistical.hpp"

namespace helpers {
    Cluster getSubCluster(const std::string &label, Cluster &vectors) {
        Cluster outCluster{};

        for (const auto &classVector : vectors) {
            if (classVector.getLabel() == label) {
                outCluster.push_back(classVector);
            }
        }

        return outCluster;
    }

    std::set<std::string> getClustersLabels(Cluster &vectors) {
        std::set<std::string> labelsSet;

        for (const auto &classVector : vectors) {
            bool isLabelInSet = (labelsSet.find(classVector.getLabel()) != labelsSet.end());

            if (!isLabelInSet) {
                labelsSet.insert(classVector.getLabel());
                continue;
            }
        }

        return labelsSet;
    }

    std::vector<Cluster> getSubClusters(Cluster &vectors) {
        std::vector<Cluster> outClusters;
        auto labelsSet = getClustersLabels(vectors);
        outClusters.reserve(labelsSet.size());

        for (const auto &label : labelsSet) {
            outClusters.push_back(getSubCluster(label, vectors));
        }

        return outClusters;
    }

    void checkVectorSizes(std::vector<double> &input, Cluster &cluster) {
        for (const auto &classVector : cluster) {
            if (classVector.getFeatures().size() != input.size()) {
                throw std::invalid_argument("Feature sizes differs!");
            }
        }
    }

    void checkVectorSizes(Cluster &cluster) {
        if (cluster.empty()) {
            return;
        }

        auto vectorSize = cluster[0].getFeatures().size();

        for (const auto &classVector : cluster) {
            if (classVector.getFeatures().size() != vectorSize) {
                throw std::invalid_argument("Feature sizes differs!");
            }
        }
    }

    std::vector<std::vector<double>> getMeanVectors(std::vector<Cluster> &subClusters) {
        std::vector<std::vector<double>> meanVectors;
        meanVectors.resize(subClusters.size());
        int i = 0;

        for (auto &cluster : subClusters) {
            meanVectors[i] = getMeanVector(cluster);
            i++;
        }

        return meanVectors;
    }

    std::vector<double> getMeanVector(Cluster &cluster) {
        std::vector<double> meanVector;
        meanVector.resize(cluster[0].getFeatures().size());
        std::fill(meanVector.begin(), meanVector.end(), 0.0);

        for (const auto &group : cluster) {
            for (size_t i = 0; i < group.getFeatures().size(); i++) {
                meanVector[i] += group.getFeatures()[i];
            }
        }

        for (auto &feature : meanVector) {
            feature /= cluster.size();
        }

        return meanVector;
    }

    int getLowestValueIndex(std::vector<double> &features) {
        if (features.empty()) {
            throw std::invalid_argument("There are no features.");
        }

        int lowestIndex = 0;
        double lowestValue = features[lowestIndex];
        int i = 0;

        for (const auto &feature : features) {
            if (feature < lowestValue) {
                lowestValue = feature;
                lowestIndex = i;
            }

            i++;
        }

        return lowestIndex;
    }

    DistRecordList getDistanceRecords(std::vector<double> &input, Cluster &cluster) {
        // ["class_label", distance_to_input, index_in_cluster]
        DistRecordList records;

        for (size_t i = 0; i < cluster.size(); i++) {
            std::vector<double> currentPoint = cluster[i].getFeatures();
            double distance = statistical::geometric_distance(input, currentPoint);
            records.emplace_back(std::make_tuple(cluster[i].getLabel(), distance, i));
        }

        return records;
    }

    void sortDistanceRecords(DistRecordList &records) {
        auto compareByDistance = [](DistRecord &a, DistRecord &b) {
            return std::get<1>(a) < std::get<1>(b);
        };

        std::sort(records.begin(), records.end(), compareByDistance);
    }

    LabelsMap getDistinctLabelsCounts(DistRecordList &records, u_short k) {
        LabelsMap counts = {};

        for (auto i = 0; i < k; i++) {
            auto label = std::get<0>(records[i]);

            if (counts.find(label) != counts.end()) {
                counts.at(label) += 1;
                continue;
            }

            counts.insert(std::make_pair(label, 1));
        }

        // ["label", occurrences_no]
        return counts;
    }

    std::tuple<std::string, Affiliation> getNearestNeighborAndAffiliation(LabelsMap &counts) {
        u_int nearest = 0;
        u_int total = 0;
        std::string neighbor;

        for (const auto &i : counts) {
            if (i.second > nearest) {
                nearest = i.second;
                neighbor = i.first;
            }

            total += i.second;
        }

        return std::make_tuple(neighbor, Affiliation(nearest, total));
    }

}
