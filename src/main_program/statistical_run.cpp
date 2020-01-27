#include "statistical_run.hpp"
#include "../matrix/matrix.hpp"
#include "../selectors/fisher.hpp"
#include "../helpers/collections.hpp"
#include "../helpers/combinations.hpp"

using data_builders::Headers;
using matrix::Matrix;
using selectors::FLD;
using helpers::Collections;
using helpers::Combinations;


namespace main_program {

    void StatisticalRun::run() {
        CSVParser parser(vars->MAPLE_OAK_PATH);
        parser.build();

        // TODO: select
        performSelection(parser);

        //TODO: classify

        parser.dispose();
    }

    void StatisticalRun::dispose() {
        if (vars != nullptr) {
            vars.reset();
        }
    }

    void StatisticalRun::performSelection(const CSVParser &csvParser) {
        const Headers &headers = csvParser.getHeaders();
        const auto headersSet = Collections::getDistinct(headers);
        const Matrix &dataset = csvParser.getDataset();
        Combinations combinations(headersSet.size(), 2);
        const std::size_t &COLUMNS = dataset.getColumns();
        FLD fld;

        const int FEATURES_TO_SELECT = 10;

        while (combinations.hasNext()) {
            const auto indices = combinations.getNext();
            const auto &first = indices[0];
            const auto &second = indices[1];
            const auto &firstHeader = *std::next(headersSet.begin(), first);
            const auto &secondHeader = *std::next(headersSet.begin(), second);

//            const std::size_t &ROWS = ;
//            Matrix clusterA(COLUMNS, ROWS);
//            csvParser.getData(firstHeader, clusterA);
            //fld.select(FEATURES_TO_SELECT, );
        }
    }

}
