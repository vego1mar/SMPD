#include <stdexcept>
#include <algorithm>
#include <unordered_map>
#include "classifiers.hpp"
#include "statistical.hpp"
#include "io_manager.hpp"

namespace classifiers {
    ClassVector::ClassVector(std::string label, std::vector<double> const &features)
            : label(std::move(label)), features(features) {
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

    bool ClassVector::operator==(const ClassVector &object) const {
        bool isLabelEqual = (this->label == object.getIdentifier());
        bool isFeaturesEqual = std::equal(this->features.begin(), this->features.end(),
                                          object.getFeatures().begin());
        return isLabelEqual && isFeaturesEqual;
    }

    bool ClassVector::operator!=(const classifiers::ClassVector &object) const {
        return !operator==(object);
    }

    Affiliation::Affiliation(u_int nominator, u_int denominator) {
        fraction = std::to_string(nominator) + '/' + std::to_string(denominator);
        percent = (static_cast<double>(nominator) / static_cast<double>(denominator)) * 100.0;
    }

    const std::string &Affiliation::getFraction() const {
        return fraction;
    }

    double Affiliation::getPercent() const {
        return percent;
    }

    std::string Affiliation::toString() const {
        return '{' + fraction + ',' + std::to_string(percent) + '}';
    }

    bool Affiliation::operator==(const Affiliation &object) const {
        return this->fraction == object.getFraction();
    }

    ClassificationResult::ClassificationResult(std::string &label, std::vector<double> &features, u_short k,
                                               Affiliation &affiliation)
            : label(label), features(features), k(k), affiliation(affiliation) {
    }

    std::string ClassificationResult::toString() const {
        return '{' + label + ',' + featuresToString() + ',' + std::to_string(k) + ',' +
               affiliation.toString() + '}';
    }

    std::string ClassificationResult::featuresToString() const {
        std::string result = "[";

        for (auto feature : features) {
            result += std::to_string(feature) + ',';
        }

        return result.substr(0, result.size() - 2) + ']';
    }

    const std::string &ClassificationResult::getLabel() const {
        return label;
    }

    bool Cluster::read(std::string &filepath) {
        return io_manager::readFileIntoCluster(filepath, this->vectors);
    }

    const std::vector<ClassVector> &Cluster::getVectors() const {
        return vectors;
    }

    void Cluster::classify(std::vector<double> &input, u_short k) {
        classified.emplace_back(nearest_neighbor_2(input, vectors, k));
    }

    const std::vector<ClassificationResult> &Cluster::getClassified() const {
        return classified;
    }

    std::string nearest_neighbor(std::vector<double> &input, std::vector<ClassVector> &cluster, u_short k) {
        return nearest_neighbor_2(input, cluster, k).getLabel();
    }

    ClassificationResult nearest_neighbor_2(std::vector<double> &input, std::vector<ClassVector> &cluster, u_short k) {
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
        u_int total = 0;
        std::string neighbor;

        for (const auto &i : counts) {
            if (i.second > nearest) {
                nearest = i.second;
                neighbor = i.first;
            }

            total += i.second;
        }

        Affiliation affiliation(nearest, total);
        return ClassificationResult(neighbor, input, k, affiliation);
    }

    // TODO: build nearest_neighbor for multiple inputs as (param := std::vector<ClassVector> &inputCluster)
    // TODO: complete Cluster::classify for single and multiple inputs
    // TODO: provide nearest_mean(input, cluster) && nearest_mean(cluster, cluster)
}
