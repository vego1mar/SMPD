#include <stdexcept>
#include <tuple>
#include <algorithm>
#include <unordered_map>
#include "classifiers.hpp"
#include "casing.hpp"
#include "statistical.hpp"
#include "helpers.hpp"

namespace classifiers {
    std::string nearest_neighbor(std::vector<double> &input, std::vector<casing::ClassVector> &cluster, u_short k) {
        return nearest_neighbor_2(input, cluster, k).getLabel();
    }

    casing::NNResultSet
    nearest_neighbor_2(std::vector<double> input, std::vector<casing::ClassVector> &cluster, u_short k) {
        if (k == 0) {
            throw std::invalid_argument("Parameter of neighbors is set to be k=0.");
        }

        for (const auto &vector : cluster) {
            if (!vector.hasSameFeaturesSize(input)) {
                throw std::invalid_argument("Features sizes differs!");
            }
        }

        // marker := ["class_label", distance_to_input, index_in_cluster]
        std::vector<std::tuple<std::string, double, size_t>> markers;

        for (size_t i = 0; i < cluster.size(); i++) {
            std::vector<double> currentPoint = cluster[i].getFeatures();
            double distance = statistical::geometric_distance(input, currentPoint);
            markers.emplace_back(std::make_tuple(cluster[i].getLabel(), distance, i));
        }

        if (k > markers.size()) {
            k = markers.size();
        }

        auto comparator = [](std::tuple<std::string, double, size_t> &a, std::tuple<std::string, double, size_t> &b) {
            return std::get<1>(a) < std::get<1>(b);
        };

        std::sort(markers.begin(), markers.end(), comparator);
        std::unordered_map<std::string, u_int> counts = {};

        for (auto i = 0; i < k; i++) {
            auto label = std::get<0>(markers[i]);

            if (counts.find(label) != counts.end()) {
                counts.at(label) += 1;
                continue;
            }

            counts.insert(std::make_pair(label, 1));
        }

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

        casing::Affiliation affiliation(nearest, total);
        return casing::NNResultSet(neighbor, input, k, affiliation);
    }

    std::vector<casing::NNResultSet>
    nearest_neighbor(std::vector<casing::ClassVector> &inputGroup, std::vector<casing::ClassVector> &cluster,
                     u_short k) {
        std::vector<casing::NNResultSet> results{};
        results.reserve(inputGroup.size());

        for (const auto &vector : inputGroup) {
            results.emplace_back(nearest_neighbor_2(vector.getFeatures(), cluster, k));
        }

        return results;
    }

    std::string nearest_mean(std::vector<double> &input, std::vector<casing::ClassVector> &cluster) {
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

    std::vector<std::string>
    nearest_mean(std::vector<casing::ClassVector> &inputGroup, std::vector<casing::ClassVector> &cluster) {
        std::vector<std::string> labels;
        labels.reserve(inputGroup.size());
        int i = 0;

        for (const auto &classVector : inputGroup) {
            auto input = classVector.getFeatures();
            labels[i] = nearest_mean(input, cluster);
            i++;
        }

        return labels;
    }

    // TODO: storeResults()
    // TODO: casing/classifiers test split
}
