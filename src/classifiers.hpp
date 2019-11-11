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

        // Large label sizes are discouraged.
        ClassVector(std::string label, std::vector<double> const &features);

        ClassVector(const ClassVector &object) = default;

        ClassVector(ClassVector &&rvalue) = default;

        ClassVector &operator=(const ClassVector &rhs) = delete;

        ClassVector &operator=(ClassVector &&rvalue) noexcept = delete;

        bool operator==(const ClassVector &object) const;

        bool operator!=(const ClassVector &object) const;

        virtual ~ClassVector() = default;

        std::string getIdentifier() const;

        std::vector<double> getFeatures() const;

        bool hasSameFeaturesSize(const std::vector<double> &object) const;
    };

    std::string nearest_neighbor(std::vector<double> &input, std::vector<ClassVector> &cluster, u_short k = 1);

    class Affiliation {
    private:
        std::string fraction;
        double percent{};

    public:
        Affiliation() = delete;

        Affiliation(u_int nominator, u_int denominator);

        Affiliation(const Affiliation &object) = delete;

        Affiliation(Affiliation &&rvalue) = delete;

        Affiliation &operator=(const Affiliation &rhs) = delete;

        Affiliation &operator=(Affiliation &&rvalue) noexcept = default;

        virtual ~Affiliation() = default;

        const std::string &getFraction() const;

        double getPercent() const;

        std::string toString() const;
    };

    class ClassificationResult {
    private:
        std::string label;
        std::vector<double> features;
        u_short k{};
        Affiliation affiliation;

    public:
        ClassificationResult();

        ClassificationResult(const ClassificationResult &object) = delete;

        ClassificationResult(ClassificationResult &&rvalue) = delete;

        ClassificationResult &operator=(const ClassificationResult &rhs) = delete;

        ClassificationResult &operator=(ClassificationResult &&rvalue) noexcept = delete;

        virtual ~ClassificationResult() = default;

        void setLabel(const std::string &resultLabel);

        void setFeatures(const std::vector<double> &resultFeatures);

        void setK(u_short resultK);

        void setAffiliation(u_int nominator, u_int denominator);

        std::string toString() const;

    private:
        std::string featuresToString() const;
    };

    class Cluster {
    private:
        std::vector<ClassVector> vectors;

    public:
        Cluster() = default;

        Cluster(const Cluster &object) = delete;

        Cluster(Cluster &&rvalue) = delete;

        Cluster &operator=(const Cluster &rhs) = delete;

        Cluster &operator=(Cluster &&rvalue) noexcept = delete;

        virtual ~Cluster() = default;

        const std::vector<ClassVector> &getVectors() const;

        bool read(std::string &filepath);
    };
}

#endif //CLASSIFIERS_HPP
