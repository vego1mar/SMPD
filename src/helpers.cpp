#include "helpers.hpp"
#include "casing.hpp"

namespace helpers {
    std::vector<ClassVector> getSubCluster(const std::string &label, std::vector<ClassVector> &vectors) {
        std::vector<ClassVector> outCluster{};

        for (const auto &classVector : vectors) {
            if (classVector.getLabel() == label) {
                outCluster.push_back(classVector);
            }
        }

        return outCluster;
    }

    std::set<std::string> getClustersLabels(std::vector<ClassVector> &vectors) {
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

    std::vector<std::vector<ClassVector>> getSubClusters(std::vector<ClassVector> &vectors) {
        std::vector<std::vector<ClassVector>> outClusters;
        auto labelsSet = getClustersLabels(vectors);
        outClusters.reserve(labelsSet.size());

        for (const auto &label : labelsSet) {
            outClusters.push_back(getSubCluster(label, vectors));
        }

        return outClusters;
    }

    void checkVectorSizes(std::vector<double> &input, std::vector<ClassVector> &cluster) {
        for (const auto &classVector : cluster) {
            if (classVector.getFeatures().size() != input.size()) {
                throw std::invalid_argument("Feature sizes differs!");
            }
        }
    }

    std::vector<std::vector<double>> getMeanVectors(std::vector<std::vector<casing::ClassVector>> &subClusters) {
        std::vector<std::vector<double>> meanVectors;
        meanVectors.resize(subClusters.size());
        int i = 0;

        for (auto &cluster : subClusters) {
            meanVectors[i] = getMeanVector(cluster);
            i++;
        }

        return meanVectors;
    }

    std::vector<double> getMeanVector(std::vector<ClassVector> &cluster) {
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

}
