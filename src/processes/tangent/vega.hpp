#ifndef VEGA_HPP
#define VEGA_HPP

#include "../process.hpp"

template<typename T>
class VegaTangent: public TangentProcess<T> {

public:
	VegaTangent(State<T> initialState, Process<T> *parent) :
			TangentProcess<T>(initialState, "Vega Tangent process", parent) {
		this->vol_ = parent->vol();
		this->rate_ = parent->rate();
	}

	virtual T drift() const override {
		return this->parent_->diffDriftVega()
				+ this->parent_->diffDriftX() * this->priceState_.value;
	}

	virtual T diffusion() const override {
		return this->parent_->diffDiffusionVega()
				+ this->parent_->diffDiffusionX() * this->priceState_.value;
	}

};

#endif  // VEGA_HPP
