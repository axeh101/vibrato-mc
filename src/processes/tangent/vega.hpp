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
				+ this->parent_->diffDriftProcess() * this->priceState_.value;
	}

	virtual T diffusion() const override {
		return this->parent_->diffDiffusionVega()
				+ this->parent_->diffDiffusionProcess()
						* this->priceState_.value;
	}

	virtual T vol() const override {
		return this->parent_->vol();
	}

	virtual T rate() const override {
		return this->parent_->rate();
	}

};

#endif  // VEGA_HPP
