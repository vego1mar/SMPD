#include <vector>
#include "selectors.hpp"
#include "helpers.hpp"

namespace selectors {

    std::vector<double> fisher_coefficients(Cluster &cloud1, Cluster &cloud2) {
        helpers::checkInnerVectorSizes(cloud1, cloud2);

        auto mean1 = helpers::getMeanVector(cloud1);
        auto mean2 = helpers::getMeanVector(cloud2);
        auto stdDeviations1 = helpers::getStandardDeviation(cloud1);
        auto stdDeviations2 = helpers::getStandardDeviation(cloud2);
        size_t iterationsNo = mean1.size();
        std::vector<double> coefficients;

        for (size_t i = 0; i < iterationsNo; i++) {
            double coefficient = std::abs(mean1[i] - mean2[i]) / (stdDeviations1[i] - stdDeviations2[i]);
            coefficients.emplace_back(coefficient);
        }

        return coefficients;
    }

    double fisher_coefficient(Cluster &cluster1, Cluster &cluster2) {
        auto coefficients = fisher_coefficients(cluster1, cluster2);
        int index = helpers::getHighestValueIndex(coefficients);
        return coefficients[index];
    }

}
