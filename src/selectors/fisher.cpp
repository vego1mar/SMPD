#include <string>
#include <memory>
#include <algorithm>
#include "fisher.hpp"
#include "../statistics/statistics.hpp"
#include "../helpers/converters.hpp"
#include "../helpers/collections.hpp"

using statistics::Statistics;
using helpers::Converters;
using helpers::Collections;
using matrix::TransformationType;


namespace selectors {

    std::string FLD::toString() {
        auto addressPtr = std::addressof(*this);
        auto addressLong = reinterpret_cast<unsigned long>(addressPtr);
        auto addressHex = Converters::toHex(addressLong);
        auto toString = "{FLD@" + addressHex + '}';
        return toString;
    }

    int FLD::getFeatureIndex() const {
        return featureIndex;
    }

    const IntVector &FLD::getFeatureIndices() const {
        return featureIndices;
    }

    void FLD::selectOne(const Matrix &clusterA, const Matrix &clusterB) {
        if (clusterA.getRows() != clusterB.getRows()) {
            throw std::invalid_argument("clusterA.getRows() != clusterB.getRows()");
        }

        if (clusterA.getRows() == 1) {
            featureIndex = 0;
            return;
        }

        const std::size_t FEATURES_NO = clusterA.getRows();
        auto coefficients = std::make_unique<FpVector>();
        (*coefficients).reserve(FEATURES_NO);

        for (std::size_t i = 0; i < FEATURES_NO; i++) {
            auto featureA = clusterA.getRow(i);
            auto featureB = clusterB.getRow(i);
            auto meanA = std::make_unique<double>(Statistics::arithmeticMean(featureA));
            auto meanB = std::make_unique<double>(Statistics::arithmeticMean(featureB));
            auto stdDevA = Statistics::standardDeviation(featureA);
            auto stdDevB = Statistics::standardDeviation(featureB);
            auto numerator = std::abs(*meanA - *meanB);
            auto denominator = stdDevA + stdDevB;
            auto coefficient = numerator / denominator;
            coefficients->emplace_back(coefficient);
        }

        featureIndex = Statistics::argMax(*coefficients);
        coefficients.reset();
    }

    void FLD::select(int howMany, const Matrix &clusterA, const Matrix &clusterB) {
        if (clusterA.getRows() != clusterB.getRows()) {
            throw std::invalid_argument("clusterA.getRows() != clusterB.getRows()");
        }

        if (static_cast<std::size_t>(howMany) == clusterA.getRows()) {
            throw std::invalid_argument("static_cast<std::size_t>(howMany) == clusterA.getRows()");
        }

        if (static_cast<std::size_t>(howMany) == 0) {
            throw std::invalid_argument("static_cast<std::size_t>(howMany) == 0");
        }

        auto coefficients = std::vector<double>();
        auto features = std::vector<std::vector<int>>();
        Combinations combinations(clusterA.getRows(), howMany);
        auto meanVectorA = std::make_unique<FpVector>(Statistics::arithmeticMean(clusterA));
        auto meanVectorB = std::make_unique<FpVector>(Statistics::arithmeticMean(clusterB));

        while (combinations.hasNext()) {
            auto featuresIndices = combinations.getNext();
            auto numerator = getNumerator(*meanVectorA, *meanVectorB, featuresIndices);
            auto minorA = getMinorMatrix(clusterA, featuresIndices);
            auto minorB = getMinorMatrix(clusterB, featuresIndices);
            auto negatedMeanA = getNegatedMeanVector(*meanVectorA, featuresIndices);
            auto negatedMeanB = getNegatedMeanVector(*meanVectorB, featuresIndices);
            Matrix matrixA(minorA);
            Matrix matrixB(minorB);
            matrixA.add(negatedMeanA);
            matrixB.add(negatedMeanB);
            Matrix transposedDiffA(matrixA);
            Matrix transposedDiffB(matrixB);
            transposedDiffA.transform(TransformationType::Transposition);
            transposedDiffB.transform(TransformationType::Transposition);
            auto matrixCA = matrixA.multiply(transposedDiffA);
            auto matrixCB = matrixB.multiply(transposedDiffB);
            matrixCA.multiply(1.0 / clusterA.getColumns());
            matrixCB.multiply(1.0 / clusterB.getColumns());
            matrixCA.add(matrixCB);
            auto determinant = matrixCA.getDeterminant();
            auto coefficient = numerator / determinant;
            coefficients.emplace_back(coefficient);
            features.emplace_back(featuresIndices);
        }

        meanVectorA.reset();
        meanVectorB.reset();
        auto argMaxIndex = Statistics::argMax(coefficients);
        featureIndices = features[argMaxIndex];
    }

    double FLD::getNumerator(const FpVector &meanVectorA, const FpVector &meanVectorB, const IntVector &featuresIndices) {
        auto accidentVector = std::make_unique<FpVector>();

        for (const auto &index : featuresIndices) {
            auto difference = meanVectorB[index] - meanVectorA[index];
            accidentVector->emplace_back(difference);
        }

        const auto numerator = Statistics::normL2(*accidentVector);
        accidentVector.reset();
        return numerator;
    }

    Matrix FLD::getMinorMatrix(const Matrix &cluster, const IntVector &featuresIndices) {
        auto minors = std::make_unique<std::vector<std::vector<double>>>();

        for (const auto &index : featuresIndices) {
            minors->emplace_back(cluster.getRow(index));
        }

        Matrix matrix((*minors)[0].size(), minors->size());
        matrix.set(*minors);
        minors.reset();
        return matrix;
    }

    FpVector FLD::getNegatedMeanVector(const FpVector &vector, const IntVector &featuresIndices) {
        std::vector<double> mean;

        for (const auto &index : featuresIndices) {
            mean.push_back(-vector[index]);
        }

        return mean;
    }

    void FLD::selectWithSFS(std::size_t howMany, const Matrix &clusterA, const Matrix &clusterB) {
        if (clusterA.getRows() != clusterB.getRows()) {
            throw std::invalid_argument("clusterA.getRows() != clusterB.getRows()");
        }

        if (howMany == clusterA.getRows()) {
            featureIndices = Collections::getOrdinals(0, clusterA.getRows());
            return;
        }

        if (howMany == 0) {
            featureIndices = IntVector();
            featureIndex = -1;
            return;
        }

        selectOne(clusterA, clusterB);
        auto features = std::make_unique<IntVector>();
        features->push_back(featureIndex);
        auto meanVectorA = std::make_unique<FpVector>(Statistics::arithmeticMean(clusterA));
        auto meanVectorB = std::make_unique<FpVector>(Statistics::arithmeticMean(clusterB));

        for (std::size_t i = 1; i < howMany; i++) {
            CombinationArgs nextArgs;
            nextArgs.combinations = std::make_unique<Combinations>(clusterA.getRows(), static_cast<int>(features->size() + 1));
            const auto &combinations = nextArgs.combinations;

            auto results = CoefficientsData();
            results.coefficients = std::make_unique<FpVector>();
            results.features = std::make_unique<std::vector<IntVector>>();

            SelectionArgs selectArgs;
            selectArgs.clusterA = std::make_unique<Matrix>(clusterA);
            selectArgs.clusterB = std::make_unique<Matrix>(clusterB);
            selectArgs.meanVectorA = std::make_unique<FpVector>(*meanVectorA);
            selectArgs.meanVectorB = std::make_unique<FpVector>(*meanVectorB);

            while (combinations->hasNext()) {
                nextArgs.nextIndices = std::make_unique<IntVector>(combinations->getNext());
                nextArgs.features = std::make_unique<IntVector>(*features);
                seekValidCombination(selectArgs, nextArgs);
                const auto numerator = getNumerator(*meanVectorA, *meanVectorB, *nextArgs.nextIndices);
                const auto denominator = getDenominator(selectArgs);
                const auto coefficient = numerator / denominator;
                results.coefficients->emplace_back(coefficient);
                results.features->push_back(*nextArgs.nextIndices);
            }

            meanVectorA.reset();
            meanVectorB.reset();

            const auto argMaxIndex = Statistics::argMax(*results.coefficients);
            const auto &indicesSetSelection = (*results.features)[argMaxIndex];
            *features = indicesSetSelection;
        }

        featureIndices = IntVector(*features);
        features.reset();
    }

    double FLD::getDenominator(SelectionArgs &args) {
        const auto &clusterA = args.clusterA;
        const auto &clusterB = args.clusterB;
        const auto &meanVectorA = args.meanVectorA;
        const auto &meanVectorB = args.meanVectorB;

        countDenominatorPart(args, *clusterA, *meanVectorA);
        auto matrixA = std::make_unique<Matrix>(*args.denominatorPart);
        countDenominatorPart(args, *clusterB, *meanVectorB);
        auto matrixB = std::make_unique<Matrix>(*args.denominatorPart);
        matrixA->add(*matrixB);
        matrixB.reset();
        auto determinant = matrixA->getDeterminant();
        matrixA.reset();

        return determinant;
    }

    void FLD::countDenominatorPart(SelectionArgs &args, const Matrix &cluster, const FpVector &meanVector) {
        const auto &nextIndices = args.nextIndices;

        auto minor = std::make_unique<Matrix>(getMinorMatrix(cluster, *nextIndices));
        auto negatedMean = std::make_unique<FpVector>(getNegatedMeanVector(meanVector, *nextIndices));
        auto matrix = std::make_unique<Matrix>(*minor);
        minor.reset();
        matrix->add(*negatedMean);
        negatedMean.reset();
        auto transposedDiff = std::make_unique<Matrix>(*matrix);
        transposedDiff->transform(TransformationType::Transposition);
        auto matrixC = std::make_unique<Matrix>(matrix->multiply(*transposedDiff));
        matrix.reset();
        transposedDiff.reset();
        matrixC->multiply(1.0 / cluster.getColumns());

        args.denominatorPart = std::make_unique<Matrix>(*matrixC);
        matrixC.reset();
    }

    void FLD::seekValidCombination(SelectionArgs &args1, const CombinationArgs &args2) {
        const auto &features = args2.features;
        const auto &nextIndices = args2.nextIndices;

        while (true) {
            bool areAllFeaturesPresent = true;

            for (const auto &index : *features) {
                bool isFeaturePresent = std::find((*nextIndices).begin(), (*nextIndices).end(), index) != (*nextIndices).end();

                if (!isFeaturePresent) {
                    *nextIndices = (*args2.combinations).getNext();
                    areAllFeaturesPresent = false;
                    break;
                }
            }

            if (areAllFeaturesPresent) {
                args1.nextIndices = std::make_unique<IntVector>(*nextIndices);
                break;
            }
        }
    }

}
