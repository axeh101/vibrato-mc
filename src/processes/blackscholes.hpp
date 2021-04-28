#ifndef BLACKSCHOLES_HPP
#define BLACKSCHOLES_HPP

#include <cmath>
#include <iostream>

#include "../random/normal.hpp"
#include "helper.hpp"
#include "process.hpp"

template<typename T>
class BlackScholesProcess : public Process<T> {

public:
    BlackScholesProcess(State<T> initialState, T rate, T vol) :
            Process<T>(initialState, "Black-Scholes") {
        this->vol_ = vol;
        this->rate_ = rate;
    }

    virtual~BlackScholesProcess() = default;

    virtual T drift() const override {
        return this->rate_ * this->priceState_.value;
    }

    virtual T diffusion() const override {
        return vol() * this->priceState_.value;
    }

    virtual State<T> nextIto(double h) {
        return {
                this->priceState_.time + h,
                ito(this->priceState_.value, h)
        };
    }

    virtual T vol() const override {
        return this->vol_;
    }

    virtual T rate() const override {
        return this->rate_;
    }

    virtual State<T> moveIto(double t) {
        this->priceState_ = nextIto(t);
        return this->priceState_;
    }

    virtual Path<T> generatePath(int nsamples, double horizon) {
        this->resetState();
        Path<T> path = Path<T>(nsamples + 1);
        path[0] = this->initialState;
        double h = horizon / nsamples;
        for (int i = 1; i <= nsamples; i++) {
            path[i] = this->moveIto(h);
        }
        return path;
    }

    virtual T eulerPriceDiff(double h) {
        return Process<T>::eulerPriceDiff(h, normal());
    }

    virtual T diffDiffusionVega() const override {
        return this->priceState_.value;
    }

    virtual T diffDriftRho() const override {
        return this->priceState_.value;
    }

    virtual T diffDiffusionX() const override {
        return this->vol_;
    }

    virtual T diffDriftX() const override {
        return this->rate_;
    }

    virtual T diffDiffusionSigmaX() const override {
        return 1;
    }

    virtual std::ostream &describe(std::ostream &o) const override {
        return Process<T>::describe(o) << "Rate: " << this->rate_ << std::endl
                                       << "Volatility: " << this->vol_ << std::endl;
    }

private:

    NormalDistribution<T> normal = NormalDistribution<T>(0, 1);

    T ito(T value, double h) {
        T vol2 = this->vol_ * this->vol_;
        T dWt = sqrt(h) * normal();
        T newValue = value * exp((this->rate_ - 0.5 * vol2) * h + this->vol_ * dWt);
        return newValue;
    }

};

#endif  // BLACKSCHOLES_HPP
