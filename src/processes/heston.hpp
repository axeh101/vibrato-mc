#ifndef HESTON_HPP
#define HESTON_HPP

#include "process.hpp"
#include "../random/normal.hpp"

template<typename T>
class HestonProcess : public Process<T> {

private:

    NormalDistribution<T> normal = NormalDistribution<T>(0, 1);
    T correlation_;
    Process<T> *volProcess_;


public:

    // dSt = r St dt + sqrt(Vt) St dWt
    // dVt = kappa (eta - Vt) dt + vol sqrt(Vt) dBt
    // <dWt, Dbt> = correlation dt
    HestonProcess(State<T> initialState, Process<T> *volProcess, T rate, T correlation) :
            Process<T>(initialState, "Heston"), correlation_(correlation), volProcess_(volProcess) {
        this->vol_ = volProcess->initialState.value;
        this->rate_ = rate;
    }

    virtual ~HestonProcess() {
        volProcess_ = nullptr;
        delete volProcess_;
    }


    virtual T drift() const override {
        return this->rate_ * this->priceState_.value;
    }

    virtual T diffusion() const override {
        return vol() * this->priceState_.value;
    }

    virtual T vol() const override {
        return sqrt(volProcess_->priceState().value);
    }

    virtual T rate() const override {
        return this->rate_;
    }

    virtual State<T> movePriceEuler(T h, T Z) override {
        T ZS = Z * correlation_ + sqrt(1 - correlation_ * correlation_) * normal();
        this->priceState_ = this->nextPriceEuler(h, ZS);
        this->volProcess_->movePriceEuler(h, Z);
        return this->priceState_;
    }

    virtual T eulerPriceDiff(T h) {
        return Process<T>::eulerPriceDiff(h, normal());
    }

    virtual T diffDiffusionVega() const override {
        return this->priceState_.value / (2 * vol());
    }

    virtual T diffDriftRho() const override {
        return this->priceState_.value;
    }

    virtual T diffDiffusionX() const override {
        return vol();
    }

    virtual T diffDriftX() const override {
        return this->rate_;
    }

    virtual T diffDiffusionSigmaX() const override {
        return 1 / (2 * this->vol());
    }

    virtual T diffDiffusionSigma2() const override {
        T v = this->vol();
        return -this->priceState_.value / (4 * v * v * v);
    }

    virtual void resetState() {
        volProcess_->resetState();
        Process<T>::resetState();
    }

    virtual std::ostream &describe(std::ostream &o) const {
        return Process<T>::describe(o) <<
                                       "Rate: " << this->rate_ << std::endl <<
                                       "Volatility: " << *this->volProcess_ << std::endl <<
                                       "Correlation:" << correlation_ << std::endl;
    }
};


#endif  // HESTON_HPP
