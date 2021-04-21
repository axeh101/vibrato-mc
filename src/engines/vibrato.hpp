#ifndef VIBRATO_HPP
#define VIBRATO_HPP


#include <cmath>
#include <list>
#include "engine.hpp"
#include "../random/normal.hpp"
#include "../processes/blackscholes.hpp"
#include "../processes/helper.hpp"

template <typename D>
class VibratoBS: public PricingEngine<D, BlackScholesProcess<D>>
{
    public:
		int n;
		int M;
		int Mz;
        double h;

        VibratoBS(Option<D>& option, BlackScholesProcess<D>& process, int n, int M, int Mz)
            : PricingEngine<D, BlackScholesProcess<D>>(option, process), n(n), M(M), Mz(Mz){
                T = option.maturity();
                h = T/n;
            };
             virtual~VibratoBS() = default;



        virtual D delta() const {
        	D r = this->process_.rate();
        	D total = 0.;
			D subtotal =0.;
        	for (int i = 0; i < M; ++i) {
        		genDelta();
        		subtotal = 0.;
        		double t = this->process_.priceState.time;
            	D sigma = this->process_.vol(t);
        		D value = this->process_.priceState.value;
        		D tangent = this->process_.tangentState.value;

				for(int i = 1; i <= Mz; i++){
					D Z = normal();
					D rh= h * r;
					D sigmah =  sigma * sqrt(h);
					D V = this->option_.payoff(value * (1 + rh) + value * sigmah * Z);
					subtotal += tangent * ((1 + rh) * V + V * (Z*Z - 1)) / sigmah;
				}
				total += subtotal / Mz;
        	}
			return exp(-this->process_.rate() * T) * total / n;


        }



    private:
        NormalDistribution<D> normal = NormalDistribution<D>(0,1);
        double T;

        void genDelta() const {
        	this->process_.resetState();
            State<D> priceState =this->process_.initialState();
            State<D> tangentState = {priceState.time, 1};
            for (int i = 0; i < n-1; ++i) {
            	D Wh = h * normal();
            	priceState = this->process_.movePriceEuler(h, Wh);
            	tangentState = this->process_.moveTangentEuler(h, Wh);
            }
        };



};



#endif  // VIBRATO_HPP
