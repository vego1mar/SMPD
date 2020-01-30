#include <iostream>
#include "statistical_run.hpp"
#include "../helpers/combinations.hpp"
#include "../helpers/stringify.hpp"

using data_builders::Headers;
using matrix::Matrix;
using matrix::TransformationType;
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
        const auto &pathOption = *std::next(CLA_OPTIONS.begin(), 1);
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

        const auto &featuresNo = *std::next(CLA_OPTIONS.begin(), 0);
        const auto &path = *std::next(CLA_OPTIONS.begin(), 1);

        if (!claParser->isOptionExists(path)) {
            std::cerr << "No path to CSV file!" << std::endl;
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
                              "./program --file MapleOak.csv\n"
                              "./program --file MapleOak.csv --features-no <number>\n";
        std::cout << helpStr << std::endl;
    }

    void StatisticalRun::performSelection(const CSVParser &csvParser) {
        auto superCluster = std::make_unique<std::vector<Matrix>>();
        auto superHeaders = std::make_unique<Headers>();
        csvParser.regroupAsSuperCluster(*superCluster, *superHeaders);

        for (auto &cluster : *superCluster) {
            cluster.transform(TransformationType::Transposition);
        }

        FLD fld;
        Combinations combinations(superCluster->size(), 2);

        while (combinations.hasNext()) {
            const auto indices = combinations.getNext();
            const auto &first = indices[0];
            const auto &second = indices[1];
            const auto &firstHeader = (*superHeaders)[first];
            const auto &secondIndex = (*superHeaders)[second];
            FLDHeader fldHeader(firstHeader, secondIndex);

            const auto &clusterA = (*superCluster)[first];
            const auto &clusterB = (*superCluster)[second];
            fld.select(*featuresToSelect, clusterA, clusterB);

            const auto &features = fld.getFeatureIndices();
            printInfo(fldHeader, features);
        }
    }

    void StatisticalRun::printInfo(const FLDHeader &fldHeader, const IntVector &selectedFeatures) {
        std::cout << Stringify::toString(fldHeader) << " -> FLD " << Stringify::toString(selectedFeatures) << std::endl;
    }

}
