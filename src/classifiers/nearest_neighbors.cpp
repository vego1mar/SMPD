#include <stdexcept>
#include <algorithm>
#include "nearest_neighbors.hpp"
#include "../statistics/statistics.hpp"
#include "../helpers/collections.hpp"
#include "../helpers/strings.hpp"

using statistics::Statistics;
using helpers::Collections;
using helpers::Strings;


namespace classifiers {

    NearestNeighborScore NearestNeighbors::classify(const Input &input, const Cluster &cluster, std::size_t neighbors) {
        NearestNeighborParams params;
        params.input = std::make_unique<Input>(input);
        params.cluster = std::make_unique<Cluster>(cluster);
        params.neighbors = std::make_unique<std::size_t>(neighbors);

        checkPrerequisites(params);
        countDistances(params);
        sortMarkersByDistance();
        countLabels(params);

        const auto result = getScore();
        dispose();
        return result;
    }

    NearestNeighborScores NearestNeighbors::classify(const Cluster &input, const Cluster &cluster, std::size_t neighbors) {
        // Features values are expected per Cluster line for both parameters.
        const auto &features = input.features;
        NearestNeighborScores scores;

        for (const auto &currentInput : features) {
            const auto result = classify(currentInput, cluster, neighbors);
            scores.emplace_back(result);
        }

        return scores;
    }

    NearestNeighborScores NearestNeighbors::classify(const Cluster &input, const SuperCluster &superCluster, std::size_t neighbors) {
        auto params = std::make_unique<NearestNeighborParams2>();
        params->input = input;
        params->superCluster = superCluster;
        params->neighbors = neighbors;

        checkSuperClusterPrerequisites(*params);
        checkInputClusterPrerequisites(*params);
        countDistances(*params);
        sortSuperMarkersByDistance();
        superCountLabels(*params);

        const auto scores = getScores();
        dispose();
        return scores;
    }

    NearestNeighborScores NearestNeighbors::classify(const NearestNeighborsArgs &args) {
        const auto &source = *args.sourceData;
        const auto &labels = *args.sourceLabels;
        const auto &input = *args.input;
        const auto &neighbors = *args.neighbors;

        if (source.getRows() != labels.size() || input.getColumns() != source.getColumns()) {
            throw std::length_error("source.getRows() != labels.size() || input.getColumns() != source.getColumns()");
        }

        if (neighbors <= 0 || neighbors >= source.getRows()) {
            throw std::invalid_argument("neighbors <= 0 || neighbors >= source.getRows()");
        }

        countDistances(args);
        sortSuperMarkersByDistance();
        superCountLabels(args);
        const auto scores = getScores();
        dispose();
        return scores;
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

    void NearestNeighbors::sortMarkersByDistance() {
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
            const auto &labelIndex = (*markers)[i].index;
            const auto &label = cluster->labels[labelIndex];
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

        // Select the neighbor label that was the most chosen one (using map sorted order).
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

        if (superMarkers != nullptr) {
            superMarkers.reset();
        }

        if (superCounts != nullptr) {
            superCounts.reset();
        }
    }

    void NearestNeighbors::checkSuperClusterPrerequisites(const NearestNeighborParams2 &params) {
        const auto &superCluster = params.superCluster;

        const auto featuresSizeCheck = [](const Cluster &cluster) {
            for (std::size_t j = 0; j < cluster.features.size() - 1; j++) {
                const auto &previousFeature = cluster.features[j];
                const auto &nextFeature = cluster.features[j + 1];

                if (previousFeature.size() != nextFeature.size()) {
                    throw std::length_error("previousFeature.size() != nextFeature.size()");
                }
            }
        };

        for (std::size_t i = 0; i < superCluster.size() - 1; i++) {
            const auto &previousCluster = superCluster[i];
            const auto &nextCluster = superCluster[i + 1];

            if (previousCluster.features.size() != nextCluster.features.size()) {
                throw std::length_error("previousCluster.features.size() != nextCluster.features.size()");
            }

            featuresSizeCheck(previousCluster);
        }

        const auto &lastCluster = superCluster[superCluster.size() - 1];
        featuresSizeCheck(lastCluster);
    }

    void NearestNeighbors::checkInputClusterPrerequisites(const NearestNeighborParams2 &params) {
        const auto &superCluster = params.superCluster;
        const auto &neighbors = params.neighbors;
        std::size_t neighborsMax = 0;

        for (const auto &cluster : superCluster) {
            const auto &features = cluster.features;
            neighborsMax += features[0].size();
        }

        // The maximum neighbors allowed equals to the number of distances to be counted.
        if (neighbors > neighborsMax) {
            throw std::length_error("neighbors > neighborsMax");
        }
    }

    void NearestNeighbors::countDistances(const NearestNeighborParams2 &params) {
        const auto &superCluster = params.superCluster;
        const auto &inputFeatures = params.input.features;
        auto params1a = std::make_unique<NearestNeighborParams>();
        auto params1b = std::make_unique<NearestNeighborParams>();
        params1a->cluster = std::make_unique<Cluster>();
        params1b->cluster = std::make_unique<Cluster>();
        params1b->cluster->features = inputFeatures;
        prepareSuperMarker(params);
        std::size_t distCounter = 0;
        std::size_t inputCounter = 0;

        for (const auto &cluster : superCluster) {
            const auto &features = cluster.features;
            params1a->cluster->features = features;
            const auto featuresSize = features[0].size();
            const auto inputFeaturesSize = inputFeatures[0].size();

            for (std::size_t k = 0; k < featuresSize; k++) {
                auto classPoints = std::make_unique<Input>(selectClass(*params1a, k));

                for (std::size_t j = 0; j < inputFeaturesSize; j++) {
                    auto inputClass = std::make_unique<Input>(selectClass(*params1b, j));
                    const auto distance = Statistics::geometricDistance(*inputClass, *classPoints);
                    (*superMarkers)[inputCounter].emplace_back(NearestNeighborMarker(distance, distCounter));
                    inputCounter++;

                    if (inputCounter >= inputFeaturesSize) {
                        inputCounter = 0;
                        distCounter++;
                    }
                }
            }
        }
    }

    void NearestNeighbors::prepareSuperMarker(const NearestNeighborParams2 &params) {
        superMarkers = std::make_unique<SuperMarkers>();
        const auto inputFeaturesSize = params.input.features[0].size();

        for (std::size_t i = 0; i < inputFeaturesSize; i++) {
            superMarkers->emplace_back();
        }
    }

    void NearestNeighbors::sortSuperMarkersByDistance() {
        auto compareByDistance = [](const NearestNeighborMarker &left, const NearestNeighborMarker &right) {
            return left.distance < right.distance;
        };

        for (auto &markersCluster : *superMarkers) {
            std::sort(markersCluster.begin(), markersCluster.end(), compareByDistance);
        }
    }

    void NearestNeighbors::superCountLabels(const NearestNeighborParams2 &params) {
        superCounts = std::make_unique<SuperCounts>();
        const auto &neighbors = params.neighbors;
        const auto superLabels = std::make_unique<Labels>(getSuperLabels(params));

        for (std::size_t i = 0; i < superMarkers->size(); i++) {
            const auto &currentMarkers = (*superMarkers)[i];
            superCounts->emplace_back(LabelsCountMap());
            auto &currentCounts = (*superCounts)[i];

            for (std::size_t j = 0; j < neighbors; j++) {
                const auto &labelIndex = currentMarkers[j].index;
                const auto &label = (*superLabels)[labelIndex];
                const auto firstWord = Strings::getFirstWord(label);
                bool isLabelInMap = currentCounts.find(firstWord) != currentCounts.end();

                if (isLabelInMap) {
                    currentCounts.at(firstWord) += 1;
                    continue;
                }

                currentCounts[firstWord] = 1;
            }
        }
    }

    Labels NearestNeighbors::getSuperLabels(const NearestNeighborParams2 &params) {
        const auto &superCluster = params.superCluster;
        auto superLabels = std::make_unique<std::vector<Labels>>();
        auto labels = Labels();

        for (const auto &cluster : superCluster) {
            (*superLabels).emplace_back(cluster.labels);
        }

        for (const auto &currentLabels : *superLabels) {
            Collections::join(currentLabels, labels);
        }

        superLabels.reset();
        return labels;
    }

    NearestNeighborScores NearestNeighbors::getScores() {
        NearestNeighborScores scores;

        for (const auto &currentCounts : *superCounts) {
            counts = std::make_unique<LabelsCountMap>(currentCounts);
            scores.emplace_back(getScore());
        }

        return scores;
    }

    void NearestNeighbors::countDistances(const NearestNeighborsArgs &args) {
        superMarkers = std::make_unique<SuperMarkers>();
        const auto &input = *args.input;
        const auto &source = *args.sourceData;

        for (std::size_t i = 0; i < input.getRows(); i++) {
            const auto &currentInput = input.getRow(i);
            superMarkers->emplace_back(Markers());

            for (std::size_t j = 0; j < source.getRows(); j++) {
                const auto &currentSource = source.getRow(j);
                const auto distance = Statistics::geometricDistance(currentInput, currentSource);
                (*superMarkers)[i].emplace_back(NearestNeighborMarker(distance, j));
            }
        }
    }

    void NearestNeighbors::superCountLabels(const NearestNeighborsArgs &args) {
        const auto &neighbors = args.neighbors;
        const auto &labels = args.sourceLabels;
        const auto neighborsMarkers = std::make_unique<SuperMarkers>();
        superCounts = std::make_unique<SuperCounts>();

        for (std::size_t i = 0; i < superMarkers->size(); i++) {
            const auto &innerMarkers = (*superMarkers)[i];
            neighborsMarkers->emplace_back(Markers());

            for (std::size_t j = 0; j < *neighbors; j++) {
                const auto &marker = innerMarkers[j];
                (*neighborsMarkers)[i].emplace_back(marker);
            }
        }

        superMarkers.reset();

        for (std::size_t i = 0; i < neighborsMarkers->size(); i++) {
            const auto &innerMarkers = (*neighborsMarkers)[i];
            superCounts->emplace_back(LabelsCountMap());

            for (const auto &currentMarker : innerMarkers) {
                const auto &index = currentMarker.index;
                const auto &label = (*labels)[index];
                const auto firstWord = Strings::getFirstWord(label);
                auto &currentCounts = (*superCounts)[i];
                bool isLabelInMap = currentCounts.find(firstWord) != currentCounts.end();

                if (isLabelInMap) {
                    currentCounts.at(firstWord) += 1;
                    continue;
                }

                currentCounts[firstWord] = 1;
            }
        }
    }

}
