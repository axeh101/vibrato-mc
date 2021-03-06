#ifndef RHO_HPP
#define RHO_HPP

#include "../process.hpp"

template<typename T>
class RhoTangent : public TangentProcess<T> {

public:
    RhoTangent(State<T> initialState, Process<T> *parent) :
            TangentProcess<T>(initialState, "Rho Tangent process", parent) {
        this->vol_ = parent->vol();
        this->rate_ = parent->rate();
    }

    virtual~RhoTangent() = default;

    virtual T drift() const override {
        return this->parent_->diffDriftRho()
               + this->parent_->diffDriftX() * this->priceState_.value;
    }

    virtual T diffusion() const override {
        return this->parent_->diffDiffusionRho()
               + this->parent_->diffDiffusionX() * this->priceState_.value;
    }

};

#endif  // RHO_HPP
