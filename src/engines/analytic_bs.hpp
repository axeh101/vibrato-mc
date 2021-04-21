#ifndef ANALYTIC_HPP
#define ANALYTIC_HPP

#include <cmath>
#include "engine.hpp"
#include "../random/normal.hpp"
#include "../processes/blackscholes.hpp"

template <typename D>
class AnalyticalBS: public PricingEngine<D, BlackScholesProcess<D>>
{
    private:
        NormalDistribution<D> normal = NormalDistribution<D>(0,1);
        D S = this->process_.priceState().value;
        D r;
        D K;
        D sigma;
        double T;
        D ract;
        D d1;
        D d2;
        D nd1;
        D nd2;
        D pd1;
        D pd2;

    public:
        AnalyticalBS(Option<D>& option, BlackScholesProcess<D>& process)
            : PricingEngine<D, BlackScholesProcess<D>>(option, process){
                    r = this->process_.rate();
                    sigma = this->process_.vol(0);
                    K = this->option_.strike();
                    T = this->option_.maturity();
                    ract = exp(-r*T);
                    d1 = (log(S/K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
                    d2 = d1  - sigma * sqrt(T);
                    nd1 = normal.cdf(d1);
                    nd2 = normal.cdf(d2);
                    pd1 = normal.pdf(d1);
                    pd2 = normal.pdf(d2);

            };

        virtual~AnalyticalBS() = default;

        virtual D rho() const {
            if (this->option_.type() == OptionType::Call) {
                return K * T * ract * nd2;
            }else {
               return  K * T * ract * (nd2-1);
            }

        }

        virtual D vega() const {
            return S * pd1 * sqrt(T);
        }

        virtual D delta() const {
            if (this->option_.type() == OptionType::Call) {
                return nd1;
            }else{
                return nd1 - 1;
            }
        }

        virtual D gamma() const {
            return  pd1 / (S * sigma * sqrt(T));
        }

        virtual D theta() const {
            if (this->option_.type() == OptionType::Call) {
                return S * pd1 * sigma / (2 * sqrt(T)) + r * K * ract * nd2;
            }else {
                return S * pd1 * sigma / (2 * sqrt(T)) - r * K * ract * (1-nd2);
            }
        }

        virtual D vanna() const {
            return vega() * (1 - d1/(sigma * sqrt(T))) / S;

        }

        virtual D volga() const {
            return  vega() * d1 * d2 / sigma;
        }

        virtual D premium() const {
            if (this->option_.type() == OptionType::Call) {
                return S * nd1 - K * ract * nd2;
            }else{
                return S * (nd1-1) + ract * K * (1-nd2);
            }
        }
};


#endif  // ANALYTIC_HPP
