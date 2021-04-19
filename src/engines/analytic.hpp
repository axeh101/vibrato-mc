#ifndef ANALYTIC_HPP
#define ANALYTIC_HPP

#include <cmath>
#include "engine.hpp"
#include "../random/normal.hpp"

template <typename T>
class AnalyticalBS: public PricingEngine<T>
{
    private:

    public:
        AnalyticalBS(Option<T>& option, Process<T>& process): PricingEngine<T>(option, process){};
        virtual~AnalyticalBS() = default;

        virtual void calculate()  override {
            NormalDistribution<T> normal = NormalDistribution<T>(0,1);
            double t = this->option_.maturity();
            double sqt = sqrt(t);
            T r = this->process_.rate();
            T S = this->process_.currentState().value;
            T K = this->option_.strike();
            T sigma = this->process_.vol();

            T ract = exp(-r*t);
            T ss = sigma * sigma;
            T d1 = (log(S/K) + (r + 0.5 * ss) * t) / (sigma * sqt);
            T d2 = d1  - sigma * sqt;

            this->vega_ = S * normal.pdf(d1) * sqt;
            this->gamma_ = normal.pdf(d1) / (S * sigma * sqt);
            this->vanna_ = this->vega_ * (1 - d1/(sigma * sqt)) / S;
            this->volga_ = this->vega_ * d1 * d2 / sigma;
            if (this->option_.type() == OptionType::Call) {
                this->delta_ = normal.cdf(d1);
                this->premium_ = S * this->delta_ - K * ract * normal.cdf(d2);
                this->theta_= S * normal.pdf(d1) * sigma / (2 * sqt) + r * K * ract * normal.pdf(d2);
                this->rho_ = K * t * ract * normal.cdf(d2);
            }else {
                this->delta_ = -normal.cdf(-d1);
                this->premium_ = S * this->delta_+ ract * K * normal.cdf(-d2);
                this->theta_ = S * normal.pdf(-d1) * sigma / (2 * sqt) - r * K * ract * normal.cdf(-d2);
                this->rho_= -K * t * ract * normal.cdf(-d2);
            }
        }

    };


#endif  // ANALYTIC_HPP
