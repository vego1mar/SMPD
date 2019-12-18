#ifndef FISHER_HPP
#define FISHER_HPP

#include "../matrix/matrix.hpp"

using matrix::Matrix;

namespace fisher {

    /// Fisher Linear Discriminant
    class FLD {
    private:
        int featureIndex;

    public:
        FLD();

        FLD(const FLD &rhs) = delete;

        FLD(FLD &&rvalue) noexcept = delete;

        FLD &operator=(const FLD &rhs) = delete;

        FLD &operator=(FLD &&rvalue) noexcept = delete;

        virtual ~FLD() = default;

        std::string toString();

    public:
        int getFeatureIndex();

    public:
        void selectOne(const Matrix &clusterA, const Matrix &clusterB);

        // TODO: below
        //void select(int howMany, const Matrix &clusterA, const Matrix &clusterB);

    };

}

#endif //FISHER_HPP
