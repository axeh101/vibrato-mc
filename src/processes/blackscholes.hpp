#ifndef BLACKSCHOLES_HPP
#define BLACKSCHOLES_HPP

#include "process.hpp"
#include "../random/normal.hpp"

template<typename T>
class BlackScholesProcess: public Process<T> {

public:
	BlackScholesProcess(State<T> initialState, T rate, T vol) :
			Process<T>(initialState, "Black-Scholes") {
		this->vol_ = vol;
		this->rate_ = rate;
	}

	virtual T drift() const {
		return this->rate_ * this->priceState_.value;
	}

	virtual T diffusion() const {
		return this->vol_ * this->priceState_.value;
	}

	virtual T tangentDrift() const {
		return this->rate_ * this->tangentState_.value;
	}

	virtual T tangentDiffusion() const {
		return this->vol_ * this->tangentState_.value;
	}

	virtual T tangent2Drift() const {
		return this->rate_ * this->tangentState2_.value;
	}

	virtual T tangent2Diffusion() const {
		return this->vol_ * this->tangentState2_.value;
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
		return this->vol_;
	}

	virtual T rate() const {
		return this->rate_;
	}

	virtual std::ostream& describe(std::ostream &o) const {
		return Process<T>::describe(o) << "Rate: " << this->rate_ << std::endl
				<< "Volatility: " << this->vol_ << std::endl;
	}

	virtual State<T> moveIto(double t) {
		this->priceState_ = nextIto(t);
		return this->priceState_;
	}

	virtual Path<T> generatePath(int nsamples, double horizon) {
		this->resetState();
		Path<T> path = Path<T>(nsamples + 1);
		path[0] = this->initialState;
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

	NormalDistribution<T> normal = NormalDistribution<T>(0,1);

	T ito(T value, double h) {
		T vol2 = this->vol_ * this->vol_;
		T dWt = sqrt(h) * normal();
		T newValue = value * exp((this->rate_ - 0.5 * vol2) * h + this->vol_ * dWt);
		return newValue;
	}

};

#endif  // BLACKSCHOLES_HPP
