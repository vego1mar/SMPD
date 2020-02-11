#ifndef CLASSIFIERS_DEFINITIONS_HPP
#define CLASSIFIERS_DEFINITIONS_HPP

#include <utility>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include <memory>
#include "../matrix/matrix.hpp"

using matrix::Matrix;


namespace classifiers {

    typedef std::vector<double> Input;
    typedef std::vector<Input> Features;
    typedef std::vector<std::string> Labels;


    struct Cluster {
        Labels labels;
        Features features;

        bool areFeaturesShaped() const {
            for (std::size_t i = 0; i < features.size() - 1; i++) {
                const auto &previousFeature = features[i];
                const auto &nextFeature = features[i + 1];

                if (previousFeature.size() != nextFeature.size()) {
                    return false;
                }
            }

            return true;
        }

        bool areLabelsShaped() const {
            if (features.empty()) {
                return labels.empty();
            }

            return labels.size() == features[0].size();
        }

        inline bool isProperlyFormed() const {
            return areFeaturesShaped() && areLabelsShaped();
        }
    };


    struct NearestNeighborParams {
        std::unique_ptr<Input> input;
        std::unique_ptr<Cluster> cluster;
        std::unique_ptr<std::size_t> neighbors;
    };


    struct NearestNeighborMarker {
        double distance;
        std::size_t index;

        NearestNeighborMarker(double dist, std::size_t idx) : distance(dist), index(idx) {
        }
    };


    typedef std::vector<NearestNeighborMarker> Markers;
    typedef std::unordered_map<std::string, std::size_t> LabelsCountMap;


    struct NearestNeighborScore {
        std::string label;
        std::size_t nearestCount;
        std::size_t totalCount;

        NearestNeighborScore(std::string &label, std::size_t nearest, std::size_t total) :
                label(std::move(label)), nearestCount(nearest), totalCount(total) {
        }

        NearestNeighborScore(std::string &&label, std::size_t nearest, std::size_t total) :
                label(std::move(label)), nearestCount(nearest), totalCount(total) {
        }

        inline std::string toString() const {
            return '{' + label + ',' + std::to_string(nearestCount) + '/' + std::to_string(totalCount) + '}';
        }
    };


    typedef std::vector<NearestNeighborScore> NearestNeighborScores;
    typedef std::vector<Cluster> SuperCluster;


    struct NearestNeighborParams2 {
        Cluster input;
        SuperCluster superCluster;
        std::size_t neighbors;
    };


    typedef std::vector<Markers> SuperMarkers;
    typedef std::vector<LabelsCountMap> SuperCounts;


    struct NearestNeighborsArgs {
        std::unique_ptr<Matrix> input;
        std::unique_ptr<Matrix> sourceData;
        std::unique_ptr<Labels> sourceLabels;
        std::unique_ptr<std::size_t> neighbors;
    };


    struct NearestMeanArgs {
        const Matrix &input;
        const Matrix &clusterA;
        const Matrix &clusterB;
        const Labels &labels;

        NearestMeanArgs(const Matrix &input, const Matrix &clusterA, const Matrix &clusterB, const Labels &labels)
                : input(input), clusterA(clusterA), clusterB(clusterB), labels(labels) {
        }
    };


    typedef std::vector<std::vector<double>> Means;
    typedef std::vector<std::vector<double>> Distances;
    typedef std::vector<std::size_t> Indices;


    struct KMeansArgs {
        const Matrix &cluster;
        const Labels &labels;
        std::unique_ptr<std::size_t> k;
        std::unique_ptr<Matrix> cycleCluster;
        std::unique_ptr<Labels> cycleLabels;
        std::unique_ptr<std::set<std::string>> distinct;

        KMeansArgs(const Matrix &cluster, const Labels &labels) : cluster(cluster), labels(labels) {
        }
    };


    typedef std::unordered_map<std::string, Features> SubClassesDict;
    typedef std::unordered_map<std::string, Input> Centroids;

}

#endif //CLASSIFIERS_DEFINITIONS_HPP
