#ifndef COMBINATIONS_HPP
#define COMBINATIONS_HPP

#include <vector>

namespace helpers {

    class Combinations {
    private:
        int loopLimit;
        int combinationLimit;
        std::vector<int> successors;

    public:
        Combinations() = delete;

        Combinations(int elements, int distinct);

        Combinations(const Combinations &rhs) = delete;

        Combinations(Combinations &rvalue) noexcept = delete;

        Combinations &operator=(const Combinations &rhs) = delete;

        Combinations &operator=(Combinations &rvalue) noexcept = delete;

        virtual ~Combinations() = default;

    public:
        std::vector<int> getNext();

        bool hasNext() const;

    private:
        void determineStartingSuccessors(int distinct);

        std::vector<int> getCurrent() const;

        void goNext(int i);

        void determineNextSuccessors(int j);

        int getSuccessorFirstInvalidIndex() const;

    };

}

#endif //COMBINATIONS_HPP
