#ifndef BLACKSCHOLES_HPP
#define BLACKSCHOLES_HPP

#include "process.hpp"
#include "../random/normal.hpp"

template<typename T>
class BlackScholesProcess: public Process<T> {

public:
	BlackScholesProcess(State<T> initialState, T rate, T vol) :
			Process<T>(initialState, "Black-Scholes"), rate_(rate), vol_(vol) {
	}

	virtual T drift() const {
		return rate_ * this->priceState_.value;
	}

	virtual T diffusion() const {
		return vol_ * this->priceState_.value;
	}

	virtual T tangentDrift() const {
		return rate_ * this->tangentState_.value;
	}

	virtual T tangentDiffusion() const {
		return vol_ * this->tangentState_.value;
	}

	virtual T tangent2Drift() const {
		return rate_ * this->tangentState2_.value;
	}

	virtual T tangent2Diffusion() const {
		return vol_ * this->tangentState2_.value;
	}

	virtual State<T> nextIto(double h) {
		return {
			this->priceState_.time + h,
			ito(this->priceState_.value, h)
		};
	}

	virtual T vol(double t) const {
		// Constant volatility we keep the parameter
		//for homogeneity with stochastic volatility models
		return vol_;
	}

	virtual T rate() const {
		return rate_;
	}

	virtual std::ostream& describe(std::ostream &o) const {
		return Process<T>::describe(o) << "Rate: " << rate_ << std::endl
				<< "Volatility: " << vol_ << std::endl;
	}

	virtual State<T> moveIto(double t) {
		this->priceState_ = nextIto(t);
		return this->priceState_;
	}

	virtual Path<T> generatePath(int nsamples, double horizon) {
		this->resetState();
		Path<T> path = Path<T>(nsamples + 1);
		path[0] = this->initialState_;
		double h = horizon / nsamples;
		for (int i = 1; i <= nsamples; i++) {
			path[i] = this->moveIto(h);
		}
		return path;
	}

	virtual T eulerPriceDiff(double h) {
		return Process<T>::eulerPriceDiff(h, normal());
	}

private:
	T rate_;
	T vol_;
	NormalDistribution<T> normal = NormalDistribution<T>(0,1);

	T ito(T value, double h) {
		T vol2 = vol_ * vol_;
		T dWt = sqrt(h) * normal();
		T newValue = value * exp((rate_ - 0.5 * vol2) * h + vol_ * dWt);
		return newValue;
	}

};

#endif  // BLACKSCHOLES_HPP
