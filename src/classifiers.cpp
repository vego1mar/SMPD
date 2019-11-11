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

    // Returns the input vector class label (classifier).
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

    bool Cluster::read(std::string &filepath) {
        return io_manager::readFileIntoCluster(filepath, this->vectors);
    }

    const std::vector<ClassVector> &Cluster::getVectors() const {
        return vectors;
    }

    ClassificationResult::ClassificationResult() :
            label({}),
            features({}),
            k{},
            affiliation(0, 0) {
    }

    void ClassificationResult::setLabel(const std::string &resultLabel) {
        this->label = resultLabel;
    }

    void ClassificationResult::setFeatures(const std::vector<double> &resultFeatures) {
        this->features = resultFeatures;
    }

    void ClassificationResult::setK(u_short resultK) {
        this->k = resultK;
    }

    void ClassificationResult::setAffiliation(u_int nominator, u_int denominator) {
        this->affiliation = Affiliation(nominator, denominator);
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
}
