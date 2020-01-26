#ifndef FISHER_HPP
#define FISHER_HPP

#include "../matrix/matrix.hpp"

using matrix::Matrix;


namespace fisher {

    typedef std::vector<double> FpVector;
    typedef std::vector<int> IntVector;


    /// Fisher Linear Discriminant
    class FLD {
    private:
        int featureIndex;
        std::vector<int> featureIndices;

    public:
        FLD();

        FLD(const FLD &rhs) = delete;

        FLD(FLD &&rvalue) noexcept = delete;

        FLD &operator=(const FLD &rhs) = delete;

        FLD &operator=(FLD &&rvalue) noexcept = delete;

        virtual ~FLD() = default;

        std::string toString();

    public:
        int getFeatureIndex() const;

        const std::vector<int> &getFeatureIndices() const;

    public:
        void selectOne(const Matrix &clusterA, const Matrix &clusterB);

        void select(int howMany, const Matrix &clusterA, const Matrix &clusterB);


    private:
        static double getNumerator(const FpVector &meanVectorA, const FpVector &meanVectorB, const IntVector &featuresIndices);

        static Matrix getMinorMatrix(const Matrix &cluster, const IntVector &featuresIndices);

        static std::vector<double> getNegatedMeanVector(const FpVector &vector, const IntVector &featuresIndices);

    };

}

#endif //FISHER_HPP
