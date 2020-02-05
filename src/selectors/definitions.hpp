#ifndef SELECTORS_DEFINITIONS_HPP
#define SELECTORS_DEFINITIONS_HPP

#include <vector>
#include <memory>
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

}

#endif //SELECTORS_DEFINITIONS_HPP
