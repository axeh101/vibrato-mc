#ifndef DIGITAL_HPP
#define DIGITAL_HPP

#include "vanilla.hpp"


template<typename T=double>
class DigitalOption : public Option<T> {

public:
    virtual~DigitalOption() = default;

    DigitalOption(double maturity, T strike, const OptionType &type) :
            Option<T>(maturity, strike, type, "Digital Option") {};

    T payoff(T price) const override {
        return (this->type_ * (price - this->strike_) > 0.0);
    }
};

#endif  // DIGITAL_HPP
