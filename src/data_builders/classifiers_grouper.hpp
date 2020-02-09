#ifndef CLASSIFIERS_GROUPER_HPP
#define CLASSIFIERS_GROUPER_HPP

#include "csv_parser.hpp"
#include "../selectors/fisher.hpp"
#include "../classifiers/definitions.hpp"

using data_builders::CSVParser;
using selectors::FLD;


namespace data_builders {

    class ClassifiersGrouper {
    private:
        std::unique_ptr<Matrix> selection;
        std::unique_ptr<Matrix> inputData;
        std::unique_ptr<std::vector<std::size_t>> inputIndices;


    public:
        ClassifiersGrouper() = default;

        ClassifiersGrouper(const ClassifiersGrouper &rhs) = delete;

        ClassifiersGrouper(ClassifiersGrouper &&rvalue) noexcept = delete;

        ClassifiersGrouper &operator=(const ClassifiersGrouper &rhs) = delete;

        ClassifiersGrouper &operator=(ClassifiersGrouper &&rvalue) noexcept = delete;

        virtual ~ClassifiersGrouper() = default;

        void group(const CSVParser &csvParser, const FLD &fld);

        const Matrix &getSelection() const;

        const Matrix &getInput() const;

        const std::vector<std::size_t> &getInputIndices() const;


    private:
        void selectFeaturesData(const CSVParser &csvParser, const FLD &fld);

        void selectInputData(const CSVParser &csvParser, const FLD &fld);

    };

}

#endif //CLASSIFIERS_GROUPER_HPP
