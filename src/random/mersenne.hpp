#ifndef MERSENNE_HPP
#define MERSENNE_HPP

#include "generator.hpp"
#include <random>

class MersenneTwister final : public Generator<std::mt19937_64> {
public:
    MersenneTwister() = default;

    virtual~MersenneTwister() = default;

    std::mt19937_64 operator()() {
        std::random_device rd;
        auto seed = rd();
        std::mt19937_64 gen(seed);
        return gen;
    }
};

#endif 
