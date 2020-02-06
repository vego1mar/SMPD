#include <stdexcept>
#include <algorithm>
#include "nearest_neighbors.hpp"
#include "../statistics/statistics.hpp"

using statistics::Statistics;


namespace classifiers {

    NearestNeighborScore NearestNeighbors::classify(const Input &input, const Cluster &cluster, std::size_t neighbors) {
        NearestNeighborParams params;
        params.input = std::make_unique<Input>(input);
        params.cluster = std::make_unique<Cluster>(cluster);
        params.neighbors = std::make_unique<std::size_t>(neighbors);

        checkPrerequisites(params);
        countDistances(params);
        sortByDistance();
        countLabels(params);

        const auto result = getScore();
        dispose();
        return result;
    }

    void NearestNeighbors::checkPrerequisites(const NearestNeighborParams &params) {
        const auto &neighbors = params.neighbors;
        const auto &cluster = params.cluster;
        const auto &input = params.input;

        if (*neighbors == 0) {
            throw std::invalid_argument("neighbors == 0");
        }

        if (!cluster->isProperlyFormed()) {
            throw std::length_error("!cluster.isProperlyFormed()");
        }

        if (input->empty() || cluster->features.empty()) {
            throw std::invalid_argument("input.empty() || cluster.features.empty()");
        }

        if (input->size() != cluster->features.size()) {
            throw std::invalid_argument("input.size() != cluster.features.size()");
        }

        if (*neighbors > cluster->features[0].size()) {
            throw std::length_error("*neighbors > cluster->features[0].size()");
        }
    }

    void NearestNeighbors::countDistances(const NearestNeighborParams &params) {
        markers = std::make_unique<Markers>();
        const auto &input = params.input;
        const auto &features = params.cluster->features;
        const auto length = features[0].size();

        for (std::size_t i = 0; i < length; i++) {
            auto classPoints = std::make_unique<Input>(selectClass(params, i));
            const auto distance = Statistics::geometricDistance(*input, *classPoints);
            markers->emplace_back(NearestNeighborMarker(distance, i));
        }
    }

    void NearestNeighbors::sortByDistance() {
        auto compareByDistance = [](const NearestNeighborMarker &left, const NearestNeighborMarker &right) {
            return left.distance < right.distance;
        };

        std::sort(markers->begin(), markers->end(), compareByDistance);
    }

    void NearestNeighbors::countLabels(const NearestNeighborParams &params) {
        counts = std::make_unique<LabelsCountMap>();
        const auto &neighbors = params.neighbors;
        const auto &cluster = params.cluster;

        for (std::size_t i = 0; i < *neighbors; i++) {
            const auto &label = cluster->labels[i];
            bool isLabelInMap = counts->find(label) != counts->end();

            if (isLabelInMap) {
                (*counts).at(label) += 1;
                continue;
            }

            (*counts)[label] = 1;
        }
    }

    Input NearestNeighbors::selectClass(const NearestNeighborParams &params, const std::size_t &featureNo) {
        const auto &features = params.cluster->features;
        Input selection;
        selection.reserve(features.size());

        for (const auto &feature : features) {
            const auto &value = feature[featureNo];
            selection.push_back(value);
        }

        return selection;
    }

    NearestNeighborScore NearestNeighbors::getScore() const {
        std::size_t nearest = 0;
        std::size_t total = 0;
        std::string neighbor;

        // Select the neighbor label that was the most chosen one.
        for (const auto &entry : *counts) {
            if (entry.second > nearest) {
                nearest = entry.second;
                neighbor = entry.first;
            }

            total += entry.second;
        }

        return NearestNeighborScore(neighbor, nearest, total);
    }

    void NearestNeighbors::dispose() {
        if (markers != nullptr) {
            markers.reset();
        }

        if (counts != nullptr) {
            counts.reset();
        }
    }

}
