#ifndef THETA_HPP
#define THETA_HPP

#include "../process.hpp"

template<typename T>
class ThetaTangent: public TangentProcess<T> {

public:
    ThetaTangent(State<T> initialState, Process<T> *parent) :
            TangentProcess<T>(initialState, "Theta Tangent process", parent) {
        this->vol_ = parent->vol();
        this->rate_ = parent->rate();
    }
    virtual~ThetaTangent() = default;
    virtual T drift() const override {
        return this->parent_->diffDriftX() * this->priceState_.value;
    }

    virtual T diffusion() const override {
        return this->parent_->diffDiffusionX() * this->priceState_.value;
    }

};

#endif  // THETA_HPP
