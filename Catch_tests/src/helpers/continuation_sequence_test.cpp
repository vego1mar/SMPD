#include "../../catch.hpp"
#include "../../../src/helpers/continuation_sequence.hpp"

using helpers::ContinuationSequence;
using helpers::Sequence;
using helpers::Range;
using helpers::Values;
using helpers::ULong;


TEST_CASE("continuation_sequence_test", "[continuation_sequence]") {

    SECTION("getNext, hasNext -> OK") {
        auto start = Sequence();
        auto range = Range(0, 5);
        start.values = {0, 3};
        auto expectedResults = std::vector<Values>({
                                                           {0, 3, 1},
                                                           {0, 3, 2},
                                                           {0, 3, 4}
                                                   });

        ContinuationSequence continuation(start, range);
        auto results = std::make_unique<std::vector<Values>>();

        while (continuation.hasNext()) {
            results->push_back(continuation.getNext());
        }

        REQUIRE_THAT(*results, Catch::UnorderedEquals(expectedResults));
    }

    SECTION("no continuation -> exception") {
        ULong iterations = 0;

        const auto continuationFunction = [&iterations]() {
            auto range = Range(0, 3);
            auto start = Sequence();
            start.values = {0, 1, 2};
            ContinuationSequence continuation(start, range);

            while (continuation.hasNext()) {
                iterations++;
            }
        };

        CHECK(iterations == 0);
        REQUIRE_THROWS_AS(continuationFunction(), std::out_of_range);
    }

    SECTION("getNext[1] -> OK") {
        auto range = Range(0, 4);
        auto start = Sequence();
        start.values = {};
        auto expectedResults = std::vector<Values>({
                                                           {0},
                                                           {1},
                                                           {2},
                                                           {3}
                                                   });

        ContinuationSequence continuation(start, range);
        auto results = std::make_unique<std::vector<Values>>();

        while (continuation.hasNext()) {
            results->push_back(continuation.getNext());
        }

        REQUIRE_THAT(*results, Catch::UnorderedEquals(expectedResults));
    }

    SECTION("getNext[2] -> OK") {
        auto range = Range(0, 4);
        auto start = Sequence();
        start.values = {1};
        auto expectedResults = std::vector<Values>({
                                                           {1, 0},
                                                           {1, 2},
                                                           {1, 3}
                                                   });

        ContinuationSequence continuation(start, range);
        auto results = std::make_unique<std::vector<Values>>();

        while (continuation.hasNext()) {
            results->push_back(continuation.getNext());
        }

        REQUIRE_THAT(*results, Catch::UnorderedEquals(expectedResults));
    }

    SECTION("getNext[3] -> OK") {
        auto range = Range(0, 4);
        auto start = Sequence();
        start.values = {1, 3};
        auto expectedResults = std::vector<Values>({
                                                           {1, 3, 0},
                                                           {1, 3, 2},
                                                   });

        ContinuationSequence continuation(start, range);
        auto results = std::make_unique<std::vector<Values>>();

        while (continuation.hasNext()) {
            results->push_back(continuation.getNext());
        }

        REQUIRE_THAT(*results, Catch::UnorderedEquals(expectedResults));
    }

}
