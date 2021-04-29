#ifndef VANILLAOPTION_HPP
#define VANILLAOPTION_HPP

#include <cmath>
#include "option.hpp"

template<typename T=double>
class VanillaOption : public Option<T> {
public:
    virtual~VanillaOption() = default;

    VanillaOption(T &maturity, T &strike, const OptionType &type) :
            Option<T>(maturity, strike, type, "Vanilla Option") {};

    T payoff(T price) const override {
        return max(this->type_ * (price - this->strike_), 0.);
    };

    dual payoff(dual price) const override {
        dual value = (dual) this->type_ * (price - this->strike_);
        return (value > (dual) 0.) ? value : (dual) 0.;
    }
};

#endif  // VANILLAOPTION_HPP
