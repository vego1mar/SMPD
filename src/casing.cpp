#include <stdexcept>
#include <algorithm>
#include <set>
#include <sstream>
#include <iterator>
#include "casing.hpp"
#include "statistical.hpp"
#include "io_manager.hpp"
#include "classifiers.hpp"
#include "helpers.hpp"

namespace casing {
    ClassVector::ClassVector(std::string label, std::vector<double> const &features)
            : label(std::move(label)), features(features) {
    }

    std::string ClassVector::getLabel() const {
        return label;
    }

    std::vector<double> ClassVector::getFeatures() const {
        return std::vector<double>(features);
    }

    bool ClassVector::operator==(const ClassVector &object) const {
        bool isLabelEqual = (this->label == object.getLabel());
        bool areFeaturesEqual = std::equal(this->features.begin(), this->features.end(),
                                           object.getFeatures().begin());
        return isLabelEqual && areFeaturesEqual;
    }

    bool ClassVector::operator!=(const ClassVector &object) const {
        return !operator==(object);
    }

    void ClassVector::setLabel(const std::string &newLabel) {
        label = newLabel;
    }

    std::string ClassVector::toString() const {
        std::string toString = '{' + label + ", [";
        std::ostringstream stringify;
        auto delimiter = ";";

        if (!features.empty()) {
            auto iterator = std::ostream_iterator<double>(stringify, delimiter);
            std::copy(features.begin(), features.end() - 1, iterator);
            stringify << features.back();
        }

        return toString + stringify.str() + "]}";
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

    NNResultSet::NNResultSet(std::string &label, std::vector<double> &features, u_short k, Affiliation &affiliation)
            : label(label), features(features), k(k), affiliation(affiliation) {
    }

    std::string NNResultSet::toString() const {
        return '{' + label + ',' + featuresToString() + ',' + std::to_string(k) + ',' +
               affiliation.toString() + '}';
    }

    std::string NNResultSet::featuresToString() const {
        std::string result = "[";

        for (auto feature : features) {
            result += std::to_string(feature) + ',';
        }

        return result.substr(0, result.size() - 2) + ']';
    }

    const std::string &NNResultSet::getLabel() const {
        return label;
    }

    ClassVector NNResultSet::getClassVector() const {
        return ClassVector(label, features);
    }

    bool NNCluster::read(std::string &filepath) {
        return io_manager::readFileIntoCluster(filepath, this->vectors);
    }

    const Cluster &NNCluster::getVectors() const {
        return vectors;
    }

    void NNCluster::classify(std::vector<double> &input, u_short k) {
        classifiedByNN.emplace_back(classifiers::nearest_neighbor_2(input, vectors, k));
    }

    void NNCluster::classify(Cluster &inputGroup, u_short k) {
        classifiedByNN = classifiers::nearest_neighbor(inputGroup, vectors, k);
    }

    const std::vector<NNResultSet> &NNCluster::getClassified() const {
        return classifiedByNN;
    }

    Cluster NNCluster::getSubCluster(const std::string &label) const {
        Cluster outCluster{};

        for (const auto &classVector : vectors) {
            if (classVector.getLabel() == label) {
                outCluster.push_back(classVector);
            }
        }

        for (const auto &classificationResult : classifiedByNN) {
            if (classificationResult.getLabel() == label) {
                outCluster.push_back(classificationResult.getClassVector());
            }
        }

        return outCluster;
    }

    std::vector<Cluster> NNCluster::getSubClusters() {
        std::vector<Cluster> outClusters;
        auto labelsSet = helpers::getClustersLabels(vectors);
        outClusters.reserve(labelsSet.size());

        for (const auto &label : labelsSet) {
            outClusters.push_back(getSubCluster(label));
        }

        return outClusters;
    }

}
