#include <set>
#include <algorithm>
#include <functional>
#include "classifiers_grouper.hpp"
#include "../helpers/strings.hpp"
#include "../helpers/collections.hpp"

using helpers::Strings;
using classifiers::Cluster;
using classifiers::Labels;
using classifiers::Features;
using matrix::TransformationType;
using helpers::Collections;


namespace data_builders {

    void ClassifiersGrouper::selectFeaturesData(const CSVParser &csvParser, const FLD &fld) {
        const auto &dataset = csvParser.getDataset();
        const auto &features = fld.getFeatureIndices();
        const auto &headers = csvParser.getHeaders();
        selectionData = std::make_unique<Matrix>(dataset.getColumns(), features.size());
        selectionLabels = std::make_unique<Headers>();
        auto data = std::make_unique<std::vector<std::vector<double>>>();

        for (const auto &index : features) {
            const auto &featureRow = dataset.getRow(index);
            const auto &label = headers[index];
            data->push_back(featureRow);
            selectionLabels->emplace_back(label);
        }

        selectionData->set(*data);
        data.reset();
    }

    void ClassifiersGrouper::makeSuperCluster(const CSVParser &csvParser, const FLD &fld) {
        selectFeaturesData(csvParser, fld);
        const auto &selection = *selectionData;
        const auto &labels = *selectionLabels;
        selectionSuperCluster = std::make_unique<SuperCluster>();
        auto &superCluster = *selectionSuperCluster;
        auto distinctClassLabels = std::make_unique<std::set<std::string>>();
        auto distinctDict = std::make_unique<std::unordered_map<std::string, std::size_t>>();

        for (const auto &fullLabel : labels) {
            const auto firstWord = Strings::getFirstWord(fullLabel);
            distinctClassLabels->insert(firstWord);
        }

        for (std::size_t i = 0; i < distinctClassLabels->size(); i++) {
            const auto &label = *std::next(distinctClassLabels->begin(), i);
            (*distinctDict)[label] = i;
        }

        distinctClassLabels.reset();
        auto cluster = Cluster();
        cluster.labels = Labels();
        cluster.features = Features();

        for (std::size_t i = 0; i < distinctDict->size(); i++) {
            superCluster.push_back(cluster);
        }

        for (std::size_t i = 0; i < selection.getRows(); i++) {
            const auto &row = selection.getRow(i);
            const auto &fullLabel = labels[i];
            const auto classLabel = Strings::getFirstWord(fullLabel);
            const auto &index = (*distinctDict)[classLabel];
            auto &currentCluster = superCluster[index];
            currentCluster.features.emplace_back(row);
            currentCluster.labels.emplace_back(classLabel);
        }

        for (auto &innerCluster : superCluster) {
            auto &innerFeatures = innerCluster.features;
            innerFeatures = Collections::transpose(innerFeatures);
        }

        selectionData.reset();
        selectionLabels.reset();
    }

    const SuperCluster &ClassifiersGrouper::getSuperCluster() const {
        return *selectionSuperCluster;
    }

    void ClassifiersGrouper::makeInputCluster(const CSVParser &csvParser, const FLD &fld) {
        // TODO: change labels grouping
        selectNonFeaturesData(csvParser, fld);
        throw std::bad_function_call();
    }

    void ClassifiersGrouper::selectNonFeaturesData(const CSVParser &csvParser, const FLD &fld) {
        const auto &dataset = csvParser.getDataset();
        const auto &features = fld.getFeatureIndices();
        const auto &headers = csvParser.getHeaders();
        selectionData = std::make_unique<Matrix>(dataset.getColumns(), dataset.getColumns() - features.size());
        selectionLabels = std::make_unique<Headers>();
        auto data = std::make_unique<std::vector<std::vector<double>>>();

        for (std::size_t i = 0; i < dataset.getColumns(); i++) {
            bool isIndexOfSelectedFeature = std::find(features.begin(), features.end(), i) != features.end();

            if (isIndexOfSelectedFeature) {
                continue;
            }

            const auto &featureRow = dataset.getRow(i);
            const auto &label = headers[i];
            data->push_back(featureRow);
            selectionLabels->emplace_back(label);
        }

        selectionData->set(*data);
        data.reset();
    }

}
