#include <string>
#include "fisher.hpp"
#include "../statistics/statistics.hpp"
#include "../helpers/converters.hpp"
#include "../helpers/combinations.hpp"

using statistics::Statistics;
using helpers::Converters;
using helpers::Combinations;

namespace fisher {

    FLD::FLD()
            : featureIndex(-1), featureIndices({}) {
    }

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

    const std::vector<int> &FLD::getFeatureIndices() const {
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
        auto coefficients = std::vector<double>();
        coefficients.reserve(FEATURES_NO);

        for (std::size_t i = 0; i < FEATURES_NO; i++) {
            auto featureA = clusterA.getRow(i);
            auto featureB = clusterB.getRow(i);
            auto meanA = Statistics::arithmeticMean(featureA);
            auto meanB = Statistics::arithmeticMean(featureB);
            auto stdDevA = Statistics::standardDeviation(featureA);
            auto stdDevB = Statistics::standardDeviation(featureB);
            auto numerator = std::abs(meanA - meanB);
            auto denominator = stdDevA + stdDevB;
            auto coefficient = numerator / denominator;
            coefficients.emplace_back(coefficient);
        }

        featureIndex = Statistics::argMax(coefficients);
    }

    void FLD::select(int howMany, const Matrix &clusterA, const Matrix &clusterB) {
        if (clusterA.getRows() != clusterB.getRows()) {
            throw std::invalid_argument("clusterA.getRows() != clusterB.getRows()");
        }

        if (howMany == clusterA.getRows()) {
            throw std::invalid_argument("howMany == clusterA.getRows()");
        }

        auto coefficients = std::vector<double>();
        auto features = std::vector<std::vector<int>>();
        Combinations combinations(clusterA.getRows(), howMany);
        auto meanVectorA = Statistics::arithmeticMean(clusterA);
        auto meanVectorB = Statistics::arithmeticMean(clusterB);
        using matrix::TransformationType;

        while (combinations.hasNext()) {
            auto featuresIndices = combinations.getNext();
            auto numerator = getNumerator(meanVectorA, meanVectorB, featuresIndices);
            auto minorA = getMinorMatrix(clusterA, featuresIndices); // x
            auto minorB = getMinorMatrix(clusterB, featuresIndices);
            auto negatedMeanA = getNegatedMeanVector(meanVectorA, featuresIndices);
            auto negatedMeanB = getNegatedMeanVector(meanVectorB, featuresIndices);
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

        auto argMaxIndex = Statistics::argMax(coefficients);
        featureIndices = features[argMaxIndex];
    }

    double FLD::getNumerator(const FpVector &meanVectorA, const FpVector &meanVectorB, const IntVector &featuresIndices) {
        FpVector accidentVector;

        for (const auto &index : featuresIndices) {
            auto difference = meanVectorB[index] - meanVectorA[index];
            accidentVector.emplace_back(difference);
        }

        return Statistics::normL2(accidentVector);
    }

    Matrix FLD::getMinorMatrix(const Matrix &cluster, const IntVector &featuresIndices) {
        std::vector<std::vector<double>> minors;

        for (std::size_t i = 0; i < featuresIndices.size(); i++) {
            auto minor = cluster.getRow(i);
            minors.emplace_back(minor);
        }

        Matrix matrix(minors[0].size(), minors.size());
        matrix.set(minors);
        return matrix;
    }

    std::vector<double> FLD::getNegatedMeanVector(const FpVector &vector, const IntVector &featuresIndices) {
        std::vector<double> mean;

        for (const auto &index : featuresIndices) {
            mean.push_back(-vector[index]);
        }

        return mean;
    }

}
