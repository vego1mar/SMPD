#include <stdexcept>
#include <algorithm>
#include <unordered_map>
#include "classifiers.hpp"
#include "statistical.hpp"

namespace classifiers {
    ClassVector::ClassVector(std::string label, std::vector<double> features)
            : label(std::move(label)), features(std::move(features)) {
    }

    ClassVector &ClassVector::operator=(const ClassVector &rhs) {
        if (this == &rhs) {
            return *this;
        }

        label = rhs.label;
        features = rhs.features;
        return *this;
    }

    ClassVector &ClassVector::operator=(ClassVector &&rvalue) noexcept {
        if (this == &rvalue) {
            return *this;
        }

        std::swap(label, rvalue.label);
        std::swap(features, rvalue.features);
        return *this;
    }

    std::string ClassVector::getIdentifier() const {
        return label;
    }

    std::vector<double> ClassVector::getFeatures() const {
        return std::vector<double>(features);
    }

    bool ClassVector::hasSameFeaturesSize(const std::vector<double> &object) const {
        return features.size() == object.size();
    }

    std::string nearest_neighbor(std::vector<double> &input, std::vector<ClassVector> &cluster, u_short k) {
        if (k == 0) {
            throw std::invalid_argument("Parameter of neighbors is set to be k=0.");
        }

        for (const auto &vector : cluster) {
            if (!vector.hasSameFeaturesSize(input)) {
                throw std::invalid_argument("Features sizes differs!");
            }
        }

        // marker := ["class_label", distance_to_input, index_in_cluster]
        std::vector<std::tuple<std::string, double, size_t>> markers;

        for (size_t i = 0; i < cluster.size(); i++) {
            std::vector<double> currentPoint = cluster[i].getFeatures();
            double distance = statistical::geometric_distance(input, currentPoint);
            markers.emplace_back(std::make_tuple(cluster[i].getIdentifier(), distance, i));
        }

        if (k > markers.size()) {
            k = markers.size();
        }

        auto comparator = [](std::tuple<std::string, double, size_t> &a, std::tuple<std::string, double, size_t> &b) {
            return std::get<1>(a) < std::get<1>(b);
        };

        std::sort(markers.begin(), markers.end(), comparator);
        std::unordered_map<std::string, u_int> counts = {};

        for (auto i = 0; i < k; i++) {
            auto label = std::get<0>(markers[i]);

            if (counts.find(label) != counts.end()) {
                counts.at(label) += 1;
                continue;
            }

            counts.insert(std::make_pair(label, 1));
        }

        u_int nearest = 0;
        std::string neighbor;

        for (const auto &i : counts) {
            if (i.second > nearest) {
                nearest = i.second;
                neighbor = i.first;
            }
        }

        return neighbor;
    }

}
