#ifndef VOLGA_HPP
#define VOLGA_HPP

#include <typeinfo>

#include "../process.hpp"

template<typename T>
class VolgaTangent : public TangentProcess<T> {

private:
    TangentProcess<T> *tangent_;

public:
    VolgaTangent(State<T> initialState, TangentProcess<T> *tangent) :
            TangentProcess<T>(initialState, "Volga Tangent process", tangent->parent_),
            tangent_(tangent) {}

    virtual~VolgaTangent() = default;

    virtual T drift() const override {
        return (
                this->parent_->diffDriftX() * this->priceState_.value +
                this->parent_->diffDriftX2() * tangent_->priceState().value *
                tangent_->priceState().value
        );
    }

    virtual T diffusion() const override {
        return (
                this->parent_->diffDiffusionSigma2() * tangent_->priceState().value +
                this->parent_->diffDiffusionX2() * tangent_->priceState().value *
                tangent_->priceState().value +
                this->parent_->diffDiffusionX() * this->priceState_.value
        );
    }

};


#endif  // VOLGA_HPP
