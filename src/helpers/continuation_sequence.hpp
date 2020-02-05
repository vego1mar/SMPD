#ifndef CONTINUATION_SEQUENCE_HPP
#define CONTINUATION_SEQUENCE_HPP

#include <memory>
#include "definitions.hpp"


namespace helpers {

    class ContinuationSequence {
    private:
        std::unique_ptr<Sequence> start;
        std::unique_ptr<Range> range;
        std::unique_ptr<ULong> successor;
        std::unique_ptr<Sequence> next;


    public:
        ContinuationSequence() = delete;

        ContinuationSequence(const Sequence &fromStart, const Range &inRange);

        ContinuationSequence(const ContinuationSequence &rhs) = delete;

        ContinuationSequence(ContinuationSequence &&rvalue) noexcept = delete;

        ContinuationSequence &operator=(const ContinuationSequence &rhs) = delete;

        ContinuationSequence &operator=(ContinuationSequence &&rvalue) noexcept = delete;

        virtual ~ContinuationSequence() = default;

        bool hasNext() const;

        Values &getNext();

    private:
        ULong getNextSuccessor(const ULong &current, const ULong &offset = 0);

        void determineSuccessorStart();

        void buildNextSequence();

    };

}

#endif //CONTINUATION_SEQUENCE_HPP
