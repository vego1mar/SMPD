#ifndef NEAREST_NEIGHBORS_HPP
#define NEAREST_NEIGHBORS_HPP

#include <memory>
#include "definitions.hpp"


namespace classifiers {

    class NearestNeighbors {
    private:
        std::unique_ptr<Markers> markers;
        std::unique_ptr<LabelsCountMap> counts;


    public:
        NearestNeighbors() = default;

        NearestNeighbors(const NearestNeighbors &rhs) = delete;

        NearestNeighbors(NearestNeighbors &&rvalue) noexcept = delete;

        NearestNeighbors &operator=(const NearestNeighbors &rhs) = delete;

        NearestNeighbors &operator=(NearestNeighbors &&rvalue) noexcept = delete;

        virtual ~NearestNeighbors() = default;

        NearestNeighborScore classify(const Input &input, const Cluster &cluster, std::size_t neighbors);

    private:
        static void checkPrerequisites(const NearestNeighborParams &params);

        void countDistances(const NearestNeighborParams &params);

        void sortByDistance();

        void countLabels(const NearestNeighborParams &params);

        static Input selectClass(const NearestNeighborParams &params, const std::size_t &featureNo);

        NearestNeighborScore getScore() const;

        void dispose();

    };

}

#endif //NEAREST_NEIGHBORS_HPP
