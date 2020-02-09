#include <iostream>
#include "statistical_run.hpp"
#include "../helpers/stringify.hpp"

using data_builders::Headers;
using matrix::Matrix;
using matrix::TransformationType;
using helpers::Combinations;
using helpers::Stringify;
using classifiers::NearestNeighbors;
using classifiers::NearestNeighborsArgs;
using classifiers::Labels;


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
        performStatisticalRun(parser);
        parser.dispose();
    }

    bool StatisticalRun::areCLIArgumentsOK() {
        if (!claParser->hasArguments()) {
            printHelp();
            return false;
        }

        const auto &featuresNo = *std::next(CLA_OPTIONS.begin(), 0);
        const auto &path = *std::next(CLA_OPTIONS.begin(), 1);
        const auto &neighbors = *std::next(CLA_OPTIONS.begin(), 3);

        if (!claParser->isOptionExists(path)) {
            std::cerr << "No path to CSV file!" << std::endl;
            return false;
        }

        if (claParser->isOptionExists(featuresNo)) {
            int argNo = std::stoi(claParser->getOption(featuresNo));
            *featuresToSelect = argNo;
        }

        if (claParser->isOptionExists(neighbors)) {
            std::size_t argNo = std::stoul(claParser->getOption(neighbors));
            *neighborsNo = argNo;
        }

        return true;
    }

    void StatisticalRun::printHelp() const {
        const auto &featuresNo = *std::next(CLA_OPTIONS.begin(), 0);
        const auto &path = *std::next(CLA_OPTIONS.begin(), 1);
        const auto &sfs = *std::next(CLA_OPTIONS.begin(), 2);
        const auto &neighbors = *std::next(CLA_OPTIONS.begin(), 3);

        /* ./program --path MapleOak.csv
         * ./program --path MapleOak.csv --features-no 3
         * ./program --path MapleOak.csv --features-no 3 --sfs
         * ./program --path MapleOak.csv --features-no 3 --sfs --neighbors 3
         */
        std::string program = "./program ";
        std::string mapleOak = " MapleOak.csv ";
        std::string three = " 3 ";
        std::cout << "USAGE:" << std::endl <<
                  program << path << mapleOak << std::endl <<
                  program << path << mapleOak << featuresNo << three << std::endl <<
                  program << path << mapleOak << featuresNo << three << sfs << std::endl <<
                  program << path << mapleOak << featuresNo << three << sfs << ' ' << neighbors << three << std::endl <<
                  std::endl;
    }

    void StatisticalRun::performStatisticalRun(const CSVParser &csvParser) {
        auto superCluster = std::make_unique<std::vector<Matrix>>();
        auto superHeaders = std::make_unique<Headers>();
        csvParser.regroupAsSuperCluster(*superCluster, *superHeaders);

        for (auto &cluster : *superCluster) {
            cluster.transform(TransformationType::Transposition);
        }

        FLD fld;
        Combinations combinations(superCluster->size(), 2);

        // Combinations will be applied to the (first-worded) label pairs in the CSV file.
        while (combinations.hasNext()) {
            const auto indices = combinations.getNext();
            const auto &first = indices[0];
            const auto &second = indices[1];
            const auto &firstHeader = (*superHeaders)[first];
            const auto &secondIndex = (*superHeaders)[second];
            FLDHeader fldHeader(firstHeader, secondIndex);
            const auto &clusterA = (*superCluster)[first];
            const auto &clusterB = (*superCluster)[second];

            performSelection(clusterA, clusterB, fld);
            printInfo(fldHeader, fld.getFeatureIndices());

            performClassification(csvParser, fld);
        }
    }

    void StatisticalRun::printInfo(const FLDHeader &fldHeader, const IntVector &selectedFeatures) {
        std::cout << Stringify::toString(fldHeader) << " -> FLD " << Stringify::toString(selectedFeatures) << std::endl;
    }

    void StatisticalRun::performSelection(const Matrix &clusterA, const Matrix &clusterB, FLD &fld) {
        const auto sfsOption = *std::next(CLA_OPTIONS.begin(), 2);
        bool isSFSOptionPresent = claParser->isOptionExists(sfsOption);

        if (isSFSOptionPresent) {
            fld.selectWithSFS(*featuresToSelect, clusterA, clusterB);
            return;
        }

        fld.select(*featuresToSelect, clusterA, clusterB);
    }

    void StatisticalRun::performClassification(const CSVParser &csvParser, const FLD &fld) {
        ClassifiersGrouper grouper;
        grouper.group(csvParser, fld);

        NearestNeighborsArgs args;
        args.input = std::make_unique<Matrix>(grouper.getInput());
        args.sourceData = std::make_unique<Matrix>(grouper.getSelection());
        args.sourceLabels = std::make_unique<Labels>(csvParser.getHeaders());

        NearestNeighbors nn;
        args.neighbors = std::make_unique<std::size_t>(1);
        auto result = std::make_unique<NearestNeighborScores>(nn.classify(args));
        auto printArgs = std::make_unique<InfoPrintingArgs>(fld, grouper, *args.neighbors, *result);
        printInfoAboutNN(*printArgs);

        *args.neighbors = *neighborsNo;
        *result = nn.classify(args);
        printArgs = std::make_unique<InfoPrintingArgs>(fld, grouper, *neighborsNo, *result);
        printInfoAboutNN(*printArgs);
        printArgs.reset();

        // TODO: classify -> NM, k means
    }

    void StatisticalRun::printInfoAboutNN(const InfoPrintingArgs &printArgs) {
        const auto &fld = printArgs.fld;
        const auto &grouper = printArgs.grouper;
        const auto &neighbors = printArgs.neighbors;
        const auto &result = printArgs.scores;

        auto toString = std::make_unique<std::string>();
        (*toString) = '(' + Stringify::toString(fld.getFeatureIndices()) + ',' + Stringify::toString(grouper.getInputIndices()) +
                      ") -> NN(" + std::to_string(neighbors) + ") " + Stringify::toString(result);
        std::cout << *toString << std::endl;
        toString.reset();
    }

}
