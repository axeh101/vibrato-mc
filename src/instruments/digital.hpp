#ifndef DIGITAL_HPP
#define DIGITAL_HPP

#include "option.hpp"


template<typename T=double>
class DigitalOption : public Option<T> {

public:
    virtual~DigitalOption() = default;

    DigitalOption(T &maturity, T &strike, const OptionType &type) :
            Option<T>(maturity, strike, type, "Digital Option") {};

    T payoff(T price) const override {
        T value = this->dtype() * (price - this->strike_);
        return value > 0.0;
    }


};

#endif  // DIGITAL_HPP
