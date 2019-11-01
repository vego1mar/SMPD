#ifndef CLASSIFIERS_HPP
#define CLASSIFIERS_HPP

#include <string>
#include <vector>

namespace classifiers {
    // This class should be immutable.
    class ClassVector {
    private:
        std::string label;
        std::vector<double> features;

    public:
        ClassVector() = delete;

        ClassVector(std::string label, std::vector<double> features);

        ClassVector(const ClassVector &object) = default;

        ClassVector(ClassVector &&rvalue) = default;

        ClassVector &operator=(const ClassVector &rhs);

        ClassVector &operator=(ClassVector &&rvalue) noexcept;

        ~ClassVector() = default;

        std::string getIdentifier() const;

        std::vector<double> getFeatures() const;

        bool hasSameFeaturesSize(const std::vector<double> &object) const;
    };

    std::string nearest_neighbor(std::vector<double> &input, std::vector<ClassVector> &cluster, u_short k = 1);

}

#endif //CLASSIFIERS_HPP
