#include <stdexcept>
#include <tuple>
#include <algorithm>
#include <chrono>
#include <random>
#include "classifiers.hpp"
#include "casing.hpp"
#include "statistical.hpp"
#include "helpers.hpp"

namespace classifiers {
    std::string nearest_neighbor(std::vector<double> &input, Cluster &cluster, u_short k) {
        return nearest_neighbor_2(input, cluster, k).getLabel();
    }

    NNResultSet nearest_neighbor_2(std::vector<double> input, Cluster &cluster, u_short k) {
        if (k == 0) {
            throw std::invalid_argument("Parameter of neighbors is set to be k=0.");
        }

        helpers::checkVectorSizes(input, cluster);
        auto markers = helpers::getDistanceRecords(input, cluster);

        if (k > markers.size()) {
            k = markers.size();
        }

        helpers::sortDistanceRecords(markers);
        auto counts = helpers::getDistinctLabelsCounts(markers, k);
        auto result = helpers::getNearestNeighborAndAffiliation(counts);
        auto neighbor = std::get<0>(result);
        auto affiliation = std::get<1>(result);
        return NNResultSet(neighbor, input, k, affiliation);
    }

    std::vector<NNResultSet> nearest_neighbor(Cluster &inputGroup, Cluster &cluster, u_short k) {
        std::vector<NNResultSet> results{};
        results.reserve(inputGroup.size());

        for (const auto &vector : inputGroup) {
            results.emplace_back(nearest_neighbor_2(vector.getFeatures(), cluster, k));
        }

        return results;
    }

    std::string nearest_mean(std::vector<double> &input, Cluster &cluster) {
        helpers::checkVectorSizes(input, cluster);
        auto subClusters = helpers::getSubClusters(cluster);
        auto meanVectors = helpers::getMeanVectors(subClusters);

        std::vector<double> distances;
        distances.resize(subClusters.size());
        size_t i = 0;

        for (auto &mean : meanVectors) {
            distances[i] = statistical::geometric_distance(mean, input);
            i++;
        }

        auto lowestIndex = helpers::getLowestValueIndex(distances);
        return subClusters[lowestIndex][0].getLabel();
    }

    std::vector<std::string> nearest_mean(Cluster &inputGroup, Cluster &cluster) {
        std::vector<std::string> labels;
        labels.resize(inputGroup.size());
        int i = 0;

        for (const auto &classVector : inputGroup) {
            auto input = classVector.getFeatures();
            labels[i] = nearest_mean(input, cluster);
            i++;
        }

        return labels;
    }

    /// &cluster will be altered!
    std::vector<Cluster> k_means(Cluster &cluster, u_short k, u_long maxIter) {
        if (k >= cluster.size()) {
            throw std::invalid_argument("k >= cluster.size()");
        }

        helpers::checkVectorSizes(cluster);
        auto permutation = helpers::getOrdinalSequence(0, cluster.size());
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        auto rng = std::default_random_engine(seed);
        std::shuffle(permutation.begin(), permutation.end(), rng);
        auto ordinals = helpers::getSubSequence(permutation, 0, k);

        helpers::determineSubLabelsInOrder(cluster, ordinals);
        auto subClassesIndicesMap = helpers::getSubClassesIndices(cluster);
        auto means = helpers::getMeans(cluster, subClassesIndicesMap, ordinals);
        auto previousCentroids = means;
        auto centroidLabels = helpers::retrieveLabels(subClassesIndicesMap);
        const double ACCEPTABLE_CHANGE = 0.0001;
        double delta = 1;
        u_long iter = 0;

        while (iter < maxIter || delta > ACCEPTABLE_CHANGE) {
            helpers::determineSubLabels(cluster, previousCentroids, centroidLabels);
            subClassesIndicesMap = helpers::getSubClassesIndices(cluster);
            means = helpers::getMeans(cluster, subClassesIndicesMap, centroidLabels);
            centroidLabels = helpers::retrieveLabels(subClassesIndicesMap);
            auto nextCentroids = means;
            delta = helpers::computeGeometricCloseness(previousCentroids, nextCentroids);
            previousCentroids = nextCentroids;
            iter++;
        }

        return helpers::getSubClusters(cluster);
    }

}
