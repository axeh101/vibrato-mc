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
        T value = this->dtype() * (price - this->strike_);
        return value > 0 ? value : 0;
    };

};

#endif  // VANILLAOPTION_HPP
