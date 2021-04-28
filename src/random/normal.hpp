#ifndef NORMAL_HPP
#define NORMAL_HPP

#include "distribution.hpp"


template<typename T=double>
class NormalDistribution final : public Distribution<T> {
private:
    T m_;
    T sigma_;
    bool available_ = false;
    T current_;

    T rnorm_box_muller() {
        T u_1 = Distribution<T>::operator()();
        T u_2 = Distribution<T>::operator()();
        T R = sqrt(-2.0 * log(u_1));
        T O = 2.0 * M_PI * u_2;
        current_ = m_ + sigma_ * R * cos(O);
        available_ = true;
        return m_ + sigma_ * R * sin(O);
    }
public:
    NormalDistribution(T m, T sigma) : m_(m), sigma_(sigma) {};

    virtual~NormalDistribution() {};

    T cdf(T x) const override {
        return erfc(-(x - m_) / (sigma_ * sqrt(2))) / 2;
    }

    T pdf(T x) const override {
        return exp(-0.5 * pow(((x - m_) / sigma_), 2)) / sqrt(2 * M_PI);
    }

    virtual T operator()() override {
        if (available_) {
            available_ = false;
            return current_;
        }
        return rnorm_box_muller();
    }



};

#endif  // NORMAL_HPP
