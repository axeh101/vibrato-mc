#ifndef DELTA_HPP
#define DELTA_HPP

#include "../process.hpp"

template<typename T>
class DeltaTangent: public TangentProcess<T> {

public:
	DeltaTangent(State<T> initialState, Process<T> *parent) :
			TangentProcess<T>(initialState, "Delta Tangent process", parent) {
		this->vol_ = parent->vol();
		this->rate_ = parent->rate();
	}

	virtual T drift() const override {
		return this->parent_->diffDriftX() * this->priceState_.value;
	}

	virtual T diffusion() const override {
		return this->parent_->diffDiffusionX() * this->priceState_.value;
	}
};

#endif  // DELTA_HPP
