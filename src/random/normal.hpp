#ifndef NORMAL_HPP
#define NORMAL_HPP

#include "mersenne.hpp"


class Normal{
    private:
        std::mt19937_64 gen_;
        std::normal_distribution<> N_;

    public:
        Normal():gen_(MersenneTwister()()), N_(0,1){}
        virtual~Normal()=default;

    double operator()()
    {
        return  N_(gen_);
    }
};

#endif  // NORMAL_HPP
