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
        const std::list<std::string> CLA_OPTIONS = {"--features-no", "--path"};
        std::unique_ptr<int> featuresToSelect = std::make_unique<int>(5);
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

        static void printHelp();

        void performSelection(const CSVParser &csvParser);

        static void printInfo(const FLDHeader &fldHeader, const IntVector &selectedFeatures);

    };

}

#endif //STATISTICAL_RUN_HPP
