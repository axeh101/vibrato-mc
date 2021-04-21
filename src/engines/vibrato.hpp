#ifndef VIBRATO_HPP
#define VIBRATO_HPP

#include <cmath>
#include <list>
#include "engine.hpp"
#include "../random/normal.hpp"
#include "../processes/blackscholes.hpp"
#include "../processes/helper.hpp"

template<typename D>
class VibratoBS: public PricingEngine<D> {
public:
	int n;
	int M;
	int Mz;
	double h;

	VibratoBS(Option<D> &option, BlackScholesProcess<D> &process, int n, int M,
			int Mz) :
			PricingEngine<D>(option, process), n(n), M(M), Mz(Mz) {
		T = option.maturity();
		h = T / n;
	}
	virtual ~VibratoBS() = default;

	virtual void calculate() override
	{
		this->delta_ = _delta();

	}

	virtual D _delta() {
		D r = this->process_->rate();
		D total = 0.;

		for (int i = 0; i < M; ++i) {
			// First step: Compute the deltas up to the n-1
			this->process_->resetState();
			this->process_->initTangentState(1);
			for (int i = 0; i < n - 1; ++i) {
				D Wh = h * normal();

				this->process_->movePriceEuler(h, Wh);
				this->process_->moveTangentEuler(h, Wh);
			}
			// End of first step

			// Replication step
			double t = this->process_->priceState().time;
			D sigma = this->process_->vol(t);
			D value = this->process_->priceState().value;
			D tangent = this->process_->tangentState().value;
			D fstHalf=0;
			D scdHalf=0;
			D rh = h * r;
			D sigmah = sigma * sqrt(h);
			D subtotal = 0.;
			D mun = value * (1 + rh);
			D sigman = value * sigmah ;
			D dmun =  tangent * (1 + rh);
			D dsigman =  2 * tangent * sigmah * value * sigmah;
			for (int j = 1; j <= Mz; j++) {
				D Z = normal();
				D V = this->option_.payoff(mun + sigman * sqrt(h) * Z );
				fstHalf += V * Z / sigman;
				scdHalf += V * (Z * Z - 1) / (sigman * sigman * value * value);
			}
			subtotal = dmun * fstHalf / sqrt(h) + dsigman * scdHalf / (2*h);
			// End of Replication step
			total += subtotal / Mz;
		}
		return exp(-this->process_->rate() * T) * total / n;

	}

private:
	NormalDistribution<D> normal = NormalDistribution<D>(0, 1);
	double T;
//
//	void genGamma() {
//		this->process_.resetState();
//		this->process_->initTangentState(1);
//		this->process_->initTangentState(2);
//		for (int i = 0; i < n - 1; ++i) {
//			D Wh = h * normal();
//			this->process_->movePriceEuler(h, Wh);
//			this->process_->moveTangentEuler(h, Wh);
//			this->process_->moveTangent2Euler(h, Wh);
//		}
//	}
//
//	void genVega(double t) {
//		this->process_->resetState();
//		this->process_->initTangentState(0);
//		for (int i = 0; i < n - 1; ++i) {
//			D Wh = h * normal();
//			tangentState = this->process_->moveTangentEuler(h, Wh);
//			tangentState.value += priceState.value * Wh;
//			priceState = this->process_->movePriceEuler(h, Wh);
//		}
//	}
//
//	void genVanna(double t) {
//		this->process_.resetState();
//		State<D> priceState = this->process_->initialState;
//		State<D> tangentState = { priceState.time, 1 };
//		State<D> tangentState2 = { priceState.time, 0.0001 };
//		double time = priceState.time;
//		for (int i = 0; i < n - 1; ++i) {
//			D Wh = h * normal();
//			tangentState2 = this->process_->moveTangent2Euler(h, Wh);
//			tangentState2.value += 2 * tangentState.value * Wh;
//			tangentState = this->process_.moveTangentEuler(h, Wh);
//			tangentState2.value += priceState.value * Wh;
//			priceState = this->process_.movePriceEuler(h, Wh);
//		}
//	}

};

#endif  // VIBRATO_HPP
