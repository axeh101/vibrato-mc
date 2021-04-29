#ifndef ANALYTIC_iDIGITAL_BS_HPP
#define ANALYTIC_DIGITAL_BS_HPP

#include <cmath>
#include "engine.hpp"
#include "../random/normal.hpp"
#include "../processes/blackscholes.hpp"

template<typename D>
class AnalyticalDigitalBS : public PricingEngine<D> {

public:
    AnalyticalDigitalBS(Option<D> *option, BlackScholesProcess<D> *process)
            : PricingEngine<D>(option, process) {
        setParams();
    };

    virtual~AnalyticalDigitalBS() = default;

    virtual D premium() override {
        setParams();
        if (this->option_->type() == OptionType::Call) {
            return ract * nd2;
        } else {
            return ract * (1 - nd2);
        }
    }

    virtual D delta() override {
        setParams();
        return this->option_->type() * ract * pd2 / (sigma * sqrt(T) * S);
    }

    virtual D rho() override {
        setParams();
        D crho = ract * pd2 * sqrt(T) / sigma - T * ract * nd2;
        if (this->option_->type() == OptionType::Call) {
            return crho;
        } else {
            return -T * ract - crho;
        }

    }

    virtual D vega() override {
        setParams();
        return -this->option_->type() * d1 * pd2 * ract / sigma;
    }


    virtual D gamma() override {
        setParams();
        return -this->option_->type() * ract * pd2 * d1 / (sigma * sigma * S * S * T);
    }

    virtual D theta() override {
        setParams();
        D ctheta = ract * nd2 * r;
        ctheta += ract * pd2 * (d1 / (2 * T) - r / (sigma * sqrt(T)));
        if (this->option_->type() == OptionType::Call) {
            return -ctheta;
        } else {
            return ctheta - r * ract;
        }
    }

    virtual D vanna() override {
        setParams();
        return this->option_->type() * ract * pd2 * (d1 * d2 - 1) / (S * sigma * sigma * sqrt(T));
    }

    virtual D volga() override {
        setParams();
        return -this->option_->type() * ract * pd2 * (d1 * d1 * d2 - d1 - d2) / (sigma * sigma);
    }

private:
    NormalDistribution<D> normal = NormalDistribution<D>(0, 1);
    D S, K, r, T, sigma;
    D ract, d1, d2, nd2, pd2;

    void setParams(void) {
        r = this->process_->rate();
        S = this->process_->initialState.value;
        sigma = this->process_->vol();
        K = this->option_->strike();
        T = this->option_->maturity();
        ract = exp(-r * T);
        d1 = (log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
        d2 = d1 - sigma * sqrt(T);
        nd2 = normal.cdf(d2);
        pd2 = normal.pdf(d2);
    }
};


#endif  // ANALYTIC_iDIGITAL_BS_HPP
