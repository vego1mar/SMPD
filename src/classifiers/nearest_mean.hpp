#ifndef NEAREST_MEAN_HPP
#define NEAREST_MEAN_HPP

#include <set>
#include "definitions.hpp"


namespace classifiers {

    class NearestMean {
    private:
        std::unique_ptr<Means> means;
        std::unique_ptr<Distances> distances;
        std::unique_ptr<Indices> indices;


    public:
        NearestMean() = default;

        NearestMean(const NearestMean &rhs) = delete;

        NearestMean(NearestMean &&rvalue) noexcept = delete;

        NearestMean &operator=(const NearestMean &rhs) = delete;

        NearestMean &operator=(NearestMean &&rvalue) noexcept = delete;

        virtual ~NearestMean() = default;

        Labels classify(const Matrix &input, const Matrix &clusterA, const Matrix &clusterB, const Labels &labels);

        void dispose();

        void kMeans(const Matrix &cluster, const Labels &labels, std::size_t maxIter = 10'000);


    private:
        static void checkPrerequisites(const NearestMeanArgs &args);

        void countMeans(const NearestMeanArgs &args);

        void countDistances(const NearestMeanArgs &args);

        Labels getResult(const NearestMeanArgs &args);

        static std::set<std::string> getDistinctLabels(const KMeansArgs &args);

        void determineCentroidIndices(const KMeansArgs &args);

        void computeMeans(const KMeansArgs &args);

        void computeDistances(const KMeansArgs &args);

    };

}

#endif //NEAREST_MEAN_HPP
