#ifndef RANDOMS_HPP
#define RANDOMS_HPP

#include <random>


namespace statistics {

    template<typename T>
    struct RandomInts {
        std::mt19937 generator;
        std::uniform_int_distribution<T> distribution;

        RandomInts(T min, T max, unsigned int seed = std::random_device{}()) {
            generator = std::mt19937(seed);
            distribution = std::uniform_int_distribution<T>(min, max);
        }

        T operator()() {
            return distribution(generator);
        }
    };

}

#endif //RANDOMS_HPP
