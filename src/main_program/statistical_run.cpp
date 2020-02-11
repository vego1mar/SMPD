#include <iostream>
#include "statistical_run.hpp"
#include "../helpers/stringify.hpp"
#include "../classifiers/nearest_mean.hpp"

using data_builders::Headers;
using matrix::Matrix;
using matrix::TransformationType;
using helpers::Combinations;
using helpers::Stringify;
using classifiers::NearestNeighbors;
using classifiers::NearestNeighborsArgs;
using classifiers::NearestMean;
using classifiers::LabelsCountMap;


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

        if (neighborsNo != nullptr) {
            neighborsNo.reset();
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
        StatisticalRunParams params(csvParser, fld);

        // Combinations will be applied to the (first-worded) label pairs in the CSV file.
        while (combinations.hasNext()) {
            const auto indices = combinations.getNext();
            const auto &first = indices[0];
            const auto &second = indices[1];
            const auto &firstHeader = (*superHeaders)[first];
            const auto &secondHeader = (*superHeaders)[second];
            FLDHeader fldHeader(firstHeader, secondHeader);

            const auto &clusterA = (*superCluster)[first];
            const auto &clusterB = (*superCluster)[second];
            params.infoHeader = std::make_unique<FLDHeader>(fldHeader);
            params.clusterA = std::make_unique<Matrix>(clusterA);
            params.clusterB = std::make_unique<Matrix>(clusterB);

            performSelection(params, fld);
            performClassification(params);
        }
    }

    void StatisticalRun::printSelectionInfo(const StatisticalRunParams &params) {
        const auto &fldHeader = *params.infoHeader;
        const auto &selectedFeatures = params.fld.getFeatureIndices();
        std::cout << Stringify::toString(fldHeader) << " -> FLD " << Stringify::toString(selectedFeatures) << std::endl;
    }

    void StatisticalRun::performSelection(const StatisticalRunParams &params, FLD &fld) {
        const auto &clusterA = *params.clusterA;
        const auto &clusterB = *params.clusterB;

        const auto sfsOption = *std::next(CLA_OPTIONS.begin(), 2);
        bool isSFSOptionPresent = claParser->isOptionExists(sfsOption);

        if (isSFSOptionPresent) {
            fld.selectWithSFS(*featuresToSelect, clusterA, clusterB);
            printSelectionInfo(params);
            return;
        }

        fld.select(*featuresToSelect, clusterA, clusterB);
        printSelectionInfo(params);
    }

    void StatisticalRun::performClassification(StatisticalRunParams &params) {
        const auto &csvParser = params.csvParser;
        const auto &fld = params.fld;
        const auto &nmLabels = *params.infoHeader;
        const auto &clusterA = *params.clusterA;
        const auto &clusterB = *params.clusterB;

        ClassifiersGrouper grouper;
        grouper.groupNN(csvParser, fld);
        grouper.groupNM(clusterA, clusterB, fld);

        NearestNeighborsArgs args;
        args.input = std::make_unique<Matrix>(grouper.getInput());
        args.sourceData = std::make_unique<Matrix>(grouper.getSelection());
        args.sourceLabels = std::make_unique<Labels>(csvParser.getHeaders());

        NearestNeighbors nn;
        args.neighbors = std::make_unique<std::size_t>(1);
        auto result = std::make_unique<NearestNeighborScores>(nn.classify(args));
        params.neighbors = std::make_unique<std::size_t>(*args.neighbors);
        params.nnResult = std::make_unique<NearestNeighborScores>(*result);
        printNearestNeighborsInfo(params, grouper);

        *args.neighbors = *neighborsNo;
        *result = nn.classify(args);
        *params.neighbors = *args.neighbors;
        *params.nnResult = *result;
        printNearestNeighborsInfo(params, grouper);
        result.reset();

        NearestMean nm;
        const auto labels = Labels({nmLabels.first, nmLabels.second});
        auto result2 = std::make_unique<Labels>(nm.classify(grouper.getSelection(), grouper.getClusterA(), grouper.getClusterB(), labels));
        params.nmResult = std::make_unique<Labels>(*result2);
        printNearestMeanInfo(params);
        result2.reset();

        auto result3 = std::make_unique<Centroids>(nm.kMeans(grouper.getDatasetTransposed(csvParser), csvParser.getHeaders()));
        params.kMeansResult = std::make_unique<Centroids>(*result3);
        result3.reset();
        printKMeansInfo(params);
    }

    void StatisticalRun::printNearestNeighborsInfo(const StatisticalRunParams &params, const ClassifiersGrouper &grouper) {
        const auto &fld = params.fld;
        const auto &neighbors = *params.neighbors;
        const auto &result = *params.nnResult;

        auto nearest = std::make_unique<LabelsCountMap>();
        auto total = std::make_unique<LabelsCountMap>();

        for (const auto &score : result) {
            const auto &label = score.label;
            bool isLabelInMap = nearest->find(label) != nearest->end();

            if (!isLabelInMap) {
                (*nearest)[label] = score.nearestCount;
                (*total)[label] = score.totalCount;
                continue;
            }

            (*nearest)[label] += score.nearestCount;
            (*total)[label] += score.totalCount;
        }

        auto toString = std::make_unique<std::string>();
        *toString = '(' + Stringify::toString(fld.getFeatureIndices()) + ',' + Stringify::toString(grouper.getInputIndices()) +
                    ") -> NN(" + std::to_string(neighbors) + ") ";
        auto dictStr = std::make_unique<std::string>();
        auto nearestIt = nearest->begin();
        auto totalIt = total->begin();
        *dictStr += '[';

        while (nearestIt != nearest->end()) {
            const auto &label = nearestIt->first;
            const auto &nearestNo = nearestIt->second;
            const auto &totalNo = totalIt->second;
            *dictStr += '(' + label + ',' + std::to_string(nearestNo) + '/' + std::to_string(totalNo) + "),";
            nearestIt++;
            totalIt++;
        }

        if (dictStr->size() > 1) {
            *dictStr = dictStr->substr(0, dictStr->size() - 1);
        }

        *dictStr += ']';
        *toString += *dictStr;
        dictStr.reset();
        std::cout << *toString << std::endl;
        toString.reset();
    }

    void StatisticalRun::printNearestMeanInfo(const StatisticalRunParams &params) {
        const auto &results = *params.nmResult;
        auto counts = std::make_unique<LabelsCountMap>();

        for (const auto &label : results) {
            bool isLabelInMap = counts->find(label) != counts->end();

            if (!isLabelInMap) {
                (*counts)[label] = 1;
                continue;
            }

            (*counts)[label] += 1;
        }

        const auto &headers = params.infoHeader;
        auto featuresStr = std::make_unique<std::string>(std::to_string(params.fld.getFeatureIndices().size()));
        auto indicesStr = std::make_unique<std::string>(Stringify::toString(params.fld.getFeatureIndices()));

        auto toString = std::make_unique<std::string>();
        *toString = '(' + headers->first + '(' + *featuresStr + ")," + headers->second + '(' + *featuresStr + ")," + *indicesStr +
                    ") -> NM" + Stringify::toString(*counts);
        featuresStr.reset();
        indicesStr.reset();
        std::cout << *toString << std::endl;
    }

    void StatisticalRun::printKMeansInfo(const StatisticalRunParams &params) {
        const auto &headers = params.infoHeader;
        const auto &centroids = *params.kMeansResult;

        auto toString = std::make_unique<std::string>();
        auto centroidsStr = std::make_unique<std::string>(Stringify::toString(centroids));
        *toString += '(' + headers->first + '+' + headers->second + ") -> k means " + *centroidsStr;
        centroidsStr.reset();

        std::cout << *toString << std::endl;
        toString.reset();
    }

}
