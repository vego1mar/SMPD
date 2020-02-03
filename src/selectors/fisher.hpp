#ifndef FISHER_HPP
#define FISHER_HPP

#include "../matrix/matrix.hpp"
#include "../helpers/combinations.hpp"

using matrix::Matrix;
using helpers::Combinations;


namespace selectors {

    typedef std::vector<double> FpVector;
    typedef std::vector<int> IntVector;


    struct SelectionArgs {
        std::unique_ptr<Matrix> clusterA;
        std::unique_ptr<Matrix> clusterB;
        std::unique_ptr<IntVector> nextIndices;
        std::unique_ptr<FpVector> meanVectorA;
        std::unique_ptr<FpVector> meanVectorB;
        std::unique_ptr<Matrix> denominatorPart;
    };

    struct CombinationArgs {
        std::unique_ptr<IntVector> features;
        std::unique_ptr<IntVector> nextIndices;
        std::unique_ptr<Combinations> combinations;
    };

    struct CoefficientsData {
        std::unique_ptr<FpVector> coefficients;
        std::unique_ptr<std::vector<IntVector>> features;
    };


    /// Fisher Linear Discriminant
    class FLD {
    private:
        int featureIndex = -1;
        IntVector featureIndices;

    public:
        FLD() = default;

        FLD(const FLD &rhs) = delete;

        FLD(FLD &&rvalue) noexcept = delete;

        FLD &operator=(const FLD &rhs) = delete;

        FLD &operator=(FLD &&rvalue) noexcept = delete;

        virtual ~FLD() = default;

        std::string toString();

        int getFeatureIndex() const;

        const IntVector &getFeatureIndices() const;

        void selectOne(const Matrix &clusterA, const Matrix &clusterB);

        void select(int howMany, const Matrix &clusterA, const Matrix &clusterB);

        void selectWithSFS(std::size_t howMany, const Matrix &clusterA, const Matrix &clusterB);

    private:
        static double getNumerator(const FpVector &meanVectorA, const FpVector &meanVectorB, const IntVector &featuresIndices);

        static Matrix getMinorMatrix(const Matrix &cluster, const IntVector &featuresIndices);

        static FpVector getNegatedMeanVector(const FpVector &vector, const IntVector &featuresIndices);

        static double getDenominator(SelectionArgs &args);

        static void countDenominatorPart(SelectionArgs &args, const Matrix &cluster, const FpVector &meanVector);

        static void seekValidCombination(SelectionArgs &args1, const CombinationArgs &args2);

    };

}

#endif //FISHER_HPP
