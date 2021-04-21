#ifndef PROCESS_HPP
#define PROCESS_HPP

#include "helper.hpp"
#include "../random/normal.hpp"

template<typename T>
class Process {
protected:
	State<T> initialState_;
	State<T> priceState_;
	State<T> tangentState_;
	State<T> tangentState2_;
	std::string name_;

public:
	virtual ~Process() = default;
	Process(State<T> &initialState, std::string name) :
			initialState_(initialState), priceState_(initialState), name_(name) {};

	virtual T drift() const = 0;
	virtual T diffusion() const = 0;

	virtual T tangentDrift() const = 0;
	virtual T tangentDiffusion() const = 0;

	virtual T tangent2Drift() const = 0;
	virtual T tangent2Diffusion() const = 0;

	virtual T eulerPriceDiff(double h, T Wh) {
		return drift() * h + diffusion() * sqrt(h) * Wh;
	}

	virtual T eulerTangentDiff(double h, T Wh) {
		return tangentDrift() * h + tangentDiffusion() * sqrt(h) * Wh;
	}

	virtual T eulerTangent2Diff(double h, T Wh) {
		return tangent2Drift() * h + tangent2Diffusion() * sqrt(h) * Wh;
	}

	virtual Path<T> eulerDiscretization(int nsamples, double horizon) {
		NormalDistribution<T> N = NormalDistribution<T>(0, 1);
		resetState();
		Path<T> path = Path<T>(nsamples + 1);
		path[0] = this->initialState_;
		double h = horizon / nsamples;
		for (int i = 1; i <= nsamples; ++i) {
			path[i] = movePriceEuler(h, N());
		}
		return path;

	}

	virtual State<T> nextPriceEuler(double h, T Wh) {
		return {
			this->priceState_.time + h,
			this->priceState_.value + eulerPriceDiff(h, Wh),
		};
	}

	virtual State<T> movePriceEuler(double h, T Wh) {
		priceState_ = nextPriceEuler(h, Wh);
		return priceState_;
	}

	virtual State<T> nextTangentEuler(double h, T Wh) {
		return {
			this->tangentState_.time + h,
			this->tangentState_.value + eulerTangentDiff(h, Wh),
		};
	}

	virtual State<T> nextTangent2Euler(double h, T Wh) {
		return {
			this->tangentState2_.time + h,
			this->tangentState2_.value + eulerTangent2Diff(h, Wh),
		};
	}

	virtual State<T> moveTangentEuler(double h, T Wh) {
		priceState_ = nextTangentEuler(h, Wh);
		return priceState_;
	}


	virtual void resetState() {
		priceState_ = initialState_;
	}



	virtual State<T> initialState() {
		return initialState_;
	}

	virtual State<T> priceState() {
		return priceState_;
	}

	virtual State<T> tangentState() {
		return tangentState_;
	}

	virtual void initTangentState(T value) { tangentState_={initialState_.time, value};}
	virtual void initTangent2State(T value) { tangentState2_={initialState_.time, value};}

	friend std::ostream& operator<<(std::ostream &o, const Process<T> &opt) {
		return opt.describe(o);
	}

	virtual std::ostream& describe(std::ostream &o) const {
		return o << "Process name: " << name_ << std::endl << "Initial State: "
				<< initialState_ << std::endl << "Price State: " << priceState_
				<< std::endl << "Tangent State: " << tangentState_ << std::endl;

	}
};

#endif  // PROCESS_HPP
