#ifndef STATISTICAL_RUN_HPP
#define STATISTICAL_RUN_HPP

#include <memory>
#include <list>
#include "../data_builders/csv_parser.hpp"
#include "../command_line/cla_parser.hpp"
#include "../selectors/fisher.hpp"

using data_builders::CSVParser;
using command_line::CLAParser;
using selectors::FLD;
using selectors::IntVector;


namespace main_program {

    typedef std::pair<std::string, std::string> FLDHeader;


    class StatisticalRun {
    private:
        const std::list<std::string> CLA_OPTIONS = {"--features-no", "--path", "--sfs"};
        std::unique_ptr<int> featuresToSelect = std::make_unique<int>(3);
        std::unique_ptr<CLAParser> claParser;


    public:
        StatisticalRun() = default;

        StatisticalRun(const StatisticalRun &rhs) = delete;

        StatisticalRun(StatisticalRun &&rvalue) noexcept = delete;

        StatisticalRun &operator=(const StatisticalRun &rhs) = delete;

        StatisticalRun &operator=(StatisticalRun &&rvalue) noexcept = delete;

        virtual ~StatisticalRun() = default;

        void run(int argc, char **argv);

        void dispose();

    private:
        void performRun();

        bool areCLIArgumentsOK();

        void printHelp() const;

        void performStatisticalRun(const CSVParser &csvParser);

        static void printInfo(const FLDHeader &fldHeader, const IntVector &selectedFeatures);

        void performSelection(const Matrix &clusterA, const Matrix &clusterB, FLD &fld);

        void performClassification(const CSVParser &csvParser, const FLD &fld);

    };

}

#endif //STATISTICAL_RUN_HPP
