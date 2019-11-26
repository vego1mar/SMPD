#include <tuple>
#include <algorithm>
#include <unordered_map>
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

    void checkVectorSizes(Means &left, Means &right) {
        if (left.size() != right.size()) {
            throw std::invalid_argument("left.size() != right.size()");
        }

        for (size_t i = 0; i < left.size(); i++) {
            if (left[i].size() != right[i].size()) {
                throw std::invalid_argument("left[i].size() != right[i].size()");
            }
        }
    }

    Means getMeanVectors(std::vector<Cluster> &subClusters) {
        Means meanVectors;
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

    std::vector<int> getOrdinalSequence(u_short start, u_short end) {
        if (start > end) {
            throw std::invalid_argument("start > end");
        }

        std::vector<int> sequence;
        auto length = end - start;
        sequence.resize(length);
        u_short i = start;

        auto increment_fn = [&i](int &seqValue) {
            seqValue = i;
            i++;
        };

        std::for_each(sequence.begin(), sequence.end(), increment_fn);
        return sequence;
    }

    std::vector<int> getSubSequence(std::vector<int> &sequence, u_short start, u_short end) {
        if (start > end || end > sequence.size()) {
            throw std::invalid_argument("start > end || end > sequence.size()");
        }

        std::vector<int> subSequence;
        subSequence.resize(end - start);

        for (size_t i = start; i < end; i++) {
            subSequence[i] = (sequence[i]);
        }

        return subSequence;
    }

    void setCentroidLabelsInOrder(Cluster &cluster, std::vector<int> &ordinals) {
        int subLabelNo = 1;

        for (int centroidIndex : ordinals) {
            std::string newLabel = cluster[centroidIndex].getLabel() + std::to_string(subLabelNo);
            cluster[centroidIndex].setLabel(newLabel);
            subLabelNo++;
        }
    }

    void determineSubLabelsInOrder(Cluster &cluster, std::vector<int> &ordinals) {
        setCentroidLabelsInOrder(cluster, ordinals);

        for (size_t i = 0; i < cluster.size(); i++) {
            bool isCentroid = std::find(ordinals.begin(), ordinals.end(), i) != ordinals.end();

            if (isCentroid) {
                continue;
            }

            std::vector<double> distances;

            for (int centroidIndex : ordinals) {
                auto centroid = cluster[centroidIndex].getFeatures();
                auto vector = cluster[i].getFeatures();
                double distance = statistical::geometric_distance(centroid, vector);
                distances.emplace_back(distance);
            }

            int minIndex = getLowestValueIndex(distances);
            int centroidIndex = ordinals[minIndex];
            std::string vectorLabel = cluster[centroidIndex].getLabel();
            cluster[i].setLabel(vectorLabel);
        }
    }

    void determineSubLabels(Cluster &cluster, Means &centroids, std::vector<std::string> &labels) {
        for (auto &cVector : cluster) {
            std::vector<double> distances;

            for (auto &centroid : centroids) {
                auto vector = cVector.getFeatures();
                double distance = statistical::geometric_distance(centroid, vector);
                distances.emplace_back(distance);
            }

            int centroidLabelIndex = getLowestValueIndex(distances);
            std::string vectorLabel = labels[centroidLabelIndex];
            cVector.setLabel(vectorLabel);
        }
    }

    SubClassesIndicesMap getSubClassesIndices(Cluster &cluster) {
        auto distinctLabels = getClustersLabels(cluster);
        SubClassesIndicesMap indicesMap{};

        for (const auto &label : distinctLabels) {
            indicesMap[label] = std::vector<int>();
        }

        for (size_t i = 0; i < cluster.size(); i++) {
            std::vector<int> &indices = indicesMap[cluster[i].getLabel()];
            indices.push_back(i);
        }

        return indicesMap;
    }

    Means getMeans(Cluster &cluster, SubClassesIndicesMap &indicesMap, std::vector<int> &ordinals) {
        Means means;

        for (int centroidIndex : ordinals) {
            auto label = cluster[centroidIndex].getLabel();
            const auto &indices = indicesMap[label];
            Cluster subCluster;

            for (const auto &index : indices) {
                subCluster.push_back(cluster[index]);
            }

            auto meanVector = getMeanVector(subCluster);
            means.emplace_back(meanVector);
        }

        return means;
    }

    Means getMeans(Cluster &cluster, SubClassesIndicesMap &indicesMap, std::vector<std::string> &centroidLabels) {
        Means means;

        for (const auto &label : centroidLabels) {
            const auto &indices = indicesMap[label];
            Cluster subCluster;

            for (const auto &index : indices) {
                subCluster.push_back(cluster[index]);
            }

            auto meanVector = getMeanVector(subCluster);
            means.emplace_back(meanVector);
        }

        return means;
    }

    std::vector<std::string> retrieveLabels(SubClassesIndicesMap &indicesMap) {
        std::vector<std::string> labels;

        for (const auto &entry : indicesMap) {
            labels.push_back(entry.first);
        }

        return labels;
    }

    double computeGeometricCloseness(Means &previous, Means &next) {
        checkVectorSizes(previous, next);

        std::vector<double> distances;

        for (size_t i = 0; i < next.size(); i++) {
            double distance = statistical::geometric_distance(next[i], previous[i]);
            distances.emplace_back(distance);
        }

        double sum = std::accumulate(distances.begin(), distances.end(), 0.0);
        return sum / distances.size();
    }

}
