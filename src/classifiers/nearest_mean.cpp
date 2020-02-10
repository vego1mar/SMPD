#include "nearest_neighbors.hpp"
#include "nearest_mean.hpp"
#include "../statistics/statistics.hpp"

using statistics::Statistics;


namespace classifiers {

    Labels NearestMean::classify(const Matrix &input, const Matrix &clusterA, const Matrix &clusterB, const Labels &labels) {
        const auto args = std::make_unique<NearestMeanArgs>(input, clusterA, clusterB, labels);

        checkPrerequisites(*args);
        countMeans(*args);
        countDistances(*args);
        return getResult(*args);
    }

    void NearestMean::checkPrerequisites(const NearestMeanArgs &args) {
        const auto &clusterA = args.clusterA;
        const auto &clusterB = args.clusterB;
        const auto &input = args.input;
        const auto &labels = args.labels;

        if (clusterA.getColumns() != clusterB.getColumns()) {
            throw std::length_error("clusterA.getColumns() != clusterB.getColumns()");
        }

        if (input.getColumns() != clusterA.getColumns()) {
            throw std::length_error("input.getColumns() != clusterA.getColumns()");
        }

        if (labels.size() < 2) {
            throw std::length_error("labels.size() < 2");
        }
    }

    void NearestMean::countMeans(const NearestMeanArgs &args) {
        const auto &clusterA = args.clusterA;
        const auto &clusterB = args.clusterB;

        means = std::make_unique<Means>();
        means->emplace_back(Statistics::arithmeticMean2(clusterA));
        means->emplace_back(Statistics::arithmeticMean2(clusterB));
    }

    void NearestMean::countDistances(const NearestMeanArgs &args) {
        const auto &input = args.input;

        const auto emplaceDistances = [&input](const Means &means1, Distances &distances1, std::size_t i) {
            const auto &currentInput = input.getRow(i);
            distances1.emplace_back(Input());

            for (std::size_t j = 0; j < 2; j++) {
                const auto distance = Statistics::geometricDistance(currentInput, means1[j]);
                distances1[i].emplace_back(distance);
            }
        };

        distances = std::make_unique<Distances>();

        for (std::size_t i = 0; i < input.getRows(); i++) {
            emplaceDistances(*means, *distances, i);
        }
    }

    Labels NearestMean::getResult(const NearestMeanArgs &args) {
        indices = std::make_unique<Indices>();

        for (const auto &inputDists : *distances) {
            const auto index = Statistics::argMin(inputDists);
            indices->emplace_back(index);
        }

        const auto &labels = args.labels;
        auto resultLabels = Labels();

        for (const auto &index : *indices) {
            const auto &currentLabel = labels[index];
            resultLabels.emplace_back(currentLabel);
        }

        return resultLabels;
    }

}
