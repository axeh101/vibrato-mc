#ifndef ANALYTIC_BS_HPP
#define ANALYTIC_BS_HPP

#include <cmath>
#include "engine.hpp"
#include "../random/normal.hpp"
#include "../processes/blackscholes.hpp"

template<typename D>
class AnalyticalVanillaBS : public PricingEngine<D> {

public:
    AnalyticalVanillaBS(Option<D> *option, BlackScholesProcess<D> *process) : PricingEngine<D>(option, process) {};

    virtual~AnalyticalVanillaBS() = default;

    virtual D premium() override {
        setParams();
        if (this->option_->type() == OptionType::Call) {
            return S * nd1 - K * ract * nd2;
        } else {
            return S * (nd1 - 1) + ract * K * (1 - nd2);
        }
    }

    virtual D delta() override {
        setParams();
        if (this->option_->type() == OptionType::Call) {
            return nd1;
        } else {
            return nd1 - 1;
        }
    }

    virtual D gamma() override {
        setParams();
        return pd1 / (S * sigma * sqrt(T));
    }

    virtual D vega() override {
        setParams();
        return S * pd1 * sqrt(T);
    }

    virtual D rho() override {
        setParams();
        if (this->option_->type() == OptionType::Call) {
            return K * T * ract * nd2;
        } else {
            return K * T * ract * (nd2 - 1);
        }

    }

    virtual D theta() override {
        setParams();
        if (this->option_->type() == OptionType::Call) {
            return -S * pd1 * sigma / (2 * sqrt(T)) - r * K * ract * nd2;
        } else {
            return -S * pd1 * sigma / (2 * sqrt(T)) + r * K * ract * (1 - nd2);
        }
    }

    virtual D vanna() override {
        setParams();
        return -pd1 * d2 / sigma;

    }

    virtual D volga() override {
        setParams();
        return S * sqrt(T) * pd1 * d1 * d2 / (sigma);
    }

private:
    NormalDistribution<D> normal = NormalDistribution<D>(0, 1);
    D S, K, r, T, sigma;
    D ract, d1, d2, nd1, nd2, pd1;


    void setParams(void) {
        r = this->process_->rate();
        S = this->process_->initialState.value;
        sigma = this->process_->vol();
        K = this->option_->strike();
        T = this->option_->maturity();
        ract = exp(-r * T);
        d1 = (log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
        d2 = d1 - sigma * sqrt(T);
        nd1 = normal.cdf(d1);
        nd2 = normal.cdf(d2);
        pd1 = normal.pdf(d1);
    }


};


#endif  // ANALYTIC_BS_HPP
