#ifndef ANALYTIC_HPP
#define ANALYTIC_HPP

#include <cmath>
#include "engine.hpp"
#include "../random/normal.hpp"
#include "../processes/blackscholes.hpp"

template <typename D>
class AnalyticalBS: public PricingEngine<D>
{
    private:
        NormalDistribution<D> normal = NormalDistribution<D>(0,1);
        D S;
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

        void setParams(void){
            r = this->process_->rate();
            S = this->process_->initialState.value;
            sigma = this->process_->vol(0);
            K = this->option_->strike();
            T = this->option_->maturity();
            ract = exp(-r*T);
            d1 = (log(S/K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
            d2 = d1  - sigma * sqrt(T);
            nd1 = normal.cdf(d1);
            nd2 = normal.cdf(d2);
            pd1 = normal.pdf(d1);
            pd2 = normal.pdf(d2);
        }

        virtual D _rho() const {
            if (this->option_->type() == OptionType::Call) {
                return K * T * ract * nd2;
            }else {
               return  K * T * ract * (nd2-1);
            }

        }

        virtual D _vega() const {
            return S * pd1 * sqrt(T);
        }

        virtual D _delta() const {
            if (this->option_->type() == OptionType::Call) {
                return nd1;
            }else{
                return nd1 - 1;
            }
        }

        virtual D _gamma() const {
            return  pd1 / (S * sigma * sqrt(T));
        }

        virtual D _theta() const {
            if (this->option_->type() == OptionType::Call) {
                return S * pd1 * sigma / (2 * sqrt(T)) + r * K * ract * nd2;
            }else {
                return S * pd1 * sigma / (2 * sqrt(T)) - r * K * ract * (1-nd2);
            }
        }

        virtual D _vanna() const {
            return this->vega_ * (1 - d1/(sigma * sqrt(T))) / S;

        }

        virtual D _volga() const {
            return  this->vega_ * d1 * d2 / sigma;
        }

        virtual D _premium() {
            if (this->option_->type() == OptionType::Call) {
                return S * nd1 - K * ract * nd2;
            }else{
                return S * (nd1-1) + ract * K * (1-nd2);
            }
        }

    public:
        AnalyticalBS(Option<D>* option, BlackScholesProcess<D>* process)
            : PricingEngine<D>(option, process){
        		setParams();
            };

        virtual~AnalyticalBS() = default;

        virtual void calculate() override {
        	setParams();
        	this->premium_ = _premium();
        	this->delta_ = _delta();
        	this->gamma_ = _gamma();
        	this->vega_ = _vega();
        	this->rho_ = _rho();
        	this->theta_ = _theta();
        	this->vanna_ = _vanna();
        }
};


#endif  // ANALYTIC_HPP
