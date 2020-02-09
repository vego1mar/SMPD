#ifndef CLASSIFIERS_GROUPER_HPP
#define CLASSIFIERS_GROUPER_HPP

#include "csv_parser.hpp"
#include "../selectors/fisher.hpp"
#include "../classifiers/definitions.hpp"

using data_builders::CSVParser;
using selectors::FLD;
using classifiers::SuperCluster;


namespace data_builders {

    class ClassifiersGrouper {
    private:
        std::unique_ptr<Matrix> selectionData;
        std::unique_ptr<Headers> selectionLabels;
        std::unique_ptr<SuperCluster> selectionSuperCluster;


    public:
        ClassifiersGrouper() = default;

        ClassifiersGrouper(const ClassifiersGrouper &rhs) = delete;

        ClassifiersGrouper(ClassifiersGrouper &&rvalue) noexcept = delete;

        ClassifiersGrouper &operator=(const ClassifiersGrouper &rhs) = delete;

        ClassifiersGrouper &operator=(ClassifiersGrouper &&rvalue) noexcept = delete;

        virtual ~ClassifiersGrouper() = default;

        void makeSuperCluster(const CSVParser &csvParser, const FLD &fld);

        const SuperCluster &getSuperCluster() const;

        void makeInputCluster(const CSVParser &csvParser, const FLD &fld);

    private:
        void selectFeaturesData(const CSVParser &csvParser, const FLD &fld);

        void selectNonFeaturesData(const CSVParser &csvParser, const FLD &fld);

    };

}

#endif //CLASSIFIERS_GROUPER_HPP
