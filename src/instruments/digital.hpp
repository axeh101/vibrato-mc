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
        return (this->type_ * (price - this->strike_) > 0.0);
    }

    dual payoff(dual price) const override {
        dual result = (double) OptionType::Call * (price - this->strike_) > 0;
        return result;
    }

};

#endif  // DIGITAL_HPP
