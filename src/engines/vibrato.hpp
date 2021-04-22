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
	bool antithetic = true;
	VibratoBS(Option<D> *option, BlackScholesProcess<D> *process, int n, int M,
			int Mz) :
			PricingEngine<D>(option, process), n(n), M(M), Mz(Mz) {
		T = option->maturity();
		h = T / n;
	}
	virtual ~VibratoBS() = default;

	virtual void calculate() override
	{
		this->premium_ = _premium();
		this->delta_ = _delta();
//		this->gamma_ = _gamma();
		this->vega_ = _vega();
//		this->vanna_ = _vanna();
		this->rho_ = _rho();
	}

private:
	NormalDistribution<D> normal = NormalDistribution<D>(0, 1);
	double T;

	virtual D _premium() {
		D r = this->process_->rate();
		D total = 0.;
		D Z;

		for (int i = 0; i < M; ++i) {
			_moveProcesses(1, 0);
			// Replication step
			double t = this->process_->priceState().time;
			D value = this->process_->priceState().value;
			D sigma = this->process_->vol(t);
			D subtotal = 0.;
			D mun = value * (1 + r * h);
			D sigman = value * sigma * sqrt(h);
			for (int j = 1; j <= Mz; j++) {
				Z = normal();
				subtotal += this->option_->payoff(mun + sigman * Z);
			}
			// End of Replication step
			total += subtotal / Mz;
		}
		return exp(-r * T) * total / M;
	}

	virtual D _delta() {
		D r = this->process_->rate();
		D total = 0.;
		for (int i = 0; i < M; ++i) {
			_moveProcesses(1, 0);
			double t = this->process_->priceState().time;
			D sigma = this->process_->vol(t);
			D value = this->process_->priceState().value;
			D tangent = this->process_->tangentState().value;
			D mun = value * (1 + r * h);
			D sigman = value * sigma * sqrt(h);
			D dmun = tangent * (1 + r * h);
			D dsigman = tangent * sigma * sqrt(h);
			total += _firstOrderVibrato(mun, sigman, dmun, dsigman);
		}
		return exp(-r * T) * total / M;
	}

	virtual D _gamma() {
		return 0;
	}

	virtual D _vega() {
		D r = this->process_->rate();
		D total = 0.;

		for (int i = 0; i < M; ++i) {
			// Replication step
			_moveProcesses(1, 0);
			double t = this->process_->priceState().time;
			D sigma = this->process_->vol(t);
			D value = this->process_->priceState().value;
			D tangent = this->process_->tangentState().value;
			D mun = value * (1 + r * h);
			D sigman = value * sigma * sqrt(h);
			D dmun = tangent * (1 + r * h);
			D dsigman = (tangent * sigma + value) * sqrt(h);
			total += _firstOrderVibrato(mun, sigman, dmun, dsigman);
		}
		return exp(-r * T) * total / M;
	}

	virtual D _vanna() {
		return 0;
	}

	virtual D _rho() {
		D r = this->process_->rate();
		D total = 0.;
		for (int i = 0; i < M; ++i) {
			// Replication step
			_moveProcesses(1, 0);
			double t = this->process_->priceState().time;
			D sigma = this->process_->vol(t);
			D value = this->process_->priceState().value;
			D tangent = this->process_->tangentState().value;
			D mun = value * (1 + r * h);
			D sigman = value * sigma * sqrt(h);
			D dmun = tangent + h * (value + r * tangent);
			D dsigman = tangent * sigma * sqrt(h);
			total += _firstOrderVibrato(mun, sigman, dmun, dsigman);
		}
		return exp(-r * T) * total / M;
	}

	D _firstOrderVibrato(D mun, D sigman, D dmun, D dsigman) {
		D Z;
		D espmu = 0;
		D espsigma = 0;
		if (antithetic) {
			D payoffPlus;
			D payoffMinus;
			D payoffMu;
			for (int j = 1; j <= Mz; j++) {
				Z = normal();
				payoffPlus = this->option_->payoff(mun + sigman * Z);
				payoffMinus = this->option_->payoff(mun - sigman * Z);
				payoffMu = this->option_->payoff(mun);
				espmu += 0.5 * Z * (payoffPlus - payoffMinus) / sigman;
				espsigma += 0.5 * (Z * Z - 1)
						* (payoffPlus - 2 * payoffMu + payoffMinus) / sigman;
			}
		} else {
			D payoff;
			for (int j = 1; j <= Mz; j++) {
				Z = normal();
				payoff = this->option_->payoff(mun + sigman * Z);
				espmu += Z * payoff / sigman;
				espsigma += 0.5 * (Z * Z - 1) * payoff / (sigman * sigman);
			}
		}
		return (dmun * espmu + dsigman * espsigma) / Mz;

	}

	void _moveProcesses(D tan1init = 1, D tan2init = 0) {
		// First step: Compute the deltas up to the n-1
		this->process_->resetState();
		this->process_->initTangentState(tan1init);
		this->process_->initTangent2State(tan2init);

		for (int i = 0; i < n - 1; ++i) {
			D Wh = h * normal();
			this->process_->movePriceEuler(h, Wh);
			this->process_->moveTangentEuler(h, Wh);
			this->process_->moveTangent2Euler(h, Wh);
		}
		// End of first step
	}
};

#endif  // VIBRATO_HPP
