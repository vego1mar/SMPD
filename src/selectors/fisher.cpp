#include <string>
#include "fisher.hpp"
#include "../statistics/statistics.hpp"
#include "../helpers/converters.hpp"

using statistics::Statistics;
using helpers::Converters;

namespace fisher {

    FLD::FLD()
            : featureIndex(-1) {
    }

    std::string FLD::toString() {
        auto addressPtr = std::addressof(*this);
        auto addressLong = reinterpret_cast<unsigned long>(addressPtr);
        auto addressHex = Converters::toHex(addressLong);
        auto toString = "{FLD@" + addressHex + '}';
        return toString;
    }

    int FLD::getFeatureIndex() {
        return featureIndex;
    }

    void FLD::selectOne(const Matrix &clusterA, const Matrix &clusterB) {
        // Assumption: rows corresponds to features.
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

}
