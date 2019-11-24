#include <stdexcept>
#include <tuple>
#include <algorithm>
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
