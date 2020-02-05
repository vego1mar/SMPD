#include <algorithm>
#include "continuation_sequence.hpp"


namespace helpers {

    ContinuationSequence::ContinuationSequence(const Sequence &fromStart, const Range &inRange) {
        if (!fromStart.hasDistinctValues()) {
            throw std::invalid_argument("!fromStart.hasDistinctValues()");
        }

        if (!inRange.isMonotonic()) {
            throw std::invalid_argument("!inRange.isMonotonic()");
        }

        start = std::make_unique<Sequence>(fromStart);
        range = std::make_unique<Range>(inRange);
        determineSuccessorStart();
        buildNextSequence();
    }

    bool ContinuationSequence::hasNext() const {
        return !(*successor >= range->end || *successor == SIZE_MAX);
    }

    Values &ContinuationSequence::getNext() {
        buildNextSequence();
        *successor = getNextSuccessor(*successor, 1);
        return (*next).values;
    }

    ULong ContinuationSequence::getNextSuccessor(const ULong &current, const ULong &offset) {
        const auto &starts = start->values;

        for (ULong i = current + offset; i < range->end; i++) {
            bool isValueInStart = std::find(starts.begin(), starts.end(), i) != starts.end();

            if (!isValueInStart) {
                return i;
            }
        }

        return SIZE_MAX;
    }

    void ContinuationSequence::determineSuccessorStart() {
        const auto &starts = start->values;
        const auto &startPoint = range->begin;
        bool isStartPointCorrect = !(std::find(starts.begin(), starts.end(), startPoint) != starts.end());

        if (!isStartPointCorrect) {
            const auto nextPoint = getNextSuccessor(startPoint);

            if (nextPoint == SIZE_MAX) {
                throw std::out_of_range("next == SIZE_MAX");
            }

            successor = std::make_unique<ULong>(nextPoint);
            return;
        }

        successor = std::make_unique<ULong>(startPoint);
    }

    void ContinuationSequence::buildNextSequence() {
        next = std::make_unique<Sequence>(*start);
        next->values.push_back(*successor);
    }

}
