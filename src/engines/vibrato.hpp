#ifndef VIBRATO_HPP
#define VIBRATO_HPP

#include <cmath>
#include <list>
#include "engine.hpp"
#include "../random/normal.hpp"
#include "../processes/blackscholes.hpp"
#include "../processes/helper.hpp"
#include "../processes/tangent/delta.hpp"
#include "../processes/tangent/gamma.hpp"
#include "../processes/tangent/vanna.hpp"
#include "../processes/tangent/rho.hpp"
#include "../processes/tangent/vega.hpp"

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
	DeltaTangent<D> deltaTangentProcess = DeltaTangent<D>(
			{ this->process_->initialState.time, 1 }, this->process_);
	VegaTangent<D> vegaTangentProcess = VegaTangent<D>(
			{ this->process_->initialState.time, 0 }, this->process_);
	RhoTangent<D> rhoTangentProcess = RhoTangent<D>(
			{ this->process_->initialState.time, 0 }, this->process_);
	NormalDistribution<D> normal = NormalDistribution<D>(0, 1);
	double T;

	virtual D _premium() {
		D r = this->process_->rate();
		D total = 0.;
		D Z;

		for (int i = 0; i < M; ++i) {
			_moveProcess(*this->process_);
			// Replication step
			D value = this->process_->priceState().value;
			D sigma = this->process_->vol();
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
		D total = 0.;
		for (int i = 0; i < M; ++i) {
			_moveProcess(*this->process_);
			_moveProcess(deltaTangentProcess);
			total += _firstOrderVibrato(this->deltaTangentProcess.mun(h),
					this->deltaTangentProcess.dmun(h),
					this->deltaTangentProcess.sigman(h),
					this->deltaTangentProcess.dsigman(h));
		}
		return exp(-this->process_->rate() * T) * total / M;
	}

	virtual D _vega() {
		D total = 0.;
		for (int i = 0; i < M; ++i) {
			_moveProcess(*this->process_);
			_moveProcess(vegaTangentProcess);
			total += _firstOrderVibrato(this->vegaTangentProcess.mun(h),
					this->vegaTangentProcess.dmun(h),
					this->vegaTangentProcess.sigman(h),
					this->vegaTangentProcess.dsigman(h));
		}
		return exp(-this->process_->rate() * T) * total / M;
	}

	virtual D _rho() {
		D total = 0.;
		for (int i = 0; i < M; ++i) {
			_moveProcess(*this->process_);
			_moveProcess(rhoTangentProcess);
			total += _firstOrderVibrato(this->rhoTangentProcess.mun(h),
					this->rhoTangentProcess.dmun(h),
					this->rhoTangentProcess.sigman(h),
					this->rhoTangentProcess.dsigman(h));
		}
		return exp(-this->process_->rate() * T) * total / M;
	}
	virtual D _vanna() {
		return 0;
	}

	virtual D _gamma() {
		return 0;
	}

	D _firstOrderVibrato(D mun, D dmun, D sigman, D dsigman) {
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

	void _moveProcess(Process<D> &process) {
		// First step: Compute the deltas up to the n-1
		process.resetState();
		for (int i = 0; i < n - 1; ++i) {
			D Wh = h * normal();
			process.movePriceEuler(h, Wh);
		}
		// End of first step
	}
};

#endif  // VIBRATO_HPP
