#ifndef ANALYTIC_iDIGITAL_BS_HPP
#define ANALYTIC_DIGITAL_BS_HPP

#include <cmath>
#include "engine.hpp"
#include "../random/normal.hpp"
#include "../processes/blackscholes.hpp"

template <typename D>
class AnalyticalDigitalBS: public PricingEngine<D>
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
            sigma = this->process_->vol();
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
            D crho = ract * pd2 * sqrt(T)  / sigma - T * ract * nd2;
            if (this->option_->type() == OptionType::Call) {
                return crho;
            }else {
               return  - T * ract - crho;
            }

        }

        virtual D _vega() const {
            return - d1 * pd2 * ract / sigma;
        }

        virtual D _delta() const {
                return this->option_->type() * ract * pd2 / (sigma * sqrt(T) * S);
        }

        virtual D _gamma() const {
            return  _vega() / (sigma * S * S *T);
        }

        virtual D _theta() const {
            D ctheta = ract * nd2 * r;
            ctheta+= ract * pd2  * (d1 / (2 * T) - r / (sigma* sqrt(T)));
            if (this->option_->type() == OptionType::Call) {
                return ctheta;
            }else {
                return r * ract - ctheta;
            }
        }

        virtual D _vanna() const {
            return  ract * pd2 * (1 - d1 * d2 ) / (S * sigma * sigma * sqrt(T));
        }

        virtual D _volga() const {
            return ract * pd2 * (d1 * d1 * d2 - d1 - d2) / (sigma * sigma);
        }

        virtual D _premium() {
            if (this->option_->type() == OptionType::Call) {
                return ract * nd2;
            }else{
                return ract * ( 1 - nd2);
            }
        }

    public:
        AnalyticalDigitalBS(Option<D>* option, BlackScholesProcess<D>* process)
            : PricingEngine<D>(option, process){
        		setParams();
            };

        virtual~AnalyticalDigitalBS() = default;

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


#endif  // ANALYTIC_iDIGITAL_BS_HPP
