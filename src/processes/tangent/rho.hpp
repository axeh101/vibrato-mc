#ifndef RHO_HPP
#define RHO_HPP

#include "../process.hpp"

template<typename T>
class RhoTangent: public TangentProcess<T> {

public:
	RhoTangent(State<T> initialState, Process<T> *parent) :
			TangentProcess<T>(initialState, "Rho Tangent process", parent) {
		this->vol_ = parent->vol();
		this->rate_ = parent->rate();
	}

	virtual T drift() const override {
		return this->parent_->diffDriftRho()
				+ this->parent_->diffDriftProcess() * this->priceState_.value;
	}

	virtual T diffusion() const override {
		return this->parent_->diffDiffusionRho()
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

#endif  // RHO_HPP
