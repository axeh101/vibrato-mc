#ifndef PROCESS_HPP
#define PROCESS_HPP

#include "helper.hpp"
#include "../random/normal.hpp"

template<typename T>
class Process {

public:
	State<T> initialState;

	virtual ~Process() = default;
	Process(State<T> &initialState, std::string name) :
			initialState(initialState), priceState_(initialState), name_(name) {
	}

	virtual T drift() const = 0;
	virtual T diffusion(double t) const = 0;

	virtual T tangentDrift() const = 0;
	virtual T tangentDiffusion(double t) const = 0;

	virtual T tangent2Drift() const = 0;
	virtual T tangent2Diffusion(double t) const = 0;

	virtual T eulerPriceDiff(double h, T Wh) {
		return drift() * h + diffusion(priceState_.time) * sqrt(h) * Wh;
	}

	virtual T eulerTangentDiff(double h, T Wh) {
		return tangentDrift() * h
				+ tangentDiffusion(tangentState_.time) * sqrt(h) * Wh;
	}

	virtual T eulerTangent2Diff(double h, T Wh) {
		return tangent2Drift() * h
				+ tangent2Diffusion(tangentState2_.time) * sqrt(h) * Wh;
	}

	virtual Path<T> eulerDiscretization(int nsamples, double horizon) {
		NormalDistribution<T> N = NormalDistribution<T>(0, 1);
		resetState();
		Path<T> path = Path<T>(nsamples + 1);
		path[0] = initialState;
		double h = horizon / nsamples;
		for (int i = 1; i <= nsamples; ++i) {
			path[i] = movePriceEuler(h, N());
		}
		return path;

	}

	virtual State<T> nextPriceEuler(double h, T Wh) {
		return {
			priceState_.time + h,
			priceState_.value + eulerPriceDiff(h, Wh),
		};
	}

	virtual State<T> movePriceEuler(double h, T Wh) {
		this->priceState_ = this->nextPriceEuler(h, Wh);
		return this->priceState_;
	}

	virtual State<T> nextTangentEuler(double h, T Wh) {
		return {
			tangentState_.time + h,
			tangentState_.value + eulerTangentDiff(h, Wh),
		};
	}

	virtual State<T> nextTangent2Euler(double h, T Wh) {
		return {
			tangentState2_.time + h,
			tangentState2_.value + eulerTangent2Diff(h, Wh),
		};
	}

	virtual State<T> moveTangentEuler(double h, T Wh) {
		tangentState_ = nextTangentEuler(h, Wh);
		return tangentState_;
	}

	virtual State<T> moveTangent2Euler(double h, T Wh) {
		tangentState2_ = nextTangent2Euler(h, Wh);
		return tangentState_;
	}

	virtual void resetState() {
		priceState_ = initialState;
	}

	virtual State<T> priceState() {
		return priceState_;
	}

	virtual State<T> tangentState() {
		return tangentState_;
	}

	virtual State<T> tangent2State() {
		return tangentState2_;
	}

	virtual void initTangentState(T value) {
		tangentState_ = { initialState.time, value };
	}
	virtual void initTangent2State(T value) {
		tangentState2_ = { initialState.time, value };
	}

	friend std::ostream& operator<<(std::ostream &o, const Process<T> &opt) {
		return opt.describe(o);
	}

	virtual std::ostream& describe(std::ostream &o) const {
		return o << "Process name: " << name_ << std::endl << "Initial State: "
				<< initialState << std::endl << "Price State: " << priceState_
				<< std::endl << "Tangent State: " << tangentState_ << std::endl;

	}

	virtual T vol(double t) const =0;
	virtual T rate() const = 0;

protected:
	T rate_;
	T vol_;
	State<T> priceState_;
	State<T> tangentState_;
	State<T> tangentState2_;
	std::string name_;
};

#endif  // PROCESS_HPP
