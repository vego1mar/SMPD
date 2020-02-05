#include <stdexcept>
#include "combinations.hpp"


namespace helpers {

    Combinations::Combinations(int elements, int distinct)
            : loopLimit(elements), combinationLimit(distinct) {
        if (distinct <= 1 || elements <= 1) {
            throw std::invalid_argument("distinct <= 1 || elements <= 1");
        }

        determineStartingSuccessors(distinct);
    }

    std::vector<int> Combinations::getNext() {
        // The first combination should be always valid.
        auto previous = getCurrent();
        auto startingPlace = successors.size() - 1;
        goNext(startingPlace);
        return previous;
    }

    bool Combinations::hasNext() const {
        return getSuccessorFirstInvalidIndex() == -1;
    }

    void Combinations::determineStartingSuccessors(int distinct) {
        successors.reserve(distinct);

        for (int i = 0; i < distinct; i++) {
            successors.push_back(i);
        }
    }

    std::vector<int> Combinations::getCurrent() const {
        auto current = std::vector<int>();

        for (const auto &distinct : successors) {
            current.push_back(distinct);
        }

        return current;
    }

    void Combinations::goNext(int i) {
        if (i == 0 && successors[i] == loopLimit) {
            return;
        }

        successors[i]++;

        if (successors[i] >= loopLimit) {
            determineNextSuccessors(i - 1);
        }

        int invalidIndex = getSuccessorFirstInvalidIndex();
        bool areSuccessorsValid = invalidIndex == -1;

        if (!areSuccessorsValid) {
            goNext(invalidIndex);
        }
    }

    void Combinations::determineNextSuccessors(int j) {
        successors[j]++;

        for (int i = j + 1; i < combinationLimit; i++) {
            successors[i] = successors[i - 1] + 1;
        }
    }

    int Combinations::getSuccessorFirstInvalidIndex() const {
        for (int i = 0; i < combinationLimit; i++) {
            if (successors[i] >= loopLimit) {
                return i;
            }
        }

        return -1;
    }

}
