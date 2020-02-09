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
        return *input;
    }

    void ClassifiersGrouper::selectInputData(const CSVParser &csvParser, const FLD &fld) {
        const auto &dataset = csvParser.getDataset();
        const auto &features = fld.getFeatureIndices();
        input = std::make_unique<Matrix>(dataset.getColumns() - features.size(), dataset.getRows());
        auto data = std::make_unique<std::vector<std::vector<double>>>();

        // TODO: randomize selection of 3 new features indices!
        const auto rng = std::make_unique<RandomInts<std::size_t>>(0, dataset.getColumns());
        auto randoms = std::vector<std::size_t>();

        for (std::size_t i = 0; i < features.size(); i++) {
            //const auto &featureColumn = dataset.getColumn(i);
            //data->emplace_back(featureColumn);
            const auto randomValue = (*rng)();
            randoms.emplace_back(randomValue);
        }

        *data = Collections::transpose(*data);
        input->set(*data);
        data.reset();
    }

}
