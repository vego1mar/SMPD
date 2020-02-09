#include <algorithm>
#include "classifiers_grouper.hpp"
#include "../helpers/strings.hpp"
#include "../helpers/collections.hpp"
#include "../statistics/randoms.hpp"

using helpers::Collections;
using statistics::RandomInts;


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

    void ClassifiersGrouper::group(const CSVParser &csvParser, const FLD &fld) {
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

}
