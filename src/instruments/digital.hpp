#ifndef DIGITAL_HPP
#define DIGITAL_HPP
#include "vanilla.hpp"


template <typename T=double>
class DigitalOption: public VanillaOption<T> {

    public:
        virtual~DigitalOption() = default;
        DigitalOption(double maturity, T strike, const OptionType& type):VanillaOption<T>(maturity, strike, type){};

        T payoff(T price) const override
        {
            return Option<T>::type_ * (price - VanillaOption<T>::strike_) > 0.0;
        }
};

#endif  // DIGITAL_HPP
