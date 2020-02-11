#include <random>
#include <chrono>
#include <algorithm>
#include "nearest_neighbors.hpp"
#include "nearest_mean.hpp"
#include "../statistics/statistics.hpp"
#include "../helpers/strings.hpp"
#include "../helpers/collections.hpp"

using statistics::Statistics;
using helpers::Strings;
using helpers::Collections;


namespace classifiers {

    Labels NearestMean::classify(const Matrix &input, const Matrix &clusterA, const Matrix &clusterB, const Labels &labels) {
        const auto args = std::make_unique<NearestMeanArgs>(input, clusterA, clusterB, labels);

        checkPrerequisites(*args);
        countMeans(*args);
        countDistances(*args);
        return getResult(*args);
    }

    void NearestMean::checkPrerequisites(const NearestMeanArgs &args) {
        const auto &clusterA = args.clusterA;
        const auto &clusterB = args.clusterB;
        const auto &input = args.input;
        const auto &labels = args.labels;

        if (clusterA.getColumns() != clusterB.getColumns()) {
            throw std::length_error("clusterA.getColumns() != clusterB.getColumns()");
        }

        if (input.getColumns() != clusterA.getColumns()) {
            throw std::length_error("input.getColumns() != clusterA.getColumns()");
        }

        if (labels.size() < 2) {
            throw std::length_error("labels.size() < 2");
        }
    }

    void NearestMean::countMeans(const NearestMeanArgs &args) {
        const auto &clusterA = args.clusterA;
        const auto &clusterB = args.clusterB;

        means = std::make_unique<Means>();
        means->emplace_back(Statistics::arithmeticMean2(clusterA));
        means->emplace_back(Statistics::arithmeticMean2(clusterB));
    }

    void NearestMean::countDistances(const NearestMeanArgs &args) {
        const auto &input = args.input;

        const auto emplaceDistances = [&input](const Means &means1, Distances &distances1, std::size_t i) {
            const auto &currentInput = input.getRow(i);
            distances1.emplace_back(Input());

            for (std::size_t j = 0; j < 2; j++) {
                const auto distance = Statistics::geometricDistance(currentInput, means1[j]);
                distances1[i].emplace_back(distance);
            }
        };

        distances = std::make_unique<Distances>();

        for (std::size_t i = 0; i < input.getRows(); i++) {
            emplaceDistances(*means, *distances, i);
        }
    }

    Labels NearestMean::getResult(const NearestMeanArgs &args) {
        indices = std::make_unique<Indices>();

        for (const auto &inputDists : *distances) {
            const auto index = Statistics::argMin(inputDists);
            indices->emplace_back(index);
        }

        const auto &labels = args.labels;
        auto resultLabels = Labels();

        for (const auto &index : *indices) {
            const auto &currentLabel = labels[index];
            resultLabels.emplace_back(currentLabel);
        }

        return resultLabels;
    }

    void NearestMean::dispose() {
        if (means != nullptr) {
            means.reset();
        }

        if (distances != nullptr) {
            distances.reset();
        }

        if (indices != nullptr) {
            indices.reset();
        }

        if (centroids != nullptr) {
            centroids.reset();
        }
    }

    Centroids NearestMean::kMeans(const Matrix &cluster, const Labels &labels, std::size_t maxIter, double delta) {
        if (cluster.getColumns() != labels.size()) {
            throw std::length_error("cluster.getColumns() != labels.size()");
        }

        KMeansArgs args(cluster, labels);
        args.distinct = std::make_unique<std::set<std::string>>(getDistinctLabels(args));
        args.k = std::make_unique<std::size_t>(args.distinct->size());
        prepareNextCycleArgs(args);

        if (*args.k >= cluster.getColumns()) {
            throw std::invalid_argument("*args.k >= cluster.getColumns()");
        }

        determineCentroidIndices1(args);
        computeCentroidMeans(args);
        auto previousCentroids = std::make_unique<Centroids>(*centroids);
        *indices = Collections::getOrdinals(args.cluster.getColumns(), args.cluster.getColumns() + *args.k);
        const auto &MAXIMUM_ITERATIONS = maxIter;
        const double &ACCEPTABLE_CHANGE = delta;
        double currentDelta = 1.0;
        std::size_t iter = 0;

        while (iter < MAXIMUM_ITERATIONS && currentDelta > ACCEPTABLE_CHANGE) {
            prepareNextCycleArgs(args, *args.k);
            computeCentroidMeans(args);
            auto nextCentroids = std::make_unique<Centroids>(*centroids);
            auto previousMeans = std::make_unique<Features>(Collections::selectSecond(*previousCentroids));
            auto nextMeans = std::make_unique<Features>(Collections::selectSecond(*nextCentroids));
            currentDelta = Statistics::geometricCloseness(*previousMeans, *nextMeans);
            previousMeans.reset();
            nextMeans.reset();
            *previousCentroids = *nextCentroids;
            nextCentroids.reset();
            iter++;
        }

        return *centroids;
    }

    std::set<std::string> NearestMean::getDistinctLabels(const KMeansArgs &args) {
        const auto &labels = args.labels;
        std::set<std::string> distinct;

        for (const auto &label : labels) {
            const auto firstWord = Strings::getFirstWord(label);
            distinct.insert(firstWord);
        }

        return distinct;
    }

    void NearestMean::determineCentroidIndices1(const KMeansArgs &args) {
        const auto &distinctLabels = *args.distinct;
        const auto &labels = args.labels;
        const auto &cluster = args.cluster;

        auto permutation = std::make_unique<Indices>(Collections::getOrdinals(0L, cluster.getColumns()));
        auto rng = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());
        std::shuffle(permutation->begin(), permutation->end(), rng);
        indices = std::make_unique<Indices>();

        for (const auto &distinct : distinctLabels) {
            for (const auto &index : *permutation) {
                const auto &fullLabel = labels[index];
                const auto &label = Strings::getFirstWord(fullLabel);

                if (label != distinct) {
                    continue;
                }

                indices->emplace_back(index);
                break;
            }
        }
    }

    void NearestMean::computeCentroidMeans(const KMeansArgs &args) {
        computeCentroidDistances(args);
        const auto &cluster = *args.cycleCluster;
        const auto &labels = *args.cycleLabels;
        auto subClasses = std::make_unique<SubClassesDict>();

        const auto buildSubClasses = [&subClasses](const std::string &label, const Input &point) {
            bool isLabelInMap = subClasses->find(label) != subClasses->end();

            if (!isLabelInMap) {
                (*subClasses)[label] = Features();
            }

            (*subClasses)[label].emplace_back(point);
        };

        for (std::size_t i = 0; i < distances->size(); i++) {
            const auto &currentDistances = (*distances)[i];
            const auto &currentPoint = std::make_unique<Input>(cluster.getColumn(i));

            if (currentDistances.empty()) {
                const auto &label = labels[i];
                buildSubClasses(label, *currentPoint);
                continue;
            }

            const auto minIndex = Statistics::argMin(currentDistances);
            const auto centroidIndexInCluster = (*indices)[minIndex];
            const auto &label = labels[centroidIndexInCluster];
            buildSubClasses(label, *currentPoint);
        }

        centroids = std::make_unique<Centroids>();

        for (const auto &entry : *subClasses) {
            const auto &label = entry.first;
            const auto &points = entry.second;
            const auto transposed = std::make_unique<Features>(Collections::transpose(points));
            const auto COLUMNS = points.size();
            const auto ROWS = transposed->size();
            auto subMatrix = std::make_unique<Matrix>(COLUMNS, ROWS);
            subMatrix->set(*transposed);
            (*centroids)[label] = Statistics::arithmeticMean(*subMatrix);
        }

        subClasses.reset();
    }

    void NearestMean::computeCentroidDistances(const KMeansArgs &args) {
        const auto &cluster = *args.cycleCluster;
        distances = std::make_unique<Distances>();

        for (std::size_t i = 0; i < cluster.getColumns(); i++) {
            distances->emplace_back(Input());
            const auto currentPoint = *std::make_unique<Input>(cluster.getColumn(i));
            bool isCentroid = std::find(indices->begin(), indices->end(), i) != indices->end();

            if (isCentroid) {
                continue;
            }

            for (std::size_t j = 0; j < cluster.getColumns(); j++) {
                isCentroid = std::find(indices->begin(), indices->end(), j) != indices->end();

                if (!isCentroid) {
                    continue;
                }

                const auto centroid = *std::make_unique<Input>(cluster.getColumn(j));
                const auto distance = Statistics::geometricDistance(currentPoint, centroid);
                (*distances)[i].emplace_back(distance);
            }
        }
    }

    void NearestMean::prepareNextCycleArgs(KMeansArgs &args, std::size_t columnsOffset) {
        const auto &cluster = args.cluster;
        const auto &labels = args.labels;
        auto &cycleCluster = args.cycleCluster;
        auto &cycleLabels = args.cycleLabels;
        cycleLabels = std::make_unique<Labels>();
        const auto COLUMNS = cluster.getColumns() + columnsOffset;
        const auto ROWS = cluster.getRows();
        cycleCluster = std::make_unique<Matrix>(COLUMNS, ROWS);
        auto dataset = std::make_unique<Features>();

        for (std::size_t i = 0; i < cluster.getColumns(); i++) {
            const auto &label = labels[i];
            const auto column = std::make_unique<Input>(cluster.getColumn(i));
            dataset->emplace_back(*column);
            cycleLabels->emplace_back(label);
        }

        if (centroids != nullptr || columnsOffset != 0) {
            for (const auto &centroid : *centroids) {
                const auto &label = centroid.first;
                const auto &center = centroid.second;
                dataset->emplace_back(center);
                cycleLabels->emplace_back(label);
            }
        }

        *dataset = Collections::transpose(*dataset);
        cycleCluster->set(*dataset);
    }

}
