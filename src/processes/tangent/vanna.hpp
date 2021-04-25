#ifndef VANNA_HPP
#define VANNA_HPP

#include <typeinfo>

#include "../process.hpp"

// TODO: Check the gamma tangent and correct the generation process

template<typename T>
class VannaTangent : public TangentProcess<T> {

private:
    TangentProcess<T> *tangent1_;
    TangentProcess<T> *tangent2_;
public:
    VannaTangent(State<T> initialState, TangentProcess<T> *tangent1, TangentProcess<T> *tangent2) :
            TangentProcess<T>(initialState, "Vanna Tangent process", tangent1->parent_),
            tangent1_(tangent1), tangent2_(tangent2) {}

    virtual~VannaTangent() = default;

    virtual T drift() const override {
        return (
                this->parent_->diffDriftX() * this->priceState_.value +
                this->parent_->diffDriftX2() * tangent1_->priceState().value *
                tangent2_->priceState().value
        );
    }

    virtual T diffusion() const override {
        return (
                this->parent_->diffDiffusionSigmaX() * tangent1_->priceState().value +
                this->parent_->diffDiffusionX2() * tangent1_->priceState().value *
                tangent2_->priceState().value +
                this->parent_->diffDiffusionX() * this->priceState_.value
        );
    }

};


#endif  // VANNA_HPP
