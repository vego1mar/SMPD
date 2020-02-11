#include <algorithm>
#include "classifiers_grouper.hpp"
#include "../helpers/strings.hpp"
#include "../helpers/collections.hpp"
#include "../statistics/randoms.hpp"

using helpers::Collections;
using statistics::RandomInts;
using matrix::TransformationType;


namespace data_builders {

    void ClassifiersGrouper::selectFeaturesData(const CSVParser &csvParser, const FLD &fld) {
        const auto &dataset = csvParser.getDataset();
        const auto &features = fld.getFeatureIndices();
        selection = std::make_unique<Matrix>(features.size(), dataset.getRows());
        auto data = std::make_unique<std::vector<std::vector<double>>>();

        for (const auto &index : features) {
            const auto &featureColumn = dataset.getColumn(index);
            data->push_back(featureColumn);
        }

        *data = Collections::transpose(*data);
        selection->set(*data);
        data.reset();
    }

    const Matrix &ClassifiersGrouper::getSelection() const {
        return *selection;
    }

    void ClassifiersGrouper::groupNN(const CSVParser &csvParser, const FLD &fld) {
        selectFeaturesData(csvParser, fld);
        selectInputData(csvParser, fld);
    }

    const Matrix &ClassifiersGrouper::getInput() const {
        return *inputData;
    }

    void ClassifiersGrouper::selectInputData(const CSVParser &csvParser, const FLD &fld) {
        const auto &dataset = csvParser.getDataset();
        const auto &features = fld.getFeatureIndices();
        inputData = std::make_unique<Matrix>(features.size(), dataset.getRows());
        auto data = std::make_unique<std::vector<std::vector<double>>>();
        const auto rng = std::make_unique<RandomInts<std::size_t>>(0, dataset.getColumns());
        inputIndices = std::make_unique<std::vector<std::size_t>>();

        for (std::size_t i = 0; i < features.size(); i++) {
            const auto randomValue = (*rng)();
            const auto &featureColumn = dataset.getColumn(randomValue);
            inputIndices->emplace_back(randomValue);
            data->emplace_back(featureColumn);
        }

        *data = Collections::transpose(*data);
        inputData->set(*data);
        data.reset();
    }

    const std::vector<std::size_t> &ClassifiersGrouper::getInputIndices() const {
        return *inputIndices;
    }

    void ClassifiersGrouper::groupNM(const Matrix &cluster1, const Matrix &cluster2, const FLD &fld) {
        typedef std::vector<std::vector<double>> Values;

        clusterA = std::make_unique<Matrix>(cluster1.getColumns(), cluster1.getRows());
        clusterB = std::make_unique<Matrix>(cluster2.getColumns(), cluster2.getRows());
        auto valuesA = std::make_unique<Values>();
        auto valuesB = std::make_unique<Values>();

        for (std::size_t i = 0; i < cluster1.getRows(); i++) {
            valuesA->emplace_back(cluster1.getRow(i));
        }

        for (std::size_t j = 0; j < cluster2.getRows(); j++) {
            valuesB->emplace_back(cluster2.getRow(j));
        }

        clusterA->set(*valuesA);
        clusterB->set(*valuesB);
        valuesA->clear();
        valuesB->clear();
        clusterA->transform(TransformationType::Transposition);
        clusterB->transform(TransformationType::Transposition);
        const auto FEATURES_NO = fld.getFeatureIndices().size();

        for (std::size_t i = 0; i < FEATURES_NO; i++) {
            valuesA->emplace_back(clusterA->getColumn(i));
            valuesB->emplace_back(clusterB->getColumn(i));
        }

        clusterA = std::make_unique<Matrix>(clusterA->getRows(), FEATURES_NO);
        clusterB = std::make_unique<Matrix>(clusterB->getRows(), FEATURES_NO);
        clusterA->set(*valuesA);
        clusterB->set(*valuesB);
        valuesA.reset();
        valuesB.reset();
        clusterA->transform(TransformationType::Transposition);
        clusterB->transform(TransformationType::Transposition);
    }

    const Matrix &ClassifiersGrouper::getClusterA() const {
        return *clusterA;
    }

    const Matrix &ClassifiersGrouper::getClusterB() const {
        return *clusterB;
    }

    const Matrix &ClassifiersGrouper::getDatasetTransposed(const CSVParser &csvParser) {
        typedef std::vector<std::vector<double>> MatrixValues;

        const auto &dataset = csvParser.getDataset();
        datasetTransposed = std::make_unique<Matrix>(dataset.getColumns(), dataset.getRows());
        auto data = std::make_unique<MatrixValues>();

        for (std::size_t i = 0; i < dataset.getRows(); i++) {
            data->emplace_back(dataset.getRow(i));
        }

        datasetTransposed->set(*data);
        data.reset();
        datasetTransposed->transform(TransformationType::Transposition);
        return *datasetTransposed;
    }

}
