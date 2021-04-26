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


    virtual T drift() const {
        return this->rate_ * this->priceState_.value;
    }

    virtual T diffusion() const {
        return vol() * this->priceState_.value;
    }

    virtual T vol() const {
        return sqrt(volProcess_->priceState().value);
    }

    virtual T rate() const {
        return this->rate_;
    }

    virtual State<T> movePriceEuler(double h, T Z) {
        T ZS = Z * correlation_ + sqrt(1 - correlation_ * correlation_) * normal();
        this->priceState_ = this->nextPriceEuler(h, ZS);
        this->volProcess_->movePriceEuler(h, Z);
        return this->priceState_;
    }

    virtual T eulerPriceDiff(double h) {
        return Process<T>::eulerPriceDiff(h, normal());
    }

    virtual T diffDiffusionVega() const {
        return this->priceState_.value / (2 * vol());
    }

    virtual T diffDriftRho() const {
        return this->priceState_.value;
    }

    virtual T diffDiffusionX() const {
        return vol();
    }

    virtual T diffDriftX() const {
        return this->rate_;
    }

    virtual std::ostream &describe(std::ostream &o) const {
        return Process<T>::describe(o) <<
                                       "Rate: " << this->rate_ << std::endl <<
                                       "Volatility: " << *this->volProcess_ << std::endl <<
                                       "Correlation:" << correlation_ << std::endl;
    }
};


#endif  // HESTON_HPP
