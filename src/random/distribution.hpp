#ifndef DISTRIBUTION_HPP
#define DISTRIBUTION_HPP


#include <random>
#include <vector>
#include <chrono>
#include <cmath>
#include <cassert>
#include <iostream>

#include "mersenne.hpp"

using namespace std;

template<typename T=double>
class Distribution {
    // We retrieve the type for it to be used elsewhere.
    using RT = T;
private:
    uniform_real_distribution<double> u_ = uniform_real_distribution<double>(0, 1.0);
    std::mt19937_64 gen = MersenneTwister()();

public:
    // Default constructor and destructor
    Distribution() = default;

    virtual~Distribution() = default;

    // Custom constructor
    virtual T pdf(T x) const = 0;

    virtual T cdf(T x) const = 0;

    virtual T operator()() {
        T retval = u_(gen);
        return retval;
    };

    virtual vector<T> &generate(int size) {
        vector<T> *v = new vector<T>(size);
        for (T &c :*v) {
            c = (*this)();
        }
        return *v;
    }
};

#endif  // DISTRIBUTION_HPP
