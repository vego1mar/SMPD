#include <set>
#include "combinations.hpp"

namespace helpers {

    Combinations::Combinations(int elements, int distinct)
            : loopLimit(elements), successors({}) {
        determineStartingSuccessors(distinct);
    }

    std::vector<int> Combinations::getNext() {
        // The first combination should be always valid.
        auto previous = getCurrent();
        goNext();

        if (!isValidCombination()) {
            goNext();
        }

        return previous;
    }

    bool Combinations::hasNext() const {
        bool hasNotNext = true;

        for (const auto &distinct : successors) {
            if (distinct == 0) {
                hasNotNext &= true;
                continue;
            }

            hasNotNext &= false;
            break;
        }

        return !hasNotNext;
    }

    void Combinations::determineStartingSuccessors(int distinct) {
        successors.reserve(distinct);

        for (int i = 0; i < distinct; i++) {
            successors.push_back(i);
        }
    }

    bool Combinations::isValidCombination() const {
        std::set<int> current;

        for (const auto &distinct : successors) {
            current.insert(distinct);
        }

        return current.size() == successors.size();
    }

    std::vector<int> Combinations::getCurrent() const {
        auto current = std::vector<int>();

        for (const auto &distinct : successors) {
            current.push_back(distinct);
        }

        return current;
    }

    void Combinations::goNext() {
        std::size_t h = 0;
        bool hasReachedTheEnd = false;

        for (std::size_t i = successors.size() - 1; i >= 0; i--) {
            successors[i]++;

            if (successors[i] == loopLimit) {
                successors[i] = 0;
                hasReachedTheEnd = true;
                h = i;
                continue;
            }

            break;
        }

        if (hasReachedTheEnd) {
            for (std::size_t j = h; j < loopLimit; j++) {
                successors[j] = successors[j - 1] + 1;
            }
        }

        // TODO: validation
        // if (successors[j] == loopLimit) // [024] == [--4]  => untilValid( hasReachedTheEnd + forNext(0) )
        // if successors[0] == loopLimit  =>  STOP (field)
    }

}
