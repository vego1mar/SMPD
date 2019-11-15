#ifndef CASING_HPP
#define CASING_HPP

#include <string>
#include <vector>

namespace casing {
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

        ClassVector &operator=(const ClassVector &rhs) = default;

        ClassVector &operator=(ClassVector &&rvalue) noexcept = delete;

        bool operator==(const ClassVector &object) const;

        bool operator!=(const ClassVector &object) const;

        virtual ~ClassVector() = default;

        std::string getLabel() const;

        std::vector<double> getFeatures() const;

        bool hasSameFeaturesSize(const std::vector<double> &object) const;
    };

    class Affiliation {
    private:
        std::string fraction;
        double percent{};

    public:
        Affiliation() = delete;

        Affiliation(u_int nominator, u_int denominator);

        Affiliation(const Affiliation &object) = default;

        Affiliation(Affiliation &&rvalue) = default;

        Affiliation &operator=(const Affiliation &rhs) = delete;

        Affiliation &operator=(Affiliation &&rvalue) noexcept = default;

        bool operator==(const Affiliation &object) const;

        virtual ~Affiliation() = default;

        const std::string &getFraction() const;

        double getPercent() const;

        std::string toString() const;
    };

    class NNResultSet {
    private:
        std::string label;
        std::vector<double> features;
        u_short k{};
        Affiliation affiliation;

    public:
        NNResultSet(std::string &label, std::vector<double> &features, u_short k, Affiliation &affiliation);

        NNResultSet(const NNResultSet &object) = default;

        NNResultSet(NNResultSet &&rvalue) = default;

        NNResultSet &operator=(const NNResultSet &rhs) = delete;

        NNResultSet &operator=(NNResultSet &&rvalue) noexcept = delete;

        virtual ~NNResultSet() = default;

        std::string toString() const;

        const std::string &getLabel() const;

        ClassVector getClassVector() const;

    private:
        std::string featuresToString() const;
    };

    class SuperCluster {
    private:
        std::vector<ClassVector> vectors;
        std::vector<NNResultSet> classifiedByNN;

    public:
        SuperCluster() = default;

        SuperCluster(const SuperCluster &object) = delete;

        SuperCluster(SuperCluster &&rvalue) = delete;

        SuperCluster &operator=(const SuperCluster &rhs) = delete;

        SuperCluster &operator=(SuperCluster &&rvalue) noexcept = delete;

        virtual ~SuperCluster() = default;

        const std::vector<ClassVector> &getVectors() const;

        bool read(std::string &filepath);

        void classify(std::vector<double> &input, u_short k);

        void classify(std::vector<ClassVector> &inputGroup, u_short k);

        const std::vector<NNResultSet> &getClassified() const;

        std::vector<ClassVector> getSubCluster(const std::string &label) const;

        std::vector<std::vector<ClassVector>> getSubClusters();
    };

}

#endif //CASING_HPP
