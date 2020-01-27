#ifndef STATISTICAL_RUN_HPP
#define STATISTICAL_RUN_HPP

#include <memory>
#include "../test/constants.hpp"
#include "../data_builders/csv_parser.hpp"

using test::Constants;
using data_builders::CSVParser;


namespace main_program {

    class StatisticalRun {
    private:
        std::unique_ptr<Constants> vars = std::make_unique<Constants>();

    public:
        StatisticalRun() = default;

        StatisticalRun(const StatisticalRun &rhs) = delete;

        StatisticalRun(StatisticalRun &&rvalue) noexcept = delete;

        StatisticalRun &operator=(const StatisticalRun &rhs) = delete;

        StatisticalRun &operator=(StatisticalRun &&rvalue) noexcept = delete;

        virtual ~StatisticalRun() = default;

        void run();

        void dispose();

    private:
        void performSelection(const CSVParser &csvParser);

    };

}

#endif //STATISTICAL_RUN_HPP
