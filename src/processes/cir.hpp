#ifndef CIR_HPP
#define CIR_HPP


#include "process.hpp"
#include "../random/normal.hpp"

template<typename T>
class CoxIngersollRossProcess : public Process<T> {
private:

    T eta_;


public:

    // d vt = kappa (eta - vt) dt + vol sqrt(vt) dWt
    CoxIngersollRossProcess(State<T> initialState, T kappa, T eta, T vol) :
            Process<T>(initialState, "Cox-Ingeresoll-Ross"), eta_(eta) {
        assert(2 * kappa * eta > vol * vol);
        this->rate_ = kappa;
        this->vol_ = vol;
    }

    virtual~CoxIngersollRossProcess() = default;

    virtual T drift() const {
        return this->rate_ * (eta_ - this->priceState_.value);
    }

    virtual T diffusion() const {
        return this->vol_ * sqrt(this->priceState_.value);
    }

    virtual T vol() const {
        return this->vol_;
    }

    virtual T rate() const {
        return this->rate_;
    }


    virtual T eulerPriceDiff(double h) {
        return Process<T>::eulerPriceDiff(h, normal());
    }


    virtual std::ostream &describe(std::ostream &o) const {
        return Process<T>::describe(o) <<
                                       "Rate: " << this->rate_ << std::endl <<
                                       "Kappa:" << this->rate_ << std::endl <<
                                       "Eta:" << eta_ << std::endl <<
                                       "Vol Vol:" << this->vol_ << std::endl <<
                                       std::endl;
    }

private:

    NormalDistribution<T> normal = NormalDistribution<T>(0, 1);


};


#endif // CIR_HPP
