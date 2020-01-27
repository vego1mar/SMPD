#include <iostream>
#include "statistical_run.hpp"
#include "../helpers/collections.hpp"
#include "../helpers/combinations.hpp"
#include "../helpers/stringify.hpp"

using data_builders::Headers;
using matrix::Matrix;
using helpers::Collections;
using helpers::Combinations;
using helpers::Stringify;


namespace main_program {

    void StatisticalRun::run(int argc, char **argv) {
        claParser = std::make_unique<CLAParser>(argc, argv);

        if (!areCLIArgumentsOK()) {
            return;
        }

        performRun();
    }

    void StatisticalRun::dispose() {
        if (featuresToSelect != nullptr) {
            featuresToSelect.reset();
        }

        if (claParser != nullptr) {
            claParser.reset();
        }
    }

    void StatisticalRun::performRun() {
        const auto &pathOption = *std::next(CLA_OPTIONS.begin(), 3);
        const auto &pathToCSV = claParser->getOption(pathOption);
        CSVParser parser(pathToCSV);
        parser.build();

        performSelection(parser);
        //TODO: classify

        parser.dispose();
    }

    bool StatisticalRun::areCLIArgumentsOK() {
        if (!claParser->hasArguments()) {
            printHelp();
            return false;
        }

        const auto &selectClasses = *std::next(CLA_OPTIONS.begin(), 0);
        const auto &selectSubclasses = *std::next(CLA_OPTIONS.begin(), 1);
        const auto &featuresNo = *std::next(CLA_OPTIONS.begin(), 2);
        const auto &path = *std::next(CLA_OPTIONS.begin(), 3);

        if (!claParser->isOptionExists(path)) {
            std::cerr << "No path to CSV file!" << std::endl;
            return false;
        }

        bool areNotSelected = !claParser->isOptionExists(selectClasses) && !claParser->isOptionExists(selectSubclasses);
        bool areAllSelected = claParser->isOptionExists(selectClasses) && claParser->isOptionExists(selectSubclasses);

        if (areNotSelected || areAllSelected) {
            std::cerr << "Choose one of: ['" << selectClasses << "','" << selectSubclasses << "']" << std::endl;
            return false;
        }

        if (claParser->isOptionExists(featuresNo)) {
            int argNo = std::stoi(claParser->getOption(featuresNo));
            *featuresToSelect = argNo;
        }

        return true;
    }

    void StatisticalRun::printHelp() {
        std::string helpStr = "USAGE:\n"
                              "./program --file MapleOak.csv --select-classes\n"
                              "./program --file MapleOak.csv --select-subclasses\n"
                              "./program --file MapleOak.csv --select-classes --features-no <number>\n";
        std::cout << helpStr << std::endl;
    }

    void StatisticalRun::performSelection(const CSVParser &csvParser) {
        const auto &selectClasses = *std::next(CLA_OPTIONS.begin(), 0);
        const auto &selectSubclasses = *std::next(CLA_OPTIONS.begin(), 1);

        if (claParser->isOptionExists(selectClasses)) {
            // TODO: performSelectionForTwoClasses(csvParser)
            return;
        }

        if (claParser->isOptionExists(selectSubclasses)) {
            performSelectionForEachLabel(csvParser);
        }
    }

    void StatisticalRun::performSelectionForEachLabel(const CSVParser &csvParser) {
        const Headers &headers = csvParser.getHeaders();
        const auto headersSet = Collections::getDistinct(headers);
        const Matrix &dataset = csvParser.getDataset();
        Combinations combinations(headersSet.size(), 2);
        const std::size_t &COLUMNS = dataset.getColumns();
        FLD fld;

        while (combinations.hasNext()) {
            const auto indices = combinations.getNext();
            const auto &first = indices[0];
            const auto &second = indices[1];
            const auto &firstHeader = *std::next(headersSet.begin(), first);
            const auto &secondHeader = *std::next(headersSet.begin(), second);
            FLDHeader fldHeader(firstHeader, secondHeader);

            std::size_t rowsA = Collections::count(headers, firstHeader);
            std::size_t rowsB = Collections::count(headers, secondHeader);
            Matrix clusterA(COLUMNS, rowsA);
            Matrix clusterB(COLUMNS, rowsB);
            csvParser.getData(firstHeader, clusterA);
            csvParser.getData(secondHeader, clusterB);

            fld.select(*featuresToSelect, clusterA, clusterB);
            const auto &selectedFeatures = fld.getFeatureIndices();

            std::cout << Stringify::toString(fldHeader) << " -> FLD " << Stringify::toString(selectedFeatures) << std::endl;
        }
    }

}
